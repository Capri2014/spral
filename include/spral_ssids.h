#ifndef SPRAL_SSIDS_H
#define SPRAL_SSIDS_H

#include <stdbool.h>

struct spral_ssids_options {
   int array_base; // Not in Fortran type
   int print_level;
   int unit_diagnostics;
   int unit_error;
   int unit_warning;
   int ordering;
   int nemin;
   int scaling;
   bool action;
   double small;
   double u;
   bool use_gpu_solve;
   int presolve;
   char unused[80]; // Allow for future expansion
};

struct spral_ssids_inform {
   int flag;
   int matrix_dup;
   int matrix_missing_diag;
   int matrix_outrange;
   int matrix_rank;
   int maxdepth;
   int maxfront;
   int num_delay;
   long num_factor;
   long num_flops;
   int num_neg;
   int num_sup;
   int num_two;
   int stat;
   int cuda_error;
   int cublas_error;
   char unused[80]; // Allow for future expansion
};

/* Initialize options to defaults */
void spral_ssids_default_options(struct spral_ssids_options *options);
/* Perform analysis phase for CSC data */
void spral_ssids_analyse(bool check, int n, int *order, const int *ptr,
      const int *row, const double *val, void **akeep,
      const struct spral_ssids_options *options,
      struct spral_ssids_inform *inform);
/* Perform analysis phase for coordinate data */
void spral_ssids_analyse(int n, int *order, int ne, const int *row,
      const int *col, const double *val, void **akeep,
      const struct spral_ssids_options *options,
      struct spral_ssids_inform *inform);
/* Perform numerical factorization */
void spral_ssids_factor(bool posdef, const int *ptr, const int *row,
      const double *val, double *scale, void *akeep, void **fkeep,
      const struct spral_ssids_options *options,
      struct spral_ssids_inform *inform);
/* Perform triangular solve(s) for single rhs */
void spral_ssids_solve1(int job, double *x1, void *akeep, void *fkeep,
      const struct spral_ssids_options *options,
      struct spral_ssids_inform *inform);
/* Perform triangular solve(s) for one or more rhs */
void spral_ssids_solve(int job, int nrhs, double *x, int ldx, void *akeep,
      void *fkeep, const struct spral_ssids_options *options,
      struct spral_ssids_inform *inform);
/* Free memory */
int spral_ssids_free_akeep(void **akeep);
int spral_ssids_free_fkeep(void **fkeep);
int spral_ssids_free(void **akeep, void **fkeep);

#endif // SPRAL_SSIDS_H
