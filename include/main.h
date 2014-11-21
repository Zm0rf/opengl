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
		/**
		 */
		LinkedObjectFactory(const char* source)
		{
			this->library = NULL;
			void* factory_function;
			//
			this->library = dlopen(source, RTLD_LAZY);
			if( this->library == NULL )
			{
				printf("Failed to open lib!\n");
				printf("Error: %s\n", dlerror());
				return;
			}
			factory_function = dlsym(this->library, "test");
			if( factory_function == NULL )
			{
				printf("Failed to fetch 'test'\n");
				printf("Error: %s\n", dlerror());
				this->free();
				return;
			}
			// Convert the void pointer to a function_type without compiler objections.
			union { function_type func; void* obj; } alias;
			alias.obj = factory_function;
			this->factory_function = alias.func;
		}

		/**
		 * Release all resources.
		 */
		~LinkedObjectFactory()
		{
			this->free();
		}

		/**
		 * Build an instance and return a pointer the created object or NULL if failed.
		 */
		BaseTestClass* build()
		{
			if( this->factory_function == NULL )
			{
				return NULL;
			}
			return this->factory_function();
		}

	private:
		typedef BaseTestClass* (*function_type)(void);
		void* library;
		function_type factory_function;

		/**
		 * Free all resources allocated by this.
		 * This will invalidate this object and any interaction with it is undefined.
		 */
		void free()
		{
			if( this->library != NULL )
			{
				dlclose(this->library);
			}
			this->library = NULL;
			this->factory_function = NULL;
		}
};
