#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TWORD 50
#define TAMVEC 10000
#define TLIN 1000
#define HORAS 24
#define RLIN 10


int funcion (char arg1, char arg2, FILE *fichero);

int main( int argc, char** argv){

	char arg1,arg2;
 	int arg2i;
	FILE *fichero;
	
	// Se comprueba el numero de argumentos
	if(argc!=3){
		printf("USO analize -a1 nomfich\n");
		exit(0);	
	}
	
	arg1 = argv[1][1];
	arg2 = argv[1][2];
	//printf("arg1 %c/ arg2 %c/ fichero %s\n",arg1,arg2,argv[2]);
	// Se comprueba que los argumento son adecuados
	if(arg1!='a' && arg1!='b' && arg1!='c'){
		printf("El primer argumento tiene que ser a,b,c\n");
		exit(0);
	}
	
	if(arg2!='1' && arg2!='2' && arg2!='3'){
		printf("El segundo argumento puede ser 1,2,3\n");
		exit(0);
	}
	
	
	

	

 	// Se abre el fichero que se pasa como argumento con las opciones dadas
	fichero = fopen( argv[2], "r");
	if(!fichero){
		printf("NO se puede abrir el fichero\n");
		exit(0);	
	}
	
	// Se llama a la funcion principal con las opciones dadas
 	funcion ( arg1, arg2, fichero);
	return 0;
}

int funcion (char arg1, char arg2, FILE *fichero){
	
	int i;
	char* cadena; // Cadena que contiene una linea del fichero

	char** clientes; // Array donde se guardan todos los clientes
	char* cliente;

	
	int bytes;
	long* array; // Amacena numero de bytes o numero de consultas	

	int hora;
	char* auxhora; 

	
	// Reservamos espacio en memoria 	
	cadena = (char*)malloc(TLIN*sizeof(char));
	cliente = (char*)malloc(TWORD*sizeof(char));
	clientes = (char**)malloc(TAMVEC*sizeof(char*));
	
	array = (long*)malloc(TAMVEC*sizeof(long));
	auxhora = (char*)malloc(3*sizeof(char));	

	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)malloc(TWORD*sizeof(char));
	for (i=0; i<TAMVEC; i++) clientes[i] = (char*)NULL;
	
	// Si la opcion es 3 usamos el case d
	if(arg2=='3') arg1= 'd';
	
	
	// Leemos cada linea del fichero y obtenemos los datos que necesitamos para cada caso
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
	
	// Si es el caso de las horas se va a un caso propio
	if(arg1=='c' && (arg2=='1' ||arg2=='2')) arg2='4'; 

	switch(arg2){
		
		//Lista completa
		case '1':	
			ordenaarrays(clientes, array);
			imprimetam(clientes,array,TAMVEC);
			break;
		
		//Los 10 primeros
		case '2':
			ordenaarrays(clientes, array);
			imprimetam(clientes,array,RLIN);
			break;

		// Histograma
		case '3':
			imprimehistograma(clientes,array);
			break;
		
		// Horas y Bytes
		case '4':
			imprimehora(array);
			break;

		default:
			break;	
	}
	
	return 0;
}

// Llena el array de clientes y el array con los bytes o consultas de cada cliente
int procesa (char** clientes, char* cliente, long* array, int num){

	int i=0;
	char* aux;
	aux = (char*)malloc(TWORD*sizeof(char));
	
	// Comprobamos si el cliente esta en el array de clientes
	while(clientes[i]!=NULL){
		if(strcmp(clientes[i],cliente)==0){
			array[i] += (long)num;
			return 0;
		}

		i ++;
	}
	
	//Si no esta en el vector se mete en la primera posicion libre i
	strcpy(aux,cliente);
	clientes[i] = (char*) aux;
	array[i] = (long)num;
	return 0;
}

// Imprime un rango de hora junto con el array pasado
int imprimehora(long* array){

	int i;
	printf("Rango horas \t Bytes\n");
	for( i=0; i<HORAS; i++){
		printf("%02d:00-%02d:59 \t %ld\n",i,i,array[i]);
	}

	return 0;
}

// Intercambiamos un cliente de posicion con otro y sus respectivos bytes o consultas
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

// Ordenamos un array de mayor a menor ordenando tambien los clientes
int ordenaarrays(char** arrayc, long* arrayi){

	int i, j, k, N=0;
	
	// Calculamos el tamaño valido del array
	while(arrayc[N]!=NULL){N++;}

	for (i = 0; i < N - 1; i++){
		for (k = i, j = i + 1; j < N; j++){
			if (arrayi[k] < arrayi[j])k = j;
		}
		if (k != i) intercambiar (arrayc, arrayi, i, k);
	}
	return 0;
}


// Imprime las ntam posiciones del array de clientes y de bytes o consultas
int imprimetam(char** clientes,long* array,int tam){

	int i=0;
	int auxhora; 
	printf("Cliente \t N.Bytes o Consultas \n");
	while(clientes[i]!=NULL && i<tam){
		printf("%s \t %ld \n",clientes[i],array[i]);
		i++;
	}
	return 0;
}

// Imprime los datos pasados en forma de Histograma
// Rango-Hora /Cliente con mas bytes en esa hora /Bytes del cliente
int imprimehistograma(char** clientes,long* array){
	int i,max;
	char** clientes24;
	long* array24;
	
	// Reservamos espacio en memoria
	array24 = (long*)malloc(HORAS*sizeof(long));
	clientes24 = (char**)malloc(HORAS*sizeof(char*));
	
	for (i=0; i<HORAS; i++) clientes24[i] = (char*)malloc(TWORD*sizeof(char));
	
	// Calculamos el cliente con el mayor nbytes para cada hora
	// Y guardamos sus bytes y su nombre en los arrays auxiliares
	for(i=0; i<HORAS; i++){
		max = maximohora(clientes,array,i);
		sscanf(clientes[max],"%s",clientes24[i]);
		array24[i] = array[max];
	}
	
	// Imprimimos el Histograma
	printf("Rango hora \t Cliente \t Tam en Bytes\n");

	for(i=0; i<HORAS; i++){
		printf("%02d:00-%02d:59 \t %s \t %ld \n",i,i,clientes24[i],array24[i]);
	}
	
	return 0;
}

// Calculamos el cliente con el mayor nbytes para cada hora y devolvemos la posicion del maximo
int maximohora (char** clientes, long* array, int hora){
	// Devuelve la posicion del cliente con mas bytes en la hora dada
	int i=0;
	int horaux=0;
	int pos=0;
	long maxb = 0;
	char* maxc = (char*)malloc(TWORD*sizeof(char)); // Reservamos espacio en memoria
	
	while(i<TAMVEC && horaux<=hora && clientes[i]!=NULL){
		//Obtenemos la hora
		sscanf(clientes[i],"%*s %2d",&horaux);
		//Si son clientes distintos y el tamaño es mayor se cambian los maximos
		if(strcmp(clientes[i],maxc)!=0 && array[i] > maxb && horaux==hora){
			strcpy(maxc,clientes[i]);
			maxb = array[i];
			pos = i;
		}
		i++;
	}
	return pos;
}
