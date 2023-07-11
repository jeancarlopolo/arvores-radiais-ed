#include "radialtree.h"
#include <math.h>
#include "fila.h"
#include "../formas/tipos.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/linha.h"
#include "../formas/texto.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

double xCentro, yCentro;

struct node
{
    Info info;
    double x, y;
    struct node **filhos;
    bool removido;
};

struct arvore
{
    int numSetores;
    double fd;
    int removidos;
    int total;
    struct node *raiz;
};

struct polar
{
    double r, theta;
};

struct menorRetangulo
{
    double x1, y1, x2, y2;
};

struct vetorNos
{
    struct node **nos;
    int indice;
};

RadialTree newRadialTree(int numSetores, double fd)
{
    struct arvore *t = malloc(sizeof(struct arvore));
    t->numSetores = numSetores;
    fd = fd < 0 ? 0 : fd;
    fd = fd > 1 ? 0.9 : fd;
    t->fd = fd;
    t->removidos = 0;
    t->total = 0;
    t->raiz = NULL;
    return t;
}

int checkSetor(double xCentro, double yCentro, double x, double y, int numSetores)
{
    double angulo = atan2(y - yCentro, x - xCentro);
    angulo = angulo < 0 ? angulo + 2 * M_PI : angulo;
    int setor = (int)(angulo / (2 * M_PI / numSetores));
    return setor;
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    struct arvore *arv = t;
    struct node *novo = malloc(sizeof(struct node));
    novo->info = i;
    novo->x = x;
    novo->y = y;
    novo->removido = false;
    novo->filhos = calloc(arv->numSetores, sizeof(struct node));
    if (arv->raiz == NULL)
    {
        arv->raiz = novo;
        return novo;
    }
    struct node *aux = arv->raiz;
    while (1)
    {
        int setor = checkSetor(aux->x, aux->y, x, y, arv->numSetores);
        if (aux->filhos[setor] == NULL)
            break;
        aux = aux->filhos[setor];
    }
    int setor = checkSetor(aux->x, aux->y, x, y, arv->numSetores);
    aux->filhos[setor] = novo;
    arv->total++;
    return novo;
}

Node getNodeRadialT(RadialTree t, double x, double y, double epsilon)
{
    struct arvore *arv = t;
    struct node *aux = arv->raiz;
    while (aux != NULL)
    {
        if (fabs(aux->x - x) < epsilon && fabs(aux->y - y) < epsilon)
            return aux == NULL || aux->removido ? NULL : aux;
        int setor = checkSetor(aux->x, aux->y, x, y, arv->numSetores);
        aux = aux->filhos[setor];
    }
    return aux;
}

// funçao fvisitano para achar o menor retângulo que contém todos os pontos não removidos
void acharMenor(Info i, double x, double y, void *aux)
{
    struct menorRetangulo *mr = aux;
    struct node *n = i;
    if (x < mr->x1)
        mr->x1 = x;
    if (x > mr->x2)
        mr->x2 = x;
    if (y < mr->y1)
        mr->y1 = y;
    if (y > mr->y2)
        mr->y2 = y;
}

// funçao fvisitano para colocar todos os elementos não removidos em um vetor
void colocarNoVetor(Info i, double x, double y, void *aux)
{
    struct vetorNos *vn = aux;
    struct node *n = i;
    vn->nos[vn->indice] = n;
    vn->indice++;
}

// função de comparação para o qsort (ordem crescente de distância do centro)
int comparaNode(const void *a, const void *b)
{
    struct node *n1 = *(struct node **)a;
    struct node *n2 = *(struct node **)b;
    double d1 = sqrt(pow(n1->x - xCentro, 2) + pow(n1->y - yCentro, 2));
    double d2 = sqrt(pow(n2->x - xCentro, 2) + pow(n2->y - yCentro, 2));
    if (d1 < d2)
        return -1;
    if (d1 > d2)
        return 1;
    return 0;
}

