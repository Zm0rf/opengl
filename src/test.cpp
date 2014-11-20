#include <stdio.h>
#include "test.h"

class TestClass: public BaseTestClass
{
	public:
		void foo()
		{
			printf("From dynamically linked class implementation\n");
		};
};

extern "C"
{
	BaseTestClass* test()
	/* int test() */
	{
		BaseTestClass *test = new TestClass();
		printf("This is from dynamical library\n");
		return test;
	}
}

