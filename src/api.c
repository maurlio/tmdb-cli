#include "api.h"
#include "http.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <string.h>

/**
 * URL base da API do TMDB.
 */
#define TMDB_BASE_URL "https://api.themoviedb.org/3/movie"

/**
 * Retorna o endpoint da API correspondente à categoria especificada.
 * @param category: categoria de filmes
 * @return: string contendo o endpoint da API
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
        LOG_ERROR("Variável de ambiente TMDB_API_KEY não definida.");
        return ERROR_INVALID_ARGS;
    }

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
        LOG_ERROR("Falha ao fazer o parse do JSON da API.");
        http_response_free(&response);
        return ERROR_PARSE;
    }

    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    if (!cJSON_IsArray(results)) {
        LOG_ERROR("Formato JSON inesperado: 'results' não é um array.");
        cJSON_Delete(json);
        http_response_free(&response);
        return ERROR_PARSE;
    }

    out_list->count = cJSON_GetArraySize(results);

    if (out_list->count == 0) {
        out_list->items = NULL;
        cJSON_Delete(json);
        http_response_free(&response);
        return SUCCESS;
    }

    out_list->items = calloc(out_list->count, sizeof(movie_t));
    if (!out_list->items) {
        LOG_ERROR("Falha ao alocar memória para a lista de filmes.");
        cJSON_Delete(json);
        http_response_free(&response);
        return ERROR_MEMORY;
    }

    int i = 0;
    cJSON *item = NULL;

    cJSON_ArrayForEach(item, results)
    {
        cJSON *id_obj = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
        cJSON *date = cJSON_GetObjectItemCaseSensitive(item, "release_date");
        cJSON *vote = cJSON_GetObjectItemCaseSensitive(item, "vote_average");

        if (cJSON_IsNumber(id_obj)) {
            out_list->items[i].id = id_obj->valueint;
        }

        /* Utilização de snprintf para garantir null-termination segura */
        if (cJSON_IsString(title) && title->valuestring != NULL) {
            snprintf(out_list->items[i].title,
                sizeof(out_list->items[i].title),
                "%s",
                title->valuestring);
        }

        if (cJSON_IsString(date) && date->valuestring != NULL) {
            snprintf(out_list->items[i].release_date,
                sizeof(out_list->items[i].release_date),
                "%s",
                date->valuestring);
        }

        out_list->items[i].rating =
            cJSON_IsNumber(vote) ? vote->valuedouble : 0.0;
        i++;
    }

    cJSON_Delete(json);
    http_response_free(&response);
    return SUCCESS;
}

/**
 * Implementação do contrato estabelecido em models.h.
 * A camada que aloca (api.c) é logicamente responsável por saber como liberar,
 * mantendo a coesão sem precisar de um arquivo models.c extra.
 */
void movie_list_free(movie_list_t *list)
{
    if (list != NULL) {
        if (list->items != NULL) {
            free(list->items);
            list->items = NULL;
        }
        list->count = 0;
    }
}
