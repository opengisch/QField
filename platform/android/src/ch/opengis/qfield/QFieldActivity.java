/**
 * QFieldActivity.java - class needed to copy files from assets to
 * getExternalFilesDir() before starting QtActivity this can be used to perform
 * actions before QtActivity takes over.
 * @author  Marco Bernasocchi - <marco@opengis.ch>
 * @version 0.5
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
import android.content.res.Resources;
import android.graphics.Color;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.text.Html;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.DisplayCutout;
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
import org.qtproject.qt5.android.bindings.QtActivity;

public class QFieldActivity extends QtActivity {

    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor sharedPreferenceEditor;
    private ProgressDialog progressDialog;
    ExecutorService executorService = Executors.newFixedThreadPool(4);

    public static native void openProject(String url);
    public static native void openPath(String path);

    private float originalBrightness;
    private String pathsToExport;
    private double sceneTopMargin = 0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        prepareQtActivity();
        super.onCreate(savedInstanceState);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            int resourceId = getResources().getIdentifier("status_bar_height",
                                                          "dimen", "android");
            if (resourceId > 0) {
                sceneTopMargin = getResources().getDimension(resourceId);
            }

            if (sceneTopMargin <= 0) {
                sceneTopMargin = Math.ceil(
                    (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? 24 : 25) *
                    getResources().getDisplayMetrics().density);
            }

            Window window = getWindow();
            window.addFlags(
                WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            window.getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
            window.setStatusBarColor(Color.TRANSPARENT);
            View decor = window.getDecorView();
            decor.setSystemUiVisibility(decor.getSystemUiVisibility() |
                                        View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR);
        }
    }

    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (intent.getAction() == Intent.ACTION_VIEW ||
            intent.getAction() == Intent.ACTION_SEND) {
            openProject(getPathFromIntent(intent));
        }
    }

    private String getPathFromIntent(Intent intent) {
        String scheme = intent.getScheme();
        String action = intent.getAction();
        String type = intent.getType();

        Uri uri = null;
        if (action.compareTo(Intent.ACTION_SEND) == 0) {
            uri = (Uri)intent.getParcelableExtra(Intent.EXTRA_STREAM);
            scheme = "";
        } else {
            uri = intent.getData();
        }

        Context context = getApplication().getApplicationContext();

        String filePath = QFieldUtils.getPathFromUri(context, uri);
        String importDatasetPath = "";
        String importProjectPath = "";
        File externalFilesDir = getExternalFilesDir(null);
        if (externalFilesDir != null) {
            importDatasetPath =
                externalFilesDir.getAbsolutePath() + "/Imported Datasets/";
            new File(importDatasetPath).mkdir();
            importProjectPath =
                externalFilesDir.getAbsolutePath() + "/Imported Projects/";
            new File(importProjectPath).mkdir();
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT &&
            (scheme.compareTo(ContentResolver.SCHEME_CONTENT) == 0 ||
             action.compareTo(Intent.ACTION_SEND) == 0) &&
            importDatasetPath != "") {
            DocumentFile documentFile =
                DocumentFile.fromSingleUri(context, uri);
            String fileName = documentFile.getName();
            long fileBytes = documentFile.length();
            if (fileName == null) {
                if (type == null) {
                    return filePath;
                }
                // File name not provided
                fileName = new SimpleDateFormat("yyyyMMdd_HHmmss")
                               .format(new Date().getTime()) +
                           "." + QFieldUtils.getExtensionFromMimeType(type);
            }
            String fileBaseName = fileName;
            String fileExtension = "";
            if (fileName.lastIndexOf(".") > -1) {
                fileBaseName = fileName.substring(0, fileName.lastIndexOf("."));
                fileExtension = fileName.substring(fileName.lastIndexOf("."));
            }

            ContentResolver resolver = getContentResolver();
            if (type != null && type.equals("application/zip")) {
                String projectName = "";
                try {
                    InputStream input = resolver.openInputStream(uri);
                    projectName = QFieldUtils.getArchiveProjectName(input);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                if (projectName != "") {
                    String projectPath = importProjectPath + fileBaseName + "/";
                    int i = 1;
                    while (new File(projectPath).exists()) {
                        projectPath =
                            importProjectPath + fileBaseName + "_" + i + "/";
                        i++;
                    }
                    new File(projectPath).mkdir();
                    try {
                        InputStream input = resolver.openInputStream(uri);
                        QFieldUtils.zipToFolder(input, projectPath);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    Log.v("QField", "Opening decompressed project: " +
                                        projectPath + projectName);
                    return projectPath + projectName;
                }
            }

            Boolean canWrite =
                filePath != "" ? new File(filePath).canWrite() : false;
            if (!canWrite) {
                Log.v("QField", "Content intent detected: " + action + " : " +
                                    intent.getDataString() + " : " + type +
                                    " : " + fileName);
                String importFilePath = importDatasetPath + fileName;
                int i = 1;
                while (new File(importFilePath).exists()) {
                    importFilePath = importDatasetPath + fileBaseName + "_" +
                                     i + fileExtension;
                    i++;
                }
                Log.v("QField",
                      "Importing document to file path: " + importFilePath);
                try {
                    InputStream input = resolver.openInputStream(uri);
                    QFieldUtils.inputStreamToFile(input, importFilePath,
                                                  fileBytes);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                return importFilePath;
            }
        }

        Log.v("QField", "Opening document file path: " + filePath);
        return filePath;
    }

    private double statusBarMargin() {
        double margin = sceneTopMargin;
        if (margin > 0) {
            int resourceId = getResources().getIdentifier("status_bar_height",
                                                          "dimen", "android");
            if (resourceId > 0) {
                double dimension = getResources().getDimension(resourceId);
                if (dimension > 0) {
                    margin = dimension;
                }
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                DisplayCutout cutout = getWindow()
                                           .getDecorView()
                                           .getRootWindowInsets()
                                           .getDisplayCutout();
                if (cutout != null) {
                    int cutoutHeight = cutout.getSafeInsetTop();
                    if (cutoutHeight > margin) {
                        margin = cutoutHeight;
                    }
                }
            }
        }
        return margin;
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

        checkPermissions();
        checkAllFileAccess(); // Storage access permission handling for Android
                              // 11+

        List<String> dataDirs = new ArrayList<String>();

        File primaryExternalFilesDir = getExternalFilesDir(null);
        if (primaryExternalFilesDir != null) {
            String dataDir = primaryExternalFilesDir.getAbsolutePath() + "/";
            // create import directories
            new File(dataDir + "Imported Datasets/").mkdir();
            new File(dataDir + "Imported Projects/").mkdir();

            dataDir = dataDir + "QField/";
            // create QField directories
            new File(dataDir).mkdir();
            new File(dataDir + "basemaps/").mkdir();
            new File(dataDir + "fonts/").mkdir();
            new File(dataDir + "proj/").mkdir();
            new File(dataDir + "auth/").mkdir();

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

                dataDirs.add(dataDir);
            }
        }

        Intent intent = new Intent();
        intent.setClass(QFieldActivity.this, QtActivity.class);
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
            intent.putExtra("QGS_PROJECT", getPathFromIntent(sourceIntent));
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
        startActivityForResult(intent, R.id.import_dataset);
        return;
    }

    private void triggerImportProjectFolder() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        try {
            startActivityForResult(intent, R.id.import_project_folder);
        } catch (ActivityNotFoundException e) {
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
        startActivityForResult(intent, R.id.import_project_archive);
        return;
    }

    private void sendDatasetTo(String paths) {
        String[] filePaths = paths.split("--;--");
        File file;
        if (filePaths.length == 1) {
            file = new File(paths);
        } else {
            File temporaryFile = new File(filePaths[0]);
            file = new File(getCacheDir(), temporaryFile.getName() + ".zip");
            try {
                OutputStream out = new FileOutputStream(file.getAbsolutePath());
                boolean success = QFieldUtils.filesToZip(out, filePaths);
                out.close();
                if (!success) {
                    return;
                }
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }
        }

        DocumentFile documentFile = DocumentFile.fromFile(file);
        Context context = getApplication().getApplicationContext();
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.putExtra(
            Intent.EXTRA_STREAM,
            FileProvider.getUriForFile(
                context, context.getPackageName() + ".fileprovider", file));
        intent.setType(documentFile.getType());
        startActivity(Intent.createChooser(intent, null));
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
            startActivityForResult(intent, R.id.export_to_folder);
        } catch (ActivityNotFoundException e) {
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
        File file = new File(path);
        File temporaryFile = new File(getCacheDir(), file.getName() + ".zip");
        QFieldUtils.folderToZip(file.getPath(), temporaryFile.getPath());

        DocumentFile documentFile = DocumentFile.fromFile(temporaryFile);
        Context context = getApplication().getApplicationContext();
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.putExtra(Intent.EXTRA_STREAM,
                        FileProvider.getUriForFile(
                            context, context.getPackageName() + ".fileprovider",
                            temporaryFile));
        intent.setType(documentFile.getType());
        startActivity(Intent.createChooser(intent, null));
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

    private void checkPermissions() {
        List<String> permissionsList = new ArrayList<String>();
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) ==
            PackageManager.PERMISSION_DENIED) {
            permissionsList.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.ACCESS_FINE_LOCATION) ==
            PackageManager.PERMISSION_DENIED) {
            permissionsList.add(Manifest.permission.ACCESS_FINE_LOCATION);
        }
        if (ContextCompat.checkSelfPermission(
                QFieldActivity.this,
                Manifest.permission.ACCESS_MEDIA_LOCATION) ==
            PackageManager.PERMISSION_DENIED) {
            permissionsList.add(Manifest.permission.ACCESS_MEDIA_LOCATION);
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this, Manifest.permission.BLUETOOTH_SCAN) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.BLUETOOTH_SCAN);
            }
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this,
                    Manifest.permission.BLUETOOTH_CONNECT) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.BLUETOOTH_CONNECT);
            }
        } else {
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this, Manifest.permission.BLUETOOTH) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.BLUETOOTH);
            }
            if (ContextCompat.checkSelfPermission(
                    QFieldActivity.this, Manifest.permission.BLUETOOTH_ADMIN) ==
                PackageManager.PERMISSION_DENIED) {
                permissionsList.add(Manifest.permission.BLUETOOTH_ADMIN);
            }
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
            // if MANAGE_EXTERNAL_STORAGE permission isn't in the manifest, bail
            // out
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
        if (requestCode == R.id.import_dataset &&
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
        } else if (requestCode == R.id.import_project_folder &&
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
        } else if (requestCode == R.id.import_project_archive &&
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
        } else if (requestCode == R.id.export_to_folder &&
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
