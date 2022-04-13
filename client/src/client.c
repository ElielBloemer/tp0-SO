#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	//logger = log_create("tp0.log","log del tp0",1,LOG_LEVEL_INFO);
    log_info(logger,"HOLA! SOY UN LOG");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	//config = config_create("./cliente.config");

	//aca é onde guardo os valor pedidos
    ip = config_get_string_value(config,"IP");
    puerto = config_get_string_value(config,"PUERTO");
    valor = config_get_string_value(config,"CLAVE");

    // Usando el config creado previamente, leemos los valores del config y los
    // dejamos en las variables 'ip', 'puerto' y 'valor'
    // Loggeamos el valor de config

    log_info(logger,"lei la CLAVE: %s \n",valor);
    //log_info(logger,puerto);
    //log_info(logger,ip);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor, OJOOO CON EL SERVIDOR APAGO NO PODES CREAR LA CONEXION
	conexion = crear_conexion(ip, puerto);

	//Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
    printf("mande al servidor la CLAVE: %s \n",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log","log del tp0",1,LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		puts("No pude hacer el log");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("/home/utnso/tp0-SO/client/cliente.config");

	if(nuevo_config == NULL){
		puts("No se puede crear el config");
		exit(1);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	//t_paquete* paquete = crear_paquete();

	//puts("Los valores de ler por consola tb enviaremos al servidor");
	while(1){
		leido = readline(" > ");
		if(strcmp(leido, "") == 0){break;}
		log_info(logger,leido);
		free(leido);
	}
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(1){
			leido = readline("ingrese un valor del paquete > ");
			if(strcmp(leido, "") == 0){break;}
	// Leemos y esta vez agregamos las lineas al paquete
			agregar_a_paquete(paquete,leido,strlen(leido)+1);
			free(leido);
		}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
    free(leido);
    enviar_paquete(paquete,conexion);
    puts("Mensaje enviada al servidor, con exito!");
    eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
