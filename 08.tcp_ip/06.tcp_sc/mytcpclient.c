#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	int sockfd, numbytes; 
	struct sockaddr_in their_addr;
	char tx_buf[128], rx_buf[128];
	int i;

	if(argc != 2){
		fprintf(stderr, "usage : client serverip \n");
		exit(1);
	}

	//socket open
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket() error");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(10000);
	their_addr.sin_addr.s_addr = inet_addr(argv[1]);    
	/*  their_addr.sin_addr.s_addr = inet_addr("70.12.117.90");  */
	memset(&(their_addr.sin_zero), '\0',8);

	printf("[ %s ]\n", inet_ntoa(their_addr.sin_addr));

	//connection request to server
	if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
		perror("connect() error");
		exit(1);
	}
	
	for(i=1; i<=10; i++) {
		memset(tx_buf, 0, sizeof(tx_buf));
		memset(rx_buf, 0, sizeof(rx_buf));
		sprintf(tx_buf, "Hello_%d server(from %d)!!\n", i, getpid());
		//messge send to server
		if(send(sockfd, tx_buf, strlen(tx_buf)+1, 0) == -1) perror("send");
		//message rx wait from server
		if((numbytes = recv(sockfd, rx_buf, sizeof(rx_buf), 0)) == -1){
			perror("recv");
			exit(1);
		}
		printf("----->Client Received : %s", rx_buf);
		sleep((getpid()+i)%5);
	}
	//close socket
	close(sockfd);
	return 0;
}

