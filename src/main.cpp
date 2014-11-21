#include "main.h"

int main(int argc, char** argv)
{
	printf("TEST compiled at %s %s\n", __DATE__, __TIME__);
	LinkedObjectFactory f("./libtest.so");
	BaseTestClass* test = f.build();
	test->foo();
	delete test;
	return 0;
}

