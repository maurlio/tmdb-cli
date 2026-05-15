#include "api.h"
#include "cli.h"
#include "common.h"
#include "http.h"
#include "view.h"

int main(int argc, char **argv)
{
    cli_args_t args = {0};
    movie_list_t movies = {0};

    if (http_init() != SUCCESS) {
        return ERROR_NETWORK;
    }

    status_code_t status = cli_parse_args(argc, argv, &args);

    if (status != SUCCESS) {
        fprintf(stderr, "Use '%s --help' para mais informações.\n", argv[0]);
        http_cleanup();
        return status;
    }

    if (args.help) {
        cli_print_help(argv[0]);
        http_cleanup();
        return SUCCESS;
    }

    status = api_get_movies(args.category, &movies);

    if (status == SUCCESS) {
        view_print_movie_list(&movies);
    } else {
        fprintf(
            stderr, "Erro ao buscar filmes. Verifique sua chave e conexão.\n");
    }

    api_free_movie_list(&movies);
    http_cleanup();

    return status;
}
