#ifndef CLI_H
#define CLI_H

#include "common.h"
#include <stdbool.h>

/**
 * Enumera as categorias validas suportadas pela API do TMDB.
 * CATEGORY_UNKNOWN: Categoria desconhecida ou não especificada.
 * CATEGORY_PLAYING: Filmes atualmente em exibição.
 * CATEGORY_POPULAR: Filmes populares.
 * CATEGORY_TOP: Filmes mais bem avaliados.
 * CATEGORY_UPCOMING: Filmes que serão lançados em breve.
 */
typedef enum {
    CATEGORY_UNKNOWN = 0,
    CATEGORY_PLAYING,
    CATEGORY_POPULAR,
    CATEGORY_TOP,
    CATEGORY_UPCOMING
} tmdb_category_t;

/**
 * Estrutura para armazenar os argumentos de linha de comando.
 * category: Categoria de filmes a ser consultada.
 * help: Flag para indicar se a ajuda deve ser exibida.
 */
typedef struct {
    tmdb_category_t category;
    bool help;
} cli_args_t;

/**
 * Função para analisar os argumentos de linha de comando.
 * @param argc: Número de argumentos.
 * @param argv: Vetor de argumentos.
 * @param out_args: Ponteiro para a estrutura cli_args_t para armazenar os
 * argumentos analisados.
 * @return: Código de status indicando o sucesso ou falha da operação.
 */
status_code_t cli_parse_args(int argc, char **argv, cli_args_t *out_args);

/**
 * Função para imprimir a ajuda do programa.
 * @param prog_name: Nome do programa.
 */
void cli_print_help(const char *prog_name);

#endif
