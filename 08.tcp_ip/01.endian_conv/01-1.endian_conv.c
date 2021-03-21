#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;
	unsigned char *addr;
	
	addr = (unsigned char *)&host_port;
	
	printf("Port 1 : %p -> %#x\n", addr, *addr++);
	printf("Port 2 : %p -> %#x\n", addr, *addr++);
	printf("Port 3 : %p -> %#x\n", addr, *addr++);
	printf("Port 4 : %p -> %#x\n", addr, *addr);
	
	addr = (unsigned char *)&host_addr;
	
	printf("Address 1 : %p -> %#x\n", addr, *addr++);
	printf("Address 2 : %p -> %#x\n", addr, *addr++);
	printf("Address 3 : %p -> %#x\n", addr, *addr++);
	printf("Address 4 : %p -> %#x\n", addr, *addr);
	
	net_port=htons(host_port);
	net_addr=htonl(host_addr);
	
	addr = (unsigned char *)&net_port;
	
	printf("Port 1 : %p -> %#x\n", addr, *addr++);
	printf("Port 2 : %p -> %#x\n", addr, *addr++);
	
	addr = (unsigned char *)&net_addr;
	
	printf("Address 1 : %p -> %#x\n", addr, *addr++);
	printf("Address 2 : %p -> %#x\n", addr, *addr++);
	printf("Address 3 : %p -> %#x\n", addr, *addr++);
	printf("Address 4 : %p -> %#x\n", addr, *addr);
	
	printf("Host ordered port: %#x \n", host_port);
	printf("Network ordered port: %#x \n", net_port);
	printf("Host ordered address: %#lx \n", host_addr);
	printf("Network ordered address: %#lx \n", net_addr);
	
	return 0;
}
