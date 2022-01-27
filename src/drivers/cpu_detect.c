#include "../include/cpu_detect.h"
#include "../include/console.h"

/* Simply call this function detect_cpu(); */
int detect_cpu(void) { /* or main() if your trying to port this as an independant application */
	unsigned long ebx, unused;
	cpuid(0, unused, ebx, unused, unused);
	switch(ebx) {
		case 0x756e6547: /* Intel Magic Code */
		do_intel();
		break;
		case 0x68747541: /* AMD Magic Code */
		do_amd();
		break;
		default:
		con_printf("Unknown x86 CPU Detected\n");
		break;
	}
	return 0;
}

/* Intel Specific brand list */
char *Intel[] = {
	"Brand ID Not Supported.", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Intel(R) Pentium(R) III Xeon(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) III processor-M", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Xeon(R) Processor", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) 4 processor-M", 
	"Mobile Intel(R) Pentium(R) Celeron(R) processor", 
	"Reserved", 
	"Mobile Genuine Intel(R) processor", 
	"Intel(R) Celeron(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Celeron(R) processor", 
	"Mobile Geniune Intel(R) processor", 
	"Intel(R) Pentium(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor"
};

/* This table is for those brand strings that have two values depending on the processor signature. It should have the same number of entries as the above table. */
char *Intel_Other[] = {
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Celeron(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved"
};

/* Intel-specific information */
int do_intel(void) {
	unsigned long eax, ebx, ecx, edx, max_eax, signature, unused;
	int model, family, type, brand, stepping, reserved;
	int extended_family = -1;
	cpuid(1, eax, ebx, unused, unused);
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	type = (eax >> 12) & 0x3;
	brand = ebx & 0xff;
	stepping = eax & 0xf;
	reserved = eax >> 14;
	signature = eax;
	con_printf("CPU Type - ");
	switch(type) {
		case 0:
		con_print("Original OEM");
		break;
		case 1:
		con_print("Overdrive");
		break;
		case 2:
		con_print("Dual-capable");
		break;
		case 3:
		con_print("Reserved");
		break;
	}
	con_print("\n");
	con_printf("CPU - ");
	switch(family) {
		case 3:
		con_print("i386");
		break;
		case 4:
		con_print("i486");
		break;
		case 5:
		con_print("Pentium");
		break;
		case 6:
		con_print("Pentium Pro");
		break;
		case 15:
		con_print("Pentium 4");
	}
	if(family == 15) {
		extended_family = (eax >> 20) & 0xff;
		con_printf("Extended family %d\n", extended_family);
	}
	con_printf(" ");
	switch(family) {
		case 3:
		break;
		case 4:
		switch(model) {
			case 0:
			case 1:
			con_print("DX");
			break;
			case 2:
			con_print("SX");
			break;
			case 3:
			con_print("487/DX2");
			break;
			case 4:
			con_print("SL");
			break;
			case 5:
			con_print("SX2");
			break;
			case 7:
			con_print("Write-back enhanced DX2");
			break;
			case 8:
			con_print("DX4");
			break;
		}
		break;
		case 5:
		switch(model) {
			case 1:
			con_print("60/66");
			break;
			case 2:
			con_print("75-200");
			break;
			case 3:
			con_print("for 486 system");
			break;
			case 4:
			con_print("MMX");
			break;
		}
		break;
		case 6:
		switch(model) {
			case 1:
			con_print("Pentium Pro");
			break;
			case 3:
			con_print("Pentium II Model 3");
			break;
			case 5:
			con_print("Pentium II Model 5/Xeon/Celeron");
			break;
			case 6:
			con_print("Celeron");
			break;
			case 7:
			con_print("Pentium III/Pentium III Xeon - external L2 cache");
			break;
			case 8:
			con_print("Pentium III/Pentium III Xeon - internal L2 cache");
			break;
		}
		break;
		case 15:
		break;
	}
	con_print("\n");
	cpuid(0x80000000, max_eax, unused, unused, unused);
	if(max_eax >= 0x80000004) {
		con_print("Brand - ");
		if(max_eax >= 0x80000002) {
			cpuid(0x80000002, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000003) {
			cpuid(0x80000003, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000004) {
			cpuid(0x80000004, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		con_print("\n");
	} else if(brand > 0) {
		con_printf("Brand - ");
		if(brand < 0x18) {
			if(signature == 0x000006B1 || signature == 0x00000F13) {
				con_printf("%s\n", Intel_Other[brand]);
			} else {
				con_printf("%s\n", Intel[brand]);
			}
		} else {
			con_printf("Reserved\n");
		}
	}
	con_printf("Stepping: %d Reserved: %d\n", stepping, reserved);
	return 0;
}

/* Print Registers */
void printregs(int eax, int ebx, int ecx, int edx) {
	int j;
	char string[17];
	string[16] = '\0';
	for(j = 0; j < 4; j++) {
		string[j] = eax >> (8 * j);
		string[j + 4] = ebx >> (8 * j);
		string[j + 8] = ecx >> (8 * j);
		string[j + 12] = edx >> (8 * j);
	}
	con_printf("%s", string);
}

/* AMD-specific information */
int do_amd(void){
	unsigned long extended, eax, ebx, ecx, edx, unused;
	int family, model, stepping, reserved;
	cpuid(1, eax, unused, unused, unused);
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	stepping = eax & 0xf;
	reserved = eax >> 12;
	con_printf("Family: %d Model: %d [", family, model);
	switch(family){
		case 4:
		con_printf("486 Model %d", model);
		break;
		case 5:
		switch(model){
			case 0:
			case 1:
			case 2:
			case 3:
			case 6:
			case 7:
			con_printf("K6 Model %d", model);
			break;
			case 8:
			con_print("K6-2 Model 8");
			break;
			case 9:
			con_print("K6-III Model 9");
			break;
			default:
			con_printf("K5/K6 Model %d", model);
			break;
		}
		break;
		case 6:
		switch(model) {
			case 1:
			case 2:
			case 4:
			con_printf("Athlon Model %d", model);
			break;
			case 3:
			con_print("Duron Model 3");
			break;
			case 6:
			con_print("Athlon MP/Mobile Athlon Model 6");
			break;
			case 7:
			con_print("Mobile Duron Model 7");
			break;
			default:
			con_printf("Duron/Athlon Model %d", model);
			break;
		}
		break;
	}
	con_print("]\n");
	cpuid(0x80000000, extended, unused, unused, unused);
	if(extended == 0) {
		return 0;
	}
	if(extended >= 0x80000002) {
		unsigned int j;
		con_print("Detected Processor Name: ");
		for(j = 0x80000002; j <= 0x80000004; j++) {
			cpuid(j, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		con_print("\n");
	}
	if(extended >= 0x80000007) {
		cpuid(0x80000007, unused, unused, unused, edx);
		if(edx & 1) {
			con_print("Temperature Sensing Diode Detected!\n");
		}
	}
	con_printf("Stepping: %d Reserved: %d\n", stepping, reserved);
	return 0;
}