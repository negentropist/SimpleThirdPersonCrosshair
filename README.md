# 🎯 Simple Third Person Crosshair

![Arma Reforger](https://img.shields.io/badge/Game-Arma%20Reforger-orange?style=for-the-badge)
![Enfusion Engine](https://img.shields.io/badge/Engine-Enfusion-blue?style=for-the-badge)
![Downloads](https://img.shields.io/badge/Downloads-70k%2B-green?style=for-the-badge)

A high-performance, **ray-casted crosshair** system for Arma Reforger. This mod ensures your aim is always accurate by calculating the real point of impact in a third-person perspective.

---

## 🚀 Key Features

* **Toggle 3PP Aiming**: Easily switch the crosshair on or off.
* **Opacity Control**: Includes a dedicated slider to adjust the crosshair visibility to your preference.
* **Smart Immersion**: 
    * Automatically hides when your weapon is lowered.
    * Only active in Third-Person Perspective (3PP) to maintain first-person immersion.
* **Dynamic Depth Effect**: The crosshair size scales based on the raycast hit distance, giving a natural sense of depth.
* **Line of Fire Feedback**: The crosshair **turns red** if your line of fire is obstructed by an object.

## 📥 Installation & Workshop

You can find the official mod on the Arma Reforger Workshop:

> [!IMPORTANT]
> **[View on Workshop: Simple 3PP Crosshair](https://reforger.armaplatform.com/workshop/64C914B4016D38B1)**

## 🛠 Technical Overview

The crosshair uses a ray-casting method to project the UI element from the camera's perspective onto the world geometry, aligned with the weapon's muzzle transform.
