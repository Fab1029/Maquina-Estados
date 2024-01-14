#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO 10
#define ALFABETO 100

//Estructura necesaria para realizar la lista de los estados
//del automata.
typedef struct Q{
	char nombre_estado[3];
	struct Q *ptr_siguiente;
} Q;

//Estructura necesaria para lista de estados finales del automata
typedef struct F{
	Q *ptr_estado;
	struct F *ptr_siguiente;
} F;

//Estructura necesaria para pila de las evaluaciones del automata
typedef struct EVALUACION{
	Q *ptr_estado;
	char *caracteres;
	struct EVALUACION *ptr_siguiente;
	
} Evaluacion;

typedef struct AUTOMATA{
	
	Q ***D;//Matriz que guarda funcion de transicion de estados del automata
	Q *ptr_estados;//puntero a lista de estados ingresados
	char E[ALFABETO];
	Q *ptr_estado_inicial;//puntero a un estado
	F *ptr_estados_finales;//puntero a lista de estados finales
	char NombreAutomata[3];//nombre del automata
	
} Automata;

Q *crear_estado(Q *ptr_nuevo);//crea estado para lista Q-estados del automata
F *crear_estado_final(F *ptr_nuevo);//crea estado para realizar lista de estados finales 
Q *lista_estados(Q *ptr_head,Q *ptr_nuevo);
void ingreso_alfabeto(Automata *vector_automatas);
Q *eliminar_estado(Q *ptr_inicio,Q *ptr_busqueda);
Evaluacion *crear_evaluacion(Evaluacion *ptr_nuevo);//crea pila de evaluaciones
void proceso_evaluacion(Automata *vector_automatas);
void liberar_matriz_dinamica(Automata *vector_automatas);
Evaluacion *Traza(Evaluacion *ptr_cima,Evaluacion *ptr_nuevo);//proceso de apilar pila de evaluaciones
void validar_nombre_estado_automata(Q *ptr_head,Q *ptr_nuevo);
void liberar_memoria(Q *lista_estados,F *lista_estados_finales);
void validar_opcion(char *mensaje_inicio,char *numero,char rango);
F *lista_estados_finales(F *ptr_head,F *ptr_nuevo,Q *ptr_busqueda);
Evaluacion *pop_traza(Evaluacion *ptr_cima,short int filas_cadena);//liberar pila de evaluaciones
F *eliminar_estado_final(Automata *vector_automatas,Q *ptr_busqueda);
Q *busqueda(char *mensaje_inicio,Automata *vector_automatas,char opcion);//funcion valida para busqueda en lista de estados
void listar(char *mensaje_inicio,Automata *vector_automatas,char opcion);
void menu_definir_AFD(Automata *vector_automatas,short int contador_AFD);
void validar_nombre_automata(Automata *vector_automatas,short int contador_AFD);
void conteo_filas_columnas(Automata *vector_automata,short int *tamano,char opcion);//funcion para determinar tamano lista y tamano alfabto
void localizar_fila(Automata *vector_automatas, Q *ptr_busqueda,short int *fila_matriz);
void seleccion_automata(Automata *vector_automatas,short int *contador_AFD,char opcion);
void separar_cadena(Automata *vector_automatas,char lenguaje[][TAMANO],short int columnas);
void mostrar_tabla_transicion(Automata *vector_automatas,short int filas,short int columnas);
void ingreso_tabla_transicion(Automata *vector_automatas,short int filas,short int columnas);
void modificar_tabla_transicion(Automata *vector_automatas,short int filas,short int columnas);
void CRUD(char *mensaje_inicio,Automata *vector_automatas,char opcion_apoyo,char numero_opciones);
void validar_cadena_alfabeto(char *cadena_validacion,char cadena[][TAMANO],short int *filas_cadena);
void localizar_columna(char alfabeto[][TAMANO],char *busqueda,short int columnas,short int *columna_matriz);

int main(){

	char opcion;
	short int contador_AFD=0;
	Automata vector_automatas[3]={0};
	
	do{
		system("cls");
		validar_opcion("=================\n1.DEFINIR UN AFD\n2.EVALUAR CADENA\n3.SALIR\n=================\n",&opcion,'3');
		system("cls");
		
		switch(opcion){
			case '1'://opcion ingreso de datos del automata finito determinista
				seleccion_automata(vector_automatas,&contador_AFD,'1');
			break;
			case '2'://opcion para el ingreso para validacion de la cadena
				seleccion_automata(vector_automatas,&contador_AFD,'2');
			break;
		}
		
	}while(opcion != '3');
	
	//prcoeso de liberacion de memoria	
	for(int i=0; i<3; i++){
		
		liberar_matriz_dinamica(vector_automatas+i);
		liberar_memoria((vector_automatas+i)->ptr_estados,(vector_automatas+i)->ptr_estados_finales);
		
	}
		
}

