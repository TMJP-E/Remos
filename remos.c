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
 *   -Leer archivo de opciones
 *   -(Agregar | Modificar | Eliminar) palabras clave
 *   -Nombrar bitacora
 *   -(Agregar | Modificar | Eliminar) URL
 *   -Desactivar Webhook
 *   -Verificacion Webhook
 * Iniciar ejecucion
 *   #Verificar archivo de opciones
 *   Desplegar configuracion actual
 *   Creacion de bitacora
 *   Encabezados de bitacora
 *   Lectura, guardado y conteo de lineas en la terminal
 *   Envio de datos mediante Webhook.
 * # = Completado
 *
 * Se omite hacer mencion a la entrada del usuario como un apartado a desarrollar, ya que es mejor practica manejarla en el flujo principal, no en funciones separadas.
 *
 * Realizado por Joshua Tellez, bajo el equipo Remos, en la asignatura Programacion Estructurada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAIN_DIR "remos"
#define LOGS_DIR "logs"
#define CONFIG_FILE "config.cfg"
#define CONFIG_LENGTH 4096
#define INPUT_LENGTH 1024
#define KEYS_COUNT 4

/**
 * @brief Despliega el logo, de manera estatica.
 */
void drawLogo()
{
    char *logo = "\n"
                 " ______       ______      ___ __ __      ______      ______      \n"
                 "/_____/\\     /_____/\\    /__//_//_/\\    /_____/\\    /_____/\\     \n"
                 "\\:::_ \\ \\    \\::::_\\/_   \\::\\| \\| \\ \\   \\:::_ \\ \\   \\::::_\\/_    \n"
                 " \\:(_) ) )_   \\:\\/___/\\   \\:.      \\ \\   \\:\\ \\ \\ \\   \\:\\/___/\\   \n"
                 "  \\: __ `\\ \\   \\::___\\/_   \\:.\\-/\\  \\ \\   \\:\\ \\ \\ \\   \\_::._\\:\\  \n"
                 "   \\ \\ `\\ \\ \\   \\:\\____/\\   \\. \\  \\  \\ \\   \\:\\_\\ \\ \\    /____\\:\\ \n"
                 "    \\_\\/ \\_\\/    \\_____\\/    \\__\\/ \\__\\/    \\_____\\/    \\_____\\/ \n"
                 "\n";

    printf("%s", logo);
}

/**
 * @brief Despliega el menu principal con tres opciones.
 */
void drawMenu()
{
    char *menu = ""
                 "\t1. Iniciar\n"
                 "\t2. Opciones\n"
                 "\t3. Salida\n";

    printf("%s", menu);
}

/**
 * @brief Despliega el menu de opciones.
 */
void drawOptions()
{
    char *menu = ""
                 "\t1. Palabras clave\n"
                 "\t2. Bitacora\n"
                 "\t3. Webhook\n"
                 "\t4. Regresar\n";

    printf("%s", menu);
}

/**
 * @brief Verifica si existe el directorio ingresado.
 *
 * @param path Direccion y nombre del directorio
 *
 * @return Si existe el directorio o no.
 */
bool verifyDirExists(const char *path)
{
    struct stat filebuf;
    return (stat(path, &filebuf) == 0);
}

/**
 * @brief Verifica si existe el archivo ingresado.
 *
 * @param path Direccion y nombre del directorio
 *
 * @return Si existe el archivo o no.
 */
bool verifyFileExists(const char *filename)
{
    FILE *temporal_pointer = open(filename, "r");
    bool exists = temporal_pointer != NULL;
    fclose(temporal_pointer);
    return exists;
}

/**
 * @brief Verifica y crea un directorio.
 *
 * @param path Direccion y nombre del directorio.
 *
 * @return Confirmacion del estado del directorio. `-1` indica que ya existe el directorio en la direccion y nombre especificados, `0` indica que se creo el directorio, `1` indica que no se pudo crear el directorio.
 *
 * @see verifyDir(), verifyFile()
 */
int createDir(const char *path, unsigned int mode)
{

    if (verifyDirExists(path))
    {
        return -1;
    }

    // Si el directorio es creado con `mkdir()`, el octal de permisos es -rwxr-xr-x
    if (mkdir(path, mode) == 0)
    {
        return 0;
    }

    // Si el directorio no pudo ser creado
    return 1;
}

/**
 * @brief Verifica y crea un archivo.
 *
 * @param path Direccion y nombre del archivo.
 *
 * @return Confirmacion del estado del archivo. `-1` indica que ya existe el archivo con la direccion y nombre especificados, `0` indica que se creo el archivo, `1` indica que no se pudo crear el archivo.
 */
