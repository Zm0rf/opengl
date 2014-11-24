#pragma once
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#define INIT_FUNCTION_NAME init
#define DEINIT_FUNCTION_NAME deinit
#define BUILD_FUNCTION_NAME "build"
/**
 * TODO move to own file
 */
template<typename T>
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
				fprintf(stderr, "Failed to open lib!\n");
				fprintf(stderr, "Error: %s\n", dlerror());
				return;
			}
			factory_function = dlsym(this->library, BUILD_FUNCTION_NAME);
			if( factory_function == NULL )
			{
				fprintf(stderr, "Failed to fetch 'test'\n");
				fprintf(stderr, "Error: %s\n", dlerror());
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
		T* build()
		{
			if( this->factory_function == NULL )
			{
				return NULL;
			}
			return this->factory_function();
		}

        bool isLoaded()
        {
            return this->factory_function != NULL;
        }

	private:
		typedef T* (*function_type)(void);
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
