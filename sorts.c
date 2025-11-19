/*
 *  - Bubble Sort
 *  - Insertion Sort
 *  - Selection Sort
 *  - Merge Sort
 *  - Quick Sort
 *  - Shell Sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------- Funções utilitárias ---------------------- */

/* Troca dois elementos inteiros */
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Imprime um vetor de inteiros */
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

/* Copia um vetor  */
void copy_array(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

/* ---------------------- Bubble Sort ---------------------- */
/*
 Algoritmo de comparação simples.
 Repetidamente percorrer o vetor, trocando pares adjacentes fora de ordem. Cada passagem "borbulha" o maior elemento restante para o fim.
 */
void bubble_sort(int arr[], int n) {
    int swapped;
    /* Faz até n-1 passagens no máximo */
    for (int pass = 0; pass < n - 1; pass++) {
        swapped = 0; /* flag para detectar se já está ordenado */
        /* Em cada passagem, comparamos pares adjacentes até o item n-pass-1 */
        for (int i = 0; i < n - 1 - pass; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(&arr[i], &arr[i + 1]);
                swapped = 1;
            }
        }
        /* Se nenhuma troca ocorreu, o vetor está ordenado e podemos sair cedo */
        if (!swapped) break;
    }
}

/* ---------------------- Insertion Sort ---------------------- */
/*
Constrói uma sublista ordenada (à esquerda). Para cada novo elemento, insere-o na posição correta deslocando elementos maiores à direita.
 */
void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        /* Desloca os elementos maiores que 'key' uma posição à direita */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        /* Insere 'key' na posição correta */
        arr[j + 1] = key;
    }
}

/* ---------------------- Selection Sort ---------------------- */
/*
Para cada posição i, encontra o menor elemento do segmento [i..n-1] e troca com a posição i. Simples, mas ineficiente para grandes vetores.
*/
void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        /* procura o mínimo na porção não ordenada */
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        /* troca o mínimo encontrado com a posição i */
        if (min_idx != i) swap(&arr[i], &arr[min_idx]);
    }
}

/* ---------------------- Merge Sort ---------------------- */
/*
 Divide o vetor em duas metades, ordena recursivamente cada metade e então intercala ("merge") as duas metades ordenadas.
 */

/* Intercala dois subarrays ordenados:
 * arr[l..m] e arr[m+1..r] -> resultado em arr[l..r]
 */
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1; /* tamanho da primeira metade */
    int n2 = r - m;     /* tamanho da segunda metade */

    /* criar arrays temporários para copiar os valores */
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    if (!L || !R) {
        fprintf(stderr, "Erro de alocação em merge\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    /* Índices para percorrer L, R e arr */
    int i = 0, j = 0, k = l;
    /* Enquanto ambos tenham elementos, copie o menor para arr */
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    /* Se sobrar em L, copie o restante */
    while (i < n1) arr[k++] = L[i++];
    /* Se sobrar em R, copie o restante */
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

/* Função recursiva de Merge Sort */
void merge_sort_rec(int arr[], int l, int r) {
    if (l >= r) return; /* base: 0 ou 1 elemento já está ordenado */
    int m = l + (r - l) / 2;
    merge_sort_rec(arr, l, m);     /* ordena metade esquerda */
    merge_sort_rec(arr, m + 1, r); /* ordena metade direita */
    merge(arr, l, m, r);           /* intercala as duas metades */
}

void merge_sort(int arr[], int n) {
    merge_sort_rec(arr, 0, n - 1);
}

/* ---------------------- Quick Sort ---------------------- */
/*
Escolhe um pivô, particiona o vetor colocanco elementos <= pivô à esquerda e > pivô à direita, e recursivamente ordena as duas partes.
*/

/* Escolhe arr[high] como pivô e retorna o índice do pivô após partição. */
int lomuto_partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1; /* índice do último elemento <= pivot */
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]); /* coloca pivô na posição correta */
    return i + 1;
}

/* Recursão do Quick Sort */
void quick_sort_rec(int arr[], int low, int high) {
    if (low < high) {
        int p = lomuto_partition(arr, low, high);
        quick_sort_rec(arr, low, p - 1);
        quick_sort_rec(arr, p + 1, high);
    }
}

void quick_sort(int arr[], int n) {
    quick_sort_rec(arr, 0, n - 1);
}

/* ---------------------- Shell Sort ---------------------- */
/*
Usa uma sequência de "gaps" (intervalos). Para cada gap, realiza uma insertion sort considerando elementos separados por gap.
Com o passar dos gaps (reduzidos), o vetor fica cada vez mais próximo de ordenado, e o insertion sort final é rápido.
*/
void shell_sort(int arr[], int n) {
    /* gap inicial grande, depois reduzido pela metade até 1 */
    for (int gap = n / 2; gap > 0; gap /= 2) {
        /* Para cada posição 'i' começando em gap, fazer uma "insertion" por gap */
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            /* desloca elementos que estão gap posições atrás e são maiores que temp */
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

/* ---------------------- Função main (testes/demonstração) ---------------------- */

int main(void) {
    int original[] = {64, 34, 25, 12, 22, 11, 90, 0, -5, 50};
    int n = sizeof(original) / sizeof(original[0]);

    printf("Vetor original: ");
    print_array(original, n);
    printf("\n");

    int arr[n];

    /* Bubble Sort */
    copy_array(original, arr, n);
    bubble_sort(arr, n);
    printf("Bubble Sort:    ");
    print_array(arr, n);

    /* Insertion Sort */
    copy_array(original, arr, n);
    insertion_sort(arr, n);
    printf("Insertion Sort: ");
    print_array(arr, n);

    /* Selection Sort */
    copy_array(original, arr, n);
    selection_sort(arr, n);
    printf("Selection Sort: ");
    print_array(arr, n);

    /* Merge Sort */
    copy_array(original, arr, n);
    merge_sort(arr, n);
    printf("Merge Sort:     ");
    print_array(arr, n);

    /* Quick Sort */
    copy_array(original, arr, n);
    quick_sort(arr, n);
    printf("Quick Sort:     ");
    print_array(arr, n);

    /* Shell Sort */
    copy_array(original, arr, n);
    shell_sort(arr, n);
    printf("Shell Sort:     ");
    print_array(arr, n);

    return 0;
}
