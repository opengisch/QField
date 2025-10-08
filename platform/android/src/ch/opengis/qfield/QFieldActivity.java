/**
 * QFieldActivity.java - class needed to copy files from assets to
 * getExternalFilesDir() before starting QtActivity this can be used to perform
 * actions before QtActivity takes over.
 * @author  Marco Bernasocchi - <marco@opengis.ch>
 */
/*
 Copyright (c) 2011, Marco Bernasocchi <marco@opengis.ch>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the  Marco Bernasocchi <marco@opengis.ch> nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY Marco Bernasocchi <marco@opengis.ch> ''AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Marco Bernasocchi <marco@opengis.ch> BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package ch.opengis.qfield;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Application;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.ActivityNotFoundException;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Insets;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.provider.MediaStore;
import android.provider.Settings;
import android.text.Html;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.DisplayCutout;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;
import androidx.documentfile.provider.DocumentFile;
import ch.opengis.qfield.QFieldUtils;
import ch.opengis.qfield.R;
import io.sentry.android.core.SentryAndroid;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.Thread;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.qtproject.qt.android.bindings.QtActivity;

public class QFieldActivity extends QtActivity {

    private static final int IMPORT_DATASET = 300;
    private static final int IMPORT_PROJECT_FOLDER = 301;
    private static final int IMPORT_PROJECT_ARCHIVE = 302;

    private static final int UPDATE_PROJECT_FROM_ARCHIVE = 400;

    private static final int EXPORT_TO_FOLDER = 500;

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor sharedPreferenceEditor;
    private ProgressDialog progressDialog;
    ExecutorService executorService = Executors.newFixedThreadPool(4);

    public static native void openProject(String url);
    public static native void openPath(String path);
    public static native void executeAction(String action);

    public static native void volumeKeyDown(int volumeKeyCode);
    public static native void volumeKeyUp(int volumeKeyCode);

    public static native void resourceReceived(String path);
    public static native void resourceOpened(String path);
    public static native void resourceCanceled(String message);

    private Intent projectIntent;
    private Intent qfieldIntent;

    private float originalBrightness;
    private boolean handleVolumeKeys = false;
    private String pathsToExport;
    private String projectPath;

    private static final int CAMERA_RESOURCE = 600;
    private static final int GALLERY_RESOURCE = 601;
    private static final int FILE_PICKER_RESOURCE = 602;
    private static final int OPEN_RESOURCE = 603;
    private String resourcePrefix;
    private String resourceFilePath;
    private String resourceSuffix;
    private String resourceTempFilePath;
    private File resourceFile;
    private File resourceCacheFile;
    private boolean resourceIsEditing;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        prepareQtActivity();
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onNewIntent(Intent intent) {
        // Prevent activity restart
        intent.setFlags(intent.getFlags() &
                        ~(Intent.FLAG_ACTIVITY_NEW_TASK |
                          Intent.FLAG_ACTIVITY_NEW_DOCUMENT));
        super.onNewIntent(intent);

        if (intent.getAction() == Intent.ACTION_VIEW ||
            intent.getAction() == Intent.ACTION_SEND) {
            String scheme = intent.getScheme();
            if (scheme != null && scheme.equals("qfield")) {
                qfieldIntent = intent;
                processQFieldIntent();
            } else if (scheme != null && scheme.equals("https")) {
                Uri uri = intent.getData();
                String host = uri.getHost();
                if (host.equals("qfield.org")) {
                    qfieldIntent = intent;
                    processQFieldIntent();
                }
            } else {
                projectIntent = intent;
                processProjectIntent();
            }
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (handleVolumeKeys && (keyCode == KeyEvent.KEYCODE_VOLUME_UP ||
                                 keyCode == KeyEvent.KEYCODE_VOLUME_DOWN ||
                                 keyCode == KeyEvent.KEYCODE_MUTE)) {
            // Forward volume keys' presses to QField
            volumeKeyDown(keyCode);
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (handleVolumeKeys && (keyCode == KeyEvent.KEYCODE_VOLUME_UP ||
                                 keyCode == KeyEvent.KEYCODE_VOLUME_DOWN ||
                                 keyCode == KeyEvent.KEYCODE_MUTE)) {
            // Forward volume keys's releases to QField
            volumeKeyUp(keyCode);
            return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    private boolean isDarkTheme() {
        switch (getResources().getConfiguration().uiMode &
                Configuration.UI_MODE_NIGHT_MASK) {
            case Configuration.UI_MODE_NIGHT_YES:
                return true;

            case Configuration.UI_MODE_NIGHT_NO:
                return false;
        }
        return false;
    }

    private void vibrate(int milliseconds) {
        Vibrator v = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);
        v.vibrate(VibrationEffect.createOneShot(
            milliseconds, VibrationEffect.DEFAULT_AMPLITUDE));
    }

    private void processQFieldIntent() {
        String data = qfieldIntent.getDataString();
        executeAction(data);
    }

    private void processProjectIntent() {
        showBlockingProgressDialog(getString(R.string.processing_message));

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                String scheme = projectIntent.getScheme();
                String action = projectIntent.getAction();
                String type = projectIntent.getType();
                Context context = getApplication().getApplicationContext();

                Uri uri = null;
                if (action.compareTo(Intent.ACTION_SEND) == 0) {
                    uri = (Uri)projectIntent.getParcelableExtra(
                        Intent.EXTRA_STREAM);
                    scheme = "";
                } else {
                    uri = projectIntent.getData();
                }

                String filePath = QFieldUtils.getPathFromUri(context, uri);
                String importDatasetPath = "";
                String importProjectPath = "";
                File externalFilesDir = getExternalFilesDir(null);
                if (externalFilesDir != null) {
                    importDatasetPath = externalFilesDir.getAbsolutePath() +
                                        "/Imported Datasets/";
                    new File(importDatasetPath).mkdir();
                    importProjectPath = externalFilesDir.getAbsolutePath() +
                                        "/Imported Projects/";
                    new File(importProjectPath).mkdir();
                }

                if ((scheme.compareTo(ContentResolver.SCHEME_CONTENT) == 0 ||
                     action.compareTo(Intent.ACTION_SEND) == 0) &&
                    importDatasetPath != "") {
                    DocumentFile documentFile =
                        DocumentFile.fromSingleUri(context, uri);
                    String fileName = documentFile.getName();
                    long fileBytes = documentFile.length();
                    if (fileName == null) {
                        if (type != null) {
                            // File name not provided
                            fileName =
                                new SimpleDateFormat("yyyyMMdd_HHmmss")
                                    .format(new Date().getTime()) +
                                "." +
                                QFieldUtils.getExtensionFromMimeType(type);
                        }
                    }

                    if (fileName != null) {
                        String fileBaseName = fileName;
                        String fileExtension = "";
                        if (fileName.lastIndexOf(".") > -1) {
                            fileBaseName = fileName.substring(
                                0, fileName.lastIndexOf("."));
                            fileExtension =
                                fileName.substring(fileName.lastIndexOf("."));
                        }

                        ContentResolver resolver = getContentResolver();
                        if (type != null && type.equals("application/zip")) {
                            String projectName = "";
                            try {
                                InputStream input =
                                    resolver.openInputStream(uri);
                                projectName =
                                    QFieldUtils.getArchiveProjectName(input);
                            } catch (Exception e) {
                                e.printStackTrace();
                            }

                            if (projectName != "") {
                                String projectPath =
                                    importProjectPath + fileBaseName + "/";
                                int i = 1;
                                while (new File(projectPath).exists()) {
                                    projectPath = importProjectPath +
                                                  fileBaseName + "_" + i + "/";
                                    i++;
                                }
                                new File(projectPath).mkdir();
                                try {
                                    InputStream input =
                                        resolver.openInputStream(uri);
                                    QFieldUtils.zipToFolder(input, projectPath);
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                                Log.v("QField",
                                      "Opening decompressed project: " +
                                          projectPath + projectName);
                                dismissBlockingProgressDialog();
                                openProject(projectPath + projectName);
                                return;
                            }
                        }

                        Boolean canWrite = filePath != ""
                                               ? new File(filePath).canWrite()
                                               : false;
                        if (!canWrite) {
                            Log.v("QField",
                                  "Content intent detected: " + action + " : " +
                                      projectIntent.getDataString() + " : " +
                                      type + " : " + fileName);
                            String importFilePath =
                                importDatasetPath + fileName;
                            int i = 1;
                            while (new File(importFilePath).exists()) {
                                importFilePath = importDatasetPath +
                                                 fileBaseName + "_" + i +
                                                 fileExtension;
                                i++;
                            }
                            Log.v("QField",
                                  "Importing document to file path: " +
                                      importFilePath);
                            try {
                                InputStream input =
                                    resolver.openInputStream(uri);
                                QFieldUtils.inputStreamToFile(
                                    input, importFilePath, fileBytes);
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                            dismissBlockingProgressDialog();
                            openProject(importFilePath);
                            return;
                        }
                    }
                }

                Log.v("QField", "Opening document file path: " + filePath);
                dismissBlockingProgressDialog();
                openProject(filePath);
            }
        });
    }

    private void dimBrightness() {
        WindowManager.LayoutParams lp = getWindow().getAttributes();
        originalBrightness = lp.screenBrightness;
        lp.screenBrightness = 0.01f;
        getWindow().setAttributes(lp);
    }

    private void restoreBrightness() {
        WindowManager.LayoutParams lp = getWindow().getAttributes();
        lp.screenBrightness = originalBrightness;
        getWindow().setAttributes(lp);
    }

    private void takeVolumeKeys() {
        handleVolumeKeys = true;
    }

    private void releaseVolumeKeys() {
        handleVolumeKeys = false;
    }

    private void showBlockingProgressDialog(String message) {
        progressDialog = new ProgressDialog(this, R.style.DialogTheme);
        progressDialog.setMessage(message);
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.show();
    }

    private void dismissBlockingProgressDialog() {
        if (progressDialog != null) {
            progressDialog.dismiss();
            progressDialog = null;
        }
    }

    private void displayAlertDialog(String title, String message) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                AlertDialog alertDialog =
                    new AlertDialog
                        .Builder(QFieldActivity.this, R.style.DialogTheme)
                        .create();
                alertDialog.setTitle(title);
                alertDialog.setMessage(message);
                alertDialog.show();
            }
        });
    }

    private void initiateSentry() {
        Context context = getApplication().getApplicationContext();

        try {
            ApplicationInfo app =
                context.getPackageManager().getApplicationInfo(
                    context.getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = app.metaData;
            SentryAndroid.init(this, options -> {
                options.setDsn(bundle.getString("io.sentry.dsn"));
                options.setEnvironment(
                    bundle.getString("io.sentry.environment"));
            });
        } catch (NameNotFoundException e) {
            return;
        }
    }

    private void prepareQtActivity() {
        sharedPreferences =
            getSharedPreferences("QField", Context.MODE_PRIVATE);
        sharedPreferenceEditor = sharedPreferences.edit();

        checkAllFileAccess(); // Storage access permission handling for Android
                              // 11+

        List<String> dataDirs = new ArrayList<String>();

        File primaryExternalFilesDir = getExternalFilesDir(null);
        if (primaryExternalFilesDir != null) {
            String dataDir = primaryExternalFilesDir.getAbsolutePath() + "/";
            // create import and creation directories
            new File(dataDir + "Imported Datasets/").mkdir();
            new File(dataDir + "Imported Projects/").mkdir();
            new File(dataDir + "Created Projects/").mkdir();

            dataDir = dataDir + "QField/";
            // create QField directories
            new File(dataDir).mkdir();
            new File(dataDir + "basemaps/").mkdir();
            new File(dataDir + "fonts/").mkdir();
            new File(dataDir + "proj/").mkdir();
            new File(dataDir + "auth/").mkdir();
            new File(dataDir + "logs/").mkdirs();
            new File(dataDir + "plugins/").mkdirs();

            dataDirs.add(dataDir);
        }

        String storagePath =
            Environment.getExternalStorageDirectory().getAbsolutePath();
        String rootDataDir = storagePath + "/QField/";
        File storageFile = new File(rootDataDir);
        storageFile.mkdir();
        if (storageFile.canWrite()) {
            // create directories
            new File(rootDataDir + "basemaps/").mkdir();
            new File(rootDataDir + "fonts/").mkdir();
            new File(rootDataDir + "proj/").mkdir();
            new File(rootDataDir + "auth/").mkdir();
            new File(rootDataDir + "logs/").mkdirs();

            dataDirs.add(rootDataDir);
        }

        File[] externalFilesDirs = getExternalFilesDirs(null);
        for (File file : externalFilesDirs) {
            if (file != null) {
                // Don't duplicate primary external files directory
                if (file.getAbsolutePath().equals(
                        primaryExternalFilesDir.getAbsolutePath())) {
                    continue;
                }

                // create QField directories
                String dataDir = file.getAbsolutePath() + "/QField/";
                new File(dataDir + "basemaps/").mkdirs();
                new File(dataDir + "fonts/").mkdirs();
                new File(dataDir + "proj/").mkdirs();
                new File(dataDir + "auth/").mkdirs();
                new File(dataDir + "logs/").mkdirs();

                dataDirs.add(dataDir);
            }
        }

        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setClass(QFieldActivity.this, QtActivity.class);
        // Prevent activity restart
        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP |
                        Intent.FLAG_ACTIVITY_SINGLE_TOP);
        try {
            ActivityInfo activityInfo = getPackageManager().getActivityInfo(
                getComponentName(), PackageManager.GET_META_DATA);
            intent.putExtra("GIT_REV", activityInfo.metaData.getString(
                                           "android.app.git_rev"));
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            finish();
            return;
        }

        StringBuilder appDataDirs = new StringBuilder();
        for (String dataDir : dataDirs) {
            appDataDirs.append(dataDir);
            appDataDirs.append("--;--");
        }
        intent.putExtra("QFIELD_APP_DATA_DIRS", appDataDirs.toString());

        Intent sourceIntent = getIntent();
        if (sourceIntent.getAction() == Intent.ACTION_VIEW ||
            sourceIntent.getAction() == Intent.ACTION_SEND) {
            String scheme = sourceIntent.getScheme();
            if (scheme != null && scheme.equals("qfield")) {
                qfieldIntent = sourceIntent;
                intent.putExtra("QF_ACTION", "trigger_load");
            } else if (scheme != null && scheme.equals("https")) {
                Uri uri = sourceIntent.getData();
                String host = uri.getHost();
                if (host.equals("qfield.org")) {
                    qfieldIntent = sourceIntent;
                    intent.putExtra("QF_ACTION", "trigger_load");
                }
            } else {
                projectIntent = sourceIntent;
                intent.putExtra("QGS_PROJECT", "trigger_load");
            }
        }

        setIntent(intent);
    }

    private String getApplicationDirectory() {
        File primaryExternalFilesDir = getExternalFilesDir(null);
        if (primaryExternalFilesDir != null) {
            return primaryExternalFilesDir.getAbsolutePath();
        }
        return "";
    }

    private String getAdditionalApplicationDirectories() {
        List<String> dirs = new ArrayList<String>();

        File externalStorageDirectory = null;
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
                PackageManager.PERMISSION_GRANTED ||
            (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R &&
             Environment.isExternalStorageManager())) {
            externalStorageDirectory =
                Environment.getExternalStorageDirectory();
        }

        File primaryExternalFilesDir = getExternalFilesDir(null);

        File[] externalFilesDirs = getExternalFilesDirs(null);
        for (File file : externalFilesDirs) {
            if (file != null) {
                // Don't duplicate external files directory or storage
                // path already added
                if (file.getAbsolutePath().equals(
                        primaryExternalFilesDir.getAbsolutePath())) {
                    continue;
                }
                if (externalStorageDirectory != null) {
                    if (!file.getAbsolutePath().contains(
                            externalStorageDirectory.getAbsolutePath())) {
                        dirs.add(file.getAbsolutePath());
                    }
                } else {
                    dirs.add(file.getAbsolutePath());
                }
            }
        }

        StringBuilder rootDirs = new StringBuilder();
        for (String dir : dirs) {
            rootDirs.append(dir);
            rootDirs.append("--;--");
        }
        return rootDirs.toString();
    }

    private String getRootDirectories() {
        List<String> dirs = new ArrayList<String>();

        File externalStorageDirectory = null;
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
                PackageManager.PERMISSION_GRANTED ||
            (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R &&
             Environment.isExternalStorageManager())) {
            externalStorageDirectory =
                Environment.getExternalStorageDirectory();
            if (externalStorageDirectory != null) {
                dirs.add(externalStorageDirectory.getAbsolutePath());
            }
        }

        StringBuilder rootDirs = new StringBuilder();
        for (String dir : dirs) {
            rootDirs.append(dir);
            rootDirs.append("--;--");
        }
        return rootDirs.toString();
    }

    private void triggerImportDatasets() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true);
        intent.setType("*/*");
        try {
            startActivityForResult(intent, IMPORT_DATASET);
        } catch (ActivityNotFoundException e) {
            displayAlertDialog(
                getString(R.string.operation_unsupported),
                getString(R.string.import_operation_unsupported));
            Log.w("QField", "No activity found for ACTION_OPEN_DOCUMENT.");
        }
        return;
    }

    private void triggerImportProjectFolder() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        try {
            startActivityForResult(intent, IMPORT_PROJECT_FOLDER);
        } catch (ActivityNotFoundException e) {
            displayAlertDialog(
                getString(R.string.operation_unsupported),
                getString(R.string.import_operation_unsupported));
            Log.w("QField", "No activity found for ACTION_OPEN_DOCUMENT_TREE.");
        }
        return;
    }

    private void triggerImportProjectArchive() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        intent.setType("application/zip");
        try {
            startActivityForResult(intent, IMPORT_PROJECT_ARCHIVE);
        } catch (ActivityNotFoundException e) {
            displayAlertDialog(
                getString(R.string.operation_unsupported),
                getString(R.string.import_operation_unsupported));
            Log.w("QField", "No activity found for ACTION_OPEN_DOCUMENT.");
        }
        return;
    }

    private void triggerUpdateProjectFromArchive(String path) {
        projectPath = path;
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        intent.setType("application/zip");
        try {
            startActivityForResult(intent, UPDATE_PROJECT_FROM_ARCHIVE);
        } catch (ActivityNotFoundException e) {
            displayAlertDialog(
                getString(R.string.operation_unsupported),
                getString(R.string.import_operation_unsupported));
            Log.w("QField", "No activity found for ACTION_OPEN_DOCUMENT.");
        }
        return;
    }

    private void sendDatasetTo(String paths) {
        showBlockingProgressDialog(getString(R.string.processing_message));

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                String[] filePaths = paths.split("--;--");
                File file;
                if (filePaths.length == 1) {
                    file = new File(paths);
                } else {
                    File temporaryFile = new File(filePaths[0]);
                    file = new File(getCacheDir(),
                                    temporaryFile.getName() + ".zip");
                    try {
                        OutputStream out =
                            new FileOutputStream(file.getAbsolutePath());
                        boolean success =
                            QFieldUtils.filesToZip(out, filePaths);
                        out.close();
                        if (!success) {
                            return;
                        }
                    } catch (Exception e) {
                        dismissBlockingProgressDialog();
                        e.printStackTrace();
                        return;
                    }
                }
                DocumentFile documentFile = DocumentFile.fromFile(file);
                dismissBlockingProgressDialog();

                Context context = getApplication().getApplicationContext();
                Intent intent = new Intent(Intent.ACTION_SEND);
                intent.putExtra(Intent.EXTRA_STREAM,
                                FileProvider.getUriForFile(
                                    context,
                                    context.getPackageName() + ".fileprovider",
                                    file));
                intent.setType(documentFile.getType());
                startActivity(Intent.createChooser(intent, null));
                return;
            }
        });
        return;
    }

    private void exportToFolder(String paths) {
        pathsToExport = paths;

        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        intent.addCategory(Intent.CATEGORY_DEFAULT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        try {
            startActivityForResult(intent, EXPORT_TO_FOLDER);
        } catch (ActivityNotFoundException e) {
            displayAlertDialog(
                getString(R.string.operation_unsupported),
                getString(R.string.export_operation_unsupported));
            Log.w("QField", "No activity found for ACTION_OPEN_DOCUMENT_TREE.");
        }
        return;
    }

    private void removeDataset(String path) {
        File file = new File(path);
        AlertDialog.Builder builder =
            new AlertDialog.Builder(this, R.style.DialogTheme);
        builder.setTitle(getString(R.string.delete_confirm_title));
        builder.setMessage(getString(R.string.delete_confirm_dataset));
        builder.setPositiveButton(
            getString(R.string.delete_confirm),
            new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    file.delete();
                    dialog.dismiss();
                    openPath(file.getParentFile().getAbsolutePath());
                }
            });
        builder.setNegativeButton(getString(R.string.delete_cancel),
                                  new DialogInterface.OnClickListener() {
                                      public void onClick(
                                          DialogInterface dialog, int id) {
                                          dialog.dismiss();
                                      }
                                  });
        AlertDialog dialog = builder.create();
        dialog.setCancelable(false);
        dialog.show();
        return;
    }

    private void sendCompressedFolderTo(String path) {
        showBlockingProgressDialog(getString(R.string.processing_message));

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                File file = new File(path);
                File temporaryFile =
                    new File(getCacheDir(), file.getName() + ".zip");
                QFieldUtils.folderToZip(file.getPath(),
                                        temporaryFile.getPath());
                dismissBlockingProgressDialog();

                DocumentFile documentFile =
                    DocumentFile.fromFile(temporaryFile);
                Context context = getApplication().getApplicationContext();
                Intent intent = new Intent(Intent.ACTION_SEND);
                intent.putExtra(Intent.EXTRA_STREAM,
                                FileProvider.getUriForFile(
                                    context,
                                    context.getPackageName() + ".fileprovider",
                                    temporaryFile));
                intent.setType(documentFile.getType());
                startActivity(Intent.createChooser(intent, null));
                return;
            }
        });
        return;
    }

    private void removeProjectFolder(String path) {
        File file = new File(path);
        AlertDialog.Builder builder =
            new AlertDialog.Builder(this, R.style.DialogTheme);
        builder.setTitle(getString(R.string.delete_confirm_title));
        builder.setMessage(getString(R.string.delete_confirm_folder));
        builder.setPositiveButton(
            getString(R.string.delete_confirm),
            new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    QFieldUtils.deleteDirectory(file, true);
                    dialog.dismiss();
                    openPath(file.getParentFile().getAbsolutePath());
                }
            });
        builder.setNegativeButton(getString(R.string.delete_cancel),
                                  new DialogInterface.OnClickListener() {
                                      public void onClick(
                                          DialogInterface dialog, int id) {
                                          dialog.dismiss();
                                      }
                                  });
        AlertDialog dialog = builder.create();
        dialog.setCancelable(false);
        dialog.show();
        return;
    }

    private void getCameraResource(String prefix, String filePath,
                                   String suffix, boolean isVideo) {
        resourcePrefix = prefix;
        resourceFilePath = filePath;
        resourceSuffix = suffix;

        String timeStamp =
            new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        resourceTempFilePath = "QFieldCamera" + timeStamp;

        Intent intent = isVideo ? new Intent(MediaStore.ACTION_VIDEO_CAPTURE)
                                : new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (intent.resolveActivity(getPackageManager()) != null) {
            Log.d("QField", "Camera intent resolved");
            File storageDir =
                getExternalFilesDir(Environment.DIRECTORY_PICTURES);
            try {
                File tempFile = File.createTempFile(resourceTempFilePath,
                                                    suffix, storageDir);

                if (tempFile != null) {
                    Log.d("QField", "Temporary camera file created");
                    if (tempFile.exists()) {
                        Log.d(
                            "QField",
                            "Temporary camera file exists already, it will be overwritten");
                    }

                    resourceTempFilePath = tempFile.getAbsolutePath();

                    Uri fileURI = FileProvider.getUriForFile(
                        this, "ch.opengis.qfield.fileprovider", tempFile);

                    Log.d("QField",
                          "Camera temporary file uri: " + fileURI.toString());
                    intent.putExtra(MediaStore.EXTRA_OUTPUT, fileURI);
                    Log.d("QField", "Camera intent starting");
                    startActivityForResult(intent, CAMERA_RESOURCE);
                }
            } catch (IOException e) {
                Log.d("QField", e.getMessage());
                resourceCanceled("");
            }
        } else {
            Log.d("QField", "Could not resolve camera intent");
            resourceCanceled("");
        }
        return;
    }

    private void getGalleryResource(String prefix, String filePath,
                                    String mimeType) {
        resourcePrefix = prefix;
        resourceFilePath = filePath;

        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType(mimeType);
        Log.d("QField", "Gallery intent starting");
        startActivityForResult(intent, GALLERY_RESOURCE);
        return;
    }

    private void getFilePickerResource(String prefix, String filePath,
                                       String mimeType) {
        resourcePrefix = prefix;
        resourceFilePath = filePath;

        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, false);
        intent.setType(mimeType);
        Log.d("QField", "File picker intent starting");
        startActivityForResult(intent, FILE_PICKER_RESOURCE);
        return;
    }

    private void openResource(String filePath, String mimeType,
                              boolean isEditing) {
        resourceFilePath = filePath;
        resourceIsEditing = isEditing;

        resourceFile = new File(filePath);
        resourceCacheFile = new File(getCacheDir(), resourceFile.getName());

        // Copy resource to a temporary file
        if (QFieldUtils.copyFile(resourceFile, resourceCacheFile)) {
            Uri contentUri = Build.VERSION.SDK_INT < 24
                                 ? Uri.fromFile(resourceFile)
                                 : FileProvider.getUriForFile(
                                       this, "ch.opengis.qfield.fileprovider",
                                       resourceCacheFile);

            Intent intent =
                new Intent(isEditing ? Intent.ACTION_EDIT : Intent.ACTION_VIEW);
            if (isEditing) {
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                                Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
                if (mimeType.contains("image/")) {
                    intent.setDataAndType(contentUri, "image/*");
                } else {
                    intent.setDataAndType(contentUri, mimeType);
                }
                intent.putExtra(MediaStore.EXTRA_OUTPUT, contentUri);
            } else {
                intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                intent.setDataAndType(contentUri, mimeType);
            }
            try {
                Log.d("QField", "Open intent starting");
                startActivityForResult(intent, OPEN_RESOURCE);
            } catch (IllegalArgumentException e) {
                Log.d("QField", e.getMessage());
                resourceCanceled("");
            } catch (Exception e) {
                Log.d("QField", e.getMessage());
                resourceCanceled("");
            }
        } else {
            resourceCanceled("");
        }

        return;
    }

    void importDatasets(Uri[] datasetUris) {
        File externalFilesDir = getExternalFilesDir(null);
        if (externalFilesDir == null || datasetUris.length == 0) {
            return;
        }

        ProgressDialog progressDialog =
            new ProgressDialog(this, R.style.DialogTheme);
        progressDialog.setMessage(getString(R.string.import_dataset_wait));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.show();

        String importDatasetPath =
            externalFilesDir.getAbsolutePath() + "/Imported Datasets/";
        new File(importDatasetPath).mkdir();

        Context context = getApplication().getApplicationContext();
        ContentResolver resolver = getContentResolver();

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                boolean imported = false;
                for (Uri datasetUri : datasetUris) {
                    DocumentFile documentFile =
                        DocumentFile.fromSingleUri(context, datasetUri);
                    String importFilePath =
                        importDatasetPath + documentFile.getName();
                    try {
                        InputStream input =
                            resolver.openInputStream(datasetUri);
                        imported = QFieldUtils.inputStreamToFile(
                            input, importFilePath, documentFile.length());
                    } catch (Exception e) {
                        e.printStackTrace();
                        imported = false;
                    }
                    if (!imported) {
                        break;
                    }
                }

                progressDialog.dismiss();
                if (!imported) {
                    if (!isFinishing()) {
                        displayAlertDialog(
                            getString(R.string.import_error),
                            getString(R.string.import_dataset_error));
                    }
                } else {
                    openPath(importDatasetPath);
                }
            }
        });
    }

    void importProjectFolder(Uri folderUri) {
        File externalFilesDir = getExternalFilesDir(null);
        if (externalFilesDir == null) {
            return;
        }

        ProgressDialog progressDialog =
            new ProgressDialog(this, R.style.DialogTheme);
        progressDialog.setMessage(getString(R.string.import_project_wait));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.show();

        String importProjectPath =
            externalFilesDir.getAbsolutePath() + "/Imported Projects/";
        new File(importProjectPath).mkdir();

        Context context = getApplication().getApplicationContext();
        ContentResolver resolver = getContentResolver();

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                DocumentFile directory =
                    DocumentFile.fromTreeUri(context, folderUri);
                String importPath =
                    importProjectPath + directory.getName() + "/";
                new File(importPath).mkdir();
                boolean imported = QFieldUtils.documentFileToFolder(
                    directory, importPath, resolver);

                progressDialog.dismiss();
                if (imported) {
                    openPath(importPath);
                } else {
                    if (!isFinishing()) {
                        displayAlertDialog(
                            getString(R.string.import_error),
                            getString(R.string.import_project_folder_error));
                    }
                }
            }
        });
    }

    void importProjectArchive(Uri archiveUri) {
        File externalFilesDir = getExternalFilesDir(null);
        if (externalFilesDir == null) {
            return;
        }

        ProgressDialog progressDialog =
            new ProgressDialog(this, R.style.DialogTheme);
        progressDialog.setMessage(getString(R.string.import_project_wait));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.show();

        String importProjectPath =
            externalFilesDir.getAbsolutePath() + "/Imported Projects/";
        new File(importProjectPath).mkdir();

        Context context = getApplication().getApplicationContext();
        ContentResolver resolver = getContentResolver();

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                DocumentFile documentFile =
                    DocumentFile.fromSingleUri(context, archiveUri);

                String projectName = "";
                try {
                    InputStream input = resolver.openInputStream(archiveUri);
                    projectName = QFieldUtils.getArchiveProjectName(input);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                if (projectName != "") {
                    String importPath =
                        importProjectPath +
                        documentFile.getName().substring(
                            0, documentFile.getName().lastIndexOf(".")) +
                        "/";
                    new File(importPath).mkdir();
                    boolean imported = false;
                    try {
                        InputStream input =
                            resolver.openInputStream(archiveUri);
                        imported = QFieldUtils.zipToFolder(input, importPath);
                    } catch (Exception e) {
                        e.printStackTrace();

                        if (!isFinishing()) {
                            displayAlertDialog(
                                getString(R.string.import_error),
                                getString(
                                    R.string.import_project_archive_error));
                        }
                    }

                    progressDialog.dismiss();
                    if (imported) {
                        openPath(importPath);
                    }
                } else {
                    progressDialog.dismiss();
                }
            }
        });
    }

    void updateProjectFromArchive(Uri archiveUri) {
        File externalFilesDir = getExternalFilesDir(null);
        if (externalFilesDir == null) {
            return;
        }

        ProgressDialog progressDialog =
            new ProgressDialog(this, R.style.DialogTheme);
        progressDialog.setMessage(getString(R.string.update_project_wait));
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.show();

        Context context = getApplication().getApplicationContext();
        ContentResolver resolver = getContentResolver();

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                DocumentFile documentFile =
                    DocumentFile.fromSingleUri(context, archiveUri);

                String projectFolder =
                    new File(projectPath).getParentFile().getAbsolutePath() +
                    "/";
                boolean imported = false;
                try {
                    InputStream input = resolver.openInputStream(archiveUri);
                    imported = QFieldUtils.zipToFolder(input, projectFolder);
                } catch (Exception e) {
                    e.printStackTrace();

                    if (!isFinishing()) {
                        displayAlertDialog(
                            getString(R.string.import_error),
                            getString(R.string.import_project_archive_error));
                    }
                }

                progressDialog.dismiss();
                if (imported) {
                    // Trigger a project re-load
                    openProject(projectPath);
                }
            }
        });
    }

    private void checkStoragePermissions() {
        List<String> permissionsList = new ArrayList<String>();
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
            PackageManager.PERMISSION_DENIED) {
            permissionsList.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this,
                    Manifest.permission.READ_MEDIA_IMAGES) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.READ_MEDIA_IMAGES);
            }
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this,
                    Manifest.permission.READ_MEDIA_VIDEO) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.READ_MEDIA_VIDEO);
            }
        }
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.ACCESS_MEDIA_LOCATION) ==
            PackageManager.PERMISSION_DENIED) {
            permissionsList.add(Manifest.permission.ACCESS_MEDIA_LOCATION);
        }
        if (permissionsList.size() > 0) {
            String[] permissions = new String[permissionsList.size()];
            permissionsList.toArray(permissions);
            ActivityCompat.requestPermissions(QFieldActivity.this, permissions,
                                              101);
        }
    }

    private void checkAllFileAccess() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R &&
            !Environment.isExternalStorageManager() &&
            !sharedPreferences.getBoolean("DontAskAllFilesPermission", false)) {
            // if MANAGE_EXTERNAL_STORAGE permission isn't in the manifest,
            // bail out
            String[] requestedPermissions;
            try {
                PackageInfo pi = getPackageManager().getPackageInfo(
                    this.getPackageName(), PackageManager.GET_PERMISSIONS);
                requestedPermissions = pi.requestedPermissions;
            } catch (NameNotFoundException e) {
                e.printStackTrace();
                finish();
                return;
            }
            if (!Arrays.asList(requestedPermissions)
                     .contains(Manifest.permission.MANAGE_EXTERNAL_STORAGE)) {
                return;
            }

            checkStoragePermissions();

            AlertDialog.Builder builder =
                new AlertDialog.Builder(this, R.style.DialogTheme);
            builder.setTitle(getString(R.string.grant_permission));
            builder.setMessage(
                Build.VERSION.SDK_INT >= Build.VERSION_CODES.N
                    ? Html.fromHtml(
                          getString(R.string.grant_all_files_permission),
                          Html.FROM_HTML_MODE_LEGACY)
                    : Html.fromHtml(
                          getString(R.string.grant_all_files_permission)));
            builder.setPositiveButton(
                getString(R.string.grant),
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        try {
                            Uri uri = Uri.parse("package:ch.opengis.qfield");
                            Intent intent = new Intent(
                                Settings
                                    .ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,
                                uri);
                            startActivity(intent);
                        } catch (Exception e) {
                            Log.e(
                                "QField",
                                "Failed to initial activity to grant all files access",
                                e);
                        }
                        dialog.dismiss();
                    }
                });
            builder.setNegativeButton(
                getString(R.string.deny_always),
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        sharedPreferenceEditor.putBoolean(
                            "DontAskAllFilesPermission", true);
                        sharedPreferenceEditor.commit();

                        dialog.dismiss();
                    }
                });

            builder.setNeutralButton(getString(R.string.deny_once),
                                     new DialogInterface.OnClickListener() {
                                         public void onClick(
                                             DialogInterface dialog, int id) {
                                             dialog.dismiss();
                                         }
                                     });

            AlertDialog dialog = builder.create();
            dialog.setCancelable(false);
            dialog.show();
        }
    }

    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        if (requestCode == CAMERA_RESOURCE) {
            if (resultCode == RESULT_OK) {
                File file = new File(resourceTempFilePath);
                String finalFilePath = QFieldUtils.replaceFilenameTags(
                    resourceFilePath, file.getName());
                File result = new File(resourcePrefix + finalFilePath);
                Log.d("QField",
                      "Taken camera picture: " + file.getAbsolutePath());
                try {
                    InputStream in = new FileInputStream(file);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  file.length());
                    file.delete();
                } catch (Exception e) {
                    e.printStackTrace();
                }

                // Let the android scan new media folders/files to make them
                // visible through MTP
                result.setReadable(true);
                result.setWritable(true);
                MediaScannerConnection.scanFile(
                    this, new String[] {result.getParentFile().toString()},
                    null, null);
                resourceReceived(finalFilePath);
            } else {
                resourceCanceled("");
            }
        } else if (requestCode == GALLERY_RESOURCE) {
            if (resultCode == RESULT_OK) {
                Uri uri = data.getData();
                DocumentFile documentFile = DocumentFile.fromSingleUri(
                    getApplication().getApplicationContext(), uri);
                String finalFilePath = QFieldUtils.replaceFilenameTags(
                    resourceFilePath, documentFile.getName());
                File result = new File(resourcePrefix + finalFilePath);
                Log.d("QField",
                      "Selected gallery file: " + data.getData().toString());
                try {
                    InputStream in = getContentResolver().openInputStream(uri);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  documentFile.length());
                } catch (Exception e) {
                    Log.d("QField", e.getMessage());
                }

                // Let the android scan new media folders/files to make them
                // visible through MTP
                result.setReadable(true);
                result.setWritable(true);
                MediaScannerConnection.scanFile(
                    this, new String[] {result.getParentFile().toString()},
                    null, null);
                resourceReceived(finalFilePath);
            } else {
                resourceCanceled("");
            }
        } else if (requestCode == FILE_PICKER_RESOURCE) {
            if (resultCode == RESULT_OK) {
                Uri uri = data.getData();
                DocumentFile documentFile = DocumentFile.fromSingleUri(
                    getApplication().getApplicationContext(), uri);
                String finalFilePath = QFieldUtils.replaceFilenameTags(
                    resourceFilePath, documentFile.getName());
                File result = new File(resourcePrefix + finalFilePath);
                Log.d("QField", "Selected file picker file: " +
                                    data.getData().toString());
                try {
                    InputStream in = getContentResolver().openInputStream(uri);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  documentFile.length());
                } catch (Exception e) {
                    Log.d("QField", e.getMessage());
                }

                resourceReceived(finalFilePath);
            } else {
                resourceCanceled("");
            }
        } else if (requestCode == OPEN_RESOURCE) {
            if (resultCode == RESULT_OK) {
                try {
                    if (resourceIsEditing) {
                        Log.d(
                            "QField",
                            "Copy file back from uri " + data.getDataString() +
                                " to file: " + resourceFile.getAbsolutePath());
                        InputStream in = getContentResolver().openInputStream(
                            data.getData());
                        OutputStream out = new FileOutputStream(resourceFile);
                        // Transfer bytes from in to out
                        byte[] buf = new byte[1024];
                        int len;
                        while ((len = in.read(buf)) > 0) {
                            out.write(buf, 0, len);
                        }
                        out.close();
                    }
                    resourceOpened(resourceFile.getAbsolutePath());
                } catch (SecurityException e) {
                    resourceCanceled(e.getMessage());
                } catch (IOException e) {
                    resourceCanceled(e.getMessage());
                }
            } else {
                resourceCanceled("");
            }
        } else if (requestCode == IMPORT_DATASET &&
                   resultCode == Activity.RESULT_OK) {
            Log.d("QField", "handling import dataset(s)");
            File externalFilesDir = getExternalFilesDir(null);
            if (externalFilesDir == null || data == null) {
                return;
            }

            String importDatasetPath =
                externalFilesDir.getAbsolutePath() + "/Imported Datasets/";

            Context context = getApplication().getApplicationContext();
            ContentResolver resolver = getContentResolver();

            Uri[] datasetUris;
            if (data.getClipData() != null) {
                datasetUris = new Uri[data.getClipData().getItemCount()];
                for (int i = 0; i < data.getClipData().getItemCount(); i++) {
                    datasetUris[i] = data.getClipData().getItemAt(i).getUri();
                }
            } else {
                datasetUris = new Uri[1];
                datasetUris[0] = data.getData();
            }

            boolean hasExists = false;
            for (Uri datasetUri : datasetUris) {
                DocumentFile documentFile =
                    DocumentFile.fromSingleUri(context, datasetUri);
                File importFilePath =
                    new File(importDatasetPath + documentFile.getName());
                if (importFilePath.exists()) {
                    hasExists = true;
                    break;
                }
            }

            if (hasExists) {
                AlertDialog.Builder builder =
                    new AlertDialog.Builder(this, R.style.DialogTheme);
                builder.setTitle(getString(R.string.import_overwrite_title));
                builder.setMessage(
                    datasetUris.length > 1
                        ? getString(R.string.import_overwrite_dataset_multiple)
                        : getString(R.string.import_overwrite_dataset_single));
                builder.setPositiveButton(
                    getString(R.string.import_overwrite_confirm),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            importDatasets(datasetUris);
                            dialog.dismiss();
                        }
                    });
                builder.setNegativeButton(
                    getString(R.string.import_overwrite_cancel),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.dismiss();
                        }
                    });
                AlertDialog dialog = builder.create();
                dialog.setCancelable(false);
                dialog.show();
            } else {
                importDatasets(datasetUris);
            }
        } else if (requestCode == IMPORT_PROJECT_FOLDER &&
                   resultCode == Activity.RESULT_OK) {
            Log.d("QField", "handling import project folder");
            File externalFilesDir = getExternalFilesDir(null);
            if (externalFilesDir == null || data == null) {
                return;
            }

            Uri uri = data.getData();
            Context context = getApplication().getApplicationContext();
            DocumentFile directory = DocumentFile.fromTreeUri(context, uri);
            File importPath =
                new File(externalFilesDir.getAbsolutePath() +
                         "/Imported Projects/" + directory.getName() + "/");
            if (importPath.exists()) {
                AlertDialog.Builder builder =
                    new AlertDialog.Builder(this, R.style.DialogTheme);
                builder.setTitle(getString(R.string.import_overwrite_title));
                builder.setMessage(getString(R.string.import_overwrite_folder));
                builder.setPositiveButton(
                    getString(R.string.import_overwrite_confirm),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            importProjectFolder(uri);
                            dialog.dismiss();
                        }
                    });
                builder.setNegativeButton(
                    getString(R.string.import_overwrite_cancel),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.dismiss();
                        }
                    });
                AlertDialog dialog = builder.create();
                dialog.setCancelable(false);
                dialog.show();
            } else {
                importProjectFolder(uri);
            }
        } else if (requestCode == IMPORT_PROJECT_ARCHIVE &&
                   resultCode == Activity.RESULT_OK) {
            Log.d("QField", "handling import project archive");
            File externalFilesDir = getExternalFilesDir(null);
            if (externalFilesDir == null || data == null) {
                return;
            }

            String importProjectPath =
                externalFilesDir.getAbsolutePath() + "/Imported Projects/";
            new File(importProjectPath).mkdir();

            Uri uri = data.getData();
            Context context = getApplication().getApplicationContext();
            ContentResolver resolver = getContentResolver();

            DocumentFile documentFile =
                DocumentFile.fromSingleUri(context, uri);
            File importPath = new File(
                externalFilesDir.getAbsolutePath() + "/Imported Projects/" +
                documentFile.getName().substring(
                    0, documentFile.getName().lastIndexOf(".")) +
                "/");
            if (importPath.exists()) {
                AlertDialog.Builder builder =
                    new AlertDialog.Builder(this, R.style.DialogTheme);
                builder.setTitle(getString(R.string.import_overwrite_title));
                builder.setMessage(getString(R.string.import_overwrite_folder));
                builder.setPositiveButton(
                    getString(R.string.import_overwrite_confirm),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            importProjectArchive(uri);
                            dialog.dismiss();
                        }
                    });
                builder.setNegativeButton(
                    getString(R.string.import_overwrite_cancel),
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.dismiss();
                        }
                    });
                AlertDialog dialog = builder.create();
                dialog.setCancelable(false);
                dialog.show();
            } else {
                importProjectArchive(uri);
            }
        } else if (requestCode == UPDATE_PROJECT_FROM_ARCHIVE &&
                   resultCode == Activity.RESULT_OK) {
            Log.d("QField", "handling updating project from archive");
            File externalFilesDir = getExternalFilesDir(null);
            if (externalFilesDir == null || data == null) {
                return;
            }

            Uri uri = data.getData();
            Context context = getApplication().getApplicationContext();
            ContentResolver resolver = getContentResolver();

            DocumentFile documentFile =
                DocumentFile.fromSingleUri(context, uri);

            updateProjectFromArchive(uri);
        } else if (requestCode == EXPORT_TO_FOLDER &&
                   resultCode == Activity.RESULT_OK) {
            Log.d("QField", "handling export to folder");

            String[] paths = pathsToExport.split("--;--");
            Uri uri = data.getData();
            Context context = getApplication().getApplicationContext();
            ContentResolver resolver = getContentResolver();

            executorService.execute(new Runnable() {
                @Override
                public void run() {
                    resolver.takePersistableUriPermission(
                        uri, Intent.FLAG_GRANT_READ_URI_PERMISSION |
                                 Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
                    DocumentFile directory =
                        DocumentFile.fromTreeUri(context, uri);

                    boolean exported = true;
                    for (String path : paths) {
                        File file = new File(path);
                        exported = QFieldUtils.fileToDocumentFile(
                            file, directory, resolver);
                        if (!exported) {
                            break;
                        }
                    }

                    if (!exported) {
                        if (!isFinishing()) {
                            displayAlertDialog(
                                getString(R.string.export_error),
                                getString(R.string.export_to_folder_error));
                        }
                    }
                }
            });
        } else {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }
}
