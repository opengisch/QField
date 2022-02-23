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
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class QFieldCameraPictureActivity extends Activity {
    private static final String TAG = "QField Camera Picture Activity";
    private String prefix;
    private String pictureFilePath;
    private String suffix;
    private String pictureTempFileName;
    private String pictureTempFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        Intent intent = getIntent();
        if (!intent.hasExtra("prefix") || !intent.hasExtra("suffix") ||
            !intent.hasExtra("pictureFilePath")) {
            Log.d(TAG, "QFieldCameraPictureActivity missing extras");
            finish();
            return;
        }

        prefix = intent.getExtras().getString("prefix");
        pictureFilePath = intent.getExtras().getString("pictureFilePath");
        suffix = intent.getExtras().getString("suffix");
        Log.d(TAG, "Received prefix: " + prefix + " and pictureFilePath: " +
                       pictureFilePath + "and suffix: " + suffix);

        String timeStamp =
            new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        pictureTempFileName = "QFieldPicture" + timeStamp;
        Log.d(TAG, "Created pictureTempFileName: " + pictureTempFileName);

        callCameraIntent();

        return;
    }

    private void callCameraIntent() {
        Log.d(TAG, "callCameraIntent()");
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            Log.d(TAG, "intent resolved");
            File storageDir =
                getExternalFilesDir(Environment.DIRECTORY_PICTURES);
            try {
                File pictureFile = File.createTempFile(pictureTempFileName,
                                                       suffix, storageDir);

                if (pictureFile != null) {
                    Log.d(TAG, "picture file created");
                    if (pictureFile.exists()) {
                        Log.d(TAG, "picture file exists");
                    }

                    pictureTempFilePath = pictureFile.getAbsolutePath();

                    Uri photoURI = FileProvider.getUriForFile(
                        this, "ch.opengis.qfield.fileprovider", pictureFile);

                    Log.d(TAG, "uri: " + photoURI.toString());
                    takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
                                               photoURI);
                    startActivityForResult(takePictureIntent, 172);
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

        File result = new File(prefix + pictureFilePath);
        File path = result.getParentFile();
        path.mkdirs();

        // Let the android scan new media folders/files to make them visible
        // through MTP
        path.setExecutable(true);
        path.setReadable(true);
        path.setWritable(true);
        MediaScannerConnection.scanFile(this, new String[] {path.toString()},
                                        null, null);

        if (resultCode == RESULT_OK) {

            File pictureFile = new File(pictureTempFilePath);
            Log.d(TAG, "Taken picture: " + pictureFile.getAbsolutePath());
            try {
                copyFile(pictureFile, result);
            } catch (IOException e) {
                Log.d(TAG, e.getMessage());
            }

            Intent intent = this.getIntent();
            intent.putExtra("PICTURE_IMAGE_FILENAME", prefix + pictureFilePath);
            setResult(RESULT_OK, intent);
        } else {
            Intent intent = this.getIntent();
            intent.putExtra("PICTURE_IMAGE_FILENAME", "");
            setResult(RESULT_CANCELED, intent);
        }

        // Let the android scan new media folders/files to make them visible
        // through MTP
        result.setReadable(true);
        result.setWritable(true);
        MediaScannerConnection.scanFile(this, new String[] {result.toString()},
                                        null, null);

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
