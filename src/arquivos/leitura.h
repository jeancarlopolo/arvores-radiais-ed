#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"
#include "../estruturas/radialtree.h"
#include "../estruturas/fila.h"

/** Conjunto de funções para leitura de arquivos. */

/** Função que lê uma linha de um arquivo e separa as palavras em um vetor de strings.
n é o número de palavras na linha. */
void ler_linha(FILE *arq, char *palavras[], int *n);

/** Função que lê o arquivo geo e chama as funções de inserção de elementos na lista. */
void ler_geo(FILE *geo, RadialTree db);

/** Função que lê o arquivo qry e chama as funções de remoção e alteração de elementos na lista. */
void ler_qry(FILE *qry, FILE *svg, FILE *txt, RadialTree db, char* nomeSvg);

#endif