#include "view.h"
#include <stdio.h>

void view_print_movie_list(const movie_list_t *list)
{
    if (list == NULL || list->count == 0) {
        printf("Nenhum filme encontrado.\n");
        return;
    }

    printf("Lista de Filmes:\n");
    for (int i = 0; i < list->count; i++) {
        printf("%d. %s (%.1f)\n",
            i + 1,
            list->items[i].title,
            list->items[i].rating);
    }
}
