#include "client.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger, "Soy un Log");
	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "Valor leido del config: %s", valor);

	conexion = crear_conexion(ip, puerto);

	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);

	return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger");
		abort();
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	if(nuevo_config == NULL){
		printf("No se pudo leer el archivo cliente.config\n");
		abort();
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// enelmaterial
    while (1) {
        leido = readline(">");
        if (!leido) {
            break;
        }
        if (!strcmp(leido, "")) {
            free(leido);
            break;
        }
        log_info(logger, "%s", leido);
        free(leido);
    }
}

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!



void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete = crear_paquete();

	while(1) {
		leido = readline(">");

		if(leido == NULL) {
			break;
		}

		if(strcmp(leido, "") == 0) {
			free(leido);
			break;
		}

		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
	}

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
