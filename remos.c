#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>

//Funcion para inicializar los archivos de configuracion.
void initializer(char config_filename[], char wordlist_filename[], char logs_filename[]) {
    //Detecta o crea el directorio para la configuracion y bitacoras.
    char *dir_name = "remos";
    struct stat statbuf;
    if (stat(dir_name, &statbuf) != 0) {
        mkdir(dir_name, 0755);
    }

    char config_filename_local[17] = "remos/";
    strcat(config_filename_local, config_filename);
    char wordlist_filename_local[16] = "remos/";
    strcat(wordlist_filename_local, wordlist_filename);
    char logs_name[64] = {0};

    /*Abre o crea el archivo de configuraciones y obtiene el nombre de la bitacora.
      Si el archivo no existe, lo crea y guarda el nombre del archivo de bitacora por defecto.
      Si el archivo existe, lee el nombre del archivo de bitacora a usar.*/
    
    FILE *config_file = fopen(config_filename_local, "r");
    if (config_file == NULL) {
        config_file = fopen(config_filename_local, "w");
        fprintf(config_file, "%s", logs_filename);
    } else {
        fgets(logs_name, 64, config_file);
        int length = strlen(logs_name);
        for (size_t i = 0; i < length; i++) {
            logs_filename[i] = logs_name[i];
        }
    }
    fclose(config_file);

    //Crea la lista de palabras por defecto
    const char* default_keywords = "error,warn,crash,note";
    FILE *wordlist_file = fopen(wordlist_filename_local, "r");
    if (wordlist_file == NULL) {
        wordlist_file = fopen(wordlist_filename_local, "w");
        fprintf(wordlist_file, "%s", default_keywords);
    } 
    fclose(wordlist_file);
}

//Funcion para imprimir el logo.
void draw_logo() {
    char* logo = "\n"
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

//Funcion para imprimir el menu principal.
void draw_menu() {
    char* menu = ""
    "\t1. Iniciar\n"
    "\t2. Opciones\n"
    "\t3. Salida\n";
    
    printf("%s", menu);
}

//Funcion de la ejecucion principal.
void start() {
    
}

//Funcion para manejar las opciones de nombre de bitacora y lista de palabras.
void options() {

}

int main(int argc, char *argv[]) {
    char config_filename[] = "config.cfg";
    char wordlist_filename[] = "words.txt";
    char logs_filename[64] = "logs";

    initializer(config_filename, wordlist_filename, logs_filename);

    int menu_input = 0;
    while (menu_input != 3) {
        draw_logo();
        draw_menu();

        scanf("%d", &menu_input);
        switch (menu_input) {
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

    return 0;
}
