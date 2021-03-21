/* myping.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
 * whole or in part in accordance to the General Public License (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

/*****************************************************************************/
/*** myping.c                                                              ***/
/***                                                                       ***/
/*** Use the ICMP protocol to request "echo" from destination.             ***/
/*****************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define PACKETSIZE	64

struct packet
{
	struct icmphdr hdr;
	char msg[PACKETSIZE-sizeof(struct icmphdr)];
};

int pid=-1;
struct protoent *proto=NULL;

/*--------------------------------------------------------------------*/
/*--- checksum - standard 1s complement checksum                   ---*/
/*--------------------------------------------------------------------*/
unsigned short checksum(void *b, int len)
{	unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

/*--------------------------------------------------------------------*/
/*--- display - present echo info                                  ---*/
/*--------------------------------------------------------------------*/
void display(void *buf, int bytes)
{	int i;
	struct iphdr *ip = buf;
	struct icmphdr *icmp = buf+ip->ihl*4;

	printf("-----------------------------------------");
	for (i = 0; i < bytes; i++){
		if ( !(i & 15) ) 
			printf("\n%x:  ", i);
		printf("%x ", ((unsigned char*)buf)[i]);
	}
	printf("\n");
	printf("IPv%d: hdr-size=%d pkt-size=%d protocol=%d TTL=%d src=%s ",
			ip->version, ip->ihl*4, ntohs(ip->tot_len), ip->protocol,
			ip->ttl, inet_ntoa(*(struct in_addr *)&ip->saddr));
	printf("dst=%s\n", inet_ntoa(*(struct in_addr *)&ip->daddr));
	if (icmp->un.echo.id == pid){
		printf("ICMP: type[%d/%d] checksum[%d] id[%d] seq[%d]\n",
			icmp->type, icmp->code, ntohs(icmp->checksum),
			icmp->un.echo.id, icmp->un.echo.sequence);
	}
}

/*--------------------------------------------------------------------*/
/*--- listener - separate process to listen for and collect messages--*/
/*--------------------------------------------------------------------*/
void listener(void){
	int sd;
	struct sockaddr_in addr;
	unsigned char buf[1024];
	unsigned char *buff;
	struct msghdr msg;
	struct iovec iov;
	char recvbuf[1024];
	char controlbuf[1024];
	char addrbuf[128];
	struct iphdr *iphdr;
	struct icmphdr *icmphdr;
	int i;
	char *tmp;

	// sd = socket(PF_INET, SOCK_RAW, proto->p_proto);
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if ( sd < 0 ){
		perror("socket");
		exit(0);
	}
	for (;;){	
		int bytes, len=sizeof(addr);
		bzero(buf, sizeof(buf));
		
		iov.iov_base = recvbuf;
		iov.iov_len = sizeof(recvbuf);
		memset(&msg, 0, sizeof(msg));
		msg.msg_name = addrbuf;
		msg.msg_namelen = sizeof(addrbuf);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
		msg.msg_control = controlbuf;
		msg.msg_controllen = sizeof(controlbuf);
		
		
		bytes = recvfrom(sd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&addr, &len);
		printf("***Got message!***\n");
		
		buff = msg.msg_iov->iov_base;
		iphdr = (struct iphdr*)buff;
		
		for(i=0, tmp=(char *)iphdr; i<sizeof(iphdr); i++){
			printf("%x ", *tmp);
			tmp++;
		}
		
		
		
		icmphdr = (struct icmphdr*)(buff+(iphdr->ihl*4));
		
		if (icmphdr->un.echo.id == pid){
			if (icmphdr->type == ICMP_ECHOREPLY)
				printf("ICMP_ECHOREPLY\n");
			else if (icmphdr->type == ICMP_DEST_UNREACH)
				printf("ICMP_DEST_UNREACH\n");
			else
				printf("DEST_DOWN\n");
		}else
			printf("*******************\n");
		sleep(1);
	}
	exit(0);
}

/*--------------------------------------------------------------------*/
/*--- ping - Create message and send it.                           ---*/
/*--------------------------------------------------------------------*/
void ping(struct sockaddr_in *addr){
	const int val=255;
	int i, sd, cnt=1;
	struct packet pckt;
	struct sockaddr_in r_addr;
	char *tmp;

	sd = socket(PF_INET, SOCK_RAW, proto->p_proto);
	// sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sd < 0){
		perror("socket");
		return;
	}
	if (setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
		perror("Set TTL option");
	if (fcntl(sd, F_SETFL, O_NONBLOCK) != 0 )
		perror("Request nonblocking I/O");
	for (;;){
		int len=sizeof(r_addr);
		// printf("Msg #%d\n", cnt);
		// if (recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)&r_addr, &len) > 0 )
			// printf("***Got message!***\n");
		bzero(&pckt, sizeof(pckt));
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = pid;
		printf("\n");
		for(i=0, tmp=(char*)&pckt.hdr; i<sizeof(pckt.hdr); i++){
			printf("%x ", *tmp++);
		}
		printf("\n");
		for (i = 0; i < sizeof(pckt.msg)-1; i++)
			pckt.msg[i] = i+'0';
		pckt.msg[i] = 0;
		printf("Msg(%d) #%d\n", pid, cnt);
		pckt.hdr.un.echo.sequence = cnt++;
		pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
		if (sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr, sizeof(*addr)) <= 0 )
			perror("sendto");
		sleep(1);
	}
}

/*--------------------------------------------------------------------*/
/*--- main - look up host and start ping processes.                ---*/
/*--------------------------------------------------------------------*/
int main(int count, char *argv[])
{	struct hostent *hname;
	struct sockaddr_in addr;
	
	if ( count != 2 ){
		printf("usage: %s <addr>\n", argv[0]);
		exit(0);
	}
	if (count > 1){
		pid = getpid();
		proto = getprotobyname("ICMP");
		
		bzero(&addr, sizeof(addr));
		
		addr.sin_family=AF_INET;
		addr.sin_addr.s_addr=inet_addr(argv[1]);
		addr.sin_port=0;
		
		if (fork() == 0)
			listener();
		else
			ping(&addr);
		wait(0);
	}else
		printf("usage: %s <ip addr>\n", argv[0]);
	return 0;
}