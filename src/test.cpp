#include <stdio.h>

extern "C"
{
	void test()
	{
		printf("This is from dynamical library\n");
	}
}
