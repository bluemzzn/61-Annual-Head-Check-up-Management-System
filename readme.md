# How to compile main program 
gcc -o checkup main.c 

# Run
./checkup

# How to compile main program with unit test
gcc -o checkup main.c unit_test.c

<!-- 
# How to compile only unit test 
gcc -DUNIT_TEST_STANDALONE -o unit_test unit_test.c

# How to run unit test
./unit_test -->