void _reorganizaRadialT(RadialTree t)
{
    // acha o menor retângulo que contém todos os pontos não removidos
    struct arvore *arv = t;
    struct node *aux = arv->raiz;
    struct menorRetangulo *mr = malloc(sizeof(struct menorRetangulo));
    visitaProfundidadeRadialT(t, acharMenor, mr);
    // acha o centro do menor retângulo
    xCentro = (mr->x1 + mr->x2) / 2;
    yCentro = (mr->y1 + mr->y2) / 2;
    free(mr);
    // coloca todos os elementos não removidos em um vetor
    struct vetorNos *vn = malloc(sizeof(struct vetorNos));
    vn->nos = malloc(sizeof(struct node) * arv->total);
    vn->indice = 0;
    visitaProfundidadeRadialT(t, colocarNoVetor, vn);
    qsort(vn->nos, vn->indice, sizeof(struct node), comparaNode);
    struct arvore *novaArvore = newRadialTree(arv->numSetores, arv->fd);
    for (int i = 0; i < vn->indice; i++)
    {
        struct node *n = vn->nos[i];
        insertRadialT(novaArvore, n->x, n->y, n->info);
    }
    free(vn->nos);
    t = novaArvore;
    killRadialTree(arv);
}

void removeNoRadialT(RadialTree t, Node n)
{
    struct arvore *arv = t;
    struct node *aux = n;
    aux->removido = true;
    arv->removidos++;
    if (arv->removidos / arv->total > arv->fd)
        _reorganizaRadialT(t);
}

Info getInfoRadialT(RadialTree t, Node n)
{
    struct node *aux = n;
    return aux->info;
}

bool checkLinhaLinha(double x1, double y1, double x2, double y2,
                     double x3, double y3, double x4, double y4)
{
    // Verifica se a linha intersecta a linha utilizando o algoritmo de segmentos de reta
    double dx12 = x2 - x1;
    double dy12 = y2 - y1;
    double dx34 = x4 - x3;
    double dy34 = y4 - y3;
    double denominator = (dy12 * dx34 - dx12 * dy34);
    if (denominator == 0)
        return false;
    double dx31 = x1 - x3;
    double dy31 = y1 - y3;
    double numerator1 = (dy12 * dx31 - dx12 * dy31);
    double numerator2 = (dy34 * dx31 - dx34 * dy31);
    double mu1 = numerator1 / denominator;
    double mu2 = numerator2 / denominator;
    return (mu1 >= 0 && mu1 <= 1) && (mu2 >= 0 && mu2 <= 1);
}

bool _nodesRegiaoRecursivo(struct node *n, double x1, double y1, double x2, double y2, Lista L, int numSetores)
{
    bool achou = false;
    if (n->x >= x1 && n->x <= x2 && n->y >= y1 && n->y <= y2)
    {
        if (!(n->removido))
        {
            insertLst(L, n);
            achou = true;
        }
    }
    int setor1 = checkSetor(x1, y1, n->x, n->y, numSetores);
    int setor2 = checkSetor(x2, y1, n->x, n->y, numSetores);
    int setor3 = checkSetor(x1, y2, n->x, n->y, numSetores);
    int setor4 = checkSetor(x2, y2, n->x, n->y, numSetores);

    // calcula se algum lado do retangulo intercepta o setor
    for (int i = 0; i < numSetores; i++)
    {
        bool intersecta = false;
        struct polar setorPolar; // reta do setor em coordenadas polares
        setorPolar.r = 3000;     // valor arbitrario gambiarra rsrsrs
        setorPolar.theta = (360 / numSetores) * i;
        // transforma a reta do setor em coordenadas cartesianas
        double xReta = setorPolar.r * cos(setorPolar.theta);
        double yReta = setorPolar.r * sin(setorPolar.theta);
        // verifica se a reta do setor intercepta algum lado do retangulo
        intersecta = checkLinhaLinha(x1, y1, x2, y1, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x2, y1, x2, y2, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x2, y2, x1, y2, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x1, y2, x1, y1, n->x, n->y, xReta, yReta);
        bool dentro = false;
        // verifica se algum vertice do retangulo esta dentro do setor
        if (setor1 == i || setor2 == i || setor3 == i || setor4 == i)
            dentro = true;
        // se não intercepta e não esta dentro, vai pro proximo setor
        if (!intersecta && !dentro)
            continue;
        if (n->filhos[i] == NULL)
            continue;
        if (n->filhos[i]->info != NULL)
            achou = _nodesRegiaoRecursivo(n->filhos[i], x1, y1, x2, y2, L, numSetores) || achou;
    }
    return achou;
}

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
    struct arvore *arv = t;
    return _nodesRegiaoRecursivo(arv->raiz, x1, y1, x2, y2, L, arv->numSetores);
}

