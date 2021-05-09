#pragma once

#include "common.h"

// Recibe un puntero a un string, y un buffer donde se guardara el resultado.
// Busca el separador y luego copia desde line_at hasta el separador (no inclusivo)
// y luego modifica line_ptr para que apunte a la derecha del separador.
bool parse_piece(char **line_ptr, char *buffer, size_t buffer_size,
                 char separator, bool newline_is_separator);

// Version especializada de parse_piece, esta hace lo mismo, pero
// primero se salta el string hasta encontrar la primera comilla,
// luego copia todo el texto que encuentre hasta la segunda comilla
// y finalmente busca el separador. (sin copiar nada).
// Ademas recibe un separador para contar dentro del string entre comillas.
bool parse_quoted_piece(char **line_ptr, char *buffer, size_t buffer_size,
                        char separator, bool newline_is_separator,
                        char separator2, int *hit_count);

// Version simplificada de strtol, donde solo
// importa si logro hacer la conversion y nada mas.
// Se asume que el string esta compuesto unicamente
// del int a convertir y nada mas. (ya preprocesado).
bool string_to_int(char *string, int *result);

char *trim_whitespace(char *string);
