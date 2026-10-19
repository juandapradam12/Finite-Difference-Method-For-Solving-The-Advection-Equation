#ifndef FIELD_H
#define FIELD_H

#include "params.h"

/**
 * Discrete 1D field with a rolling pair of solution buffers.
 * Only the current and next time levels are kept in memory.
 */
typedef struct {
    int n;
    double dx;
    double *x;
    double *u;
    double *u_next;
} Field1D;

Field1D *field_create(const AdvectionParams *p);
void field_free(Field1D *f);

/** Parabolic pulse: u = 1/4 - x² for |x| ≤ 1/2, zero otherwise. */
void field_set_initial_pulse(Field1D *f);

/** Enforce homogeneous Dirichlet boundaries. */
void field_apply_dirichlet_zero(Field1D *f);

#endif /* FIELD_H */
