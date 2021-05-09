package com.rebelmodz;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Toast;

import com.rebelmodz.modmenu.FloatingModMenuService;

import static com.rebelmodz.modmenu.Preferences.context;

public class MainActivity extends Activity {

    public String GameActivity = "com.unity3d.player.UnityPlayerActivity";
    public boolean hasLaunched = false;

    public static native void Toast(Context context);

    //Load lib
    static {
        // When you change the lib name, change also on Android.mk file
        // Both must have same name
        System.loadLibrary("MyLibName");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        context = this;
        //To launch mod menu. If you don't use mod menu, remove or comment out Start(this);
        Start(this);

        //To launch game activity
        if (!hasLaunched) {
            try {
                //Start service
                //MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(MainActivity.this.GameActivity)));
                MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(MainActivity.this.GameActivity)));
            } catch (ClassNotFoundException e) {
                //Uncomment this if you are following METHOD 2 of CHANGING FILES
                //Toast.makeText(MainActivity.this, "Error. Game's main activity does not exist", Toast.LENGTH_LONG).show();
                e.printStackTrace();
                return;
            }
        }
    }

    //Load mod menu
    public static void Start(final Context context) {
        //Check for permission
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(context)) {
            AlertDialog alertDialog = new AlertDialog.Builder(context)
                    .setTitle("No overlay permission")
                    .setMessage("Overlay permission is required in order to display the mod menu on top of the screen. Do you want to open the settings?")
                    .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            context.startActivity(new Intent("android.settings.action.MANAGE_OVERLAY_PERMISSION", Uri.parse("package:" + context.getPackageName())));
                            AlertDialog alertDialog1 = new AlertDialog.Builder(context)
                                    .setTitle("Restart Required!")
                                    .setMessage("Restarting the game is required after you have granted the permission inorder to display the mod menu on screen.")
                                    .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialog, int which) {
                                            System.exit(1);
                                        }
                                    }).setCancelable(false)
                                    .create();
                            alertDialog1.show();
                        }
                    })
                    .setNegativeButton("No", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            Toast.makeText(context.getApplicationContext(), "No permission granted. cannot load mod menu,", Toast.LENGTH_LONG).show();
                        }
                    }).setCancelable(false)
                    .create();
            alertDialog.show();
        } else {
            final Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    context.startService(new Intent(context, FloatingModMenuService.class));
                }
            }, 500);
        }

        //Use getApplicationContext() to fix dark background of Toast message
        CallToast(context);
    }

    public static void CallToast(final Context context) {
        new Handler().postDelayed(new Runnable() {
            public void run() {
                Toast(context.getApplicationContext());
            }
        }, 500);
    }
}
