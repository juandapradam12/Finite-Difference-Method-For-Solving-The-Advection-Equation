#ifndef PARAMS_H
#define PARAMS_H

/**
 * Physical and numerical parameters for the 1D advection problem.
 *
 * Domain: [x_min, x_max], wave speed c > 0, first-order upwind scheme.
 * Stability requires the CFL number |c * dt / dx| <= 1.
 */
typedef struct {
    double c;       /* Advection speed [m/s] */
    double x_min;   /* Left domain boundary [m] */
    double x_max;   /* Right domain boundary [m] */
    int n_points;   /* Number of spatial nodes */
    double dt;      /* Time step [s] */
    int n_steps;    /* Number of time steps */
} AdvectionParams;

/** Default portfolio demo configuration. */
AdvectionParams params_default(void);

/** Spatial spacing implied by the parameter set. */
double params_dx(const AdvectionParams *p);

/** Courant–Friedrichs–Lewy number α = c Δt / Δx. */
double params_cfl(const AdvectionParams *p);

/**
 * Returns 0 if parameters are valid and CFL-stable, non-zero otherwise.
 * Writes a short message to stderr on failure.
 */
int params_validate(const AdvectionParams *p);

#endif /* PARAMS_H */
