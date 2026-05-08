#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "log_utils.h"
#include "config_utils.h"
#include "discord_utils.h"

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

        if (checkKeywords(linea, keywords, counters)) {
            
            saveToLog(log_name, linea);

            if (url_enabled && strlen(url) > 0) {
                sendDiscordAlert(url, linea);
            }
        }
    }

    pclose(proceso);
}

//Limpia el archivo de bitacora, osea, sobrescribe el archivo con una nueva version vacia, manteniendo el mismo nombre.
void initLogFile(const char *log_name) {
    char file_path[CONFIG_LENGTH];
    snprintf(file_path, sizeof(file_path), "%s/%s/%s", MAIN_DIRNAME, LOGS_DIRNAME, log_name);
    FILE *log_file = fopen(file_path, "w");
    if (log_file != NULL) {
        fclose(log_file);
    } else {
        printf("Error: no se pudo crear el archivo de bitacora.\n");
    }
}

/**
 * @brief Busca si alguna de las palabras clave existe dentro de una linea de texto.
 *
 * @details Itera sobre el vector dinamico de palabras clave y utiliza la funcion nativa strstr() para
 * buscar coincidencias exactas. Si encuentra una palabra, incrementa su contador correspondiente.
 * Es importante liberar la memoria del getElement() en cada iteracion.
 *
 * @param linea La cadena de texto (linea leida de la terminal) que se va a analizar.
 * @param keywords Puntero al vector dinamico que contiene las palabras clave registradas.
 * @param counters Arreglo de enteros (paralelo al vector) para llevar la cuenta de detecciones.
 * * @return `true` si encontro al menos una coincidencia, `false` si la linea esta limpia.
 */
bool checkKeywords(const char *line, StringVector *keywords, int *counters) {
    for (size_t i = 0; i < getSize(keywords); i++) {
        char *word = getElement(keywords, i);
        if (strstr(line, word) != NULL) {
            counters[i]++;
            free(word);
            return true;
        }
        free(word);
    }
    return false;

}

/**
 * @brief Guarda una linea de error en el archivo de bitacora fisico.
 *
 * @details Construye la ruta del archivo y lo abre en modo "append" ("a") para no sobreescribir 
 * el historial previo. Obtiene la hora y la fecha actual del sistema y la formatea antes de escribir la linea.
 *
 * @param log_name El nombre del archivo de bitacora (sin extension .txt).
 * @param linea El texto exacto del error que se va a guardar.
 */
void saveToLog(const char *log_name, const char *line) {
    char file_path[CONFIG_LENGTH];
    snprintf(file_path, sizeof(file_path), "%s/%s/%s", MAIN_DIRNAME, LOGS_DIRNAME, log_name);
    FILE *log_file = fopen(file_path, "a");
    
    if (log_file != NULL) {
        time_t current_time = time(NULL);
        struct tm *local_time = localtime(&current_time);
        fprintf(log_file, "[%02d-%02d-%04d %02d:%02d:%02d] %s", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900, local_time->tm_hour, local_time->tm_min, local_time->tm_sec, line);
        fclose(log_file);
    } else {
        printf("Error: no se pudo abrir el archivo de bitacora para escribir.\n");
    }
}