Q *crear_estado(Q *ptr_nuevo){
	
	ptr_nuevo=malloc(sizeof(Q));
	
	if( ptr_nuevo == NULL){
		printf("PROBLEMAS CON MEMORIA\n");
		exit(1);
	}
	else{
		ptr_nuevo->ptr_siguiente=NULL;
		return ptr_nuevo; 	
	}	
}

F *crear_estado_final(F *ptr_nuevo){
	ptr_nuevo=malloc(sizeof(F));
	
	if( ptr_nuevo == NULL){
		printf("PROBLEMAS CON MEMORIA\n");
		exit(1);
	}
	else{
		ptr_nuevo->ptr_siguiente=NULL;
		return ptr_nuevo; 	
	}	
}

Q *lista_estados(Q *ptr_head,Q *ptr_nuevo){
	
	if(ptr_head != NULL)
		ptr_nuevo->ptr_siguiente=ptr_head;	
	
	ptr_head=ptr_nuevo;
	
	return ptr_head;
}

//Funcion para borrar estado de lista teniendo el inicio de la lista 
//y el puntero al estado de busqueda
Q *eliminar_estado(Q *ptr_inicio,Q *ptr_busqueda){
	Q *ptr_auxiliar=ptr_inicio;
	Q *ptr_auxiliar_anterior=NULL;
	
	//proceso borrar de lista
	while(ptr_auxiliar != NULL){
		
		if(ptr_auxiliar == ptr_busqueda){
			
			if(ptr_inicio == ptr_busqueda)//primer estado de lista a eliminar
				ptr_inicio=ptr_auxiliar->ptr_siguiente;
				
			else if(ptr_busqueda->ptr_siguiente == NULL)//ultimo estado a eliminar
				ptr_auxiliar_anterior->ptr_siguiente=NULL;
				
			else//cualquier estado a eliminar
				ptr_auxiliar_anterior->ptr_siguiente=ptr_auxiliar->ptr_siguiente;
			
			free(ptr_auxiliar);
			printf("ESTADO ELIMINADO\n");
			system("pause");
			return ptr_inicio; 
		}
	
		ptr_auxiliar_anterior=ptr_auxiliar;
		ptr_auxiliar=ptr_auxiliar->ptr_siguiente;	
		
	}
	return ptr_inicio;
}

void ingreso_alfabeto(Automata *vector_automatas){
	char contador='0';
	
	while(contador == '0'){
		fflush(stdin);
		printf("INTRODUZCA EL ALFABETO SEPARANDO POR ESPACIOS LOS CARACTERES O CONJUNTO DE  LOS MISMOS:");
		gets(vector_automatas->E);
		//la cadena ingresa debe ser mayor a cero como primera instancia
		if( strlen(vector_automatas->E) > 0 ){
			contador='1';
			short int i=0;
			short int contador_letras=0;
			//Hasta completar el tamano de la cadena ingresada
			while(i != strlen(vector_automatas->E)){
				
				if(*((vector_automatas->E)+i) == ' '){
					//se ingreso la misma subcadena mas de una vez
					if(contador_letras == i){
						printf("REPETICION DE LENGUAJES\n");
						contador='0';
						break;
					}
					else
						contador_letras=0;
				}
				//proceso de contador al identificar caracteres iguales
				for(short int j=1+i; j<strlen(vector_automatas->E); j++){
					
					if(*((vector_automatas->E)+i) == *((vector_automatas->E)+j)){
						contador_letras+=1;
						break;
					}
				}
				
				i+=1;
			}
		}
		else
			printf("ALFABETO INCORRECTO\n");
	}

}

Evaluacion *crear_evaluacion(Evaluacion *ptr_nuevo){
	ptr_nuevo=malloc(sizeof(Evaluacion));
	
	if( ptr_nuevo == NULL){
		printf("PROBLEMAS CON MEMORIA\n");
		exit(1);
	}
	else{
		ptr_nuevo->ptr_siguiente=NULL;
		return ptr_nuevo; 	
	}
}

