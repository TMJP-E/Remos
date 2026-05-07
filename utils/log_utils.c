#include <stdio.h>
#include <string.h>
#include "log_utils.h"
#include "config_utils.h"

/**
 * @brief Ejecuta un comando y lee su output linea por linea mediante un pipe.
 *
 * @details Limpia el salto de linea del comando antes de ejecutarlo. Abre el pipe con popen() en modo lectura. fgets() espera output del proceso, y retorna NULL unicamente cuando el proceso termina y el pipe se cierra.
 *
 * @param command Comando a ejecutar, recibido directamente de fgets() en remos.c.
 * @param keywords Vector de palabras clave a buscar en cada linea.
 * @param counters Cuenta ocurrencias de keywords.
 * @param log_name Nombre del archivo de bitacora.
 * @param url URL del Webhook de Discord.
 * @param url_enabled Indica si el Webhook esta activo.
 */
void startLog(char *command, StringVector *keywords, int *counters, char *log_name, char *url, bool url_enabled) {
    command[strcspn(command, "\n")] = '\0';

    //Redirige la salida de stderr a stdout;
    char full_command[CONFIG_LENGTH];
    snprintf(full_command, sizeof(full_command), "%s 2>&1", command);

    FILE *proceso = popen(full_command, "r");
    if (proceso == NULL) {
        printf("Error: no se pudo ejecutar el comando.\n");
        return;
    }

    char linea[CONFIG_LENGTH];

    while (fgets(linea, sizeof(linea), proceso) != NULL) {
        printf("%s", linea);
    }

    pclose(proceso);
}