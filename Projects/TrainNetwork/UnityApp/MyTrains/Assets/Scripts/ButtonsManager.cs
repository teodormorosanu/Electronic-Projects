using TMPro;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Networking;
using System.Collections;

public class ButtonsManager : MonoBehaviour
{
    private bool direction = true; // true = forward, false = backward
    private bool lights = false; // true = on, false = off
    private bool beams = false; // true = on, false = off
    private int throttle = 0;

    [SerializeField] private Slider throttleSlider;
    [SerializeField] private Button directionButton;
    [SerializeField] private TMP_Text throttlePercentText;
    [SerializeField] private TMP_Text currentDirectionText;
    [SerializeField] private TMP_Text lightsText;
    [SerializeField] private TMP_Text beamsText;
    [SerializeField] private WifiManager wifiManagerScript;

    public void ChangeDirection() {
        direction = !direction;
        currentDirectionText.text = direction ? "Current Direction: F" : "Current Direction: B";
        StartCoroutine(ChangeDirectionCoroutine());
    }

    public void ThrottleUpdate() {
        throttle = (int)throttleSlider.value;
        float throttlePercent = throttle / 10f;
        throttlePercentText.text = ((int)throttlePercent).ToString() + "%";
        StartCoroutine(ThrottleCoroutine());
    }

    public void ToggleLights() {
        lights = !lights;
        lightsText.text = lights ? "L:ON" : "L:OFF";
        StartCoroutine(ToggleLightsCoroutine());
    }

    public void ToggleBeams() {
        beams = !beams;
        beamsText.text = beams ? "B:ON" : "B:OFF";
        StartCoroutine(ToggleBeamsCoroutine());
    }

    private IEnumerator ChangeDirectionCoroutine() {
        string directionName = direction ? "forward" : "backward";
        string url = $"{wifiManagerScript.GetUrl()}/changeDirection?directionName={directionName}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator ToggleLightsCoroutine() {
        string lightsMode = lights ? "on" : "off";
        string url = $"{wifiManagerScript.GetUrl()}/lights?lightsMode={lightsMode}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator ToggleBeamsCoroutine() {
        string beamsMode = beams ? "on" : "off";
        string url = $"{wifiManagerScript.GetUrl()}/beams?beamsMode={beamsMode}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
    private IEnumerator ThrottleCoroutine() {
        string url = $"{wifiManagerScript.GetUrl()}/throttle?throttle={throttle}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
}