void proceso_evaluacion(Automata *vector_automatas){
	short int filas=0;
	char verificador='0';
	short int columnas=0;
	short int fila_matriz=0;
	short int filas_cadena=0;
	short int columna_matriz=0;
	Evaluacion *ptr_cima=NULL;
	F *ptr_auxiliar=vector_automatas->ptr_estados_finales;
	
	conteo_filas_columnas(vector_automatas,&filas,0);//conteo de filas
	conteo_filas_columnas(vector_automatas,&columnas,1);//conteto de columnas
	char alfabeto[columnas][TAMANO];//alfabeto de la maquina
	char cadena[TAMANO][TAMANO];//la cadena la separamos acorde al alfabeto
	
	//obtenmos el alfabeto en modo matriz tal que: alfabteto[][]=ab\n1
	separar_cadena(vector_automatas,alfabeto,columnas);
	//se valida la cadena que se ingresa por el usuario
	validar_cadena_alfabeto(vector_automatas->E,cadena,&filas_cadena);
	
	//se realiza el proceso iterativo para crear pila evaluacion sabiendo el tamano de la lista
	for(short int i=0; i<filas_cadena; i++){
		Evaluacion *ptr_nuevo=NULL;
		
		ptr_nuevo=crear_evaluacion(ptr_nuevo);
		
		ptr_cima=Traza(ptr_cima,ptr_nuevo);
		
		//primera iteracion la evaluacion debe empezar desde el estado inicial 
		if(i == 0)
			localizar_fila(vector_automatas,vector_automatas->ptr_estado_inicial,&fila_matriz);
		else//despues localiza la fila en la matriz funcion transicion
			localizar_fila(vector_automatas,vector_automatas->D[fila_matriz][columna_matriz],&fila_matriz);
		
		//localiza columna utilizando alfabeto y caracteres cadena ingresada
		localizar_columna(alfabeto,cadena[i],columnas,&columna_matriz);
		//en estructura evaluacion se modifcan valores
		ptr_cima->ptr_estado=vector_automatas->D[fila_matriz][columna_matriz];
		ptr_cima->caracteres=alfabeto[columna_matriz];	

	}
	//VERIFICA SI ES ACEPTADA LA CADENA
	while(ptr_auxiliar != NULL){
		
		if(ptr_auxiliar->ptr_estado == ptr_cima->ptr_estado){
			printf("===================================\nCADENA FUE ACEPTADA POR EL AUTOMATA\n===================================\n");
			verificador='1';
			break;
		}
		
		ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
			
	}
	//NO ES ACEPTADA LA CADENA
	if(verificador == '0')
		printf("======================================\nCADENA NO FUE ACEPTADA POR EL AUTOMATA\n======================================\n");
	//Deque pila
	ptr_cima=pop_traza(ptr_cima,filas_cadena);
	
	system("pause");
}

void liberar_matriz_dinamica(Automata *vector_automatas){
	//matrices de transicion se liberar si no estan apuntado a NULL
	if(vector_automatas->D != NULL){
		short int filas;
		
		conteo_filas_columnas(vector_automatas,&filas,'0');
		for(short int i=0; i<filas; i++){
			//liberar filas
			free(vector_automatas->D[i]);
		}
		//liberar columnas
		free(vector_automatas->D);	
	}
	
}

void validar_nombre_estado_automata(Q *ptr_head,Q *ptr_nuevo){
	char contador='0';
	Q *ptr_auxiliar=ptr_head;
	
	while(contador == '0'){
		
		fflush(stdin);
		printf("INGRESE NOMBRE DE ESTADO:");
		scanf("%s",ptr_nuevo->nombre_estado);
		//primera instancia el nombre del estado debe ser maximo 2 caracteres
		if( strlen(ptr_nuevo->nombre_estado) < 3){
			contador='1';
			ptr_auxiliar=ptr_head;
			//se verifica en lista si ya se ingreso el estado
			while(ptr_auxiliar != NULL){
					
				if(ptr_auxiliar != ptr_nuevo && strcmp(ptr_nuevo->nombre_estado,ptr_auxiliar->nombre_estado) == 0){
					printf("NOMBRE DE ESTADO NO VALIDO\n");
					contador='0';
					break;
				}
						
				ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
			}
				
		}
		else
			printf("NOMBRE DE ESTADO NO VALIDO\n");
	}	
}

Evaluacion *Traza(Evaluacion *ptr_cima,Evaluacion *ptr_nuevo){
	
	if(ptr_cima != NULL)
		ptr_nuevo->ptr_siguiente=ptr_cima;	
	
	ptr_cima=ptr_nuevo;
	
	return ptr_cima;
}

void liberar_memoria(Q *lista_estados,F *lista_estados_finales){
	Q *ptr_auxiliar_estados=lista_estados;
	F *ptr_auxiliar_finales=lista_estados_finales;
	//liberar lista de estados
	while(ptr_auxiliar_estados != NULL){
				
		lista_estados=lista_estados->ptr_siguiente;	
		free(ptr_auxiliar_estados);
		ptr_auxiliar_estados=lista_estados;
				
	}
	//liberar lista de estados finales	
	while(ptr_auxiliar_finales != NULL){
				
		lista_estados_finales=lista_estados_finales->ptr_siguiente;	
		free(ptr_auxiliar_finales);
		ptr_auxiliar_finales=lista_estados_finales;
				
	}
	
}

void validar_opcion(char *mensaje_inicio,char *opcion,char rango){
	char contador='0';
	
	while(contador == '0'){
		fflush(stdin);
		printf(mensaje_inicio);
		scanf("%c",opcion);
		if( *opcion > '0' && *opcion <= rango)
			contador='1';
		else
			printf("OPCION INCORRECTA\n");
	}
}

F *lista_estados_finales(F *ptr_head,F *ptr_nuevo,Q *ptr_busqueda){
	
	if(ptr_head != NULL)
		ptr_nuevo->ptr_siguiente=ptr_head;
	
	ptr_nuevo->ptr_estado=ptr_busqueda;
	ptr_head=ptr_nuevo;
	
	return ptr_head;
}

