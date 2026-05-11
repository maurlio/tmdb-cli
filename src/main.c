#include "cli.h"
#include "common.h"
#include "http.h"

int main(int argc, char **argv)
{
    cli_args_t args = {0};

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

    // -- Bloco de teste HTTP --

    printf("Iniciando teste HTTP...\n");
    http_response_t response = {0};

    const char *test_url = "https://github.com/";

    if (http_get(test_url, NULL, &response) == SUCCESS) {
        printf("Resposta HTTP recebida com sucesso. Tamanho: %zu bytes\n",
            response.size);
        http_response_free(&response);
    }

    // -- Fim do bloco de teste HTTP --

    http_cleanup();
    return SUCCESS;
}
