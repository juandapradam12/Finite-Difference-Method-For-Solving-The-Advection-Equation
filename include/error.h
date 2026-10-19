#ifndef ERROR_H
#define ERROR_H

#include "field.h"

/**
 * Exact Cauchy solution for the initial parabolic pulse:
 * u(x,t) = u_0(x - c t), with u_0(ξ) = 1/4 - ξ² on |ξ| ≤ 1/2.
 *
 * Valid as a reference on a finite domain while the support of the
 * translated pulse remains inside [x_min, x_max].
 */
double exact_pulse(double x, double t, double c);

/**
 * Discrete error norms against the exact pulse at time t:
 *
 *   E_2   = √(Δx Σ_i (u_i - u_exact(x_i,t))²)
 *   E_∞   = max_i |u_i - u_exact(x_i,t)|
 */
void error_norms(const Field1D *f, double t, double c,
                 double *l2, double *linf);

#endif /* ERROR_H */