Evaluacion *pop_traza(Evaluacion *ptr_cima,short int filas_cadena){
	short int contador=filas_cadena;
	Evaluacion *ptr_auxiliar=ptr_cima;
	
	printf("\tEVALUACIONES REALIZADAS\n===================================\n");
	while(ptr_cima != NULL){
		
		ptr_auxiliar=ptr_cima;
		printf("EVALUACION %d\nESTADO:%s\tCARACTER:%s\n",contador,ptr_cima->ptr_estado->nombre_estado,ptr_cima->caracteres);
		ptr_cima=ptr_cima->ptr_siguiente;
		free(ptr_auxiliar);
		contador-=1;
	}
	ptr_cima=NULL;

	return ptr_cima;
}

F *eliminar_estado_final(Automata *vector_automatas,Q *ptr_busqueda){
	F *ptr_auxiliar=vector_automatas->ptr_estados_finales;
	F *ptr_auxiliar_anterior=NULL;
	
	while(ptr_auxiliar != NULL){
		
		if(ptr_auxiliar->ptr_estado == ptr_busqueda){
			
			if(vector_automatas->ptr_estados_finales->ptr_estado == ptr_busqueda)//primer estado de lista a eliminar
				vector_automatas->ptr_estados_finales=ptr_auxiliar->ptr_siguiente;
			else if(ptr_auxiliar->ptr_siguiente == NULL)//ultimo estado a eliminar
				ptr_auxiliar_anterior->ptr_siguiente=NULL;
			else//estado intermedio a eliminar
				ptr_auxiliar_anterior->ptr_siguiente=ptr_auxiliar->ptr_siguiente;
			
			free(ptr_auxiliar);
			printf("ESTADO ELIMINADO\n");
			system("pause");
			return vector_automatas->ptr_estados_finales; 
		}
	
		ptr_auxiliar_anterior=ptr_auxiliar;
		ptr_auxiliar=ptr_auxiliar->ptr_siguiente;	
	}
	
	return vector_automatas->ptr_estados_finales;
}

Q *busqueda(char *mensaje_inicio,Automata *vector_automatas,char opcion){
	char nombre_estado[3];
	
	fflush(stdin);
	printf(mensaje_inicio);
	scanf("%s",nombre_estado);
	
	//busqueda lista estados
	if(opcion == '0'){
		
		Q *ptr_auxiliar = vector_automatas->ptr_estados;
		while (ptr_auxiliar != NULL){
						
			if (strcmp(ptr_auxiliar->nombre_estado, nombre_estado) == 0){
			
				printf("ENCONTRADO\n");
				system("pause");
				return ptr_auxiliar;//retorno puntero a estado de busqueda
			}
		
			ptr_auxiliar = ptr_auxiliar->ptr_siguiente;
		}
		
	}
	//busqueda lista estados finales
	else{
		F *ptr_auxiliar = vector_automatas->ptr_estados_finales;
		while (ptr_auxiliar != NULL){
			if (strcmp(ptr_auxiliar->ptr_estado->nombre_estado, nombre_estado) == 0){
				printf("ENCONTRADO\n");
				system("pause");
				return ptr_auxiliar->ptr_estado;//retorno puntero a estado de busqueda
			}
		
			ptr_auxiliar = ptr_auxiliar->ptr_siguiente;
		}
	}
	
	printf("NO SE ENCONTRO EL ESTADO\n");
	system("pause");
	return (Q*)NULL;
}

void listar(char *mensaje_inicio,Automata *vector_automatas,char opcion){
	//lista de estados ingresados
	if(opcion == '0'){
		
		if(vector_automatas->ptr_estados != NULL){
		
			Q *ptr_auxiliar=vector_automatas->ptr_estados;
			printf(mensaje_inicio);
		
			while(ptr_auxiliar != NULL){
			
				printf("%s\n",ptr_auxiliar->nombre_estado);
				ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
			}	
		
		}
		else
			printf("ESTADOS NO INGRESADOS AUN\n");
	}
	//lista de estados finales ingresados
	else{
		if(vector_automatas->ptr_estados_finales != NULL){
		
			F *ptr_auxiliar=vector_automatas->ptr_estados_finales;
			printf(mensaje_inicio);
		
			while(ptr_auxiliar != NULL){
			
				printf("%s\n",ptr_auxiliar->ptr_estado->nombre_estado);
				ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
			}	
		
		}
		else
			printf("ESTADOS NO INGRESADOS AUN\n");
	}
	
	system("pause");
}

