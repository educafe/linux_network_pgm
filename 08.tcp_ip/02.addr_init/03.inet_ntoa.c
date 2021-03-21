/* 	
*	The function inet_ntoa() converts a network address in a struct in_addr 
*	to a dots-andnumbers format string. 
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct sockaddr_in addr1, addr2, addr3, addr4;
	char *str_ptr;
   
	addr1.sin_addr.s_addr=htonl(0x01020304);
	addr2.sin_addr.s_addr=htonl(0x01010101);
	addr3.sin_addr.s_addr=htonl(0x4e7cd47f);			//127.212.124.78
	addr4.sin_addr.s_addr=htonl(0x7fd47c4e);
	
	printf("%x\n", addr1.sin_addr.s_addr);
	str_ptr=inet_ntoa(addr1.sin_addr);
	printf("Dotted-Decimal notation1: %s \n", str_ptr);
	
	str_ptr=inet_ntoa(addr2.sin_addr);
	printf("Dotted-Decimal notation2: %s \n", str_ptr);
	
	printf("%x\n", addr3.sin_addr.s_addr);
	str_ptr=inet_ntoa(addr3.sin_addr);
	printf("Dotted-Decimal notation3: %s \n", str_ptr);
	
	printf("%x\n", addr4.sin_addr.s_addr);
	str_ptr=inet_ntoa(addr4.sin_addr);
	printf("Dotted-Decimal notation3: %s \n", str_ptr);
	
	return 0;
}

