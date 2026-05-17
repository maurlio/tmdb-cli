#ifndef HTTP_H
#define HTTP_H

#include "common.h"
#include <stddef.h>

/**
 * Estrutura para armazenar os dados de uma requisição HTTP.
 */
typedef struct {
    char *data;
    size_t size;
} http_response_t;

/**
 * Inicializa o módulo HTTP globalmente.
 * @return Código de status da operação.
 */
status_code_t http_init(void);

/**
 * Limpa os recursos do módulo HTTP globalmente.
 */
void http_cleanup(void);

/**
 * Realiza uma requisição HTTP GET para a URL especificada.
 * * @param url URL de destino.
 * @param auth_token Token de autenticação (NULL se não necessário).
 * @param out_response Estrutura (pré-alocada na stack) para armazenar os dados.
 * @return Código de status da operação.
 */
status_code_t http_get(
    const char *url, const char *auth_token, http_response_t *out_response);

/**
 * Libera de forma segura a memória alocada no heap para a resposta HTTP.
 * @param response Ponteiro para a estrutura que contém a resposta.
 */
void http_response_free(http_response_t *response);

#endif
