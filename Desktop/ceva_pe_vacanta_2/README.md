# Calculator Avansat

An advanced calculator written in C, featuring both a command-line interface (CLI) and a graphical interface (GUI) built with SDL2.

## Features

- Basic arithmetic: `+`, `-`, `*`, `/`
- Math functions: `sin`, `cos`, `sqrt`, `log`
- Unit conversions:
  - `CtoF` — Celsius to Fahrenheit
  - `FtoC` — Fahrenheit to Celsius
  - `DEG` — degrees to radians
  - `RAD` — radians to degrees
- Base conversions: `BIN` (binary), `HEX` (hexadecimal)
- Memory operations: `M+`, `MR`, `MC`
- Calculation history (last 100 entries)
- Parentheses support for complex expressions

## Dependencies

- GCC
- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://github.com/libsdl-org/SDL2_ttf)

On macOS:
```bash
brew install sdl2 sdl2_ttf
```

On Debian/Ubuntu:
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

## Building

### GUI version

```bash
make
```

This produces the `calculator_avansat_2` executable.

### CLI version

```bash
gcc -Wall -o main_cli main_cli.c calc_core.c -lm
```

## Usage

### GUI

```bash
./calculator_avansat_2
```

- Click buttons or type on your keyboard to enter expressions.
- Press `Enter` or `=` to evaluate.
- Press `Backspace` to delete the last character.
- Press `Escape` to quit.
- The history panel on the right shows previous calculations.

### CLI

```bash
./main_cli
```

| Command         | Description                          |
|----------------|--------------------------------------|
| `<expression>` | Evaluate an arithmetic expression    |
| `history`      | Show calculation history             |
| `M+ <expr>`    | Evaluate and add result to memory    |
| `MR`           | Recall value from memory             |
| `MC`           | Clear memory                         |
| `quit`         | Exit the calculator                  |

#### Example

```
> 3 + 4 * 2
Rezultat = 11.000000
> sin(3.14)
Rezultat = 0.001593
> M+ 10 + 5
Adaugat 15.000000 in memorie
> MR
Memorie = 15.000000
> history
1) 3 + 4 * 2 = 11.000000
2) sin(3.14) = 0.001593
> quit
```

## Project Structure

| File                        | Description                              |
|-----------------------------|------------------------------------------|
| `calc_core.c` / `calc_core.h` | Shared expression evaluator and memory |
| `main_cli.c`                | CLI entry point                          |
| `calculator_avansat_gui.c`  | SDL2 GUI entry point                     |
| `Makefile`                  | Build rules for the GUI version          |
| `DejaVuSans.ttf`            | Font used by the GUI                     |
