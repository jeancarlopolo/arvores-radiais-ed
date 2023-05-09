#include "radialtree.h"

struct radialtree;

struct node
{
    struct radialtree *arvorePertencente;
    Item item;
    double x;
    double y;
    bool deletado;
    struct node *pai;
    struct node **filhos;
};

struct radialtree
{
    struct node *raiz;
    double fdAtual; // removidos / total
    double fdMax;
    int nFilhos;
    int nNodes;
    int nRemovidos;
};

RadialTree newRadialTree(int numSetores, double fd)
{
    struct radialtree *arvore = (struct radialtree *)malloc(sizeof(struct radialtree));
    arvore->raiz = NULL;
    arvore->fdAtual = 0;
    fd = fd > 0 ? fd : 0;    // fd deve ser maior que 0
    fd = fd < 1 ? fd : 0.75; // fd deve ser menor que 1
    arvore->fdMax = fd;
    arvore->nFilhos = numSetores;
    arvore->nNodes = 0;
    return arvore;
}

int getNumSetores(RadialTree T)
{
    struct radialtree *arvore = (struct radialtree *)T;
    return arvore->nFilhos;
}

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    if (node != NULL)
    {
        f(node->item, node->x, node->y, aux);
        for (int i = 0; i < arvore->nFilhos; i++)
        {
            if (node->filhos[i] != NULL)
                visitaProfundidadeRadialT(node->filhos[i], f, aux);
        }
    }
}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = arvore->raiz;
    if (node != NULL)
    {
        Fila fila = criaFila();
        enqueue(fila, node);
        while (!filaVazia(fila))
        {
            node = (struct node *)dequeue(fila);
            f(node->item, node->x, node->y, aux);
            for (int i = 0; i < arvore->nFilhos; i++)
            {
                if (node->filhos[i] != NULL)
                    enqueue(fila, node->filhos[i]);
            }
        }
    }
}

int checkSetor(Node n, double x, double y)
{
    struct node *node = (struct node *)n;
    // Calcula a inclinação da reta entre o nó e o ponto (x,y)
    double inclinacao = atan2(y - node->y, x - node->x) * 180 / M_PI;
    if (inclinacao < 0)
    {
        inclinacao += 360;
    }
    // Calcula o número do setor a partir da inclinação
    int setor = (int)(inclinacao / 360 * node->arvorePertencente->nFilhos);
    // Retorna o número do setor
    return setor;
}

double calculaFd(RadialTree t)
{
    struct radialtree *arvore = (struct radialtree *)t;
    return arvore->nRemovidos / arvore->nNodes;
}

Node _criaNode(double x, double y, Info i, RadialTree t)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    struct radialtree *arvore = (struct radialtree *)t;
    node->item = i;
    node->x = x;
    node->y = y;
    node->deletado = false;
    node->arvorePertencente = t;
    node->pai = NULL;
    node->filhos = (struct node **)malloc(sizeof(struct node *) * arvore->nFilhos);
    for (int i = 0; i < arvore->nFilhos; i++)
    {
        node->filhos[i] = NULL;
    }
    return node;
}

Node _insertRecursivo(Node n, double x, double y, Info i)
{
    struct node *node = (struct node *)n;
    int setor = checkSetor(node, x, y);
    if (node->filhos[setor] == NULL)
    {
        node->filhos[setor] = _criaNode(x, y, i, node->arvorePertencente);
        node->filhos[setor]->pai = node;
        return node->filhos[setor];
    }
    else
    {
        return _insertRecursivo(node->filhos[setor], x, y, i);
    }
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)malloc(sizeof(struct node));
    if (arvore->raiz == NULL)
    {
        arvore->raiz = node;
    }
    else
    {
        _insertRecursivo(arvore->raiz, x, y, i);
    }
    arvore->nNodes++;
    return node;
}

Node _getRecursivo(Node n, double x, double y, double epsilon)
{
    struct node *node = (struct node *)n;
    if (node == NULL)
    {
        return NULL;
    }
    if (x >= node->x - epsilon && x <= node->x + epsilon && y >= node->y - epsilon && y <= node->y + epsilon)
    {
        return node;
    }
    else
    {
        int setor = checkSetor(node, x, y);
        return _getRecursivo(node->filhos[setor], x, y, epsilon);
    }
}

Node getNodeRadialT(RadialTree t, double x, double y, double epsilon)
{
    struct radialtree *arvore = (struct radialtree *)t;
    return _getRecursivo(arvore->raiz, x, y, epsilon);
}

