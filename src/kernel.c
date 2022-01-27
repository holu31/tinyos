#include "include/console.h"
#include "include/cpu_detect.h"
#include "include/keyboard.h"
#include "include/command.h"
#include "include/string.h"

#define VERSION "0.01"
char buffer[80];

void keyboard(){
	con_setcolor(VGA_COLOR_MAGENTA);
	con_putch('>');con_setcolor(VGA_COLOR_LIGHT_GREY);
	kb_init();
    while(1){
    	int scancode = kb_get();
    	if(scancode < 129){
    		if(scancode == 28){
    			con_putch(scancodes[scancode]);
    			command(buffer);
    			int bufflen = strlen(buffer);
    			for(int i=0;i<bufflen; i++) buffer[i] = '\0';
    			con_setcolor(VGA_COLOR_MAGENTA);
				con_putch('>');con_setcolor(VGA_COLOR_LIGHT_GREY);
    		} else {
    			con_putch(scancodes[scancode]);
    			buffer[strlen(buffer)] = scancodes[scancode];
    		}
    		if(strlen(buffer) == 79){
    			con_setcolor(VGA_COLOR_RED);
    			con_print("[ERROR] Buffer is full.\n\n");
    			int bufflen = strlen(buffer);
    			for(int i=0;i<bufflen; i++) buffer[i] = '\0';
    			con_setcolor(VGA_COLOR_MAGENTA);
				con_putch('>');con_setcolor(VGA_COLOR_LIGHT_GREY);
    		}
    	}
    }
}

void kernel_main(){
	con_init();
	con_setcolor(VGA_COLOR_LIGHT_MAGENTA);
	for(int i=0;i<81;i++) con_putch('#');
	for(int i=0;i<36;i++) con_putch(' ');
	con_setcolor(VGA_COLOR_LIGHT_GREY);
    con_print("TinyOS");
    con_setcolor(VGA_COLOR_LIGHT_MAGENTA);
    for(int i=0;i<36;i++) con_putch(' ');
	for(int i=0;i<81;i++) con_putch('#');
	con_setcolor(VGA_COLOR_LIGHT_GREY); con_putch('\n');
    keyboard();
	while(1);
}