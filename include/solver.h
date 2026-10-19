#ifndef SOLVER_H
#define SOLVER_H

#include "field.h"
#include "params.h"

/**
 * Advance the solution one time step with the first-order upwind scheme
 * for c > 0:
 *
 *   u_i^{n+1} = u_i^n - α (u_i^n - u_{i-1}^n),   α = c Δt / Δx
 *
 * Boundary nodes are left unchanged (Dirichlet).
 */
void solver_step_upwind(Field1D *f, double cfl);

/**
 * Run the full time integration, writing selected snapshots via the
 * provided callback (may be NULL).
 *
 * snapshot_steps: zero-terminated list of time indices to export,
 *                 or NULL to skip intermediate exports.
 */
typedef void (*SnapshotFn)(const Field1D *f, int step, void *ctx);

void solver_run(Field1D *f,
                const AdvectionParams *p,
                const int *snapshot_steps,
                SnapshotFn on_snapshot,
                void *ctx);

#endif /* SOLVER_H */
