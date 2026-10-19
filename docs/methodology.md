# Methodology

## Governing equation

The one-dimensional linear advection equation transports a scalar field
$u(x,t)$ at constant speed $c$:

$$
\frac{\partial u}{\partial t} + c \frac{\partial u}{\partial x} = 0,
\qquad x \in [x_{\min}, x_{\max}],\quad t > 0.
$$

For $c > 0$ information travels to the right. The exact solution of the
Cauchy problem is a pure translation of the initial data:

$$
u(x,t) = u_0(x - c t).
$$

## Initial and boundary data

The demo uses a compact parabolic pulse

$$
u_0(x) =
\begin{cases}
\dfrac{1}{4} - x^{2}, & |x| \le \dfrac{1}{2}, \\[0.6em]
0, & \text{otherwise},
\end{cases}
$$

on the interval $[-2, 2]$, with homogeneous Dirichlet conditions
$u(x_{\min}, t) = u(x_{\max}, t) = 0$.

## Spatial–temporal discretisation

Nodes $x_i = x_{\min} + i\,\Delta x$, $i = 0,\ldots,N-1$, with

$$
\Delta x = \frac{x_{\max} - x_{\min}}{N - 1},
$$

and time levels $t^{n} = n\,\Delta t$. Let $u_i^{n} \approx u(x_i, t^{n})$.

## First-order upwind scheme

A backward difference in space (consistent with $c > 0$) and a forward
Euler step in time yield

$$
\frac{u_i^{n+1} - u_i^{n}}{\Delta t}
+ c\,\frac{u_i^{n} - u_{i-1}^{n}}{\Delta x} = 0,
$$

or equivalently

$$
u_i^{n+1}
= u_i^{n}
- \alpha \bigl(u_i^{n} - u_{i-1}^{n}\bigr),
\qquad
\alpha = \frac{c\,\Delta t}{\Delta x}.
$$

The dimensionless factor $\alpha$ is the Courant–Friedrichs–Lewy (CFL)
number. For this explicit scheme, stability requires

$$
0 < \alpha \le 1.
$$

The scheme is first-order accurate in both space and time and introduces
numerical diffusion that smooths sharp features as $t$ grows — a known
trade-off of classical upwinding.

## Implementation notes

The solver keeps only two solution buffers (current / next), advances
with the update above, and exports CSV snapshots at selected time
indices for post-processing. Default demo parameters are chosen so that
$\alpha < 1$ and the pulse remains inside the domain over the simulated
horizon.
