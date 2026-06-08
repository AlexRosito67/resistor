# resistor

A bidirectional resistor color code calculator for the terminal. Part of **Rosito Bench** — a collection of pure C++17 CLI engineering utilities.

---

## Why resistor?

Every electronics workbench has a multimeter and a pile of unmarked resistors. Reading color bands by eye is slow and error-prone. Opening a browser to use an online calculator is slower.

`resistor` lives in your terminal. It is a single binary with no dependencies, no runtime, and no internet connection required. Build it once and it is always there.

---

## Features

- Converts a resistor value and tolerance to color bands
- Converts color bands to a resistor value and tolerance
- Supports 4-band and 5-band resistors
- Supports engineering notation: `2k2`, `4R7`, `1M`, `470k`
- Displays color bands with ANSI terminal colors
- Validates input against standard resistor tolerance values
- Shows valid measurement range based on tolerance
- No external dependencies — pure C++17

---

## Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## Usage

```bash
# Convert a value to color bands
resistor -v 1000 -p 5
resistor -v 4.7k -p 1
resistor -v 2k2 -p 5%
resistor -v 4R7 -p 5

# Convert color bands to a value
resistor -c red,brown,black,gold
resistor -c yellow,violet,black,brown,brown
```

---

## Arguments

| Flag | Description |
|------|-------------|
| `-v` | Resistor value in ohms. Supports engineering notation: `1000`, `4.7k`, `2k2`, `4R7`, `1M` |
| `-p` | Tolerance in percentage: `5`, `1`, `0.5`, `5%` |
| `-c` | Color bands as comma-separated names: `red,brown,black,gold` |
| `-h` | Show help |

**Note:** `-v` and `-p` must be used together. `-c` cannot be combined with `-v` or `-p`.

---

## Supported Tolerances

| Tolerance | Color |
|-----------|-------|
| ±0.05%    | Grey |
| ±0.1%     | Violet |
| ±0.25%    | Blue |
| ±0.5%     | Green |
| ±1%       | Brown |
| ±2%       | Red |
| ±5%       | Gold |
| ±10%      | Silver |

---

## Examples

```bash
$ resistor -v 220 -p 5

Your resistor's color code should look like this:  rd rd bn gl

$ resistor -c yellow,violet,black,brown,brown

The resistor value is:  47.0KΩ  +/- 1.0%
Valid range: 46.5KΩ - 47.5KΩ
If the measured value is outside this range, return the resistors lot.
You've been scammed!
```

---

## Part of Rosito Bench

`resistor` is the first tool in **Rosito Bench** — a growing collection of small, dependency-free C++17 CLI utilities for engineers and makers.

---

## License

Please read LICENSE.md.

---

## Support

If this tool saved you time, consider buying me a coffee ☕

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/AlexRosito67)

---

## 🧠 Looking for Embedded AI?

If you are building hardware and want to deploy Machine Learning models without the bloat of TensorFlow or cloud dependencies, check out my other project:
👉 [Hasaki CLI](https://github.com/AlexRosito67/hasaki) — Ultra-lightweight TinyML deployment in pure C.

---

*Created by Alex Rosito — Valley Glen, Los Angeles, California*
