package ch.opengis.qfield;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetFileDescriptor;
import android.content.res.TypedArray;
import android.database.Cursor;
import android.database.MatrixCursor;
import android.graphics.Point;
import android.os.CancellationSignal;
import android.os.Handler;
import android.os.ParcelFileDescriptor;
import android.provider.DocumentsContract.Document;
import android.provider.DocumentsContract.Root;
import android.provider.DocumentsProvider;
import android.webkit.MimeTypeMap;
import android.util.Log;
import android.os.Environment;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.FilenameFilter;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Set;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;
import javax.xml.xpath.XPathExpressionException;
import org.xml.sax.InputSource;

/**
 * Manages documents and exposes them to the Android system for sharing.
 */
public class QgsDocumentsProvider extends DocumentsProvider {
    private static final String TAG = "MyCloudProvider";

    // Use these as the default columns to return information about a root if no specific
    // columns are requested in a query.
    private static final String[] DEFAULT_ROOT_PROJECTION = new String[]{
            Root.COLUMN_ROOT_ID,
            Root.COLUMN_MIME_TYPES,
            Root.COLUMN_FLAGS,
            Root.COLUMN_ICON,
            Root.COLUMN_TITLE,
            Root.COLUMN_SUMMARY,
            Root.COLUMN_DOCUMENT_ID,
            Root.COLUMN_AVAILABLE_BYTES
    };

    // Use these as the default columns to return information about a document if no specific
    // columns are requested in a query.
    private static final String[] DEFAULT_DOCUMENT_PROJECTION = new String[]{
            Document.COLUMN_DOCUMENT_ID,
            Document.COLUMN_MIME_TYPE,
            Document.COLUMN_DISPLAY_NAME,
            Document.COLUMN_LAST_MODIFIED,
            Document.COLUMN_FLAGS,
            Document.COLUMN_SIZE
    };

    // No official policy on how many to return, but make sure you do limit the number of recent
    // and search results.
    private static final int MAX_SEARCH_RESULTS = 20;
    private static final int MAX_LAST_MODIFIED = 5;
    private static final String ROOT = "root";

    // A file object at the root of the file hierarchy.  Depending on your implementation, the root
    // does not need to be an existing file system directory.  For example, a tag-based document
    // provider might return a directory containing all tags, represented as child directories.
    private File mBaseDir;

    // A set to store the parent direcories of the qgs files, to ensure that we
    // add them only once
    private Set<String> parentDirectories = new HashSet<String>();

    @Override
    public boolean onCreate() {
        //Log.v(TAG, "onCreate");

        mBaseDir = getContext().getFilesDir();
        return true;
    }

    @Override
    public Cursor queryRoots(String[] projection) throws FileNotFoundException {
        //Log.v(TAG, "queryRoots");

        // Create a cursor with either the requested fields, or the default projection.  This
        // cursor is returned to the Android system picker UI and used to display all roots from
        // this provider.
        final MatrixCursor result = new MatrixCursor(resolveRootProjection(projection));

        // TODO
        // If the user is not using QField, return an empty root cursor.  This removes our provider from
        // the list entirely.
        //if (!getContext().getPackageName().equals("ch.opengis.qfield")) {
        //   return result;
        //}

        // It's possible to have multiple roots (e.g. for multiple accounts in the same app) -
        // just add multiple cursor rows.
        // Construct one row for a root called "MyCloud".
        final MatrixCursor.RowBuilder row = result.newRow();

        row.add(Root.COLUMN_ROOT_ID, ROOT);
        row.add(Root.COLUMN_SUMMARY, getContext().getString(R.string.root_summary));

        // FLAG_SUPPORTS_CREATE means at least one directory under the root supports creating
        // documents.  FLAG_SUPPORTS_RECENTS means your application's most recently used
        // documents will show up in the "Recents" category.  FLAG_SUPPORTS_SEARCH allows users
        // to search all documents the application shares.
        row.add(Root.COLUMN_FLAGS, Root.FLAG_SUPPORTS_CREATE |
                Root.FLAG_SUPPORTS_RECENTS |
                Root.FLAG_SUPPORTS_SEARCH);

        // COLUMN_TITLE is the root title (e.g. what will be displayed to identify your provider).
        row.add(Root.COLUMN_TITLE, getContext().getString(R.string.app_name));

        // This document id must be unique within this provider and consistent across time.  The
        // system picker UI may save it and refer to it later.
        row.add(Root.COLUMN_DOCUMENT_ID, getDocIdForFile(mBaseDir));

        // The child MIME types are used to filter the roots and only present to the user roots
        // that contain the desired type somewhere in their file hierarchy.
        row.add(Root.COLUMN_MIME_TYPES, getChildMimeTypes(mBaseDir));
        row.add(Root.COLUMN_AVAILABLE_BYTES, mBaseDir.getFreeSpace());
        row.add(Root.COLUMN_ICON, R.drawable.icon);

        return result;
    }