bool _infosRegiaoRecursivo(struct node *n, double x1, double y1, double x2, double y2,
                           FdentroDeRegiao f, Lista L, int numSetores)
{
    bool achou = false;
    if (f(n->info, x1, y1, x2, y2))
    {
        if (!(n->removido))
        {
            insertLst(L, n->info);
            achou = true;
        }
    }
    int setor1 = checkSetor(x1, y1, n->x, n->y, numSetores);
    int setor2 = checkSetor(x2, y1, n->x, n->y, numSetores);
    int setor3 = checkSetor(x1, y2, n->x, n->y, numSetores);
    int setor4 = checkSetor(x2, y2, n->x, n->y, numSetores);

    // calcula se algum lado do retangulo intercepta o setor
    for (int i = 0; i < numSetores; i++)
    {
        bool intersecta = false;
        struct polar setorPolar; // reta do setor em coordenadas polares
        setorPolar.r = 3000;     // valor arbitrario gambiarra rsrsrs
        setorPolar.theta = (360 / numSetores) * i;
        // transforma a reta do setor em coordenadas cartesianas
        double xReta = setorPolar.r * cos(setorPolar.theta);
        double yReta = setorPolar.r * sin(setorPolar.theta);
        // verifica se a reta do setor intercepta algum lado do retangulo
        intersecta = checkLinhaLinha(x1, y1, x2, y1, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x2, y1, x2, y2, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x2, y2, x1, y2, n->x, n->y, xReta, yReta) ||
                     checkLinhaLinha(x1, y2, x1, y1, n->x, n->y, xReta, yReta);
        bool dentro = false;
        // verifica se algum vertice do retangulo esta dentro do setor
        if (setor1 == i || setor2 == i || setor3 == i || setor4 == i)
            dentro = true;
        // se não intercepta e não esta dentro, vai pro proximo setor
        if (!intersecta && !dentro)
            continue;
        if (n->filhos[i] == NULL)
            continue;
        if (n->filhos[i]->info != NULL)
            achou = _nodesRegiaoRecursivo(n->filhos[i], x1, y1, x2, y2, L, numSetores) || achou;
    }
    return achou;
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
                                 FdentroDeRegiao f, Lista L)
{
    struct arvore *arv = t;
    return _infosRegiaoRecursivo(arv->raiz, x1, y1, x2, y2, f, L, arv->numSetores);
}

bool _infosPontoRecursivo(struct node *n, double x, double y, FpontoInternoAInfo f, Lista L, int numSetores)
{
    bool achou = false;
    if (f(n->info, x, y))
    {
        if (!(n->removido))
        {
            insertLst(L, n->info);
            achou = true;
        }
    }
    // não faço ideia de como podar a busca desse
    // pode ter um retângulão que engloba tudo no setor 8952389532 e o ponto no setor 2 e é pra pegar
    // genuinamente nao tem como podar a busca desse sem colocar um monte de função específica das formas

    for (int i = 0; i < numSetores; i++)
    {
        if (n->filhos[i] == NULL)
            continue;
        if (n->filhos[i]->info != NULL)
            achou = _infosPontoRecursivo(n->filhos[i], x, y, f, L, numSetores) || achou;
    }
    return achou;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L)
{
    struct arvore *arv = t;
    return _infosPontoRecursivo(arv->raiz, x, y, f, L, arv->numSetores);
}

