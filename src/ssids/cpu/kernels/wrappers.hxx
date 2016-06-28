#pragma once

#include <stdexcept>
#include "common.hxx"

extern "C" {
   void dgemm_(char* transa, char* transb, int* m, int* n, int* k, double* alpha, const double* a, int* lda, const double* b, int* ldb, double *beta, double* c, int* ldc);
   void dpotrf_(char *uplo, int *n, double *a, int *lda, int *info);
   void dsytrf_(char *uplo, int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info);
   void dtrsm_(char *side, char *uplo, char *transa, char *diag, int *m, int *n, const double *alpha, const double *a, int *lda, double *b, int *ldb);
   void dsyrk_(char *uplo, char *trans, int *n, int *k, double *alpha, const double *a, int *lda, double *beta, double *c, int *ldc);
   void dtrsv_(char *uplo, char *trans, char *diag, int *n, const double *a, int *lda, double *x, int *incx);
}

namespace spral { namespace ssids { namespace cpu {

/* _GEMM */
template <typename T>
void host_gemm(enum spral::ssids::cpu::operation transa, enum spral::ssids::cpu::operation transb, int m, int n, int k, T alpha, const T* a, int lda, const T* b, int ldb, T beta, T* c, int ldc);
template <>
void host_gemm<double>(enum spral::ssids::cpu::operation transa, enum spral::ssids::cpu::operation transb, int m, int n, int k, double alpha, const double* a, int lda, const double* b, int ldb, double beta, double* c, int ldc) {
   char ftransa = (transa==spral::ssids::cpu::OP_N) ? 'N' : 'T';
   char ftransb = (transb==spral::ssids::cpu::OP_N) ? 'N' : 'T';
   dgemm_(&ftransa, &ftransb, &m, &n, &k, &alpha, a, &lda, b, &ldb, &beta, c, &ldc);
}

/* _POTRF */
template <typename T>
int lapack_potrf(enum spral::ssids::cpu::fillmode uplo, int n, T* a, int lda);
template<>
int lapack_potrf<double>(enum spral::ssids::cpu::fillmode uplo, int n, double* a, int lda) {
   char fuplo;
   switch(uplo) {
      case spral::ssids::cpu::FILL_MODE_LWR: fuplo = 'L'; break;
      case spral::ssids::cpu::FILL_MODE_UPR: fuplo = 'U'; break;
      default: throw std::runtime_error("Unknown fill mode");
   }
   int info;
   dpotrf_(&fuplo, &n, a, &lda, &info);
   return info;
}

/* _SYTRF - Bunch-Kaufman factorization */
template <typename T>
int lapack_sytrf(enum spral::ssids::cpu::fillmode uplo, int n, T* a, int lda, int* ipiv, T* work, int lwork);
template<>
int lapack_sytrf<double>(enum spral::ssids::cpu::fillmode uplo, int n, double* a, int lda, int *ipiv, double* work, int lwork) {
   char fuplo;
   switch(uplo) {
      case spral::ssids::cpu::FILL_MODE_LWR: fuplo = 'L'; break;
      case spral::ssids::cpu::FILL_MODE_UPR: fuplo = 'U'; break;
      default: throw std::runtime_error("Unknown fill mode");
   }
   int info;
   dsytrf_(&fuplo, &n, a, &lda, ipiv, work, &lwork, &info);
   return info;
}

/* _SYRK */
template <typename T>
void host_syrk(enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation trans, int n, int k, T alpha, const T* a, int lda, T beta, T* c, int ldc);
template <>
void host_syrk<double>(enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation trans, int n, int k, double alpha, const double* a, int lda, double beta, double* c, int ldc) {
   char fuplo = (uplo==spral::ssids::cpu::FILL_MODE_LWR) ? 'L' : 'U';
   char ftrans = (trans==spral::ssids::cpu::OP_N) ? 'N' : 'T';
   dsyrk_(&fuplo, &ftrans, &n, &k, &alpha, a, &lda, &beta, c, &ldc);
}

/* _TRSV */
template <typename T>
void host_trsv(enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation trans, enum spral::ssids::cpu::diagonal diag, int n, const T* a, int lda, T* x, int incx);
template <>
void host_trsv<double>(enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation trans, enum spral::ssids::cpu::diagonal diag, int n, const double* a, int lda, double* x, int incx) {
   char fuplo = (uplo==spral::ssids::cpu::FILL_MODE_LWR) ? 'L' : 'U';
   char ftrans = (trans==spral::ssids::cpu::OP_N) ? 'N' : 'T';
   char fdiag = (diag==spral::ssids::cpu::DIAG_UNIT) ? 'U' : 'N';
   dtrsv_(&fuplo, &ftrans, &fdiag, &n, a, &lda, x, &incx);
}

/* _TRSM */
template <typename T>
void host_trsm(enum spral::ssids::cpu::side side, enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation transa, enum spral::ssids::cpu::diagonal diag, int m, int n, T alpha, const T* a, int lda, T* b, int ldb);
template <>
void host_trsm<double>(enum spral::ssids::cpu::side side, enum spral::ssids::cpu::fillmode uplo, enum spral::ssids::cpu::operation transa, enum spral::ssids::cpu::diagonal diag, int m, int n, double alpha, const double* a, int lda, double* b, int ldb) {
   char fside = (side==spral::ssids::cpu::SIDE_LEFT) ? 'L' : 'R';
   char fuplo = (uplo==spral::ssids::cpu::FILL_MODE_LWR) ? 'L' : 'U';
   char ftransa = (transa==spral::ssids::cpu::OP_N) ? 'N' : 'T';
   char fdiag = (diag==spral::ssids::cpu::DIAG_UNIT) ? 'U' : 'N';
   dtrsm_(&fside, &fuplo, &ftransa, &fdiag, &m, &n, &alpha, a, &lda, b, &ldb);
}

}}} /* namespaces spral::ssids::cpu */
