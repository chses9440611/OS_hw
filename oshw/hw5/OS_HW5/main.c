#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "os_hw5_driver.h"

int fd;

// Obtain my cr3 value (a.k.a. PML4 table physical address)
uint64_t get_cr3_value()
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_CR3;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Given a physical address, return the value
uint64_t read_physical_address(uint64_t physical_address)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_READ;
	cmd.request[1] = physical_address;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Write value to a physical address
void write_physical_address(uint64_t physical_address, uint64_t value)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_WRITE;
	cmd.request[1] = physical_address;
	cmd.request[2] = value;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
}

uint64_t find_pte_addr(uint64_t virtual_address){
	uint64_t 	offset_mask = 0x00000000000001FF;
	uint64_t 	base_mask   = 0x000FFFFFFFFFF000;
	uint64_t 	PML4_offset = (virtual_address >> 39) & offset_mask;
	uint64_t 	PDP_offset = (virtual_address >> 30) & offset_mask;
	uint64_t  	PD_offset	= (virtual_address >> 21) & offset_mask;
	uint64_t  	PT_offset	= (virtual_address >> 12) & offset_mask;
	uint64_t 	physical_page_offset = virtual_address & 0x0000000000000FFF;
	uint64_t 	PML4_base   = get_cr3_value() & base_mask;
	uint64_t 	PML4_addr 	= PML4_base + (PML4_offset << 3);
	uint64_t 	PML4_content= read_physical_address(PML4_addr);

	uint64_t	PDP_base	= PML4_content & base_mask;
	uint64_t	PDP_addr	= PDP_base + (PDP_offset << 3);
	uint64_t	PDP_content= read_physical_address(PDP_addr);

	uint64_t	PD_base	= PDP_content & base_mask;
	uint64_t	PD_addr	= PD_base + (PD_offset << 3);
	uint64_t	PD_content = read_physical_address(PD_addr);

	uint64_t	PT_base	= PD_content & base_mask;
	uint64_t	PT_addr	= PT_base + (PT_offset << 3);
	uint64_t	PT_content	= read_physical_address(PT_addr);

	uint64_t	physical_page_base = PT_content & base_mask;
	uint64_t	physical_page_addr = physical_page_base + physical_page_offset;
	uint64_t	physical_addr = read_physical_address(physical_page_addr);


	printf("Virtual Address: 0x%016lx\n", virtual_address);
	printf("PML4_offset: 	0x%016lx\n", PML4_offset);
	printf("PDPE_offset: 	0x%016lx\n", PDP_offset);
	printf("PDE_offset:  	0x%016lx\n", PD_offset);
	printf("PTE_offset: 	0x%016lx\n", PT_offset);
	printf("physical_page_offset:0x%016lx\n", physical_page_offset);
	printf("PML4_base: 		0x%016lx\n", PML4_base);
	printf("PML4_addr: 		0x%016lx\n", PML4_addr);
	printf("PML4_content: 	0x%016lx\n", PML4_content);

	printf("PDPE_base:	0x%016lx\n", PDP_base);
	printf("PDPE_addr:	0x%016lx\n", PDP_addr);
	printf("PDPE_content:	0x%016lx\n", PDP_content);
	
	printf("PDE_base:	0x%016lx\n", PD_base);
	printf("PDE_addr:	0x%016lx\n", PD_addr);
	printf("PDE_content:	0x%016lx\n", PD_content);
	
	printf("PTE_base:	0x%016lx\n", PT_base);
	printf("PTE_addr:	0x%016lx\n", PT_addr);
	printf("PTE_content:	0x%016lx\n", PT_content);
	
	printf("physical_page_base:	0x%016lx\n", physical_page_base);
	printf("physical_page_addr:	0x%016lx\n", physical_page_addr);
	printf("physical__addr: 0x%016lx\n", physical_addr);
	return PT_addr;
}

int main()
{
	char *x = (char*)aligned_alloc(4096, 4096) + 0x123;
	char *y = (char*)aligned_alloc(4096, 4096) + 0x123;
	strcpy(x, "This is OS homework 5.");
	strcpy(y, "You have to modify my page table.");

	fd = open("/dev/os", O_RDONLY);
	if(fd < 0) 
	{
		printf("Cannot open device!\n");
		return 0;
	}

	printf("Before\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 1 */
	// ------------------------------------------------
	// Modify page table entry of y
	// Let y point to x's physical address
	uint64_t x_pte_addr = find_pte_addr(x);
	uint64_t y_pte_addr = find_pte_addr(y);
	
	uint64_t y_pte_origin = read_physical_address(y_pte_addr);
	uint64_t x_pte = read_physical_address(x_pte_addr);
	write_physical_address(y_pte_addr, x_pte);
	// ------------------------------------------------

	getchar();

	printf("After modifying page table\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	getchar();

	strcpy(y, "When you modify y, x is modified actually.");
	printf("After modifying string y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 2 */
	// ------------------------------------------------
	// Recover page table entry of y
	// Let y point to its original address
	// You may need to store y's original address at previous step
	write_physical_address(y_pte_addr, y_pte_origin);
	// ------------------------------------------------

	getchar();

	printf("After recovering page table of y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	close(fd);
}
