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
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.text.TextUtils;

public class QFieldProjectActivity extends Activity {

    private static final String TAG = "QField Project Activity";
    private String path;
    private SharedPreferences sharedPreferences;
    private ListView list;

    @Override
    public void onCreate(Bundle bundle) {
        Log.d(TAG, "onCreate() ");
        super.onCreate(bundle);

        sharedPreferences = getPreferences(Context.MODE_PRIVATE);
        setContentView(R.layout.list_projects);
        getActionBar().setBackgroundDrawable(new ColorDrawable(Color.parseColor("#80CC28"))); 
        drawView();
    }

    private void drawView() {

        ArrayList<QFieldProjectListItem> values = new ArrayList<QFieldProjectListItem>();

        // Roots
        if (!getIntent().hasExtra("path")) {

            File externalStorageDirectory = Environment.getExternalStorageDirectory();
            Log.d(TAG, "externalStorageDirectory: " + externalStorageDirectory);
            if (externalStorageDirectory != null){
                values.add(new QFieldProjectListItem(externalStorageDirectory, getString(R.string.primary_storage),
                                                     R.drawable.tablet, QFieldProjectListItem.TYPE_ROOT));
           }

            File[] externalFilesDirs = getExternalFilesDirs(null);
            Log.d(TAG, "externalFilesDirs: " + Arrays.toString(externalFilesDirs));
            for (File file: externalFilesDirs){
                if (file != null){
                    // Don't add a external storage path if already included in the primary one
                    if(externalStorageDirectory != null){
                        if (!file.getAbsolutePath().contains(externalStorageDirectory.getAbsolutePath())){
                            values.add(new QFieldProjectListItem(file, getString(R.string.secondary_storage), R.drawable.card, QFieldProjectListItem.TYPE_ROOT));
                        }
                    }else{
                        values.add(new QFieldProjectListItem(file, getString(R.string.secondary_storage), R.drawable.card, QFieldProjectListItem.TYPE_ROOT));
                    }
                }
            }

            setTitle(getString(R.string.select_project));
            Collections.sort(values);

            String lastUsedProjects = sharedPreferences.getString("LastUsedProjects", null);
            if (lastUsedProjects != null){
                String[] lastUsedProjectsArray = lastUsedProjects.split("--;--");
                values.add(new QFieldProjectListItem(null, getString(R.string.recent_projects), 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (int i=lastUsedProjectsArray.length-1; i>=0; i--) {
                    File f = new File(lastUsedProjectsArray[i]);
                    if (f.exists()){
                        values.add(new QFieldProjectListItem(f, f.getName(), R.drawable.icon, QFieldProjectListItem.TYPE_ITEM));
                    }
                }
            }

            String favoriteDirs = sharedPreferences.getString("FavoriteDirs", null);
            if (favoriteDirs != null){
                String[] favoriteDirsArray = favoriteDirs.split("--;--");
                values.add(new QFieldProjectListItem(null, getString(R.string.favorite_directories), 0, QFieldProjectListItem.TYPE_SEPARATOR));

                for (int i=favoriteDirsArray.length-1; i>=0; i--) {
                    File f = new File(favoriteDirsArray[i]);
                    if (f.exists()){
                        values.add(new QFieldProjectListItem(f, f.getName(), R.drawable.directory, QFieldProjectListItem.TYPE_ITEM));
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
        list = (ListView) findViewById(R.id.list);
        QFieldProjectListAdapter adapter = new QFieldProjectListAdapter(this, values);
        list.setAdapter(adapter);

        list.setOnItemClickListener(new OnItemClickListener(){
            public void onItemClick(AdapterView<?> parent, View view, int position, long id){
                Log.d(TAG, "onItemClick ");
                QFieldProjectActivity.this.onItemClick(position);
            }
        });

        list.setOnItemLongClickListener(new OnItemLongClickListener(){
            public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id){
                Log.d(TAG, "onItemLongClick ");
                return QFieldProjectActivity.this.onItemLongClick(position);
            }
        });

    }

    public void onRestart(){
        Log.d(TAG, "onRestart ");

        // The first opened activity
        if (!getIntent().hasExtra("path")) {
            drawView();
        }
        super.onRestart();
    }
    private void onItemClick(int position) {
        Log.d(TAG, "onListItemClick ");

        QFieldProjectListItem item = (QFieldProjectListItem) list.getAdapter().getItem(position);
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

            String lastUsedProjects = sharedPreferences.getString("LastUsedProjects", null);
            ArrayList<String> lastUsedProjectsArray = new ArrayList<String>();
            if (lastUsedProjects != null){
                lastUsedProjectsArray = new ArrayList<String>(Arrays.asList(lastUsedProjects.split("--;--")));
            }
            // If the element is already present, delete it. It will be added again in the last position
            lastUsedProjectsArray.remove(file.getPath());
            if (lastUsedProjectsArray.size() >= 5){
                lastUsedProjectsArray.remove(0);
            }
            // Add the project path to the array
            lastUsedProjectsArray.add(file.getPath());

            // Write the recent projects into the shared preferences
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("LastUsedProjects", TextUtils.join("--;--", lastUsedProjectsArray));
            editor.commit();

            finish();
        }
    }

    private boolean onItemLongClick(int position) {

        QFieldProjectListItem item = (QFieldProjectListItem) list.getAdapter().getItem(position);
        if (item.getType() != QFieldProjectListItem.TYPE_ITEM) {
            return true;
        }
        File file = item.getFile();
        if (! file.isDirectory()){
            return true;
        }

        String favoriteDirs = sharedPreferences.getString("FavoriteDirs", null);
        ArrayList<String> favoriteDirsArray = new ArrayList<String>();
        if (favoriteDirs != null){
            favoriteDirsArray = new ArrayList<String>(Arrays.asList(favoriteDirs.split("--;--")));
        }

        // If the element is already present, delete it. It will be added again in the last position
        favoriteDirsArray.remove(file.getPath());

        // First activity
        if (! getIntent().hasExtra("path")) {
            // Remove the recent projects from shared preferences
            SharedPreferences.Editor editor = sharedPreferences.edit();
            favoriteDirs = TextUtils.join("--;--", favoriteDirsArray);
            if (favoriteDirs == ""){
                favoriteDirs = null;
            }

            editor.putString("FavoriteDirs", favoriteDirs);
            editor.commit();
            drawView();

            Toast.makeText(this, file.getName() + " " + getString(R.string.removed_from_favorites), Toast.LENGTH_LONG).show();
        } else {
            // Write the recent projects into the shared preferences
            favoriteDirsArray.add(file.getPath());
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("FavoriteDirs", TextUtils.join("--;--", favoriteDirsArray));
            editor.commit();

            Toast.makeText(this, file.getName() + " " + getString(R.string.added_to_favorites), Toast.LENGTH_LONG).show();
        }

        return true;
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
