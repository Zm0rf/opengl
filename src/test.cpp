#include <stdio.h>
#include "test.h"

class TestClass: public DynamicLoadingTestBaseClass
{
	public:
		void runTest();
		void runAltTest();
};

extern "C"
{
	DynamicLoadingTestBaseClass* test()
	/* int test() */
	{
		DynamicLoadingTestBaseClass *test = new TestClass();
		printf("This is from dynamical library\n");
		return test;
	}
}

void TestClass::runTest()
{
	printf("From dynamically linked class implementation\n");
}
void TestClass::runAltTest()
{
	printf("Running alternative test method\n");
}
