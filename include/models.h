#ifndef MODELS_H
#define MODELS_H

#include "common.h"

/**
 * Estrutura para armazenar informações de um filme.
 */
typedef struct {
    int id;
    char title[256];
    char release_date[16];
    double rating;
} movie_t;

/**
 * Estrutura para armazenar uma lista de filmes (Retorno da API).
 */
typedef struct {
    movie_t *items;
    size_t count;
} movie_list_t;

/**
 * Libera de forma segura a memória alocada para uma lista de filmes.
 * * @param list Ponteiro para a estrutura da lista.
 */
void movie_list_free(movie_list_t *list);

#endif
