# Tower Defense (COMP 345 team project) - Assignment 3

Group:
- Jana Alnoman 40272288
- Adila Burga 40319763
- Chloe Fokom 40342914
- Emine Nisa Mahmutogullari 40375863

Assignment 3 adds three design patterns to the game and uses all of them in
the playable game:
1. **Strategy**: how a tower chooses its target (`TargetingStrategy.h`).
2. **Factory**: the critters of each wave depend on the number of waves
   already destroyed (`CritterFactory.h`, `WaveFactory.h`).
3. **Decorator**: towers are changed by level ups and by the effects
   splash, burning and freezing (`TowerDecorator.h` and its subclasses).

The Observer pattern of Assignment 2 is still in the project (`main.cpp`).

## Contents
| Program | Main file | What it is |
|---|---|---|
| `game` | `game_main.cpp` | The playable game (SFML window). Demonstrates the three patterns. |
| `a3_tests` | `Tests_A3.cpp` | Console test cases for the three patterns and the game. |
| `assignment2` | `main.cpp` | The Observer demonstration of Assignment 2. |

## Requirements
- A C++17 compiler (g++ or clang++).
- SFML 3 (the code uses the SFML 3 API; SFML 2 will not compile). Only `game`
  and `assignment2` need SFML. The tests do not.

Install SFML 3:
- macOS (Homebrew): `brew install sfml`
- Linux: a package manager that has SFML 3 (for example `sudo apt install libsfml-dev`
  on a recent distribution), otherwise build it from https://www.sfml-dev.org/download/
- Windows: download SFML 3 from https://www.sfml-dev.org/download/ for your
  compiler (for example MinGW) and use the -I and -L paths of that folder.

## Compile and run
Open a terminal in this folder. The model files used by every program are:

    MODEL = Map.cpp Cell.cpp Tower.cpp DirectDamageTower.cpp AreaDamageTower.cpp
            SlowTower.cpp Critter.cpp CritterGroup.cpp Player.cpp
            TargetingStrategy.cpp TowerDecorator.cpp LevelUpDecorator.cpp
            EffectDecorator.cpp SplashDecorator.cpp BurningDecorator.cpp
            FreezingDecorator.cpp CritterFactory.cpp WaveFactory.cpp Game.cpp

### The test cases (no SFML needed; any platform)
    g++ -std=c++17 Tests_A3.cpp Map.cpp Cell.cpp Tower.cpp DirectDamageTower.cpp \
      AreaDamageTower.cpp SlowTower.cpp Critter.cpp CritterGroup.cpp Player.cpp \
      TargetingStrategy.cpp TowerDecorator.cpp LevelUpDecorator.cpp \
      EffectDecorator.cpp SplashDecorator.cpp BurningDecorator.cpp \
      FreezingDecorator.cpp CritterFactory.cpp WaveFactory.cpp Game.cpp \
      -o a3_tests
    ./a3_tests

(On macOS use `clang++`.) Every check prints PASS or FAIL; the program ends
with the number of checks that passed and failed.

### The playable game (macOS, Homebrew SFML)
    clang++ -std=c++17 game_main.cpp Map.cpp Cell.cpp Tower.cpp \
      DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp \
      Critter.cpp CritterGroup.cpp Player.cpp \
      TargetingStrategy.cpp TowerDecorator.cpp LevelUpDecorator.cpp \
      EffectDecorator.cpp SplashDecorator.cpp BurningDecorator.cpp \
      FreezingDecorator.cpp CritterFactory.cpp WaveFactory.cpp Game.cpp \
      -I"$(brew --prefix sfml)/include" -L"$(brew --prefix sfml)/lib" \
      -lsfml-graphics -lsfml-window -lsfml-system -o game
    ./game

Linux: use `g++` with the same file list and only the three `-lsfml-...`
flags (add `-I<sfml>/include -L<sfml>/lib` and run with
`LD_LIBRARY_PATH=<sfml>/lib ./game` if SFML is in a custom folder).
Windows (MinGW): same file list with `-I<sfml>/include -L<sfml>/lib`, output
`game.exe`, and copy the SFML `.dll` files from `<sfml>/bin` next to it.
If macOS cannot find the standard headers, add `-isysroot "$(xcrun --show-sdk-path)"`.

