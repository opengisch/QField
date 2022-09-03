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

public class QFieldGalleryPictureActivity extends Activity {
    private static final String TAG = "QField Gallery Picture Activity";
    private static final int GALLERY_ACTIVITY = 173;

    private String prefix;
    private String pictureFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        if (!intent.hasExtra("prefix") || !intent.hasExtra("pictureFilePath")) {
            Log.d(TAG, "QFieldGalleryPictureActivity missing extras");
            finish();
            return;
        }

        prefix = intent.getExtras().getString("prefix");
        pictureFilePath = intent.getExtras().getString("pictureFilePath");
        Log.d(TAG, "Received prefix: " + prefix +
                       " and pictureFilePath: " + pictureFilePath);

        callGalleryIntent();

        return;
    }

    private void callGalleryIntent() {
        Log.d(TAG, "callGalleryIntent()");
        Intent intent = new Intent();
        intent.setType("image/*");
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
                File result = new File(prefix + pictureFilePath);
                File path = result.getParentFile();
                path.mkdirs();
                path.setExecutable(true);
                path.setReadable(true);
                path.setWritable(true);

                Log.d(TAG, "Selected picture: " + data.getData().toString());
                try {
                    Uri uri = data.getData();
                    DocumentFile documentFile = DocumentFile.fromSingleUri(
                        getApplication().getApplicationContext(), uri);
                    InputStream in = getContentResolver().openInputStream(uri);
                    QFieldUtils.inputStreamToFile(in, result.getPath(),
                                                  documentFile.length());
                } catch (Exception e) {
                    Log.d(TAG, e.getMessage());
                }

                // Let the android scan new media folders/files to make them
                // visible through MTP
                result.setReadable(true);
                result.setWritable(true);
                MediaScannerConnection.scanFile(
                    this, new String[] {path.toString()}, null, null);

                Intent intent = this.getIntent();
                intent.putExtra("PICTURE_IMAGE_FILENAME",
                                prefix + pictureFilePath);
                setResult(RESULT_OK, intent);
            } else {
                Intent intent = this.getIntent();
                intent.putExtra("PICTURE_IMAGE_FILENAME", "");
                setResult(RESULT_CANCELED, intent);
            }

            finish();
        }
    }
}
