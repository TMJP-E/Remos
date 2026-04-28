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
 *   -Leer archivo de opciones
 *   -(Agregar | Modificar | Eliminar) palabras clave
 *   -Nombrar bitacora
 *   -(Agregar | Modificar | Eliminar) URL
 *   -Desactivar Webhook
 *   -Verificacion Webhook
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
#define TEMP_FILE "config.tmp"

#define INPUT_LENGTH 1024



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
    if ((temporal_pointer = fopen(filename, "r")) != NULL)
    {
        fclose(temporal_pointer);
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

/**
 * @brief Actualiza o agrega un valor en config.cfg en el formato llave=valor.
 *
 * @details Utiliza un archivo temporal para copiar la informacion y luego 
 * reemplazarlo por el original. 
 * Se utilizara tanto para cambiar el nombre de la bitacora, como para 
 * agregar la URL del webhook y activar o desactivar el mismo. Si la llave 
 * existe en el archivo, actualiza su valor; si no existe, la agrega al final.
 *
 * @param key La llave a buscar dentro del archivo de configuracion.
 * @param newValue El nuevo valor que se asignara a la llave especificada.
 */
void updateConfig(const char *key, const char *newValue) {
    char configPath[128];
    char tempPath[128];

    snprintf(configPath, sizeof(configPath), "%s/%s", MAIN_DIR, CONFIG_FILE);
    snprintf(tempPath, sizeof(tempPath), "%s/%s", MAIN_DIR, TEMP_FILE);

    FILE *originalFile = fopen(configPath, "r");
    FILE *tempFile = fopen(tempPath, "w");

    char currentLine[1024];
    char searchKey[256];
    bool keyFound = false;

    // Prepara la cadena para la busqueda en el formato "key="
    snprintf(searchKey, sizeof(searchKey), "%s=", key);
    int keyLength = strlen(searchKey);


    if (originalFile != NULL) {
        while (fgets(currentLine, sizeof(currentLine), originalFile)!=NULL){
            //Comparamos solo hasta la longitud de "key="
            if (strncmp(currentLine, searchKey, keyLength)==0){
                //Si lo encontramos, escribimos la llave y el nuevo valor en el archivo temporal, y activamos keyFound
                fprintf(tempFile, "%s=%s\n", key, newValue);
                keyFound = true;
            } else {
                // Si no es la linea que buscamos, la copiamos tal cual al archivo temporal
                fprintf(tempFile, "%s", currentLine);
            }
        }
        fclose(originalFile);
    }

    //si no se encontro la llave, la agregamos al final del archivo temporal
    if (!keyFound) {
        fprintf(tempFile, "%s=%s\n", key, newValue);
    }

    fclose (tempFile);
    // Reemplazamos el archivo original con el temporal
    remove(configPath);
    rename(tempPath, configPath);
}

//Funcion que actualiza la URL del webhook de Discord en el archivo de configuracion
void updateWebhooklink(){
    char newLink[256];
    printf("Ingrese la nueva URL del webhook de Discord: ");
    if(fgets(newLink, sizeof(newLink), stdin)!=NULL){
        newLink[strcspn(newLink, "\n")] = '\0';
        updateConfig("url", newLink);
        printf("URL del webhook de Discord actualizada a: %s\n", newLink);
    }
    else {
        printf("Error al leer la URL del webhook de Discord.\n");
    }
}

//Funcion cambia el estado del webhook de discord en el archivo de configuracion
void toggleWebhook() {
    int currentStatus;
    printf("Ingrese el nuevo estado del webhook de Discord (1 para activar, 0 para desactivar): ");
    scanf("%d", &currentStatus);
    while (getchar() != '\n');
    if (currentStatus == 1) {
        getchar();
        updateConfig("enabled", "1");
        printf("Webhook de Discord Activado\n");
    } else if (currentStatus == 0) {
        getchar();
        updateConfig("enabled", "0");
        printf("Webhook de Discord Desactivado\n");
    } else {
        printf("Entrada invalida. Por favor ingrese 1 para activar o 0 para desactivar el webhook de Discord.\n");
    }
}


int main(int argc, char *argv[])
{
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
            // TODO Flujo principal del programa
            break;
        case 2: // Opciones
            // Flujo para la inicializacion de archivos
            const char *root_dir = MAIN_DIR;
            const char *logs_dir = LOGS_DIR;
            const char *config_file = CONFIG_FILE;
            char aux[32] = {0};
            int file_result = 0;

            file_result = createDir(root_dir, 0755);
            logDirOrFile(file_result, root_dir, "d");
            if (file_result == 1)
            {
                break;
            }

            // Para que el proceso solo dependa de los nombres estaticos, se concatenan las variables.
            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, logs_dir);
            file_result = createDir(aux, 0755);
            logDirOrFile(file_result, aux, "d");
            if (file_result == 1)
            {
                break;
            }

            strcpy(aux, root_dir);
            strcat(aux, "/");
            strcat(aux, config_file);
            file_result = createFile(aux);
            logDirOrFile(file_result, aux, "f");
            if (file_result == 1)
            {
                break;
            }

            do
            {
                printf("Ingrese la accion que desea realizar.\n");
                drawOptions();
                scanf("%d", &input_int);
                getchar();

                switch (input_int)
                {
                case 1: // Palabaras clave
                    // TODO (Agregar | Modificar | Eliminar) Palabras
                    break;
                case 2: // Bitacora
                    // TODO (Nombrar | Modificar) Bitacora
                    break;
                case 3: // Webhook
                    // TODO (Agregar | Modificar | Eliminar) URL, Toggle de Webhook
                    break;
                default:
                    break;
                }
            } while (input_int != 4);
        }
    } while (input_int != 3);

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
