REM Build everything on Windows

cd lib
nmake /f Makefile.Windows %1 %2 %3 %4 %5 %6 %7 %8 %9
cd ..
nmake /f Makefile.Windows %1 %2 %3 %4 %5 %6 %7 %8 %9

