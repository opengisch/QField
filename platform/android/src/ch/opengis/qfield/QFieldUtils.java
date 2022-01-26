/***************************************************************************
                            QFieldUtils.java
                            -------------------
              begin                : December 6, 2020
              copyright            : (C) 2020 by Mathieu Pellerin
              email                : nirvn dot asia at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

package ch.opengis.qfield;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;
import androidx.documentfile.provider.DocumentFile;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class QFieldUtils {

    public static String getArchiveProjectName(InputStream in) {
        String projectName = "";
        try {
            ZipInputStream zin = new ZipInputStream(in);
            ZipEntry entry;
            while ((entry = zin.getNextEntry()) != null) {
                String entryName = entry.getName().toLowerCase();
                if (entryName.endsWith(".qgs") || entryName.endsWith(".qgz")) {
                    projectName = entry.getName();
                    break;
                }
            }
            zin.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return projectName;
    }

    public static boolean documentFileToFolder(DocumentFile directory,
                                               String folder,
                                               ContentResolver resolver) {
        DocumentFile[] files = directory.listFiles();
        for (DocumentFile file : files) {
            if (file.isDirectory()) {
                String directoryPath = folder + file.getName() + "/";
                new File(directoryPath).mkdir();
                boolean success =
                    documentFileToFolder(file, directoryPath, resolver);
                if (!success) {
                    return false;
                }
            } else {
                String filePath = folder + file.getName();
                try {
                    InputStream input = resolver.openInputStream(file.getUri());
                    QFieldUtils.inputStreamToFile(input, filePath);
                } catch (Exception e) {
                    e.printStackTrace();
                    return false;
                }
            }
        }
        return true;
    }

    public static boolean inputStreamToFolder(InputStream in, String folder) {
        try {
            ZipInputStream zin = new ZipInputStream(in);
            ZipEntry entry;
            while ((entry = zin.getNextEntry()) != null) {
                if (entry.isDirectory()) {
                    new File(folder + entry.getName()).mkdirs();
                    continue;
                }
                OutputStream out =
                    new FileOutputStream(new File(folder + entry.getName()));
                int size = 0;
                byte[] buffer = new byte[1024];
                while ((size = zin.read(buffer, 0, buffer.length)) != -1) {
                    out.write(buffer, 0, size);
                }
                out.close();
            }
            zin.close();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static boolean inputStreamToFile(InputStream in, String file) {
        try {
            OutputStream out = new FileOutputStream(new File(file));

            int size = 0;
            byte[] buffer = new byte[1024];

            while ((size = in.read(buffer)) != -1) {
                out.write(buffer, 0, size);
            }

            out.close();
        } catch (Exception e) {
            Log.e("QField", "inputStreamToFile exception: " + e.getMessage());
            return false;
        }
        return true;
    }

    public static boolean deleteDirectory(File file, boolean recursive) {
        if (!file.isDirectory()) {
            return false;
        }
        File[] files = file.listFiles();
        for (File f : files) {
            boolean success = true;
            if (f.isDirectory()) {
                if (recursive) {
                    success = deleteDirectory(f, true);
                }
            } else {
                success = f.delete();
            }
            if (!success) {
                return false;
            }
        }
        return true;
    }

    public static String getExtensionFromMimeType(String type) {
        if (type.equals("application/pdf")) {
            return "pdf";
        } else if (type.equals("application/vnd.sqlite3") ||
                   type.equals("application/x-sqlite3")) {
            return "db";
        } else if (type.equals("application/geopackage+sqlite3")) {
            return "gpkg";
        } else if (type.equals("application/vnd.geo+json") ||
                   type.equals("application/geo+json")) {
            return "geojson";
        } else if (type.equals("application/gpx+xml")) {
            return "gpx";
        } else if (type.equals("application/vnd.google-earth.kml+xml")) {
            return "kml";
        } else if (type.equals("application/vnd.google-earth.kmz")) {
            return "kmz";
        } else if (type.equals("application/zip")) {
            return "zip";
        } else if (type.equals("image/tiff")) {
            return "tif";
        } else if (type.equals("image/x-jp2")) {
            return "jp2";
        }
        return "";
    }

    // original script by SANJAY GUPTA
    // (https://stackoverflow.com/questions/17546101/get-real-path-for-uri-android)
    public static String getPathFromUri(final Context context, final Uri uri) {
        final boolean isKitKat =
            Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;
        String path = null;

        if (isKitKat && DocumentsContract.isDocumentUri(context, uri)) {
            // DocumentProvider
            if (isExternalStorageDocument(uri)) {
                // ExternalStorageProvider
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" +
                        split[1];
                }
                // TODO handle non-primary volumes
            } else if (isDownloadsDocument(uri)) {
                // DownloadsProvider
                final String id = DocumentsContract.getDocumentId(uri);
                final Uri contentUri = ContentUris.withAppendedId(
                    Uri.parse("content://downloads/public_downloads"),
                    Long.valueOf(id));

                path = getDataColumn(context, contentUri, null, null);
            } else if (isMediaDocument(uri)) {
                // MediaProvider
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                Uri contentUri = null;
                if ("image".equals(type)) {
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }

                final String selection = "_id=?";
                final String[] selectionArgs = new String[] {split[1]};

                path = getDataColumn(context, contentUri, selection,
                                     selectionArgs);
            }
        }

        // Fallback
        if (path == null && ("content".equalsIgnoreCase(uri.getScheme()) ||
                             "file".equalsIgnoreCase(uri.getScheme()))) {
            path = uri.getPath();
            if (path != null) {
                path = path.replaceFirst("^/storage_root", "");
            }
        }

        if (path != null) {
            if (new File(path).exists() == false) {
                path = "";
            }
        }

        return path;
    }

    public static String getDataColumn(Context context, Uri uri,
                                       String selection,
                                       String[] selectionArgs) {

        Cursor cursor = null;
        final String column = "_data";
        final String[] projection = {column};

        try {
            cursor = context.getContentResolver().query(
                uri, projection, selection, selectionArgs, null);
            if (cursor != null && cursor.moveToFirst()) {
                final int index = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(index);
            }
        } catch (Exception e) {
            if (cursor != null)
                cursor.close();
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }

    public static boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(
            uri.getAuthority());
    }

    public static boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(
            uri.getAuthority());
    }

    public static boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(
            uri.getAuthority());
    }
}
