#include <stdio.h>
#include <stdlib.h>
#include "../headers/sort.h"

/**
 * Conteo de operaciones:
 * - Operaciones básicas cuentan como 1 operación.
 * - Operaciones con asignación son 2 operaciones.
 * - Relacionales considerar peores casos.
 * - Crear una variable cuenta como 1 operación.
 * - Extraer el valor de un arreglo es 1 operación.
 * - Asignar un valor a un arreglo es 1 operación.
 * - Llamar a una función es hacer f(g(x)), es decir según que
 *   función sea varia a su función en terminos de operaciones.
 * - Una condicional if no introduce operaciones extras, pero se
 *   cuenta el peor de los casos, es decir evaluar todos los ifs.
 * - Un switch tiene un coste constante, más el coste de la peor rama.
 * - For, dado a que es una iteración, tenemos que ver si depende o no
 *   de la entrada, si no, entonces se cuentan las operaciones por el
 *   número de repeticiones, si no, se deja en terminos de n.
 */

/**
 * bubble_sort(n) = 13n^2 + 17/2n + 3
 */
void bubble_sort(int n, int* array) {
    int swap; // 1 Operación.
    // (11n^2 + 11n)/2 + 1 + 1 + 1 + n + n + ((n-1) n)
    for(int i = 0 /* 1 Operación */; i < n /* n Operaciones */; i++ /* n Operaciones */) {
        // i = 0 -> 1 Operación
        // i = 1 -> 2 Operaciones + 1 operación = 3 operaciones
        // i = 2 -> 3 Operaciones + 2 operación = 5 operaciones
        // i = 3 -> 4 Operaciones + 3 operación = 7 operaciones
        // i = 4 -> 5 Operaciones + 4 operación = 9 operaciones
        // ...
        // i = n -> (n + 1) + n = 2n + 1
        for(int j = i /* 1 Operación */; j > 0 /* i */; j-- /* i Operaciones */) {
            // Sum(i=0 hasta i=n) = ((n-1) n)/2
            // 11 ((n-1)n)/2 = (11n^2 + 11n)/2
            if(array[j] < array[j - 1] /* 4 Operaciones */) { // 11 Operaciones del if
                swap = array[j]; // 2 operaciones
                array[j] = array[j - 1]; // 3 operaciones
                array[j - 1] = swap; // 2 operaciones
            }
        }
    }
}

// 6n + 2
static int mina(int n, int *array) {
    int min = INT_MAX; // 1

    for(int i = 0 /*1*/; i < n/*n*/; i++/*n*/) {
        if(array[i] < min) { // 2
            min = array[i]; // 2
        }
    }

    return min;
}

// 6n + 2
static int maxa(int n, int *array) {
    int max = INT_MIN;

    for(int i = 0; i < n; i++) {
        if(array[i] > max) {
            max = array[i];
        }
    }

    return max;
}

/**
 * Arreglos con muchos datos repetidos.
 * 
 * Ocupa mucho espacio en memoria y desperdicia más si los valores
 * son muy dispares.
 * 
 * 5n + x + 6n + 2 + 6n + 2 + 3 + n + 5n + 1 + 2x + 1 + 1
 * 
 * counting_sort(n) = 23n + 3x + 9; x = max(n) - min(n) + 1
 * 
 * Si los datos están muy dispersos, entonces x es muy grande.
 * Si los datos están muy comprimidos, entonces x es despreciable.
 */
void counting_sort(int n, int* array) {
    int min = mina(n, array); // 6n + 2
    int max = maxa(n, array); // 6n + 2
    int size = max - min + 1; // 3

    int *counter = (int*) calloc(size,  sizeof(int)); // n
    for(int i = 0; i < n; i++) { // 1 + n + n + n*(3) = 5n + 1
        counter[array[i] - min]++; // 3
    }

    int j = 0; // 1
    for(int i = 0; i < size; i++) { // 1 + x + x
        while(counter[i]) { // x + n + n * (4) = 5n + x
            array[j++] = i + min; // 3
            counter[i]--; // 1
        }
    }
}

/**
 * O(n^2)
 */
void insertion_sort(int n, int *array) {
    for(int i = 1; i < n; i++) {
        // 2 7 4 8 1; key = 7
        // key = 4; 2 7 7 8 1 -> 2 4 7 8 1
        // key = 8; 2 4 7 8 1
        // key = 1; 2 4 7 8 8 -> 2 4 7 7 8 -> 2 4 4 7 8 -> 2 2 4 7 8 -> 1 2 4 7 8
        int key = array[i];
        int j = i - 1;
        while(j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

/**
 * O(n^2)
 */
void selection_sort(int n, int *array) {
    int swap;
    for(int i = 0; i < n; i++) {
        /**
         * 10 9 8 7 6; minidx = 4
         * 6 9 8 7 10; minidx = 3
         * 6 7 8 9 10
         */
        int minidx = i;
        for(int j = i + 1; j < n; j++) {
            if(array[j] < array[minidx]) {
                minidx = j;
            }
        }
        
        if(minidx == i) {
            continue;
        }

        swap = array[i];
        array[i] = array[minidx];
        array[minidx] = swap;
    }
}