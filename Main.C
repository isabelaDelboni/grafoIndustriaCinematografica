#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structAtor.h"
#include "structMovie.h"

#define MAX_ATORES 14000000
#define MAX_FILMES 14000000

ator *atores[MAX_ATORES];
movie *filmes[MAX_FILMES];
int atorCount = 0;
int filmeCount = 0;

void lerAtor(const char *filename) {
    FILE *arq = fopen(filename, "r");
    if (!arq) {
        perror("Erro ao abrir de atores");
        return;
    }

    char linha[256];
    int i[4], j = 0;
    int k; 
    
    while (fgets(linha, sizeof(linha), arq) != NULL && atorCount < MAX_ATORES) {
        int id, birth, death;
        char name[64], surname[64];

        sscanf(linha, "nm%d %s %s %d %d %*s tt%d,tt%d,tt%d,tt%d",
               &id, name, surname, &birth, &death,
               &i[0], &i[1], &i[2], &i[3]);

        ator *newAtor = criarAtor(id, name, surname, birth, death);
        
        for(k = 0; k < 4; k++) {
            if (i[k] != 0) {
                adicionarFilmeAoAtor(newAtor, i[k]);
            }
        }

        atores[atorCount++] = newAtor;
        j++;
    }

    fclose(arq);
}

void lerFilme(const char *filename) {
    FILE *arq = fopen(filename, "r");
    if (!arq) {
        perror("Erro ao abrir arquivo");
        return;
    }

    char linha[512]; 

    while (fgets(linha, sizeof(linha), arq) != NULL && filmeCount < MAX_FILMES) {
        int id, indicacao, anoInicio, anoFinal, duracaoMinutos;
        char tipo[64], tituloPrimario[128], tituloOriginal[128], generos[128];

        sscanf(linha, "tt%d %s %[^\t] %[^\t] %d %d %d %d %[^\t]",
               &id, tipo, tituloPrimario, tituloOriginal, &indicacao, &anoInicio, &anoFinal, &duracaoMinutos, generos);

        if (strstr(tipo, "movie") != NULL) {
            movie *newMovie = criarFilme(id, tituloOriginal);
            filmes[filmeCount++] = newMovie;
        }
    }

    fclose(arq);
}

movie *buscarFilme(int id) {
    int esquerda = 0, direita = filmeCount - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (filmes[meio]->id == id) {
            return filmes[meio];
        }
        if (filmes[meio]->id < id) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return NULL;
}

void gerarDOT(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erro ao criar arquivo DOT");
        return;
    }

    fprintf(file, "graph Neighbor {\n\tconcentrate=true;\n");

    for (int i = 0; i < atorCount; i++) {
        if (!atores[i]) continue;

        node *movie_node = atores[i]->movies;
        if (!movie_node) continue;

        movie *filmesAtor[100]; 
        int numFilmes = 0;

        while (movie_node) {
            movie *filme = buscarFilme(movie_node->movieId);
            if (filme != NULL && numFilmes < 100) { 
                filmesAtor[numFilmes++] = filme;
            }
            movie_node = movie_node->next;
        }

        for (int j = 0; j < numFilmes; j++) {
            for (int k = j + 1; k < numFilmes; k++) {
                addVizinhoFilme(filmesAtor[j], filmesAtor[k]->id);
                addVizinhoFilme(filmesAtor[k], filmesAtor[j]->id);
            }
        }
    }

    for (int i = 0; i < filmeCount; i++) {
        adjNode *current = filmes[i]->neighbors;
        while (current) {
            movie *vizinho = buscarFilme(current->movieId);
            if (vizinho) {
                fprintf(file, "\t\"%s\" -- \"%s\";\n", filmes[i]->title, vizinho->title);
            }
            current = current->next;
        }
    }

    fprintf(file, "}\n");
    fclose(file);
}

int main(void) {
    printf("---Atores---\n\n");
    lerAtor("name.basics.tsv");
    printf("\n");

    printf("---Filmes---\n\n");
    lerFilme("title.basics.tsv");
    printf("\n");

    int algumAtorComFilme = 0;

    for (int i = 0; i < atorCount; i++) {
        node *movie_node = atores[i]->movies;

        while (movie_node) {
            movie *filme = buscarFilme(movie_node->movieId);
            if (filme != NULL) {
                if (!algumAtorComFilme) {
                    printf("---Atores e seus filmes---\n\n");
                    algumAtorComFilme = 1; 
                }
                break; 
            }
            movie_node = movie_node->next;
        }
    }

    if (!algumAtorComFilme) {
        printf("Nenhum ator possui filmes associados.\n");
    }
    
    gerarDOT("input.dot");
    
    for (int i = 0; i < atorCount; i++) {
        liberarAtor(atores[i]);
    }

    for (int i = 0; i < filmeCount; i++) {
        freeFilme(filmes[i]);
    }

    return 0;
}
