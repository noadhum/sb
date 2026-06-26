# sb
Simple StringBuilder implementation in C

## Example
```c
#include <stdio.h>

#define SB_IMPLEMENTATION
#include "sb.h"

int main(void)
{
    String_Builder sb = {0};
    sb_append(&sb, "Hello World");

    printf("%s\n", sb.data); // Prints "Hello World"!

    sb_free(&sb);
    return 0;
}
```
