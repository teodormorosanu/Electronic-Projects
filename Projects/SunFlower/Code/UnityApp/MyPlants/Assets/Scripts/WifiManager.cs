using UnityEngine;
using UnityEngine.Networking;
using System.Collections;
using TMPro;
using UnityEngine.UI;

public class WifiManager : MonoBehaviour {

    /* Variables */

    [SerializeField] private Slider waterLevelSlider;
    [SerializeField] private Slider moistureSlider;
    [SerializeField] private TMP_Text waterLevelText;
    [SerializeField] private TMP_Text moistureText;
    [SerializeField] private Image status;
    private float connectionTimer = 0f, waterUpdateTimer = 0f, moistureUpdateTimer = 0f;
    private string myPlantsUrl = "http://MyPlants.local";

    /* Main Methods */

    void Update() {
        if(connectionTimer >= 3) {
            StartCoroutine(ESP32Connection());
            connectionTimer = 0;
        }
        connectionTimer += Time.deltaTime;
        UpdateWaterLevel();
        UpdateMoisture();
    }

    /* Added Methods */

    void UpdateWaterLevel() {
        waterUpdateTimer += Time.deltaTime;
        if(waterUpdateTimer >= 1) {
            StartCoroutine(WaterStatusCoroutine());
            waterUpdateTimer = 0;
        }
    }

    void UpdateMoisture() {
        moistureUpdateTimer += Time.deltaTime;
        if(moistureUpdateTimer >= 1) {
            moistureUpdateTimer = 0;
            StartCoroutine(MoistureStatusCoroutine());
        } 
    }

    private IEnumerator ESP32Connection() {
        UnityWebRequest request = UnityWebRequest.Get($"{myPlantsUrl}/status");
        request.timeout = 2;
        yield return request.SendWebRequest();
        if (request.result == UnityWebRequest.Result.Success) {
            status.color = new Color(0, 1, 0);
        } else {
            status.color = new Color(1, 0, 0);
        }
    }
    private IEnumerator WaterStatusCoroutine() {
        UnityWebRequest request = UnityWebRequest.Get($"{myPlantsUrl}/waterStatus");
        request.timeout = 2;
        yield return request.SendWebRequest();
        if (request.result == UnityWebRequest.Result.Success) {
            string waterImportedString = $"{request.downloadHandler.text}";
            if (!string.IsNullOrEmpty(waterImportedString) && int.TryParse(waterImportedString, out int waterPercent)) {
                waterLevelSlider.value = waterPercent;
                waterLevelText.text = waterImportedString;
            }
        }
    }
    private IEnumerator MoistureStatusCoroutine() {
        UnityWebRequest request = UnityWebRequest.Get($"{myPlantsUrl}/moistureStatus");
        request.timeout = 2;
        yield return request.SendWebRequest();
        if (request.result == UnityWebRequest.Result.Success) {
            string moistureImportedString = $"{request.downloadHandler.text}";
            if (!string.IsNullOrEmpty(moistureImportedString) && int.TryParse(moistureImportedString, out int moisturePercent)) {
                moistureSlider.value = moisturePercent;
                moistureText.text = moistureImportedString;
            }
        }
    }
    public string GetMyPlantsUrl() {
        return myPlantsUrl;
    }
}
