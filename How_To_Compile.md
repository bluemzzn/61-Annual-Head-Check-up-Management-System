# How to compile main program 

` gcc -o checkup main.c fortest.c unit_test.c e2e_test.c `

# Run main program
` ./checkup `

# How to compile unit test

` gcc -o unit_test unit_test.c fortest.c `

# Run unit test

` ./unit_test `

# How to run end-to-end test

` gcc -o e2e_runner e2e_test.c fortest.c `

# Run End-to-end test

` ./e2e_runner `