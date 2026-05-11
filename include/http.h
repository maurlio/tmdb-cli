#ifndef HTTP_H
#define HTTP_H

#include "common.h"
#include <stddef.h>

/**
 * Estrutura para armazenar os dados de uma requisição HTTP.
 * data: Ponteiro para os dados da resposta.
 * size: Tamanho dos dados da resposta.
 */
typedef struct {
    char *data;
    size_t size;
} http_response_t;

/**
 * Inicializa o módulo HTTP.
 * @return Código de status da operação.
 */
status_code_t http_init(void);

/**
 * Limpa os recursos do módulo HTTP.
 */
void http_cleanup(void);

/**
 * Realiza uma requisição HTTP GET.
 * @param url URL para a qual a requisição será feita.
 * @param auth_token Token de autenticação (NULL se não for necessário).
 * @param out_response Ponteiro para armazenar a resposta da requisição.
 * @return Código de status da operação.
 */
status_code_t http_get(
    const char *url, const char *auth_token, http_response_t *out_response);

/**
 * Libera os recursos alocados para a resposta HTTP.
 * @param response Ponteiro para a resposta HTTP a ser liberada.
 */
void http_response_free(http_response_t *response);

#endif
