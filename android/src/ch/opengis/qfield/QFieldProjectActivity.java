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
import android.text.TextUtils;

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

            String lastUsedDirs = sharedPreferences.getString("LastUsedDirectories", null);
            Log.d(TAG, "lastUsedDirs: " + lastUsedDirs);
            if (lastUsedDirs != null){
                String[] lastUsedDirsArray = lastUsedDirs.split(";");
                values.add(new QFieldProjectListItem(null, getString(R.string.recent_directories), 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (int i=lastUsedDirsArray.length-1; i>=0; i--) {
                    File f = new File(lastUsedDirsArray[i]);
                    if(f.exists()){
                        values.add(new QFieldProjectListItem(f, f.getName(), R.drawable.directory, QFieldProjectListItem.TYPE_ITEM));
                    }
                }
            }

            String lastUsedProjects = sharedPreferences.getString("LastUsedProjects", null);
            if (lastUsedProjects != null){
                String[] lastUsedProjectsArray = lastUsedProjects.split(";");
                values.add(new QFieldProjectListItem(null, getString(R.string.recent_projects), 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (int i=lastUsedProjectsArray.length-1; i>=0; i--) {
                    File f = new File(lastUsedProjectsArray[i]);
                    if (f.exists()){
                        values.add(new QFieldProjectListItem(f, f.getName(), R.drawable.icon, QFieldProjectListItem.TYPE_ITEM));
                    }
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

            String lastUsedDirs = sharedPreferences.getString("LastUsedDirectories", null);
            ArrayList<String> lastUsedDirsArray = new ArrayList<String>();
            if (lastUsedDirs != null){
                lastUsedDirsArray = new ArrayList<String>(Arrays.asList(lastUsedDirs.split(";")));
            }
            // If the element is already present, delete it. It will be added again in the last position
            lastUsedDirsArray.remove(file.getParent());
            if (lastUsedDirsArray.size() >= 3){
                lastUsedDirsArray.remove(0);
            }
            // Add the directory to the array
            lastUsedDirsArray.add(file.getParent());

            String lastUsedProjects = sharedPreferences.getString("LastUsedProjects", null);
            ArrayList<String> lastUsedProjectsArray = new ArrayList<String>();
            if (lastUsedProjects != null){
                lastUsedProjectsArray = new ArrayList<String>(Arrays.asList(lastUsedProjects.split(";")));
            }
            // If the element is already present, delete it. It will be added again in the last position
            lastUsedProjectsArray.remove(file.getPath());
            if (lastUsedProjectsArray.size() >= 5){
                lastUsedProjectsArray.remove(0);
            }
            // Add the project path to the array
            lastUsedProjectsArray.add(file.getPath());

            // Write the recent directories and projects into the shared preferences
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("LastUsedDirectories", TextUtils.join(";", lastUsedDirsArray));
            editor.putString("LastUsedProjects", TextUtils.join(";", lastUsedProjectsArray));
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
