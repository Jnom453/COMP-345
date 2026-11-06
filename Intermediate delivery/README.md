# Tower Defense (COMP 345 team project)

Group:
- Jana Alnoman 40272288
- Adila Burga 40319763
- Chloe Fokom 40342914
- Emine Nisa Mahmutogullari 40375863

This is the intermediate delivery. It contains:
1. The game model (map, towers, critters, waves, player).
2. The Observer pattern with four SFML windows (map, tower, critter,
   critter group): the Assignment 2 demonstration, built from `main.cpp`.
3. A playable game with one SFML window, built from `game_main.cpp`.

The source code is commented for Doxygen; see "Documentation" below.
Both programs share the same model classes and are compiled separately.

## Requirements
- A C++17 compiler (g++ or clang++).
- SFML 3 (the code uses the SFML 3 API; SFML 2 will not compile).

Install SFML 3:
- macOS (Homebrew): `brew install sfml`
- Linux: install SFML 3 from your package manager if it has version 3
  (for example `sudo apt install libsfml-dev` on a recent distribution),
  otherwise build it from https://www.sfml-dev.org/download/
- Windows: download SFML 3 from https://www.sfml-dev.org/download/ matching
  your compiler (for example MinGW), and use the -I and -L paths of that folder.

## Compile the Observer demonstration (main.cpp)
Open a terminal in this folder.

### macOS (Homebrew, Apple silicon)
    clang++ -std=c++17 main.cpp Map.cpp Cell.cpp Tower.cpp \
      DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp \
      Critter.cpp CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp \
      MapObserver.cpp TowerObserver.cpp CritterObserver.cpp \
      CritterGroupObserver.cpp \
      -I/opt/homebrew/opt/sfml/include -L/opt/homebrew/opt/sfml/lib \
      -lsfml-graphics -lsfml-window -lsfml-system -o assignment2

If the compiler cannot find the standard library headers, add:
`-isysroot "$(xcrun --show-sdk-path)"`

### Linux (SFML installed system-wide)
    g++ -std=c++17 main.cpp Map.cpp Cell.cpp Tower.cpp \
      DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp \
      Critter.cpp CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp \
      MapObserver.cpp TowerObserver.cpp CritterObserver.cpp \
      CritterGroupObserver.cpp \
      -lsfml-graphics -lsfml-window -lsfml-system -o assignment2

If SFML is in a custom folder, add `-I<sfml>/include -L<sfml>/lib` and run with
`LD_LIBRARY_PATH=<sfml>/lib ./assignment2`.

### Windows (MinGW g++)
    g++ -std=c++17 main.cpp Map.cpp Cell.cpp Tower.cpp DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp Critter.cpp CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp MapObserver.cpp TowerObserver.cpp CritterObserver.cpp CritterGroupObserver.cpp -I<sfml>/include -L<sfml>/lib -lsfml-graphics -lsfml-window -lsfml-system -o assignment2.exe

Copy the SFML `.dll` files from `<sfml>/bin` next to `assignment2.exe`.

## Playable game (game_main.cpp)
Compile (macOS, Homebrew SFML):

    clang++ -std=c++17 game_main.cpp Game.cpp Map.cpp Cell.cpp Tower.cpp \
      DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp \
      Critter.cpp CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp \
      -I"$(brew --prefix sfml)/include" -L"$(brew --prefix sfml)/lib" \
      -lsfml-graphics -lsfml-window -lsfml-system -o game

On Linux use `g++` with the same file list and only the three `-lsfml-...`
flags; on Windows use the MinGW command above with the same file list.
Run with `./game` (Windows: `game.exe`).

How to play: you start with 350 coins and 10 lives. Click a green cell to buy
and place the chosen tower (keys 1, 2, 3 choose Direct Damage $100,
Area Damage $150, Slow $125). Click a tower to select it (its range is shown),
then press U to upgrade it (up to level 3) or S to sell it. Press SPACE to
send the next wave. Towers shoot critters in range, killing a critter gives
coins, and every critter that reaches the exit costs a life and some coins.
You lose at 0 lives and win by surviving 10 waves. R restarts, ESC quits.

## Run the Observer demonstration
    ./assignment2        (Windows: assignment2.exe)

The driver opens four windows one after the other. Close each window to move
on to the next one:
1. Map observer: a 12-column map with a straight path (blue = entry,
   red = exit, brown = path, green = scenery). A path cell is then added and
   the window shows the change.
2. Tower observer: a direct damage tower is upgraded once and its new level,
   power and range are shown.
3. Critter observer: a critter takes 30 damage and its hit points are updated.
4. Critter group observer: critters are added to and removed from a wave and
   the list is updated.

The text in the windows uses a system font (Arial on macOS/Windows, DejaVu or
Liberation Sans on Linux). If none is found the windows still open but show no text.

## Documentation (Doxygen)
The Doxygen configuration is `Doxyfile`; the generated HTML is in
`docs/html/index.html` (open `index.html` in a browser). The front page
explains the game rules and the Observer design. To regenerate:

    doxygen Doxyfile

(needs Doxygen, and Graphviz for the class diagrams).

## Not implemented yet
A map editor and choosing between maps (the game uses one built-in map), and
saving/loading maps to files. The playable game's rules are in `Game.cpp`
and are documented in `Game.h`.
