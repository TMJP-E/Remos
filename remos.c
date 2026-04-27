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
 *   Crear estructura de archivos
 *   Leer archivo de opciones
 *   (Agregar | Modificar | Eliminar) palabras clave
 *   Nombrar bitacora
 *   (Agregar | Modificar | Eliminar) URL
 *   Desactivar Webhook
 * Iniciar ejecucion
 *   Verificar archivo de opciones
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
                 "\t3. Regresar\n";

    printf("%s", menu);
}

/**
 * @brief Verifica y crea un directorio.
 *
 * @param path Direccion y nombre del directorio.
 *
 * @return Confirmacion del estado del directorio. `-1` indica que ya existe el directorio en la direccion y nombre especificados, `0` indica que se creo el directorio, `1` indica que no se pudo crear el directorio.
 */
int createDir(const char *path, unsigned int mode)
{
    struct stat filebuf;
    // Si el directorio ya existe, carga el directorio en el buffer usando `stat()`
    if (stat(path, &filebuf) == 0)
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
    if (temporal_pointer = fopen(filename, "r") != NULL)
    {
        fclose(temporal_pointer);
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
void logDirOrFile(int result, char *type)
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
        printf("El %s ya existe.", message);
        break;
    case 0:
        printf("El %s fue creado.", message);
        break;
    default:
        printf("No pudo crearse el %s.", message);
        break;
    }
}

/* DEPRECATED
// Funcion para inicializar los archivos de configuracion.
void initializer(char config_filename[], char wordlist_filename[], char logs_filename[])
{
    // Detecta o crea el directorio para la configuracion y bitacoras.
    char *dir_name = "remos";
    struct stat statbuf;
    if (stat(dir_name, &statbuf) != 0)
    {
        printf("Directorio de configuracion creado.");
        mkdir(dir_name, 0755);
    }
    else
    {
        printf("Directorio de configuracion ya existente.");
    }

    char config_filename_local[17] = "remos/";
    strcat(config_filename_local, config_filename);
    char wordlist_filename_local[16] = "remos/";
    strcat(wordlist_filename_local, wordlist_filename);
    char logs_name[64] = {0};


    FILE *config_file = fopen(config_filename_local, "r");
    if (config_file == NULL)
    {
        config_file = fopen(config_filename_local, "w");
        fprintf(config_file, "%s", logs_filename);
    }
    else
    {
        fgets(logs_name, 64, config_file);
        int length = strlen(logs_name);
        for (size_t i = 0; i < length; i++)
        {
            logs_filename[i] = logs_name[i];
        }
    }
    fclose(config_file);

    // Crea la lista de palabras por defecto
    const char *default_keywords = "error,warn,crash,note";
    FILE *wordlist_file = fopen(wordlist_filename_local, "r");
    if (wordlist_file == NULL)
    {
        wordlist_file = fopen(wordlist_filename_local, "w");
        fprintf(wordlist_file, "%s", default_keywords);
    }
    fclose(wordlist_file);
}
*/

// Funcion de la ejecucion principal.
void start()
{
}

// Funcion para manejar las opciones de nombre de bitacora y lista de palabras.
void options()
{
}

int main(int argc, char *argv[])
{
    /* WIP
    const char *root_dir = MAIN_DIR;
    const char *logs_dir = LOGS_DIR;
    const char *config_file = CONFIG_FILE;
    char aux[32] = {0};

    createDir(root_dir, 0755);

    // Para que el proceso solo dependa de los nombres estaticos, se concatenan las variables.
    strcpy(aux, root_dir);
    strcat(aux, "/");
    strcat(aux, logs_dir);
    createDir(aux, 0755);

    strcpy(aux, root_dir);
    strcat(aux, "/");
    strcat(aux, config_file);
    createFile(aux);
    */

    /* DEPRECATED
    char config_filename[] = "config.cfg";
    char wordlist_filename[] = "words.txt";
    char logs_filename[64] = "logs";

    initializer(config_filename, wordlist_filename, logs_filename);

    int menu_input = 0;
    while (menu_input != 3)
    {
        draw_logo();
        draw_menu();

        scanf("%d", &menu_input);
        switch (menu_input)
        {
        case 1:
            start();
            break;
        case 2:
            options();
            break;
        default:
            break;
        }
    }
    */
    return 0;
}
