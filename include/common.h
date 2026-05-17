#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Macro genérica para suprimir warnings de 'unused parameter'.
 */
#define UNUSED(x) (void)(x)

/**
 * Códigos genéricos de retorno da aplicação.
 */
typedef enum {
    SUCCESS = 0,
    ERROR_GENERIC = 1,
    ERROR_MEMORY = 2,
    ERROR_NETWORK = 3,
    ERROR_INVALID_ARGS = 4,
    ERROR_PARSE = 5,
    ERROR_NOT_FOUND = 6
} status_code_t;

/**
 * Macros para logging de mensagens de erro e informação.
 * Utilizam fprintf para enviar mensagens para stderr e stdout, respectivamente.
 */
#define LOG_ERROR(...)                                                         \
    do {                                                                       \
        fprintf(stderr, "[ERROR] ");                                           \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
    } while (0)

#define LOG_INFO(...)                                                          \
    do {                                                                       \
        fprintf(stdout, "[INFO] ");                                            \
        fprintf(stdout, __VA_ARGS__);                                          \
        fprintf(stdout, "\n");                                                 \
    } while (0)

#endif
