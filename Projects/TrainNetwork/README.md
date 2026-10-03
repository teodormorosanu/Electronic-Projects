# TrainNetwork

> **A fully 3D-printed, 1:50 scale model train network that aims to significantly reduce the cost of commercially available train sets.**

## Overview

**Theme:**
*3D-Printed Model Railroading, Embedded Systems, Hardware & Automation*.

**Objectives:**
- To address the **high cost** of commercially available train sets on the market.
- To build a model train network where every piece of the assembly is **fully 3D printed** (or as much as possible).
- To implement a **reliable system** that simulates real life and also showcases new features (e.g. autonomous system, digital mapping, train driving simulation).

---

**Description:**
This project started as a fairly standard model railroading build: a 3D-printed track system, custom wagons and locomotives, chassis, buffers, a rerailer, and a set of switches and intersections. Along the way, the track curvature was redesigned for smoother, more realistic rolling, and the switches were upgraded from simple crossings to proper manual turnouts, with electric control planned as a future step.

The more interesting parts of the project are the **`autonomous`**, **`digital mapping`**, and **`driving simulation`** systems. At the core of all three is the digital mapping system **(🚧 In progress)**. The original approach used four optical sensors reading printed grayscale markers on the track ballast (a kind of barcode for each piece). It never worked reliably: ambient light, thermal drift in the sensors, and electrical noise from the motors made consistent readings practically impossible, even after extensive hardware and software tuning. The idea was eventually replaced with **`RFID`**: a single tag embedded in the ballast of each track piece, read by a module mounted under the locomotive. To avoid needing two tags per curved piece (one for each direction), a gyroscope was added to the locomotive, which determines whether a curve bends left or right by integrating yaw during the crossing, collapsing the whole track catalog down to one tag per physical piece.

Once a layout is mapped, that digital track data becomes the foundation for the other two systems. The **`autonomous system`** **(🚧 In progress)** will add signaling and let the locomotive drive itself along the mapped route, making routing and stopping decisions without manual throttle input. 

The **`driving simulation system`** **(🚧 In progress)** takes the opposite approach: a physical cabin console that mirrors a real locomotive's controls, letting a person drive the train manually while the console reflects the mapped track in real time.

Around these core systems, the locomotive also runs its own captive portal for **`Wi-Fi`** setup, supports **`OTA`** firmware updates over the network, drives direction-aware lighting, and plays motor and horn sounds through a small onboard speaker. The project is controlled through a companion **`mobile app`**.

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

* **[Infrastructure](./Infrastructure/Cost)** - track pieces, switches, ballast, scenery.
* **Rolling Stock** - **[locomotives](./RollingStock/Locomotives/Cost)**, **[wagons](./RollingStock/Wagons/Cost)**.

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

6. **Testing**: Verify motors, lights, and sound independently, then confirm RFID tags are read consistently before running the train network.

## Future Improvements

- [ ] Electrically controllable switches, replacing the current manual turnouts.
- [ ] Finished digital cabin console with a live, rendered view of the mapped track.
- [ ] Polished companion app UI (speedometer, toggle switches currently functional but not visually refined).

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.