# Train Network

> **A fully 3D-printed, 1:50 scale train network that aims to significantly reduce the cost of commercially available train sets.**

## Overview

**Theme:**
*3D Printed Model Railroading, Embedded Systems, Hardware & Automation*.

**Objectives:**
- To address the **high cost** of commercially available train sets on the market.
- To build a model train network where every piece of the assembly is **fully 3D printed** (or as much as possible).
- To implement a **reliable system** that simulates real life and also showcases new features (e.g. autonomous system, digital mapping, train driving simulation).

---

**Description:**
This project started as a standard model railroading build: a 3D-printed track system, custom wagons and locomotives, chassis, buffers, a rerailer, and a set of switches and intersections. After initial tests, the track curvature was redesigned for smoother, more realistic rolling, and the switches were upgraded from simple crossings to manual turnouts, with electric control planned as a future step.

The more interesting parts of the project are the **`autonomous`**, **`digital mapping`**, and **`driving simulation`** systems. At the base of all three is the digital mapping system **(🚧 In progress)**. The first approach used optical sensors reading printed markers on the track ballast (a kind of barcode for each piece). It never worked reliably: ambient light, drift in the sensors, and huge noise from the motors made consistent readings impossible, even after extensive hardware and software tuning. The idea was replaced with **`RFID`**: a single tag embedded in the ballast of each track, read by a module placed under the locomotive. To avoid needing two tags for each curved piece (one for each direction), a gyroscope was added to the locomotive, which determines whether a curve is left or right by integrating yaw during the crossing, simplifying the track types down to one tag per piece.

Once a layout is mapped, that digital track data becomes the core for the other two systems. The **`autonomous system`** **(🚧 In progress)** will add signaling and let the locomotive drive autonomous along the route, making routing and stopping decisions without manual throttle input. 

The **`driving simulation system`** **(🚧 In progress)** takes is the opposite: a physical cabin console that mirrors real locomotive's controls, letting a user drive the train while the console reflects the mapped track in real time.

Around these base systems, the locomotive also runs its own captive portal for **`Wi-Fi`** setup, supports **`OTA`** firmware updates over the network, drives direction lighting, and plays motor and horn sounds using a smallspeaker. The project is controlled through a **`mobile app`**.

## Project Parameters

| Total Cost | Time Invested | Status |
| :---: | :---: | :---: |
| Estimated at **400€** | **24 months** | **🚧 In progress** |

## Folder Structure

This project is self-contained. Below is a breakdown of the included resources:

- **`./Infrastructure`**:

  - `/Cost` - Itemized bill of materials and total infrastructure expenses.

  - `/Design` - Complete mechanical and electrical design files.

    - `/3DModels` - 3D CAD files and slicer print profiles.
    - `/Assembly` - Assembly guides and exploded view diagrams.
    - `/Dimensions` - Dimensional drawings and mechanical specifications.

  - `/Pictures` - High-resolution CAD renders and real life photos of the physical build.


- **`./RollingStock`**:

  - `/Extra`, `/Wagons` and `/Locomotives` folders:

    - `/Code` - Firmware, scripts, and microcontroller code (depending on the folder).

    - `/Cost` - Itemized bill of materials and total rolling stock expenses.

    - `/Design` - Complete mechanical and electrical design files.

      - `/3DModels` - 3D CAD files and slicer print profiles.
      - `/Assembly` - Assembly guides and exploded view diagrams.
      - `/Dimensions` - Dimensional drawings and mechanical specifications.
      - `/WiringDiagram` - Circuit schematics and electrical wiring diagrams.

    - `/Pictures` - High-resolution CAD renders and real life photos of the physical build.

- **`./UnityApp`**:

  - `/MyTrains` - Unity app project (6000.3.10f1).

## Bill of Materials (BOM)

For the complete, itemized component list and costs, see the `Cost` folder:

* **[Infrastructure](./Infrastructure/Track/Cost)** - track pieces, switches, ballast, scenery.
* **Rolling Stock** - **[locomotives](./RollingStock/Locomotives/EA-236/Cost)**, **[wagons](./RollingStock/Wagons/CFR-2176/Cost)**.

## **Tools Required:**

* FDM 3D printer (**minimum 180 x 180 mm bed**)
* Small screwdrivers set
* Wire strippers / cutters and tweezers
* Soldering iron and soldering wire
* Multimeter
* USB cable (**matching the ESP32 board's port**)
* PlatformIO (VS Code) or Arduino IDE
* Unity Editor

## Build Instructions

1. **Preparation**: Review the Bill of Materials in the `Cost` folder (`Infrastructure` and `RollingStock`) and acquire all necessary parts.

2. **3D Printing**: Print the track pieces from `Infrastructure/Design/3DModels`, and the wagon / locomotive parts from the corresponding `RollingStock/.../Design/3DModels` folder.

3. **Assembly**: Follow the assembly sheets in `Design/Assembly` to assemble the wagon / locomotive, mount the motors, and fit the electronics.

4. **Wiring**: Wire the motors, LEDs, and sensor modules according to `Design/WiringDiagram`.

5. **Software Setup**: Flash the firmware from `RollingStock/Locomotives/<model>/Code` onto the ESP32 over USB for the initial setup. Connect to the locomotive's Wi-Fi access point and complete setup through the captive portal; use the onboard OTA page for all future firmware updates (**be aware** - disconnect the sound module from the TX/RX for the initial USB flash).

6. **Testing**: Verify the motors, lights, and sound independently, then confirm RFID tags are read consistently before running the train network.

## Future Improvements

- [ ] Electrically controllable switches, replacing the current manual turnouts.
- [ ] Finished cabin console with a live view of the mapped track.
- [ ] Polished app UI (speedometer, toggle switches currently functional but not visually refined).

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.