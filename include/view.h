#ifndef VIEW_H
#define VIEW_H

#include "models.h"

/**
 * Função para imprimir a lista de filmes
 * @param list: ponteiro para a lista de filmes a ser impressa
 */
void view_print_movie_list(const movie_list_t *list);

/**
 * Função para imprimir uma mensagem de erro
 * @param message: ponteiro para a mensagem de erro a ser impressa
 */
void view_print_error(const char *message);

#endif
