/*
 **  Listener socket i.e datagram socket code taken from beej guide
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MYPORT "22841" // the port users will be connecting to

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd;

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;

	int rv, j, size, sizeActual, opcode;
	int sum=0, min=0, sumofsquares=0, max =0;
	int *adder, *maximum, *minimum, *sos, *array, *arrRecieved;
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	while(1)
	{
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP


	if ((rv = getaddrinfo("nunki.usc.edu", MYPORT, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) 
		{
			perror("listener: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
		{
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}
	if (p == NULL) 
	{
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);
	
	printf("The Server B is up and running using UDP on port %s\n", MYPORT);
	addr_len = sizeof their_addr;

	int *sizeReceived;
	
	/* Operation code or Function code received by Backend Server B from aws server*/
	
		if ((recvfrom(sockfd, sizeReceived, 4 , 0,
				(struct sockaddr *)&their_addr, &addr_len)) == -1) 
		{
			perror("recvfrom");
			exit(1);
		}
		opcode=*sizeReceived;
		
		/* Number of elements received by Backend Server B from aws server*/

		if ((recvfrom(sockfd, sizeReceived, 4 , 0,
				(struct sockaddr *)&their_addr, &addr_len)) == -1) 
		{
			perror("recvfrom");
			exit(1);
		}
		printf("The Server B has received %d numbers\n",sizeReceived[0]);

		sizeActual = *sizeReceived;
		size = 4*(*sizeReceived);
		
		arrRecieved=(int *) calloc(sizeof(int),(size));
		array=(int *) calloc(sizeof(int),(size));
		
		/* Elements received by Backend Server B from aws server*/

		if ((recvfrom(sockfd, arrRecieved, size , 0,
				(struct sockaddr *)&their_addr, &addr_len)) == -1) 
		{
			perror("recvfrom");
			exit(1);
		} 
		for (j = 0 ; j < sizeActual ; j++) {
			array[j] = ntohl(arrRecieved[j]);}

	/* If operation code is sum then find the sum of elements and send to aws*/
	
		if(opcode==1)
		{
		sum = 0;
		
			for (j = 0 ; j < sizeActual ; j++)
			{
				sum = sum + array[j];
			}
			adder = &sum;

			printf("The Server B has successfully finished the reduction SUM: %d\n",sum);
			if ((sendto(sockfd, adder, 4 , 0,
					(struct sockaddr *)&their_addr, addr_len)) == -1) {
				perror("sendto");
				exit(1);} 
		printf("The Server B has successfully finished sending the reduction value to AWS server.\n");

		}
		
		/* If operation code is max then find the max of elements and send to aws*/

		if(opcode==2)
		{
			max = array[0];
			for (j = 0 ; j < sizeActual ; j++)
			{
				if(array[j]>max)
					max = array[j];
			}

			maximum = &max;
			printf("The Server B has successfully finished the reduction MAX: %d\n",max);
			if ((sendto(sockfd, maximum, 4 , 0,
					(struct sockaddr *)&their_addr, addr_len)) == -1) {
				perror("sendto");
				exit(1);} 
			printf("The Server B has successfully finished sending the reduction value to AWS server.\n");
		}
		
		/* If operation code is min then find the min of elements and send to aws*/

		if(opcode==3)
		{
			min = array[0];
			for (j = 1 ; j < sizeActual ; j++)
			{
				if(array[j]<min)
					min = array[j];
			}

			minimum = &min;
			printf("The Server B has successfully finished the reduction MIN: %d\n",min);
			if ((sendto(sockfd, minimum, 4 , 0,
					(struct sockaddr *)&their_addr, addr_len)) == -1) {
				perror("sendto");
				exit(1);} 
			printf("The Server B has successfully finished sending the reduction value to AWS server.\n");
		}

		/* If operation code is sos then find the sos of elements and send to aws*/
		
		if(opcode==4)
		{
		sumofsquares=0;
		
			for (j = 0 ; j < sizeActual ; j++)
			{
				sumofsquares = sumofsquares + (array[j] * array[j]);
			}
			printf("The Server B has successfully finished the reduction SOS: %d\n",sumofsquares);
			sos = &sumofsquares;
			if ((sendto(sockfd, sos, 4 , 0,
					(struct sockaddr *)&their_addr, addr_len)) == -1) {
				perror("sendto");
				exit(1);} 
			printf("The Server B has successfully finished sending the reduction value to AWS server.\n");
		}


		close(sockfd);
	}//end of while
	return 0;
}