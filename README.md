# Legends of Arkania

A turn-based role-playing game built with C++ and Python, featuring exploration, combat, leveling, and multiple regions to conquer.

## 🎮 Game Overview

**Legends of Arkania** is a text-based RPG where players:
- Choose from three character classes (Warrior, Mage, Archer)
- Explore a vast fantasy world across multiple regions
- Engage in turn-based combat with various enemies
- Level up and improve stats
- Manage inventory and shop for items
- Save and load game progress
- Defeat the Dark Lord in the Dark Citadel to win!

## 🏗️ Project Structure

```
RPG Game/
├── main.cpp              # Entry point
├── Player.h/cpp          # Player class with stats, inventory, leveling
├── Enemy.h/cpp           # Enemy class for combat
├── Battle.h/cpp          # Turn-based battle system
├── Map.h/cpp             # Map loading and navigation
├── Shop.h/cpp            # Shop system for buying items
├── Game.h/cpp            # Main game loop and logic
├── maps/                 # Map files for each region
│   ├── Verdant Woods.txt
│   ├── Scorched Dunes.txt
│   ├── Frost Peaks.txt
│   └── Dark Citadel.txt
├── map_visualizer.py     # Python tool to visualize maps
├── stats_analyzer.py     # Python tool to analyze player stats
├── Makefile             # Build configuration
└── README.md            # This file
```

## 🚀 Building and Running

### Prerequisites
- C++ compiler with C++11 support (g++, clang++, etc.)
- Python 3 (for visualization tools)
- Optional: matplotlib (for stats plotting)

### Build the Game

```bash
# Build the game
make

# Or manually:
g++ -std=c++11 -Wall -Wextra -O2 -o legends_of_arkania *.cpp
```

### Run the Game

```bash
# Using make
make run

# Or directly
./legends_of_arkania
```

### Clean Build Files

```bash
make clean
```

## 🎯 How to Play

1. **Start the Game**: Run the executable
2. **Create Character**: Choose a name and class
3. **Explore**: Use WASD keys to move around the map
4. **Combat**: When encountering enemies, choose to Attack, Defend, or Use Items
5. **Level Up**: Gain experience from battles to increase your stats
6. **Visit Towns**: Rest to restore HP/MP or shop for items
7. **Explore Dungeons**: Challenge yourself for greater rewards
8. **Win**: Reach the Dark Citadel and defeat the Dark Lord!

### Controls

- **W/A/S/D** - Move (North/West/South/East)
- **I** - View Inventory
- **S** - View Stats
- **M** - View Map
- **H** - Help
- **Q** - Quit (with save option)

### Map Tiles

- `.` - Grass (safe terrain)
- `T` - Town (shop & rest)
- `F` - Forest (may encounter enemies)
- `D` - Desert
- `M` - Mountain
- `W` - Water
- `~` - Dungeon (multiple battles, great rewards)
- `C` - Castle (final boss in Dark Citadel)
- `#` - Wall (cannot pass)
- `@` - Your position

## 🐍 Python Tools

### Map Visualizer

Visualize game maps with color coding:

```bash
python3 map_visualizer.py maps/Verdant\ Woods.txt
python3 map_visualizer.py maps/Verdant\ Woods.txt 5 5  # With player position
```

### Stats Analyzer

Analyze player progress from save files:

```bash
# Display stats in terminal
python3 stats_analyzer.py savegame.txt

# Display stats with visualization (requires matplotlib)
python3 stats_analyzer.py savegame.txt --plot
```

Install matplotlib for plotting:
```bash
pip3 install matplotlib
# Or using make
make python-deps
```

## 🎲 Character Classes

### Warrior
- **High HP** (120 base)
- **High Strength** (15 base)
- **High Defense** (12 base)
- **Low Mana** (30 base)
- Best for: Tanking damage, melee combat

### Mage
- **Low HP** (80 base)
- **High Mana** (100 base)
- **Balanced Stats**
- Best for: Magic abilities, ranged combat

### Archer
- **Balanced HP** (100 base)
- **High Agility** (15 base)
- **Balanced Stats**
- Best for: Quick attacks, dodging

## 🌍 Regions

1. **Verdant Woods** - Starting area with forests and towns
2. **Scorched Dunes** - Desert region with dungeons
3. **Frost Peaks** - Mountainous region with icy challenges
4. **Dark Citadel** - Final area with the Dark Lord

## 💾 Save System

The game automatically saves to `savegame.txt` when you quit. You can load your progress when starting a new game session.

## 🛠️ Technical Details

### C++ Features Used
- Object-Oriented Programming (classes, inheritance concepts)
- File I/O for maps and save/load
- STL containers (vector, map, string)
- Random number generation
- Memory management

### Python Integration
- Map visualization with color coding
- Statistical analysis of player progress
- Data visualization with matplotlib

## 📝 Future Enhancements

Potential additions:
- More character classes
- Magic system with spells
- Equipment system (weapons/armor affecting stats)
- Quest system
- Multiple save slots
- Procedural map generation
- More enemy types and bosses

## 🐛 Troubleshooting

**Build errors?**
- Ensure you have a C++11 compatible compiler
- Check that all header files are present
- Try `make clean` then rebuild

**Map files not found?**
- Run `make maps` to create the maps directory
- Maps will be auto-generated if files don't exist

**Python tools not working?**
- Ensure Python 3 is installed
- Install matplotlib: `pip3 install matplotlib`
- Check file paths are correct

## 📄 License

This project is created for educational purposes as a 2nd-year CS project.

## 👨‍💻 Development

Built with:
- C++11 for core game logic
- Python 3 for visualization tools
- Make for build automation

---

**Enjoy your adventure in Arkania!** ⚔️🛡️✨