    @Override
    public Cursor queryDocument(String documentId, String[] projection)
        throws FileNotFoundException {
        //Log.v(TAG, "queryDocument");

        // Create a cursor with the requested projection, or the default projection.
        final MatrixCursor result = new MatrixCursor(resolveDocumentProjection(projection));
        includeFile(result, documentId, null);
        return result;
    }

    @Override
    public Cursor queryChildDocuments(String parentDocumentId, String[] projection,
                                      String sortOrder) throws FileNotFoundException {
        //Log.v(TAG, "queryDocument " + parentDocumentId);

        parentDirectories.clear();
        
        final MatrixCursor result = new MatrixCursor(resolveDocumentProjection(projection));

        if(parentDocumentId.endsWith(getContext().getPackageName()+"/files")){

            // Get all external files dirs in the form of
            // /storage/emulated/0/Android/data/ch.opengis.qfield/files
            // and change dir to the parent 4x

            File[] externalFilesDirs = getContext().getExternalFilesDirs(null);
            for (int i = 0; i < externalFilesDirs.length; i++){
                File root = externalFilesDirs[i].getParentFile().getParentFile().getParentFile().getParentFile();
                String rootPath= root.getPath();
                scanFiles(new File(rootPath), result);
                Log.v(TAG, "Root scanned for qgs projects: " + rootPath);
            }

        }else{
            listFiles(new File(parentDocumentId), result);
        }

        return result;
    }

    /**
     * Scan recursively all the directories to find .qgs files and add the qgs
     * files and the very parent directory of each qgs file to the matrix cursor
     * @param file the starting root of the search
     * @param result the matrix cursor  
     */
    public void scanFiles(File file, MatrixCursor result) throws FileNotFoundException {
        File[] fileArray = file.listFiles();
        for (File f : fileArray){
            if (f.isDirectory()){
                scanFiles(f, result);
            }else if (f.isFile()){
                if (f.getPath().toLowerCase().endsWith(".qgs")){

                    // If the file is in the root directory, add it
                    // otherwise add the parent directory
                    if (f.getParentFile() == file){
                        includeFile(result, null, f);
                    } else{
                    //add the parent directory
                    includeFile(result, null, f.getParentFile());
                    }
                }
            }
        }
    }

    /**
     * Scan a directory to find .qgs files and add them to the matrix cursor
     * @param file the directory to be scanned
     * @param result the matrix cursor
     */
    public void listFiles(File file, MatrixCursor result) throws FileNotFoundException{
        File[] fileArray = file.listFiles();
        for (File f : fileArray){
            if (f.isFile()){
                if (f.getPath().toLowerCase().endsWith(".qgs")){
                    includeFile(result, null, f);
                }
            }
        }
    }
    
    @Override
    public ParcelFileDescriptor openDocument(final String documentId, final String mode,
                                             CancellationSignal signal)
            throws FileNotFoundException {
        //Log.v(TAG, "openDocument, mode: " + mode);
        // It's OK to do network operations in this method to download the document, as long as you
        // periodically check the CancellationSignal.  If you have an extremely large file to
        // transfer from the network, a better solution may be pipes or sockets
        // (see ParcelFileDescriptor for helper methods).

        final File file = getFileForDocId(documentId);
        final int accessMode = ParcelFileDescriptor.parseMode(mode);

        return ParcelFileDescriptor.open(file, accessMode);
    }

