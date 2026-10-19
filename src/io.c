#include "io.h"

#include <stdio.h>

int io_write_snapshot_csv(const char *path, const Field1D *f)
{
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror(path);
        return -1;
    }

    fprintf(fp, "x,u\n");
    for (int i = 0; i < f->n; ++i) {
        fprintf(fp, "%.10e,%.10e\n", f->x[i], f->u[i]);
    }

    if (fclose(fp) != 0) {
        perror(path);
        return -1;
    }
    return 0;
}

void io_snapshot_callback(const Field1D *f, int step, void *ctx)
{
    const char *dir = ctx ? (const char *)ctx : "output";
    char path[512];
    snprintf(path, sizeof(path), "%s/u_t%04d.csv", dir, step);
    if (io_write_snapshot_csv(path, f) != 0) {
        fprintf(stderr, "warning: failed to write snapshot %s\n", path);
    }
}
