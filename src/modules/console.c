#include "../include/console.h"
#include "../include/ports.h"
#include "../include/string.h"

size_t row;
size_t col;
uint8_t con_color;
uint16_t* con_buffer;
 
void con_init(void){
    row = 0;
    col = 0;
    con_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    con_buffer = (uint16_t*) 0xC03FF000;
    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            con_buffer[index] = vga_entry(' ', con_color);
        }
    }
}

void update_cursor(){
    uint16_t pos = row * VGA_WIDTH + col;
 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void con_scroll() {
    row--;
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t src_index = y * VGA_WIDTH + x;
            const size_t dstty_index = (y + 1) * VGA_WIDTH + x;
            con_buffer[src_index] = con_buffer[dstty_index];
        }
        con_buffer[y] = con_buffer[y + 1];
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        con_buffer[index] = vga_entry(' ', con_color);
    }
}
 
void con_setcolor(uint8_t color){
    con_color = color;
}
 
void con_putch(char c){
    if(c != '\n'){
        const size_t index = row * VGA_WIDTH + col;
        con_buffer[index] = vga_entry(c, con_color);
    }
    if (++col == VGA_WIDTH || c == '\n') {
        col = 0;
        if (++row == VGA_HEIGHT) {
            con_scroll();
        }
    }
    update_cursor();
}
 
void con_print(const char* data){
    for(size_t i = 0; i < strlen(data); i++) con_putch(data[i]);
}

void con_putint(int i){
    if(i >= 0){
        unsigned int n, d = 1000000000;
        char str[255];
        unsigned int dec_index = 0;
        while( ( i/d == 0 ) && ( d >= 10 ) ) d /= 10;
        n = i;
        while(d >= 10){
            str[dec_index++] = ((char)((int)'0' + n/d));
            n = n % d;
            d /= 10;
        }
        str[dec_index++] = ((char)((int)'0' + n));
        str[dec_index] = 0;
        con_print(str);
    } else {
        con_putch('-');
        unsigned int n, d = 1000000000;
        char str[255];
        unsigned int dec_index = 0;
        while( ( i/d == 0 ) && ( d >= 10 ) ) d /= 10;
        n = -i;
        while(d >= 10){
            str[dec_index++] = ((char)((int)'0' + n/d));
            n = n % d;
            d /= 10;
        }
        str[dec_index++] = ((char)((int)'0' + n));
        str[dec_index] = 0;
        con_print(str);
    }
}


void con_puthex(uint32_t i){
    const unsigned char hex[16]  =  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned int n, d = 0x10000000;

    con_print("0x");
    while((i / d == 0) && (d >= 0x10)) d /= 0x10;
    n = i;
    while( d >= 0xF )
    {
        con_putch(hex[n/d]);
        n = n % d;
        d /= 0x10;
    }
    con_putch(hex[n]);
}

void con_printf(char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    int i = 0;
    char *string;

    while (fmt[i]){
        if (fmt[i] == '%'){
            i++;
            switch (fmt[i]){
                case 's':
                    string = va_arg(args, char*);
                    con_printf(string);
                    break;
                case 'c':
                    con_putch(va_arg(args, int));
                    break;
                case 'd':
                    con_putint(va_arg(args, int));
                    break;
                case 'i':
                    con_putint(va_arg(args, int));
                    break;
                case 'u':
                    con_putint(va_arg(args, unsigned int));
                    break;
                case 'x':
                    con_puthex(va_arg(args, uint32_t));
                    break;
                default:
                    con_putch(fmt[i]);
            }
        } else {
            con_putch(fmt[i]);
        }
        i++;
    }
}