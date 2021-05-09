#include "menu.h"

// Funciones de menu:
//   Se encargan de obtener los datos del usuario y luego
//   llaman la version sin el menu_*()
bool menu_cargar_csv(HashMap *pokedex, HashMap *pokemons)
{
    char filename[LINE_MAX];
    leer_linea("Ingrese el nombre del archivo a cargar: ", filename, sizeof(filename), false);

    bool loaded = load_csv(pokedex, pokemons, filename);
    if (loaded)
    {
        printf("Archivo CSV cargado correctamente!\n");
    }
    else
    {
        printf("Error al cargar el archivo CSV!\n");
    }

    return (true);
}

bool menu_pokemon_atrapado(HashMap *pokedex, HashMap *pokemons)
{
    // Ej:
    char nombre[STR_ITEM_MAX];
    char sexo_letra[16];
    char ev_previa[STR_ITEM_MAX];
    char ev_siguiente[STR_ITEM_MAX];
    char region[STR_ITEM_MAX];
    int PS = 0;
    int PC = 0;
    int num_poke = 0;

    leer_numero("Ingrese el indice en la Pokedex: ", &num_poke, false);

    leer_linea("Ingrese el nombre: ", nombre, sizeof(nombre), false);
    leer_linea("Ingrese el sexo (M/H/F):", sexo_letra, sizeof(sexo_letra), false);
    const char *sexo = (strcmp_insensitive(sexo_letra, "m") == 0) ? "Macho" : (strcmp_insensitive(sexo_letra, "h") ? "Hembra" : "No tiene");
    leer_linea("Ingrese la region: ", region, sizeof(region), false);

    // Leer los tipos, separados por /
    char tipos_coma[LINE_MAX];
    leer_linea("Ingrese el/los tipo(s) separados por coma: ",
               tipos_coma, sizeof(tipos_coma), false);

    leer_numero("Ingrese los PS: ", &PS, false);
    leer_numero("Ingrese los PC: ", &PC, false);

    leer_linea("Ingrese la evolucion previa (o vacio): ", ev_previa, sizeof(ev_previa), true);
    if (!ev_previa[0])
    {
        strcpy(ev_previa, "No tiene");
    }

    leer_linea("Ingrese la evolucion siguiente (o vacio): ", ev_siguiente, sizeof(ev_siguiente), true);
    if (!ev_siguiente[0])
    {
        strcpy(ev_siguiente, "No tiene");
    }

    // Obtener id optima, buscando hasta encontrar el menor id que esta vacio en el mapa.
    size_t id = 0;
    for (id = 0; id < sizeOfMap(pokemons); ++id)
    {
        if (!searchMap(pokemons, &id))
        {
            break;
        }
    }
    // Aca la ID esta garantizada a ser unica.

    pokemon_atrapado(pokedex, pokemons,
                     id, nombre, tipos_coma, PC, PS, (char *)sexo, ev_previa, ev_siguiente, num_poke, region);

    printf("Pokemon de ID: %zu agregado correctamente!\n", id);

    return (true);
}

bool menu_evolucionar_pokemon(HashMap *pokedex, HashMap *pokemons)
{
    int id;
    leer_numero("Ingrese ID a evolucionar: ", &id, false);
    if (id <= 0)
    {
        printf("Abortado!\n");
        return (true);
    }

    evolucionar_pokemon(pokedex, pokemons, id);
    return (true);
}

bool menu_buscar_pokemon_por_tipo(HashMap *pokedex, HashMap *pokemons)
{
    char tipo[POKEMON_TYPE_MAX];
    leer_linea("Ingrese tipo a buscar: ", tipo, sizeof(tipo), true);
    if (!*tipo)
    {
        printf("Abortado!\n");
        return (true);
    }

    buscar_pokemon_por_tipo(pokedex, pokemons, tipo);
    return (true);
}

bool menu_buscar_pokemon_por_nombre(HashMap *pokedex, HashMap *pokemons)
{
    char nombre[STR_ITEM_MAX];
    leer_linea("Ingrese el nombre a buscar: ", nombre, sizeof(nombre), true);
    if (!*nombre)
    {
        printf("Abortado!\n");
        return (true);
    }

    buscar_pokemon_por_nombre(pokedex, pokemons, nombre);
    return (true);
}

