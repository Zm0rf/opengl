#include <stdio.h>

#include <dlfcn.h>
#include <stdlib.h>
#include "test.h"

/**
 * TODO move to own file
 */
class LinkedObjectFactory
{
	public:
		LinkedObjectFactory(const char* source);
		~LinkedObjectFactory();
		void* build();
	private:
		void* library;
		void* factory_function;
};
