#include <stdio.h>
#include <unistd.h>

#define COLOR(name, value) \
    char *color_##name() { \
        static char *out = NULL; \
        if (out == NULL) { \
            out = isatty(fileno(stderr)) ? "\x1b[" value "m" : ""; \
        } \
        return out; \
    }

COLOR(reset, "0")
COLOR(info, "96")
COLOR(warn, "93")
COLOR(error, "91")
COLOR(debug, "92")