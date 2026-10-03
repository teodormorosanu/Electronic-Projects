// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

using UnityEngine.Networking;
using System.Collections;
using UnityEngine.UI;
using UnityEngine;
using TMPro;
using UnityEngine.AI;

public class ButtonsManager : MonoBehaviour {

    // ----------------------------------------------------------------------
    // VARIABLES
    // ----------------------------------------------------------------------

    // --- Locomotive Control ---
    private char direction = 'F'; // Train direction ('F' = forward,
                                  // 'B' = backward, 'S' = brake)
    private bool lights = false; // Train lights mode (true = on, false = off)
    private bool beams = false; // Train beams mode (true = on, false = off)
    private bool autoMapping = false; // Train auto mapping feature
                                      // (true = on, false = off)
    private uint throttle = 0; // Train current motor speed (0 - 1023)

    // --- Console UI Elements ---
    [SerializeField] private Slider throttleSlider; // Slider for throttle
    [SerializeField] private Button directionButton; // Button for direction
    [SerializeField] private GameObject[] switches = new GameObject[4];
    // List of console switches
    [SerializeField] private Image[] switchHolders = new Image[4];
    // List of console switch holders
    [SerializeField] private TMP_Text throttlePercentText; // Text holder for
                                                           // throttle 

    // --- References ---
    [SerializeField] private NetworkManager network; // Network reference

    // ----------------------------------------------------------------------
    // CUSTOM FUNCTIONS
    // ----------------------------------------------------------------------

    public void UpdateThrottle() {
        // Getting the wanted throttle from UI and sending it
        throttle = (uint)throttleSlider.value;
        double throttlePercent = throttle / 10.23f;
        throttlePercentText.text = ((int)throttlePercent).ToString() + "%";
        StartCoroutine(UpdateThrottleCoroutine());
    }

    public void UpdateSwitchUI(GameObject sw, Image swHolder, bool trigger) {
        // Updating switch UI based on a trigger
        Vector3 swPos = sw.transform.localPosition;
        float newSwYPos = (trigger) ? swPos.y - 58f : swPos.y + 58f;

        sw.transform.localPosition = new Vector3(0f, newSwYPos, 0f);
        sw.transform.Rotate(new Vector3(0f, 0f, 180));

        swHolder.color = (trigger) ? new Color(0.7f, 0.21f, 0.21f) :
                                     new Color(0.08f, 0.08f, 0.08f);
    }

    public void ToggleLights() {
        // Toggling the lights from UI and sending it
        lights = !lights;
        UpdateSwitchUI(switches[0], switchHolders[0], lights);
        StartCoroutine(ToggleLightsCoroutine());
    }

    public void ToggleBeams() {
        // Toggling the beams from UI and sending it
        beams = !beams;
        UpdateSwitchUI(switches[1], switchHolders[1], beams);
        StartCoroutine(ToggleBeamsCoroutine());
    }

    public void ToggleDirection() {
        // Toggling the direction from UI and sending it
        if (direction == 'F') {
            direction = 'B';
        }
        else {
            direction = 'F';
        }
        UpdateSwitchUI(switches[2], switchHolders[2], direction == 'B');
        StartCoroutine(UpdateDirectionCoroutine());
    }

    public void ToggleAutoMapping() {
        // Toggling the auto mapping from UI and sending it
        autoMapping = !autoMapping;
        UpdateSwitchUI(switches[3], switchHolders[3], autoMapping);
        StartCoroutine(ToggleAutoMappingCoroutine());
    }

    public void ToggleObject(GameObject obj) {
        // Toggling an object (active/inactive)
        obj.SetActive(!obj.activeSelf);
    }

    // ----------------------------------------------------------------------
    // COROUTINES
    // ----------------------------------------------------------------------

    private IEnumerator UpdateDirectionCoroutine() {
        // Sending the wanted direction
        string url = $"{network.GetUrl()}/direction?direction={direction}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }

    private IEnumerator UpdateThrottleCoroutine() {
        // Sending the wanted throttle
        string url = $"{network.GetUrl()}/throttle?throttle={throttle}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }

    private IEnumerator ToggleLightsCoroutine() {
        // Sending the lights
        string lightsState = lights ? "on" : "off";
        string url = $"{network.GetUrl()}/lights?lights={lightsState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }

    private IEnumerator ToggleBeamsCoroutine() {
        // Sending the beams
        string beamsState = beams ? "on" : "off";
        string url = $"{network.GetUrl()}/beams?beams={beamsState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }

    private IEnumerator ToggleAutoMappingCoroutine() {
        // Sending the beams
        string mappingState = autoMapping ? "on" : "off";
        string url = $"{network.GetUrl()}/mapping?mapping={mappingState}";
        UnityWebRequest request = UnityWebRequest.Get(url);
        request.timeout = 2;
        yield return request.SendWebRequest();
    }

    // ----------------------------------------------------------------------
    // GETTERS
    // ----------------------------------------------------------------------

    public uint GetThrottle() {
        return throttle;
    }
}
