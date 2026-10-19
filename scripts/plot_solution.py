#!/usr/bin/env python3
"""Plot solver snapshots, exact reference, and error norms."""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "output"
FIGURES = ROOT / "figures"

# Default demo parameters (must match params_default in C).
C = 1.0
DT = 1e-3
SNAPSHOTS = [
    (0, r"$t = 0$"),
    (350, r"$t = 350\,\Delta t$"),
    (600, r"$t = 600\,\Delta t$"),
    (700, r"$t = 700\,\Delta t$"),
]


def exact_pulse(x: np.ndarray, t: float, c: float = C) -> np.ndarray:
    xi = x - c * t
    u = np.zeros_like(x)
    mask = np.abs(xi) <= 0.5
    u[mask] = 0.25 - xi[mask] ** 2
    return u


def load_snapshot(step: int) -> pd.DataFrame:
    path = OUTPUT / f"u_t{step:04d}.csv"
    if not path.exists():
        raise FileNotFoundError(f"Missing snapshot: {path}")
    return pd.read_csv(path)


def plot_snapshots() -> Path:
    fig, ax = plt.subplots(figsize=(8.5, 4.5), constrained_layout=True)

    for step, label in SNAPSHOTS:
        df = load_snapshot(step)
        ax.plot(df["x"], df["u"], label=label, linewidth=1.8)

    # Exact reference at the final snapshot time.
    final_step, _ = SNAPSHOTS[-1]
    x = load_snapshot(final_step)["x"].to_numpy()
    t_final = final_step * DT
    ax.plot(
        x,
        exact_pulse(x, t_final),
        color="0.25",
        linestyle="--",
        linewidth=1.4,
        label=rf"exact at $t = {final_step}\,\Delta t$",
    )

    ax.set_xlabel(r"$x$")
    ax.set_ylabel(r"$u(x,t)$")
    ax.set_title("1D advection — first-order upwind FDM")
    ax.legend(frameon=False, fontsize=9)
    ax.set_xlim(-2.0, 2.0)
    ax.axhline(0.0, color="0.7", linewidth=0.8)

    out = FIGURES / "advection_snapshots.png"
    fig.savefig(out, dpi=160)
    print(f"Saved {out}")
    return out


def plot_errors() -> Path:
    path = OUTPUT / "errors.csv"
    if not path.exists():
        raise FileNotFoundError(f"Missing error report: {path}")
    err = pd.read_csv(path)

    fig, ax = plt.subplots(figsize=(8.0, 4.2), constrained_layout=True)
    ax.plot(err["t"], err["L2"], marker="o", label=r"$E_2$")
    ax.plot(err["t"], err["Linf"], marker="s", label=r"$E_\infty$")
    ax.set_xlabel(r"$t$")
    ax.set_ylabel("error")
    ax.set_title("Error vs exact solution $u_0(x - c t)$")
    ax.legend(frameon=False)
    ax.set_xlim(left=0.0)

    out = FIGURES / "error_norms.png"
    fig.savefig(out, dpi=160)
    print(f"Saved {out}")
    return out


def plot_convergence() -> Path:
    path = OUTPUT / "convergence.csv"
    if not path.exists():
        raise FileNotFoundError(f"Missing convergence report: {path}")
    conv = pd.read_csv(path)

    fig, ax = plt.subplots(figsize=(8.0, 4.5), constrained_layout=True)
    ax.loglog(conv["dx"], conv["L2"], "o-", label=r"$E_2$", linewidth=1.8)
    ax.loglog(conv["dx"], conv["Linf"], "s-", label=r"$E_\infty$", linewidth=1.8)

    # Reference slope-1 line anchored at the coarsest E_2.
    dx_ref = conv["dx"].to_numpy()
    e_ref = conv["L2"].to_numpy()
    ref = e_ref[0] * (dx_ref / dx_ref[0])
    ax.loglog(dx_ref, ref, "--", color="0.4", label=r"$O(\Delta x)$")

    ax.set_xlabel(r"$\Delta x$")
    ax.set_ylabel("error at fixed $T$")
    ax.set_title(r"Grid convergence at fixed CFL ($\alpha = 0.25$)")
    ax.legend(frameon=False)
    ax.invert_xaxis()

    out = FIGURES / "convergence.png"
    fig.savefig(out, dpi=160)
    print(f"Saved {out}")
    return out


def main() -> None:
    FIGURES.mkdir(parents=True, exist_ok=True)
    plot_snapshots()
    plot_errors()
    plot_convergence()


if __name__ == "__main__":
    main()
