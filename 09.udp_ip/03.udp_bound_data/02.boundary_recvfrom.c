#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 50

int main(int argc, char *argv[]){
	int sock, str_len, i;
	char message[BUF_SIZE];
	struct sockaddr_in my_adr, your_adr;
	socklen_t adr_sz;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if((sock=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&my_adr, 0, sizeof(my_adr));
	my_adr.sin_family=AF_INET;
	my_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	my_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr))==-1) {
		perror("bind() error!!");
		exit(1);
	}
	
	for(i=0; i<3; i++) {
		sleep(2);	// delay 5 sec.
		adr_sz=sizeof(your_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&your_adr, &adr_sz);     
	
		printf("Message %d: %s \n", i+1, message);
	}
	close(sock);	
	return 0;
}
