# 🎮 El Último Palito Pierde (Juego en C)

Implementación en lenguaje **C** del clásico juego de estrategia *"El Último Palito Pierde"*. El proyecto fue desarrollado aplicando **estructuras de datos dinámicas (Pilas - LIFO)**, **gestión explícita de memoria en el Heap** y una arquitectura **modular con funciones fuera del `main`**.

---

## 📌 Descripción del Juego

Dos jugadores (o un Jugador vs. CPU) se turnan para retirar entre 1 y $N$ palitos de una pila. El objetivo es forzar al oponente a tomar la última pieza: **el jugador que retira el último palito pierde la partida**.

* **Palitos Iniciales (Default):** 30 (se puede cambiar)
* **Máximo a retirar por turno (Default):** 3 (se puede cambiar)
* **Representación Visual:** La pila se renderiza dinámicamente como barras verticales (`|`).

---

##  👩🏽‍💻Conceptos de C Aplicados

### 1. 📚Pila Dinámica (LIFO - *Last In, First Out*)
La pila se modela como una lista enlazada simple donde todas las inserciones y eliminaciones ocurren en el **tope**.

* **Nodos Dinámicos:** Cada palito del juego representa una estructura `nodo` asignada dinámicamente.
  ```c
  typedef struct nodo {
      char valor;        // Representa el palito '|'
      struct nodo* sig;  // Puntero al nodo inferior en la pila
  } nodo;

* **Comportamiento LIFO:** Los palitos agregados al inicio (`apilar`) quedan en la cima de la pila y son los primeros en ser removidos (`desapilar`) durante los turnos.  


### 2. 🧠Gestión Explícita de Memoria (`malloc` y `free`)

* **Reserva de Memoria:** Al iniciar la partida o configurar la pila, se utiliza `malloc(sizeof(nodo))` para crear los palitos en la memoria **Heap**, evitando desperdiciar memoria estática.
* **Liberación Segura de Memoria:** Durante la ejecución (al desapilar) y al terminar la partida (`vaciarPila`), se libera la memoria de cada nodo con `free()` mediante variables auxiliares para **prevenir Fugas de Memoria (*Memory Leaks*) y Punteros Colgantes (*Dangling Pointers*)**.


### 3. 🪆Punteros Dobles (`nodo**`)

Para lograr una modularización real fuera del `main`, las funciones que modifican la estructura de la pila reciben la dirección del puntero del tope (puntero a puntero):

* Permite modificar el puntero `tope` original por **referencia** y no por valor.
* Evita la pérdida del estado de la pila al salir del ámbito de las funciones de manipulación.


### 4. ⌨️ Robustez en la Entrada de Datos (Validación de Buffer)

Se implementó una función personalizada `pedirEntero()` que reemplaza el uso frágil de `scanf()` por `fgets()` y `atoi()`. Esto previene ciclos infinitos (*bucle por error de lectura*) ante entradas no numéricas o saltos de línea vacíos (`Enter`).


### 5. 🛠️ Estructura de Funciones (Modularidad) 
             
El programa cuenta con un diseño modular compuesto por 11 funciones independientes:

| Categoría | Función | Descripción |
| --- | --- | --- |
| **Gestión de Pila** | `inicializarPila()` | Retorna un puntero nulo (`NULL`) para inicializar la pila. |
| **Gestión de Pila** | `apilar(nodo**)` | Reserva memoria y agrega un nuevo palito al tope. |
| **Gestión de Pila** | `desapilar(nodo**)` | Quita el nodo del tope y libera su memoria con `free()`. |
| **Gestión de Pila** | `mostrarPila(nodo*)` | Recorre e imprime la serie de barras verticales restantes. |
| **Gestión de Pila** | `vaciarPila(nodo**)` | Libera de forma completa y segura todos los nodos al finalizar la partida. |
| **I/O y Validación** | `pedirEntero(const char*)` | Captura entradas vía `fgets`, valida caracteres numéricos y previene cierres colgados. |
| **I/O y Validación** | `cambiarPalitosIniciales(int*)` | Permite reconfigurar la cantidad total de palitos iniciales (soporta valor por defecto). |
| **I/O y Validación** | `cambiarMaximoRetiro(int*, int)` | Configura el límite a retirar (mínimo 3 y máximo 10% del total inicial). |
| **Lógica del Juego** | `jugarTurno(nodo**, int, const char*)` | Ejecuta la jugada actual, valida rangos y llama a `desapilar()`. |
| **Lógica del Juego** | `jugarVsJugador(int, int)` | Controla el flujo completo en modo multijugador local (Jugador 1 vs Jugador 2). |
| **Lógica del Juego** | `jugarVsCPU(int, int)` | Controla el flujo de juego frente a la computadora (movimientos mediante `rand()`). |



### 6. 🚀 Compilación y Ejecución

Para compilar y ejecutar el proyecto desde la terminal utilizando GCC:

**Compilar el código fuente**

```bash
gcc -o ultimoPalito ultimoPalito.c
```

**Compilar el código fuente**

```bash
./ultimoPalito
```