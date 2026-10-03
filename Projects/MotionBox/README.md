# Motion Box

> **❌ DEPRECATED: This project is no longer actively maintained. The documentation and files are displayed here only for archival and reference reasons.**

## Overview

**Theme:**
*3D Printed Motion Box, Embedded Systems, Hardware & Automation*.

**Objectives:**
- To address and resolve the **motion security** problem.
- To implement a **reliable and secure system** while keeping a pleasant and practical design.

---

**Description:**
This project was designed as a remote security system, implementing motion detection with physical alerts and automated cloud storage.

The most interesting parts of the project are the **`motion detection`**, **`remote alerting`**, and **`cloud displaying`** systems. At the base of the build is the **`motion detection`** system. Implemented in the transmitter module, it uses a PIR sensor to continuously monitor the environment for movement. Based on the signal received from the sensor, it initiates the security protocol to warn the user of a potential intrusion.

Once a presence is detected, the hardware data becomes the **`remote alerting`** system. The transmitter and receiver modules are connected over long distances using **`NRF antennas`**. The receiver unit translates this wireless data into feedback using a buzzer an LED signals, clearly indicating whether the zone is secure or if someone has trespassed.

The **`cloud displaying`** system aims to improve tracking and visual verification. An integrated camera on the transmitter captures a photo the moment it detects motion, uploading the image directly to a designated Google Drive folder to ensure the evidence is stored and accessible from anywhere.

## Project Parameters

| Total Cost | Time Invested | Status |
| :---: | :---: | :---: |
| **47€** | **2 months** | **❌ Deprecated** |

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.