### The Assignment 2 Observer demonstration
    clang++ -std=c++17 main.cpp Map.cpp Cell.cpp Tower.cpp \
      DirectDamageTower.cpp AreaDamageTower.cpp SlowTower.cpp \
      Critter.cpp CritterGroup.cpp CritterGroupGenerator.cpp Player.cpp \
      MapObserver.cpp TowerObserver.cpp CritterObserver.cpp \
      CritterGroupObserver.cpp TargetingStrategy.cpp \
      -I"$(brew --prefix sfml)/include" -L"$(brew --prefix sfml)/lib" \
      -lsfml-graphics -lsfml-window -lsfml-system -o assignment2
    ./assignment2

It opens four windows one after the other (map, tower, critter, critter
group); close each one to see the next.

## Game rules
- The map is a grid. Critters enter at the blue cell, follow the brown path
  and try to reach the red exit. Towers can only be built on green scenery.
- You start with 350 coins and 10 lives. You win by destroying 10 waves and
  lose when the lives reach 0.
- Towers (price / refund / range / power):
  - Direct Damage 100 / 50 / 3 / 20: hits one critter.
  - Area Damage 150 / 75 / 2.5 / 12: hits every critter in range.
  - Slow 125 / 60 / 4 / 8: hits one critter and halves its speed.
- Level up: costs the buying cost times the current level, up to level 3.
  Each level adds 5 power and 1 range, and raises the sell refund by half of
  the price paid.
- Effects (one of each per tower, bought separately):
  - Splash $80: critters next to the target take half the damage
    (not for Area Damage towers, which already hit everything).
  - Burning $90: the target burns for 3 seconds, taking damage every second.
  - Freezing $70: the target moves at half speed for 2 seconds.
- Targeting strategy (press T to change; free): nearest to the tower, nearest
  to the exit (the default), strongest (most hit points) or weakest.
- Waves: wave n has 3 + n critters. Critters get stronger with n and the kinds
  depend on the wave: Normal (red) always; Fast (yellow) from wave 3, every
  third critter; Tank (blue) from wave 5, every fourth critter; one Boss
  (purple) at the end of waves 8, 9 and 10.
- A killed critter gives coins (its reward). A critter that reaches the exit
  costs 1 life and as many coins as its strength.

### Controls of the game
| Key / mouse | Action |
|---|---|
| 1, 2, 3 | Choose the tower type to build (direct, area, slow) |
| Left click on green cell | Buy and place the chosen tower |
| Left click on a tower | Select it (its range is shown) |
| U | Level up the selected tower (Decorator) |
| P / B / F | Add Splash / Burning / Freezing to the selected tower (Decorator) |
| T | Change the targeting strategy of the selected tower (Strategy) |
| S | Sell the selected tower |
| SPACE | Start the next wave (Factory) |
| R / ESC | Restart / quit |

The text under the map shows the selected tower's level, power, range,
strategy, effects and the price of each change. A burning critter has an
orange ring and a frozen one a light blue ring.

## Design (short)
- **Strategy.** `Tower` holds a `std::shared_ptr<TargetingStrategy>`. The four
  strategies (`NearestToTowerStrategy`, `NearestToExitStrategy`,
  `StrongestStrategy`, `WeakestStrategy`) only decide which of the critters in
  range to hit. `Tower::setStrategy()` changes it while the game runs.
- **Factory.** `CritterFactory` (abstract) has four subclasses, one per kind of
  critter, each with its own formulas for hit points, strength, speed and
  reward. `WaveFactory::createWave(wavesDestroyed)` chooses which factory makes
  each critter of the wave, so the wave changes with the number of waves destroyed.
- **Decorator.** `TowerDecorator` is a `Tower` that holds another `Tower` and
  forwards every call to it. `LevelUpDecorator` changes level, power, range and
  refund; `SplashDecorator`, `BurningDecorator` and `FreezingDecorator` (through
  `EffectDecorator`) add an effect to attacks. Decorators can be stacked in any
  order, and the game keeps the outermost one in a `std::unique_ptr<Tower>`.

The code is commented for Doxygen, and the front page of the generated
documentation (`docs/html/index.html`) explains the rules and the design.

## Documentation (Doxygen)
The Doxygen configuration is `Doxyfile`; the generated HTML is in
`docs/html/index.html`. To regenerate it:

    doxygen Doxyfile

(needs Doxygen, and Graphviz for the class diagrams).
