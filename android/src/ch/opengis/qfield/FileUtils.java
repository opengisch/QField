package ch.opengis.qfield;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;

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
        Log.d(TAG, "*** In getPathFromUri() ");
        Log.d(TAG, "Uri: " + uri);
        Log.d(TAG, "Authority: " + uri.getAuthority());
        Log.d(TAG, "Scheme: " + uri.getScheme());

        String result = null;

        // All methods return a real file path or null if no file is found and they
        // print a log of all variables and possibly the exceptions

        result = getPathFromFileUri(uri, resolver);
        if (result != null){
            return result;
        }

        result = getPathFromQFieldUri(uri, resolver);
        if (result != null){
            return result;
        }

        result = getPathFromExternalStorageDirectoryUri(uri, resolver);
        if (result != null){
            return result;
        }

        result = getPathFromAllExternalFilesDirectoriesWithDocId(uri, resolver);
        if (result != null){
            return result;
        }

        result = getPathFromAllExternalFilesDirectoriesWithoutDocId(uri, resolver);
        if (result != null){
            return result;
        }

        result = getPathFromScanningAllDirectories(uri, resolver);
        if (result != null){
            return result;
        }

        // Will cause an error but at least QField will show the uri to the user
        return uri.toString();
    }

    private static String getPathFromFileUri(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromFileUri");

            String result;

            if ("file".equalsIgnoreCase(uri.getScheme())){
                result = uri.getPath();
                Log.d(TAG, "Uri.getPath(): " + result);
                File f = new File(result);
                if (f.exists()){
                    Log.d(TAG, "Found: " + f.getPath());
                    return f.getPath();
                }
            }
        }catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String getPathFromQFieldUri(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromQFieldUri");

            String result;
            Cursor cursor;

            if (uri.getAuthority().equals("ch.opengis.qfield.documents")){
                cursor = resolver.query(uri, null, null, null, null);
                if (cursor.moveToFirst()) {
                    result = cursor.getString(0);
                    Log.d(TAG, "cursor.getString(0): " + result);
                    File f = new File(result);
                    cursor.close();
                    if (f.exists()){
                        Log.d(TAG, "Found: " + f.getPath());
                        return f.getPath();
                    }
                }
            }
        } catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String getPathFromExternalStorageDirectoryUri(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromExternalStorageDirectoryUri");

            String result;
            Cursor cursor;

            if (uri.getAuthority().equals("com.android.externalstorage.documents")){
                final String docId = DocumentsContract.getDocumentId(uri);
                Log.d(TAG, "docId: " + docId);
                String idArr[] = docId.split(":");
                if(idArr.length == 2){
                    String type = idArr[0];
                    String realDocId = idArr[1];

                    if("primary".equalsIgnoreCase(type)){
                        Log.d(TAG, "Is primary");
                        String externalStorageDirectory = Environment.getExternalStorageDirectory().getPath();
                        Log.d(TAG, "externalStorageDirectory: " + externalStorageDirectory);
                        result = externalStorageDirectory + "/" + realDocId;

                        File f = new File(result);
                        if (f.exists()){
                            Log.d(TAG, "Found: " + f.getPath());
                            return f.getPath();
                        }
                    }
                }
            }

        } catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String getPathFromAllExternalFilesDirectoriesWithDocId(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromExternalFilesDirectoriesWithDocId");
            Context context = QFieldActivity.getContext();
            final String docId = DocumentsContract.getDocumentId(uri);
            Log.d(TAG, "docId: " + docId);
            String idArr[] = docId.split(":");
            if(idArr.length == 2){
                String type = idArr[0];
                String realDocId = idArr[1];

                File[] externalFilesDirs = context.getExternalFilesDirs(null);
                Log.d(TAG, "External Files Dirs: " + Arrays.toString(externalFilesDirs));
                for (int i = 0; i < externalFilesDirs.length; i++){
                    Log.d(TAG, "External Files Dirs[" + i +"]: " + externalFilesDirs[i].getPath());
                    try{
                        File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                        if(root.exists() && root.isDirectory()){
                            File result = new File(root, realDocId);
                            Log.d(TAG, "result: "+result.getPath());
                            if (result.exists()){
                                Log.d(TAG, "Found: " + result.getPath());
                                return result.getPath();
                            }
                        }
                    }catch (Exception e){
                        Log.d(TAG, "Exception (internal): " + e);
                    }
                }
            }
        } catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String getPathFromAllExternalFilesDirectoriesWithoutDocId(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromExternalFilesDirectoriesWithoutDocId");
            Context context = QFieldActivity.getContext();
            String decodedUri = Uri.decode(uri.toString());
            Log.d(TAG, "decodedUri: " + decodedUri);
            String[] split = decodedUri.split(":");
            String partialPath = split[split.length-1];

            File[] externalFilesDirs = context.getExternalFilesDirs(null);
            Log.d(TAG, "External Files Dirs: " + Arrays.toString(externalFilesDirs));
            for (int i = 0; i < externalFilesDirs.length; i++){
                try{
                    Log.d(TAG, "External Files Dirs[" + i +"]: " + externalFilesDirs[i].getPath());
                    File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                    if(root.exists() && root.isDirectory()){
                        File result = new File(root, partialPath);
                        Log.d(TAG, "result: "+result.getPath());
                        if (result.exists()){
                            Log.d(TAG, "Found: " + result.getPath());
                            return result.getPath();
                        }
                    }
                }catch (Exception e){
                    Log.d(TAG, "Exception (internal): " + e);
                }
            }
        } catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String getPathFromScanningAllDirectories(Uri uri, ContentResolver resolver){
        try{
            Log.d(TAG, "** In getPathFromScanningAllDirectories");

            Context context = QFieldActivity.getContext();
            String[] split1 = uri.toString().split("/");
            String last1 = split1[split1.length-1];

            String[] split2 = last1.split("%2F");
            String filename = split2[split2.length-1];
            Log.d(TAG, "filename : " + filename);

            File[] externalFilesDirs = context.getExternalFilesDirs(null);
            Log.d(TAG, "External Files Dirs: " + Arrays.toString(externalFilesDirs));
            for (int i = 0; i < externalFilesDirs.length; i++){
                try{
                    Log.d(TAG, "External Files Dirs[" + i +"]: " + externalFilesDirs[i].getPath());
                    File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                    if (root.exists() && root.isDirectory()){
                        String rootPath= root.getPath();
                        String result = scanFiles(new File(rootPath), filename);
                        if(!result.equals("")){
                            return result;
                        }
                    }
                }catch (Exception e){
                    Log.d(TAG, "Exception: " + e);
                }
            }
        } catch (Exception e){
            Log.d(TAG, "Exception: " + e);
        }
        return null;
    }

    private static String scanFiles(File root, String filename) throws Exception {
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
                    Log.d(TAG, "Found! "+f.getPath());
                    return f.getPath();
                }
            }
        }
        return "";
    }
}
