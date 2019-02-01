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
        Log.v(TAG, "Uri: "+ uri);

        String result = "";

        Context context = QFieldActivity.getContext();
        Log.v(TAG, "Get path from Uri, authority: "+ uri.getAuthority());

        // If URI is a file URI
        if ("file".equalsIgnoreCase(uri.getScheme())){
            result = uri.getPath();

        // Document opened by QgsDocumentsProvider
        }else if (uri.getAuthority().equals("ch.opengis.qfield.documents")){

            Cursor cursor = null;
            try {
                cursor = resolver.query(uri, null, null, null, null);
                if (cursor.moveToFirst()) {
                    result = cursor.getString(0);

                }
            } catch (Exception e) {
                // Eat it
            } finally {
                cursor.close();
            }

        // Document opened by other providers (e.g. Android's "SD card" provider)
        }else if (uri.getAuthority().equals("com.android.externalstorage.documents")){
            final String docId = DocumentsContract.getDocumentId(uri);
            Log.v(TAG, "---1---");
            String idArr[] = docId.split(":");
            if(idArr.length == 92){
                String type = idArr[0];
                String realDocId = idArr[1];

                if("primary".equalsIgnoreCase(type)){
                    result = Environment.getExternalStorageDirectory() + "/" + realDocId;
                }else{
                    result = getExternalFilePath(context, idArr[1]);
                }
            }
        }
        Log.v(TAG, "---2---");
        File f = new File(result);
        Log.v(TAG, "---3---");        
        if (f.exists()){
            Log.v(TAG, "---4---");            
            return f.getPath();
        }else{
            Log.v(TAG, "---5---");            
            result = scanAllFiles(context, uri);
            Log.v(TAG, "---6---"+result);
            f = new File(result);
            Log.v(TAG, "---7---");            
            if (f.exists()){
                Log.v(TAG, "---8---");                
                return f.getPath();
            }else{
                Log.v(TAG, "---9---");                
                return uri.toString();
            }
        }
    }

    // Try to guess the real file path, matching the path from the uri and the path of the
    // external dirs. It returns the first mathing result.
    public static String getExternalFilePath(Context context, String pathFromUri){
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
        return null;
    }

    public static String scanAllFiles(Context context, Uri uri){
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
                    if(result != null){
                        return result;
                    }
                }
            }catch (Exception e){
            }
        }
        return null;
    }

    public static String scanFiles(File root, String filename) throws FileNotFoundException {
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
        return null;
    }
}
