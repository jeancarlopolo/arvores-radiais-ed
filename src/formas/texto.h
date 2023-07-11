#ifndef TEXTO_H_
#define TEXTO_H_

#include "tipos.h"
#include <string.h>

// Um texto possui id, x, y, cor de borda, cor de preenchimento, âncora e conteúdo.
// Além disso, o texto v_O_v representa um balão e o texto |-T-| representa um caça.
typedef void *Texto;

/** Cria um texto com os parâmetros dados. */
Texto criaTexto(int id, double x, double y, char *corBorda, char *corPreenchimento, char *ancora, char *conteudo, char *textoFamilia, char *textoPeso, char *textoTamanho);

/** Retorna o id do texto. */
int getTextoId(Texto t);

/** Retorna o x do texto. */
double getTextoX(Texto t);

/** Retorna o y do texto. */
double getTextoY(Texto t);

/** Retorna a cor de borda do texto. */
char *getTextoCorBorda(Texto t);

/** Retorna a cor de preenchimento do texto. */
char *getTextoCorPreenchimento(Texto t);

/** Retorna a âncora do texto. */
char *getTextoAncora(Texto t);

/** Retorna o conteúdo do texto. */
char *getTextoConteudo(Texto t);

/** Retorna a família do texto. */
char *getTextoFamilia(Texto t);

/** Retorna o peso do texto. */
char *getTextoPeso(Texto t);

/** Retorna o tamanho do texto. */
char *getTextoTamanho(Texto t);

/** Retorna o número de caracteres do texto. */
int getTextoLength(Texto t);

/** Retorna a rotação do texto. */
double getTextoRotacao(Texto t);

/** Define o id do texto. */
void setTextoId(Texto t, int id);

/** Define o x do texto. */
void setTextoX(Texto t, double x);

/** Define o y do texto. */
void setTextoY(Texto t, double y);

/** Define a cor de borda do texto. */
void setTextoCorBorda(Texto t, char *corBorda);

/** Define a cor de preenchimento do texto. */
void setTextoCorPreenchimento(Texto t, char *corPreenchimento);

/** Define a âncora do texto. */
void setTextoAncora(Texto t, char *ancora);

/** Define o conteúdo do texto. */
void setTextoConteudo(Texto t, char *conteudo);

/** Define a família do texto. */
void setTextoFamilia(Texto t, char *textoFamilia);

/** Define o peso do texto. */
void setTextoPeso(Texto t, char *textoPeso);

/** Define o tamanho do texto. */
void setTextoTamanho(Texto t, char *textoTamanho);

/** Rotaciona o texto em torno do ponto (x, y) em um ângulo de theta graus. */
void rotacionaTexto(Texto t, double theta);

/** Libera a memória alocada para o texto. */
void liberaTexto(Texto t);


#endif