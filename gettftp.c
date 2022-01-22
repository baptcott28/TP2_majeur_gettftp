#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


// fonction pour récupérer un fichier sur le serveur

#define port "1069"
#define BUFFER_SIZE 512																	// pour l'instant, on definit la taille du buffer a 512 octets

int main(int argc,char **argv){

	char error_cmd[]="erreur commande\n";
	char getaddrinfo_echoue[]="getaddrinfo a échoué\n";
	char buffer_receive[BUFFER_SIZE];
	int file_descriptor;
	// argv[1] = adresse du serveur
	// argv[2] = fichier a transmettre


	//on verifie qu'il y a le bon nombre d'arguments prévu dans la commande
	if(argc != 3){
		write(STDOUT_FILENO,error_cmd,strlen(error_cmd));
		exit(EXIT_FAILURE);
	}

	//infos sur le serveur
	struct addrinfo hints={0,0,0,0,0,0,0,0};
	struct addrinfo *res;
	int sock;
	int nb_caractere_emis;


	// On se règle sur un echange via protocole UDP
	hints.ai_family=AF_INET;
	hints.ai_protocol=IPPROTO_UDP;
	hints.ai_socktype=SOCK_DGRAM;

	if((getaddrinfo(argv[1],port,&hints,&res))==0){										// recupération des infos sur le serveur
		printf("getaddrinfo a reussi\n");
	} else {
		write(STDOUT_FILENO,getaddrinfo_echoue,strlen(getaddrinfo_echoue));
		exit(EXIT_FAILURE);
	}	


	// une fois qu'on a toutes les infos du serveur, on reserve le socket
	if((sock=socket(hints.ai_family,hints.ai_socktype,0))==-1){							// tentative d'ouverture du socket avec retour d'erreur si c'est pas ouvert
		perror("erreur_socket\n");
		exit(EXIT_FAILURE);
	} else{
		printf("socket ouvert\n");
	}


	//Fabrication de la Read Request (0 1 lucas.txt 0 octet 0)
	char *RRQ;
	RRQ=malloc(2+strlen(argv[2]+1+strlen("octet")+1));
	RRQ[0]=0;
	RRQ[1]=1;
	strcpy(RRQ+2,argv[2]);
	RRQ[2+strlen(argv[2])]=0;
	strcpy(RRQ+3+strlen(argv[2]),"octet");
	RRQ[3+strlen(argv[2])+strlen("octet")]=0;


	//test de la RRQ
	printf("adresse serveur : '%s'\n",argv[1]);
	printf("Read Reaquest : '%d'",RRQ[0]);
	printf("'%d'",RRQ[1]);
	printf("'%c'",RRQ[2]);
	printf("'%c'",RRQ[3]);
	printf("'%c'",RRQ[4]);
	printf("'%c'",RRQ[5]);
	printf("'%c'",RRQ[6]);
	printf("'%c'",RRQ[7]);
	printf("'%c'",RRQ[8]);
	printf("'%c'",RRQ[9]);
	printf("'%c'",RRQ[10]);
	printf("'%c'",RRQ[11]);
	printf("'%c'",RRQ[12]);
	printf("'%d'",RRQ[13]);
	printf("'%c'",RRQ[14]);
	printf("'%c'",RRQ[15]);
	printf("'%c'",RRQ[16]);
	printf("'%c'",RRQ[17]);
	printf("'%c'",RRQ[18]);
	printf("'%d'\n",RRQ[19]);


	// Envoie de RRQ
	if((nb_caractere_emis=sendto(sock,RRQ, strlen(RRQ),0,hints.ai_addr, hints.ai_addrlen))==-1){ 				// Test d'envoi avec retour d'erreur
		perror("erreur envoie RRQ ");
		exit(EXIT_FAILURE);
	}
	printf("nombre de caractere emis : %d\n",nb_caractere_emis);

	
	//reception des données des données demandé dans buffer_receive
	if(recvfrom(sock,buffer_receive,strlen(buffer_receive),0,hints.ai_addr,hints.ai_addrlen)==-1){				// Test de reception avec retour d'erreur
		perror("erreur reception ");			// erreure de compilation avec hints.ai-addrlen car ce n'est pas un socklen restrict ce que je ne comprend pas
		exit(EXIT_FAILURE);
	}


	//on crée le fichier récupéré et on y écrit le contenu de buffer_receive
	if((file_descriptor=open(argv[2],O_CREAT|O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO))==-1){								// Test d'ouverture du fichier avec retour d'erreur
		perror("erreur ouverture/creation fichier");
		exit(EXIT_FAILURE);
	}else{
		write(file_descriptor,buffer_receive,strlen(buffer_receive));											// si le fichier est ouvert, on y ecrit le contenu de buffer_receive
		exit(EXIT_SUCCESS);	
	}
		
}