#include "error.h"
#include "field.h"
#include "params.h"
#include "solver.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * Grid-convergence study at fixed CFL.
 *
 * Refine N while holding α = c Δt / Δx constant so that Δt ∝ Δx.
 * The first-order upwind scheme then yields global error O(Δx).
 */

static int ensure_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) ? 0 : -1;
    }
    return mkdir(path, 0755);
}

static AdvectionParams params_for_resolution(int n_points,
                                             double cfl_target,
                                             double t_final)
{
    AdvectionParams p = params_default();
    p.n_points = n_points;

    const double dx = params_dx(&p);
    p.dt = cfl_target * dx / p.c;
    p.n_steps = (int)lround(t_final / p.dt);
    if (p.n_steps < 1) {
        p.n_steps = 1;
    }
    return p;
}

int main(void)
{
    const char *out_dir = "output";
    const double cfl_target = 0.25;
    const double t_final = 0.5;
    /* Successive dyadic refinement of the spatial mesh. */
    const int resolutions[] = {125, 250, 500, 1000, 2000};
    const int n_res = (int)(sizeof(resolutions) / sizeof(resolutions[0]));

    if (ensure_dir(out_dir) != 0) {
        fprintf(stderr, "error: cannot create directory '%s'\n", out_dir);
        return EXIT_FAILURE;
    }

    char path[512];
    snprintf(path, sizeof(path), "%s/convergence.csv", out_dir);
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return EXIT_FAILURE;
    }

    fprintf(fp, "N,dx,dt,n_steps,t,L2,Linf,rate_L2,rate_Linf\n");

    printf("Grid convergence (fixed CFL = %.2f, T ≈ %.2f)\n",
           cfl_target, t_final);
    printf("%6s %12s %12s %12s %12s %10s %10s\n",
           "N", "dx", "E_2", "E_inf", "rate_E2", "rate_Einf", "t");

    double prev_dx = 0.0;
    double prev_l2 = 0.0;
    double prev_linf = 0.0;

    for (int r = 0; r < n_res; ++r) {
        AdvectionParams params =
            params_for_resolution(resolutions[r], cfl_target, t_final);

        if (params_validate(&params) != 0) {
            fclose(fp);
            return EXIT_FAILURE;
        }

        Field1D *field = field_create(&params);
        if (!field) {
            fprintf(stderr, "error: out of memory\n");
            fclose(fp);
            return EXIT_FAILURE;
        }

        field_set_initial_pulse(field);
        solver_run(field, &params, NULL, NULL, NULL);

        const double t = (double)params.n_steps * params.dt;
        const double dx = params_dx(&params);
        double l2 = 0.0;
        double linf = 0.0;
        error_norms(field, t, params.c, &l2, &linf);

        double rate_l2 = NAN;
        double rate_linf = NAN;
        if (r > 0 && prev_l2 > 0.0 && prev_linf > 0.0) {
            const double log_refine = log(prev_dx / dx);
            rate_l2 = log(prev_l2 / l2) / log_refine;
            rate_linf = log(prev_linf / linf) / log_refine;
        }

        if (r == 0) {
            fprintf(fp, "%d,%.10e,%.10e,%d,%.10e,%.10e,%.10e,,\n",
                    params.n_points, dx, params.dt, params.n_steps, t, l2,
                    linf);
            printf("%6d %12.4e %12.4e %12.4e %12s %10s %10.4f\n",
                   params.n_points, dx, l2, linf, "—", "—", t);
        } else {
            fprintf(fp, "%d,%.10e,%.10e,%d,%.10e,%.10e,%.10e,%.10e,%.10e\n",
                    params.n_points, dx, params.dt, params.n_steps, t, l2,
                    linf, rate_l2, rate_linf);
            printf("%6d %12.4e %12.4e %12.4e %12.4f %10.4f %10.4f\n",
                   params.n_points, dx, l2, linf, rate_l2, rate_linf, t);
        }

        prev_dx = dx;
        prev_l2 = l2;
        prev_linf = linf;
        field_free(field);
    }

    fclose(fp);
    printf("Wrote %s\n", path);
    return EXIT_SUCCESS;
}
