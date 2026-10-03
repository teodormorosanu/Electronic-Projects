# 8.4V Charger

> **A power management circuit that aims to solve the charging problem of an 2S 18650 battery configuration.**

## Overview

**Theme:**
*3D Printed 8.4V Charger, Hardware*.

**Objectives:**
- To address and resolve the **2S 18650 charging** problem.
- To implement a **reliable system** while keeping a pleasant and practical design.

---

**Description:**
This project was designed as a simple and minimalist battery charger, combining universal power input with a dedicated 8.4V output for custom battery packs.

The most interesting parts of the project are the **`power delivery`**, **`charging management`**, and **`status display`** systems. At the base of the build is the **`charging management`** system. It aims to safely charge 2S 18650 lithium-ion battery modules at 8.4V. The output functionality is kept simple and it uses a standard round jack connector to plug directly into the 2S module.

The **`power delivery`** system aims to supply power using an USB Type-C port.

Around these base systems, the charger showcases a **`status display`** designed to providehardware feedback. It relies on two LEDs to indicate the current state of the device: whether the battery is actively charging or if the charging has successfully finished.

## Project Parameters

| Total Cost | Time Invested | Status |
| :---: | :---: | :---: |
| **6€** | **1 week** | **✅ Completed** |

## Folder Structure

This project is self-contained. Below is a breakdown of the included resources:

  - `/Cost` - Itemized bill of materials and total project expenses.

  - `/Design` - Complete mechanical and electrical design files.

    - `/3DModels` - 3D CAD files and slicer print profiles.
    - `/Assembly` - Assembly guides and exploded view diagrams.
    - `/Dimensions` - Dimensional drawings and mechanical specifications.
    - `/WiringDiagram` - Circuit schematics and electrical wiring diagrams.

  - `/Pictures` - High-resolution CAD renders and real life photos of the physical build.

## Bill of Materials (BOM)

For the complete, itemized component list and costs, see the `Cost` folder:

* **[8.4V_Charger](./Cost)** - charger case and components.

## **Tools Required:**

* FDM 3D printer (**minimum 180 x 180 mm bed**)
* Small screwdrivers set
* Wire strippers / cutters and tweezers
* Soldering iron and soldering wire
* Multimeter

## Build Instructions

1. **Preparation**: Review the Bill of Materials in the `Cost` folder and acquire all necessary parts.

2. **3D Printing**: Print the charger case from `Design/3DModels` folder.

3. **Assembly**: Follow the assembly sheets in `Design/Assembly` to assemble the charger and fit the electronics.

4. **Wiring**: Wire the components according to `Design/WiringDiagram`.

5. **Testing**: Verify the charging ports, LEDs and actual charging functionality, then confirm the system work reliably.

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.