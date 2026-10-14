# Keep all classes in the Qt Android runtime package
-keep class org.qtproject.qt.android.** { *; }
-keep class ch.opengis.** {*; }

# Prevent native methods and JNI callback methods from being obfuscated or removed
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep standard Android components used by Qt templates
-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider

# Preserve Attributes for Troubleshooting
-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod
-keepattributes SourceFile,LineNumberTable
