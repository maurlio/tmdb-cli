#include "http.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

/**
 * Callback para escrever os dados recebidos da resposta HTTP.
 * @param contents Ponteiro para os dados recebidos.
 * @param size Tamanho de cada elemento (geralmente 1 byte).
 * @param nmemb Número de elementos (tamanho total dos dados é size * nmemb).
 * @param userp Ponteiro onde os dados devem ser armazenados.
 * @return O número total de bytes processados (size * nmemb) ou 0 em caso de
 * erro.
 */
static size_t write_callback(
    void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    http_response_t *mem = (http_response_t *)userp;

    /* Realoca a memória para acomodar os novos dados recebidos.
     * Se falhar, realloc retorna NULL mas o ponteiro original (mem->data)
     * permanece intocado, prevenindo double-frees. */
    char *ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) {
        LOG_ERROR("Falha ao reservar memória para o download HTTP.");
        return 0; /* Retornar 0 força o curl_easy_perform a abortar */
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0';

    return total_size;
}

status_code_t http_init(void)
{
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        LOG_ERROR(
            "Não foi possível iniciar a libcurl: %s", curl_easy_strerror(res));
        return ERROR_NETWORK;
    }

    return SUCCESS;
}

void http_cleanup(void) { curl_global_cleanup(); }

status_code_t http_get(
    const char *url, const char *auth_token, http_response_t *out_response)
{
    CURL *curl_handle = NULL;
    CURLcode res;
    struct curl_slist *headers = NULL;
    status_code_t status = SUCCESS;

    if (!url || !out_response) {
        return ERROR_INVALID_ARGS;
    }

    out_response->data = malloc(1);
    if (out_response->data == NULL) {
        LOG_ERROR("Falha ao iniciar a resposta HTTP por falta de memória.");
        return ERROR_MEMORY;
    }

    out_response->data[0] = '\0';
    out_response->size = 0;

    curl_handle = curl_easy_init();
    if (!curl_handle) {
        LOG_ERROR("Falha ao criar a sessão da libcurl.");
        status = ERROR_NETWORK;
        goto cleanup;
    }

    /* Configura as opções da requisição HTTP */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)out_response);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 15L);

    if (auth_token) {
        char auth_header[1024];
        /* snprintf garante que não haverá buffer overflow mesmo com tokens
         * imensos */
        snprintf(auth_header,
            sizeof(auth_header),
            "Authorization: Bearer %s",
            auth_token);
        headers = curl_slist_append(headers, auth_header);
        headers = curl_slist_append(headers, "Accept: application/json");
        curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
    }

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        LOG_ERROR("Não foi possível concluir a requisição HTTP: %s",
            curl_easy_strerror(res));
        status = ERROR_NETWORK;
        goto cleanup;
    }

    long http_code = 0;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code >= 400) {
        LOG_ERROR("Falha na resposta do servidor TMDB (HTTP %ld)", http_code);
        LOG_ERROR("Resposta do servidor: %s", out_response->data);
        status = ERROR_NETWORK;
        goto cleanup;
    }

cleanup:
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (curl_handle) {
        curl_easy_cleanup(curl_handle);
    }

    if (status != SUCCESS) {
        http_response_free(out_response);
    }

    return status;
}

void http_response_free(http_response_t *response)
{
    if (response != NULL) {
        if (response->data != NULL) {
            free(response->data);
        }
        response->data = NULL;
        response->size = 0;
    }
}
