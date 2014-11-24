/* #include "gtest/gtest.h" */

#include <stdio.h>
#include "dynamic_loading/BaseClass.h"
#include "LinkedObjectFactory.h"

int main(int argc, char** argv)
{
    //
    LinkedObjectFactory<BaseClass> f(TEST_LIBRARY_PATH);
    if( !f.isLoaded() )
    {
        fprintf(stderr, "Library not loaded!");
        return -1;
    }
    BaseClass* test = f.build();
    test->printTestMessage();
    delete test;
    return 0;
}
