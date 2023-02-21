package ch.opengis.qfield;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import androidx.core.content.FileProvider;
import ch.opengis.qfield.QFieldUtils;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class QFieldCameraActivity extends Activity {
    private static final String TAG = "QField Camera Activity";
    private static final int CAMERA_ACTIVITY = 172;

    private Boolean isVideo = false;
    private String prefix;
    private String filePath;
    private String suffix;
    private String tempFileName;
    private String tempFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        if (!intent.hasExtra("prefix") || !intent.hasExtra("suffix") ||
            !intent.hasExtra("filePath")) {
            Log.d(TAG, "QFieldCameraActivity missing extras");
            finish();
            return;
        }

        if (intent.hasExtra("isVideo")) {
            Log.d(TAG, "Video requested");
            isVideo = true;
        }

        prefix = intent.getExtras().getString("prefix");
        filePath = intent.getExtras().getString("filePath");
        suffix = intent.getExtras().getString("suffix");
        Log.d(TAG, "Received prefix: " + prefix + " and filePath: " + filePath +
                       "and suffix: " + suffix);

        String timeStamp =
            new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        tempFileName = "QFieldCamera" + timeStamp;
        Log.d(TAG, "Created tempFileName: " + tempFileName);

        callCameraIntent();

        return;
    }

    private void callCameraIntent() {
        Log.d(TAG, "callCameraIntent()");
        Intent cameraIntent = isVideo
                                  ? new Intent(MediaStore.ACTION_VIDEO_CAPTURE)
                                  : new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (cameraIntent.resolveActivity(getPackageManager()) != null) {
            Log.d(TAG, "intent resolved");
            File storageDir =
                getExternalFilesDir(Environment.DIRECTORY_PICTURES);
            try {
                File tempFile =
                    File.createTempFile(tempFileName, suffix, storageDir);

                if (tempFile != null) {
                    Log.d(TAG, "temporary file created");
                    if (tempFile.exists()) {
                        Log.d(TAG, "temporary file exists");
                    }

                    tempFilePath = tempFile.getAbsolutePath();

                    Uri fileURI = FileProvider.getUriForFile(
                        this, "ch.opengis.qfield.fileprovider", tempFile);

                    Log.d(TAG, "uri: " + fileURI.toString());
                    cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, fileURI);
                    startActivityForResult(cameraIntent, CAMERA_ACTIVITY);
                }
            } catch (IOException e) {
                Log.d(TAG, e.getMessage());
            }
        } else {
            Log.d(TAG, "Could not resolve intent...");
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "resultCode: " + resultCode);

        if (requestCode == CAMERA_ACTIVITY) {
            if (resultCode == RESULT_OK) {
                File file = new File(tempFilePath);
                String finalFilePath =
                    QFieldUtils.replaceFilenameTags(filePath, file.getName());
                File result = new File(prefix + finalFilePath);
                Log.d(TAG, "Taken media: " + file.getAbsolutePath());
                try {
                    InputStream in = new FileInputStream(file);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  file.length());
                } catch (Exception e) {
                    e.printStackTrace();
                }

                Intent intent = this.getIntent();
                intent.putExtra("RESOURCE_FILENAME", prefix + finalFilePath);
                setResult(RESULT_OK, intent);
            } else {
                Intent intent = this.getIntent();
                intent.putExtra("RESOURCE_FILENAME", "");
                setResult(RESULT_CANCELED, intent);
            }
            finish();
        }
    }
}
