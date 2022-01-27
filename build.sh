echo "Compiling modules"
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/modules/console.c -o bin/console.o
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/modules/ports.c -o bin/ports.o
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/modules/command.c -o bin/command.o

echo "Compiling libc"
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/libc/string.c -o bin/string.o

echo "Compiling drivers"
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/cpu_detect.c -o bin/cpu_detect.o
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/drivers/keyboard.c -o bin/keyboard.o

echo "Compiling kernel"
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/kernel.c -o bin/kernel.o
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -c src/boot.s -o bin/boot.o
./1686-elf/bin/i686-elf-gcc -g -I include -ffreestanding -Wall -Wextra -O0 -nostdlib -lgcc -T src/link.ld -o bin/kernel.elf bin/kernel.o bin/boot.o bin/console.o bin/ports.o bin/cpu_detect.o bin/keyboard.o bin/string.o bin/command.o
cp bin/kernel.elf build/boot/kernel.elf
grub-mkrescue -o kernel.iso build/