void menu_definir_AFD(Automata *vector_automatas,short int contador_AFD){
	char opcion='0';
	
	do{
		system("cls");
		validar_opcion("===========================\n1.NOMBRE AUTOMATA\n2.ESTADOS AUTOMATA\n3.ALFABETO\n4.ESTADO INICIAL\n5.ESTADOS FINALES\n6.TABLA TRANSICION\n7.SALIR\n===========================\n",&opcion,'7');
		system("cls");
		
		switch(opcion){
			case '1':
				validar_nombre_automata(vector_automatas,contador_AFD);
			break;
			case '2':
				//opciones relacionadas a estados del automata
				CRUD("=================\n1.INGRESO ESTADOS\n2.LISTAR ESTADOS\n3.BUSQUEDA ESTADO\n4.ELIMINAR ESTADO\n5.SALIR\n=================\n",(vector_automatas+contador_AFD),opcion,'5');
			break;
			case '3':
				//opciones relaciondas al alfabeto del automata
				CRUD("==================\n1.INGRESO ALFABETO\n2.MOSTRAR ALFABETO\n3.SALIR\n==================\n",(vector_automatas+contador_AFD),opcion,'3');
			break;
			case '4':
				//opciones relacionadas al estado inicial
				CRUD("========================\n1.INGRESO ESTADO INICIAL\n2.MOSTRAR ESTADO INICIAL\n3.SALIR\n========================\n",(vector_automatas+contador_AFD),opcion,'3');
			break;
			case '5':
				//opciones relacionadas a los estados finales
				CRUD("=========================\n1.INGRESO ESTADOS FINALES\n2.LISTAR ESTADOS FINALES\n3.BUSQUEDA ESTADO FINAL\n4.ELIMINAR ESTADO FINAL\n5.SALIR\n=========================\n",(vector_automatas+contador_AFD),opcion,'5');
			break;
			case '6':
				//si se ingreso alfabeto,y se ingreso estados automatas
				if((vector_automatas+contador_AFD)->E[0] != '\0' && (vector_automatas+contador_AFD)->ptr_estados != NULL)	
					CRUD("==========================\n1.INGRESO TABLA TRANSICION\n2.MOSTRAR TABLA\n3.MODIFICAR TABLA\n4.SALIR\n==========================\n",(vector_automatas+contador_AFD),opcion,'4');
				else{
					printf("INGRESE INFORMACION DE ESTADOS O LENGUAJE ANTES DE ESTE PROCESO\n");
					system("pause");
				}
					
			break;
			case '7':
				//valiadacion si se dea salir al menu principal pero 
				//no se a ingresado el nombre aun del automata
				if((vector_automatas+contador_AFD)->NombreAutomata[0] == '\0'){
					printf("ANTES DE SALIR INGRESE NOMBRE DE AUTOMATA\n");
					opcion='0';
					system("pause");
				} 
					
			break;
		}
	}while(opcion != '7');
}

void validar_nombre_automata(Automata *vector_automatas,short int contador_AFD){
	char contador='0';
	
	while(contador == '0'){
		
		fflush(stdin);
		printf("INGRESE NOMBRE AUTOMATA FINITO DETERMINISTA:");
		scanf("%s",(vector_automatas+contador_AFD)->NombreAutomata);
		//primero el nombre del Automata tiene que ser maximo 2 caracteres
		if( strlen((vector_automatas+contador_AFD)->NombreAutomata) < 3){
			contador='1';
			//se verifica en el vector_automatas los nombres de los mismos
			for(int i=0; i<3; i++){
				if(vector_automatas+i != vector_automatas+contador_AFD){
					if(strcmp((vector_automatas+contador_AFD)->NombreAutomata,(vector_automatas+i)->NombreAutomata) == 0){
						printf("NOMBRE NO VALIDO\n");
						contador='0';
						break;
					}			
				}
			}			
		}
		else
			printf("NOMBRE NO VALIDO\n");
			
	}
}

void conteo_filas_columnas(Automata *vector_automata,short int *tamano,char opcion){
	short int contador=0;
	//opcion para obtener filas tamano lista de estados
	if(opcion == '0'){
		Q *ptr_auxiliar=vector_automata->ptr_estados;
		
		while(ptr_auxiliar != NULL){
			contador+=1;
			ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
		}
		
	}
	//opcion para obtener columnas o numero de caracteres alfabeto
	else{
		
		for(short int i=0; *(vector_automata->E+i) != '\0'; i++){
			//se tiene un espacion el contador +1
			if(*(vector_automata->E+i) == ' ')
				contador+=1;
		}
		contador+=1;
	}
	*tamano=contador;
}

void localizar_fila(Automata *vector_automatas, Q *ptr_busqueda,short int *fila_matriz){
	short int contador_fila=0;
	Q *ptr_auxiliar=vector_automatas->ptr_estados;
	//se obtiene la fila de la lista de estados del automata
	//ya que la tabla de transicion sigue el mismo orden de filas
	//en definitva el orden de la lista de estados
	while(ptr_auxiliar != NULL){
		
		if(ptr_auxiliar == ptr_busqueda){
			*fila_matriz=contador_fila;
			break;
		}
			
		contador_fila+=1;
		ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
	}
	
}