bool menu_buscar_pokedex_por_nombre(HashMap *pokedex, HashMap *pokemons)
{
    char nombre[STR_ITEM_MAX];
    leer_linea("Ingrese nombre a buscar: ", nombre, sizeof(nombre), true);
    if (!*nombre)
    {
        printf("Abortado!\n");
        return (true);
    }

    buscar_pokedex_por_nombre(pokedex, pokemons, nombre);
    return (true);
}

bool menu_mostrar_pokedex(HashMap *pokedex, HashMap *pokemons)
{
    mostrar_pokedex(pokedex, pokemons);
    return (true);
}

bool menu_mostrar_ordenados_pc(HashMap *pokedex, HashMap *pokemons)
{
    mostrar_ordenados_pc(pokedex, pokemons);
    return (true);
}

bool menu_liberar_pokemon(HashMap *pokedex, HashMap *pokemons)
{
    int id;
    leer_numero("Ingrese ID a liberar: ", &id, false);
    if (id <= 0)
    {
        printf("Abortado!\n");
        return (true);
    }

    liberar_pokemon(pokedex, pokemons, id);
    return (true);
}

bool menu_mostrar_ordenados_region(HashMap *pokedex, HashMap *pokemons)
{
    char region[STR_ITEM_MAX];
    leer_linea("Ingrese region a mostrar: ", region, sizeof(region), true);
    if (!*region)
    {
        printf("Abortado!\n");
        return (true);
    }

    mostrar_ordenados_region(pokedex, pokemons, region);
    return (true);
}

bool menu_salir(HashMap *pokedex, HashMap *pokemons)
{
    (void)pokedex;
    (void)pokemons;
    return (false);
}

// Menu
typedef bool menuFunc(HashMap *, HashMap *);
typedef struct menuOption
{
    const char *option;
    const char *msg;
    menuFunc *func;
} menuOption;

bool mostrar_menu(HashMap *pokedex, HashMap *pokemons)
{
    menuOption options[] = {
        {"1", "Importar Pokedex desde archivo CSV", menu_cargar_csv},
        {"2", "Pokemon atrapado", menu_pokemon_atrapado},
        {"3", "Evolucionar Pokemon", menu_evolucionar_pokemon},
        {"4", "Buscar Pokemon por tipo", menu_buscar_pokemon_por_tipo},
        {"5", "Buscar Pokemon por nombre", menu_buscar_pokemon_por_nombre},
        {"6", "Buscar Pokemon en Pokedex por nombre", menu_buscar_pokedex_por_nombre},
        {"7", "Mostrar todos los Pokemon de la Pokedex", menu_mostrar_pokedex},
        {"8", "Mostrar mis Pokemon ordenados por PC", menu_mostrar_ordenados_pc},
        {"9", "Liberar Pokemon", menu_liberar_pokemon},
        {"10", "Mostrar Pokemon por Region", menu_mostrar_ordenados_region},
        {"0", "Salir", menu_salir}};

    // Mostrar todas las opciones en pantalla
    printf("+----------------+\n");
    printf("| PokeCenter XD! |\n");
    printf("+----------------+\n");
    printf("Ingrese una opcion:\n");

    menuOption *option;
    int options_len = sizeof(options) / sizeof(options[0]);
    for (int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        printf("\t%-2s) %s\n", option->option, option->msg);
    }

    // Obtener el input del usuario.
    printf("> ");
    char line[LINE_MAX];
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = 0;

    if (!(*line))
    {
        printf("Ingrese una opcion\n");
        return (true);
    }

    // Buscamos si la opcion corresponde a alguna de la lista.
    bool found = false;
    for (int i = 0; i < options_len; ++i)
    {
        option = &options[i];
        found = strcmp_insensitive(option->option, line) == 0;

        if (found)
        {
            found = true;
            break;
        }
    }

    // De encontrar la funcion, la llamamos y retornamos lo que nos de,
    // sino, asumimos un mal input y preguntamos denuevo.
    if (found)
    {
        // printf("calling %c's function!\n", option->option);
        return (option->func(pokedex, pokemons));
    }
    else
    {
        printf("Opcion '%s' invalida\n", line);
        return (true);
    }
}
