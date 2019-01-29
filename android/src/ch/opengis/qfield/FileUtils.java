package ch.opengis.qfield;

import java.io.File;

import android.content.Context;
import android.net.Uri;
import android.database.Cursor;
import android.util.Log;
import android.app.Activity;
import android.content.ContentResolver;
import android.os.Environment;
import android.provider.DocumentsContract;

import ch.opengis.qfield.QFieldActivity;

class FileUtils{

    private static final String TAG = "QField Documents Provider";

    public static String getPathFromUri(Uri uri, ContentResolver resolver){

        Context context = QFieldActivity.getContext();
        Log.v(TAG, "Get path from Uri, authority: "+ uri.getAuthority());

        // If URI is a file URI
        if ("file".equalsIgnoreCase(uri.getScheme())){
            return uri.getPath();

        // Document opened by QgsDocumentsProvider
        }else if (uri.getAuthority().equals("ch.opengis.qfield.documents")){

            Cursor cursor = null;
            try {
                cursor = resolver.query(uri, null, null, null, null);
                if (cursor.moveToFirst()) {
                    return cursor.getString(0);
                }
            } catch (Exception e) {
                // Eat it
            } finally {
                cursor.close();
            }

        // Document opened by other providers (e.g. Android's "SD card" provider)
        }else if (uri.getAuthority().equals("com.android.externalstorage.documents")){
            final String docId = DocumentsContract.getDocumentId(uri);

            String idArr[] = docId.split(":");
            if(idArr.length == 2){
                String type = idArr[0];
                String realDocId = idArr[1];

                if("primary".equalsIgnoreCase(type)){
                    return Environment.getExternalStorageDirectory() + "/" + realDocId;
                }else{
                    return getExternalFilePath(context, idArr[1]);
                }
            }
        }

        return null;
    }

    // Try to guess the real file path, matching the path from the uri and the path of the
    // external dirs. It returns the first mathing result.
    public static String getExternalFilePath(Context context, String pathFromUri){
        File[] externalFilesDirs = context.getExternalFilesDirs(null);
        for (int i = 0; i < externalFilesDirs.length; i++){
            File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
            if(root.exists() && root.isDirectory()){
                File guess = new File(root, pathFromUri);
                Log.v(TAG, "Try "+i+", open file: "+guess.getPath());
                if (guess.exists()){
                    return guess.getPath();
                }
            }
        }
        return null;
    }
}
