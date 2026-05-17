#ifndef CLI_H
#define CLI_H

#include "api.h"
#include "common.h"
#include <stdbool.h>

/**
 * Estrutura para armazenar os argumentos de linha de comando.
 * category: Categoria de filmes a ser consultada (Vem da api.h).
 * has_category: Flag de controle interno do parser.
 * help: Flag para indicar se a ajuda deve ser exibida.
 */
typedef struct {
    tmdb_category_t category;
    bool has_category;
    bool help;
} cli_args_t;

/**
 * Função para analisar os argumentos de linha de comando.
 * @param argc: Número de argumentos.
 * @param argv: Vetor de argumentos.
 * @param out_args: Ponteiro para a estrutura cli_args_t.
 * @return: Código de status indicando o sucesso ou falha.
 */
status_code_t cli_parse_args(int argc, char **argv, cli_args_t *out_args);

/**
 * Função para imprimir a ajuda do programa.
 * @param prog_name: Nome do programa.
 */
void cli_print_help(const char *prog_name);

#endif
