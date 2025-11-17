using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using System.Collections;

public class WifiManager : MonoBehaviour {
    [SerializeField] private Image wifiStatusImage;
    private float connectionTimer = 0f;
    private string url = "http://192.168.137.90";

    void Update() {
        connectionTimer += Time.deltaTime;
        if (connectionTimer >= 1.5f)
        {
            StartCoroutine(ESP32Connection());
            connectionTimer = 0f;
        }
    }

    public void OpenWifiSettings() {
    #if UNITY_ANDROID
        try {
            using (AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer"))
            {
                AndroidJavaObject currentActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
                using (AndroidJavaObject intent = new AndroidJavaObject("android.content.Intent", "android.settings.WIFI_SETTINGS"))
                {
                    currentActivity.Call("startActivity", intent);
                }
            }
        }
        catch (System.Exception exception) {
            Debug.LogError("Failed to open Wi-Fi settings: " + exception.Message);
        }
    #endif
    }

    private IEnumerator ESP32Connection() {
        UnityWebRequest request = UnityWebRequest.Get($"{url}/status");
        request.timeout = 2;
        yield return request.SendWebRequest();
        if (request.result == UnityWebRequest.Result.Success) {
            wifiStatusImage.color = new Color(0, 1, 0);
        }
        else {
            wifiStatusImage.color = new Color(1, 0, 0);
        }
    }

    public string GetUrl() {
        return url;
    }
}
