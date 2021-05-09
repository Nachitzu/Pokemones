#pragma once

#include "common.h"
#include "hashmap.h"

typedef struct PokedexItem
{
    int numeroPkx;
    int existencia;

    char **tipos;
    int tipos_count;

    char nombre[STR_ITEM_MAX];
    char region[STR_ITEM_MAX];

    char ev_previa[STR_ITEM_MAX];
    char ev_siguiente[STR_ITEM_MAX];
} PokedexItem;

void pokedexItemPrintTable(void)
{
    printf("%-4s | %-15s | %-6s | %-32s | %-10s | %-15s | %-15s\n",
           "nDEX", "NOMBRE", "TIENES", "TIPOS", "REGION", "EV_PREVIA", "EV_SIGUIENTE");
}

void pokedexItemPrint(PokedexItem *item)
{
    char tipos[256];
    char *tipos_ptr = tipos;
    for (size_t i = 0; i < item->tipos_count; ++i)
    {
        char *ptr = item->tipos[i];
        while (*ptr)
        {
            *tipos_ptr++ = *ptr++;
        }
        if (i < item->tipos_count - 1)
        {
            *tipos_ptr++ = ',';
            *tipos_ptr++ = ' ';
        }
    }
    *tipos_ptr = '\0';

    printf("%-4d | %15s | %-6d | %-32s | %-10s | %-15s | %-15s\n",
           item->numeroPkx, item->nombre, item->existencia, tipos, item->region, item->ev_previa, item->ev_siguiente);
}