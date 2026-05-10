#include "cli.h"
#include <string.h>

/**
 * Converte uma string de categoria em um valor do enum tmdb_category_t.
 * @param str: String representando a categoria.
 * @return: Valor do enum tmdb_category_t correspondente à string, ou
 * CATEGORY_UNKNOWN se a string não for reconhecida.
 */
static tmdb_category_t parse_category_string(const char *str)
{
    if (strcmp(str, "playing") == 0) {
        return CATEGORY_PLAYING;
    } else if (strcmp(str, "popular") == 0) {
        return CATEGORY_POPULAR;
    } else if (strcmp(str, "top") == 0) {
        return CATEGORY_TOP;
    } else if (strcmp(str, "upcoming") == 0) {
        return CATEGORY_UPCOMING;
    }

    return CATEGORY_UNKNOWN;
}

status_code_t cli_parse_args(int argc, char **argv, cli_args_t *out_args)
{
    out_args->category = CATEGORY_UNKNOWN;
    out_args->help = false;

    if (argc == 1) {
        LOG_ERROR("Nenhum argumento especificado.");
        return ERROR_INVALID_ARGS;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            out_args->help = true;
            return SUCCESS;
        } else if (strcmp(argv[i], "--type") == 0 ||
                   strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {
                out_args->category = parse_category_string(argv[i + 1]);
                if (out_args->category == CATEGORY_UNKNOWN) {
                    LOG_ERROR("Categoria invalida: %s", argv[i + 1]);
                    return ERROR_INVALID_ARGS;
                }
                i++;
            } else {
                LOG_ERROR("Falta valor para o argumento --type.");
                return ERROR_INVALID_ARGS;
            }
        } else {
            LOG_ERROR("Argumento desconhecido: %s", argv[i]);
            return ERROR_INVALID_ARGS;
        }
    }

    if (out_args->category == CATEGORY_UNKNOWN && !out_args->help) {
        LOG_ERROR("Use --type para especificar uma categoria.");
        return ERROR_INVALID_ARGS;
    }

    return SUCCESS;
}

void cli_print_help(const char *prog_name)
{
    printf("Uso: %s [OPÇÕES]\n", prog_name);
    printf("Opções:\n");
    printf("  -t, --type <categoria>   Especifica a categoria de filmes a "
           "consultar.\n");
    printf("                           Categorias válidas: playing, popular, "
           "top, upcoming\n");
    printf("  -h, --help               Exibe esta mensagem de ajuda.\n");
}
