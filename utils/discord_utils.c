#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "discord_utils.h"
#include "config_utils.h"

/**
 * @def JSON_PAYLOAD_LENGTH_H
 * @brief Tamaño máximo del buffer para el payload JSON.
 * @details 2000 caracteres es el limite de contenido para discord. Tambien lo separé de CONFIG_LENGTH por si por cualquier razón se volvía muy pequeño.
 */
#define JSON_PAYLOAD_LENGTH_H 2048

void sendDiscordAlert(const char *url, const char *log) {
    
    CURL *curl;
    CURLcode res;
    
    char json_payload[JSON_PAYLOAD_LENGTH_H];

    // Construye el JSON, hay varios parámetros que se pueden incluir. como \"username\": \"Remos\"}"
    // Analizar posible caso  en el que el log sea  ->    error: expected ";" before "}"   <-     las comillas lo rompen?
    snprintf(json_payload, sizeof(json_payload), "{\"content\": \"%s\"}", log);

    curl = curl_easy_init();

    if(curl != NULL) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        //Especifica los headers al API destino, por defecto se envía como si fuese datos de formulario web.
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // El envío del request en sí.
        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            //Referirse a la documentación para ver la lista a la que refiere curl_easy_strerror : se puede ejecutar el comando man libcurl-errors.
            fprintf(stderr, "Error Webhook: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}