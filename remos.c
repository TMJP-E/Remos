/** Remos
 *
 * El flujo principal es el siguiente:
 *   Logo
 *   Menu principal
 *   Opciones
 *   Iniciar
 *   Salida
 *
 * Las diferentes funciones requeridas son las siguientes:
 * #Desplegar Logo
 * #Desplegar Menu Principal
 * #Desplegar Menu Opciones
 *   #Crear estructura de archivos
 *   #Verificar archivo de opciones
 *   (#Agregar | Modificar | Eliminar) palabras clave
 *   Nombrar bitacora
 *   #(Agregar/Modificar/Eliminar) URL
 *   #Desactivar Webhook
 *   Verificacion Webhook
 * Iniciar ejecucion
 *   #Verificar archivo de opciones
 *   #Leer archivo de opciones
 *   1. Desplegar configuracion actual
 *   2. Creacion de bitacora
 *   3. Encabezados de bitacora
 *   Lectura, guardado y conteo de lineas en la terminal
 *   Envio de datos mediante Webhook.
 * # = Completado
 *
 * Se omite hacer mencion a la entrada del usuario como un apartado a desarrollar, ya que es mejor practica manejarla en el flujo principal, no en funciones separadas.
 *
 * Realizado por Joshua Tellez, Edrick Puc y Mitchell Bachtold, bajo el equipo Remos, en la asignatura Programacion Estructurada.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "utils/draw_utils.h"
#include "utils/file_utils.h"
#include "utils/config_utils.h"
#include "utils/vector.h"

#define INPUT_MESSAGE "Ingrese una opcion: "
#define INVALID_MESSAGE "Opcion Invalida.\n"

// Macro para validacion de entrada, en formato de funcion, se realizo en formato de macro por el contexto que necesita `continue`.
#define inputValidation(input)   \
    printf(INPUT_MESSAGE);       \
    if (scanf("%d", &input) > 1) \
    {                            \
        getchar();               \
        printf(INVALID_MESSAGE); \
        continue;                \
    }                            \
    getchar()

// Manejo de la bitacora

bool readLogName(char *config_filename)
{
    char *log_name = readValueFromKey(config_filename, "log");
    if (log_name != NULL && strlen(log_name) > 0)
    {
        printf("El nombre actual de la bitacora es: %s\n", log_name);
        free(log_name);
    }
    else
    {
        printf("El nombre actual de la bitacora es: log\n");
    }
    return true;
}

bool updateLogName(char *config_filename)
{
    char new_name[INPUT_LENGTH] = "";
    printf("Cambiar nombre: ");
    if (fgets(new_name, sizeof(new_name), stdin) != NULL)
    {
        // Validamos que el nombre no contenga el delimitador de `=`.
        new_name[strcspn(new_name, "\n")] = 0;
        if (strlen(new_name) == 0)
        {
            printf("No se cambio el nombre.");
            return true;
        }
        if (strchr(new_name, '=') != NULL)
        {
            printf("Nombre invalido. La bitacora no puede tener el caracter '=' en el nombre.\n");
            return false;
        }
        printf("Nuevo nombre de la bitacora: %s.\n", new_name);
        updateConfig(config_filename, "log", new_name);
        return true;
    }
    return false;
}

// Manejo de Palabras clave en un Vector

bool loadKeywords(char *config_filename, StringVector *kwords_vector)
{
    // Cargamos todas las palabras clave, en la memoria, en un vector dinamico
    char *current_kwords = readValueFromKey(config_filename, "keywords");

    if (current_kwords != NULL && strlen(current_kwords) > 0)
    {
        // Creamos copia de la cadena original para no modificarla con strtok
        char mut_kwords[CONFIG_LENGTH];
        strcpy(mut_kwords, current_kwords);
        char *token = strtok(mut_kwords, ",");
        while (token != NULL)
        {
            pushElement(kwords_vector, token);
            token = strtok(NULL, ",");
        }
        free(current_kwords);
        return true;
    }
    return false;
}

bool addKeyword(StringVector *kwords_vector)
{
    char new_kword[INPUT_LENGTH] = "";
    printf("Nueva palabra: ");
    if (fgets(new_kword, sizeof(new_kword), stdin) != NULL)
    {
        // Validamos que la nueva palabra clave no contenga el delimitador de nueva linea, coma o igual
        new_kword[strcspn(new_kword, "\n")] = 0;
        if (strlen(new_kword) == 0)
        {
            printf("No se agrego ninguna palabra.");
            return true;
        }
        if (strchr(new_kword, ',') != NULL && strchr(new_kword, '=') != NULL)
        {
            printf("Palabra invalida. Las palabras clave no pueden contener los caracteres ',' o '='.\n");
            return false;
        }
        // Agregamos la nueva palabra clave al vector dinamico
        pushElement(kwords_vector, strdup(new_kword));
        printf("Palabra clave agregada.\n");
        return true;
    }
    return false;
}

bool modifyKeyword(StringVector *kwords_vector)
{
    if (isEmpty(kwords_vector))
    {
        printf("No hay palabras clave para modificar.\n");
        return false;
    }

    int index_option = 0;
    char new_kword[INPUT_LENGTH] = "";
    printf("Ingrese el indice de la palabra que desea modificar (o -1 para regresar).\n");
    for (size_t i = 0; i < getSize(kwords_vector); i++)
    {
        printf("%d. %s\n", (i + 1), getElement(kwords_vector, i));
    }

    if (scanf("%d", &index_option) > 1 && !(index_option >= 0 && index_option < getSize(kwords_vector)))
    {
        getchar();
        printf(INVALID_MESSAGE);
        return false;
    }

    if (index_option == -1)
    {
        printf("Ninguna palabra fue eliminada.");
    }

    printf("Nueva palabra: ");
    if (fgets(new_kword, sizeof(new_kword), stdin) != NULL)
    {
        new_kword[strcspn(new_kword, "\n")] = 0;
        if (strlen(new_kword) == 0)
        {
            printf("No se modifico la palabra.");
            return true;
        }
        if (strchr(new_kword, ',') != NULL && strchr(new_kword, '=') != NULL)
        {
            printf("Palabra invalida. Las palabras clave no pueden contener los caracteres ',' o '='.\n");
            return false;
        }
        printf("Palabra clave modificada de \"%s\" a \"%s\"\n", getElement(kwords_vector, index_option - 1), new_kword);
        modifyElement(kwords_vector, index_option - 1, new_kword);
        return true;
    }
    return false;
}

bool deleteKeyword(StringVector *kwords_vector)
{
    if (isEmpty(kwords_vector))
    {
        printf("No hay palabras clave para eliminar.\n");
        return false;
    }

    int index_option = 0;
    printf("Ingrese el indice de la palabra que desea eliminar (o -1 para regresar).\n");
    for (size_t i = 0; i < getSize(kwords_vector); i++)
    {
        printf("%d. %s\n", (i + 1), getElement(kwords_vector, i));
    }

    if (scanf("%d", &index_option) > 1 && !(index_option >= 0 && index_option < getSize(kwords_vector)))
    {
        getchar();
        printf(INVALID_MESSAGE);
        return false;
    }

    if (index_option == -1)
    {
        printf("Ninguna palabra fue eliminada.");
        return true;
    }

    printf("Palabra clave %s eliminada.\n", getElement(kwords_vector, index_option - 1));
    removeElement(kwords_vector, index_option - 1);
    return true;
}

bool saveKeywords(char *config_filename, StringVector *kwords_vector)
{
    // Recorremos el vector dinamico para crear la cadena final de palabras clave, separadas por comas, para guardar en el archivo de configuracion.
    int size = getSize(kwords_vector);
    char final_kwords[CONFIG_LENGTH] = "";
    char *word;
    if (size > 0)
    {
        word = getElement(kwords_vector, 0);
        strcat(final_kwords, word);
        free(word);
        for (size_t i = 1; i < size; i++)
        {
            word = getElement(kwords_vector, i);
            strcat(final_kwords, ",");
            strcat(final_kwords, word);
            free(word);
        }
    }

    // Guardamos la cadena final de palabras clave en el archivo de configuracion, bajo la llave "keywords"
    updateConfig(config_filename, "keywords", final_kwords);
    printf("Palabras clave actualizadas.\n");
    return true;
}

// Manejo del Webhook

bool loadURL(char *config_filename)
{
    char *current_URL = readValueFromKey(config_filename, "url");
    bool validURL = current_URL == NULL;

    printf("La URL actual del Webhook es: %s\n", validURL ? current_URL : "Ninguna");
    free(current_URL);
    return current_URL;
}

bool modifyWebhook(char *config_filename)
{
    char new_URL[INPUT_LENGTH] = {0};
    printf("Ingrese la nueva URL del Webhook:\n");

    // Leer a nueva URL, validar que no contenga el delimitador de nueva linea, si la entrada es vacia, cancelar la operacion.
    if (fgets(new_URL, sizeof(new_URL), stdin) != NULL)
    {
        new_URL[strcspn(new_URL, "\n")] = 0;
        if (strchr(new_URL, '=') != NULL)
        {
            printf("URL invalida. Cree un nuevo Webhook que no contenga el caracter '='.\n");
            return false;
        }
        if (strlen(new_URL) > 0)
        {
            updateConfig(config_filename, "url", new_URL);
            printf("URL del Webhook actualizada.\n");
        }
        return true;
    }
    return false;
}

bool toggleWebhook(char *config_filename)
{
    int new_status = -1;
    char *current_status = readValueFromKey(config_filename, "enabled");
    printf("El Webhook actualmente esta: %s\n", (current_status != NULL && strcmp(current_status, "1") == 0) ? "Activado" : "Desactivado");
    free(current_status);

    printf("Ingrese 1 para activar o 0 para desactivar el Webhook: ");
    if (scanf("%d", &new_status) > 1 && !(new_status == 1 || new_status == 0))
    {
        getchar();
        printf(INVALID_MESSAGE);
        return false;
    }
    getchar();
    updateConfig(config_filename, "enabled", (new_status == 1) ? "1" : "0");
    printf("El Webhook ha sido %s.\n", (new_status == 1) ? "activado" : "desactivado");
    return true;
}

// Manejo del funcionamiento principal

bool verifyStructure(char *root_dir, char *logs_dir, char *config_filename)
{
    // Verificar directorio base.
    if (!verifyDirExists(root_dir))
    {
        printf("No se encontro el directorio principal.\n");
        printf("Ingresa al menu de Opciones para crearlo.\n");
        return false;
    };

    // Verifica directorio de bitacoras
    snprintf(logs_dir, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, LOGS_DIRNAME);
    if (!verifyDirExists(logs_dir))
    {
        printf("No se encontro el directorio de bitacoras.\n");
        printf("Ingresa al menu de Opciones para crearlo.\n");
        return false;
    };

    // Verifica archivo de configuracion
    snprintf(config_filename, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, CONFIG_FILENAME);
    if (!verifyFileExists(config_filename))
    {
        printf("No se encontro el archivo de configuracion.\n");
        printf("Ingresa al menu de Opciones para crearlo.\n");
        return false;
    }

    // Valida que el archivo de configuracion tenga el formato adecuado
    if (!validateConfigFile(config_filename))
    {
        printf("El archivo de configuracion esta corrupto.\n");
        printf("Ingresa al menu de Opciones para ingresar los datos necesarios.\n");
        return false;
    };

    return true;
}

// WIP
bool readOptions(char *config_filename, StringVector *kwords_vector)
{
    // Copia cada valor de cada clave a un arreglo local.
    char keys[OPTIONS_COUNT][OPTIONS_LENGTH] = OPTIONS;
    char options[OPTIONS_COUNT][INPUT_LENGTH];
    for (size_t i = 0; i < OPTIONS_COUNT; i++)
    {
        strcpy(options[i], readValueFromKey(config_filename, keys[i]));
    }

    return true;
}

// Manejo de las opciones

bool optionsInitializer(char *root_dir, char *logs_dir, char *config_filename)
{
    int file_result = 0;
    // Inicializar directorio base
    file_result = createDir(root_dir, 0755);
    logDirOrFile(file_result, root_dir, "d");
    if (file_result == 1)
        return false;

    // Inicializar directorio de bitacoras
    snprintf(logs_dir, INPUT_LENGTH, "%s/%s", MAIN_DIRNAME, LOGS_DIRNAME);
    file_result = createDir(logs_dir, 0755);
    logDirOrFile(file_result, logs_dir, "d");
    if (file_result == 1)
        return false;

    // Inicializar archivo de configuracion
    snprintf(config_filename, INPUT_LENGTH, "%s/%s", MAIN_DIRNAME, CONFIG_FILENAME);
    file_result = createFile(config_filename);
    logDirOrFile(file_result, config_filename, "f");
    if (file_result == 1)
        return false;
}

int main(int argc, char *argv[])
{
    // Variables de Archivos
    char *root_dir = MAIN_DIRNAME;
    char logs_dir[INPUT_LENGTH] = {0};
    char config_filename[INPUT_LENGTH] = {0};
    StringVector *kwords_vector = createVector();

    int actions_input = 0;
    bool exit_condition = false;
    drawLogo();
    printf("Bienvenido a Remos, un programa para bitacoras en la terminal.\n");
    do
    {
        drawMenu();
        inputValidation(actions_input);
        switch (actions_input)
        {
        case 1: // Iniciar
            if (!verifyStructure(root_dir, logs_dir, config_filename))
                break;

            // TODO Obtener parametros de palabras, bitacora, URL y activacion, alerta de sobreescritura, recuadro de confirmacion, ingresar comando a ejecutar, agregar encabezados, leer linea de comandos, almacenar cada linea detectada con hora e indice, enviar cada linea mediante el Webhook.
            // WIP
            readOptions(config_filename, kwords_vector);
            break;
        case 2: // Opciones
            optionsInitializer(root_dir, logs_dir, config_filename);
            int options_input = 0;
            do
            {
                drawOptions();
                inputValidation(options_input);
                switch (options_input)
                {
                case 1:
                    int kword_option = 0;
                    bool kword_operation = false;
                    loadKeywords(config_filename, kwords_vector);
                    do
                    {
                        drawKeywordsMenu();
                        drawKeywords(kwords_vector);
                        inputValidation(kword_option);
                        switch (kword_option)
                        {
                        case 1:
                            do
                            {
                                kword_operation = addKeyword(kwords_vector);
                            } while (!kword_operation);
                            break;
                        case 2:
                            do
                            {
                                kword_operation = modifyKeyword(kwords_vector);
                            } while (!kword_operation);
                            break;
                        case 3:
                            do
                            {
                                kword_operation = deleteKeyword(kwords_vector);
                            } while (!kword_operation);
                            break;
                        case 4:
                            saveKeywords(config_filename, kwords_vector);
                            break;
                        default:
                            printf(INVALID_MESSAGE);
                            break;
                        }
                    } while (kword_option != 4);
                    break;
                case 2:
                    bool log_valid = false;
                    readLogName(config_filename); // To Implement
                    do
                    {
                        log_valid = updateLogName(config_filename); // To Implement
                    } while (!log_valid);
                    // Leer nombre del archivo o cargar por defecto `log`.txt, preguntar si desea modificar, dejar vacio en caso de que no, validar que no contenga =
                    break;
                case 3:
                    int webhook_option = 0;
                    do
                    {
                        drawWebhookMenu();
                        inputValidation(webhook_option);
                        switch (webhook_option)
                        {
                        case 1: // Modificar Webhook
                            loadURL(config_filename);
                            modifyWebhook(config_filename);
                            break;
                        case 2: // Activar\Desactivar Webhook
                            toggleWebhook(config_filename);
                            break;
                        case 3: // Regresar
                            break;
                        default: // Invalido
                            printf(INVALID_MESSAGE);
                            break;
                        }
                    } while (webhook_option != 3);
                    break;
                case 4:
                    return true;
                default:
                    printf(INVALID_MESSAGE);
                    break;
                }
            } while (options_input != 4);
            break;
        case 3: // Salir
            freeVector(kwords_vector);
            return 0;
        default: // Invalido
            printf(INVALID_MESSAGE);
            break;
        }
    } while (actions_input != 3);

    return -1;
}
