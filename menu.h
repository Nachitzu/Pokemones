#pragma once

#include "common.h"
#include "hashmap.h"
#include "operaciones.h"

#include "fancy_pause.h"
#include "strcmp_insensitive.h"

bool menu_cargar_csv(HashMap *pokedex, HashMap *pokemons);
bool menu_pokemon_atrapado(HashMap *pokedex, HashMap *pokemons);
bool menu_evolucionar_pokemon(HashMap *pokedex, HashMap *pokemons);
bool menu_buscar_pokemon_por_tipo(HashMap *pokedex, HashMap *pokemons);
bool menu_buscar_pokemon_por_nombre(HashMap *pokedex, HashMap *pokemons);
bool menu_buscar_pokedex_por_nombre(HashMap *pokedex, HashMap *pokemons);
bool menu_mostrar_pokedex(HashMap *pokedex, HashMap *pokemons);
bool menu_mostrar_ordenados_pc(HashMap *pokedex, HashMap *pokemons);
bool menu_liberar_pokemon(HashMap *pokedex, HashMap *pokemons);
bool menu_mostrar_ordenados_region(HashMap *pokedex, HashMap *pokemons);
bool menu_salir(HashMap *pokedex, HashMap *pokemons);
bool mostrar_menu(HashMap *pokedex, HashMap *pokemons);
