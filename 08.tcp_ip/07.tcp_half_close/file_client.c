#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
	int sd, ret, read_cnt;
	FILE *fp;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	
	if(argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("receive.dat", "wb");
	sd=socket(PF_INET, SOCK_STREAM, 0);   

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	
	while((read_cnt=read(sd, buf, BUF_SIZE ))!=0)
		fwrite((void*)buf, 1, read_cnt, fp);
	
	puts("Received file data");
	fclose(fp);

	printf("Enter to send message...");
	getchar();
	ret = write(sd, "Thank you~~", 12);
	if(ret <= 0) perror("write error");
	else printf("write ok(ret:%d)!!\n", ret);
	
	printf("Enter to send message...");
	getchar();
	ret = write(sd, "Thank you!!", 12);
	if(ret <= 0) perror("write error");
	else printf("write ok(ret:%d)!!\n", ret);

	close(sd);
	return 0;
}
