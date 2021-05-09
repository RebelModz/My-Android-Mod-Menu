
bool titleValid = false, headingValid = false, iconValid = false;

void *antiLeech(void *) {
    sleep(15);
    if (!titleValid || !headingValid || !iconValid) {
        //Bad function to make it crash
        int *p = 0;
        *p = 0;
    }
    return NULL;
}

extern "C" {
JNIEXPORT jstring
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_Title(JNIEnv *env, jobject thiz) {
    titleValid = true;

    //Html is supported
    return env->NewStringUTF(OBFUSCATE("<b>Modded by RebelModz</b>"));
}

JNIEXPORT jstring
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_Heading(JNIEnv *env, jobject thiz) {
    headingValid = true;

    //It will autoscroll if the text is too long
    //Html is supported
    return env->NewStringUTF(OBFUSCATE("<b><marquee><p style=\"font-size:30\">"
                                       "<p style=\"color:green;\">Modded By RebelModz [YouTube]</p> | "
                                       "Original Mod Menu: https://github.com/LGLTeam | Modded Version Of LGLTeam Android Mod Menu</p>"
                                       "</marquee></b>"));
    // return env->NewStringUTF(OBFUSCATE("Put your website or notes here"));
}

JNIEXPORT jstring
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_Icon(JNIEnv *env, jobject thiz) {
    iconValid = true;

    //Use https://www.base64encode.org/ to encode your image to base64
    return env->NewStringUTF(OBFUSCATE("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"));
}

JNIEXPORT jstring
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_IconWebViewData(JNIEnv *env, jobject thiz) {
    iconValid = true;
    //WebView support GIF animation. Upload your image or GIF on imgur.com or other sites

    // From internet (Requires android.permission.INTERNET)
    // return env->NewStringUTF(OBFUSCATE_KEY("https://i.imgur.com/SujJ85j.gif", 'd'));

    // From assets folder: (Requires android.permission.INTERNET)
    // return env->NewStringUTF(OBFUSCATE_KEY("file:///android_asset/example.gif", 'r');

    // Base64 html:
    // return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

    // To disable it, return NULL. It will use normal image above:
    // return NULL

    //return env->NewStringUTF(OBFUSCATE_KEY("https://i.imgur.com/SujJ85j.gif", 'u'));
    return NULL;
}

JNIEXPORT jobjectArray
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_settingsList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_SETTINGS"),
            OBFUSCATE("-1_Toggle_Color animation"),
            OBFUSCATE("-2_Toggle_Auto size vertically"),
            OBFUSCATE("-3_Toggle_Save feature preferences"), //-3 is checked on Preferences.java
            OBFUSCATE("-6_FloatSeekBar_Icon Transparency_0.2_1.0"),
            OBFUSCATE("-7_IntSeekBar_Icon Size_50_80"),
            OBFUSCATE("-8_FloatSeekBar_Menu Corner_0.0_30.0"),
            OBFUSCATE("Category_LOGCAT"),
            OBFUSCATE("RichTextView_Save logcat if a bug occured and sent it to the modder. Clear logcat and reproduce bug again if the log file is too large"),
            OBFUSCATE("RichTextView_<small>Saving logcat does not need file permission. Logcat location:<br/>Android 11: /storage/emulated/0/Documents/Mod Menu logs/"
                    "<br/>Android 10 and below: /storage/emulated/0/Android/data/(package name)/files/Mod Menu logs</small>"),
            OBFUSCATE("-4_Button_Save logcat to file"),
            OBFUSCATE("-5_Button_Clear logcat"),

    };

    int Total_Feature = (sizeof features /
                         sizeof features[0]); //Now you dont have to manually update the number everytime;
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    int i;
    for (i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}
}