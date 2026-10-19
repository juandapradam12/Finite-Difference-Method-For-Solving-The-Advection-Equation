#include "params.h"

#include <stdio.h>

AdvectionParams params_default(void)
{
    AdvectionParams p = {
        .c = 1.0,
        .x_min = -2.0,
        .x_max = 2.0,
        .n_points = 1000,
        .dt = 1e-3,
        .n_steps = 700,
    };
    return p;
}

double params_dx(const AdvectionParams *p)
{
    return (p->x_max - p->x_min) / (double)(p->n_points - 1);
}

double params_cfl(const AdvectionParams *p)
{
    return p->c * p->dt / params_dx(p);
}

int params_validate(const AdvectionParams *p)
{
    if (p->n_points < 3) {
        fprintf(stderr, "error: n_points must be >= 3\n");
        return 1;
    }
    if (p->x_max <= p->x_min) {
        fprintf(stderr, "error: x_max must be greater than x_min\n");
        return 1;
    }
    if (p->dt <= 0.0 || p->n_steps < 1) {
        fprintf(stderr, "error: dt and n_steps must be positive\n");
        return 1;
    }
    if (p->c <= 0.0) {
        fprintf(stderr, "error: this solver assumes c > 0 (rightward upwind)\n");
        return 1;
    }

    const double cfl = params_cfl(p);
    if (cfl > 1.0 + 1e-12) {
        fprintf(stderr,
                "error: CFL = %.6f > 1; reduce dt or increase dx for stability\n",
                cfl);
        return 1;
    }
    return 0;
}
