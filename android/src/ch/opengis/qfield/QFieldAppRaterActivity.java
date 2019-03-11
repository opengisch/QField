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
import android.widget.ImageView;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.LayoutInflater;
import android.util.Log;
import android.graphics.drawable.ColorDrawable;

public class QFieldAppRaterActivity extends Activity{
    private final static String APP_PNAME = "ch.opengis.qfield"; // Package Name

    private final static int DAYS_UNTIL_PROMPT = 30; //Min number of days
    private final static int LAUNCHES_UNTIL_PROMPT = 10; //Min number of launches

    private static final String TAG = "QField AppRater Activity";
    private SharedPreferences sharedPreferences;
    private SharedPreferences.Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        Log.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);

        sharedPreferences = getSharedPreferences("AppRater", Context.MODE_PRIVATE);

        if (sharedPreferences.getBoolean("DontShowAgain", false)) {
            Log.d(TAG, "Dont show again");
            finish();
            return;
        }

        editor = sharedPreferences.edit();

        // Increment launch counter
        long launchCount = sharedPreferences.getLong("LaunchCount", 0) + 1;
        Log.d(TAG, "Launch count: " + (launchCount - 1));
        editor.putLong("LaunchCount", launchCount);
        editor.commit();

        // Get date of first launch
        Long firstLaunch = sharedPreferences.getLong("FirstLaunch", 0);
        Log.d(TAG, "First launch: " + firstLaunch);
        if (firstLaunch == 0) {
            firstLaunch = System.currentTimeMillis();
            editor.putLong("FirstLaunch", firstLaunch);
            editor.commit();
        }

        // Wait at least n days before opening
        if (launchCount >= LAUNCHES_UNTIL_PROMPT) {
            if (System.currentTimeMillis() >= firstLaunch +
                    (DAYS_UNTIL_PROMPT * 24 * 60 * 60 * 1000)) {
                showRateDialog();
            }
        }

        return;
    }

    public void showRateDialog() {

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        Log.d(TAG, "showRateDialog()");

        LayoutInflater inflater = getLayoutInflater();

        // Inflate and set the layout for the dialog
        // Pass null as the parent view because its going in the dialog layout
        View view = inflater.inflate(R.layout.rate_dialog, null);
        builder.setView(view);

        builder.setTitle(getString(R.string.rate_title));
        builder.setMessage(getString(R.string.rate_message));

        // Set 5 stars image
        ImageView imgView = (ImageView) view.findViewById(R.id.image);
        imgView.setImageResource(R.drawable.icon);
        imgView.setOnClickListener(new View.OnClickListener(){
                public void onClick(View v) {
                    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + APP_PNAME)));
                    if (editor != null) {
                        editor.putBoolean("DontShowAgain", true);
                        editor.commit();
                    }
                    finish();
                }
            });

        builder.setPositiveButton(getString(R.string.rate_now), new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + APP_PNAME)));
                    if (editor != null) {
                        editor.putBoolean("DontShowAgain", true);
                        editor.commit();
                    }
                    finish();
                }
            });

        builder.setNeutralButton(getString(R.string.remind_later), new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {

                    finish();
                }
            });

        builder.setNegativeButton(getString(R.string.no_thanks), new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int id) {
                    if (editor != null) {
                        editor.putBoolean("DontShowAgain", true);
                        editor.commit();
                    }
                    finish();
                }
            });

        AlertDialog dialog = builder.create();
        dialog.show();
    }
}
