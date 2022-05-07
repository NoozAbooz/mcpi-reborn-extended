#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Colors
char *color_reset();
char *color_info();
char *color_warn();
char *color_error();
char *color_debug();

// Server Mode
#define INFO(format, ...) { fprintf(stderr, "%s[INFO]: " format "%s\n", color_info(), ##__VA_ARGS__, color_reset()); }
#define WARN(format, ...) { fprintf(stderr, "%s[WARN]: " format "%s\n", color_warn(), ##__VA_ARGS__, color_reset()); }
#define ERR(format, ...) { fprintf(stderr, "%s[ERR]: (%s:%i): " format "%s\n", color_error(), __FILE__, __LINE__, ##__VA_ARGS__, color_reset()); exit(EXIT_FAILURE); }
#define DEBUG(format, ...) { const char *debug = getenv("MCPI_DEBUG"); if (debug != NULL && strlen(debug) > 0) { fprintf(stderr, "%s[DEBUG]: " format "%s\n", color_debug(), ##__VA_ARGS__, color_reset()); } }
#define IMPOSSIBLE() ERR("\x1b[41mWe've reached an unreachable state. Anything is possible. The limits were in our heads all along. Follow your dreams.\x1b[0m ");

#ifdef __cplusplus
}
#endif