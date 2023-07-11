#include "highorder.h"

void printSvg(Info i, double x, double y, void *aux)
{
    enum TipoForma tipo = getTipoForma(i);
    switch (tipo)
    {
    case CIRCULO:
        svg_circle(aux,
                   getCirculoX(i),
                   getCirculoY(i),
                   getCirculoR(i),
                   getCirculoRotacao(i),
                   getCirculoCorp(i),
                   getCirculoCorb(i),
                   NULL);
        break;
    case RETANGULO:
        svg_rect(aux,
                 getRetanguloX(i),
                 getRetanguloY(i),
                 getRetanguloLargura(i),
                 getRetanguloAltura(i),
                 getRetanguloRotacao(i),
                 getRetanguloCorPreenchimento(i),
                 getRetanguloCorBorda(i),
                 NULL);
        break;
    case TEXTO:
        svg_text(aux,
                 getTextoX(i),
                 getTextoY(i),
                 getTextoConteudo(i),
                 getTextoCorPreenchimento(i),
                 getTextoCorBorda(i),
                 getTextoRotacao(i),
                 getTextoFamilia(i),
                 getTextoPeso(i),
                 getTextoTamanho(i),
                 getTextoAncora(i),
                 NULL);
        break;
    case LINHA:
        svg_line(aux,
                 getLinhaX1(i),
                 getLinhaY1(i),
                 getLinhaX2(i),
                 getLinhaY2(i),
                 getLinhaRotacao(i),
                 getLinhaCor(i),
                 NULL);
        break;
    }
}