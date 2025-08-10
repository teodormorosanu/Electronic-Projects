using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using UnityEngine.EventSystems;

public class ButtonsManager : MonoBehaviour, IPointerUpHandler {

    /* Variables */

    [SerializeField] private Animator colorPickerCover;
    [SerializeField] private Animator brightnessSliderBackground;
    [SerializeField] private Animator ledOn;
    [SerializeField] private Animator ledOff;
    [SerializeField] private Animator onoffButtons;
    [SerializeField] private Animator autoWaterButtonHolder;
    [SerializeField] private Animator autoWaterButton;
    [SerializeField] private Slider brightnessSlider;
    [SerializeField] private Image brightnessSliderFill;
    [SerializeField] private WifiManager wifiManagerScript;
    [SerializeField] private GameObject wateringButton;
    private static bool ON = false;
    private static bool auto = false;
    private static bool watering = false;

    /* Main Methods */

    public void Update() {
        brightnessSliderFill.color = new Color(brightnessSliderFill.color.r, brightnessSliderFill.color.g, brightnessSliderFill.color.b, brightnessSlider.value / 255f);
    }

    /* Added Methods */

    private void ColorPickerAnimationsController() {
        colorPickerCover.SetBool("on", ON);
        brightnessSliderBackground.SetBool("on", ON);
        brightnessSlider.enabled = ON;
        ledOn.SetBool("on", ON);
        ledOff.SetBool("on", !ON);
        onoffButtons.SetBool("on", ON);
    }
    public void StripOnOffButton(int index) {
        ON = index != 0;
        ColorPickerAnimationsController();
        StartCoroutine(StripOnOffCoroutine());
    }
    public void BrightnessSlider() {
        StartCoroutine(BrightnessSliderCoroutine());
    }

    public void AutoWateringButton() {
        auto = !auto;
        autoWaterButtonHolder.SetBool("auto", auto);
        autoWaterButton.SetBool("auto", auto);
        StartCoroutine(AutoWateringCoroutine());
    }

    public void WateringButton() {
        watering = !watering;
        auto = false;
        autoWaterButtonHolder.SetBool("auto", auto);
        autoWaterButton.SetBool("auto", auto);
        StartCoroutine(WateringCoroutine());
    }

    private void WiFiNetworks() {
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

    public void NewAp() {
        StartCoroutine(NewApCoroutine());
        //WiFiNetworks();
    }


    private IEnumerator StripOnOffCoroutine() {
        string ledStripState = (ON) ? "on" : "off";
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/stripOnOff?ledStripState={ledStripState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator BrightnessSliderCoroutine() {
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/brightness?brightnessValue={brightnessSlider.value.ToString()}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator AutoWateringCoroutine() {
        string autoState = (auto) ? "on" : "off";
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/autoWatering?autoState={autoState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator WateringCoroutine() {
        string wateringState = (watering) ? "on" : "off";
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/watering?wateringState={wateringState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator NewApCoroutine() {
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/newAp?newApState={"on"}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    public void OnPointerUp(PointerEventData eventData) {
        if (eventData.pointerEnter == brightnessSlider.gameObject) {
            StartCoroutine(BrightnessSliderCoroutine());
        }
    }

    // public void OnPointerDown(PointerEventData eventData) {
    //     if(eventData.pointerEnter == wateringButton.gameObject) {
    //         Debug.Log("Intrat");
    //         auto = false;
    //         watering = true;
    //         autoWaterButtonHolder.SetBool("auto", auto);
    //         autoWaterButton.SetBool("auto", auto);
    //         StartCoroutine(WateringCoroutine());
    //     }
    // }

    /* Getters */

    public bool GetON() {
        return ON;
    }
}


