# Termon
A Creature-Catching game designed to be played in the Linux Terminal, made using C.

## Download & Installation
Either download and extract the compressed zip directly from the [GitHub](https://github.com/InSunlitGrace/Termon) page, or `gh repo clone InSunlitGrace/Termon` if using GitHub terminal client, or clone `https://github.com/InSunlitGrace/Termon.git`

Make sure `make` and `gcc` are installed, or suitably change the `Makefile` so that it works with the desired C compiler.

Open your terminal and navigate to inside the main directory run: `make` or `make all`. This generates the game binary in `./bin/`.
### Dependecies
If your version of linux or any other compatible operating systen does not have the `ncurses` library installed, kindly refer to that OS's documentation for installing the said library. Some common ways are:

* Ubuntu:
```
sudo apt update
sudo apt install libncurses-dev
```


## Running the Executable

The game can be run by execution `./bin/game` in the terminal at current directory as the main folder, or `./game` inside the `./bin` folder. 

The executable takes the following arguments as `./bin/game <argument>` or `./game <argument>`:

* ` ` : (No arguments) The game runs in RPG mode, without colour.
* `colour` : The game runs in RPG mode, with colour. This returns an error message if the terminal does not support colours.
* `pvp` : Play the game in Player v/s Player mode, without colour.
* `pvp colour` : Play the game in Player v/s Player mode, with colour. This returns an error message if the terminal does not support colours.

## Playing the Game

Playing the game is reltively simple, the commands are shown when the game is run. Pressing`Q` quits the game without saving, but all Menus should be closed and all Battles concluded before closing. Pressing `S` saves the game in an encyrpted format.

Saved games can also be continued.

### Movement

Use the ARROW KEYS: `UP/DOWN/LEFT/RIGHT` to move.

### Map Symbols

* `T` : These are Trees, they cannot be passed through.
* `#` : These are Bushes, moving in them has a chance of spawning Termon.
* `]` or `[` : These are Pathways, these are used to move from one Map to another.
* `O` : These look cool.

### Catching & Batyling

To attack a termon, press `A`.

To attempt to catch a wild Termon during a battle, press `C`. It is easier to catch a Termon with low Health/HP, fainted Termon cannot be caught.

To run (end a battle) press `R`.

The Player can only swap to a pokemon adjacent to the current pokemon in the team, using P for previous, N for next.

### Bag and Team

The Bag and Team Menus can be oppened and closed using `B` and `T` respectively. These can only be oppened if no other menu is open, and the Player is not in a battle.

#### Team

* `H` : Heal the current Termon.
* `LEFT/RIGHT` : Move through the Termons in the team.
* `U` : Move the current Termon 1 position up in the team.
* `D` : Move the current Termon 1 position down in the team.
* `R` : Release the current Termon into the wild. This cannot be undone.

## Player v\s Player

First, Players take turns in choosing Termon to make up their team. The size of the team is usually fixed, hence it can be changed in `h/constants.h` at Line 50, `#define PVP_TEAM_SIZE N`. The level of the battling termon can also be changed, at Line 51 of the aforementioned file: `#define PVP_MON_LEVEL 50
`

Player 1 is the first to "Move". A "Move" consists of either attacking or swapping the current Termon. The game is won when all the Termon of a Player have fainted, or a Player forfeits.