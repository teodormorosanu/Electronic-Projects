using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.EventSystems;
using UnityEngine.Networking;

public class ColorPicker : MonoBehaviour
{
    [SerializeField] private Texture2D colorChart; // Color texture chart
    [SerializeField] private GameObject chart; // Parent chart GameObject
    [SerializeField] private RectTransform cursor; // Cursor RectTransform
    [SerializeField] private Image cursorColor; // Cursor color indicator
    [SerializeField] private ButtonsManager buttonsManagerScript;
    [SerializeField] private WifiManager wifiManagerScript;

    private Vector2 lastValidPosition; // To track the last valid cursor position

    public void ONOFFPickColor(BaseEventData data) {
        if(buttonsManagerScript.GetON()) {
            PickColor(data);
        }
    }

    private void PickColor(BaseEventData data) {
        PointerEventData pointer = data as PointerEventData;
        RectTransform chartRect = chart.GetComponent<RectTransform>();
        RectTransformUtility.ScreenPointToLocalPointInRectangle(chartRect, pointer.position, null, out Vector2 localPoint);
        localPoint += chartRect.sizeDelta / 2f;
        Vector2 center = chartRect.sizeDelta / 2f;
        float radius = Mathf.Min(chartRect.sizeDelta.x, chartRect.sizeDelta.y) / 2f;
        if (Vector2.Distance(localPoint, center) > radius) {
            localPoint = lastValidPosition;
        } else {
            lastValidPosition = localPoint;
        }
        Vector2 offset = localPoint - center;
        if (offset.magnitude > radius) {
            offset = offset.normalized * radius;
            localPoint = center + offset;
        }
        cursor.anchoredPosition = localPoint - chartRect.sizeDelta / 2f;
        float texCoordX = (localPoint.x / chartRect.sizeDelta.x) * colorChart.width;
        float texCoordY = (localPoint.y / chartRect.sizeDelta.y) * colorChart.height;
        int x = Mathf.Clamp((int)texCoordX, 0, colorChart.width - 1);
        int y = Mathf.Clamp((int)texCoordY, 0, colorChart.height - 1);
        Color pickedColor = colorChart.GetPixel(x, y);
        cursorColor.color = pickedColor;
        StartCoroutine(PickedColorCoroutine(pickedColor));
    }
    private IEnumerator PickedColorCoroutine(Color pickedColor) {
        string stripColor = (int)(pickedColor.r * 255) + "," + (int)(pickedColor.g * 255) + " " + (int)(pickedColor.b * 255);
        Debug.Log(stripColor);
        string url = $"{wifiManagerScript.GetMyPlantsUrl()}/pickedColor?stripColor={stripColor}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }
}
