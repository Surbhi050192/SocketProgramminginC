/*
 *** A stream socket and UDP Datagram socket taken from Beej Guide
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

//All the port required numbers

#define PORT "25841" // the TCP port client will is connected to
#define SERVERPORTA "21841"//the port UDP server A will be connecting to
#define SERVERPORTB "22841"//the port UDP server B will be connecting to
#define SERVERPORTC "23841"//the port UDP server C will be connecting to

#define BACKLOG 10
void sigchld_handler(int s)
{
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int functionServerA(int *Arr,int arrSize,int Opcode)
{
	int rv, sockfd, sum=0;
	int *add, *operationCode;
	operationCode=&Opcode;

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;


	if ((rv = getaddrinfo("nunki.usc.edu", SERVERPORTA, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1; 
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	} 
	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	/*Already passed ArraySize in function represents 
	total number sent to Backend-Server A*/

	printf("The AWS has sent %d numbers to Backend-Server A \n",arrSize);

	int *p1 = NULL;
	p1=&arrSize;

	/* Sending the operation code for sum/max/min/sos */

	if ((sendto(sockfd, operationCode, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}


	if ((sendto(sockfd, p1, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	/* Sending the array elements with array size */
	if ((sendto(sockfd, Arr, 4*arrSize, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	} 
	add=&sum;

	/* If operation code is sum then receive the result from Backend Server A */

	if(Opcode==1)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		}

	}

	/* If operation code is max then receive the result from Backend Server A */
	if(Opcode==2)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}
	/* If operation code is min then receive the result from Backend Server A */
	if(Opcode==3)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}
	/* If operation code is sum of squares then receive the result from Backend Server A */
	if(Opcode==4)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}

	freeaddrinfo(servinfo);
	close(sockfd);
	return sum;

}//end of functionServerA

int functionServerB(int *Arr,int arrSize,int Opcode)
{
	int rv, sockfd, sum=0;
	int *add, *operationCode;
	operationCode=&Opcode;

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;


	if ((rv = getaddrinfo("nunki.usc.edu", SERVERPORTB, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1; 
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	} 
	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}
	/*Already passed ArraySize in function represents 
	total number sent to Backend-Server B*/
	
	printf("The AWS has sent %d numbers to Backend-Server B \n",arrSize);

	int *p1;
	p1=&arrSize;
	
	/* Sending the operation code for sum/max/min/sos */

	if ((sendto(sockfd, operationCode, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}


	if ((sendto(sockfd, p1, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	/* Sending the array elements with array size */
	
	if ((sendto(sockfd, Arr, 4*arrSize, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	} 
	add=&sum;

	/* If operation code is sum then receive the result from Backend Server B */
	if(Opcode==1)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}
	
	/* If operation code is max then receive the result from Backend Server B */
	
	if(Opcode==2)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}
	/* If operation code is min then receive the result from Backend Server B */
	
	if(Opcode==3)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		}  

	}
	/* If operation code is sum of squares then receive the result from Backend Server B */
	
	if(Opcode==4)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}

	freeaddrinfo(servinfo);
	close(sockfd);

	return sum;
}//end of functionServerB


int functionServerC(int *Arr,int arrSize,int Opcode)
{
	int rv, sockfd, sum=0;
	int *add, *operationCode;
	operationCode=&Opcode;

	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;


	if ((rv = getaddrinfo("nunki.usc.edu", SERVERPORTC, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1; 
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	} 
	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}
	
	/*Already passed ArraySize in function represents 
	total number sent to Backend-Server C*/
	
	printf("The AWS has sent %d numbers to Backend-Server C \n",arrSize);

	int *p1;
	p1=&arrSize;
	
	/* Sending the operation code for sum/max/min/sos */

	if ((sendto(sockfd, operationCode, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}


	if ((sendto(sockfd, p1, 4, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

	/* Sending the operation code for sum/max/min/sos */
	
	if ((sendto(sockfd, Arr, 4*arrSize, 0,
			p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	} 
	add=&sum;

	/* If operation code is sum then receive the result from Backend Server C */
	if(Opcode==1)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}

	/* If operation code is max then receive the result from Backend Server C */
	
	if(Opcode==2)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		}

	}
	/* If operation code is min then receive the result from Backend Server C */
	
	if(Opcode==3)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		}  

	}
	
	/* If operation code is sum of squares then receive the result from Backend Server C */

	if(Opcode==4)
	{
		if ((recvfrom(sockfd, add, 4, 0,
				p->ai_addr, &(p->ai_addrlen))) == -1) {
			perror("talker: sendto");
			exit(1);
		} 

	}

	freeaddrinfo(servinfo);
	close(sockfd);
	return sum;
	
}//end of functionServerC



int main(void)
{
	int i, j, rv, funcSize, sockfd, new_fd, sizeActual, operationCode, size;
	int sum, max=0, min=0;
	int *add, *arrRecieved, *CompleteArray, *arrServerA, *arrServerB, *arrServerC;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	int yes=1;
	int k,l,m;
	socklen_t sin_size;
	struct sigaction sa;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP


	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	printf("The AWS is up and running.%s\n");
	
	for(p = servinfo; p != NULL; p = p->ai_next) 
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);


	if (p == NULL) 
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}
	sa.sa_handler = sigchld_handler; 
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	while(1)
	{ 	sum=k=l=m=0;
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) 
		{
			perror("accept");
			continue;
		}
		inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *)&their_addr),
				s, sizeof s);

		/* Receiving the type of operation code sent by client*/

		int *sizeReceived;

		if ((recv(new_fd, sizeReceived, 4, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		//printf("%d\n", sizeReceived[0]);
		operationCode=*sizeReceived;

		/* Receiving the size of array sent by client*/
	
		if ((recv(new_fd, sizeReceived, 4, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		
		printf("The AWS has received %d numbers from the client using TCP over port %s\n",sizeReceived[0],PORT);
		
		sizeActual = *sizeReceived;
		size = 4*(*sizeReceived);
		
		arrRecieved=(int *) calloc(sizeof(int),(size));
		CompleteArray=(int *) calloc(sizeof(int),(size));

		/* Receiving the array elements sent by client*/
		
		if ((recv(new_fd, arrRecieved, size, 0)) == -1) {
			perror("recv");
			exit(1);
		}

		for (j = 0 ; j < sizeActual ; j++) {
			CompleteArray[j] = ntohl(arrRecieved[j]);}

		//for (j = 0 ; j < sizeActual ; j++) {
		//printf("%d\n",CompleteArray[j]);}


		/* Creating methods to create UDP connection 
		from aws to Backend Server A, B, C and sending 
		and receiving data between aws and backend servers
		Receiving data from servers and performs computation on 
		data send by client i.e min/max/sum/sos 				
		*/
		
		
		funcSize = sizeActual/3;
		
		arrServerA=(int *) calloc(sizeof(int),(sizeActual/3));
		arrServerB=(int *) calloc(sizeof(int),(sizeActual/3));
		arrServerC=(int *) calloc(sizeof(int),(sizeActual/3));

		for(j=0;j<(sizeActual/3);j++)
		{
			arrServerA[j] = CompleteArray[j];
		} 

		for(j=0;j<(sizeActual/3);j++)
		{
			arrServerB[j] = CompleteArray[j+funcSize];
		}

		for(j=0;j<(sizeActual/3);j++)
		{
			arrServerC[j] = CompleteArray[j+funcSize+funcSize];
		}

		k = functionServerA(arrServerA,(sizeActual/3),operationCode);
		l = functionServerB(arrServerB,(sizeActual/3),operationCode);
		m = functionServerC(arrServerC,(sizeActual/3),operationCode);

		if(operationCode==1)
		{
			printf("The AWS received reduction result of SUM from Backend-Server A using UDP over port %s and it is %d\n",SERVERPORTA,k);
			printf("The AWS received reduction result of SUM from Backend-Server B using UDP over port %s and it is %d\n",SERVERPORTB,l);
			printf("The AWS received reduction result of SUM from Backend-Server C using UDP over port %s and it is %d\n",SERVERPORTC,m);
			sum = k + l + m;
			printf("The AWS has successfully finished the reduction SUM: %d\n",sum);
			add = &sum;
			if ((send(new_fd, add, 4, 0)) == -1) 
			{
				perror("send");
				exit(1);
			}
			printf("AWS has successfully finished sending the reduction value to client. \n");

		}

		if(operationCode==2)
		{
			printf("The AWS received reduction result of MAX from Backend-Server A using UDP over port %s and it is %d\n",SERVERPORTA,k);
			printf("The AWS received reduction result of MAX from Backend-Server B using UDP over port %s and it is %d\n",SERVERPORTB,l);
			printf("The AWS received reduction result of MAX from Backend-Server C using UDP over port %s and it is %d\n",SERVERPORTC,m);
			if(k>l && k>m)
				max = k;
			else if(l>k && l>m)
				max = l;
			else
				max = m;
			printf("The AWS has successfully finished the reduction MAX: %d\n",max);
			add = &max;
			if ((send(new_fd, add, 4, 0)) == -1) 
			{
				perror("send");
				exit(1);
			}
			printf("AWS has successfully finished sending the reduction value to client. \n");
		}

		if(operationCode==3)
		{
			printf("The AWS received reduction result of MIN from Backend-Server A using UDP over port %s and it is %d\n",SERVERPORTA,k);
			printf("The AWS received reduction result of MIN from Backend-Server B using UDP over port %s and it is %d\n",SERVERPORTB,l);
			printf("The AWS received reduction result of MIN from Backend-Server C using UDP over port %s and it is %d\n",SERVERPORTC,m);
			if(k<l && k<m)
				min = k;
			else if(l<k && l<m)
				min = l;
			else
				min = m;
			printf("The AWS has successfully finished the reduction MIN: %d\n",min);
			add = &min;

			if ((send(new_fd, add, 4, 0)) == -1) 
			{
				perror("send");
				exit(1);
			}
			printf("AWS has successfully finished sending the reduction value to client. \n");
		}

		if(operationCode==4)
		{
		
			printf("The AWS received reduction result of SOS from Backend-Server A using UDP over port %s and it is %d\n",SERVERPORTA,k);
			printf("The AWS received reduction result of SOS from Backend-Server B using UDP over port %s and it is %d\n",SERVERPORTB,l);
			printf("The AWS received reduction result of SOS from Backend-Server C using UDP over port %s and it is %d\n",SERVERPORTC,m);
			sum = k + l + m;
			printf("The AWS has successfully finished the reduction SOS: %d\n",sum);
			add = &sum;
			if ((send(new_fd, add, 4, 0)) == -1) 

			{
				perror("send");
				exit(1);
			}
			printf("AWS has successfully finished sending the reduction value to client. \n");
		}

		close(new_fd); // parent doesn't need this


	}
	return 0;
}