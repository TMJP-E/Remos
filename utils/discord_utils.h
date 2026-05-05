#ifndef DISCORD_UTILS_H
#define DISCORD_UTILS_H

/**
 * @file discord_utils.h
 * @brief Funciones de utilidad para la integración con el API de Discord. Actualmente solo incluye sendDiscordAlert
 */

/// @brief Envía una cadena de texto (log) a un Webhook de Discord mediante HTTP POST.
void sendDiscordAlert(const char *url, const char *log);

#endif