int createFile(const char *filename)
{
    FILE *temporal_pointer;

    // Si el archivo ya existe
    if (verifyFileExists(filename))
    {
        return -1;
    }

    // Si el archivo es creado, utilizando el modo de escritura, ya que no existe opcion exclusiva para creacion
    if (temporal_pointer = fopen(filename, "w") != NULL)
    {
        fclose(temporal_pointer);
        return 0;
    }

    // Si el archivo no pudo ser creado
    return 1;
}

/**
 * @brief Despliega el estado de creacion y accesso de archivos y directorios.
 *
 * @param result El resultado de la operacion, la cual se encuentra especificada exactamente igual en directorios y archivos.
 * @param type Cadena indicando el tipo a registrar, `"d"` para directorios, `"f"` para archivos.
 */
void logDirOrFile(int result, char *name, char *type)
{
    char *message;
    if (type == "d")
    {
        message = "directorio";
    }
    else if (type == "f")
    {
        message = "archivo";
    };

    switch (result)
    {
    case -1:
        printf("El %s `%s` ya existe.", message, name);
        break;
    case 0:
        printf("El %s `%s` fue creado.", message, name);
        break;
    default:
        printf("No pudo crearse el %s `%s`.", message, name);
        break;
    }
}

/**
 * @brief Realiza una validacion en el archivo de configuracion para que tenga un formato acorde a la especficacion.
 *
 * @details Lee cada linea por separado, y garantiza que se encuentren las claves de los pares clave-valor definidos en la especificacion,
 *
 * @param config_file El archivo de configuracion a validar.
 *
 * @return Si tiene un formato valido o no.
 */
bool validateConfigFile(FILE *config_file)
{
    bool valid = false;
    int i = 0;
    char config_string[CONFIG_LENGTH] = {0};
    char keys[KEYS_COUNT][10] = {"keywords", "log", "url", "enabled"};
    char *key_delimiter = "=";
    char *token;

    while (fgets(config_string, CONFIG_LENGTH, config_file) != NULL)
    {
        token = strtok(config_string, key_delimiter);
        if (strcmp(token, keys[i]) == 0)
        {
            valid = true;
            i++;
        }
    }

    return valid;
}

int main(int argc, char *argv[])
{
    const char *root_dir = MAIN_DIR;
    const char *logs_dir = LOGS_DIR;
    const char *config_file = CONFIG_FILE;
    int file_result = 0;
    char aux[32] = {0};

    int input_int = 0;
    char input_char[INPUT_LENGTH] = "";
    drawLogo();
    printf("\n\nBienvenido a Remos, un programa para bitacoras en la terminal.\n\n");
    do
    {
        printf("Ingrese la accion que desea realizar.\n");
        drawMenu();
        scanf("%d", &input_int);
        getchar();

        switch (input_int)
        {
        case 1: // Iniciar
            // Verificar estructura de archivos existente
            if (!verifyDirExists(root_dir))
            {
                printf("No se encontro el directorio principal.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            };

            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, logs_dir);
            if (!verifyDirExists(aux))
            {
                printf("No se encontro el directorio de bitacoras.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            };

            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, config_file);
            if (!verifyFileExists(aux))
            {
                printf("No se encontro el archivo de configuracion.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            }

            // TODO Validar archivo de configuracion, obtener parametros de palabras, bitacora, URL y activacion, alerta de sobreescritura, recuadro de confirmacion, ingresar comando a ejecutar, agregar encabezados, leer linea de comandos, almacenar cada linea detectada con hora e indice, enviar cada linea mediante el Webhook.

            break;
        case 2: // Opciones
            // Inicializacion de archivos
            file_result = createDir(root_dir, 0755);
            logDirOrFile(file_result, root_dir, "d");
            if (file_result == 1)
                break;

            // Para que el proceso solo dependa de los nombres estaticos, se concatenan las variables.
            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, logs_dir);
            file_result = createDir(aux, 0755);
            logDirOrFile(file_result, aux, "d");
            if (file_result == 1)
                break;

            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, config_file);
            file_result = createFile(aux);
            logDirOrFile(file_result, aux, "f");
            if (file_result == 1)
                break;

            do
            {
                printf("Ingrese la accion que desea realizar.\n");
                drawOptions();
                scanf("%d", &input_int);
                getchar();

                switch (input_int)
                {
                case 1: // Palabaras clave, ninguna entrada puede poseer el delimitador de nueva linea.
                    // TODO (Agregar | Modificar | Eliminar) Palabras, Validar que las entradas no posean el delimitador de coma.
                    break;
                case 2: // Bitacora
                    // TODO (Nombrar | Modificar) Bitacora, Validar que el nombre no contenga el delimitador de igual.
                    break;
                case 3: // Webhook
                    // TODO (Agregar | Modificar | Eliminar) URL, Toggle de Webhook, Validar que el nombre no contenga el delimitador de igual.
                    break;
                default:
                    break;
                }
            } while (input_int != 4);
        }
    } while (input_int != 3);

    return 0;
}
