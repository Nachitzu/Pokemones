#include "operaciones.h"

bool load_csv(HashMap *pokedex, HashMap *pokemons, const char *path)
{
    FILE *fptr = fopen(path, "r");
    if (!fptr)
    {
        printf("Error cargando archivo '%s'\n", path);
        return (true);
    }

    char line_buffer[4096];

    while (fgets(line_buffer, sizeof(line_buffer), fptr))
    {
        // printf("----------------\n");
        line_buffer[strcspn(line_buffer, "\n")] = 0;
        char *line_ptr = line_buffer;

        // Skip empty lines
        if (!line_ptr || *line_ptr == '\n')
        {
            // printf("Skipping empty line!\n");
            continue;
        }

        // Skip comments
        if (line_ptr[0] == '/' && line_ptr[1] == '/')
        {
            // printf("Skipping comment!\n");
            continue;
        }

        // Skip template line (starts by id)
        if (line_ptr[0] == 'i' && line_ptr[1] == 'd')
        {
            // printf("Skipping template!\n");
            continue;
        }

        // Reusable data for the parsing.
        char piece_buffer[256];
        char *trimmed;
        piece_buffer[0] = 0;
        char types[256];

        Pokemon poke;
        PokedexItem dex;

        // Get the ID
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true) ||
            !string_to_int(piece_buffer, &poke.id))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        // Get the name
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
        {
            printf("Failed to extract the name!\n");
            continue;
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(poke.nombre, trimmed);

        // Check if the ID is already in use
        Pokemon *used_poke = searchMap(pokemons, &poke.id);
        if (used_poke)
        {
            printf("La id %d ya esta en uso por '%s', ignorando.\n", poke.id, poke.nombre);
            continue;
        }

        // Get the type(s)
        // If it starts with "", use the special case.
        if (*line_ptr == '\"')
        {
            int commas = 0;
            if (!parse_quoted_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true,
                                    ',', &commas))
            {
                printf("Failed to extract the quoted types!\n");
                continue;
            }
        }
        else
        {
            if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
            {
                printf("Failed to extract the unquoted type!\n");
                continue;
            }
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(types, trimmed);

        // Get the PC
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true) ||
            !string_to_int(piece_buffer, &poke.pc))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        // Get the PS
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true) ||
            !string_to_int(piece_buffer, &poke.ps))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        // Get the gender
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
        {
            printf("Failed to extract the gender!\n");
            continue;
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(poke.sexo, trimmed);

        // Get the prev_evolution
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
        {
            printf("Failed to extract the prev evolution!\n");
            continue;
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(dex.ev_previa, trimmed);

        // Get the next_evolution
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
        {
            printf("Failed to extract the next evolution!\n");
            continue;
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(dex.ev_siguiente, trimmed);

        // Get the pokedex_index
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true) ||
            !string_to_int(piece_buffer, &dex.numeroPkx))
        {
            printf("Failed to convert %s to integer!\n", piece_buffer);
            continue;
        }

        // Get the region
        if (!parse_piece(&line_ptr, piece_buffer, sizeof(piece_buffer), ',', true))
        {
            printf("Failed to extract the region!\n");
            continue;
        }
        trimmed = trim_whitespace(piece_buffer);
        strcpy(dex.region, trimmed);

        // Si llegamos aca, entonces tenemos todos los datos necesarios para
        // Crear un pokemon / item en la pokedex.
        pokemon_atrapado(pokedex, pokemons,
                         poke.id, poke.nombre, types,
                         poke.pc, poke.ps, poke.sexo,
                         dex.ev_previa, dex.ev_siguiente,
                         dex.numeroPkx, dex.region);
    }

    return (true);
}

bool pokemon_atrapado(HashMap *pokedex, HashMap *pokemons,
                      int id, char *nombre, char *types,
                      int pc, int ps, char *sexo,
                      char *ev_previa, char *ev_siguiente,
                      int numeroPkx, char *region)
{

    // Estamos llenos?
    if (sizeOfMap(pokemons) >= POKEMON_MAX)
    {
        printf("El almacenamiento Pokemon esta lleno!\n");
        return (false);
    }

    // Buscamos el pokemon en la pokedex
    PokedexItem *item = searchMap(pokedex, nombre);
    if (item == NULL)
    {
        // El pokemon no esta en la pokedex, agregalo!
        item = calloc(1, sizeof(*item));
        item->numeroPkx = numeroPkx;
        item->existencia = 1;
        strcpy(item->nombre, nombre);
        strcpy(item->region, region);
        strcpy(item->ev_previa, ev_previa);
        strcpy(item->ev_siguiente, ev_siguiente);

        // Contamos la cantidad de comas
        int tipos = 1;
        char *type_ptr = types;
        while (*type_ptr)
        {
            if (*type_ptr == ',')
            {
                ++tipos;
            }
            ++type_ptr;
        }

        // Obtenemos suficiente memoria para guardar (tipos + 1) strings
        item->tipos_count = tipos;
        item->tipos = malloc(sizeof(char *) * item->tipos_count);
        for (int i = 0; i < item->tipos_count; ++i)
        {
            item->tipos[i] = malloc(sizeof(char) * POKEMON_TYPE_MAX);
        }

        // Separar types por comas
        if (item->tipos_count > 1)
        {
            char *type_piece = strtok(types, ",");
            int type_index = 0;
            while (type_piece)
            {
                char temp[POKEMON_TYPE_MAX];
                strcpy(temp, type_piece);
                char *trimmed = trim_whitespace(temp);

                strcpy(item->tipos[type_index], trimmed);

                type_piece = strtok(NULL, ",");
                ++type_index;
            }
        }
        else
        {
            strcpy(item->tipos[0], types);
        }

        insertMap(pokedex, nombre, item);
    }
    else
    {
        // El pokemon ya existe, incrementa la existencia
        item->existencia++;
    }

    // Creamos el pokemon
    Pokemon *poke = calloc(1, sizeof(*poke));
    poke->id = id;
    poke->pc = pc;
    poke->ps = ps;
    strcpy(poke->nombre, nombre);
    strcpy(poke->sexo, sexo);

    // Aca hay que cambiar nombre por poke->id
    insertMap(pokemons, &poke->id, poke);
    return (true);
}

