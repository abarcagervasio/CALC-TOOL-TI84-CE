<div align="center">
  <h1>CALC-TOOL-TI84-CE</h1>
  <p><strong>Symbolic Calculus &amp; Computer Algebra System for the TI-84 Plus CE</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-TI--84%20Plus%20CE%20(eZ80)-blue" alt="Platform">
    <img src="https://img.shields.io/badge/Language-C%20(CEdev)-brightgreen" alt="Language">
    <img src="https://img.shields.io/badge/License-MIT-lightgrey" alt="License">
  </p>
</div>

<hr>

## Overview

**CALC-TOOL-TI84-CE** is a native C Computer Algebra System (CAS) engineered for the TI-84 Plus CE (eZ80 processor). Built on an Abstract Syntax Tree (AST) and pattern-matching identity engine with arbitrary-precision arithmetic, it extends traditional algebraic simplification and differentiation with a **Symbolic Integration Engine**, an interactive on-calculator GUI, and TI-Basic scripting hooks.

---

## Key Features

* **Symbolic Integration Engine (`src/cas/integral.c`, `src/cas/integral.h`):**
  * **Linearity & Constant Multiples:** Automatically splits sums ($\int (f + g)\,dx$) and factors out constants ($\int c \cdot f(x)\,dx$).
  * **General Power Rule:** Evaluates $\int x^n\,dx = \frac{x^{n+1}}{n+1}$ for arbitrary rational/symbolic constants ($n \neq -1$), plus logarithmic handling for $\int x^{-1}\,dx$ and $\int \frac{1}{x}\,dx$.
  * **Elementary Functions:** Built-in antiderivative table for exponential ($e^x$), logarithmic ($\ln(x)$), trigonometric ($\sin, \cos, \tan, \sec^2, \csc^2$), and inverse-trigonometric forms.
* **Algebra & Differential Calculus:** Symbolic simplification, polynomial expansion, substitution, exact constant evaluation, and symbolic differentiation with respect to any variable ($A$–$Z$, $\theta$).
* **Native GUI & TI-Basic Integration:**
  * Interactive **Integral** tab directly inside the calculator app (`src/calc/gui.c`).
  * Programmatic TI-Basic support via `"INT,Y1,Y2,X"` (`src/calc/interface.c`).

---

## Usage Examples

### 1. Using the Calculator GUI
1. Enter your function into **`Y1`** (for example: `3X^2 + cos(X)`).
2. Launch **`PCAS`** from the **`PRGM`** menu.
3. Select **`Integral`** from the left function panel, choose your variable of integration (`X`), and press **`Integrate`**.
4. View the simplified antiderivative in **`Y2`**:
   * **Input (`Y1`):** `3X^2 + cos(X)`
   * **Output (`Y2`):** `X^3 + sin(X)`

### 2. Calling from a TI-Basic Program
Store a command string in `Ans` and run `prgmPCAS` (`"INT,Y1,Y2,X"` followed by `Asm(prgmPCAS)`). It returns `1` in `Ans` on success and writes the symbolic antiderivative into `Y2`.

---

## Installation

1. Download **`PCAS.8xp`** from the [Releases](https://github.com/abarcagervasio/CALC-TOOL-TI84-CE/releases) page.
2. Download the [TI-84 Plus CE C Libraries](https://github.com/CE-Programming/libraries/releases) (`clibs.8xg`).
3. Using **TI Connect CE**, send both `PCAS.8xp` and `clibs.8xg` to your calculator's **Archive** memory (archiving prevents `ERR:MEMORY` during execution).
4. Run `prgmPCAS` from the `PRGM` menu. *(On OS 5.5+, use [arTIfiCE](https://yvantt.github.io/arTIfiCE/) to launch ASM programs.)*

---

## Building from Source

1. Install the [CE C Toolchain (CEdev)](https://github.com/CE-Programming/toolchain).
2. Clone this repository and compile:
   * `git clone https://github.com/abarcagervasio/CALC-TOOL-TI84-CE.git`
   * `cd CALC-TOOL-TI84-CE`
   * `make`
3. The compiled calculator binary will be generated at `bin/PCAS.8xp`.

---

## Roadmap

* [x] Symbolic antiderivative engine (linearity, constant multiples, power rule, elementary functions)
* [x] Interactive GUI `Integral` tab and TI-Basic `"INT"` command
* [ ] $u$-Substitution (reverse chain rule for composite functions)
* [ ] Integration by parts ($\int u\,dv = uv - \int v\,du$)
* [ ] Definite integrals ($\int_a^b f(x)\,dx$) using the Fundamental Theorem of Calculus

---

## Credits & License

* Originally forked from [PineappleCAS](https://github.com/nathanfarlow/PineappleCAS) by Nathan Farlow.
* Arbitrary-precision rational arithmetic powered by [imath](https://github.com/creachadair/imath) by Michael J. Fromberger.
* Licensed under the **MIT License** — see [`LICENSE`](LICENSE) for details.