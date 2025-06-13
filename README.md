# 🎮 Black Jack Game (C++ + Raylib)

A simple graphical Blackjack game built using **C++** and the **Raylib** graphics library. This game allows you to play against a dealer using visual playing cards, buttons, and keyboard input.

---

## 🧠 Features

- Dynamic card deck with full suit and rank combinations
- Player vs. Dealer logic
- Graphical rendering of cards using Raylib
- Interactive game buttons and keyboard controls (H = Hit, S = Stand)
- Visual feedback for bust, win, lose, or tie outcomes
- Player win tracking

---

## 🚀 How to Run

### ✅ Requirements:
- C++ Compiler (`g++`)
- Raylib installed  
  - [Install Raylib using vcpkg (recommended)](https://github.com/raysan5/raylib/wiki/Using-raylib-with-vcpkg)

### 🛠️ Compile and Run:
```bash
g++ main.cpp -o blackjack -lraylib -lopengl32 -lgdi32 -lwinmm
./blackjack
