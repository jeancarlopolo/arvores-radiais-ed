#ifndef HIGHORDER_H
#define HIGHORDER_H

#include "radialtree.h"
#include "../formas/tipos.h"
#include "../formas/retangulo.h"
#include "../formas/circulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"
#include "../arquivos/svg.h"

// Função que imprime o svg da forma de acordo com o tipo
void printSvg(Info i, double x, double y, void *aux);

#endif