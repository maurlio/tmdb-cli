#ifndef API_H
#define API_H

#include "cli.h"
#include "common.h"
#include "models.h"

/**
 * Função para obter a lista de filmes de uma categoria específica
 * @param category: categoria de filmes a ser obtida
 * @param out_list: ponteiro para a estrutura onde a lista de filmes será
 * armazenada
 * @return: código de status indicando o resultado da operação
 */
status_code_t api_get_movies(tmdb_category_t category, movie_list_t *out_list);

/**
 * Função para liberar a memória alocada para a lista de filmes
 * @param list: ponteiro para a estrutura da lista de filmes a ser liberada
 */
void api_free_movie_list(movie_list_t *list);

#endif
