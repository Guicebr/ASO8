#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TWORD 50
#define TAMVEC 10000
#define TLIN 1000
#define HORAS 24
#define RLIN 10


int funcion (char arg1, int arg2i, FILE *fichero);

int main( int argc, char** argv){
	/*
	char arg1,arg2;
 	int arg2i;

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
	
	
	
	*/

	FILE *fichero; // Puntero al fichero
 	//Se abre el fichero que se pasa como argumento
	fichero = fopen( argv[1], "r");

 	//funcion ( arg1, arg2i, fichero);
	funcion ('c', 2, fichero);
	return 0;
}

int funcion (char arg1, int arg2i, FILE *fichero){
	
	int i;
	char* cadena;

	char** clientes;
	char* cliente;

	
	int bytes;
	long* array; // Amacena numero de bytes o numero de consultas	

	int hora;
	char* auxhora;
	
	cadena = (char*)malloc(TLIN*sizeof(char));
	cliente = (char*)malloc(TWORD*sizeof(char));
	clientes = (char**)malloc(TAMVEC*sizeof(char*));
	
	array = (long*)malloc(TAMVEC*sizeof(long));
	auxhora = (char*)malloc(3*sizeof(char));	

	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)malloc(TWORD*sizeof(char));
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)NULL;
	
	if(arg2i==3) arg1= 'd';
	
	rewind(fichero);
	while (!feof(fichero)){	
		fgets(cadena,TLIN,fichero);
		switch(arg1){
		
		// Cliente N.Consultas
		case 'a':
			sscanf(cadena, "%s",cliente);
			//printf("Cliente %s\n",cliente);
			procesa(clientes,cliente,array,1);
			
			break;	
		// Cliente N.Bytes
		case 'b':
			sscanf(cadena, "%s - %*s %*s %*s %*s %*s %*s %*d %d",cliente,&bytes);
			//printf("Cliente %s Bytes %d\n",cliente,bytes);
			procesa(clientes,cliente,array,bytes);
			
			break;
		
		// Horas N.Bytes
		case 'c':
			sscanf(cadena, "%*s - %*s %*12c:%2d:%*d:%*d  %*s %*s %*s %*s %*d %d",&hora,&bytes);
			//printf("Hora %d Bytes %d\n",hora,bytes);
			array[hora] += (long)bytes;
			
			break;
		
		//Cliente Hora N.Bytes
		case 'd':
			sscanf(cadena, "%s - %*s %*12c:%2d:%*d:%*d  %*s %*s %*s %*s %*d %d",cliente,&hora,&bytes);
			sprintf(auxhora," %02d",hora);
			strcat(cliente,auxhora);
			//printf("Cliente %s Bytes %d\n",cliente,bytes);
			procesa(clientes,cliente,array,bytes);
			break;
		default:
			break;	
		}
	
	}

	if(arg1=='c' && (arg2i==1 ||arg2i==2)) arg2i=4; 

	switch(arg2i){
		
		//Lista completa
		case 1:	
			imprimetam(clientes,array,TAMVEC);
			ordenaarrays(clientes, array);
			imprimetam(clientes,array,TAMVEC);
			break;
		
		//Los 10 primeros
		case 2:
			ordenaarrays(clientes, array);
			imprimetam(clientes,array,RLIN);
			break;

		// Histograma
		case 3:
			//imprimehistograma(clientes,array,TAMVEC);
			break;
		
		// Horas y Bytes
		case 4:
			imprimehora(array);
			break;

		default:
			break;	
	}
	
	return 0;
}

int procesa (char** clientes, char* cliente, long* array, int num){

	int i=0;
	char* aux;
	aux = (char*)malloc(TWORD*sizeof(char));
	
	
	while(clientes[i]!=NULL){
		if(strcmp(clientes[i],cliente)==0){
			array[i] += (long)num;
			return 0;
		}

		i ++;
	}
	
	//Si no esta en el vector se mete en la primera posicion libre
	strcpy(aux,cliente);
	clientes[i] = (char*) aux;
	array[i] = (long)num;
	return 0;
}


int imprimehora(long* array){

	int i;
	printf("Rango horas \t Bytes\n");
	for( i=0; i<HORAS; i++){
		printf("%02d:00-%02d:59 \t %ld\n",i,i,array[i]);
	}

	return 0;
}

void intercambiar (char** arrayc, long* arrayi,int i, int j){
	long tmpi;
	char* tmpc = (char*)malloc(TWORD*sizeof(char));
	
	tmpi = arrayi[i];
	tmpc = arrayc[i];
	
	arrayi[i] = arrayi[j];
	arrayc[i] = arrayc[j];

	arrayi[j] = tmpi;
	arrayc[j] = tmpc;
}

int ordenaarrays(char** arrayc, long* arrayi){

	int i, j, k, N=0;
	
	while(arrayc[N]!=NULL){N++;}

	for (i = 0; i < N - 1; i++){
		for (k = i, j = i + 1; j < N; j++){
			if (arrayi[k] < arrayi[j])k = j;
		}
		if (k != i) intercambiar (arrayc, arrayi, i, k);
	}
	return 0;
}



int imprimetam(char** clientes,long* array,int tam){

	int i=0;
	printf("Cliente \t N.Bytes o Consultas \n");
	while(clientes[i]!=NULL && i<tam){
		printf("%s \t %ld \n",clientes[i],array[i]);
		i++;
	}
}