void _transferePraArvoreRecursivo(struct radialtree *old, struct radialtree *nova, Node n)
{
    struct node *node = (struct node *)n;
    if (node != NULL)
    {
        if (!node->deletado)
        {
            insertRadialT(nova, node->x, node->y, node->item);
        }
        for (int i = 0; i < old->nFilhos; i++)
        {
            if (node->filhos[i] != NULL)
                _transferePraArvoreRecursivo(old, nova, node->filhos[i]);
        }
        free(node);
    }
}

void removeNoRadialT(RadialTree t, Node n)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)n;
    node = (struct node *)_getRecursivo(arvore->raiz, node->x, node->y, 0.01);
    if (node != NULL)
    {
        node->deletado = true;
        arvore->nRemovidos++;
        arvore->fdAtual = calculaFd(t);
        if (arvore->fdAtual > arvore->fdMax)
        {
            struct radialtree *novaArvore = newRadialTree(arvore->nFilhos, arvore->fdMax);
            _transferePraArvoreRecursivo(arvore, novaArvore, arvore->raiz);
            arvore->raiz = novaArvore->raiz;
            arvore->nNodes = novaArvore->nNodes;
            arvore->nRemovidos = novaArvore->nRemovidos;
            arvore->fdAtual = 0;
            free(novaArvore);
        }
    }
}

Info getInfoRadialT(RadialTree t, Node n)
{
    struct node *node = (struct node *)n;
    return node->item;
}

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false;
    if (node != NULL)
    {
        if (node->x >= x1 && node->x <= x2 && node->y >= y1 && node->y <= y2)
        {
            insertLst(L, node->item);
            achou = true;
        }
        double anguloinicial = atan2(y1 - node->y, x1 - node->x) * 180 / M_PI;
        double angulofinal = atan2(y2 - node->y, x2 - node->x) * 180 / M_PI;
        double incremento = 360 / arvore->nFilhos;
        for (int i = 0; i < arvore->nFilhos; i++)
        {
            double anguloFilho = incremento * i;
            if (anguloFilho >= anguloinicial && anguloFilho <= angulofinal) // só entra se o retangulo de busca estiver dentro do setor
            {
                achou = achou || getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, L);
            }
            {
                achou = achou || getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, L);
            }
            if (anguloFilho >= angulofinal && anguloFilho <= anguloinicial)
            {
                achou = achou || getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, L);
            }
        }
    }
    return achou;
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
                                 FdentroDeRegiao f, Lista L)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false;
    if (node != NULL)
    {
        if (f(node->item, x1, y1, x2, y2))
        {
            achou = true;
            insertLst(L, node->item);
        }
        double anguloinicial = atan2(y1 - node->y, x1 - node->x) * 180 / M_PI;
        double angulofinal = atan2(y2 - node->y, x2 - node->x) * 180 / M_PI;
        double incremento = 360 / arvore->nFilhos;
        for (int i = 0; i < arvore->nFilhos; i++)
        {
            double anguloFilho = incremento * i;
            if (anguloFilho >= anguloinicial && anguloFilho <= angulofinal) // só entra se o retangulo de busca estiver dentro do setor
            {
                achou = achou || getInfosDentroRegiaoRadialT(t, x1, y1, x2, y2, f, L);
            }
            {
                achou = achou || getInfosDentroRegiaoRadialT(t, x1, y1, x2, y2, f, L);
            }
            if (anguloFilho >= angulofinal && anguloFilho <= anguloinicial)
            {
                achou = achou || getInfosDentroRegiaoRadialT(t, x1, y1, x2, y2, f, L);
            }
        }
    }
    return achou;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L)
{
    struct radialtree *arvore = (struct radialtree *)t;
    struct node *node = (struct node *)arvore->raiz;
    bool achou = false;
    if (node != NULL)
    {
        if (f(node->item, x, y))
        {
            achou = true;
            insertLst(L, node->item);
        }
        double incremento = 360 / arvore->nFilhos;
        for (int i = 0; i < arvore->nFilhos; i++)
        {
            double anguloFilho = incremento * i;
            double anguloPonto = atan2(y - node->y, x - node->x) * 180 / M_PI;
            if (anguloPonto < 0)
            {
                anguloPonto += 360;
            }
            int setor = (int)anguloPonto / incremento;
            if (setor == i)
            {
                achou = achou || getInfosAtingidoPontoRadialT(t, x, y, f, L);
            }
        
        }
    }
    return achou;
}