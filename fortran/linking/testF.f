
      ! define the function (subroutine) "fortfunc"
      subroutine fortfunc(ii,ff)

      ! identify some variables: ii is integer, and ff is "float"
      integer ii
      real*4  ff

      ! write and format go togther.  This means "write onto device
      ! 6 (standard output in C) using the format statement identified
      ! as label 100, and the arguments are ii and ff.  The format
      ! statement tells us how to actually format the output
      write(6,100) ii, ff
 100  format('ii=',i2,' ff=',f6.3)

      ! "return" tells us to leave the function, and "end" tells us that
      ! it is the end of the program source
      return
      end
