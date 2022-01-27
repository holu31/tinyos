#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "ports.h"


char scancodes[128];

int kb_get();
void kb_init();