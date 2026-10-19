#!/usr/bin/env python3
"""Plot selected snapshots from the 1D advection solver."""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "output"
FIGURES = ROOT / "figures"

# Snapshots produced by the default demo configuration.
SNAPSHOTS = [
    (0, r"$t = 0$"),
    (350, r"$t = 350\,\Delta t$"),
    (600, r"$t = 600\,\Delta t$"),
    (700, r"$t = 700\,\Delta t$"),
]


def load_snapshot(step: int) -> pd.DataFrame:
    path = OUTPUT / f"u_t{step:04d}.csv"
    if not path.exists():
        raise FileNotFoundError(f"Missing snapshot: {path}")
    return pd.read_csv(path)


def main() -> None:
    FIGURES.mkdir(parents=True, exist_ok=True)

    fig, ax = plt.subplots(figsize=(8.5, 4.5), constrained_layout=True)

    for step, label in SNAPSHOTS:
        df = load_snapshot(step)
        ax.plot(df["x"], df["u"], label=label, linewidth=1.8)

    ax.set_xlabel(r"$x$")
    ax.set_ylabel(r"$u(x,t)$")
    ax.set_title("1D advection — first-order upwind FDM")
    ax.legend(frameon=False)
    ax.set_xlim(-2.0, 2.0)
    ax.axhline(0.0, color="0.7", linewidth=0.8)

    out = FIGURES / "advection_snapshots.png"
    fig.savefig(out, dpi=160)
    print(f"Saved {out}")


if __name__ == "__main__":
    main()
