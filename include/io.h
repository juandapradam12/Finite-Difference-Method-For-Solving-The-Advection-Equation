#ifndef IO_H
#define IO_H

#include "field.h"

/**
 * Write a two-column CSV (x, u) for the current field state.
 * Returns 0 on success, -1 on I/O error.
 */
int io_write_snapshot_csv(const char *path, const Field1D *f);

/**
 * Convenience callback compatible with SnapshotFn: ctx is a directory
 * path (const char *). Writes <dir>/u_tXXXX.csv.
 */
void io_snapshot_callback(const Field1D *f, int step, void *ctx);

#endif /* IO_H */
