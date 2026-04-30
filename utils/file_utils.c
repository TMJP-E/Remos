#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "file_utils.h"

/**
 * @brief Verifica si existe el directorio ingresado.
 *
 * @param path Direccion y nombre del directorio.
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
 * @param path Direccion y nombre del archivo.
 *
 * @return Si existe el archivo o no.
 */
bool verifyFileExists(const char *path)
{
    FILE *temporal_pointer = fopen(path, "r");
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
 * @related verifyDirExists()
 */
int createDir(const char *path, unsigned int mode)
{
    // Verifica si el directorio ya existe
    if (verifyDirExists(path))
    {
        return -1;
    }

    // El directorio es creado con `mkdir()` correctamente.
    if (mkdir(path, mode) == 0)
    {
        return 0;
    }

    // Si el directorio no pudo ser creado por cualquier motivo.
    return 1;
}

/**
 * @brief Verifica y crea un archivo.
 *
 * @param path Direccion y nombre del archivo.
 *
 * @return Confirmacion del estado del archivo. `-1` indica que ya existe el archivo con la direccion y nombre especificados, `0` indica que se creo el archivo, `1` indica que no se pudo crear el archivo.
 *
 * @related verifyFileExists()
 */
int createFile(const char *path)
{
    FILE *temporal_pointer;

    // Serifica si el archivo ya existe
    if (verifyFileExists(path))
    {
        return -1;
    }

    // El archivo es creado, utilizando el modo de escritura, ya que no existe opcion exclusiva para creacion.
    if ((temporal_pointer = fopen(path, "w")) != NULL)
    {
        fclose(temporal_pointer);
        return 0;
    }

    // Si el archivo no pudo ser creado por cualquier motivo.
    return 1;
}

/**
 * @brief Despliega el estado de creacion y accesso de archivos y directorios.
 *
 * @details Utiliza el esquema tricotomico de las funciones `createDir()` y `createFile()`
 *
 * @param result Resultado de la operacion, la cual se encuentra especificada exactamente igual que en los directorios y archivos.
 * @param name Nombre del archivo o directorio.
 * @param type Cadena indicando el tipo a registrar, `"d"` para directorios, `"f"` para archivos.
 *
 * @related createDir(), createFile()
 */
void logDirOrFile(int result, char *name, char *type)
{
    // Determina el tipo de dato para registrar.
    char *message;
    if (type == "d")
    {
        message = "directorio";
    }
    else if (type == "f")
    {
        message = "archivo";
    };

    // Estado segun la especificacion tricotomica.
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