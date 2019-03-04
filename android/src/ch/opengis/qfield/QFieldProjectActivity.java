package ch.opengis.qfield;

import java.io.File;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Arrays;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

import android.os.Bundle;
import android.os.Environment;
import android.net.Uri;
import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.ArrayAdapter;
import android.widget.Toast;
import android.widget.ListView;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;

public class QFieldProjectActivity extends ListActivity{

    private static final String TAG = "QField Project Activity";
    private String path;
    private SharedPreferences sharedPreferences;

    @Override
    public void onCreate(Bundle bundle) {
        Log.d(TAG, "onCreate() ");
        super.onCreate(bundle);

        sharedPreferences = getPreferences(Context.MODE_PRIVATE);
        setContentView(R.layout.list_projects);
        getActionBar().setBackgroundDrawable(new ColorDrawable(Color.parseColor("#80CC28"))); 

        ArrayList<QFieldProjectListItem> values = new ArrayList<QFieldProjectListItem>();

        // Roots
        if (!getIntent().hasExtra("path")) {
                  
            File externalStorageDirectory = Environment.getExternalStorageDirectory();
            Log.d(TAG, "externalStorageDirectory: " + externalStorageDirectory);
            if (externalStorageDirectory != null){
                values.add(new QFieldProjectListItem(externalStorageDirectory, getString(R.string.primary_storage),
                                                     R.drawable.tablet, QFieldProjectListItem.TYPE_ITEM));
           }

            File[] externalFilesDirs = getExternalFilesDirs(null);
            Log.d(TAG, "externalFilesDirs: " + Arrays.toString(externalFilesDirs));
            for (File file: externalFilesDirs){
                if (file != null){
                    // Don't add a external storage path if already included in the primary one
                    if(externalStorageDirectory != null){
                        if (!file.getAbsolutePath().contains(externalStorageDirectory.getAbsolutePath())){
                            values.add(new QFieldProjectListItem(file, getString(R.string.secondary_storage), R.drawable.card, QFieldProjectListItem.TYPE_ITEM));
                        }
                    }else{
                        values.add(new QFieldProjectListItem(file, getString(R.string.secondary_storage), R.drawable.card, QFieldProjectListItem.TYPE_ITEM));
                    }
                }
            }

            setTitle(getString(R.string.select_project));
            Collections.sort(values);

            Set<String> lastUsedDirs = sharedPreferences.getStringSet("LastUsedDirectories", null);

            if (lastUsedDirs != null){
                    // TODO: translation of "recents"
                values.add(new QFieldProjectListItem(null, "Recent directories", 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (String s : lastUsedDirs) {
                    File f = new File(s);
                    values.add(new QFieldProjectListItem(f, f.getName(),
                                                     R.drawable.directory, QFieldProjectListItem.TYPE_ITEM));
                }
            }

            Set<String> lastUsedProjects = sharedPreferences.getStringSet("LastUsedProjects", null);            
            if (lastUsedProjects != null){
                    // TODO: translation of "recents"
                values.add(new QFieldProjectListItem(null, "Recent projects", 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (String s : lastUsedProjects) {
                    File f = new File(s);
                    values.add(new QFieldProjectListItem(f, f.getName(),
                                                     R.drawable.icon, QFieldProjectListItem.TYPE_ITEM));
                }
            }

        }else{ // Over the roots
            Log.d(TAG, "extra path: " + getIntent().getStringExtra("path"));
            File dir = new File(getIntent().getStringExtra("path"));
            setTitle(getIntent().getStringExtra("label"));
            getActionBar().setSubtitle(dir.getPath());

            if (!dir.canRead()) {
                setTitle(getTitle() + " ("+getString(R.string.inaccessible)+")");
            }
            File[] list = dir.listFiles();
            if (list != null) {
                for (File file : list) {
                    if (file.getName().startsWith(".")) {
                        continue;
                    }else if (file.getName().toLowerCase().endsWith(".qgs")){
                        values.add(new QFieldProjectListItem(file, file.getName(), R.drawable.icon, QFieldProjectListItem.TYPE_ITEM));
                    }else if (file.isDirectory()){
                        values.add(new QFieldProjectListItem(file, file.getName(), R.drawable.directory, QFieldProjectListItem.TYPE_ITEM));
                    }
                }
            }
            Collections.sort(values);
        }


        // Put the data into the list
        QFieldProjectListAdapter adapter = new QFieldProjectListAdapter(this, values);
        setListAdapter(adapter);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        Log.d(TAG, "onListItemClick ");

        QFieldProjectListItem item = (QFieldProjectListItem) getListAdapter().getItem(position);
        if (item.getType() == QFieldProjectListItem.TYPE_SEPARATOR){
            return;
        }
        File file = item.getFile();
        Log.d(TAG, "file: "+file.getPath());                
        if (file.isDirectory()) {
            Intent intent = new Intent(this, QFieldProjectActivity.class);
            intent.putExtra("path", file.getPath());
            intent.putExtra("label", item.getText());
            startActivityForResult(intent, 123);
        } else {
            Intent data = new Intent();

            Uri uri = Uri.fromFile(file);
            data.setData(uri);
            
            Toast.makeText(this, getString(R.string.loading) + " " + file.getPath(), Toast.LENGTH_LONG).show();
            setResult(Activity.RESULT_OK, data);

            Set<String> lastUsedDirs = sharedPreferences.getStringSet("LastUsedDirectories", null);
            if (lastUsedDirs == null){
                lastUsedDirs = new TreeSet<String>();
            }
            Set<String> lastUsedProjects = sharedPreferences.getStringSet("LastUsedProjects", null);
            if (lastUsedProjects == null){
                lastUsedProjects = new TreeSet<String>();
            }
            
            // Store directory
            SharedPreferences.Editor editor = sharedPreferences.edit();

            lastUsedDirs.add(file.getParent());
            lastUsedProjects.add(file.getPath());            
            editor.putStringSet("LastUsedDirectories", lastUsedDirs);
            editor.putStringSet("LastUsedProjects", lastUsedProjects);            
            editor.commit();
            finish();
        }
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        Log.d(TAG, "onActivityResult ");
        Log.d(TAG, "resultCode: " + resultCode);

        // Close recursively the activity stack
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