void evolucionar_pokemon(HashMap *pokedex, HashMap *pokemons, int id_pokemon)
{
    // Buscar pokemon por ID
    Pokemon *poke = searchMap(pokemons, &id_pokemon);
    if (!poke)
    {
        printf("No se encontro un pokemon con la id %d!\n", id_pokemon);
        return;
    }

    // Buscamos la entrada en la pokedex para el pokemon
    PokedexItem *dex = searchMap(pokedex, poke->nombre);
    if (!dex)
    {
        printf("No se encontro el pokemon '%s' en la pokedex! (bug?)\n", poke->nombre);
        return;
    }

    // Buscamos la evolucion en la pokedex
    PokedexItem *dex_ev = searchMap(pokedex, dex->ev_siguiente);
    if (!dex_ev)
    {
        printf("La evolucion no se encuentra en la pokedex!\n");
        return;
    }

    // Hacemos la evolucion
    dex->existencia--;
    dex_ev->existencia++;

    strcpy(poke->nombre, dex_ev->nombre);
    int old_ps = poke->ps;
    int old_pc = poke->pc;
    poke->ps += (int)(0.25f * (float)poke->ps);
    poke->pc += (int)(0.50f * (float)poke->pc);
    printf("Evolucion finalizada! ahora %s se llama %s!\n",
           dex->nombre, poke->nombre);
    printf("PS: %d <- %d\n", poke->ps, old_ps);
    printf("PC: %d <- %d\n", poke->pc, old_pc);
}

void buscar_pokemon_por_tipo(HashMap *pokedex, HashMap *pokemons, char *tipo)
{
    // Mostraremos todos los pokemon que tengan el tipo que se esta buscando
    Pokemon *poke = firstMap(pokemons);
    size_t found_count = 0;
    while (poke)
    {
        PokedexItem *dex_item = searchMap(pokedex, poke->nombre);
        if (!dex_item)
        {
            // Como llegamos a esta situacion?
            printf("ERROR! tenemos un pokemon que no existe en la pokedex?! (bug?)\n");
        }

        // Vamos tipo por tipo en el pokemon, viendo si este corresponde
        // al que estamos buscando...
        bool found = false;
        for (size_t i = 0; i < dex_item->tipos_count; ++i)
        {
            if (strcmp_insensitive(tipo, dex_item->tipos[i]) == 0)
            {
                found = true;
            }
            else
            {
                // Busqueda parcial
                if (starts_with_insensitive(tipo, dex_item->tipos[i]))
                {
                    found = true;
                }
            }
        }

        if (found)
        {
            if (found_count == 0)
            {
                pokemonPrintTable();
            }
            pokemonPrint(poke);
            ++found_count;
        }

        poke = nextMap(pokemons);
    }

    if (!found_count)
    {
        printf("No se encontraron pokemon con el/los tipos ingresado(s)\n");
    }
}

void buscar_pokemon_por_nombre(HashMap *pokedex, HashMap *pokemons, char *nombre)
{
    Pokemon *poke = firstMap(pokemons);
    size_t found_count = 0;
    while (poke)
    {
        if (strcmp_insensitive(nombre, poke->nombre) == 0)
        {
            if (found_count == 0)
            {
                pokemonPrintTable();
            }
            pokemonPrint(poke);
            ++found_count;
        }
        else
        {
            // Busqueda parcial
            if (starts_with_insensitive(nombre, poke->nombre))
            {
                if (found_count == 0)
                {
                    pokemonPrintTable();
                }
                pokemonPrint(poke);
                ++found_count;
            }
        }

        poke = nextMap(pokemons);
    }

    if (!found_count)
    {
        printf("No se encontraron pokemon de nombre %s\n", nombre);
    }
}

