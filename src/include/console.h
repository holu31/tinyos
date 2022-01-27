#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "vga.h"

void con_init(void);
void con_setcolor(uint8_t color);
void update_cursor();
void con_putch(char c);
void con_print(const char* data);
void con_putint(int i);
void con_puthex(uint32_t i);
void con_printf(char *fmt, ...);