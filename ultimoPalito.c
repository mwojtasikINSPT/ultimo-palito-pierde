#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define p printf

typedef struct nodo
{
    char valor; // '|'
    struct nodo *sig;
} nodo;

// --- Funciones de la Pila ---
nodo *inicializarPila(void);
void apilar(nodo **);
void desapilar(nodo **);
void mostrarPila(nodo *);
void vaciarPila(nodo **);

// --- Funciones de Configuración y Menú ---
void menuPrincipal();
void cambiarPalitosIniciales(int *);
void cambiarMaximoRetiro(int *, int);

// --- Funciones de Lógica del Juego ---
int jugarTurno(nodo **, int, const char *);
void jugarVsJugador(int, int);
void jugarVsCPU(int, int);

int pedirEntero(const char *);

int main(void)
{
    // Inicializo la semilla de números aleatorios para los movimientos de la CPU
    srand((unsigned int)time(NULL));

    menuPrincipal();

    return 0;
}

// MENU
void menuPrincipal()
{
    // Inicializo valores por defecto
    int palitosIniciales = 30;
    int maxRetiro = 3;
    char opcion;

    do
    {
        p("\n=========================================\n");
        p("       EL ULTIMO PALITO PIERDE          \n");
        p("=========================================\n");
        p("1. Jugar\n");
        p("2. Cambiar palitos iniciales (Actual: %d)\n", palitosIniciales);
        p("3. Cambiar maximo de palitos a sacar (Actual: %d)\n", maxRetiro);
        p("0. Salir\n");
        p("Seleccione una opcion: ");

        scanf(" %c", &opcion);
        // Limpio el buffer del teclado tras leer la opcion
        while (getchar() != '\n')
            ;

        switch (opcion)
        {
        case '1':
        {
            char modo;
            p("\n--- SELECCION DE MODO ---\n");
            p("1. Jugar vs Otro Jugador\n");
            p("2. Jugar vs CPU\n");
            p("Seleccione modo: ");
            scanf(" %c", &modo);
            while (getchar() != '\n')
                ;

            if (modo == '1')
            {
                jugarVsJugador(palitosIniciales, maxRetiro);
            }
            else if (modo == '2')
            {
                jugarVsCPU(palitosIniciales, maxRetiro);
            }
            else
            {
                p("Modo invalido.\n");
            }
            break;
        }
        case '2':
            cambiarPalitosIniciales(&palitosIniciales);
            break;
        case '3':
            cambiarMaximoRetiro(&maxRetiro, palitosIniciales);
            break;
        case '0':
            p("\n¡Gracias por jugar! Hasta luego.\n");
            break;
        default:
            p("\nOpcion invalida. Intente de nuevo.\n");
        }

    } while (opcion != '0');
}

// PILA
nodo *inicializarPila()
{
    // La pila arranca vacía
    return NULL;
}

void apilar(nodo **tope)
{
    // Reservo memoria dinámica para un nuevo nodo
    nodo *nuevo = (nodo *)malloc(sizeof(nodo));

    // Asigno el carácter que representa al palito
    nuevo->valor = '|';

    // Apunto el nuevo nodo al que antes era el tope
    nuevo->sig = *tope;

    // Actualizo el tope para que apunte a este nuevo nodo
    *tope = nuevo;
}

void desapilar(nodo **tope)
{
    // Verifico que la pila no esté vacía antes de intentar desapilar
    if (*tope != NULL)
    {
        // Guardo temporalmente la dirección del nodo del tope
        nodo *aux = *tope;

        // Muevo el tope hacia el siguiente nodo de abajo
        *tope = (*tope)->sig;

        // Libero la memoria del nodo que acabo de quitar
        free(aux);
    }
}

void mostrarPila(nodo *tope)
{
    // Creo un puntero auxiliar para recorrer la pila sin modificar el tope real
    nodo *actual = tope;

    p("\nPalitos restantes: ");

    // Recorro la pila nodo por nodo imprimiendo cada palito
    while (actual != NULL)
    {
        printf("%c", actual->valor);
        actual = actual->sig;
    }
    p("\n");
}

void vaciarPila(nodo **tope)
{
    // Mientras la pila contenga nodos, llamo a desapilar para ir liberando de a uno
    while (*tope != NULL)
    {
        desapilar(tope);
    }
}

void cambiarPalitosIniciales(int *palitosIniciales)
{
    char buffer[100];

    p("\nPalitos iniciales actuales: %d\n", *palitosIniciales);
    p("Ingrese la nueva cantidad de palitos (presione Enter para valor por defecto: 30): ");

    // Leo la linea completa para detectar si presiono Enter vacio
    fgets(buffer, sizeof(buffer), stdin);

    // Si el primer caracter es salto de linea, significa que presiono Enter sin ingresar datos
    if (buffer[0] == '\n')
    {
        *palitosIniciales = 30;
        p("Se restablecio el valor por defecto: 30 palitos.\n");
    }
    else
    {
        // Convierto el texto ingresado a un entero
        int nuevoValor = atoi(buffer);
        if (nuevoValor > 0)
        {
            *palitosIniciales = nuevoValor;
            p("Cantidad actualizada a %d palitos.\n", *palitosIniciales);
        }
        else
        {
            p("Valor invalido. Se mantiene el valor previo.\n");
        }
    }
}

