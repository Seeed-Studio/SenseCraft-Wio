#include "FreeRTOS.h"
#include  <stdlib.h>
#if 0
// #ifdef __cplusplus
// extern "C" {
// #endif
// void *malloc(size_t size) {
//     /* Call the FreeRTOS version of malloc. */
//     return pvPortMalloc(size);
// }
// void free(void *ptr) {
//     /* Call the FreeRTOS version of free. */
//     vPortFree(ptr);
// }

// #ifdef __cplusplus
// }
// #endif

/*inline void *operator new(size_t size)
{
        void *p;
        if(uxTaskGetNumberOfTasks())
                p=pvPortMalloc(size);
        else
                p=malloc(size);
        return p;
}
inline void operator delete(void *p) noexcept
{
        if(uxTaskGetNumberOfTasks())
                vPortFree( p );
        else
                free( p );
        p = NULL;
}*/

// Define the �new� operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void *operator new(size_t size) {
    void *p;

    if (uxTaskGetNumberOfTasks())
        p = pvPortMalloc(size);
    else
        p = malloc(size);

    /*#ifdef __EXCEPTIONS
     if (p==0) // did pvPortMalloc succeed?
      throw std::bad_alloc(); // ANSI/ISO compliant behavior
    #endif*/

    return p;
}

//
// Define the �delete� operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void operator delete(void *p) noexcept {

    if (uxTaskGetNumberOfTasks())
        vPortFree(p);
    else
        free(p);

    p = NULL;
}

void *operator new[](size_t size) {
    void *p;

    if (uxTaskGetNumberOfTasks())
        p = pvPortMalloc(size);
    else
        p = malloc(size);

    /*#ifdef __EXCEPTIONS
     if (p==0) // did pvPortMalloc succeed?
      throw std::bad_alloc(); // ANSI/ISO compliant behavior
    #endif*/

    return p;
}

//
// Define the �delete� operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void operator delete[](void *p) noexcept {

    if (uxTaskGetNumberOfTasks())
        vPortFree(p);
    else
        free(p);

    p = NULL;
}
#endif 