void seleccion_automata(Automata *vector_automatas,short int *contador_AFD,char opcion){
	//si no se ingreso aun ningun automata
	if(vector_automatas->NombreAutomata[0] == '\0'){
		//ingreso del automata
		if(opcion == '1')
			menu_definir_AFD(vector_automatas,*contador_AFD);
		//opcion de la validacion de cadena
		else{
			printf("INGRESAR PRIMERA OPCION ANTES DE CONTINUAR\n");
			system("pause");
		}
			
	}
	//ya se ingresaron anteriormente automatas
	else{
					
		*contador_AFD=-1;
		//se muestran automatas ingresados y se selecciona en un menu			
		while(*contador_AFD < 0){
			system("cls");
			printf("=======================================\nSELECCION AUTOMATA FINITO DETERMINISTA\n");
			for(short int i=0; i<3; i++){
				//es un atomata ya ingresado
				if( (vector_automatas+i)->NombreAutomata[0] != '\0' )
					printf("%d.AUTOMATA:%s\n",i+1,(vector_automatas+i)->NombreAutomata);
				//muestra que se puede ingresar un nuevo automata
				else
					printf("%d.NUEVO\n",i+1);
			}
				printf("4.SALIR\n=======================================\n");
				scanf("%d",contador_AFD);
										
		}
					
		if(*contador_AFD != 4){
			//el vector comienza con index cero por ende se resta uno
			*contador_AFD-=1;
			//si es la opcion de ingreso de automata
			if(opcion == '1')
				menu_definir_AFD(vector_automatas,*contador_AFD);
			//es opcion de evaluacion
			else{
				system("cls");
				//es un automata ya ingresado
				if((vector_automatas+*contador_AFD)->NombreAutomata[0] != '\0')
					//se verifica que para el proceso de evaluacion requiera todos los datos
					//en el ingreso de automata
					if((vector_automatas+*contador_AFD)->ptr_estados == NULL || (vector_automatas+*contador_AFD)->E[0] == '\0' || (vector_automatas+*contador_AFD)->ptr_estado_inicial == NULL || (vector_automatas+*contador_AFD)->ptr_estados_finales == NULL || (vector_automatas+*contador_AFD)->D[0][0] == NULL){
						printf("FALTA DE DATOS ASEGURESE DE HABER INGRESADO LOS DATOS NECESARIOS\n");
						system("pause");
					}
					//paso al proceso de evaluacion	
					else
						proceso_evaluacion(vector_automatas+*contador_AFD);
				else{
					printf("SELECCIONAR AUTOMATA YA CREADO O CREE UNO NUEVO\n");
					system("pause");
				}
					
			}
				
		}
						
	}
	
}

void separar_cadena(Automata *vector_automatas,char lenguaje[][TAMANO],short int columnas){
	char alfabeto[ALFABETO];
	
	strcpy(alfabeto,vector_automatas->E);
    strcpy(lenguaje[0], strtok(alfabeto, " "));
    
    for (short int i = 1; i < columnas; i++){
        strcpy(lenguaje[i], strtok(NULL, " "));
    }
    
}

void mostrar_tabla_transicion(Automata *vector_automatas,short int filas,short int columnas){
	char lenguaje[columnas][TAMANO];
	Q *ptr_auxiliar=vector_automatas->ptr_estados;
							
	separar_cadena(vector_automatas,lenguaje,columnas);
							
	printf("=========================\n\tTABLA TRANSICION\n=========================\n\t");
						
	for (short int j = 0; j < columnas; j++){
        printf("%s\t",lenguaje[j]);
            						
	}
    printf("\n");
	for (short int i = 0; i < filas; i++){
        printf("%s\t",ptr_auxiliar->nombre_estado);
        
		for (short int j = 0; j < columnas; j++){
            printf("%s\t", vector_automatas->D[i][j]->nombre_estado);
            						
        }
    	printf("\n");
        ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
    }
}

void ingreso_tabla_transicion(Automata *vector_automatas, short int filas, short int columnas){
	char lenguaje[columnas][TAMANO];
    Q *ptr_auxiliar = vector_automatas->ptr_estados;
    
    separar_cadena(vector_automatas,lenguaje,columnas);
    
    vector_automatas->D = malloc(filas * sizeof(Q **));    
    
    for (short int i = 0; i < filas; i++){
        vector_automatas->D[i] = malloc(columnas * sizeof(Q*));
		for (short int j = 0; j < columnas; j++){
			
            Q *ptr_busqueda = NULL;
            //Esta parte del codigo nos ayuda a verificar el ingreso del nombre del estado si consta en la lista y devolver 
            //el puntero al estado
            while (ptr_busqueda == NULL){
            	
            	printf("INGRESO ESTADO [%d,%d], CORRESPONDIENTE A [%s,%s]: ", i + 1, j + 1, ptr_auxiliar->nombre_estado, lenguaje[j]);
                ptr_busqueda = busqueda("",vector_automatas,'0');
            }
            
            vector_automatas->D[i][j]= ptr_busqueda;
        }
        ptr_auxiliar = ptr_auxiliar->ptr_siguiente;
    }
}

