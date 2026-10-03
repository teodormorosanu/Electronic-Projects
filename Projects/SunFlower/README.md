# Sunflower

> **A autonomous self-watering system that aims to have a pleasant design and resolve the plants watering problem.**

## Overview

**Theme:**
*3D Printed Autonomous Vase, Embedded Systems, Hardware & Automation*.

**Objectives:**
- To address and resolve the **plants watering** problem.
- To implement a **reliable system** while keeping a pleasant and warm design.

---

**Description:**
This project was designed as a modern vase, combining automated plant watering with real-time monitoring and a modular build.

The most interesting parts of the project are the **`autonomous watering`**, **`control`**, and **`energy management`** systems. At the core of the build is the **`autonomous watering`** system. It uses a capacitive moisture sensor to continuously monitor the soil's humidity. Based on the value received from the sensor, it powers a water pump to water the plant from the built-in reservoir. To provide feedback without needing a screen, the vase features a custom LED display that indicates the current water level.

Once connected to the network, the hardware data becomes the foundation for the **`control`** system. The vase is connected over **`Wi-Fi`** and syncs with a phone app developed in Unity. The app's interface displays real-time data for soil moisture and water level, while providing controls to toggle the system between automatic and manual watering..

The **`energy management`** system aims to improve the device battery autonomy. A solar panel is used to charge the accumulators, extending the device uptime and reducing the need for frequent manual recharging. 

Around these core systems, the vase showcases a LED strip designed for two functionalities: offering warm ambient lighting for the room, while also offering a supplementary light for the plant when natural light is insufficient. The smart vase is monitored and configured through the **`mobile app`**.

## Project Parameters

| Total Cost | Time Invested | Status |
| :---: | :---: | :---: |
| **62€** | **1 month** | **✅ Completed** |

## Folder Structure

This project is self-contained. Below is a breakdown of the included resources:

  - `/Code` - Firmware, scripts, and source code.

    - `/Microcontroller` - The microcontroller part of code
    - `/UnityApp` - Unity app project (2022.3.12f1)

  - `/Cost` - Itemized bill of materials and total project expenses.

  - `/Design` - Complete mechanical and electrical design files.

    - `/3DModels` - 3D CAD files and slicer print profiles.
    - `/Assembly` - Assembly guides and exploded view diagrams.
    - `/Dimensions` - Dimensional drawings and mechanical specifications.
    - `/WiringDiagram` - Circuit schematics and electrical wiring diagrams.

  - `/Pictures` - High-resolution CAD renders and real life photos of the physical build.

## Bill of Materials (BOM)

For the complete, itemized component list and costs, see the `Cost` folder:

* **[Sunflower](./Cost)** - vase, core and components.

## **Tools Required:**

* FDM 3D printer (**minimum 256 x 256 mm bed**)
* Small screwdrivers set
* Wire strippers / cutters and tweezers
* Soldering iron and soldering wire
* Multimeter
* USB cable (**matching the ESP32 board's port**)
* PlatformIO (VS Code) or Arduino IDE
* Unity Editor

## Build Instructions

1. **Preparation**: Review the Bill of Materials in the `Cost` folder and acquire all necessary parts.

2. **3D Printing**: Print the vase pieces from `Design/3DModels` folder.

3. **Assembly**: Follow the assembly sheets in `Design/Assembly` to assemble the vase, mount the pump, and fit the electronics.

4. **Wiring**: Wire the pump motor, LEDs, and sensor modules according to `Design/WiringDiagram`.

5. **Software Setup**: Flash the firmware from `Code/Microcontroller` onto the ESP32 over USB for the initial setup. Connect to the vase's Wi-Fi access point and complete setup through the captive portal.

6. **Testing**: Verify the pump, LED's, and watering independently, then confirm the system work reliably.

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.