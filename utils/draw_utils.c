#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

#include "draw_utils.h"
#include "vector.h"

/// @brief Logo de Remos
void drawLogo()
{
    char *logo = ""
                 " ______       ______      ___ __ __      ______      ______      \n"
                 "/_____/\\     /_____/\\    /__//_//_/\\    /_____/\\    /_____/\\     \n"
                 "\\:::_ \\ \\    \\::::_\\/_   \\::\\| \\| \\ \\   \\:::_ \\ \\   \\::::_\\/_    \n"
                 " \\:(_) ) )_   \\:\\/___/\\   \\:.      \\ \\   \\:\\ \\ \\ \\   \\:\\/___/\\   \n"
                 "  \\: __ `\\ \\   \\::___\\/_   \\:.\\-/\\  \\ \\   \\:\\ \\ \\ \\   \\_::._\\:\\  \n"
                 "   \\ \\ `\\ \\ \\   \\:\\____/\\   \\. \\  \\  \\ \\   \\:\\_\\ \\ \\    /____\\:\\ \n"
                 "    \\_\\/ \\_\\/    \\_____\\/    \\__\\/ \\__\\/    \\_____\\/    \\_____\\/ \n\n"
                 "";

    printf("%s", logo);
}

/**
 * @brief Menu Principal
 *
 * 1. Iniciar
 * 2. Opciones
 * 3. Salir
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
 * @brief Menu de Opciones
 *
 * 1. Palabras Clave
 * 2. Bitacora
 * 3. Webhook
 * 4. Regresar
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
 * @brief Menu de Palabras Clave
 *
 * 1. Agregar
 * 2. Modificar
 * 3. Eliminar
 * 4. Guardar y Salir
 */
void drawKeywordsMenu()
{
    char *menu = ""
                 "\n Configuracion de Palabras Clave\n"
                 "\t1. Agregar Palabra Clave\n"
                 "\t2. Modificar Palabra Clave\n"
                 "\t3. Eliminar Palabra Clave\n"
                 "\t4. Guardar y Salir\n";

    printf("%s", menu);
}

/**
 * @brief Menu de Webhook
 *
 * 1. Modificar
 * 2. Activar/Desactivar
 * 3. Regresar
 */
void drawWebhookMenu()
{
    char *menu = ""
                 "\n Configuracion de Webhook\n"
                 "\t1. Modificar URL del Webhook\n"
                 "\t2. Activar/Desactivar Webhook\n"
                 "\t3. Regresar\n";

    printf("%s", menu);
}

/// @brief Despliega todas las palabras clave guardadas en un vector de cadenas.
void drawKeywords(StringVector *kwords_vector)
{
    printf("Palabras clave guardadas: ");
    for (size_t i = 0; i < getSize(kwords_vector) - 1; i++)
    {
        printf("%s, ", getElement(kwords_vector, i));
    }
    printf("%s.\n", getElement(kwords_vector, getSize(kwords_vector) - 1));
}
