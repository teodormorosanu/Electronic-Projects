// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

using UnityEngine;
using System;
using TMPro;

public class Speedometer : MonoBehaviour {

    // ----------------------------------------------------------------------
    // VARIABLES
    // ----------------------------------------------------------------------

    // --- Locomotive Control ---
    // private uint speed; // The train current speed (*REPLACE WITH THROTTLE*) 

    // --- Console UI Elements ---
    [SerializeField] GameObject speedometer; // The speedometer object
    [SerializeField] GameObject needleHolder; // The needle holder object
    [SerializeField] TMP_Text speedText; // The speedometer speed text
    [SerializeField] TMP_Text hourText; // The speedometer hour text
    [SerializeField] TMP_Text minutesText; // The speedometer minutes text
    [SerializeField] TMP_Text[] separators = new TMP_Text[2]; // The separators 

    // --- References ---
    [SerializeField] private ButtonsManager buttonsManager; // Buttons reference

    // ----------------------------------------------------------------------
    // CUSTOM METHODS
    // ----------------------------------------------------------------------

    void ShowSpeed() {
        // Showing the speed on the speedometer
        uint throttle = buttonsManager.GetThrottle();

        // Calculating the degrees and the speed based on the throttle value
        // (*TEMPORARY*) - needs to get speed from encoder!
        float degrees = (throttle / 1023f) * 143f - 120f;
        float speed = (throttle / 1023f) * 120f;

        // Updating UI elements accordingly
        needleHolder.transform.localEulerAngles = new Vector3(0, 0, -degrees);
        speedText.text = ((int)speed).ToString("000");
    }

    void ShowTime() {
        // Showing the time on the speedometer
        DateTime currentTime = DateTime.Now;

        // Updating UI elements accordingly
        hourText.text = currentTime.Hour.ToString("00");
        minutesText.text = currentTime.Minute.ToString("00");
        if(currentTime.Second % 2 == 0) {
            separators[0].text = "."; separators[1].text = ".";
        } else {
            separators[0].text = ""; separators[1].text = "";
        }
    }

    // ----------------------------------------------------------------------
    // CUSTOM METHODS
    // ----------------------------------------------------------------------

    void Update() {
        ShowSpeed();
        ShowTime();
    }
}