void modificar_tabla_transicion(Automata *vector_automatas,short int filas,short int columnas){
	char cadena[TAMANO];
	Q *ptr_busqueda = NULL;
	short int fila_auxiliar=0;
	short int columa_auxiliar=-1;
	char lenguaje[columnas][TAMANO];
    Q *ptr_auxiliar = vector_automatas->ptr_estados;
    
	mostrar_tabla_transicion(vector_automatas,filas,columnas);
    separar_cadena(vector_automatas,lenguaje,columnas);
    
    while(ptr_busqueda == NULL){
    	ptr_busqueda=busqueda("INGRESE ESTADO FILA PARA BUSCAR EN LA TABLA DE TRANSICION:",vector_automatas,'0');
	}
	
    for(short int i=0; i<filas; i++){
    	if(ptr_busqueda == ptr_auxiliar)
    		break;
    	fila_auxiliar+=1;
    	ptr_auxiliar=ptr_auxiliar->ptr_siguiente;
	}
	
	while(columa_auxiliar < 0){
		fflush(stdin);
		printf("INGRESE CARACTERES COLUMNA ALFABETO PARA BUSQUEDA:");
		gets(cadena);
		for(short int i=0; i<columnas; i++){
			if(strcmp(cadena,lenguaje[i]) == 0){
				columa_auxiliar=i;
				break;		
			}
		}
		
		if(columa_auxiliar < 0)
			printf("CADENA INCORRECTA\n");	
	}

	ptr_busqueda=NULL;
	while(ptr_busqueda == NULL){
		ptr_busqueda=busqueda("INGRESE ESTADO:",vector_automatas,'0');
	}
	vector_automatas->D[fila_auxiliar][columa_auxiliar]= ptr_busqueda;
	
}

void CRUD(char *mensaje_inicio,Automata *vector_automatas,char opcion_apoyo,char numero_opciones){
	char opcion='0';
	
	do{
		short int filas;
		short int columnas;
		Q *ptr_nuevo=NULL;
		Q *ptr_busqueda=NULL;
		F *ptr_nuevo_estado_final=NULL;
		conteo_filas_columnas(vector_automatas,&filas,'0');
		conteo_filas_columnas(vector_automatas,&columnas,'1');
		
		system("cls");
		validar_opcion(mensaje_inicio,&opcion,numero_opciones);
		system("cls");
		
		switch(opcion){
			case '1'://ingreso
				switch(opcion_apoyo){
					case '2'://Ingreso estados
						if(vector_automatas->D != NULL){
							liberar_matriz_dinamica(vector_automatas);
							printf("TIP:RECUERDE INGRESAR NUEVAMENTE FUNCION DE TRANSICION\n");
						}
						ptr_nuevo=crear_estado(ptr_nuevo);
						validar_nombre_estado_automata(vector_automatas->ptr_estados,ptr_nuevo);
						vector_automatas->ptr_estados=lista_estados(vector_automatas->ptr_estados,ptr_nuevo);
						vector_automatas->D = NULL;
					break;
					case '3'://Ingreso alfabeto
						if(vector_automatas->D != NULL){
							liberar_matriz_dinamica(vector_automatas);
							printf("TIP:RECUERDE INGRESAR NUEVAMENTE FUNCION DE TRANSICION\n");
						}
						ingreso_alfabeto(vector_automatas);
						vector_automatas->D = NULL;
					break;
					case '4'://Ingreso estado inicial
						ptr_busqueda=busqueda("INGRESE EL NOMBRE DEL ESTADO A SER EL INICIAL:",vector_automatas,'0');	
						if(ptr_busqueda != NULL){
							vector_automatas->ptr_estado_inicial=ptr_busqueda;
						}
						
					break;
					case '5'://Ingreso estados finales
						ptr_busqueda=busqueda("INGRESE NOMBRE DE ESTADO A INSERTAR A ESTADOS FINALES:",vector_automatas,'0');
						if(ptr_busqueda != NULL){
							ptr_nuevo_estado_final=crear_estado_final(ptr_nuevo_estado_final);
							vector_automatas->ptr_estados_finales=lista_estados_finales(vector_automatas->ptr_estados_finales,ptr_nuevo_estado_final,ptr_busqueda);
						}
							
					break;
					case '6'://Ingreso tabla transicion
						ingreso_tabla_transicion(vector_automatas,filas,columnas);
					break;
				}
			break;
			case '2'://listar
				switch(opcion_apoyo){
					case '2':// estados
						listar("ESTADOS AUTOMATA:\n",vector_automatas,'0');
					break;
					case '3':// alfabeto
						if(*((vector_automatas->E)+0) == '\0')
							printf("ALFABETO NO INGRESADO AUN\n");
						else
							printf("ALFABETO: [%s]\n",vector_automatas->E);
						system("pause");
					break;
					case '4'://estado inicial
						if(vector_automatas->ptr_estado_inicial != NULL)
							printf("ESTADO INICIAL:%s\n",vector_automatas->ptr_estado_inicial);
						else
							printf("ESTADO INICIAL NO INGRESADO AUN\n");
							
						system("pause");
					break;
					case '5':// estados finales
						listar("ESTADOS FINALES AUTOMATA:\n",vector_automatas,'1');
					break;
					case '6'://tabla transicion
						if(vector_automatas->D != NULL){
							mostrar_tabla_transicion(vector_automatas,filas,columnas);
						}
						else{
							printf("FUNCION DE TRANSICION AUN NO INGRESADA\n");
						}
    					system("pause");
					break;
				}
			break;
			case '3'://busqueda
				switch(opcion_apoyo){	
					case '2':// estados
				 	 	ptr_busqueda=busqueda("INGRESE NOMBRE DE ESTADO A BUSCAR:",vector_automatas,'0');
						if(ptr_busqueda != NULL){
							do{
								system("cls");
								validar_opcion("==================\n1.MODIFICAR NOMBRE\n2.SALIR\n==================\n",&opcion,'2');
								system("cls");
								
								if(opcion == '1')
									validar_nombre_estado_automata(vector_automatas->ptr_estados,ptr_busqueda);
									
							}while(opcion != '2');
						}
			
					break;                                                                                                                         
					case '5':// estados finales
						ptr_busqueda=busqueda("INGRESE EL NOMBRE DEL ESTADO A BUSCAR EN LOS ESTADOS FINALES:",vector_automatas,'1');
					break;
					case '6'://busqueda cambio funcion transicion
						if(vector_automatas->D != NULL){
							modificar_tabla_transicion(vector_automatas,filas,columnas);
						}
						else{
							printf("FUNCION DE TRANSICION AUN NO INGRESADA\n");
							system("pause");
						}
					break;
				}
			break;	
			case '4'://eliminacion
				switch(opcion_apoyo){
					case '2':// estados
						if(vector_automatas->D != NULL){
							liberar_matriz_dinamica(vector_automatas);
							printf("TIP:RECUERDE INGRESAR NUEVAMENTE FUNCION DE TRANSICION\n");
						}
						ptr_busqueda=busqueda("INGRESE NOMBRE DE ESTADO A ELIMINAR:",vector_automatas,'0');
						if(ptr_busqueda != NULL){
							vector_automatas->ptr_estados=eliminar_estado(vector_automatas->ptr_estados,ptr_busqueda);
							vector_automatas->ptr_estados_finales=eliminar_estado_final(vector_automatas,ptr_busqueda);
							if(ptr_busqueda == vector_automatas->ptr_estado_inicial)//Si se elimino el estado inicial, al eliminar un estado	
								vector_automatas->ptr_estado_inicial=NULL;	
						}
						vector_automatas->D = NULL;
					
					break;
					case '5':// estados finales
						ptr_busqueda=busqueda("INGRESE NOMBRE DE ESTADO A ELIMINAR:",vector_automatas,'1');
						if(ptr_busqueda != NULL)
							vector_automatas->ptr_estados_finales=eliminar_estado_final(vector_automatas,ptr_busqueda);
					break;
				}
			break;

		}
		
	}while(opcion != numero_opciones);
}

