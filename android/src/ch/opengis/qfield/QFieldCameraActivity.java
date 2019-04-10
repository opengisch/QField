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
import android.app.Dialog;
import android.app.AlertDialog;
import android.app.Activity;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.Context;
import android.content.Intent;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.LayoutInflater;
import android.util.Log;
import android.graphics.drawable.ColorDrawable;
import android.provider.MediaStore;
import android.graphics.Bitmap;
import android.support.v4.content.FileProvider;

public class QFieldCameraActivity extends Activity{
    private static final String TAG = "QField Camera Activity";
    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;
    private String prefix;
    private File cameraFile;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        prefix = getIntent().getExtras().getString("prefix");
        Log.d(TAG, "prefix: "+prefix);
        
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
        
        cameraFile = File.createTempFile(imageFileName,  /* prefix */
                                         ".jpg",         /* suffix */
                                         getCacheDir()      /* directory */
                                         );

        // Save a file: path for use with ACTION_VIEW intents
        currentPhotoPath = cameraFile.getAbsolutePath();
        Log.d(TAG, "currentPhotoPath: "+currentPhotoPath);
        return cameraFile;
    }
    
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult()");
        Log.d(TAG, "request: "+requestCode);
        Log.d(TAG, "resultCode: "+resultCode);
        if (requestCode == 172) {  // && resultCode == RESULT_OK) {

            Log.d(TAG, "tmp exists: "+cameraFile.exists());
            Log.d(TAG, "tmp path: "+cameraFile.getAbsolutePath());
            try{
                copyFile(cameraFile, new File(prefix, cameraFile.getName()));
            }catch(IOException e){
            }
            
            // TODO: after copy, verify if is correctly copied and then remove the old one
            
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
