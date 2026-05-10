#include "cli.h"
#include "common.h"

int main(int argc, char **argv)
{
    cli_args_t args = {0};
    status_code_t status = cli_parse_args(argc, argv, &args);

    if (status != SUCCESS) {
        fprintf(stderr, "Use '%s --help' para mais informações.\n", argv[0]);
        return status;
    }

    if (args.help) {
        cli_print_help(argv[0]);
        return SUCCESS;
    }

    printf("Categoria selecionada com sucesso: %d\n", args.category);
    return SUCCESS;
}
