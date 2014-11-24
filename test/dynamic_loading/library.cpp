#include <stdio.h>
#include "dynamic_loading/BaseClass.h"

class ImplementationClass: public BaseClass
{
    public:
        void printTestMessage();
};
extern "C"
{
    BaseClass* build()
    {
        return new ImplementationClass();
    }
}
void ImplementationClass::printTestMessage()
{
    printf("Printing from internal class!\n");
}
