// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

using UnityEngine.Networking;
using System.Collections;
using UnityEngine.UI;
using UnityEngine;
using TMPro;

public class NetworkManager : MonoBehaviour {

    // ----------------------------------------------------------------------
    // VARIABLES
    // ----------------------------------------------------------------------

    // --- Console UI elements ---
    [SerializeField] private Image statusImage; // Image for wifi status
    [SerializeField] private TMP_InputField ipField; // IP input field

    // --- Network Settings ---
    private float connectionTimer = 0f; // Timer for checking the connection
    private string url; // Train connection IP

    // ----------------------------------------------------------------------
    // CUSTOM METHODS
    // ----------------------------------------------------------------------

    public void AddIp() {
        // Adding an IP to the url
        url = "http://" + ipField.text;
    }

    private IEnumerator ConnectTrain() {
        // Connecting to a train using a specific URL
        UnityWebRequest request = UnityWebRequest.Get($"{url}/status");
        request.timeout = 2;
        yield return request.SendWebRequest();

        // Showing the user if the connection is available
        if (request.result == UnityWebRequest.Result.Success) {
            statusImage.color = new Color(0, 1, 0);
            string newIP = request.downloadHandler.text;
            if (!string.IsNullOrEmpty(newIP) && newIP != "Not connected") {
                url = "http://" + newIP;
            }
        }
        else {
            statusImage.color = new Color(1, 0, 0);
        }
    }

    // ----------------------------------------------------------------------
    // MAIN METHODS
    // ----------------------------------------------------------------------

    void Update() {
        // Trying to connect continously
        connectionTimer += Time.deltaTime;
        if (connectionTimer >= 1.5f) {
            StartCoroutine(ConnectTrain());
            connectionTimer = 0f;
        }
    }

    // ----------------------------------------------------------------------
    // GETTERS
    // ----------------------------------------------------------------------

    public string GetUrl() {
        return url;
    }
}