    /**
     * @param projection the requested root column projection
     * @return either the requested root column projection, or the default projection if the
     * requested projection is null.
     */
    private static String[] resolveRootProjection(String[] projection) {
        return projection != null ? projection : DEFAULT_ROOT_PROJECTION;
    }

    private static String[] resolveDocumentProjection(String[] projection) {
        return projection != null ? projection : DEFAULT_DOCUMENT_PROJECTION;
    }

    /**
     * Get a file's MIME type
     *
     * @param file the File object whose type we want
     * @return the MIME type of the file
     */
    private static String getTypeForFile(File file) {
        if (file.isDirectory()) {
            return Document.MIME_TYPE_DIR;
        } else {
            return "application/x-qgis-project";
        }
    }

    /**
     * Gets a string of unique MIME data types a directory supports, separated by newlines.  This
     * should not change.
     *
     * @param parent the File for the parent directory
     * @return a string of the unique MIME data types the parent directory supports
     */
    private String getChildMimeTypes(File parent) {
        Set<String> mimeTypes = new HashSet<String>();
        mimeTypes.add("application/x-qgis-project");

        // Flatten the list into a string and insert newlines between the MIME type strings.
        StringBuilder mimeTypesString = new StringBuilder();
        for (String mimeType : mimeTypes) {
            mimeTypesString.append(mimeType).append("\n");
        }

        return mimeTypesString.toString();
    }

    /**
     * Get the document ID given a File.  The document id must be consistent across time. Other
     * applications may save the ID and use it to reference documents later.
     *
     * @param file the File whose document ID you want
     * @return the corresponding document ID
     */
    private String getDocIdForFile(File file) {
        String path = file.getAbsolutePath();
        return path;
    }

    /**
     * Add a representation of a file to a cursor.
     *
     * @param result the cursor to modify
     * @param docId  the document ID representing the desired file (may be null if given file)
     * @param file   the File object representing the desired file (may be null if given docID)
     * @throws java.io.FileNotFoundException
     */
    private void includeFile(MatrixCursor result, String docId, File file)
            throws FileNotFoundException {

        if (docId == null) {
            docId = getDocIdForFile(file);
        } else {
            file = getFileForDocId(docId);
        }

        int flags = 0;
        String displayName = file.getName();
        if (!file.isDirectory()) {
            displayName = qgisName(file);
        }

        if (file.isDirectory() && parentDirectories.contains(docId)){
            return;
        }

        final String mimeType = getTypeForFile(file);

        final MatrixCursor.RowBuilder row = result.newRow();
        row.add(Document.COLUMN_DOCUMENT_ID, docId);
        row.add(Document.COLUMN_DISPLAY_NAME, displayName);
        row.add(Document.COLUMN_SIZE, file.length());
        row.add(Document.COLUMN_MIME_TYPE, mimeType);
        row.add(Document.COLUMN_LAST_MODIFIED, file.lastModified());
        row.add(Document.COLUMN_FLAGS, flags);

        // Add a custom icon
        row.add(Document.COLUMN_ICON, R.drawable.icon);

        parentDirectories.add(docId);
    }

    /**
     * Translate your custom URI scheme into a File object.
     *
     * @param docId the document ID representing the desired file
     * @return a File represented by the given document ID
     * @throws java.io.FileNotFoundException
     */
    private File getFileForDocId(String docId) throws FileNotFoundException {
        return new File(docId);
    }

    /**
     * @param file the qgs file
     * @return a String with the title of the QGIS project or the file name if
     * no title is defined
     */
    private String qgisName(File file) throws FileNotFoundException {

        InputStream is = new FileInputStream(file);
        InputSource inputSrc = new InputSource(is);

        XPath xPath = XPathFactory.newInstance().newXPath();
        String expression = "/qgis/title";
        String projectName = "";
        try{
           projectName = xPath.compile(expression).evaluate(inputSrc);
        }catch(XPathExpressionException e){

        }
        if (projectName.trim().equals("")){
            projectName = file.getName();
        }
        return projectName;
    }    
}

