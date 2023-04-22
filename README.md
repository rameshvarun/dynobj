# dynobj

[![C/C++ CI](https://github.com/rameshvarun/dynobj/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/rameshvarun/dynobj/actions/workflows/c-cpp.yml)

Prototypical object system in C. Not really that useful.

```c
#include <stdio.h>
#include "dynobj.h"

// Define a simple message handler that increments the count of the current object.
DYNOBJ_DEFINE_SIMPLE_MESSAGE(counter_increment) {
  dynobj_set_int_slot(self, "count", dynobj_get_int_slot(self, "count") + 1);
}

int main() {
  // Create a prototype representing our Counter "class"
  dynobj *counter_proto = dynobj_new();
  dynobj_set_int_slot(counter_proto, "count", 0);
  dynobj_set_ptr_slot(counter_proto, "increment", counter_increment);

  // Create two counter objects.
  dynobj *a = dynobj_extend(counter_proto);
  dynobj *b = dynobj_extend(counter_proto);

  DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");
  DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");
  DYNOBJ_SEND_SIMPLE_MESSAGE(a, "increment");

  DYNOBJ_SEND_SIMPLE_MESSAGE(b, "increment");
  DYNOBJ_SEND_SIMPLE_MESSAGE(b, "increment");

  printf("Counter A = %d, Counter B = %d\n",
    dynobj_get_int_slot(a, "count"),
    dynobj_get_int_slot(b, "count"));

  dynobj_destroy(a);
  dynobj_destroy(b);
  dynobj_destroy(counter_proto);
}
```
