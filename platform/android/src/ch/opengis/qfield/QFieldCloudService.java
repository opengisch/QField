/**
 * QFieldCloudService.java
 * @author  Mathieu Pellerin - <mathieu@opengis.ch>
 */
/*
 Copyright (c) 2021, Mathieu Pellerin <mathieu@opengis.ch>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the  Marco Bernasocchi <marco@opengis.ch> nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY Marco Bernasocchi <marco@opengis.ch> ''AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Marco Bernasocchi <marco@opengis.ch> BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package ch.opengis.qfield;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ServiceInfo;
import android.os.Build;
import android.util.Log;
import org.qtproject.qt.android.bindings.QtService;

public class QFieldCloudService extends QtService {

    private NotificationManager notificationManager;
    private NotificationChannel notificationChannel;

    private final String CHANNEL_ID = "qfield_service_01";
    private final int NOTIFICATION_ID = 101;

    private static QFieldCloudService instance = null;
    public static QFieldCloudService getInstance() {
        return instance;
    }

    public static void startQFieldCloudService(Context context) {
        Log.v("QFieldCloudService", "Starting QFieldCloudService");
        Intent intent = new Intent(context, QFieldCloudService.class);
        context.startForegroundService(intent);
    }

    public static void triggerShowNotification(String message, int progress) {
        if (getInstance() != null) {
            getInstance().showNotification(message, progress);
        } else {
            Log.v("QFieldCloudService",
                  "Showing message failed, no instance available.");
        }
    }

    public static void triggerCloseNotification() {
        if (getInstance() != null) {
            getInstance().closeNotification();
        } else {
            Log.v("QFieldCloudService",
                  "Closing message failed, no instance available.");
        }
    }

    @Override
    public void onCreate() {
        Log.v("QFieldCloudService", "onCreate triggered");
        super.onCreate();

        if (getInstance() != null) {
            Log.v("QFieldCloudService",
                  "service already running, aborting onCreate.");
            stopSelf();
            return;
        }
    }

    @Override
    public void onDestroy() {
        Log.v("QFieldCloudService", "onDestroy triggered");
        notificationManager.cancel(NOTIFICATION_ID);
        super.onDestroy();
        instance = null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.v("QFieldCloudService", "onStartCommand triggered");

        int ret = super.onStartCommand(intent, flags, startId);
        if (instance != null) {
            Log.v("QFieldCloudService",
                  "service already running, aborting onStartCommand.");
            return START_NOT_STICKY;
        }

        instance = this;

        notificationManager =
            (NotificationManager)getSystemService(NOTIFICATION_SERVICE);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            notificationChannel =
                new NotificationChannel(CHANNEL_ID, "QFieldCloud",
                                        NotificationManager.IMPORTANCE_DEFAULT);
            notificationChannel.setDescription("QFieldCloud service");
            notificationChannel.setImportance(
                NotificationManager.IMPORTANCE_LOW);
            notificationChannel.enableLights(false);
            notificationChannel.enableVibration(false);
            notificationManager.createNotificationChannel(notificationChannel);
        }

        Notification.Builder builder =
            new Notification.Builder(this)
                .setSmallIcon(R.drawable.qfield_logo)
                .setWhen(System.currentTimeMillis())
                .setOngoing(true)
                .setContentTitle("QFieldCloud")
                .setContentText(getString(R.string.upload_pending_attachments));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            builder.setChannelId(CHANNEL_ID);
        }

        Notification notification = builder.build();

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
                startForeground(NOTIFICATION_ID, notification,
                                ServiceInfo.FOREGROUND_SERVICE_TYPE_DATA_SYNC);
            } else {
                startForeground(NOTIFICATION_ID, notification);
            }
        } catch (SecurityException e) {
            Log.v("QFieldCloudService",
                  "Missing permission to launch the QFieldCloud service");
            return START_NOT_STICKY;
        }

        return START_STICKY;
    }

    public void showNotification(String contentText, int progress) {
        // Return to QField activity when clicking on the notification
        Log.v("QFieldCloudService", "Showing notification message");
        PendingIntent contentIntent = PendingIntent.getActivity(
            this, 0, new Intent(this, QFieldActivity.class),
            PendingIntent.FLAG_MUTABLE);

        Notification.Builder builder = new Notification.Builder(this)
                                           .setSmallIcon(R.drawable.qfield_logo)
                                           .setWhen(System.currentTimeMillis())
                                           .setOngoing(true)
                                           .setContentTitle("QFieldCloud")
                                           .setContentText(contentText)
                                           .setProgress(100, progress, false)
                                           .setContentIntent(contentIntent);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            builder.setChannelId(CHANNEL_ID);
        }

        Notification notification = builder.build();
        notificationManager.notify(NOTIFICATION_ID, notification);
    }

    public void closeNotification() {
        notificationManager.cancel(NOTIFICATION_ID);
    }
}
