#pragma once

#include <stdio.h>
#include <stdlib.h>

// Logging
#define INFO(format, ...) { fprintf(stderr, "\033[96m[INFO]:\x1b[0m " format "\n", __VA_ARGS__); }
#define WARN(format, ...) { fprintf(stderr, "\x1b[33m[WARN]:\x1b[0m " format "\n", __VA_ARGS__); }
#define DEBUG(format, ...) { const char *debug = getenv("MCPI_DEBUG"); if (debug != NULL && strlen(debug) > 0) { fprintf(stderr, "[DEBUG]: " format "\n", __VA_ARGS__); } }
#define ERR(format, ...) { fprintf(stderr, "\x1b[33m[ERR]: (%s:%i):\x1b[0m " format "\n", __FILE__, __LINE__, __VA_ARGS__); exit(EXIT_FAILURE); }
#define IMPOSSIBLE() ERR("%s", "\x1b[31mThis Should Never Be Call'\x1b[0m")
