<div align="center">
  <h1>CALC-TOOL-TI84-CE</h1>
  <p><strong>Full-Featured Symbolic Calculus &amp; Computer Algebra System for the TI-84 Plus CE</strong></p>
  <p>
    <img src="https://img.shields.io/badge/Platform-TI--84%20Plus%20CE%20(eZ80)-blue" alt="Platform">
    <img src="https://img.shields.io/badge/Language-C%20(CEdev)-brightgreen" alt="Language">
    <img src="https://img.shields.io/badge/License-MIT-lightgrey" alt="License">
  </p>
</div>

<hr>

## Overview

**CALC-TOOL-TI84-CE** is a fast, memory-safe Computer Algebra System (CAS) written in C for the TI-84 Plus CE (eZ80 processor). Powered by an Abstract Syntax Tree (AST), a pattern-matching identity engine, and the `imath` arbitrary-precision library, it brings full symbolic algebra, differential calculus, and **symbolic integration** directly to the TI-84 Plus CE—both through a native on-calculator GUI and TI-Basic commands.

---

## Core Capabilities

* **Symbolic Integration *(New Engine)*:**
  * Evaluates indefinite antiderivatives via recursive AST rules and pattern matching (`src/cas/integral.c`, `src/cas/integral.h`).
  * Supports **linearity** ($\int (f + g)\,dx$), **constant multiples** ($\int c \cdot f(x)\,dx$), the **general power rule** ($\int x^n\,dx$ for $n \neq -1$), **logarithmic/exponential rules** ($\int \frac{1}{x}\,dx$, $\int e^x\,dx$, $\int \ln(x)\,dx$), and **trigonometric / inverse-trig** antiderivatives.
* **Symbolic Differentiation:**
  * Computes exact first and higher-order derivatives with respect to any variable ($A$–$Z$, $\theta$) using the product, quotient, power, and chain rules (`src/cas/derivative.c`).
* **Algebraic Simplification & Expansion:**
  * Simplifies rational expressions, combines like terms, reduces fractions, applies trigonometric identities, and expands factored polynomials (`src/cas/simplify.c`, `src/cas/expand.c`, `src/cas/identities.c`).
* **Arbitrary-Precision Exact Evaluation:**
  * Evaluates exact integer powers, large factorials, and rational arithmetic without floating-point rounding errors (`src/cas/eval.c`, `src/imath/`).
* **Dual Interface (GUI + TI-Basic):**
  * Interactive menu-driven UI supporting `Y1`–`Y0`, `Str1`–`Str0`, and `Ans` (`src/calc/gui.c`).
  * Scriptable TI-Basic interface supporting `"SIMP"`, `"EVAL"`, `"EXP"`, `"DERIV"`, and `"INT"` (`src/calc/interface.c`).

---

## Screenshots

| Main GUI & Simplification | Symbolic Differentiation |
| :---: | :---: |
| ![Main screen](img/simplify.png) | ![Derivative example](img/deriv.png) |

| Trigonometric Identities | Polynomial Expansion |
| :---: | :---: |
| ![Trig example](img/trig.png) | ![Expand example](img/expand.png) |

| Arbitrary-Precision Arithmetic | TI-Basic Interface |
| :---: | :---: |
| ![Factorial example](img/eval_factorial.png) | ![Basic interface](img/interface.png) |

---

## Usage Examples

### 1. Calculator GUI
1. Enter an expression into **`Y1`** (e.g., `3X^2 + cos(X)`).
2. Run **`PCAS`** from the **`PRGM`** menu.
3. Choose an operation from the left menu (**Simplify**, **Evaluate**, **Expand**, **Derivative**, or **Integral**), pick your variable (`X`), and press the action button to write the result to **`Y2`**.

### 2. TI-Basic Scripting
Store a command string in `Ans` and run `Asm(prgmPCAS)` (or `prgmPCAS` on OS 5.3+):
* **Integrate:** `"INT,Y1,Y2,X"`
* **Differentiate:** `"DERIV,Y1,Y2,X"`
* **Simplify:** `"SIMP,Y1,Y2"`
* **Expand:** `"EXP,Y1,Y2"`

---

## Installation

1. Download **`PCAS.8xp`** from the [Releases](https://github.com/abarcagervasio/CALC-TOOL-TI84-CE/releases) page.
2. Download the [TI-84 Plus CE C Libraries](https://github.com/CE-Programming/libraries/releases) (`clibs.8xg`).
3. Using **TI Connect CE**, transfer both files to your calculator's **Archive** memory (archiving prevents `ERR:MEMORY`).
4. Run `prgmPCAS` from the `PRGM` menu. *(On OS 5.5+, use [arTIfiCE](https://yvantt.github.io/arTIfiCE/) to run ASM programs.)*

---

## Building from Source

1. Install the [CE C Toolchain (CEdev)](https://github.com/CE-Programming/toolchain).
2. Clone and compile:
   * `git clone https://github.com/abarcagervasio/CALC-TOOL-TI84-CE.git`
   * `cd CALC-TOOL-TI84-CE`
   * `make`
3. Output binary: `bin/PCAS.8xp`.

---

## Roadmap

* [x] Algebraic simplification, expansion, exact evaluation, and symbolic differentiation
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