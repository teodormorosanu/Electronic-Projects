# Laser Pointer

> **A simple laser pointer that aims to have a pleasant design.**

## Overview

**Theme:**
*3D Printed Laser Pointer, Hardware*.

**Objectives:**
- To implement a **reliable system** while keeping a pleasant and practical design.

---

**Description:**
This project was designed as a simple minimalist laser pointer, combining a standard optical output with high-capacity energy storage.

The most interesting parts of the project are the **`optical output`**, **`energy management`**, and **`status display`** systems. At the base of the build is the **`optical output`** system. It uses a reliable 5mW red laser diode to project a focused beam. The functionality is kept simple, operated by a single button to activate the laser.

The **`energy management`** system aims to improve the device's battery autonomy compared to standard alternatives. When it needs charging, the system features a built-in USB Type-C port.

Around these base systems, the laser showcases **`status display`** designed to provide hardware feedback such as currently charging or powered on feedback.

## Project Parameters

| Total Cost | Time Invested | Status |
| :---: | :---: | :---: |
| **5€** | **2 days** | **✅ Completed** |

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

* **[LaserPointer](./Cost)** - laser case and components.

## **Tools Required:**

* FDM 3D printer (**minimum 180 x 180 mm bed**)
* Small screwdrivers set
* Wire strippers / cutters and tweezers
* Soldering iron and soldering wire
* Multimeter

## Build Instructions

1. **Preparation**: Review the Bill of Materials in the `Cost` folder and acquire all necessary parts.

2. **3D Printing**: Print the laser case from `Design/3DModels` folder.

3. **Assembly**: Follow the assembly sheets in `Design/Assembly` to assemble the laser and fit the electronics.

4. **Wiring**: Wire the components according to `Design/WiringDiagram`.

5. **Testing**: Verify the laser diode, button and battery display, then confirm the system work reliably.

## License

This project is distributed under the terms specified in the **`LICENSE.md`** file located in the root of the main repository.

## Contact

If you have any questions, feedback, or suggestions regarding this project, feel free to open an issue or reach out directly.