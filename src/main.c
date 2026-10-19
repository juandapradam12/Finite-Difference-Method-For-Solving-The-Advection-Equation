#include "field.h"
#include "io.h"
#include "params.h"
#include "solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static int ensure_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode) ? 0 : -1;
    }
    return mkdir(path, 0755);
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

    field_set_initial_pulse(field);

    /* Snapshots matching the original demo times (plus final state). */
    const int snapshots[] = {0, 350, 600, 700, -1};

    printf("1D advection (upwind FDM)\n");
    printf("  N = %d, dt = %.3e, steps = %d\n",
           params.n_points, params.dt, params.n_steps);
    printf("  dx = %.6e, CFL = %.6f\n",
           params_dx(&params), params_cfl(&params));

    solver_run(field, &params, snapshots, io_snapshot_callback, (void *)out_dir);

    printf("Wrote snapshots to %s/\n", out_dir);

    field_free(field);
    return EXIT_SUCCESS;
}