void cambiarMaximoRetiro(int *maxRetiro, int palitosIniciales)
{
    char buffer[100];

    // Calculo el 10% de los palitos iniciales
    int maxPermitido = palitosIniciales * 0.10;

    // Si el 10% resulta menor a 3, el tope superior de la validacion se fija en 3
    if (maxPermitido < 3)
    {
        maxPermitido = 3;
    }

    int valido = 0;
    while (!valido)
    {
        p("\nMaximo de palitos a sacar actual: %d\n", *maxRetiro);
        p("Ingrese el nuevo maximo (entre 3 y %d, o Enter para por defecto: 3): ", maxPermitido);

        // Leo el ingreso del usuario
        fgets(buffer, sizeof(buffer), stdin);

        // Si el usuario presiono Enter de una
        if (buffer[0] == '\n')
        {
            *maxRetiro = 3;
            p("Se restablecio el valor por defecto: 3 palitos.\n");
            valido = 1;
        }
        else
        {
            // Si buffer tiene el texto "30\n", atoi "ASCII to Integer" lo transforma al número entero 30
            int nuevoValor = atoi(buffer);

            // Valido que este estrictamente entre 3 y el 10% calculado
            if (nuevoValor >= 3 && nuevoValor <= maxPermitido)
            {
                *maxRetiro = nuevoValor;
                p("Maximo de retiro actualizado a %d palitos.\n", *maxRetiro);
                valido = 1;
            }
            else
            {
                p("Error: El valor debe ser como minimo 3 y como maximo %d (10%% del total).\n", maxPermitido);
            }
        }
    }
}

// JUEGO
int jugarTurno(nodo **tope, int maxRetiro, const char *nombreJugador)
{
    int palitosARetirar = 0;

    // Contamos cuantos palitos quedan actualmente en la pila
    int disponibles = 0;
    nodo *actual = *tope;
    while (actual != NULL)
    {
        disponibles++;
        actual = actual->sig;
    }

    // El limite real a sacar en este turno no puede superar los palitos disponibles
    int limiteTurno = (maxRetiro < disponibles) ? maxRetiro : disponibles;

    if (strcmp(nombreJugador, "CPU") == 0)
    {
        palitosARetirar = (rand() % limiteTurno) + 1;
        p("\nTurno de CPU: decide retirar %d palito(s).\n", palitosARetirar);
    }
    else
    {
        // Mantenemos el bucle condicionado a los limites validos del turno
        while (palitosARetirar < 1 || palitosARetirar > limiteTurno)
        {
            p("\nTurno de %s. Cuantos palitos retiras? (1 a %d): ", nombreJugador, limiteTurno);
            palitosARetirar = pedirEntero("Error: Debe ingresar un numero entero valido.\n");

            // Si el numero es valido pero esta fuera del rango permitido
            if (palitosARetirar < 1 || palitosARetirar > limiteTurno)
            {
                p("Rango invalido. La cantidad debe estar entre 1 y %d.\n", limiteTurno);
            }
        }
    }

    // Desapilo la cantidad indicada
    for (int i = 0; i < palitosARetirar; i++)
    {
        desapilar(tope);
    }

    return palitosARetirar;
}

void jugarVsJugador(int palitosIniciales, int maxRetiro)
{
    nodo *pila = inicializarPila();

    // Apilo la cantidad inicial de palitos
    for (int i = 0; i < palitosIniciales; i++)
    {
        apilar(&pila);
    }

    int turno = 1; // 1 para Jugador 1, 2 para Jugador 2

    while (pila != NULL)
    {
        mostrarPila(pila);
        const char *jugadorActual = (turno == 1) ? "Jugador 1" : "Jugador 2";

        jugarTurno(&pila, maxRetiro, jugadorActual);

        // Si al desapilar la pila quedo vacia, el jugador que jugo acaba de sacar el ultimo palito
        if (pila == NULL)
        {
            p("\n=========================================\n");
            p("¡%s perdio! Se quedo con el ultimo palito.\n", jugadorActual);
            p("=========================================\n");
        }
        else
        {
            // Cambio de turno alternando entre 1 y 2
            turno = (turno == 1) ? 2 : 1;
        }
    }

    // Aseguro la liberacion de la memoria si hubiese quedado algo
    vaciarPila(&pila);
}

void jugarVsCPU(int palitosIniciales, int maxRetiro)
{
    nodo *pila = inicializarPila();

    // Apilo los palitos iniciales
    for (int i = 0; i < palitosIniciales; i++)
    {
        apilar(&pila);
    }

    int turno = 1; // 1 para Jugador 1, 2 para CPU

    while (pila != NULL)
    {
        mostrarPila(pila);
        const char *jugadorActual = (turno == 1) ? "Jugador 1" : "CPU";

        jugarTurno(&pila, maxRetiro, jugadorActual);

        if (pila == NULL)
        {
            p("\n=========================================\n");
            p("¡%s perdio! Se quedo con el ultimo palito.\n", jugadorActual);
            p("=========================================\n");
        }
        else
        {
            turno = (turno == 1) ? 2 : 1;
        }
    }

    vaciarPila(&pila);
}

// AUXILIARES
int pedirEntero(const char *mensajeError)
{
    char buffer[100];
    int valido = 0;
    int numero = 0;

    while (!valido)
    {
        // Leo la linea completa del teclado como texto
        if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        {
            int i = 0;

            // Si el usuario solo presiono Enter sin ingresar nada
            if (buffer[0] == '\n')
            {
                p("%s", mensajeError);
                continue;
            }

            // Recorro la cadena verificando que todos los caracteres sean numeros
            valido = 1;
            while (buffer[i] != '\n' && buffer[i] != '\0')
            {
                if (buffer[i] < '0' || buffer[i] > '9')
                {
                    valido = 0; // Algún caracter no es un digito
                    break;
                }
                i++;
            }

            // Si todos eran digitos, lo convierto a entero con atoi
            if (valido)
            {
                numero = atoi(buffer);
            }
            else
            {
                p("%s", mensajeError);
                p("Ingrese un numero valido: ");
            }
        }
    }
    return numero;
}