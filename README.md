# Finite-Difference Advection Solver

A compact C implementation of the **first-order upwind finite-difference method**
for the one-dimensional linear advection equation — a standard prototype in
computational fluid dynamics and hyperbolic PDE numerics.

This repository is a portfolio case study: clean module boundaries, a
CFL-aware parameter layer, reproducible build/plot targets, and a short
mathematical write-up of the scheme.

---

## Problem

Transport of a scalar field $u(x,t)$ at constant speed $c$:

$$
\frac{\partial u}{\partial t} + c \frac{\partial u}{\partial x} = 0.
$$

Exact solution: $u(x,t) = u_0(x - c t)$. Numerically, we discretise the PDE
with an explicit upwind stencil and track a compact parabolic pulse as it
advects across a finite interval.

Full derivation, stability condition, and initial/boundary data:
[docs/methodology.md](docs/methodology.md).

---

## Method (at a glance)

$$
u_i^{n+1}
= u_i^{n}
- \alpha \bigl(u_i^{n} - u_{i-1}^{n}\bigr),
\qquad
\alpha = \frac{c\,\Delta t}{\Delta x},\quad
0 < \alpha \le 1.
$$

| Choice | Detail |
| --- | --- |
| Scheme | First-order upwind (forward Euler + backward difference) |
| Domain | $x \in [-2, 2]$, $N = 1000$ nodes |
| Time | $\Delta t = 10^{-3}$, $700$ steps |
| IC | $u_0(x) = 1/4 - x^{2}$ for $|x| \le 1/2$, else $0$ |
| BC | Homogeneous Dirichlet |

---

## Architecture

```
include/          Public headers (params, field, solver, io)
src/              Implementation + thin driver (main.c)
scripts/          Snapshot plotting
docs/             Mathematical methodology
output/           CSV snapshots (generated)
figures/          Plots (generated)
```

| Module | Responsibility |
| --- | --- |
| `params` | Physical/numerical configuration, $\Delta x$, CFL validation |
| `field` | Grid, rolling solution buffers, initial pulse |
| `solver` | Single-step upwind update and time loop |
| `io` | CSV snapshot export |
| `main` | Glue: validate → initialise → integrate → write |

Only two time levels are stored in memory; selected frames are exported for
analysis rather than retaining the full space–time array.

---

## Build & run

```bash
make run          # compile and write output/u_tXXXX.csv
pip install -r requirements.txt
make plot         # run solver + save figures/advection_snapshots.png
```

Requires a C11 compiler (`gcc`/`clang`) and Python 3 with
`matplotlib` / `pandas`.

---

## Results

The pulse translates to the right while numerical diffusion from the
first-order upwind scheme gradually flattens the profile — the expected
behaviour for this classical discretisation.

![Advection snapshots](figures/advection_snapshots.png)

---

## Stack

- **C11** — numerical core
- **Make** — build orchestration
- **Python** — post-processing only

---

## License

Use freely for portfolio and educational reference.
