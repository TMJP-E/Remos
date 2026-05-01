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
                    // TODO (Modificar | Eliminar) Palabras, Validar que las entradas no posean el delimitador de coma.
                    {
                        //Cargamos todas las palabras clave, en la memoria, en un vector dinamico
                        StringVector *kwords_vector = createVector();
                        char *current_kwords = readValueFromKey(config_filename, "keywords");

                        if (current_kwords != NULL && strlen(current_kwords)>0)
                        {
                            //Creamos copia de la cadena original para no modificarla con strtok
                            char temp_kword_str[CONFIG_LENGTH];
                            strcpy(temp_kword_str, current_kwords);
                            char *token = strtok(temp_kword_str, ",");
                            while (token != NULL)
                            {
                                pushElement(kwords_vector, strdup(token));
                                token = strtok(NULL, ",");
                            }
                        }
                        int kword_option = 0;
                        do
                        {
                            
                            printf("\n Configuracion de Palabras Clave \n");
                            printf("\t1. Agregar Palabra Clave \n");
                            printf("\t2. Modificar Palabra Clave \n");
                            printf("\t3. Eliminar Palabra Clave \n");
                            printf("\t4. Guardar y Salir \n");
                            printf("Ingrese la accion que desea realizar.\n");
                            if (scanf("%d", &kword_option)==1)
                            {
                                getchar();
                                if (kword_option == 1)
                                {
                                    char new_kword[INPUT_LENGTH] = "";
                                    //Este modo se repetira hasta que el usuario decida salir, este booleano es la bandera para controlar eso.
                                    bool add_more = true;
                                    printf("Ingrese la nueva palabra clave (o presiona Enter para cancelar):\n");
                                    while (add_more)
                                    {
                                        printf("\nNueva palabra: ");
                                        if (fgets(new_kword, sizeof(new_kword), stdin) != NULL)
                                        {
                                            //Validamos que la nueva palabra clave no contenga el delimitador de nueva linea, coma o igual
                                            new_kword[strcspn(new_kword, "\n")] = 0;
                                            if (strlen(new_kword) == 0)
                                            {
                                                printf("Saliendo del modo agregar...\n");
                                                add_more = false; //Apagamos la bandera para salir del modo agregar si la entrada es vacia
                                            }
                                            else if (strchr(new_kword, ',')!= NULL || strchr(new_kword,'=') != NULL)
                                            {
                                                printf("Entrada Invalida. Las palabras clave no pueden contener los caracteres ',' o '='.\n");
                                            }
                                            else
                                            {
                                                //Agregamos la nueva palabra clave al vector dinamico
                                                pushElement(kwords_vector, strdup(new_kword));
                                                printf("Palabra clave agregada exitosamente.\n");
                                            }
                                        }
                                        else {
                                            //si algo falla en la lectura de la entrada nos salimos
                                            add_more = false;
                                        }
                                    }
                                }
                                else if (kword_option == 2)
                                {
                                    //TODO
                                }
                                else if (kword_option == 3)
                                {
                                    //TODO
                                }
                                else if (kword_option != 4)
                                {
                                    printf("Entrada Invalida. Por favor ingrese un numero correspondiente a la accion que desea realizar.\n");
                                }
                            }
                            else {
                                getchar();
                                printf("Entrada Invalida. Por favor ingrese un numero correspondiente a la accion que desea realizar.\n");
                                kword_option = 0;
                            }
                        } while (kword_option != 4);
                        
                        char final_kwords[CONFIG_LENGTH] = "";
                        //recorremos el vector dinamico para crear la cadeda final de palabras clave, separadas por comas, para guardar en el archivo de configuracion.
                        for (size_t i = 0; i < kwords_vector->size; i++)
                        {
                            char *word = getElement(kwords_vector, i);
                            if (i==0)
                            {
                                strcat(final_kwords, word);
                            } else {
                                strcat(final_kwords, ",");
                                strcat(final_kwords, word);
                            }
                            //liberamos memoria del strdup de cada palabra clave, ya que no se necesita despues de crear la cadena final.
                            free(word);
                        }
                        //Guardamos la cadena final de palabras clave en el archivo de configuracion, bajo la llave "keywords"
                        updateConfig(config_filename, "keywords", final_kwords);
                        printf("Palabras clave actualizadas exitosamente.\n");
                        //Liberamos memoria del vector dinamico
                        if (kwords_vector->data != NULL)
                        {
                            free(kwords_vector->data);
                        }
                        free(kwords_vector);

                        break;
                    }
                case 2: // Bitacora
                    // TODO (Crear | Modificar | Leer) Bitacora, Validar que el nombre no contenga el delimitador de igual.
                    break;
                case 3: // Webhook
                    {
                        int webhook_option = 0;
                        printf("\n Configuracion de Webhook \n");
                        printf("\t1. Modificar URL del Webhook \n");
                        printf("\t2. Activar/Desactivar Webhook \n");
                        printf("Ingrese la accion que desea realizar.\n");
                        if (scanf("%d", &webhook_option)==1)
                        {
                            getchar();
                            //OPCION 1: MODIFICAR URL
                            if (webhook_option == 1)
                            {
                                char new_URL[INPUT_LENGTH] = {0};
                                //Obtener URL actual y mostrarla como referencia para el usuario, si no existe, mostrar "Ninguna"
                                char *current_URL = readValueFromKey(config_filename, "url");
                                printf("La URL actual del Webhook es: %s\n", (current_URL != NULL) ? current_URL : "Ninguna");
                                printf("Ingrese la nueva URL del Webhook (o presiona Enter para cancelar):\n");

                                //Leer a nueva URL, validar que no contenga el delimitador de nueva linea, si la entrada es vacia, cancelar la operacion.
                                if (fgets(new_URL, sizeof(new_URL),stdin) != NULL)
                                {
                                    new_URL[strcspn(new_URL, "\n")] = 0;
                                    if (strlen(new_URL)>0)
                                    {
                                        updateConfig(config_filename,"url", new_URL);
                                        printf("URL del Webhook actualizada exitosamente.\n");

                                    }
                                }
                            }

                            //OPCION 2: ACTIVAR/DESACTIVAR WEBHOOK
                            else if (webhook_option == 2)
                            {
                                int new_status = -1; // iniciamos en -1 para validar la entrada del usuario, ya que solo se aceptan 0 y 1 como entradas validas.
                                //Obtener estado actual del Webhook y mostrarlo como referencia para el usuario, si no existe o es 0, mostrar "Desactivado"
                                char *current_status = readValueFromKey(config_filename, "enabled");
                                printf("El Webhook actualmente esta: %s\n", (current_status != NULL && strcmp(current_status, "1")==0) ? "ACTIVADO" : "DESACTIVADO");
                                printf("Ingrese 1 para activar o 0 para desactivar el Webhook: ");

                                if (scanf("%d", &new_status)==1)
                                {
                                    getchar();
                                    //validar que solo se acepten 0 y 1 como entradas validas, si la entrada es invalida, mostrar un mensaje de error.
                                    if (new_status == 1 || new_status == 0)
                                    {
                                        updateConfig(config_filename,"enabled", (new_status == 1) ? "1" : "0");
                                        printf("El Webhook ha sido %s exitosamente.\n", (new_status == 1) ? "activado" : "desactivado");
                                    } else {
                                        printf("Entrada invalida. Por favor ingrese 1 para activar o 0 para desactivar.\n");
                                    }
                                }
                                else 
                                {
                                    getchar();
                                    printf("Entrada Invalida. Por favor ingrese un numero correspondiente a la accion que desea realizar.\n");
                                }
                             
                            }
                            else 
                            {
                                printf("Entrada Invalida. Por favor ingrese un numero correspondiente a la accion que desea realizar.\n");
                            }

                        }
                        else 
                        {
                            getchar();
                            printf("Entrada Invalida. Por favor ingrese un numero correspondiente a la accion que desea realizar.\n");
                        }

                    
                        break;
                    }
                default:
                    break;
                }
            } while (input_int != 4);
        }
    } while (input_int != 3);

    return 0;
}
