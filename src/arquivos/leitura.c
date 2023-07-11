#include "leitura.h"
#include "../formas/circulo.h"
#include "../formas/retangulo.h"
#include "../formas/texto.h"
#include "../formas/linha.h"

#define MAX 100

void ler_linha(FILE *arq, char *palavras[], int *n)
{
	char linha[MAX];
	char *token;
	const char *delim = " \t\n";
	int i = 0;
	*n = i;
	// lê uma linha do arquivo
	if (fgets(linha, MAX, arq) != NULL)
	{
		// separa a primeira palavra da linha
		token = strtok(linha, delim);
		// enquanto houver palavras na linha
		while (token != NULL)
		{
			// aloca memória para armazenar a palavra
			palavras[i] = (char *)malloc(strlen(token) + 1);
			// copia a palavra para a variável
			strcpy(palavras[i], token);
			// incrementa o contador de palavras
			i++;
			// separa a próxima palavra da linha
			token = strtok(NULL, delim);
		}
		// atualiza o número de palavras na linha
		*n = i;
	}
	else
	{
		// atualiza o número de palavras na linha
		*n = 0;
	}
}

void ler_geo(FILE *geo, RadialTree db)
{
	char *palavras[MAX];
	char *textoFamilia = "sans (sans-serif)";
	char *textoPeso = "normal";
	char *textoTamanho = "20px";
	int n;
	char **ponteiro = NULL; // strtod
	char texto[MAX];
	// enquanto não chegar ao final do arquivo
	while (true)
	{
		// lê uma linha do arquivo
		ler_linha(geo, palavras, &n);
		if (n > 0)
		{
			if (strcmp(palavras[0], "c") == 0)
			{
				double x = strtod(palavras[2], ponteiro);
				double y = strtod(palavras[3], ponteiro);
				Circulo c = criaCirculo(atoi(palavras[1]),
										x,
										y,
										strtod(palavras[4], ponteiro),
										palavras[5],
										palavras[6]);
				insertRadialT(db, x, y, c);
			}
			else if (strcmp(palavras[0], "r") == 0)
			{
				double x = strtod(palavras[2], ponteiro);
				double y = strtod(palavras[3], ponteiro);
				Retangulo r = criaRetangulo(atoi(palavras[1]),
											x,
											y,
											strtod(palavras[4], ponteiro),
											strtod(palavras[5], ponteiro),
											palavras[6],
											palavras[7]);
				insertRadialT(db, x, y, r);
			}
			else if (strcmp(palavras[0], "t") == 0)
			{
				strcpy(texto, palavras[7]);
				for (int i = 8; i < n; i++)
				{
					strcat(texto, " ");
					strcat(texto, palavras[i]);
				}
				char *ancora = (char *)malloc(10 * sizeof(char));
				if (strcmp(palavras[6], "i") == 0)
					strcpy(ancora, "start");
				else if (strcmp(palavras[6], "f") == 0)
					strcpy(ancora, "end");
				else
					strcpy(ancora, "middle");
				double x = strtod(palavras[2], ponteiro);
				double y = strtod(palavras[3], ponteiro);
				Texto t = criaTexto(atoi(palavras[1]),
									x,
									y,
									palavras[4],
									palavras[5],
									ancora,
									texto,
									textoFamilia,
									textoPeso,
									textoTamanho);
				insertRadialT(db, x, y, t);
			}
			else if (strcmp(palavras[0], "l") == 0)
			{
				double x1 = strtod(palavras[2], ponteiro);
				double y1 = strtod(palavras[3], ponteiro);
				Linha l = criaLinha(atoi(palavras[1]),
									x1,
									y1,
									strtod(palavras[4], ponteiro),
									strtod(palavras[5], ponteiro),
									palavras[6]);
				insertRadialT(db, x1, y1, l);
			}
			else if (strcmp(palavras[0], "ts") == 0)
			{
				textoFamilia = palavras[1];
				if (strcmp(palavras[2], "n") == 0)
					textoPeso = "normal";
				else if (strcmp(palavras[2], "b") == 0)
					textoPeso = "bold";
				else if (strcmp(palavras[2], "b+") == 0)
					textoPeso = "bolder";
				else if (strcmp(palavras[2], "l") == 0)
					textoPeso = "lighter";
				textoTamanho = palavras[3];
			}
		}
		else
		{
			break;
		}
	}
}

/*void moveForma(Item item, double dx, double dy, RadialTree db)
{
	enum TipoForma tipo = getTipoForma(item);
	switch (tipo)
	{
	case CIRCULO:; // SÓ DEUS SABE PORQUE EU PRECISO COLOCAR ESSE PONTO E VÍRGULA AQUI
		Circulo c = (Circulo)item;
		double x = getCirculoX(c);
		double y = getCirculoY(c);
		setCirculoX(c, x + dx);
		setCirculoY(c, y + dy);
		removeNoRadialT(db, c);
		insertRadialT(db, x + dx, y + dy, c);
		break;
	case RETANGULO:;
		Retangulo r = (Retangulo)item;
		double x = getRetanguloX(r);
		double y = getRetanguloY(r);
		setRetanguloX(r, x + dx);
		setRetanguloY(r, y + dy);
		removeNoRadialT(db, r);
		insertRadialT(db, x + dx, y + dy, r);
		break;
	case TEXTO:;
		Texto t = (Texto)item;
		double x = getTextoX(t);
		double y = getTextoY(t);
		setTextoX(t, x + dx);
		setTextoY(t, y + dy);
		removeNoRadialT(db, t);
		insertRadialT(db, x + dx, y + dy, t);
		break;
	case LINHA:;
		Linha l = (Linha)item;
		double x1 = getLinhaX1(l);
		double y1 = getLinhaY1(l);
		double x2 = getLinhaX2(l);
		double y2 = getLinhaY2(l);
		setLinhaX1(l, x1 + dx);
		setLinhaY1(l, y1 + dy);
		setLinhaX2(l, x2 + dx);
		setLinhaY2(l, y2 + dy);
		removeNoRadialT(db, l);
		insertRadialT(db, x1 + dx, y1 + dy, l);
		break;
	}
}
*/
bool achaId(Info i, double x, double y, void *aux)
{
	return getIdForma(i) == *(int *)aux;
}

void ler_qry(FILE *qry, FILE *svg, FILE *txt, RadialTree db, char *nomeSvg)
{
	char *palavras[MAX];
	int n;
	char **ponteiro = NULL; // strtod
	int id;
	Item itemEncontrado = NULL;
	// enquanto não chegar ao final do arquivo
	while (true)
	{
		// lê uma linha do arquivo
		ler_linha(qry, palavras, &n);
		if (n > 0)
		{
			// escreve no txt a linha lida
			fprintf(txt, "\n[*] ");
			for (int i = 0; i < n; i++)
			{
				fprintf(txt, "%s ", palavras[i]);
			}
			fprintf(txt, "\n\n");
			// switch case para cada comando
			if (strcmp(palavras[0], "cl") == 0)
			{
				id = atoi(palavras[1]);
				itemEncontrado = getInfoRadialT(db, procuraNoRadialT(db, achaId, &id));
				if (getTipoForma(itemEncontrado) == RETANGULO)
				{
					retanguloToColheitadeira((Retangulo)itemEncontrado);
					fprintf(txt, "Retângulo de código %d agora é uma colheitadeira.\n", id);
				}
				else
				{
					fprintf(txt, "Retângulo de código %d não é um retângulo.\n", id);
				}
			} // RESTO DOS COMANDOS
		}
		else
		{
			break;
		}
	}
}