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

public class QFieldGalleryPictureActivity extends Activity{
    private static final String TAG = "QField Gallery Picture Activity";
    private String prefix;
    private String pictureFileName;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        prefix = getIntent().getExtras().getString("prefix");
        Log.d(TAG, "Received prefix: " + prefix);

        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        pictureFileName = "JPEG_" + timeStamp + ".jpg";
        callGalleryIntent();

        return;
    }

    private void callGalleryIntent() {
        Log.d(TAG, "callGalleryIntent()");
        Intent intent = new Intent();
        intent.setType("image/jpeg");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(intent, 173);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "resultCode: "+resultCode);

        File result = new File(prefix, pictureFileName);

        if (resultCode == RESULT_OK) {

            Log.d(TAG, "Selected picture: " + data.getData().toString());
            try{
                copyUriContentToFile(data.getData(), result);
            }catch(IOException e){
                Log.d(TAG, e.getMessage());
            }

            Intent intent = this.getIntent();
            intent.putExtra("PICTURE_IMAGE_FILENAME", pictureFileName);
            setResult(RESULT_OK, intent);
        } else {
            Intent intent = this.getIntent();
            intent.putExtra("PICTURE_IMAGE_FILENAME", "");
            setResult(RESULT_CANCELED, intent);
        }

        finish();
    }

    private void copyUriContentToFile(Uri src, File dst) throws IOException {
        Log.d(TAG, "Copy Uri content: "+src.toString()+" to file: "+dst.getAbsolutePath());

        try (InputStream in = getContentResolver().openInputStream(src)) {
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
