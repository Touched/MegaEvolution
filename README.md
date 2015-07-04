<p align="center"><img height="284" width="131" src="https://raw.githubusercontent.com/Touched/MegaEvolution/master/symbol.png"></p>

# Mega Evolutions
This is a hack written mostly in C, but some parts (hook code) are in ASM. It 
aims to allow changing of species mid-battle for Mega Evolutiion and similar
mechanics (Primal Reversion, etc.)

## Unix-like Installation
- Make sure you have DevKitARM installed and its binaries in your path.
- You'll also need `make` and Python3
- Place a clean FireRed ROM called 'BPRE0.gba' in the project root.
- Create a directory called 'build' in the project root.
- Run `make && ./insert.py`. This will create test.gba in the project root.
- For future builds you might want to run `make && ./insert.py && vba-sdl-h test.gba` to run the emulator after a successful build.

## Windows Installation
- Make sure you have devkitARM installed and its binaries in your path
- You'll also need `make` (Use the devkitPro installer for this and devkitARM) and Python3
- Create a directory called 'build' in the project root, to go along with the 'assets' and 'src' folders.
- Place a clean FireRed ROM called 'BPRE0.gba' in the project root.
- Run `make && ./insert.py` in the project root through Command Prompt (Shift + Right Click -> Open Command window here). This will create test.gba in the project root.
- For future builds you might want to run `make && ./insert.py && vba-sdl-h test.gba` to run the emulator after a successful build if the emulator is also in the project root.

## Credits
- MrDollSteak: Animation Script and Graphics
- Bela: Graphics
- daniilS: Miscellaneous code fixes
