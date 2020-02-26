package ch.opengis.qfield;

import java.io.File;

import android.os.Bundle;
import android.os.Build;
import android.os.Environment;
import android.net.Uri;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.support.v4.content.FileProvider;

import java.io.IOException;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.util.Date;
import java.text.SimpleDateFormat;

public class QFieldOpenExternallyActivity extends Activity{
    private static final String TAG = "QField Open (file) Externally Activity";
    private String filePath;
    private String mimeType;
    private String tempFileName;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        filePath = getIntent().getExtras().getString("filepath");
        Log.d(TAG, "Received filepath: " + filePath);
        mimeType = getIntent().getExtras().getString("filetype");
        Log.d(TAG, "Received mimeType: " + mimeType);

        File file = new File(filePath);
        tempFileName = "JPEG_" + new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date()) + ".jpg";
        File cacheFile = new File(getCacheDir(), tempFileName);
        //copy file to a temporary file
        try{
            copyFile( file, cacheFile );
        }catch(IOException e){
            Log.d(TAG, e.getMessage());
        }

        Uri contentUri =  Build.VERSION.SDK_INT < 24 ? Uri.fromFile(file) : FileProvider.getUriForFile( this, BuildConfig.APPLICATION_ID+".fileprovider", cacheFile );

        Log.d(TAG, "content URI: " + contentUri);
        Log.d(TAG, "call ACTION_VIEW intent");
        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_VIEW);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        intent.setDataAndType(contentUri, mimeType);
        startActivityForResult(intent, 102);

        finish();
    }

    private void copyFile(File src, File dst) throws IOException {
        Log.d(TAG, "Copy file: "+src.getAbsolutePath()+" to file: "+dst.getAbsolutePath());
        try (InputStream in = new FileInputStream(src)) {
            try (OutputStream out = new FileOutputStream(dst)) {
                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
                out.close();
            }
        }
    }
}
