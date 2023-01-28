package ch.opengis.qfield;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
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

public class QFieldFilePickerActivity extends Activity {
    private static final String TAG = "QField File Picker Activity";
    private static final int FILE_PICKER_ACTIVITY = 174;

    private String prefix;
    private String filePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        if (!intent.hasExtra("prefix") || !intent.hasExtra("filePath")) {
            Log.d(TAG, "QFieldFilePickerActivity missing extras");
            finish();
            return;
        }

        prefix = intent.getExtras().getString("prefix");
        filePath = intent.getExtras().getString("filePath");
        Log.d(TAG, "Received prefix: " + prefix +
                       " and resourceFilePath: " + filePath);

        callFilePickerIntent();

        return;
    }

    private void callFilePickerIntent() {
        Log.d(TAG, "callFilePickerIntent()");
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                        Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);
        intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
        intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, false);
        intent.setType("*/*");
        startActivityForResult(intent, FILE_PICKER_ACTIVITY);
        return;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "resultCode: " + resultCode);

        if (requestCode == FILE_PICKER_ACTIVITY) {
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
                intent.putExtra("FILENAME", prefix + finalFilePath);
                setResult(RESULT_OK, intent);
            } else {
                Intent intent = this.getIntent();
                intent.putExtra("FILENAME", "");
                setResult(RESULT_CANCELED, intent);
            }

            finish();
        }
    }
}
