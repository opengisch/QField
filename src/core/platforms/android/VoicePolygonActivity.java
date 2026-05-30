// src/core/platforms/android/VoicePolygonActivity.java
package org.qfield;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.speech.SpeechRecognizer;
import java.util.ArrayList;

public class VoicePolygonActivity extends Activity {

  private static final int REQ_CODE = 9001;

  static {
    System.loadLibrary("qfield");
  }

  private native void nativeOnResult(String text, boolean isOffline);
  private native void nativeOnError(String errorMsg);
  private native void nativeOnOfflineModelMissing();

  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    super.onActivityResult(requestCode, resultCode, data);
    if (requestCode != REQ_CODE) return;

    if (resultCode == RESULT_OK && data != null) {
      ArrayList<String> results =
        data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
      if (results != null && !results.isEmpty()) {
        boolean isOffline = data.getBooleanExtra(
          "android.speech.extra.IS_LANGUAGE_MODEL_OFFLINE", false);
        nativeOnResult(results.get(0), isOffline);
      }
    } else if (resultCode == RESULT_CANCELED) {
      int errorCode = data != null
        ? data.getIntExtra(RecognizerIntent.EXTRA_ERROR, -1) : -1;
      if (errorCode == SpeechRecognizer.ERROR_LANGUAGE_NOT_SUPPORTED
          || errorCode == SpeechRecognizer.ERROR_LANGUAGE_UNAVAILABLE) {
        nativeOnOfflineModelMissing();
      } else {
        nativeOnError("Código de error RecognizerIntent: " + errorCode);
      }
    }
  }
}
