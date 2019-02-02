package ch.opengis.qfield;

import java.io.File;
import java.io.FileNotFoundException;

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
        Log.v(TAG, "In getPathFromUri() ");
        Log.v(TAG, "Uri: "+ uri);
        Log.v(TAG, "Authority: " + uri.getAuthority());

        String result = "";

        // If URI is a file URI
        if ("file".equalsIgnoreCase(uri.getScheme())){
            Log.v(TAG, "In if file");
            result = uri.getPath();
            Log.v(TAG, "Result: " + result);

        // Document opened by QgsDocumentsProvider
        }else if (uri.getAuthority().equals("ch.opengis.qfield.documents")){
            Log.v(TAG, "In if qfield.documents");
            Cursor cursor = null;
            try {
                cursor = resolver.query(uri, null, null, null, null);
                if (cursor.moveToFirst()) {
                    result = cursor.getString(0);
                    Log.v(TAG, "Result: " + result);
                }
            } catch (Exception e) {
                Log.v(TAG, "Exception: " + e);
            } finally {
                cursor.close();
            }

       // Document opened by other providers (e.g. Android's "SD card" provider)
        }else if (uri.getAuthority().equals("com.android.externalstorage.documents")){
            Log.v(TAG, "In if externalstorage.documents");
            try{
                Context context = QFieldActivity.getContext();
                final String docId = DocumentsContract.getDocumentId(uri);
                Log.v(TAG, "docId: " + docId);
                String idArr[] = docId.split(":");
                if(idArr.length == 2){
                    String type = idArr[0];
                    String realDocId = idArr[1];

                    if("primary".equalsIgnoreCase(type)){
                        Log.v(TAG, "In if primary");
                        result = Environment.getExternalStorageDirectory() + "/" + realDocId;
                        Log.v(TAG, "Result: " + result);
                    }else{
                        Log.v(TAG, "Not primary");
                        result = getExternalFilePath(context, idArr[1]);
                        Log.v(TAG, "Result " + result);
                    }
                }
            }catch (Exception e){
                Log.v(TAG, "Exception: " + e);
            }
        }

        File f = new File(result);
        if (f.exists()){
            return f.getPath();
        }

        try{
            Log.v(TAG, "In last try");
            Context context = QFieldActivity.getContext();
            result = scanAllFiles(context, uri);
            Log.v(TAG, "Result: " + result);
            f = new File(result);
            if (f.exists()){
                return f.getPath();
            }
        }catch (Exception e){
            Log.v(TAG, "Exception: " + e);
        }
        return uri.toString();
    }

    // Try to guess the real file path, matching the path from the uri and the path of the
    // external dirs. It returns the first mathing result.
    public static String getExternalFilePath(Context context, String pathFromUri) throws Exception{
        File[] externalFilesDirs = context.getExternalFilesDirs(null);
        for (int i = 0; i < externalFilesDirs.length; i++){
            try{
                File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                if(root.exists() && root.isDirectory()){
                    File guess = new File(root, pathFromUri);
                    Log.v(TAG, "Try "+i+", open file: "+guess.getPath());
                    if (guess.exists()){
                        return guess.getPath();
                    }
                }
            }catch (Exception e){
            }
        }
        return "";
    }

    public static String scanAllFiles(Context context, Uri uri) throws Exception{
        Log.v(TAG, "Scan all files ");
        Log.v(TAG, "Uri: " + uri.toString());
        String[] split1 = uri.toString().split("/");
        String last1 = split1[split1.length-1];
        Log.v(TAG, "Last1 : " + last1);
        String[] split2 = last1.split("%2F");
        String filename = split2[split2.length-1];
        Log.v(TAG, "filename : " + filename);

        File[] externalFilesDirs = context.getExternalFilesDirs(null);
        for (int i = 0; i < externalFilesDirs.length; i++){
            try{
                File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                if (root.exists() && root.isDirectory()){
                    String rootPath= root.getPath();
                    String result = scanFiles(new File(rootPath), filename);
                    if(!result.equals("")){
                        return result;
                    }
                }
            }catch (Exception e){
            }
        }
        return "";
    }

    public static String scanFiles(File root, String filename) throws Exception {
        File[] fileArray = root.listFiles();
        for (File f : fileArray){
            if (f.isDirectory()){
                // Avoid to scan big directory where is very unlikely to find qgs projects
                if (! (f.isHidden() ||
                       f.getPath().contains("DCIM") ||
                       f.getPath().contains("Pictures") ||
                       f.getPath().contains("Movies") ||
                       f.getPath().contains("Books") ||
                       f.getPath().contains("MyImages") ||
                       f.getPath().contains("Playlists") ||
                       f.getPath().contains("Podcasts") ||
                       f.getPath().contains("Sounds") ||
                       f.getPath().contains("Music") ||
                       f.getPath().contains("WhatsApp")
                       )) {
                    return scanFiles(f, filename);
                }
            }else if (f.isFile()){
                if (f.getName().equals(filename)){
                    Log.v(TAG, "Found! "+f.getPath());                    
                    return f.getPath();
                }
            }
        }
        return "";
    }
}
