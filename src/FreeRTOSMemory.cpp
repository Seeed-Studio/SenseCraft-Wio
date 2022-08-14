#include "FreeRTOS.h"
#include <malloc.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

_PTR _malloc_r(struct _reent *r, size_t sz) {
    return pvPortMalloc(sz);
}

_PTR _calloc_r(struct _reent *r, size_t a, size_t b) {
    return pvPortMalloc(a * b);
}

void _free_r(struct _reent *r, _PTR x) {
    vPortFree(x);
}

_PTR _realloc_r(struct _reent *r, _PTR x, size_t sz) {
    vPortFree(x);
    return pvPortMalloc(sz);
}

#ifdef __cplusplus
}
#endif