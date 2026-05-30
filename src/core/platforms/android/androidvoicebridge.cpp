// src/core/platforms/android/androidvoicebridge.cpp
#include "androidvoicebridge.h"
#include <QtCore/private/qandroidextras_p.h>
#include <QDebug>

AndroidVoiceBridge *AndroidVoiceBridge::s_instance = nullptr;

AndroidVoiceBridge::AndroidVoiceBridge(QObject *parent)
  : QObject(parent)
{
  s_instance = this;
}

void AndroidVoiceBridge::setLocale(const QString &loc) {
  if (m_locale != loc) { m_locale = loc; emit localeChanged(); }
}

void AndroidVoiceBridge::startListening() {
  QJniObject intent("android/content/Intent");
  QJniObject action = QJniObject::fromString(
    "android.speech.RecognizerIntent.ACTION_RECOGNIZE_SPEECH");
  intent.callObjectMethod("setAction",
    "(Ljava/lang/String;)Landroid/content/Intent;", action.object<jstring>());

  QJniObject extraOffline = QJniObject::fromString(
    "android.speech.RecognizerIntent.EXTRA_PREFER_OFFLINE");
  intent.callObjectMethod("putExtra",
    "(Ljava/lang/String;Z)Landroid/content/Intent;",
    extraOffline.object<jstring>(), jboolean(JNI_TRUE));

  QJniObject extraLang = QJniObject::fromString(
    "android.speech.RecognizerIntent.EXTRA_LANGUAGE");
  QJniObject langValue = QJniObject::fromString(m_locale);
  intent.callObjectMethod("putExtra",
    "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
    extraLang.object<jstring>(), langValue.object<jstring>());

  QAndroidJniObject activity = QtAndroid::androidActivity();
  activity.callMethod<void>("startActivityForResult",
    "(Landroid/content/Intent;I)V",
    intent.object<jobject>(), jint(9001));
}

void AndroidVoiceBridge::stopListening() {
  // RecognizerIntent es one-shot — no hay acción de cancelar activa
}

void AndroidVoiceBridge::onJavaResult(JNIEnv *env, jobject, jstring text, jboolean) {
  if (!s_instance) return;
  const char *chars = env->GetStringUTFChars(text, nullptr);
  emit s_instance->result(QString::fromUtf8(chars));
  env->ReleaseStringUTFChars(text, chars);
}

void AndroidVoiceBridge::onJavaError(JNIEnv *env, jobject, jstring errorMsg) {
  if (!s_instance) return;
  const char *chars = env->GetStringUTFChars(errorMsg, nullptr);
  emit s_instance->error(QString::fromUtf8(chars));
  env->ReleaseStringUTFChars(errorMsg, chars);
}

void AndroidVoiceBridge::onJavaOfflineModelMissing(JNIEnv *, jobject) {
  if (s_instance) emit s_instance->offlineModelMissing();
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
  JNIEnv *env;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

  jclass cls = env->FindClass("org/qfield/VoicePolygonActivity");
  if (!cls) return JNI_ERR;

  static const JNINativeMethod methods[] = {
    { "nativeOnResult",             "(Ljava/lang/String;Z)V",
      reinterpret_cast<void*>(AndroidVoiceBridge::onJavaResult) },
    { "nativeOnError",              "(Ljava/lang/String;)V",
      reinterpret_cast<void*>(AndroidVoiceBridge::onJavaError) },
    { "nativeOnOfflineModelMissing","()V",
      reinterpret_cast<void*>(AndroidVoiceBridge::onJavaOfflineModelMissing) },
  };
  env->RegisterNatives(cls, methods, 3);
  return JNI_VERSION_1_6;
}
