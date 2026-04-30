#include <stdlib.h>
#include <stdio.h>

#include "draw_utils.h"

/**
 * @brief Despliega el logo, de manera estatica.
 */
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
