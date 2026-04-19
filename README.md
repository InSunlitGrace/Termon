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

Saved games can also be continued, inside the game.
