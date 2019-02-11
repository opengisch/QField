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
        ArrayList values = new ArrayList();
        
        if (getIntent().hasExtra("path")) {
            File dir = new File(getIntent().getStringExtra("path"));
            setTitle(dir.getPath());
            // Read all files sorted into the values-array
        
            if (!dir.canRead()) {
                setTitle(getTitle() + " (inaccessible)");
            }
            File[] list = dir.listFiles();
            if (list != null) {
                for (File file : list) {
                    if (!file.getName().startsWith(".")) {
                        if((!file.isDirectory()) && (!file.getName().endsWith(".qgs"))){
                            continue;
                        }
                        values.add(file);
                    }
                }
            }
        }else{

            File[] externalFilesDirs = getExternalFilesDirs(null);
            Log.d(TAG, "External Files Dirs: " + Arrays.toString(externalFilesDirs));
        
            for (File file: externalFilesDirs){  
                values.add(file);
            }

            values.add(Environment.getExternalStorageDirectory());
            setTitle("Select a Qgs file");
        }

        Collections.sort(values);

        // Put the data into the list
        //ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_2, android.R.id.text1, values);
        QFieldProjectListAdapter adapter = new QFieldProjectListAdapter(this, values);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        Log.d(TAG, "in onListItemClick ");        

        File file = (File) getListAdapter().getItem(position);
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
