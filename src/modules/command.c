#include "../include/command.h"
#include "../include/string.h"
#include "../include/console.h"
#include "../include/cpu_detect.h"

#define VERSION "0.01"

void command(char *cmd){
	cmd = strtok(cmd, " ");
	if(strcmp(cmd, "info")==0){
		con_printf("OS - TinyOS v%s\n", VERSION);
		detect_cpu();
		con_print("\n\n");
	} else if(strcmp(cmd, "help")==0){
		con_print("info - shows information about your computer\nhelp - shows all commands");
		con_print("\n\n");
	} else {
		con_setcolor(VGA_COLOR_RED);
		con_print("[ERROR] Unknown command.\n\n"); con_setcolor(VGA_COLOR_LIGHT_GREY);
	}
}