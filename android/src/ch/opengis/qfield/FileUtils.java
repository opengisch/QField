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

    private static final String TAG = "MyCloudProvider";

    public static String getPathFromUri(Uri uri, ContentResolver resolver){

        Context context = QFieldActivity.getContext();

        // Document opened by QgsDocumentsProvider
        if (uri.getAuthority().equals("ch.opengis.qfield.documents")){
            Cursor cursor = null;
            try {
                cursor = resolver.query(uri, null, null, null, null);
                if (cursor.moveToFirst()) {
                    return cursor.getString(0);
                }
            } catch (Exception e) {
                // Eat it
            }

            // Document opened by other providers (e.g. Android's "SD card" provider)
        }else if (uri.getAuthority().equals("com.android.externalstorage.documents")){
            final String docId = DocumentsContract.getDocumentId(uri);

            final String[] split = docId.split(":");
            final String type = split[0];

            return getExternalFilePath(context, split[1]);
        }

        return null;
    }

    // Try to guess the real file path, matching the path from the uri and the path of the
    // external dirs. It returns the first mathing result.
    public static String getExternalFilePath(Context context, String pathFromUri){
        File[] externalFilesDirs = context.getExternalFilesDirs(null);
        for (int i = 0; i < externalFilesDirs.length; i++){
            File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
            File guess = new File(root, pathFromUri);
            if (guess.exists()){
                return guess.getPath();
            }
        }
        return null;
    }
}
