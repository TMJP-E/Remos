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
 *   (#Agregar | #Modificar | Eliminar) URL
 *   #Desactivar Webhook
 *   -Verificacion Webhook
 * Iniciar ejecucion
 *   #Verificar archivo de opciones
 *   Leer archivo de opciones
 *   2. Desplegar configuracion actual
 *   3. Creacion de bitacora
 *   4. Encabezados de bitacora
 *   Lectura, guardado y conteo de lineas en la terminal
 *   Envio de datos mediante Webhook.
 * # = Completado
 *
 * Se omite hacer mencion a la entrada del usuario como un apartado a desarrollar, ya que es mejor practica manejarla en el flujo principal, no en funciones separadas.
 *
 * Realizado por Joshua Tellez, bajo el equipo Remos, en la asignatura Programacion Estructurada.
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

int main(int argc, char *argv[])
{
    char *root_dir = MAIN_DIRNAME;
    char logs_dir[INPUT_LENGTH] = {0};
    char config_filename[INPUT_LENGTH] = {0};
    int file_result = 0;

    char keys[OPTIONS_COUNT][OPTIONS_LENGTH] = OPTIONS;
    char options[OPTIONS_COUNT][INPUT_LENGTH];

    int input_int = 0;
    char input_char[INPUT_LENGTH] = "";

    drawLogo();
    printf("Bienvenido a Remos, un programa para bitacoras en la terminal.\n\n");
    do
    {
        printf("Ingrese la accion que desea realizar.\n");
        drawMenu();
        scanf("%d", &input_int);
        getchar();

        switch (input_int)
        {
        case 1: // Iniciar
            // Verificar directorio base.
            if (!verifyDirExists(root_dir))
            {
                printf("No se encontro el directorio principal.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            };

            // Verifica directorio de bitacoras
            snprintf(logs_dir, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, LOGS_DIRNAME);
            if (!verifyDirExists(logs_dir))
            {
                printf("No se encontro el directorio de bitacoras.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            };

            // Verifica archivo de configuracion
            snprintf(config_filename, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, CONFIG_FILENAME);
            if (!verifyFileExists(config_filename))
            {
                printf("No se encontro el archivo de configuracion.\n");
                printf("Ingresa al menu de Opciones para crearlo.\n");
                break;
            }

            // Valida que el archivo de configuracion tenga el formato adecuado
            if (!validateConfigFile(config_filename))
            {
                printf("El archivo de configuracion esta corrupto.\n");
                printf("Ingresa al menu de Opciones para ingresar los datos necesarios.\n");
                break;
            };

            // Copia cada valor de cada clave a un arreglo local.
            for (size_t i = 0; i < OPTIONS_COUNT; i++)
            {
                strcpy(options[i], readValueFromKey(config_filename, keys[i]));
            }

            // TODO Obtener parametros de palabras, bitacora, URL y activacion, alerta de sobreescritura, recuadro de confirmacion, ingresar comando a ejecutar, agregar encabezados, leer linea de comandos, almacenar cada linea detectada con hora e indice, enviar cada linea mediante el Webhook.

            break;
        case 2: // Opciones
            // Inicializar directorio base
            file_result = createDir(root_dir, 0755);
            logDirOrFile(file_result, root_dir, "d");
            if (file_result == 1)
                break;

            // Inicializar directorio de bitacoras
            snprintf(logs_dir, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, LOGS_DIRNAME);
            file_result = createDir(logs_dir, 0755);
            logDirOrFile(file_result, logs_dir, "d");
            if (file_result == 1)
                break;

            // Inicializar archivo de configuracion
            snprintf(config_filename, sizeof(logs_dir), "%s/%s", MAIN_DIRNAME, CONFIG_FILENAME);
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
