#pragma once

namespace spral { namespace ssids { namespace cpu {

int ldlt_nopiv_factor(int m, int n, double* a, int lda, double* work);
void ldlt_nopiv_solve_fwd(int m, int n, double const* a, int lda, double *x);
void ldlt_nopiv_solve_diag(int m, int n, double const* a, int lda, double *x);
void ldlt_nopiv_solve_bwd(int m, int n, double const* a, int lda, double *x);

}}} /* namespaces spral::ssids::cpu */
