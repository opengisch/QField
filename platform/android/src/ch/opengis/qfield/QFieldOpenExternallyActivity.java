package ch.opengis.qfield;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Toast;
import androidx.core.content.FileProvider;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class QFieldOpenExternallyActivity extends Activity {
    private static final String TAG = "QField Open (file) Externally Activity";
    private File file;
    private File cacheFile;
    private boolean isEditing;
    private String errorMessage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        String filePath = getIntent().getExtras().getString("filepath");
        String mimeType = getIntent().getExtras().getString("filetype");
        isEditing = getIntent()
                        .getExtras()
                        .getString("fileediting")
                        .compareTo("true") == 0;
        Log.d(TAG,
              "Received filepath: " + filePath + " and mimeType: " + mimeType);

        file = new File(filePath);
        cacheFile = new File(getCacheDir(), file.getName());

        // copy file to a temporary file
        try {
            copyFile(file, cacheFile);
        } catch (IOException e) {
            Log.d(TAG, e.getMessage());
            finish();
        }

        Uri contentUri =
            Build.VERSION.SDK_INT < 24
                ? Uri.fromFile(file)
                : FileProvider.getUriForFile(
                      this, BuildConfig.APPLICATION_ID + ".fileprovider",
                      cacheFile);

        Log.d(TAG, "content URI: " + contentUri);
        Log.d(TAG, isEditing ? "call ACTION_EDIT intent"
                             : "call ACTION_VIEW intent");
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
            startActivityForResult(intent, 102);
        } catch (IllegalArgumentException e) {
            Log.d(TAG, e.getMessage());
            errorMessage = e.getMessage();
        } catch (Exception e) {
            Log.d(TAG, e.getMessage());
            errorMessage = e.getMessage();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        // on ACTION_VIEW back key pressed it returns RESULT_CANCEL - on error
        // as well
        if (resultCode == RESULT_OK) {
            try {
                if (isEditing) {
                    Log.d(TAG, "Copy file back from uri " +
                                   data.getDataString() +
                                   " to file: " + file.getAbsolutePath());
                    InputStream in =
                        getContentResolver().openInputStream(data.getData());
                    OutputStream out = new FileOutputStream(file);
                    // Transfer bytes from in to out
                    byte[] buf = new byte[1024];
                    int len;
                    while ((len = in.read(buf)) > 0) {
                        out.write(buf, 0, len);
                    }
                    out.close();
                }
                Intent intent = this.getIntent();
                setResult(RESULT_OK, intent);
            } catch (SecurityException e) {
                Intent intent = this.getIntent();
                intent.putExtra("ERROR_MESSAGE", e.getMessage());
                setResult(RESULT_CANCELED, intent);
            } catch (IOException e) {
                Intent intent = this.getIntent();
                intent.putExtra("ERROR_MESSAGE", e.getMessage());
                setResult(RESULT_CANCELED, intent);
            }
        } else {
            Intent intent = this.getIntent();
            intent.putExtra("ERROR_MESSAGE", errorMessage);
            setResult(RESULT_CANCELED, intent);
        }

        finish();
    }

    private void copyFile(File src, File dst) throws IOException {
        Log.d(TAG, "Copy file: " + src.getAbsolutePath() +
                       " to file: " + dst.getAbsolutePath());
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
