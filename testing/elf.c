#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <elf.h>

int main(int argc, char *argv[])
{
	Elf32_Ehdr ehdr; // main header

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	read(fd, &ehdr, sizeof(ehdr));
	printf("e_ident: %x %x %x %x\n", ehdr.e_ident[0], ehdr.e_ident[1], ehdr.e_ident[2], ehdr.e_ident[3]);
	printf("e_type: %x\n", ehdr.e_type);
	printf("e_machine: %x\n", ehdr.e_machine);
	printf("e_version: %x\n", ehdr.e_version);
	printf("e_entry: %x\n", ehdr.e_entry);
	printf("e_phoff: %x\n", ehdr.e_phoff);
	printf("e_shoff: %x\n", ehdr.e_shoff);
	printf("e_flags: %x\n", ehdr.e_flags);
	printf("e_ehsize: %x\n", ehdr.e_ehsize);
	printf("e_phentsize: %x\n", ehdr.e_phentsize);
	printf("e_phnum: %x\n", ehdr.e_phnum);
	printf("e_shentsize: %x\n", ehdr.e_shentsize);
	printf("e_shnum: %x\n", ehdr.e_shnum);
	printf("e_shstrndx: %x\n", ehdr.e_shstrndx);

	int addr = atoi(argv[2]);
	printf("chaning entry point to %x\n", addr);
	ehdr.e_entry = addr;

	lseek(fd, 0, SEEK_SET);
	write(fd, &ehdr, sizeof(ehdr));
	close(fd);
	
	return (0);
}
