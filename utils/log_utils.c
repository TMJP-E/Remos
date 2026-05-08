#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "log_utils.h"
#include "config_utils.h"
#include "discord_utils.h"

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

//Convierte una cadena a minusculas
void toLowerCase(char *str) {
    for (size_t i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}
//Elimina secuencias ANSI de una linea, dejando solo el texto limpio, para evitar falsos positivos en la deteccion de palabras clave.
void cleanLogLine(const char *input, char *output){
    int i=0, j=0;
    int in_ansi = 0, fin_ansi = 0;
    while (input[i] != '\0') {
        if (input[i] == '\033' && input[i+1] == '[') {

            in_ansi = 1;
            i++;
        } 
        else if(in_ansi){
            fin_ansi = 1;
        }
        if (in_ansi && fin_ansi) {
            if (input[i] == 'm') {
                in_ansi = 0; // Fin de la secuencia ANSI
            }
            fin_ansi = 0; // Reiniciar fin_ansi para la próxima secuencia
        }
        if (!in_ansi){
            output[j++] = input[i];
        }
        i++;
    }
    output[j] = '\0';
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

    char lower_line[CONFIG_LENGTH];
    strncpy(lower_line, line, sizeof(lower_line) - 1);
    lower_line[sizeof(lower_line) - 1] = '\0';
    toLowerCase(lower_line);

    for (size_t i = 0; i < getSize(keywords); i++) {
        char *word = getElement(keywords, i);
        char lower_word[CONFIG_LENGTH];
        strncpy(lower_word, word, sizeof(lower_word) - 1);
        lower_word[sizeof(lower_word) - 1] = '\0';
        toLowerCase(lower_word);
        
        if (strstr(lower_line, lower_word) != NULL) {
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
    snprintf(file_path, sizeof(file_path), "%s/%s/%s", MAIN_DIRNAME,LOGS_DIRNAME, log_name);
    FILE *log_file = fopen(file_path, "a");
    
    if (log_file != NULL) {
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        char formatted_time[20];
        strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", time_info);

        fprintf(log_file, "[%s] %s", formatted_time, line);
        fclose(log_file);
    } else {
        printf("Error: no se pudo abrir el archivo de bitacora para escribir.\n");
    }
}


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

    initLogFile(log_name);
    //Redirige la salida de stderr a stdout;
    char full_command[CONFIG_LENGTH];
    snprintf(full_command, sizeof(full_command), "%s 2>&1", command);

    FILE *proceso = popen(full_command, "r");
    if (proceso == NULL) {
        printf("Error: no se pudo ejecutar el comando.\n");
        return;
    }

    char linea[CONFIG_LENGTH];
    char clean_line[CONFIG_LENGTH*2];

    while (fgets(linea, sizeof(linea), proceso) != NULL) {
        printf("%s", linea);
        cleanLogLine(linea, clean_line);
        if (checkKeywords(linea, keywords, counters)) {
            
            saveToLog(log_name, clean_line);

            if (url_enabled && strlen(url) > 0) {
                sendDiscordAlert(url, clean_line);
            }
        }
    }

    pclose(proceso);
}