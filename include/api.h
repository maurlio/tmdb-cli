#ifndef API_H
#define API_H

#include "cli.h"
#include "common.h"

/**
 * Busca filmes da TMDB com base na categoria e imprime os resultados.
 * @param category A categoria de filmes a ser buscada (ex: CATEGORY_PLAYING,
 * CATEGORY_POPULAR, etc.).
 * @return status_code_t indicando o resultado da operação (SUCCESS,
 * ERROR_GENERIC, etc.).
 */
status_code_t api_fetch_and_print_movies(tmdb_category_t category);

#endif
