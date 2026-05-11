#include "http.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

static size_t write_callback(
    void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    http_response_t *mem = (http_response_t *)userp;

    // Realoca a memória para armazenar os dados recebidos
    char *ptr = realloc(mem->data, mem->size + total_size + 1);
    if (ptr == NULL) {
        LOG_ERROR("Falha ao reservar memória para o download HTTP.");
        return 0;
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
    CURL *curl_handle;
    CURLcode res;
    struct curl_slist *headers = NULL;

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
        http_response_free(out_response);
        return ERROR_NETWORK;
    }

    // Configura as opções da requisição HTTP (URL e callbacks)
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)out_response);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    if (auth_token) {
        char auth_header[256];
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
        http_response_free(out_response);
        return ERROR_NETWORK;
    }

    return SUCCESS;
}

void http_response_free(http_response_t *response)
{
    if (response != NULL) {
        free(response->data);
        response->data = NULL;
        response->size = 0;
    }
}
