#include "tipos.h"

/** Todas as structs de formas devem ter o primeiro membro como o enum TipoForma
para que seja possível identificar o tipo de forma */
struct formaGenerica
{
    enum TipoForma tipo;
    int id;
};

enum TipoForma getTipoForma(void *forma)
{
    return ((struct formaGenerica *)forma)->tipo;
}

int getIdForma(void *forma)
{
    return ((struct formaGenerica *)forma)->id;
}
