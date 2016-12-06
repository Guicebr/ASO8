#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TWORD 50
#define TAMVEC 10000
#define TLIN 1000
	 
int main(int argc, char** argv){
	/*
	char arg1,arg2;
 	int arg2i;

	arg1 = argv[1][1];
	if(argc!=2 | !argv[1][2]){
		printf("USO analize -a1\n");
		exit(0);	
	}
	
	
	arg1 = argv[1][1];
	arg2 = argv[1][2];
	arg2i = atoi(arg2);

	if(arg1!='a' | arg1!='b' | argv1!='c'){
		printf("El primer argumento tiene que ser a,b,c\n");
		exit(0);
	}
	
	if(arg2i!=1 | arg2i!=2 | arg2i!=3){
		printf("El segundo argumento puede ser 1,2,3\n");
		exit(0);
	}
	
	
	switch(arg2){
		case 'a':
			printf("a\n");
			break;
		case 'b':
			printf("b\n");
			break;
		case 'c':
			printf("c\n");
			break;
		deafult:
			break;	
	}
	
	printf("argc %d\t %s %s\n",argc,argv[0],argv[1]);
	printf("salida =%c \n",argv[1][0]);
	*/

	FILE *fichero; // Puntero al fichero
 	//Se abre el fichero que se pasa como argumento
	fichero = fopen(argv[1],"r");
 	//funcionconsultas (300,fichero);
	funcionc_bytes(300,fichero);
	/*
	char* cadena;
	char* cliente;

	int hora,min,sec;
	int nBytes;

	cadena = (char*)malloc(300*sizeof(char));
	cliente = (char*)malloc(30*sizeof(char));
	
	rewind(fichero);
	while (!feof(fichero)){
	fgets(cadena,300,fichero);	
	sscanf(cadena, "%s - - %*13c%2d:%2d:%2d %*s %*s %*s %*s %*d %5d",cliente,&hora,&min,&sec,&nBytes);
	
	printf("cliente %s/ hora %d:%d:%d/ Bytes %d\n",cliente,hora,min,sec,nBytes);
	}
	*/

	return 0;
}

int funcionconsultas ( int nlineas, FILE *fichero){
	
	char* cliente;
	char* cadena;
	char** clientes;
	int* nconsultas;
	int i;

	cadena = (char*)malloc(TLIN*sizeof(char));
	cliente = (char*)malloc(TWORD*sizeof(char));
	clientes = (char**)malloc(TAMVEC*sizeof(char*));	
	nconsultas = (int*)malloc(TAMVEC*sizeof(int));
	
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)malloc(TWORD*sizeof(char));
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)NULL;
	
		
	rewind(fichero);
	while (!feof(fichero)){	
		fgets(cadena,TLIN,fichero);
		sscanf(cadena, "%s",cliente);
		printf("Cliente %s\n",cliente);
		procesaconsultas(clientes,cliente,nconsultas);
	}
	
	// Imprime
	printf("Cliente \t Consultas\n");
	i=0;
	while( clientes[i]!=NULL){
		printf("%s \t %d\n",clientes[i],nconsultas[i]);
		i++;
	}
	return 0;
}

int procesaconsultas(char** clientes, char* cliente, int* nconsultas){
	
	int i=0;
	char* aux;
	aux = (char*)malloc(TWORD*sizeof(char));
	//Si esta en el vector se suma uno
	while(clientes[i]!=NULL){
		if(strcmp(clientes[i],cliente)==0){
			nconsultas[i] += 1;
			printf("Cliente %s \t ConsultasDespues  %d\n",clientes[i],nconsultas[i]);
			return 0;
		}

		i ++;
	}
	//Si no esta en el vector se mete en la primera posicion libre
	strcpy(aux,cliente);
	clientes[i] = (char*) aux;
	nconsultas[i] = 1;
	printf("Metido Cliente %s en la posicion %d Consultas  %d\n",clientes[i],i,nconsultas[i]);
		
	return 0;
}

int funcionc_bytes ( int nlineas, FILE *fichero){
	
	char* cliente;
	int bytes;
	char* cadena;
	char** clientes;
	int* nbytes;
	int i;

	cadena = (char*)malloc(TLIN*sizeof(char));
	cliente = (char*)malloc(TWORD*sizeof(char));
	clientes = (char**)malloc(TAMVEC*sizeof(char*));	
	nbytes = (int*)malloc(TAMVEC*sizeof(int));
	
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)malloc(TWORD*sizeof(char));
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)NULL;
	
		
	rewind(fichero);
	while (!feof(fichero)){	
		fgets(cadena,TLIN,fichero);
		sscanf(cadena, "%s - %*s %*s %*s %*s %*s %*s %*d %d",cliente,&bytes);
		printf("Cliente %s Bytes %d\n",cliente,bytes);
		procesac_bytes(clientes,cliente,nbytes,bytes);
	}
	
	printf("Cliente \t Bytes\n");
	i=0;
	while( clientes[i]!=NULL){
		printf("%s \t %d\n",clientes[i],nbytes[i]);
		i++;
	}
	return 0;
}

int procesac_bytes(char** clientes, char* cliente, int* nbytes, int bytes){

	int i=0;
	char* aux;
	aux = (char*)malloc(TWORD*sizeof(char));
	
	//
	while(clientes[i]!=NULL){
		if(strcmp(clientes[i],cliente)==0){
			nbytes[i] += bytes;
			printf("Cliente %s \t Bytes  %d\n",clientes[i],nbytes[i]);
			return 0;
		}

		i ++;
	}
	//Si no esta en el vector se mete en la primera posicion libre
	strcpy(aux,cliente);
	clientes[i] = (char*) aux;
	nbytes[i] = bytes;
	printf("Metido Cliente %s en la posicion %d Bytes  %d\n",clientes[i],i,nbytes[i]);
		
	return 0;
}
