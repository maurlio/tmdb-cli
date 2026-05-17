#ifndef VIEW_H
#define VIEW_H

#include "models.h"

/**
 * Imprime no formato padrão (stdout) a lista de filmes.
 * @param list: ponteiro constante para a lista de filmes a ser impressa.
 */
void view_print_movie_list(const movie_list_t *list);

/**
 * Imprime uma mensagem de erro na saída de erro padrão (stderr).
 * @param message: ponteiro para a string contendo a mensagem.
 */
void view_print_error(const char *message);

#endif
