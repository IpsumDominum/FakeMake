 @echo off
@setlocal

set start=%time%

:: Running all the tests...
echo ============TEST1=================
echo Running 'fakemake -p -f tests/compileSelf' (Serial Compile Self)
cmd /c %fakemake -p -f tests/compileSelf
echo ============DONE==================
echo ============TEST2=================
echo Running 'fakemake -t -p -f tests/compileSelf' (Parallel Compile Self)
cmd /c %fakemake -t -p -f tests/compileSelf
echo ============DONE==================
echo ============TEST3=================
echo Running 'fakemake -f tests/empty' (Empty File)
cmd /c %fakemake -f tests/empty
echo ============DONE==================
echo ============TEST4=================
echo Running 'fakemake -f tests/badSyntax' (Bad Syntax)
cmd /c %fakemake -f tests/badSyntax
echo ============DONE==================
echo ============TEST5=================
echo Running 'fakemake -f tests/badRule' (Bad Rule)
cmd /c %fakemake -f tests/badRule
echo ============DONE==================
echo ============TEST6=================
echo Running 'fakemake -f tests/dependencyNotFound' (No Dependency)
cmd /c %fakemake -f tests/dependencyNotFound
echo ============DONE==================
echo ============TEST7=================
echo Running 'fakemake -f tests/badCommand' (Command Error)
cmd /c %fakemake -f tests/badCommand
echo ============DONE==================
echo ============TEST8=================
echo Running 'fakemake -f tests/convolutedDependency' (Convoluted)
cmd /c %fakemake -f tests/convolutedDependency
echo ============DONE==================
echo ============TEST9=================
echo Running 'fakemake -p -f tests/lotsOfCommands' (Serial)
cmd /c %fakemake -p -f tests/lotsOfCommands
echo ============DONE==================
echo ============TEST10================
echo Running 'fakemake -t -p -f tests/lotsOfCommands' (Parallel)
cmd /c %fakemake -t -p -f tests/lotsOfCommands
echo ============DONE==================
echo ============TEST11================
echo Running 'fakemake -t -p -f tests/circularDependency' (Circular Dependency)
cmd /c %fakemake -t -p -f tests/circularDependency
echo ============DONE==================

set end=%time%
set options="tokens=1-4 delims=:.,"
for /f %options% %%a in ("%start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f %options% %%a in ("%end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100

set /a hours=%end_h%-%start_h%
set /a mins=%end_m%-%start_m%
set /a secs=%end_s%-%start_s%
set /a ms=%end_ms%-%start_ms%
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms%
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs%
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins%
if %hours% lss 0 set /a hours = 24%hours%
if 1%ms% lss 100 set ms=0%ms%

:: Mission accomplished
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
echo Tests Done. Everything took %hours%:%mins%:%secs%.%ms% (%totalsecs%.%ms%s total)