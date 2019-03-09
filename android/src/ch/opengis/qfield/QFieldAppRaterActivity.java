package ch.opengis.qfield;

import android.os.Bundle;
import android.net.Uri;
import android.app.Dialog;
import android.app.AlertDialog;
import android.app.Activity;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.Context;
import android.content.Intent;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.util.Log;

public class QFieldAppRaterActivity extends Activity{
    private final static String APP_TITLE = "QField";// App Name
    private final static String APP_PNAME = "ch.opengis.qfield";// Package Name

    private final static int DAYS_UNTIL_PROMPT = 30;//Min number of days
    private final static int LAUNCHES_UNTIL_PROMPT = 10;//Min number of launches

    private static final String TAG = "QField AppRater Activity";

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d("QField Project AppRater", "onCreate()");
        super.onCreate(savedInstanceState);

        Log.d(TAG, "app_launched()");

        //Context mContext = QFieldActivity.getContext();
        SharedPreferences prefs = getSharedPreferences("apprater", 0);
        if (prefs.getBoolean("dontshowagain", false)) { return; }

        SharedPreferences.Editor editor = prefs.edit();

        //FIXME:
        showRateDialog(editor);

        // Increment launch counter
        long launch_count = prefs.getLong("launch_count", 0) + 1;
        editor.putLong("launch_count", launch_count);

        // Get date of first launch
        Long date_firstLaunch = prefs.getLong("date_firstlaunch", 0);
        if (date_firstLaunch == 0) {
            date_firstLaunch = System.currentTimeMillis();
            editor.putLong("date_firstlaunch", date_firstLaunch);
        }

        // Wait at least n days before opening
        if (launch_count >= LAUNCHES_UNTIL_PROMPT) {
            if (System.currentTimeMillis() >= date_firstLaunch +
                    (DAYS_UNTIL_PROMPT * 24 * 60 * 60 * 1000)) {
                //showRateDialog(mContext, editor);
            }
        }

        editor.commit();
        return;
    }

    public void showRateDialog(SharedPreferences.Editor editor) {

        AlertDialog.Builder builder = new AlertDialog.Builder(this);

        // TODO: translations
        builder.setTitle("Rate QField");
        builder.setMessage("If you enjoy using " + APP_TITLE + ", please take a moment to rate it. Thanks for your support!");

        builder.setPositiveButton("Rate", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + APP_PNAME)));    
                    finish();
                }
            });

        builder.setNeutralButton("Remind me later", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {

                    finish();
                }
            });


        builder.setNegativeButton("No, thanks", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    if (editor != null) {
                        editor.putBoolean("dontshowagain", true);
                        editor.commit();
                    }
                    finish();
                }
            });

        AlertDialog dialog = builder.create();
    }
}
