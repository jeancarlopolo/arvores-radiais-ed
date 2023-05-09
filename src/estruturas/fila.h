#ifndef FILA_H
#define FILA_H

#include "lista.h"

// Estrutura de dados que representa uma fila
typedef void* Fila;

// Cria uma fila
Fila criaFila();

// Insere um item no final da fila
void enqueue(Fila f, Item item);

// Remove o primeiro item da fila
Item dequeue(Fila f);

// Retorna se a fila está vazia
int filaVazia(Fila f);

// Retorna o tamanho da fila
void destroiFila(Fila f);

#endif
