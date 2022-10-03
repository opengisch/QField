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
import android.webkit.MimeTypeMap;
import androidx.documentfile.provider.DocumentFile;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.SecurityException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

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
                    QFieldUtils.inputStreamToFile(input, filePath,
                                                  file.length());
                } catch (Exception e) {
                    e.printStackTrace();
                    return false;
                }
            }
        }
        return true;
    }

    public static boolean fileToDocumentFile(File file, DocumentFile directory,
                                             ContentResolver resolver) {
        File[] files =
            file.isDirectory() ? file.listFiles() : new File[] {file};
        for (File f : files) {
            String filePath = f.getPath();
            String fileName = f.getName();
            if (f.isDirectory()) {
                // Use pre-existing directory if present
                DocumentFile newDirectory = directory.findFile(fileName);
                if (newDirectory == null) {
                    newDirectory = directory.createDirectory(fileName);
                }
                boolean success = fileToDocumentFile(f, newDirectory, resolver);
                if (!success) {
                    return false;
                }
            } else {
                String extension = "";
                String mimeType = "";
                if (fileName.lastIndexOf(".") > -1) {
                    extension =
                        fileName.substring(fileName.lastIndexOf(".") + 1);
                    mimeType =
                        MimeTypeMap.getSingleton().getMimeTypeFromExtension(
                            extension);
                }
                // Use pre-existing file if present
                DocumentFile documentFile = directory.findFile(fileName);
                if (documentFile == null) {
                    documentFile = directory.createFile(mimeType, fileName);
                }
                try {
                    InputStream input = new FileInputStream(f);
                    OutputStream output =
                        resolver.openOutputStream(documentFile.getUri());
                    QFieldUtils.inputStreamToOutputStream(input, output,
                                                          f.length());
                    output.close();
                } catch (Exception e) {
                    e.printStackTrace();
                    return false;
                }
            }
        }
        return true;
    }

    public static boolean filesToZip(OutputStream out, String[] files) {
        try {
            ZipOutputStream zout = new ZipOutputStream(out);
            for (String path : files) {
                File file = new File(path);
                InputStream in = new FileInputStream(file);
                ZipEntry entry = new ZipEntry(file.getName());
                zout.putNextEntry(entry);
                int size = 0;
                byte[] buffer = new byte[1024];
                while ((size = in.read(buffer, 0, buffer.length)) != -1) {
                    zout.write(buffer, 0, size);
                }
                in.close();
            }
            zout.close();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public static boolean zipToFolder(InputStream in, String folder) {
        try {
            ZipInputStream zin = new ZipInputStream(in);
            ZipEntry entry;
            while ((entry = zin.getNextEntry()) != null) {
                File f = new File(folder + entry.getName());
                if (!f.getCanonicalPath().startsWith(folder)) {
                    // ZIP path traversal protection
                    throw new SecurityException(
                        "ZIP path traversal attack detected, aborting.");
                }
                if (entry.isDirectory()) {
                    f.mkdirs();
                    continue;
                } else {
                    // some ZIP files don't include directory items, we
                    // therefore have to make sure parent directories are always
                    // created
                    new File(f.getParent()).mkdirs();
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

    public static boolean folderToZip(String folder, String archivePath) {
        try {
            FileOutputStream out = new FileOutputStream(archivePath);
            ZipOutputStream zip = new ZipOutputStream(out);

            boolean success = addFolderToZip(zip, folder, folder);

            zip.flush();
            zip.close();
            return success;
        } catch (Exception e) {
            Log.e("QField",
                  "inputStreamToOutputStream exception: " + e.getMessage());
        }
        return false;
    }

    private static boolean addFolderToZip(ZipOutputStream zip, String folder,
                                          String rootFolder) {
        File dir = new File(folder);
        File[] files = dir.listFiles();
        String pathPrefix = "";
        if (folder.length() > rootFolder.length()) {
            pathPrefix = folder.substring(rootFolder.length() + 1);
            if (!pathPrefix.substring(pathPrefix.length() - 1).equals("/")) {
                pathPrefix = pathPrefix + "/";
            }
        }
        for (File file : files) {
            String filePath = file.getPath();
            String fileName = file.getName();

            if (file.isDirectory()) {
                boolean success =
                    addFolderToZip(zip, file.getPath(), rootFolder);
                if (!success) {
                    return false;
                }
            } else {
                try {
                    ZipEntry zipFile = new ZipEntry(pathPrefix + fileName);
                    zip.putNextEntry(zipFile);

                    InputStream input = new FileInputStream(file);
                    inputStreamToOutputStream(input, zip, file.length());
                    zip.closeEntry();
                } catch (Exception e) {
                    Log.e("QField", "inputStreamToOutputStream exception: " +
                                        e.getMessage());
                    return false;
                }
            }
        }
        return true;
    }

    public static boolean inputStreamToOutputStream(InputStream in,
                                                    OutputStream out,
                                                    long totalBytes) {
        try {
            int size = 0;
            int bufferSize = 1024;
            long bufferRead = 0;
            byte[] buffer = new byte[bufferSize];

            if (totalBytes > 0 && bufferRead + bufferSize > totalBytes) {
                bufferSize = (int)(totalBytes - bufferRead);
            }
            while (bufferSize > 0 &&
                   (size = in.read(buffer, 0, bufferSize)) != -1) {
                out.write(buffer, 0, size);
                bufferRead += bufferSize;
                if (totalBytes > 0 && bufferRead + bufferSize > totalBytes) {
                    bufferSize = (int)(totalBytes - bufferRead);
                }
            }
        } catch (Exception e) {
            Log.e("QField",
                  "inputStreamToOutputStream exception: " + e.getMessage());
            return false;
        }
        return true;
    }

    public static boolean inputStreamToFile(InputStream in, String file,
                                            long totalBytes) {
        try {
            OutputStream out = new FileOutputStream(new File(file));

            int size = 0;
            int bufferSize = 1024;
            long bufferRead = 0;
            byte[] buffer = new byte[bufferSize];

            if (totalBytes > 0 && bufferRead + bufferSize > totalBytes) {
                bufferSize = (int)(totalBytes - bufferRead);
            }
            while (bufferSize > 0 &&
                   (size = in.read(buffer, 0, bufferSize)) != -1) {
                out.write(buffer, 0, size);
                bufferRead += bufferSize;
                if (totalBytes > 0 && bufferRead + bufferSize > totalBytes) {
                    bufferSize = (int)(totalBytes - bufferRead);
                }
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
        return file.delete();
    }

    public static String getExtensionFromMimeType(String type) {
        if (type == null) {
            return "";
        } else if (type.equals("application/pdf")) {
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
                try {
                    final String id = DocumentsContract.getDocumentId(uri);
                    final Uri contentUri = ContentUris.withAppendedId(
                        Uri.parse("content://downloads/public_downloads"),
                        Long.valueOf(id));

                    path = getDataColumn(context, contentUri, null, null);
                } catch (NumberFormatException e) {
                    // Not numerical IDs, skipping
                    path = null;
                }
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
