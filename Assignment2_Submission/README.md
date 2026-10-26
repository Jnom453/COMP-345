Group that worked on it:
Jana Alnoman 40272288
Adila Burga 40319763
Chloe Fokom 40342914
Emine Nisa Mahmutogullari 40375863

## How to Run

Open the terminal in the 'Assignment 1' folder.

### Assignment 1
Compile:
g++ -std=c++17 -Wall -Wextra -pedantic \
  main.cpp Map.cpp Cell.cpp Tower.cpp \
  DirectDamageTower.cpp AreaDamageTower.cpp \
  SlowTower.cpp Critter.cpp CritterGroup.cpp \
  CritterGroupGenerator.cpp Player.cpp Tests.cpp \
  -o tower_defense.exe

Run:
./tower_defense.exe

The original program includes 14 tests.

### Assignment 2 — Observer Pattern GUIs
The graphical demonstrations use SFML 3. Compile each GUI separately.
The commands below assume SFML is installed through Homebrew on macOS.
The program uses one combined driver, main.cpp, to demonstrate the Observer pattern for the map, tower, critter, and critter group.
The graphical interfaces use SFML 3.

Compile:
clang++ -std=c++17 main.cpp \
  Map.cpp Cell.cpp Tower.cpp DirectDamageTower.cpp \
  AreaDamageTower.cpp SlowTower.cpp Critter.cpp \
  CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp \
  MapObserver.cpp TowerObserver.cpp CritterObserver.cpp \
  CritterGroupObserver.cpp \
  -isysroot "$(xcrun --show-sdk-path)" \
  -isystem "$(xcrun --show-sdk-path)/usr/include/c++/v1" \
  -I/opt/homebrew/opt/sfml/include \
  -L/opt/homebrew/opt/sfml/lib \
  -lsfml-graphics -lsfml-window -lsfml-system \
  -o assignment2

Run:
./assignment2

The driver opens the Map Observer, Tower Observer, Critter Observer, and Critter Group Observer demonstrations in sequence. Close each window to continue to the next demonstration.
The Assignment 1 program and Assignment 2 graphical demonstrations use separate driver programs and must be compiled separately.

## Requirements

- A C++17-compatible compiler.
- SFML 3 for the graphical demonstrations.
- On macOS, the compile commands above assume Homebrew SFML and the Apple Command Line Tools.
