/* file: minunit.h taken from by Jera design - https://jera.com/techinfo/jtns/jtn002 and modified */

#ifndef MINUNIT_H
#define MINUNIT_H

#include <string.h>

extern int tests_run;
extern int tests_passed;

int run_all_tests(int argc, char **argv);

#define mu_assert(message, test) do \
{ \
	tests_run++; \
	if (!(test)) \
		printf("%s\n",message); \
	else \
		tests_passed++; \
	} \
while (0)

#define mu_run_test(test) do \
{ \
	test(); \
} \
while (0)

#endif
