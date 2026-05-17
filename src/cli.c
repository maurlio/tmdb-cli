#include "cli.h"
#include <string.h>

/**
 * Converte a string de forma segura para o enum oficial da API.
 * @param str: String de entrada.
 * @param out_cat: Ponteiro para armazenar o resultado.
 * @return: true se a conversão foi bem-sucedida, false caso contrário.
 */
static bool parse_category_string(const char *str, tmdb_category_t *out_cat)
{
    if (strcmp(str, "playing") == 0) {
        *out_cat = CATEGORY_PLAYING;
        return true;
    } else if (strcmp(str, "popular") == 0) {
        *out_cat = CATEGORY_POPULAR;
        return true;
    } else if (strcmp(str, "top") == 0) {
        *out_cat = CATEGORY_TOP;
        return true;
    } else if (strcmp(str, "upcoming") == 0) {
        *out_cat = CATEGORY_UPCOMING;
        return true;
    }
    return false;
}

status_code_t cli_parse_args(int argc, char **argv, cli_args_t *out_args)
{
    out_args->has_category = false;
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
                if (!parse_category_string(argv[i + 1], &out_args->category)) {
                    LOG_ERROR("Categoria invalida: %s", argv[i + 1]);
                    return ERROR_INVALID_ARGS;
                }
                out_args->has_category = true;
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

    if (!out_args->has_category && !out_args->help) {
        LOG_ERROR("Use --type para especificar uma categoria.");
        return ERROR_INVALID_ARGS;
    }

    return SUCCESS;
}

void cli_print_help(const char *prog_name)
{
    printf("Uso: %s [OPÇÕES]\n", prog_name);
    printf("Opções:\n");
    printf("  -t, --type <categoria>   Especifica a categoria de filmes.\n");
    printf("                           (playing, popular, top, upcoming)\n");
    printf("  -h, --help               Mostra esta mensagem de ajuda.\n");
}
