#pragma once

#include <ctype.h>

// strcmp, pero no le importa si hay diferencias entre 'A' y 'a'
int strcmp_insensitive(char const *a, char const *b)
{
    for (;; a++, b++)
    {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
        {
            return d;
        }
    }
}

// Chequea si str1 esta en str2, ignorando UpperCase.
int starts_with_insensitive(char const *a, char const *b)
{
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t len_min = (len_a < len_b) ? len_a : len_b;

    const char *ptr_a = a;
    const char *ptr_b = b;

    int result = 1;
    size_t count = 0;
    while (*ptr_a && count < len_min)
    {
        if (tolower(*ptr_a++) != tolower(*ptr_b++))
        {
            result = 0;
            break;
        }
        ++count;
    }

    return (result);
}
