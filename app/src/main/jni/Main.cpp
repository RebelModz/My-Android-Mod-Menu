/*
 * Credits:
 *
 * Octowolve - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu
 * And hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 * VanHoevenTR A.K.A Nixi: https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
 * MrIkso - Mod menu: https://github.com/MrIkso/FloatingModMenu
 * Rprop - https://github.com/Rprop/And64InlineHook
 * MJx0 A.K.A Ruit - KittyMemory: https://github.com/MJx0/KittyMemory
 * */
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu.h"

#if defined(__aarch64__) //Compile for arm64 lib only
#include <And64InlineHook/And64InlineHook.hpp>
#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#endif

// fancy struct for patches for kittyMemory
struct My_Patches {
    // let's assume we have patches for these functions for whatever game
    // like show in miniMap boolean function
    MemoryPatch GodMode, GodMode2, SliderExample;
    // etc...
} hexPatches;

bool feature1 = false, feature2 = false, featureHookToggle = false;
int sliderValue = 1;
void *instanceBtn;

// Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
// If you putted getAbsoluteAddress here, the lib tries to read the address without il2cpp loaded,
// will result in a null pointer which will cause crash
// See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
void (*AddMoneyExample)(void *instance, int amount);

//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

// we will run our patches in a new thread so our while loop doesn't block process main thread
// Don't forget to remove or comment out logs before you compile it.

//KittyMemory Android Example: https://github.com/MJx0/KittyMemory/blob/master/Android/test/src/main.cpp
//Use ARM Converter to convert ARM to HEX: https://armconverter.com/
//Note: We use OBFUSCATE_KEY for offsets which is the important part xD

// Hooking example

bool (*old_get_BoolExample)(void *instance);
bool get_BoolExample(void *instance) {
    if (instance != NULL && featureHookToggle) {
        return true;
    }
    return old_get_BoolExample(instance);
}

float (*old_get_FloatExample)(void *instance);

float get_FloatExample(void *instance) {
    if (instance != NULL && sliderValue > 1) {
        return (float) sliderValue;
    }
    return old_get_FloatExample(instance);
}

void (*old_Update)(void *instance);

void Update(void *instance) {
    instanceBtn = instance;
    old_Update(instance);
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__) //Compile for arm64 lib only
    // New way to patch hex via KittyMemory without need to  specify len. Spaces or without spaces are fine
    // ARM64 assembly example
    // MOV X0, #0x0 = 00 00 80 D2
    // RET = C0 03 5F D6
    hexPatches.GodMode = MemoryPatch::createWithHex(targetLibName,
                                                    string2Offset(OBFUSCATE_KEY("0x123456", '3')),
                                                    OBFUSCATE("00 00 80 D2 C0 03 5F D6"));
    //You can also specify target lib like this
    hexPatches.GodMode2 = MemoryPatch::createWithHex("libtargetLibHere.so",
                                                     string2Offset(OBFUSCATE_KEY("0x222222", 'g')),
                                                     OBFUSCATE("20 00 80 D2 C0 03 5F D6"));

    // Offset Hook example
    //A64HookFunction((void *) getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE_KEY("0x123456", 'l'))), (void *) get_BoolExample,
    //                (void **) &old_get_BoolExample);

    // Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
    // See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
    AddMoneyExample = (void(*)(void *,int))getAbsoluteAddress(targetLibName, 0x123456);

#else //Compile for armv7 lib only. Do not worry about greyed out highlighting code, it still works

    // New way to patch hex via KittyMemory without need to specify len. Spaces or without spaces are fine
    // ARMv7 assembly example
    // MOV R0, #0x0 = 00 00 A0 E3
    // BX LR = 1E FF 2F E1
    hexPatches.GodMode = MemoryPatch::createWithHex(targetLibName,
                                                    string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                    OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));
    //You can also specify target lib like this
    hexPatches.GodMode2 = MemoryPatch::createWithHex("libtargetLibHere.so",
                                                     string2Offset(OBFUSCATE_KEY("0x222222", 'g')),
                                                     OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
    //Can apply patches directly here without need to use switch
    //hexPatches.GodMode.Modify();
    //hexPatches.GodMode2.Modify();

    // Offset Hook example
    //MSHookFunction((void *) getAbsoluteAddress(targetLibName,
    //               string2Offset(OBFUSCATE_KEY("0x123456", '?'))),
    //               (void *) get_BoolExample, (void **) &old_get_BoolExample);

    // Symbol hook example (untested). Symbol/function names can be found in IDA if the lib are not stripped. This is not for il2cpp games
    //MSHookFunction((void *) ("__SymbolNameExample"), (void *) get_BoolExample, (void **) &old_get_BoolExample);

    // Function pointer splitted because we want to avoid crash when the il2cpp lib isn't loaded.
    // See https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
    AddMoneyExample = (void (*)(void *, int)) getAbsoluteAddress(targetLibName, 0x123456);

    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}

