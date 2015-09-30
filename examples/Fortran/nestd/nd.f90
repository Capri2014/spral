    PROGRAM example
      USE spral_nd
      IMPLICIT NONE

      ! Local variables
      INTEGER :: mtx, n, ne
      INTEGER :: row(14), ptr(9), perm(8)

      TYPE (nd_options) :: options
      TYPE (nd_inform) :: inform

      ! Set order n of the matrix and the number
      ! of non-zeros in its lower triangular part.
      n = 8 
      ne = 14

      ! Matrix data
      ptr(1:n+1) = (/ 1,5,8,10,11,13,15,15,15 /)
      row(1:ne) = (/ 2,4,5,6,4,6,7,5,6,7,6,8,7,8 /)

      ! Call nested dissection and switch to approximate minimum degree when
      ! sub matrix has order less than or equal to 4
      options%amd_switch1 = 4
      options%amd_call = 3
      mtx = 0
      CALL nd_order(mtx,n,ptr,row,perm,options,inform)

      ! Print out nested dissection ordering
      WRITE (6,'(a)') ' Permutation : '
      WRITE (6,'(8i8)') perm

    END PROGRAM example