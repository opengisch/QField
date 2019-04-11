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

public class QFieldCameraActivity extends Activity{
    private static final String TAG = "QField Camera Activity";
    private String prefix;
    private File cameraFile;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        prefix = getIntent().getExtras().getString("prefix");
        Log.d(TAG, "Received prefix: "+prefix);
        
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {

            File photoFile = null;
            try {
                photoFile = createImageFile(prefix);
            } catch (IOException ex) {
                // Error occurred while creating the File
            }
            // Continue only if the File was successfully created
            if (photoFile != null) {
                Uri photoURI = FileProvider.getUriForFile(this,
                                                          "ch.opengis.qfield.fileprovider",
                                                          photoFile);
                
                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
                startActivityForResult(takePictureIntent, 172);
            }
        }

        return;
    }

    private File createImageFile(String prefix) throws IOException {
        // Create an image file name
        String currentPhotoPath;
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";
        
        cameraFile = File.createTempFile(imageFileName,
                                         ".jpg",
                                         getCacheDir());

        return cameraFile;
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "resultCode: "+resultCode);

        if (requestCode == 172) {
            if (resultCode == RESULT_OK) {

                Log.d(TAG, "Taken picture: "+cameraFile.getAbsolutePath());

                File result = new File(prefix, cameraFile.getName());
                try{
                    copyFile(cameraFile, result);
                }catch(IOException e){
                }
            
                Intent intent = this.getIntent();
                intent.putExtra("CAMERA_IMAGE_FILENAME", result.getName());
                setResult(Activity.RESULT_OK, intent);

            } else {
                Intent intent = this.getIntent();
                intent.putExtra("CAMERA_IMAGE_FILENAME", "");
                setResult(Activity.RESULT_CANCELED, intent);
            }
            finish();
        }
    }

    private void copyFile(File src, File dst) throws IOException {
        Log.d(TAG, "Copied file: "+src.getAbsolutePath()+" to file: "+dst.getAbsolutePath());
        try (InputStream in = new FileInputStream(src)) {
            try (OutputStream out = new FileOutputStream(dst)) {
                // Transfer bytes from in to out
                byte[] buf = new byte[1024];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }
            }
        }
    }    

}
