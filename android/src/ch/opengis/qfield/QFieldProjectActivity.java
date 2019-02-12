package ch.opengis.qfield;

import java.io.File;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Arrays;

import android.os.Bundle;
import android.os.Environment;
import android.net.Uri;
import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Toast;
import android.widget.ListView;

public class QFieldProjectActivity extends ListActivity{

    private static final String TAG = "QField Project Activity";
    private String path;

    @Override
    public void onCreate(Bundle bundle) {
        Log.d(TAG, "*** In onCreate() ");
        super.onCreate(bundle);

        setContentView(R.layout.list_projects);
        ArrayList<QFieldProjectListItem> values = new ArrayList<QFieldProjectListItem>();

        // Roots
        if (!getIntent().hasExtra("path")) {

            File externalStorageDirectory = Environment.getExternalStorageDirectory();
            if (externalStorageDirectory != null){
                values.add(new QFieldProjectListItem(externalStorageDirectory, "Primary storage",
                                                     R.drawable.tablet));
            }

            File[] externalFilesDirs = getExternalFilesDirs(null);
            Log.d(TAG, "External Files Dirs: " + Arrays.toString(externalFilesDirs));
            for (File file: externalFilesDirs){
                if (file != null){
                    // Don't add a external storage path if already included in the primary one
                    if(externalStorageDirectory != null){
                        if (!file.getAbsolutePath().contains(externalStorageDirectory.getAbsolutePath())){
                            values.add(new QFieldProjectListItem(file, "Secondary storage", R.drawable.card));
                        }
                    }else{
                        values.add(new QFieldProjectListItem(file, "Secondary storage", R.drawable.card));
                    }
                }
            }

            setTitle("Select a Qgs file");

        }else{ // Over the roots
            File dir = new File(getIntent().getStringExtra("path"));
            setTitle(dir.getPath());
            // Read all files sorted into the values-array
        
            if (!dir.canRead()) {
                setTitle(getTitle() + " (inaccessible)");
            }
            File[] list = dir.listFiles();
            if (list != null) {
                for (File file : list) {
                    if (file.getName().startsWith(".")) {
                        continue;
                    }else if (file.getName().endsWith(".qgs")){
                        values.add(new QFieldProjectListItem(file, file.getName(), R.drawable.icon));
                    }else if (file.isDirectory()){
                        values.add(new QFieldProjectListItem(file, file.getName(), R.drawable.directory));
                    }
                }
            }
        }

        // Put the data into the list
        QFieldProjectListAdapter adapter = new QFieldProjectListAdapter(this, values);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        Log.d(TAG, "in onListItemClick ");        

        // File file = (File) getListAdapter().getItem(position);
        File file = ((QFieldProjectListItem) getListAdapter().getItem(position)).getFile();
        Log.d(TAG, "file: "+file.getPath());                
        if (file.isDirectory()) {
            Intent intent = new Intent(this, QFieldProjectActivity.class);
            intent.putExtra("path", file.getPath());
            startActivityForResult(intent, 123);
        } else {
            Log.d(TAG, "filename: "+file.getPath());
            Intent data = new Intent();

            Uri uri = Uri.fromFile(file);
            data.setData(uri);
            
            Toast.makeText(this, "Opening " +file.getPath(), Toast.LENGTH_LONG).show();
            setResult(Activity.RESULT_OK, data);
            finish();
        }
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult: ");

        if (resultCode == Activity.RESULT_OK){
            if (getParent() == null) {
                setResult(Activity.RESULT_OK, data);
            } else {
                getParent().setResult(Activity.RESULT_OK, data);
            }
            finish();
        }
    }
}
