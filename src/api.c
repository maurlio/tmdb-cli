#include "api.h"
#include "http.h"
#include <cjson/cJSON.h>
#include <string.h>

/**
 * Define a URL base para as requisições à API do TMDB.
 */
#define TMDB_BASE_URL "https://api.themoviedb.org/3/movie"

/**
 * Retorna o endpoint da API do TMDB para uma determinada categoria.
 * @param category A categoria para a qual obter o endpoint.
 * @return O endpoint da API correspondente à categoria.
 */
static const char *get_endpoint_for_category(tmdb_category_t category)
{
    switch (category) {
    case CATEGORY_PLAYING:
        return "/now_playing";
    case CATEGORY_POPULAR:
        return "/popular";
    case CATEGORY_TOP:
        return "/top_rated";
    case CATEGORY_UPCOMING:
        return "/upcoming";
    default:
        return "";
    }
}

status_code_t api_get_movies(tmdb_category_t category, movie_list_t *out_list)
{
    const char *api_token = getenv("TMDB_API_KEY");
    if (!api_token) {
        return ERROR_GENERIC;
    }

    // Constrói a URL completa para a requisição
    const char *endpoint = get_endpoint_for_category(category);
    char url[256];

    snprintf(url,
        sizeof(url),
        "%s%s?language=pt-BR&page=1",
        TMDB_BASE_URL,
        endpoint);

    http_response_t response = {0};
    status_code_t status = http_get(url, api_token, &response);

    if (status != SUCCESS) {
        return status;
    }

    cJSON *json = cJSON_Parse(response.data);
    if (!json) {
        http_response_free(&response);
        return ERROR_GENERIC;
    }

    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    if (!cJSON_IsArray(results)) {
        cJSON_Delete(json);
        http_response_free(&response);
        return ERROR_GENERIC;
    }

    out_list->count = cJSON_GetArraySize(results);
    out_list->items = calloc(out_list->count, sizeof(movie_t));
    if (!out_list->items) {
        cJSON_Delete(json);
        http_response_free(&response);
        return ERROR_MEMORY;
    }

    int i = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, results)
    {
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
        cJSON *vote = cJSON_GetObjectItemCaseSensitive(item, "vote_average");

        if (cJSON_IsString(title) && title->valuestring != NULL) {
            strncpy(out_list->items[i].title,
                title->valuestring,
                sizeof(out_list->items[i].title) - 1);
        }
        out_list->items[i].rating =
            cJSON_IsNumber(vote) ? vote->valuedouble : 0.0;
        i++;
    }

    cJSON_Delete(json);
    http_response_free(&response);
    return SUCCESS;
}

void api_free_movie_list(movie_list_t *list)
{
    if (list && list->items) {
        free(list->items);
        list->items = NULL;
        list->count = 0;
    }
}