void _visitaProfundidadeRecursivo(struct node *n, FvisitaNo f, void *aux, int numSetores)
{
    if (!n->removido)
        f(n->info, n->x, n->y, aux);
    for (int i = 0; i < numSetores; i++)
    {
        if (n->filhos[i] != NULL)
            if (n->filhos[i]->info != NULL)
                _visitaProfundidadeRecursivo(n->filhos[i], f, aux, numSetores);
    }
}

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    struct arvore *arv = t;
    _visitaProfundidadeRecursivo(arv->raiz, f, aux, arv->numSetores);
}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    struct arvore *arv = t;
    Fila fila = criaFila();
    enqueue(fila, arv->raiz);
    while (!filaVazia(fila))
    {
        struct node *n = dequeue(fila);
        if (!n->removido)
            f(n->info, n->x, n->y, aux);
        for (int i = 0; i < arv->numSetores; i++)
        {
            if (n->filhos[i] != NULL)
                if (n->filhos[i]->info != NULL)
                    enqueue(fila, n->filhos[i]);
        }
    }
}

Node procuraNoRadialT(RadialTree t, FsearchNo f, void *aux)
{
    struct arvore *arv = t;
    Fila fila = criaFila();
    enqueue(fila, arv->raiz);
    while (!filaVazia(fila))
    {
        struct node *n = dequeue(fila);
        if (f(n->info, n->x, n->y, aux))
            return n;
        for (int i = 0; i < arv->numSetores; i++)
        {
            if (n->filhos[i] != NULL)
                if (n->filhos[i]->info != NULL)
                    enqueue(fila, n->filhos[i]);
        }
    }
    return NULL;
}

void _printDotRecursivo(FILE *arq, struct node *n, int numSetores)
{
    char *corp, *corb;
    switch (getTipoForma(n->info))
    {
    case RETANGULO:
        corp = getRetanguloCorPreenchimento(n->info);
        corb = getRetanguloCorBorda(n->info);
        break;
    case CIRCULO:
        corp = getCirculoCorp(n->info);
        corb = getCirculoCorb(n->info);
        break;
    case TEXTO:
        corp = getTextoCorPreenchimento(n->info);
        corb = getTextoCorBorda(n->info);
        break;
    case LINHA:
        corp = getLinhaCor(n->info);
        corb = getLinhaCor(n->info);
        break;
    }
    for (int i = 0; i < numSetores; i++)
    {
        fprintf(arq, "%d", getIdForma(n->info));
        fprintf(arq, " [label=\"%d", getIdForma(n->info));
        fprintf(arq, "\", style = \"filled");
        fprintf(arq, "\", color = \"%s", corb);
        fprintf(arq, "\", fillcolor = \"%s", corp);
        if (strcmp(corp, "black") == 0)
            fprintf(arq, "\", fontcolor = \"white");
        fprintf(arq, "\"];\n");
        if (n->filhos[i] != NULL)
        {
            fprintf(arq, "%d", getIdForma(n->info));
            fprintf(arq, " -> ");
            fprintf(arq, "%d", getIdForma(n->filhos[i]->info));
            fprintf(arq, " [label=\"%d", i);
            fprintf(arq, "\"]");
            fprintf(arq, ";\n");
            _printDotRecursivo(arq, n->filhos[i], numSetores);
        }
    }
}

bool printDotRadialTree(RadialTree t, char *fn)
{
    FILE *arq = fopen(fn, "w");
    if (arq == NULL)
        return false;
    struct arvore *arv = t;
    fprintf(arq, "digraph G {\n");
    fprintf(arq, "node [shape=box];\n");
    fprintf(arq, "graph [rankdir=TB];\n");
    fprintf(arq, "graph [center=true];\n");
    fprintf(arq, "splines = false;\n");
    fprintf(arq, "graph [bgcolor=white];\n");
    _printDotRecursivo(arq, arv->raiz, arv->numSetores);
    fprintf(arq, "}\n");
    fclose(arq);
    return true;
}

void killRadialTree(RadialTree t)
{
    struct arvore *arv = t;
    free(arv);
}