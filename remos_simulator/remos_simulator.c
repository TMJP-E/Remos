#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAX_MESSAGES 50
#define MAX_MSG_LEN 200

/**
 * @struct thread_data
 * @brief Estructura de comunicación entre el hilo principal y el secundario.
 * 
 * Durante la mayor parte del desarrollo del codigo, esta estructura tuvo varios campos 
 * y fue muy util, la dejare asi por ahora.
 */
struct thread_data {
    int *working;
};

/**
 * @brief Filtra mensajes buscando patrones de error o criticidad.
 * strstr espera un const char.
 */
int isCritical(const char *msg) {
    const char *keywords[] = {
        "Error", "error", "Falla", "falla", "Fallo", "fallo",
        "Timeout", "Critico", "critico", "no se pudo", "No se pudo",
        "No esta", "no esta", "denegado", "Denegado", NULL
    };
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strstr(msg, keywords[i]))
            return 1;
    }
    return 0;
}

void *generateLog(void *voidarg);

int main(void) {
    srand(time(NULL));
    int is_working = 1;

    struct thread_data thread_args;
    thread_args.working = &is_working;

    pthread_t log_thread;
    // Inicia la ejecución del hilo secundario
    pthread_create(&log_thread, NULL, generateLog, (void *)&thread_args);

    char input_exit[10] = "";
    printf("Presiona Enter para detener el programa...\n");
    
    // Espera activa hasta recibir un salto de línea (Enter)
    while (strcmp(input_exit, "\n") != 0) {
        fgets(input_exit, sizeof(input_exit), stdin);
    }
    
    // Notifica al hilo que debe finalizar su ciclo
    is_working = 0;

    // Sincronización: espera a que el hilo cierre archivos y termine
    pthread_join(log_thread, NULL);
    printf("Programa finalizado correctamente.\n");
    return 0;
}

void *generateLog(void *voidarg) {
    struct thread_data *args = (struct thread_data *)voidarg;

    // Crea el archivo bitacora donde pondra mensajes criticos
    FILE *bitacora_file = fopen("bitacora.txt", "w");
    if (bitacora_file == NULL) {
        printf("Error al abrir bitacora.txt");
        return NULL;
    }

    // Abre log_pool.txt en modo lectura. Contiene los mensajes genericos.
    FILE *pool_file = fopen("log_pool.txt", "r");
    if (pool_file == NULL) {
        printf("Error: No se encontró log_pool.txt, por favor reinicie.\n");
        fclose(bitacora_file);
        return NULL;
    }

    char log_array[MAX_MESSAGES][MAX_MSG_LEN];
    int message_count = 0;
    
    // Lectura de mensajes hasta alcanzar el límite o el fin del archivo
    while (message_count < MAX_MESSAGES && fgets(log_array[message_count], MAX_MSG_LEN, pool_file) != NULL) {
        log_array[message_count][strcspn(log_array[message_count], "\r\n")] = '\0';
        message_count++;
    }
    fclose(pool_file);

    int *is_running = args->working;
    
    // Simulación de terminal en ejecución
    if (message_count > 0) { 
        while (*is_running != 0) {
        // Selección aleatoria de un mensaje
        int random_index = rand() % message_count;

        // Genera timestamp
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);
        char formatted_time[20];
        strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", time_info);

        // Imprime en consola
        printf("[%s] %s\n", formatted_time, log_array[random_index]);

        // Evaluación y registro de mensajes críticos en bitacora.txt
        const char *current_msg = log_array[random_index];
        if (isCritical(current_msg)) {
            fprintf(bitacora_file, "[%s] CRITICO: %s\n", formatted_time, current_msg);
        }

        sleep(1);
        }
    } else {
        printf("Error, no se cargaron mensajes.");
    }
    fclose(bitacora_file);
    return NULL;
}