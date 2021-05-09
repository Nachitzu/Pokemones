#pragma once

#include "common.h"
#include "hashmap.h"
#include "pokedex.h"

typedef struct Pokemon
{
    int id;
    int pc;
    int ps;

    char nombre[STR_ITEM_MAX];
    char sexo[STR_ITEM_MAX];
} Pokemon;

void pokemonPrintTable(void)
{
    printf("%-3s | %-15s | %-10s | %-5s | %-5s\n",
           "ID", "NOMBRE", "SEXO", "PC", "PS");
}

void pokemonPrint(Pokemon *item)
{
    printf("%-3d | %15s | %-10s | %-5d | %-5d\n",
           item->id, item->nombre, item->sexo, item->pc, item->ps);
}