void buscar_pokedex_por_nombre(HashMap *pokedex, HashMap *pokemons, char *nombre)
{
    PokedexItem *item = searchMap(pokedex, nombre);
    if (item)
    {
        pokedexItemPrintTable();
        pokedexItemPrint(item);
    }
    else
    {
        printf("No se encontro %s en la pokedex, buscando parecidos...\n", nombre);
        // Si no lo encontramos, tratar de buscar ignorando UpPeR CasE
        PokedexItem *dex = firstMap(pokedex);
        size_t found_count = 0;
        while (dex)
        {
            if (strcmp_insensitive(nombre, dex->nombre) == 0)
            {
                if (found_count == 0)
                {
                    pokedexItemPrintTable();
                }
                pokedexItemPrint(dex);
                ++found_count;
            }
            else
            {
                // Podria ser que estamos buscando <Ka> y nos
                // podria salir <Kangaskhan> y <Kabuto> etc...
                if (starts_with_insensitive(nombre, dex->nombre))
                {
                    if (found_count == 0)
                    {
                        pokedexItemPrintTable();
                    }
                    pokedexItemPrint(dex);
                    ++found_count;
                }
            }

            dex = nextMap(pokedex);
        }

        if (!found_count)
        {
            printf("No se encontraron pokemon de nombre %s\n", nombre);
        }
    }
}

// Funcion a utilizar en qsort para ordenar los items de la pokedex
// basado en el numero de este.
int ordenar_pokedex_items_por_id(const void *raw_a, const void *raw_b)
{
    PokedexItem *a = *(PokedexItem **)raw_a;
    PokedexItem *b = *(PokedexItem **)raw_b;
    return (a->numeroPkx - b->numeroPkx);
}

void mostrar_pokedex(HashMap *pokedex, HashMap *pokemons)
{
    // Idea: iterar el map, por cada pkmn valido, tomar un puntero a este
    // y meterlos todos en un array y aplicarle al quicksort, luego se
    // printea cada elemento del array ordenado.

    size_t map_count = sizeOfMap(pokedex);
    size_t ptr_size = sizeof(PokedexItem *);
    // Necesitamos espacio para map_count punteros a PokedexItem;
    PokedexItem **items = malloc(map_count * ptr_size);

    // Obtenemos los punteros
    PokedexItem *map_item = firstMap(pokedex);
    size_t i = 0;
    while (map_item)
    {
        items[i] = map_item;
        map_item = nextMap(pokedex);
        ++i;
    }

    // Ordenar los items
    qsort(items, map_count, ptr_size, ordenar_pokedex_items_por_id);

    // Mostrar los items
    pokedexItemPrintTable();
    for (size_t i = 0; i < map_count; ++i)
    {
        PokedexItem *item = items[i];
        pokedexItemPrint(item);
    }

    free(items);
}

int ordenar_pokemons_por_pc(const void *raw_a, const void *raw_b)
{
    Pokemon *a = *(Pokemon **)raw_a;
    Pokemon *b = *(Pokemon **)raw_b;
    return (a->pc - b->pc);
}

void mostrar_ordenados_pc(HashMap *pokedex, HashMap *pokemons)
{
    size_t poke_count = sizeOfMap(pokemons);
    size_t ptr_size = sizeof(Pokemon *);

    Pokemon **items = malloc(poke_count * ptr_size);
    Pokemon *map_item = firstMap(pokemons);
    size_t i = 0;
    while (map_item)
    {
        items[i] = map_item;
        map_item = nextMap(pokemons);
        ++i;
    }

    qsort(items, poke_count, ptr_size, ordenar_pokemons_por_pc);

    // Mostrar los items
    pokemonPrintTable();
    for (size_t i = 0; i < poke_count; ++i)
    {
        Pokemon *item = items[i];
        pokemonPrint(item);
    }

    free(items);
}

void liberar_pokemon(HashMap *pokedex, HashMap *pokemons, int id_pokemon)
{
    // Buscar pokemon por ID
    Pokemon *poke = searchMap(pokemons, &id_pokemon);
    if (!poke)
    {
        printf("No se encontro un pokemon con la id %d!\n", id_pokemon);
        return;
    }

    // Buscamos la entrada en la pokedex para el pokemon
    PokedexItem *dex = searchMap(pokedex, poke->nombre);
    if (!dex)
    {
        printf("No se encontro el pokemon '%s' en la pokedex! (bug?)\n", poke->nombre);
        return;
    }

    dex->existencia--;
    eraseMap(pokemons, &id_pokemon);
    printf("Pokemon '%s' de id %d ha sido eliminado correctamente\n", poke->nombre, poke->id);
    free(poke);
}

void mostrar_ordenados_region(HashMap *pokedex, HashMap *pokemons, char *region)
{
    int cantidad = 0;
    PokedexItem *item = firstMap(pokedex);
    while (item)
    {
        if (strcmp_insensitive(region, item->region) == 0)
        {
            if (cantidad == 0)
            {
                pokedexItemPrintTable();
            }
            pokedexItemPrint(item);

            cantidad += item->existencia;
        }
		else
		{
			if (starts_with_insensitive(region, item->region))
			{
				if (cantidad == 0)
				{
					pokedexItemPrintTable();
				}
				pokedexItemPrint(item);

				cantidad += item->existencia;
			}
		}

        item = nextMap(pokedex);
    }
}
