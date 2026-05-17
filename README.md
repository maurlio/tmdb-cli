# TMDB-CLI

Aplicação de linha de comando em C para consultar filmes utilizando a API do The Movie Database (TMDB).

## Funcionalidades

* Consulta de filmes por categoria: `playing`, `popular`, `top` e `upcoming`
* Exibição de ID, título, data de lançamento e nota média
* Tratamento de erros de rede, parsing JSON e entradas inválidas

## Dependências

O projeto foi desenvolvido em conformidade com o padrão C17, mantendo total retrocompatibilidade com C11. Para realizar a compilação, é necessário ter instalados o gcc, o make e as seguintes bibliotecas de desenvolvimento:

* `libcurl`
* `cJSON`

## Compilação

O projeto utiliza um Makefile configurado com rastreamento automático de dependências de headers (-MMD -MP) e flags de compilação rigorosas (-Wall -Wextra -pedantic -Werror).

```bash
make            # Build de produção
make debug      # Build com símbolos de debug
make sanitizer  # Build com Address Sanitizer
make clean      # Remove artefatos de compilação
make format     # Formata o código com clang-format
```

## Uso

Antes de executar, é obrigatório fornecer o seu token de acesso da API do TMDB através de uma variável de ambiente.

```bash
# 1. Exporte a sua chave da API
export TMDB_API_KEY="seu_token_de_acesso_aqui"

# 2. Execute o programa solicitando uma categoria
./tmdb-cli --type popular
```

## Argumentos Suportados

```text
-t, --type <categoria>   playing | popular | top | upcoming
-h, --help               ajuda
```

## Estrutura do Projeto

O código foi organizado em camadas com responsabilidade única, priorizando baixo acoplamento, separação clara de responsabilidades e facilidade de manutenção:

```text
cli.*      Parser de argumentos e fluxo principal
api.*      Integração com a API do TMDB
http.*     Wrapper sobre libcurl
view.*     Renderização no terminal
models.h   Estruturas de dados
common.h   Utilitários e macros
```

## Licença

Este projeto é distribuído sob os termos da Licença MIT.
