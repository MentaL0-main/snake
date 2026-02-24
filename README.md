# Snake game

<p align="center">
  <img src="screenshots/screenshot.png" alt="Snake Gameplay" width="600px" style="border-radius: 10px; border: 2px solid #3a7bd5;">
</p>

## Features
- High performance: Minimal resource consumption thanks to SDL2.
- Smooth control: Responsive input without delay.
- Modular architecture: The code is easy to expand (add levels, skins or enemies).

## Requirements
To build and launch you will need:
- Compiler: GCC 7+, Clang 5+ or MSVC 2017+.
- Libraries: 
  - SDL2 (Core library)

## Assembly and launch
### Arch linux 
```shell
sudo pacman -S sdl2 clang git
git clone https://github.com/mental0-main/snake.git
cd snake
clang++ -o snake_game *.cpp -lSDL2
./snake_game
```

### Debian linux 
```shell
sudo apt install libsdl2-dev clang git
git clone https://github.com/mental0-main/snake.git
cd snake
clang++ -o snake_game *.cpp -lSDL2
./snake_game
```

### Windows
1. Download the SDL2 Development Libraries from [official site] (https://www.libsdl.org/).
2. Connect paths to include and lib in your IDE (Visual Studio, CLion).
3. Make sure that the SDL2.dll is next to the .exe file.

| Key | Action |
| :--- | :--- |
| W / Up | Move up |
| S / Down | Moving down |
| A / Left | Movement to the left |
| D / Right | Rightward |
| Esc | Exit the game |

## License
This project is licensed under the MIT license. 
