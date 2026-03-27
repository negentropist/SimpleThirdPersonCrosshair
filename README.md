# 🎯 Simple Third Person Crosshair

![Arma Reforger](https://img.shields.io/badge/Game-Arma%20Reforger-orange?style=for-the-badge)
![Enfusion Engine](https://img.shields.io/badge/Engine-Enfusion-blue?style=for-the-badge)
![Downloads](https://img.shields.io/badge/Downloads-70k%2B-green?style=for-the-badge)

A high-performance, **ray-casted crosshair** system for Arma Reforger. This mod ensures your aim is always accurate by calculating the real point of impact in a third-person perspective.

---

## 🚀 Key Features

* **Real-time Ray-casting**: Dynamically adjusts the crosshair position based on where your weapon is actually pointing.
* **Performance Oriented**: Written in Enforce Script (C# equivalent) with optimization in mind.
* **Seamless Integration**: Designed specifically for the Enfusion Engine.

## 📥 Installation & Workshop

You can find the official mod on the Arma Reforger Workshop:

> [!IMPORTANT]
> **[View on Workshop: Simple 3PP Crosshair](https://reforger.armaplatform.com/workshop/64C914B4016D38B1)**

## 🛠 Technical Overview

The crosshair uses a ray-casting method to project the UI element from the camera's perspective onto the world geometry, aligned with the weapon's muzzle transform.
