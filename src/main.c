#include "error.h"
#include "field.h"
#include "io.h"
#include "params.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

typedef struct {
    const char *dir;
    const AdvectionParams *params;
    FILE *errors;
} DemoCtx;

static int ensure_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) ? 0 : -1;
    }
    return mkdir(path, 0755);
}

static void on_snapshot(const Field1D *f, int step, void *ctx)
{
    DemoCtx *demo = ctx;
    double l2 = 0.0;
    double linf = 0.0;
    const double t = (double)step * demo->params->dt;

    io_snapshot_callback(f, step, (void *)demo->dir);
    error_norms(f, t, demo->params->c, &l2, &linf);

    fprintf(demo->errors, "%d,%.10e,%.10e,%.10e\n", step, t, l2, linf);
    printf("  step %4d  t = %.4f  E_2 = %.6e  E_∞ = %.6e\n",
           step, t, l2, linf);
}

int main(void)
{
    const char *out_dir = "output";
    AdvectionParams params = params_default();

    if (params_validate(&params) != 0) {
        return EXIT_FAILURE;
    }
    if (ensure_dir(out_dir) != 0) {
        fprintf(stderr, "error: cannot create directory '%s'\n", out_dir);
        return EXIT_FAILURE;
    }

    Field1D *field = field_create(&params);
    if (!field) {
        fprintf(stderr, "error: out of memory\n");
        return EXIT_FAILURE;
    }

    char err_path[512];
    snprintf(err_path, sizeof(err_path), "%s/errors.csv", out_dir);
    FILE *errors = fopen(err_path, "w");
    if (!errors) {
        perror(err_path);
        field_free(field);
        return EXIT_FAILURE;
    }
    fprintf(errors, "step,t,L2,Linf\n");

    field_set_initial_pulse(field);

    /* Snapshots matching the original demo times (plus final state). */
    const int snapshots[] = {0, 350, 600, 700, -1};
    DemoCtx demo = {
        .dir = out_dir,
        .params = &params,
        .errors = errors,
    };

    printf("1D advection (upwind FDM)\n");
    printf("  N = %d, dt = %.3e, steps = %d\n",
           params.n_points, params.dt, params.n_steps);
    printf("  dx = %.6e, CFL = %.6f\n",
           params_dx(&params), params_cfl(&params));
    printf("Error vs exact pulse u_0(x - c t):\n");

    solver_run(field, &params, snapshots, on_snapshot, &demo);

    fclose(errors);
    printf("Wrote snapshots and %s\n", err_path);

    field_free(field);
    return EXIT_SUCCESS;
}
