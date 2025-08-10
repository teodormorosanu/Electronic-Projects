using System.Collections;
using System.Collections.Generic;
using UnityEngine.Networking;
using UnityEngine;

public class LocalManager : MonoBehaviour {

    public void WiFiNetworks() {
        #if UNITY_ANDROID
        try {
            using (AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer")) {
                AndroidJavaObject currentActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
                using (AndroidJavaObject intent = new AndroidJavaObject("android.content.Intent", "android.settings.WIFI_SETTINGS")) {
                    currentActivity.Call("startActivity", intent);
                }
            }
        }
        catch (System.Exception exception) {
            Debug.LogError("Failed to open Wi-Fi settings: " + exception.Message);
        }
        #endif
    }
    
}
