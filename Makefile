all:
	
	gcc -o serverA1 serverA.c -lsocket -lnsl -lresolv

	gcc -o serverB1 serverB.c -lsocket -lnsl -lresolv

	gcc -o serverC1 serverC.c -lsocket -lnsl -lresolv

	gcc -o aws1 aws.c -lsocket -lnsl -lresolv

	gcc -o client client.c -lsocket -lnsl -lresolv

.PHONY: serverA serverB serverC aws

serverA:
	./serverA1

serverB:
	./serverB1

serverC:
	./serverC1

aws:	
	./aws1