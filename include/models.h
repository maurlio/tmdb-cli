#ifndef MODELS_H
#define MODELS_H

/**
 * Estrutura para armazenar informações de um filme
 * title: título do filme
 * release_date: data de lançamento do filme
 * rating: avaliação do filme
 */
typedef struct {
    char title[256];
    double rating;
} movie_t;

/**
 * Estrutura para armazenar uma lista de filmes
 * items: array de filmes
 * count: número de filmes na lista
 */
typedef struct {
    movie_t *items;
    int count;
} movie_list_t;

#endif
