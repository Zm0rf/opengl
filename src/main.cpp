#include "main.h"

int main(int argc, char** argv)
{
	printf("TEST\n");

	void* lib_test = dlopen("./libtest.so", RTLD_LAZY);
	if( lib_test == NULL )
	{
		printf("Failed to open lib!\n");
		printf("Error: %s\n", dlerror());
		return 1;
	}
	void* foo = dlsym(lib_test, "test");
	if( foo == NULL )
	{
		printf("Failed to fetch 'test'\n");
		printf("Error: %s\n", dlerror());
		dlclose(lib_test);
		return 2;
	}
	typedef BaseTestClass* (*func_test)(void);
	union { func_test func; void* obj; } alias;
	alias.obj = foo;
	func_test func = alias.func;
	/* func(); */
	BaseTestClass *test = func();
	if( test != NULL )
	{
		test->foo();
	}
	dlclose(lib_test);

	/* LinkedObjectFactory f("./libtest.so"); */
	return 0;
}


/* LinkedObjectFactory::LinkedObjectFactory(const char* source) */
/* { */
/* 	this->library = NULL; */
/* 	this->factory_function = NULL; */
/* 	// */
/* 	this->library = dlopen(source, RTLD_LAZY); */
/* 	if( this->library == NULL ) */
/* 	{ */
/* 		printf("Failed to open lib!\n"); */
/* 		printf("Error: %s\n", dlerror()); */
/* 		return; */
/* 	} */
/* 	this->factory_function = dlsym(lib_test, "test"); */
/* 	if( this->factory_function != NULL ) */
/* 	{ */
/* 		printf("Failed to fetch 'test'\n"); */
/* 		printf("Error: %s\n", dlerror()); */
/* 		return; */
/* 	} */
/* 	/1* typedef BaseTestClass* (*func_test)(void); *1/ */
/* 	/1* union { func_test func; void* obj; } alias; *1/ */
/* 	/1* alias.obj = foo; *1/ */
/* 	/1* func_test func = alias.func; *1/ */
/* 	/1* /2* func(); *2/ *1/ */
/* 	/1* BaseTestClass *test = func(); *1/ */
/* 	/1* if( test != NULL ) *1/ */
/* 	/1* { *1/ */
/* 	/1* 	test->foo(); *1/ */
/* 	/1* } *1/ */
/* 	/1* dlclose(lib_test); *1/ */
/* } */
/* LinkedObjectFactory::~LinkedObjectFactory() */
/* { */
/* 	if( this->library ) */
/* 		dlclose(this->library); */
/* } */
/* void* LinkedObjectFactory::build() */
/* { */
/* 	return NULL; */
/* } */
