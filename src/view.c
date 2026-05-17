#include "view.h"
#include <stdio.h>

void view_print_movie_list(const movie_list_t *list)
{
    if (list == NULL || list->count == 0) {
        printf("Nenhum filme encontrado.\n");
        return;
    }

    printf("Lista de Filmes:\n");

    for (size_t i = 0; i < list->count; i++) {
        /* Previne impressão de lixo se a data vier vazia da API */
        const char *date = (list->items[i].release_date[0] != '\0')
                               ? list->items[i].release_date
                               : "Sem data";

        printf("%zu. [%d] %s (%s) - Nota: %.1f\n",
            i + 1,
            list->items[i].id,
            list->items[i].title,
            date,
            list->items[i].rating);
    }
}

void view_print_error(const char *message)
{
    if (message != NULL) {
        fprintf(stderr, "[ERRO] %s\n", message);
    }
}
