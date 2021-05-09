package com.rebelmodz.modmenu;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

import static android.preference.PreferenceManager.getDefaultSharedPreferences;

public class Preferences {
    private static SharedPreferences.Editor editor;
    public static Context context;
    public static boolean savePref = false, animation = false, expanded = false;

    public static native void Changes(Context context, int feature, String featureName, int value, boolean bool, String str, float valueFloat);

    public static void changeFeatureInt(String featureName, int featureNum, int value) {
        Changes(context, featureNum, featureName, value, false, null, 0f);
        SharedPreferences sharedPreferences = getDefaultSharedPreferences(context);
        editor = sharedPreferences.edit();
        editor.putInt(String.valueOf(featureNum), value).apply();
    }

    public static void changeFeatureFloat(String featureName, int featureNum, float valueFloat) {
        Changes(context, featureNum, featureName, 0, false, null, valueFloat);
        SharedPreferences sharedPreferences = getDefaultSharedPreferences(context);
        editor = sharedPreferences.edit();
        editor.putFloat(String.valueOf(featureNum), valueFloat).apply();
    }

    public static void changeFeatureString(String featureName, int featureNum, String str) {
        Changes(context, featureNum, featureName, 0, false, str, 0f);
        SharedPreferences sharedPreferences = getDefaultSharedPreferences(context);
        editor = sharedPreferences.edit();
        editor.putString(String.valueOf(featureNum), str).apply();
    }

    public static void changeFeatureBoolean(String featureName, int featureNum, boolean value) {
        if (featureNum == -1)
            animation = value;
        if (featureNum == -2)
            expanded = value;
        if (featureNum == -3)
            savePref = value;
        Changes(context, featureNum, featureName, 0, value, null, 0f);
        SharedPreferences sharedPreferences = getDefaultSharedPreferences(context);
        editor = sharedPreferences.edit();
        editor.putBoolean(String.valueOf(featureNum), value).apply();
    }

    public static int loadPrefInt(String featureName, int featureNum) {
        try {
            if (savePref || featureNum < 0) {
                SharedPreferences preferences = PreferenceManager
                        .getDefaultSharedPreferences(context);
                if (featureNum == -7){
                    return preferences.getInt(String.valueOf(featureNum), 50);
                }
                int i = preferences.getInt(String.valueOf(featureNum), 0);
                Changes(context, featureNum, featureName, i, false, null, 0);
                return i;
            }
        } catch (ClassCastException e) {
            Log.e(FloatingModMenuService.TAG, e.getMessage());
        }
        return 0;
    }

    public static float loadPrefFloat(String featureName, int featureNum) {
        try {
            if (savePref || featureNum < 0) {
                SharedPreferences preferences = PreferenceManager
                        .getDefaultSharedPreferences(context);
                if (featureNum == -6) {
                    return preferences.getFloat(String.valueOf(featureNum), 0.7f);
                }
                float i = preferences.getFloat(String.valueOf(featureNum), 0);
                Changes(context, featureNum, featureName, 0, false, null, i);
                return i;
            }
        } catch (ClassCastException e) {
            Log.e(FloatingModMenuService.TAG, e.getMessage());
        }
        return 0;
    }

    public static boolean loadPrefBoolean(String featureName, int featureNum, boolean def) {
        try {
            SharedPreferences preferences = getDefaultSharedPreferences(context);
            if (featureNum == -3) {
                savePref = preferences.getBoolean(String.valueOf(featureNum), false);
            }
            if (savePref || featureNum < 0) {
                //if (featureNum == 1001 && !preferences.contains("1001"))
                //    return true;
                def = preferences.getBoolean(String.valueOf(featureNum), def);
            }
            Changes(context, featureNum, featureName, 0, def, null, 0f);
            return def;
        } catch (ClassCastException e) {
            Log.e(FloatingModMenuService.TAG, e.getMessage());
        }
        return def;
    }

    public static String loadPrefString(String featureName, int featureNum) {
        try {
            SharedPreferences preferences = getDefaultSharedPreferences(context);
            if (savePref || featureNum <= 0) {
                //if (featureNum == 1001 && !preferences.contains("1001"))
                //    return true;
                String str = preferences.getString(String.valueOf(featureNum), "");
                Changes(context, featureNum, featureName, 0, false, str, 0f);
                return str;
            }
        } catch (ClassCastException e) {
            Log.e(FloatingModMenuService.TAG, e.getMessage());
        }
        return "";
    }
}