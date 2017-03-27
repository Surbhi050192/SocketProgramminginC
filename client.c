/*
 *** Stream socket for client taken from beej guide
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT "25841" // client is connected to this port


void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
} 
int main(int argc, char *argv[])
{

	int sockfd, numbytes;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("nunki.usc.edu", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	/* Reading File*/
	
	FILE *fileNum;
	fileNum = fopen("nums.csv" , "r");
	int size=0;
	int *arr =(int *) malloc(sizeof(int)*1000);
	int i=0;
	if(fileNum == NULL) {
		perror("Error opening file");
		return(-1);
	}
	while (!feof(fileNum))  {
		fscanf(fileNum, "%d", &arr[i++]); 
	}
	
	size=i-1;
	printf("\n");
	fclose(fileNum);
	
	/* File Read done in array named arr*/
	
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	freeaddrinfo(servinfo); 
	
	
	printf("The client is up and running.\n");
	
	int j;
	int *p1;
	p1 = &size;
	int opcode;
	int *OP;
	int sum=0;
	int *add;
	opcode=0;
	OP=&opcode;

	/* Creating the functions to be send to 
	servers in order to perform computations i.e
	max, min, sum, sum of squares*/
	
	if(argc==2)
	{
		if (strcmp(argv[1], "sum" )== 0)
		{ 
			*OP=1;
		}
		else if (strcmp(argv[1], "max" )== 0)
		{ 
			*OP=2;
		}
		else if (strcmp(argv[1], "min" )== 0)
		{ 
			*OP=3;
		}
		else if (strcmp(argv[1], "sos" )== 0)
		{ 
			*OP=4;
		}
	}

	printf("The client has sent the reduction type %s to AWS.\n",argv[1]);

	printf("The client has sent %d numbers to AWS \n",size);
	
	/* Sending the operation code to aws server*/

	if (send(sockfd, OP , 4, 0) == -1){
		perror("send");
		exit(1);
	} 

	/* Sending the array size to aws server*/
	
	if (send(sockfd, p1 , 4, 0) == -1){
		perror("send");
		exit(1);
	}

	/* Sending the array elements to aws server*/
	
	if (send(sockfd, arr , 4*size, 0) == -1){
		perror("send");
		exit(1);
	}

	// for(i = 0;i<size;i++)
	//{
	//printf("%d\n",arr[i]);
	// }

	if (recv(sockfd, add , 4, 0) == -1){
		perror("send");
		exit(1);}

	/* Client received the final computation done on data from aws server*/

	if(*OP==1)
	{
		sum= *add;
		printf("The client has received reduction SUM: %d\n",sum);
	}

	if(*OP==2)
	{
		sum= *add;
		printf("The client has received reduction MAX: %d\n",sum);
	}

	if(*OP==3)
	{
		sum= *add;
		printf("The client has received reduction MIN: %d\n",sum);
	}

	if(*OP==4)
	{
		sum= *add;
		printf("The client has received reduction SOS: %d\n",sum);
	}




	close(sockfd); 

	return 0;
}