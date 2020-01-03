/**
 * QFieldActivity.java - class needed to copy files from assets to getExternalFilesDir() before starting QtActivity
 * this can be used to perform actions before QtActivity takes over.
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

 THIS SOFTWARE IS PROVIDED BY Marco Bernasocchi <marco@opengis.ch> ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Marco Bernasocchi <marco@opengis.ch> BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package ch.opengis.qfield;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.lang.Thread;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import ch.opengis.qfield.R;

import android.app.Activity;
import android.app.Application;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import org.qtproject.qt5.android.bindings.QtActivity;
public class QFieldActivity extends Activity {
  private static final String QtTAG = "QField";
  private static final int PROGRESS_DIALOG = 0;
  private static final int NOEXTERNALSTORAGE_DIALOG = 1;
  private ProgressDialog mProgressDialog = null;
  private UnzipTask mUnzipTask = new UnzipTask();
  private ActivityInfo mActivityInfo = null; // activity info object, used to
  private SharedPreferences mPrefs = null; // access the metadata
  private String mThisRev = null; // the git_rev of qgis
  private boolean mExternalStorageAvailable = false;
  private boolean mExternalStorageWriteable = false;
  private String mDotQgis2Dir;
  private String mShareDir;

    private static Application application;
    
    public static Context getContext() {
        return application.getApplicationContext();
    }
    
  /** Called when the activity is first created. */
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    application = this.getApplication();

    // Put launch count and first launch values from parameters into AppRater properties
    // to be used for test purposes
    SharedPreferences appRaterSharedPreferences = getSharedPreferences("AppRater", Context.MODE_PRIVATE);
    SharedPreferences.Editor editor = appRaterSharedPreferences.edit();
    if (getIntent().hasExtra("LaunchCount")){
        long launchCount = getIntent().getLongExtra("LaunchCount", 0);
        editor.putLong("LaunchCount", launchCount);
    }
    if (getIntent().hasExtra("FirstLaunch")){
        long firstLaunch = getIntent().getLongExtra("FirstLaunch", 0);
        editor.putLong("FirstLaunch", firstLaunch);
    }
    if (getIntent().hasExtra("DontShowAgain")){
        boolean dontShowAgain = getIntent().getBooleanExtra("DontShowAgain", false);
        editor.putBoolean("DontShowAgain", dontShowAgain);
    }
    editor.commit();

    // get preferences, 0 = mode private. only this app can read these
    mPrefs = this.getApplicationContext().getSharedPreferences("qgisPrefs",
             0);
    try {
      mActivityInfo = getPackageManager().getActivityInfo(
                        getComponentName(), PackageManager.GET_META_DATA);
    } catch (NameNotFoundException e) {
      e.printStackTrace();
      finish();
      return;
    }

    String state = Environment.getExternalStorageState();
    if (Environment.MEDIA_MOUNTED.equals(state)) {
      mExternalStorageAvailable = mExternalStorageWriteable = true;
    } else if (Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
      mExternalStorageAvailable = true;
      mExternalStorageWriteable = false;
    } else {
      mExternalStorageAvailable = mExternalStorageWriteable = false;
    }

    if (mExternalStorageWriteable) {
      startFirstRun();
    } else {
      showDialog(NOEXTERNALSTORAGE_DIALOG);
    }
  }

  public void onDestroy() {
    super.onDestroy();
    mUnzipTask.cancel(true);
  }

  protected Dialog onCreateDialog(int id) {
    switch (id) {
    case PROGRESS_DIALOG:
      mProgressDialog = new ProgressDialog(QFieldActivity.this);
      mProgressDialog.setMessage(getString(R.string.unpacking_msg));
      mProgressDialog.setIndeterminate(false);
      mProgressDialog.setMax(100);
      mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
      return mProgressDialog;

    case NOEXTERNALSTORAGE_DIALOG:
      return new AlertDialog.Builder(QFieldActivity.this)
             .setTitle(getString(R.string.external_storage_unavailable))
             .setMessage(getString(R.string.noexternalstorage_dialog))
             .setPositiveButton(
               getString(R.string.use_internal_storage),
      new DialogInterface.OnClickListener() {
        public void onClick(DialogInterface dialog,
                            int whichButton) {
          dialog.cancel();
          startFirstRun();
        }
      })
      .setNegativeButton(getString(android.R.string.no),
      new DialogInterface.OnClickListener() {
        public void onClick(DialogInterface dialog,
                            int whichButton) {
          finish();

        }
      }).create();

    default:
      return null;
    }
  }

  private void startFirstRun() {
    // get git_rev from the manifest metadata
    mThisRev = mActivityInfo.metaData.getString("android.app.git_rev");
    String lastRev = mPrefs.getString("lastRunGitRevision", "");
    Log.i(QtTAG, "last git_rev:" + lastRev);
    Log.i(QtTAG, "this git_rev:" + mThisRev);

    if (lastRev.equals(mThisRev)) {
      Log.i(QtTAG, "not first run, forwarding to QField");
      startQtActivity();
    } else {
      // this is a first run after install or update
      mUnzipTask.execute("assets.zip");
    }
  }

  private void startQtActivity() {
    // forward to startQtActivity and finish QFieldActivity
    Intent intent = new Intent();
    intent.setClass(QFieldActivity.this, QtActivity.class);
    intent.putExtra("DOTQGIS2_DIR", mDotQgis2Dir);
    intent.putExtra("SHARE_DIR", mShareDir);
    intent.putExtra("PACKAGE_PATH", getFilesDir().toString() + "/share");

    Intent sourceIntent = getIntent();
    if (sourceIntent.getAction() == Intent.ACTION_VIEW) {
      Uri uri = sourceIntent.getData();
      intent.putExtra("QGS_PROJECT", (uri.getPath()));
    }

    startActivity(intent);
    finish();
  }

  private void makeSymlink(String path, String pathAlias) {
    try {
      String cmd = "rm -rf " + pathAlias;
      Runtime.getRuntime().exec(cmd);
      cmd = "ln -s " + path + " " + pathAlias;
      Runtime.getRuntime().exec(cmd);

      Log.i(QtTAG, "Symlinked '" + path + " to " + pathAlias + "'");

    } catch (IOException e) {
      Log.w(QtTAG, "Can't symlink '" + path + " to " + pathAlias + "'", e);
    }
  }

  private class UnzipTask extends AsyncTask<String, Integer, String> {
      protected String doInBackground(String... urlString) {
          try{
              extractZipFile(urlString[0]);
          }catch (Exception e){
              e.printStackTrace();
          }
          return null;
      }

      private void extractZipFile(String zipFile) throws IOException, InterruptedException{
          Log.d(QtTAG, "Exctract zip file asset: "+zipFile);
          InputStream is = getAssets().open(zipFile);
          ZipInputStream zis = new ZipInputStream(new BufferedInputStream(is));
          long totalSize = is.available();
          float currentSize = 0;
          float percent = 0;
          try {
              Thread.sleep(2500);
              ZipEntry ze;
              int count;
              byte[] buffer = new byte[8192];
              while ((ze = zis.getNextEntry()) != null) {
                  File file = new File(getFilesDir(), ze.getName());
                  File dir = ze.isDirectory() ? file : file.getParentFile();
                  if (!dir.isDirectory() && !dir.mkdirs())
                      throw new FileNotFoundException("Failed to ensure directory: " + dir.getAbsolutePath());
                  if (ze.isDirectory())
                      continue;
                  FileOutputStream fout = new FileOutputStream(file);
                  try {
                      while ((count = zis.read(buffer)) != -1)
                          fout.write(buffer, 0, count);
                  } finally {
                      fout.close();
                  }
                  currentSize = is.available();

                  // 108 instead of 100 beacuse the is.available() is not accurate on large files and with
                  // 108 we have a 100% when the unzip is finished
                  percent = 108 - currentSize / totalSize * 100;
                  publishProgress((int) percent);

                  Log.d(QtTAG, "File unzipped: " + file.getPath());
                  Log.d(QtTAG, "Exists: " + file.exists());
                  Log.d(QtTAG, "Percent: " + percent);
              }
          } finally {
              zis.close();
          }
      }

    protected void onPreExecute() {
      showDialog(PROGRESS_DIALOG);
      // create symlink
      // alias paths for storage dir (/sdcard or similar)
      String storagePathAlias = getFilesDir() + "/storage";
      String storagePath = Environment.getExternalStorageDirectory()
                           .getAbsolutePath();

      if (mExternalStorageAvailable) {
        if (mExternalStorageWriteable) {
          String pathAlias;
          String externalFilesDir = getExternalFilesDir(null).getAbsolutePath();
          String filesDir = getFilesDir().getAbsolutePath();

          // put the share files to externalFilesDir
          pathAlias = filesDir + "/share";
          mShareDir = externalFilesDir + "/share";
          new File(mShareDir).mkdir();
          makeSymlink(mShareDir, pathAlias);

          // put .qgis to externalFilesDir
          pathAlias = filesDir + "/.qgis2";
          mDotQgis2Dir = externalFilesDir + "/.qgis2";
          new File(mDotQgis2Dir).mkdir();
          makeSymlink(mDotQgis2Dir, pathAlias);
        } else {
          storagePathAlias = storagePathAlias + "ReadOnly";
        }
        makeSymlink(storagePath, storagePathAlias);
      }
    }

    protected void onProgressUpdate(Integer... progress) {
      super.onProgressUpdate(progress);
      mProgressDialog.setProgress(progress[0]);

      if (progress[0] >= 60) {
          mProgressDialog.setMessage(getString(R.string.unpacking_msg_following));
      }
    }

    protected void onPostExecute(String result) {
      SharedPreferences.Editor editor = mPrefs.edit();
      editor.putString("lastRunGitRevision", mThisRev);
      editor.commit();
      startQtActivity();
    }
  }
}
