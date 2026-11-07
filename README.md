<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=FF8C1A&height=170&section=header&text=Pac-Dora%3A%20Star%20Catcher&fontColor=FFFFFF&fontSize=42&fontAlignY=38&desc=A%20Dora-themed%20Pac-Man%20game%20in%20C%2B%2B%20%2B%20SFML&descColor=FFF3D6&descSize=17&descAlignY=60" width="100%" alt="Pac-Dora: Star Catcher" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C++-FF8C1A?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/SFML-E91E8C?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Genre-Arcade-FFC107?style=for-the-badge&logoColor=black" />
  <img src="https://img.shields.io/badge/Course-PF--BSE--1B-26A69A?style=for-the-badge" />
</p>



---

## 🗺️ Overview

**Pac-Dora: Star Catcher** reimagines classic Pac-Man in a **Dora-inspired world**. Guide Dora through a foggy maze, collect every star, and stay away from the sneaky **Swipers**.

The game blends classic arcade gameplay with Dora's familiar music and voice dialogues, built as our **Programming Fundamentals** project (PF-BSE-1B).

---

## ⭐ Features

- Classic Pac-Man mechanics with a Dora twist
- **Dora background music** and **voice dialogues**
- **Custom maze textures and background** themed around Dora
- Smooth keyboard movement and collision handling
- Dynamic score tracking

---

## 🎮 Controls

| Key | Action |
|---|---|
| `Arrow Keys` / `W A S D` | Move Dora |
| `Esc` | Pause or exit the game |

---

## 🎒 Built With

- **C++**
- **SFML** (`sfml-graphics`, `sfml-audio`, `sfml-window`, `sfml-system`)

---

## 🧭 Setup Instructions

1. Install **SFML** from the [official download page](https://www.sfml-dev.org/download.php).
2. Clone this repository:
   ```bash
   git clone https://github.com/username/PF-BSE-1B.git
   ```
3. Open the project in your preferred C++ IDE, or build it with CMake.
4. Link the SFML libraries: `sfml-graphics`, `sfml-audio`, `sfml-window`, `sfml-system`.
5. Run the executable and dive into Dora's foggy maze adventure!

> Replace `username` in the clone URL with the repository owner's GitHub username.

---

## 👭 Group Members

| Name | Role |
|---|---|
| Haiba Naeem | Student of Software Engineering |
| Eman Fatima | Student of Software Engineering |
| Jannat Fatima | Student of Software Engineering |

---

## 🚧 Known Issue

The Swipers sometimes **glitch** during gameplay (jittery or stuck movement).

## 🔭 Future Improvements

- **Smoother Swiper movement.** Ideas we plan to try:
  - Move on a **tile grid** and only choose a new direction when the Swiper is centred on a tile, so it never turns mid-wall.
  - Use **delta time** (`sf::Clock`) for movement so speed does not depend on frame rate.
  - Stop Swipers from reversing direction instantly, unless they are cornered.
  - Use **BFS pathfinding** on the maze grid for smarter chasing.
- Smarter Swiper/enemy AI with a better movement algorithm
- Scoreboard and menu UI enhancements

---

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=FF8C1A&height=80&section=footer" width="100%" />
</p>

<p align="center"><i>⭐ Swiper, no swiping! ⭐</i></p>
