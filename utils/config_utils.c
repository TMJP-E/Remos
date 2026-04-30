#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "config_utils.h"

/**
 * @brief Realiza la validacion del archivo de configuracion para que tenga un formato acorde a la especficacion.
 *
 * @details Lee cada linea por separado, y garantiza que se encuentren las claves de los pares clave-valor definidos en la especificacion, no valida los contenidos al ser dinamicos
 *
 * @param path Direccion del archivo de configuracion a validar.
 *
 * @return Si tiene un formato valido o no, esto es, que se encuentren todas las claves
 */
bool validateConfigFile(char *path)
{
    char option_keys[OPTIONS_COUNT][OPTIONS_LENGTH] = OPTIONS;
    char current_line[INPUT_LENGTH] = {0};
    char *key_delimiter = "=";
    char *token;
    bool valid = true;
    bool checked[OPTIONS_COUNT] = {false};

    // Abre el archivo con la direccion especificada y comprueba si existe.
    FILE *config_file = fopen(path, "r");
    if (config_file == NULL)
    {
        return false;
    }

    // Verifica que se encuentren las cuatro claves, sin importar el orden o claves adicionales.
    while (fgets(current_line, sizeof(current_line), config_file) != NULL)
    {
        token = strtok(current_line, key_delimiter);
        for (size_t i = 0; i < OPTIONS_COUNT; i++)
        {
            // Si la llave coincide y no ha sido encontrada anteriormente.
            if (strcmp(token, option_keys[i]) == 0 && !checked[i])
            {
                checked[i] = true;
                break;
            }
        }
    }

    // Valida que se encuentren todas las claves.
    for (size_t i = 0; i < OPTIONS_COUNT; i++)
    {
        valid = checked[i] && valid;
    }

    return valid;
}

/**
 * @brief Actualiza o agrega un valor en el archivo de configuracion con el formato clave=valor.
 *
 * @details Utiliza un archivo temporal para copiar la informacion y luego reemplazarlo por el original.
 * Se utilizara tanto para cambiar el nombre de la bitacora. Si la clave existe en el archivo, actualiza su valor; si no existe, la agrega al final.
 *
 * @param config_path Direccion del archivo de configuracion.
 * @param key Clave a buscar dentro del archivo de configuracion.
 * @param new_value Nuevo valor que se asignara a la clave especificada.
 */
void updateConfig(char *config_path, const char *key, const char *new_value)
{
    char temp_path[INPUT_LENGTH];

    snprintf(temp_path, sizeof(temp_path), "%s/%s", MAIN_DIRNAME, TEMP_FILENAME);

    FILE *original_file = fopen(config_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    char current_line[CONFIG_LENGTH];
    char search_key[INPUT_LENGTH];
    bool key_found = false;

    // Prepara la cadena para la busqueda en el formato "key="
    snprintf(search_key, sizeof(search_key), "%s=", key);
    int key_length = strlen(search_key);

    while (fgets(current_line, sizeof(current_line), original_file) != NULL)
    {
        // Comparamos solo hasta la longitud de "key="
        if (strncmp(current_line, search_key, key_length) == 0)
        {
            // Si lo encontramos, escribimos la clave y el nuevo valor en el archivo temporal, y activamos key_found
            fprintf(temp_file, "%s=%s\n", key, new_value);
            key_found = true;
        }
        else
        {
            // Si no es la linea que buscamos, la copiamos tal cual al archivo temporal
            fprintf(temp_file, "%s", current_line);
        }
    }
    fclose(original_file);

    // Si no se encontro la clave, la agregamos al final del archivo temporal
    if (!key_found)
    {
        fprintf(temp_file, "%s=%s\n", key, new_value);
    }

    fclose(temp_file);

    // Reemplazamos el archivo original con el temporal
    remove(config_path);
    rename(temp_path, config_path);
}

/**
 * @brief Lee el valor de la clave especificada en el archivo de configuracion
 *
 * @param config_path Direccion del archivo de configuracion.
 * @param key Clave a buscar para obtener el valor.
 *
 * @return Valor asociado con la clave.
 * @throw `NULL` si no encuentra la clave.
 */
char *readValueFromKey(char *config_path, const char *key)
{
    FILE *config_file = fopen(config_path, "r");

    char current_line[CONFIG_LENGTH];
    char search_key[INPUT_LENGTH];
    char *key_delimiter = "=";
    char *token;
    bool key_found = false;

    // Prepara la cadena para la busqueda en el formato "key="
    snprintf(search_key, sizeof(search_key), "%s=", key);
    int key_length = strlen(search_key);

    while (fgets(current_line, sizeof(current_line), config_file) != NULL)
    {
        // Comparamos solo hasta la longitud de "key="
        if (strncmp(current_line, search_key, key_length) == 0)
        {
            // Si lo encontramos, obtenemos el valor usando tokens y se retorna.
            strtok(current_line, key_delimiter);
            token = strtok(current_line, key_delimiter);
            fclose(config_file);
            return token;
        }
    }
    return NULL;
}