//JNI calls
extern "C" {
JNIEXPORT void JNICALL
Java_com_rebelmodz_MainActivity_Toast(JNIEnv *env, jclass obj, jobject context) {
    MakeToast(env, context, OBFUSCATE("Modded by RebelModz [YouTube]"), Toast::LENGTH_LONG); //This toast will show at the beginning
}

// Note:
// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_The Check Box
// To learn HTML, go to this page: https://www.w3schools.com/

JNIEXPORT jobjectArray
JNICALL
Java_com_rebelmodz_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_MOD MENU FEATURES"), //Not counted
            OBFUSCATE("Toggle_The toggle"), //Starts with 0
            OBFUSCATE("100_Toggle_True_The toggle 2"), //This one have feature number assigned, and switched on by default
            OBFUSCATE("110_Toggle_The toggle 3"), //This one too
            OBFUSCATE("IntSeekBar_The slider_1_100"), //Assigned numbers are not counted, so 1
            OBFUSCATE("FloatSeekBar_The slider_2.4_10.5"),
            OBFUSCATE("IntSeekBar_Kittymemory slider example_1_5"), //2
            OBFUSCATE("Spinner_The spinner_Items 1,Items 2,Items 3"), //3
            OBFUSCATE("Button_The button"), //4
            OBFUSCATE("ButtonLink_The button with link_https://www.youtube.com/"), //Not counted
            OBFUSCATE("ButtonOnOff_The On/Off button"), //5
            OBFUSCATE("CheckBox_The Check Box"),
            OBFUSCATE("InputValue_Input number"),
            OBFUSCATE("InputText_Input text"),
            OBFUSCATE("RadioButton_Radio buttons_OFF,Mod 1,Mod 2,Mod 3"),
            OBFUSCATE(
                    "RichTextView_This is text view, not fully HTML."
                    "<b>Bold</b> <i>italic</i> <u>underline</u>"
                    "<br />New line <font color='red'>Support colors</font>"
                    "<br/><big>bigger Text</big>"),
            OBFUSCATE(
                    "RichWebView_<html><head><style>body{color: white;}</style></head><body>"
                    "This is WebView, with REAL HTML support!"
                    "<div style=\"background-color: darkblue; text-align: center;\">Support CSS</div>"
                    "<marquee style=\"color: green; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">This is <u>scrollable</u> text</marquee>"
                    "</body></html>")
    };

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_com_rebelmodz_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {
    //Convert java string to c++
    const char *featureName = env->GetStringUTFChars(featName, 0);
    const char *TextInput;
    if (str != NULL)
        TextInput = env->GetStringUTFChars(str, 0);
    else
        TextInput = "Empty";

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         featureName, value,
         boolean, TextInput);

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 0:
            feature2 = boolean;
            if (feature2) {
                // To print bytes you can do this
                //if (hexPatches.GodMode.Modify()) {
                //    LOGD(OBFUSCATE("Current Bytes: %s"),
                //         hexPatches.GodMode.get_CurrBytes().c_str());
                //}
                hexPatches.GodMode.Modify();
                hexPatches.GodMode2.Modify();
                //LOGI(OBFUSCATE("On"));
            } else {
                hexPatches.GodMode.Restore();
                hexPatches.GodMode2.Restore();
                //LOGI(OBFUSCATE("Off"));
            }
            break;
        case 100:
            break;
        case 110:
            break;
        case 1:
            if (value >= 1) {
                sliderValue = value;
            }
            break;
        case 2:
            switch (value) {
                //For noobies
                case 0:
                    hexPatches.SliderExample = MemoryPatch::createWithHex(
                            targetLibName, string2Offset(
                                    OBFUSCATE_KEY("0x100000", 't')),
                            OBFUSCATE(
                                    "00 00 A0 E3 1E FF 2F E1"));
                    hexPatches.SliderExample.Modify();
                    break;
                case 1:
                    hexPatches.SliderExample = MemoryPatch::createWithHex(
                            targetLibName, string2Offset(
                                    OBFUSCATE_KEY("0x100000",
                                                  'b')),
                            OBFUSCATE(
                                    "01 00 A0 E3 1E FF 2F E1"));
                    hexPatches.SliderExample.Modify();
                    break;
                case 2:
                    hexPatches.SliderExample = MemoryPatch::createWithHex(
                            targetLibName,
                            string2Offset(
                                    OBFUSCATE_KEY("0x100000",
                                                  'q')),
                            OBFUSCATE(
                                    "02 00 A0 E3 1E FF 2F E1"));
                    hexPatches.SliderExample.Modify();
                    break;
            }
            break;
        case 3:
            switch (value) {
                case 0:
                    LOGD(OBFUSCATE("Selected item 1"));
                    break;
                case 1:
                    LOGD(OBFUSCATE("Selected item 2"));
                    break;
                case 2:
                    LOGD(OBFUSCATE("Selected item 3"));
                    break;
            }
            break;
        case 4:
            // Since we have instanceBtn as a field, we can call it out of Update hook function
            // See more https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
            if (instanceBtn != NULL)
                AddMoneyExample(instanceBtn, 999999);
            break;
        case 5:
            break;
        case 6:
            featureHookToggle = boolean;
            break;
        case 7:
            break;
        case 8:
            MakeToast(env, obj, TextInput, Toast::LENGTH_SHORT);
            break;
        case 9:
            break;
    }
}
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    return JNI_VERSION_1_6;
}
 */
