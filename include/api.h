#ifndef API_H
#define API_H

#include "common.h"
#include "models.h"

/**
 * Categorias de filmes suportadas pela API do TMDB.
 */
typedef enum {
    CATEGORY_PLAYING,
    CATEGORY_POPULAR,
    CATEGORY_TOP,
    CATEGORY_UPCOMING
} tmdb_category_t;

/**
 * Função para obter a lista de filmes de uma categoria específica.
 * @param category: categoria de filmes a ser obtida
 * @param out_list: ponteiro para a estrutura onde a lista será armazenada
 * @return: código de status indicando o resultado da operação
 */
status_code_t api_get_movies(tmdb_category_t category, movie_list_t *out_list);

#endif
