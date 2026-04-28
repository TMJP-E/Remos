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
 *   (#Agregar | #Modificar | Eliminar) palabras clave
 *   #Nombrar bitacora
 *   #(Agregar | Modificar) URL
 *   #Desactivar Webhook
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
#define TEMP_FILE "temp.tmp"
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
    struct stat file_buf;
    return (stat(path, &file_buf) == 0);
}

/**
 * @brief Verifica si existe el archivo ingresado.
 *
 * @param filename Direccion y nombre del directorio
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
 * @param mode Bytes de permisos en el formato UNIX `-rwxrwxrwx`.
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
 * @param filename Direccion y nombre del archivo.
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
    if ((temporal_pointer = fopen(filename, "w")) != NULL)
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
 * @param name Nombre del archivo o directorio.
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
 * @details Lee cada linea por separado, y garantiza que se encuentren las claves de los pares clave-valor definidos en la especificacion, no valida los contenidos al ser dinamicos
 *
 * @param config_path La direccion del archivo de configuracion a validar.
 *
 * @return Si tiene un formato valido o no, esto es, que se encuentren todas las claves
 */
bool validateConfigFile(char *config_path)
{
    char keys[KEYS_COUNT][10] = {"keywords", "log", "url", "enabled"};
    char config_string[CONFIG_LENGTH] = {0};
    char *key_delimiter = "=";
    char *token;
    bool valid = true;
    bool checked[KEYS_COUNT] = {false};

    // Abre el archivo con la direccion especificada y comprueba si existe.
    FILE *config_file = fopen(config_path, "r");
    if (config_file == NULL)
    {
        return false;
    }

    // Verifica que se encuentren las cuatro claves, sin importar el orden o claves adicionales.
    while (fgets(config_string, CONFIG_LENGTH, config_file) != NULL)
    {
        token = strtok(config_string, key_delimiter);
        for (size_t i = 0; i < KEYS_COUNT; i++)
        {
            if (strcmp(token, keys[i]) == 0 && !checked[i])
            {
                checked[i] = true;
                break;
            }
        }
    }

    // Valida que se encuentren todas las claves.
    for (size_t i = 0; i < KEYS_COUNT; i++)
    {
        valid = checked[i] && valid;
    }

    return valid;
}

/**
 * @brief Actualiza o agrega un valor en config.cfg en el formato clave=valor.
 *
 * @details Utiliza un archivo temporal para copiar la informacion y luego
 * reemplazarlo por el original.
 * Se utilizara tanto para cambiar el nombre de la bitacora, como para
 * agregar la URL del webhook y activar o desactivar el mismo. Si la clave
 * existe en el archivo, actualiza su valor; si no existe, la agrega al final.
 *
 * @param config_path La direccion del archivo de configuracion.
 * @param key La clave a buscar dentro del archivo de configuracion.
 * @param new_value El nuevo valor que se asignara a la clave especificada.
 */
void updateConfig(char *config_path, const char *key, const char *new_value)
{
    char temp_path[128];

    snprintf(temp_path, sizeof(temp_path), "%s/%s", MAIN_DIR, TEMP_FILE);

    FILE *original_file = fopen(config_path, "r");
    FILE *temp_file = fopen(temp_path, "w");

    char current_line[CONFIG_LENGTH];
    char search_key[INPUT_LENGTH];
    bool key_found = false;

    // Prepara la cadena para la busqueda en el formato "key="
    snprintf(search_key, sizeof(search_key), "%s=", key);
    int keyLength = strlen(search_key);

    if (original_file != NULL)
    {
        while (fgets(current_line, sizeof(current_line), original_file) != NULL)
        {
            // Comparamos solo hasta la longitud de "key="
            if (strncmp(current_line, search_key, keyLength) == 0)
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
    }

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

char **readParameterContents()
{
}

int main(int argc, char *argv[])
{
    const char *root_dir = MAIN_DIR;
    char logs_dir[32] = {0};
    char config_filename[32] = {0};
    int file_result = 0;

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

            snprintf(logs_dir, sizeof(logs_dir), "%s/%s", MAIN_DIR, LOGS_DIR);
            if (!verifyDirExists(logs_dir))
            {
                printf("No se encontro el directorio de bitacoras.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            };

            snprintf(config_filename, sizeof(logs_dir), "%s/%s", MAIN_DIR, CONFIG_FILE);
            if (!verifyFileExists(config_filename))
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
            snprintf(logs_dir, sizeof(logs_dir), "%s/%s", MAIN_DIR, LOGS_DIR);
            file_result = createDir(logs_dir, 0755);
            logDirOrFile(file_result, logs_dir, "d");
            if (file_result == 1)
                break;

            snprintf(config_filename, sizeof(logs_dir), "%s/%s", MAIN_DIR, CONFIG_FILE);
            file_result = createFile(config_filename);
            logDirOrFile(file_result, config_filename, "f");
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
                    // TODO (Agregar | Leer | Modificar | Eliminar) Palabras, Validar que las entradas no posean el delimitador de coma.

                    break;
                case 2: // Bitacora
                    // TODO (Crear | Modificar | Leer) Bitacora, Validar que el nombre no contenga el delimitador de igual.
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
