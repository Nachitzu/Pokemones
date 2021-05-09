#pragma once

#include "common.h"

#include "hashmap.h"
#include "parse.h"
#include "pokedex.h"
#include "pokemon.h"
#include "read.h"
#include "strcmp_insensitive.h"


bool load_csv(HashMap *pokedex, HashMap *pokemons, const char *path);
bool pokemon_atrapado(HashMap *pokedex, HashMap *pokemons,
                      int id, char *nombre, char *types,
                      int pc, int ps, char *sexo,
                      char *ev_previa, char *ev_siguiente,
                      int numeroPkx, char *region);
void evolucionar_pokemon(HashMap *pokedex, HashMap *pokemons, int id_pokemon);
void buscar_pokemon_por_tipo(HashMap *pokedex, HashMap *pokemons, char *tipo);
void buscar_pokemon_por_nombre(HashMap *pokedex, HashMap *pokemons, char *nombre);
void buscar_pokedex_por_nombre(HashMap *pokedex, HashMap *pokemons, char *nombre);
int ordenar_pokedex_items_por_id(const void *raw_a, const void *raw_b);
void mostrar_pokedex(HashMap *pokedex, HashMap *pokemons);
int ordenar_pokemons_por_pc(const void *raw_a, const void *raw_b);
void mostrar_ordenados_pc(HashMap *pokedex, HashMap *pokemons);
void liberar_pokemon(HashMap *pokedex, HashMap *pokemons, int id_pokemon);
void mostrar_ordenados_region(HashMap *pokedex, HashMap *pokemons, char *region);
