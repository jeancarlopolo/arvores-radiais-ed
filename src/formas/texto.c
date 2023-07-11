#include "texto.h"

struct texto
{
    enum TipoForma tipo;
    int id;
    double x;
    double y;
    char conteudo[50]; // por favor, não coloque mais de 50 caracteres Evandro eu IMPLORO
    char *corBorda;
    char *corPreenchimento;
    char *ancora;
    double rotacao;
    char *textoFamilia;
    char *textoPeso;
    char *textoTamanho;
};


Texto criaTexto(int id, double x, double y, char *corBorda, char *corPreenchimento, char *ancora, char *conteudo, char *textoFamilia, char *textoPeso, char *textoTamanho)
{
    struct texto *t = (struct texto *)malloc(sizeof(struct texto));
    t->tipo = TEXTO;
    t->id = id;
    t->x = x;
    t->y = y;
    t->corBorda = corBorda;
    t->corPreenchimento = corPreenchimento;
    t->ancora = ancora;
    strncpy(t->conteudo, conteudo, 50);
    if (strlen(conteudo) > 50)
    {
        t->conteudo[49] = '\0';
    }
    t->rotacao = 0;
    t->textoFamilia = textoFamilia;
    t->textoPeso = textoPeso;
    t->textoTamanho = textoTamanho;
    return t;
}

int getTextoId(Texto t)
{
    return ((struct texto *)t)->id;
}

double getTextoX(Texto t)
{
    return ((struct texto *)t)->x;
}

double getTextoY(Texto t)
{
    return ((struct texto *)t)->y;
}

char *getTextoCorBorda(Texto t)
{
    return ((struct texto *)t)->corBorda;
}

char *getTextoCorPreenchimento(Texto t)
{
    return ((struct texto *)t)->corPreenchimento;
}

char *getTextoAncora(Texto t)
{
    return ((struct texto *)t)->ancora;
}

char *getTextoConteudo(Texto t)
{
    return ((struct texto *)t)->conteudo;
}

int getTextoLength(Texto t)
{
    return strlen(((struct texto *)t)->conteudo);
}

char *getTextoFamilia(Texto t)
{
    return ((struct texto *)t)->textoFamilia;
}

char *getTextoPeso(Texto t)
{
    return ((struct texto *)t)->textoPeso;
}

char *getTextoTamanho(Texto t)
{
    return ((struct texto *)t)->textoTamanho;
}

void setTextoId(Texto t, int id)
{
    ((struct texto *)t)->id = id;
}

void setTextoX(Texto t, double x)
{
    ((struct texto *)t)->x = x;
}

void setTextoY(Texto t, double y)
{
    ((struct texto *)t)->y = y;
}

void setTextoCorBorda(Texto t, char *corBorda)
{
    ((struct texto *)t)->corBorda = corBorda;
}

void setTextoCorPreenchimento(Texto t, char *corPreenchimento)
{
    ((struct texto *)t)->corPreenchimento = corPreenchimento;
}

void setTextoAncora(Texto t, char *ancora)
{
    ((struct texto *)t)->ancora = ancora;
}

void setTextoConteudo(Texto t, char *conteudo)
{
    strncpy(((struct texto *)t)->conteudo, conteudo, 50);
}

void setTextoFamilia(Texto t, char *textoFamilia)
{
    ((struct texto *)t)->textoFamilia = textoFamilia;
}

void setTextoPeso(Texto t, char *textoPeso)
{
    ((struct texto *)t)->textoPeso = textoPeso;
}

void setTextoTamanho(Texto t, char *textoTamanho)
{
    ((struct texto *)t)->textoTamanho = textoTamanho;
}

void liberaTexto(Texto t)
{
    struct texto *texto = (struct texto *)t;
    free(texto->corBorda);
    free(texto->corPreenchimento);
    free(texto->ancora);
    free(texto->textoFamilia);
    free(texto->textoPeso);
    free(texto->textoTamanho);
    free(texto);
}

void rotacionaTexto(Texto t, double theta)
{
    ((struct texto *)t)->rotacao += theta;
}

double getTextoRotacao(Texto t)
{
    return ((struct texto *)t)->rotacao;
}
