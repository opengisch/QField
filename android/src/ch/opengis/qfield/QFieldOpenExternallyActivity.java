package ch.opengis.qfield;

import java.text.SimpleDateFormat;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.util.Date;

import android.os.Bundle;
import android.os.Environment;
import android.net.Uri;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.provider.MediaStore;
import android.graphics.Bitmap;
import android.support.v4.content.FileProvider;

public class QFieldOpenExternallyActivity extends Activity{
    private static final String TAG = "QField Open (file) Externally Activity";
    private String filePath;
    private String mimeType;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        filePath = getIntent().getExtras().getString("filepath");
        Log.d(TAG, "Received filepath: " + filePath);
        mimeType = getIntent().getExtras().getString("filetype");
        Log.d(TAG, "Received mimeType: " + mimeType);

        File file = new File(filePath);
        Uri contentUri = FileProvider.getUriForFile( this, BuildConfig.APPLICATION_ID+".fileprovider", file );

        Log.d(TAG, "call ACTION_VIEW intent");
        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_VIEW);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.setDataAndType(contentUri, mimeType);
        startActivityForResult(intent, 102);

        finish();
    }
}
