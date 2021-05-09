#include "common.h"

#include "hashmap.c"
#include "menu.c"
#include "operaciones.c"
#include "parse.c"
#include "read.c"

int main(void)
{
    HashMap *pokedex = createStringMap(POKEDEX_MAX);
    HashMap *pokemons = createIntMap(POKEMON_MAX);

    // Main menu
    while (true)
    {
        bool ret_val = mostrar_menu(pokedex, pokemons);
        if (!ret_val)
        {
            break;
        }
        fancy_pause("Presione cualquier tecla para volver al menu principal\n");
    }

    return (EXIT_SUCCESS);
}