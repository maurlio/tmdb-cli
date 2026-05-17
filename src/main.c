#include "api.h"
#include "cli.h"
#include "common.h"
#include "http.h"
#include "view.h"

int main(int argc, char **argv)
{
    cli_args_t args = {0};
    movie_list_t movies = {0};
    status_code_t status = SUCCESS;
    bool http_initialized = false;

    status = cli_parse_args(argc, argv, &args);

    if (status != SUCCESS) {
        fprintf(stderr, "Use '%s --help' para mais informações.\n", argv[0]);
        goto cleanup;
    }

    if (args.help) {
        cli_print_help(argv[0]);
        status = SUCCESS;
        goto cleanup; /* Encerra sem alocar rede ou modelos */
    }

    if (http_init() != SUCCESS) {
        view_print_error("Falha ao inicializar o módulo de rede.");
        status = ERROR_NETWORK;
        goto cleanup;
    }
    http_initialized = true;

    status = api_get_movies(args.category, &movies);

    if (status == SUCCESS) {
        view_print_movie_list(&movies);
    } else {
        view_print_error(
            "Falha ao buscar filmes. Verifique a TMDB_API_KEY e sua conexão.");
    }

cleanup:
    movie_list_free(&movies);

    if (http_initialized) {
        http_cleanup();
    }

    return status == SUCCESS ? 0 : (int)status;
}