void validar_cadena_alfabeto(char *cadena_validacion,char cadena[][TAMANO],short int *filas_cadena){
	short int contador=0;
	short int contador_inicio=0;
	short int contador_columnas=0;
    short int contador_iteracion=0;
    short int contador_evaluacion=0;
	char cadena_ingreso[ALFABETO];
	
	do{
		contador=0;
		contador_inicio=0;
		contador_columnas=0;
    	contador_iteracion=0;
    	contador_evaluacion=0;
    	
		fflush(stdin);
		printf("INGRESE CADENA SIN ESPACIOS PARA EVALUACION:");
		gets(cadena_ingreso);
		
		while(cadena_ingreso[contador] != '\0'){
		
			contador_iteracion=0;
			contador_inicio=contador;
		
			while(cadena_validacion[contador_iteracion] != '\0'){
			
				if(cadena_ingreso[contador] == cadena_validacion[contador_iteracion]){
				
					if(cadena_validacion[contador_iteracion+1] == ' ' || cadena_validacion[contador_iteracion+1] == '\0'){
						short int j=0;
						for(short int i=contador_inicio; i<contador+1; i++){
							cadena[contador_columnas][j]=cadena_ingreso[i];
							j+=1;
						}
					
						cadena[contador_columnas][j]='\0';
						break;
					}
					else
						contador+=1;
				}
			
				contador_iteracion+=1;
			}
			contador+=1;
			contador_columnas+=1;
		}
		
		for(short int k=0; k<contador_columnas; k++){
			contador_evaluacion+=strlen(cadena[k]);
		}
		if(contador_evaluacion == strlen(cadena_ingreso))
			printf("CADENA CORRECTA\n");
		else{
			printf("CADENA INCORRECTA\n");
			for(short int i=0; i<contador_columnas; i++){
				for(short int j=0; j<TAMANO; j++){
					cadena[i][j]=0;
				}
			}
		}
		
	}while(contador_evaluacion != strlen(cadena_ingreso));
	*filas_cadena=contador_columnas;
}

void localizar_columna(char alfabeto[][TAMANO],char *busqueda,short int columnas,short int *columna_matriz){
	
	for(short int i=0; i<columnas; i++){
		
		if(strcmp(alfabeto[i],busqueda) == 0){
			*columna_matriz=i;
			break;
		}
	}
	
}
