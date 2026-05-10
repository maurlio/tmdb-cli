#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Códigos genéricos de retorno da aplicação.
 * SUCCESS: Operação bem-sucedida.
 * ERROR_GENERIC: Erro genérico.
 * ERROR_MEMORY: Erro relacionado à alocação de memória.
 * ERROR_NETWORK: Erro relacionado à rede.
 * ERROR_INVALID_ARGS: Erro relacionado a argumentos inválidos.
 */
typedef enum {
    SUCCESS = 0,
    ERROR_GENERIC = 1,
    ERROR_MEMORY = 2,
    ERROR_NETWORK = 3,
    ERROR_INVALID_ARGS = 4,
} status_code_t;

/**
 * Macro para log de erros.
 */
#define LOG_ERROR(...)                                                         \
    do {                                                                       \
        fprintf(stderr, "Erro: " __VA_ARGS__);                                 \
        fprintf(stderr, "\n");                                                 \
    } while (0)

#endif
