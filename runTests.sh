# Running all the tests...
fakemake -p -f tests/compileSelf
echo "============DONE=================="
echo "============TEST2================="
echo "Running (Parallel Compile Self)"
fakemake -t -p -f tests/compileSelf
echo "============DONE=================="
echo "============TEST3================="
echo "Running (Empty File)"
fakemake -f tests/empty
echo "============DONE=================="
echo "============TEST4================="
echo "Running (Bad Syntax)"
fakemake -f tests/badSyntax
echo "============DONE=================="
echo "============TEST5================="
echo "Running (Bad Rule)"
fakemake -f tests/badRule
echo "============DONE=================="
echo "============TEST6================="
echo "Running (No Dependency)"
fakemake -f tests/dependencyNotFound
echo "============DONE=================="
echo "============TEST7================="
echo "Running (Command Error)"
cmd /c %fakemake -f tests/badCommand
echo "============DONE=================="
echo "============TEST8================="
echo "Running (Convoluted)"
cmd /c %fakemake -f tests/convolutedDependency
echo "============DONE=================="
echo "============TEST9================="
echo "Running (Heavy Duty Serial)"
cmd /c %fakemake -p -f tests/lotsOfCommands
echo "============DONE=================="
echo "============TEST10================="
echo "Running (Heavy Duty Parallel)"
cmd /c %fakemake -t -p -f tests/lotsOfCommands
echo "============DONE=================="
echo "============TEST11================"
echo "Running 'fakemake -t -p -f tests/circularDependency' (Circular Dependency)"
cmd /c %fakemake -t -p -f tests/circularDependency
echo "============DONE=================="
