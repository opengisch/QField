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
import androidx.documentfile.provider.DocumentFile;
import ch.opengis.qfield.QFieldUtils;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class QFieldGalleryActivity extends Activity {
    private static final String TAG = "QField Gallery Activity";
    private static final int GALLERY_ACTIVITY = 173;

    private String prefix;
    private String filePath;
    private String mimeType;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        if (!intent.hasExtra("prefix") || !intent.hasExtra("filePath") ||
            !intent.hasExtra("mimeType")) {
            Log.d(TAG, "QFieldGalleryActivity missing extras");
            finish();
            return;
        }

        prefix = intent.getExtras().getString("prefix");
        filePath = intent.getExtras().getString("filePath");
        mimeType = intent.getExtras().getString("mimeType");

        Log.d(TAG, "Received prefix: " + prefix + " and filePath: " + filePath);

        callGalleryIntent();

        return;
    }

    private void callGalleryIntent() {
        Log.d(TAG, "callGalleryIntent()");
        Intent intent = new Intent();
        intent.setType(mimeType);
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(intent, GALLERY_ACTIVITY);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "resultCode: " + resultCode);

        if (requestCode == GALLERY_ACTIVITY) {
            if (resultCode == RESULT_OK) {
                Uri uri = data.getData();
                DocumentFile documentFile = DocumentFile.fromSingleUri(
                    getApplication().getApplicationContext(), uri);
                String finalFilePath = QFieldUtils.replaceFilenameTags(
                    filePath, documentFile.getName());
                File result = new File(prefix + finalFilePath);
                Log.d(TAG, "Selected file: " + data.getData().toString());
                try {
                    InputStream in = getContentResolver().openInputStream(uri);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  documentFile.length());
                } catch (Exception e) {
                    Log.d(TAG, e.getMessage());
                }

                Intent intent = this.getIntent();
                intent.putExtra("MEDIA_FILENAME", prefix + finalFilePath);
                setResult(RESULT_OK, intent);
            } else {
                Intent intent = this.getIntent();
                intent.putExtra("MEDIA_FILENAME", "");
                setResult(RESULT_CANCELED, intent);
            }

            finish();
        }
    }
}
