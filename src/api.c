#include "api.h"
#include "http.h"
#include <cjson/cJSON.h>
#include <stdlib.h>
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

/**
 * Processa a resposta JSON da API do TMDB e imprime os filmes encontrados.
 * @param json_string A string JSON retornada pela API.
 * @return status_code_t indicando o resultado da operação (SUCCESS,
 * ERROR_GENERIC, etc.).
 */
static status_code_t parse_and_print_movies(const char *json_string)
{
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        LOG_ERROR("Falha ao processar o JSON. Antes de: %s",
            error_ptr ? error_ptr : "Desconhecido");
        return ERROR_GENERIC;
    }

    cJSON *results = cJSON_GetObjectItemCaseSensitive(json, "results");
    if (!cJSON_IsArray(results)) {
        LOG_ERROR("Resposta inesperada da API ('results' não encontrado).");
        cJSON_Delete(json);
        return ERROR_GENERIC;
    }

    printf("Filmes encontrados:\n");

    cJSON *movie = NULL;
    int index = 1;

    cJSON_ArrayForEach(movie, results)
    {
        cJSON *title = cJSON_GetObjectItemCaseSensitive(movie, "title");
        cJSON *vote = cJSON_GetObjectItemCaseSensitive(movie, "vote_average");

        if (cJSON_IsString(title) && (title->valuestring != NULL)) {
            double rating = cJSON_IsNumber(vote) ? vote->valuedouble : 0.0;
            printf("%02d. %s (%.1f) \n", index++, title->valuestring, rating);
        }
    }

    cJSON_Delete(json);
    return SUCCESS;
}

status_code_t api_fetch_and_print_movies(tmdb_category_t category)
{
    const char *api_token = getenv("TMDB_API_KEY");
    if (api_token == NULL || strlen(api_token) == 0) {
        LOG_ERROR("A variável 'TMDB_API_KEY' não está definida no ambiente.");
        fprintf(stderr,
            "Dica: Configure no terminal seu 'API Read Access Token' (v4 "
            "auth):\n");
        fprintf(stderr, "   export TMDB_API_KEY=\"seu_token_aqui\"\n");
        return ERROR_INVALID_ARGS;
    }

    const char *endpoint = get_endpoint_for_category(category);
    char full_url[256];

    // Constrói a URL completa para a requisição, com endpoint e os parâmetros.
    snprintf(full_url,
        sizeof(full_url),
        "%s%s?language=pt-BR&page=1",
        TMDB_BASE_URL,
        endpoint);

    http_response_t response = {0};
    status_code_t status = http_get(full_url, api_token, &response);

    if (status == SUCCESS) {
        status = parse_and_print_movies(response.data);
    }

    http_response_free(&response);
    return status;
}
