!> \file
!> \copyright 2016 The Science and Technology Facilities Council (STFC)
!> \licence   BSD licence, see LICENCE file for details
!> \author    Jonathan Hogg
module spral_ssids_inform
   use spral_cuda, only : cudaGetErrorString
   use spral_scaling, only : auction_inform
   use spral_ssids_datatypes
   implicit none

   private
   public :: ssids_inform
   public :: ssids_print_flag

   !
   ! Data type for information returned by code
   !
   type ssids_inform
      integer :: flag ! Takes one of the enumerated flag values:
         ! SSIDS_SUCCESS
         ! SSIDS_ERROR_XXX
         ! SSIDS_WARNING_XXX
      integer :: matrix_dup = 0 ! Number of duplicated entries.
      integer :: matrix_missing_diag = 0 ! Number of missing diag. entries
      integer :: matrix_outrange = 0 ! Number of out-of-range entries.
      integer :: matrix_rank = 0 ! Rank of matrix (anal=structral, fact=actual)
      integer :: maxdepth ! Maximum depth of tree
      integer :: maxfront ! Maximum front size
      integer :: num_delay = 0 ! Number of delayed variables
      integer(long) :: num_factor = 0_long ! Number of entries in factors
      integer(long) :: num_flops = 0_long ! Number of floating point operations
      integer :: num_neg = 0 ! Number of negative pivots
      integer :: num_sup = 0 ! Number of supernodes
      integer :: num_two = 0 ! Number of 2x2 pivots used by factorization
      integer :: stat = 0 ! stat parameter
      type(auction_inform) :: auction
      integer :: cuda_error
      integer :: cublas_error

      ! Undocumented FIXME: should we document them?
      integer :: not_first_pass
      integer :: not_second_pass
   contains
      procedure, pass(this) :: flagToCharacter
   end type ssids_inform

contains

!
! Returns a string representation
! Member function inform%flagToCharacter
!
function flagToCharacter(this) result(msg)
   class(ssids_inform), intent(in) :: this
   character(len=200) :: msg ! return value

   select case(this%flag)
   !
   ! Success
   !
   case(SSIDS_SUCCESS)
      msg = 'Success'
   !
   ! Errors
   !
   case(SSIDS_ERROR_CALL_SEQUENCE)
      msg = 'Error in sequence of calls.'
   case(SSIDS_ERROR_A_N_OOR)
      msg = 'n or ne is out of range (or has changed)'
   case(SSIDS_ERROR_A_PTR)
      msg = 'Error in ptr'
   case(SSIDS_ERROR_A_ALL_OOR)
      msg = 'All entries in a column out-of-range (ssids_analyse) &
            &or all entries out-of-range (ssids_analyse_coord)'
   case(SSIDS_ERROR_SINGULAR)
      msg = 'Matrix found to be singular'
   case(SSIDS_ERROR_NOT_POS_DEF)
      msg = 'Matrix is not positive-definite'
   case(SSIDS_ERROR_PTR_ROW)
      msg = 'ptr and row should be present'
   case(SSIDS_ERROR_ORDER)
      msg = 'Either control%ordering out of range or error in user-supplied  &
            &elimination order'
   case(SSIDS_ERROR_X_SIZE)
      msg = 'Error in size of x or nrhs'
   case(SSIDS_ERROR_JOB_OOR)
      msg = 'job out of range'
   case(SSIDS_ERROR_NOT_LLT)
      msg = 'Not a LL^T factorization of a positive-definite matrix'
   case(SSIDS_ERROR_NOT_LDLT)
      msg = 'Not a LDL^T factorization of an indefinite matrix'
   case(SSIDS_ERROR_ALLOCATION)
      write (msg,'(a,i6)') 'Allocation error. stat parameter = ', this%stat
   case(SSIDS_ERROR_VAL)
      msg = 'Optional argument val not present when expected'
   case(SSIDS_ERROR_NO_SAVED_SCALING)
      msg = 'Requested use of scaling from matching-based &
            &ordering but matching-based ordering not used'
   case(SSIDS_ERROR_PRESOLVE_INCOMPAT)
      msg = 'Invalid combination of options%presolve, options%use_gpu_solve &
         &and requested operation - see documentation for legal combinations'
   case(SSIDS_ERROR_UNIMPLEMENTED)
      msg = 'Functionality not yet implemented'
   case(SSIDS_ERROR_CUDA_UNKNOWN)
      write(msg,'(2a)') ' Unhandled CUDA error: ', &
         cudaGetErrorString(this%cuda_error)
   case(SSIDS_ERROR_CUBLAS_UNKNOWN)
      msg = 'Unhandled CUBLAS error:'
      ! FIXME?

   !
   ! Warnings
   !
   case(SSIDS_WARNING_IDX_OOR)
      msg = 'out-of-range indices detected'
   case(SSIDS_WARNING_DUP_IDX)
      msg = 'duplicate entries detected'
   case(SSIDS_WARNING_DUP_AND_OOR)
      msg = 'out-of-range indices detected and duplicate entries detected'
   case(SSIDS_WARNING_MISSING_DIAGONAL)
      msg = 'one or more diagonal entries is missing'
   case(SSIDS_WARNING_MISS_DIAG_OORDUP)
      msg = 'one or more diagonal entries is missing and out-of-range and/or &
            &duplicate entries detected'
   case(SSIDS_WARNING_ANAL_SINGULAR)
      msg = 'Matrix found to be structually singular'
   case(SSIDS_WARNING_FACT_SINGULAR)
      msg = 'Matrix found to be singular'
   case(SSIDS_WARNING_MATCH_ORD_NO_SCALE)
      msg = 'Matching-based ordering used but associated scaling ignored'
   case default
      msg = 'SSIDS Internal Error'
   end select

end function flagToCharacter

!
! routine to print errors and warnings
!
subroutine ssids_print_flag(inform,nout,context)
   type(ssids_inform), intent(in) :: inform
   integer, intent(in) :: nout
   character (len=*), optional, intent(in) :: context

   character(len=200) :: msg

   if (nout < 0) return
   if (inform%flag < 0) then
      write (nout,'(/3a,i3)') ' Error return from ',trim(context),&
         '. Error flag = ', inform%flag
   else
      write (nout,'(/3a,i3)') ' Warning from ',trim(context),&
         '. Warning flag = ', inform%flag
   end if
   msg = inform%flagToCharacter()
   write(nout, '(a)') msg

end subroutine ssids_print_flag


end module spral_ssids_inform
