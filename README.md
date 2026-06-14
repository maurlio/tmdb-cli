> English | [Português](README.pt-BR.md)

# TMDB-CLI

A command-line interface (CLI) application developed in C that allows querying movie information directly from the terminal, consuming the official The Movie Database (TMDB) API.

This project was developed as a solution to the [TMDB CLI Tool](https://roadmap.sh/projects/tmdb-cli) challenge from the **Roadmap.sh** platform.

## Features

* Query movies by category: `playing`, `popular`, `top`, and `upcoming`
* Display of ID, title, release date, and average rating
* Error handling for network issues, JSON parsing, and invalid inputs

## Dependencies

The project was developed in compliance with the C17 standard, maintaining full backward compatibility with C11. To compile the project, you must have gcc, make, and the following development libraries installed:

* `libcurl`
* `cJSON`

## Building

The project utilizes a Makefile configured with automatic header dependency tracking (-MMD -MP) and strict compilation flags (-Wall -Wextra -pedantic -Werror).

```bash
make            # Production build
make debug      # Build with debug symbols
make sanitizer  # Build with Address Sanitizer
make clean      # Remove compilation artifacts
make format     # Format code with clang-format
```

## Usage

Before running the application, you must provide your TMDB API access token via an environment variable.

```bash
# 1. Export your API key
export TMDB_API_KEY="your_access_token_here"

# 2. Run the program requesting a category
./tmdb-cli --type popular
```

## Supported Arguments

```text
-t, --type <category>   playing | popular | top | upcoming
-h, --help               help
```

## Project Structure

The code is organized into single-responsibility layers, prioritizing low coupling, a clear separation of concerns, and ease of maintenance:

```text
cli.*       Argument parser and main flow
api.*       Integration with the TMDB API
http.*      Wrapper over libcurl
view.*      Terminal rendering
models.h    Data structures
common.h    Utilities and macros
```

## License

This project is distributed under the terms of the MIT License.
