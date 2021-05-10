README:

Jorge: RUT => 19.357.933-7      
Nathalia: RUT => 20.919.493-7    
Antonia: RUT => 20.704.874-7
Ignacio: RUT => 20.995.869-4

Instrucciones de compilacion: 
- correr el comando:
 > make

- Para correr, puede usar:
 > make run
- O tambien:
 > make
 > ./PokeCenter

- Tambien esta make clean para limpiar todos los .o y el ejecutable.

- Si no esta make disponible, puede compilarse usando)
 > gcc main.c -Wall -lm -o PokeCenter
 > ./PokeCenter


Info.
Todos los items parecen funcionar bien, 
	- Importar pokemon desde csv ignora correctamente las ids repetidas y parece
	no fallar con datos raros ni lineas incompletas, etc.
	El formato del csv es dato1,dato2,dato3,...,datoN
	donde dato3 puede ser del tipo ...,"sub1,sub2,...,subN",... para los tipos.
	* DEBERIA (pero no esta probado) poder ignorar espacios a los lados de los separadores
	(haciendo strip).

	- Pokemon atrapado funciona igual que cargar csv porque se usan mutuamente. internamente
	(ambos usan la funcion pokemon_atrapado), load_csv prepara los datos y la llama por cada linea
	mientras que menu_pokemon_atrapado prepara los datos de la consola y luego la llama.

	- Evolucionar pokemon funciona como el enunciado dice (solo evoluciona si la evolucion
	ya esta en la pokedex (?) )
	Una posible solucion seria poner la opcion para cargar una especie de Pokedex.csv o algo asi,
	pero eso esta fuera del alcance de este trabajo.

	- Los buscar estan funcionando, permiten buscar ignorando mayusculas y permiten
	busquedas parciales (buscar Azu deberia mostrar tanto Azurril como Azumarril, etc.)
	NO funciona con partes en medio del string eso si.

	- Mostrar todos los pokemon en la pokedex funciona como se espera, pero hay que tener
	una consola de minimo 80 caracteres, sino el formato se ve mal.

	- Liberar pokemon recibe la id y funciona correctamente, pero seria ideal el mostrar los pokemon
	antes de pedir la id, para poder ver cual borrar, pero bueno, siempre se puede usar 8
	antes de liberar.

	mostrar por region tambien usa la busqueda parcial, pero seria ideal que
	mostrara una lista de las posibles regiones a buscar antes, pero bueno...

ASPECTOS POSITIVOS/NEGATIVOS

Jorge -> Trabaja rápido y es muy productivo, se maneja muy bien programando pero a veces avanza solo y los demás del grupo se pierden
Nathalia -> Da opiniones y pregunta si no entiende. Sin embargo, aún le cuesta mucho el implementar ejercicios con los tdas.
Ignacio -> No se queda con lo que sabe, investiga y aprende si no desconoce la información. Aún así, a veces es muy disperso y desconcentra a los demás pero aún así es buen aporte.
Antonia -> Ideas muy eficientes con respecto a la tarea pero a veces desaparece sin avisar.


