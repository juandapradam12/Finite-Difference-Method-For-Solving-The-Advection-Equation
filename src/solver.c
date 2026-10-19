#include "solver.h"

void solver_step_upwind(Field1D *f, double cfl)
{
    /* Interior nodes: first-order upwind for c > 0. */
    for (int i = 1; i < f->n - 1; ++i) {
        f->u_next[i] = f->u[i] - cfl * (f->u[i] - f->u[i - 1]);
    }

    /* Homogeneous Dirichlet boundaries. */
    f->u_next[0] = 0.0;
    f->u_next[f->n - 1] = 0.0;

    double *tmp = f->u;
    f->u = f->u_next;
    f->u_next = tmp;
}

static int should_snapshot(const int *snapshot_steps, int step)
{
    if (!snapshot_steps) {
        return 0;
    }
    for (const int *s = snapshot_steps; *s >= 0; ++s) {
        if (*s == step) {
            return 1;
        }
    }
    return 0;
}

void solver_run(Field1D *f,
                const AdvectionParams *p,
                const int *snapshot_steps,
                SnapshotFn on_snapshot,
                void *ctx)
{
    const double cfl = params_cfl(p);

    if (on_snapshot && should_snapshot(snapshot_steps, 0)) {
        on_snapshot(f, 0, ctx);
    }

    for (int n = 0; n < p->n_steps; ++n) {
        solver_step_upwind(f, cfl);
        const int step = n + 1;
        if (on_snapshot && should_snapshot(snapshot_steps, step)) {
            on_snapshot(f, step, ctx);
        }
    }
}
