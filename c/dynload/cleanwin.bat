REM Build everything on Windows

cd lib
nmake /f Makefile.Windows clean
cd ..
nmake /f Makefile.Windows clean

