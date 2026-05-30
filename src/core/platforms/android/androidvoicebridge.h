// src/core/platforms/android/androidvoicebridge.h
#pragma once
#include <QObject>
#include <QString>
#include <jni.h>

class AndroidVoiceBridge : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)

public:
  explicit AndroidVoiceBridge(QObject *parent = nullptr);

  QString locale() const { return m_locale; }
  void    setLocale(const QString &loc);

  Q_INVOKABLE void startListening();
  Q_INVOKABLE void stopListening();

  static void onJavaResult(JNIEnv *env, jobject thiz, jstring text, jboolean isOfflineResult);
  static void onJavaError(JNIEnv *env, jobject thiz, jstring errorMsg);
  static void onJavaOfflineModelMissing(JNIEnv *env, jobject thiz);

signals:
  void result(const QString &text);
  void error(const QString &message);
  void offlineModelMissing();
  void localeChanged();

private:
  QString m_locale = "es-MX";
  static AndroidVoiceBridge *s_instance;
};
