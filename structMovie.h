#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct adjacenciaNode adjNode;

struct adjacenciaNode {
    int movieId;
    adjNode *next;
};

typedef struct movie movie;

struct movie {
    int id;
    char title[128];
    adjNode *neighbors;
};

movie *criarFilme(int id, const char *title) {
    movie *newMovie = (movie *)malloc(sizeof(movie));
    if (!newMovie) {
        printf("Erro ao alocar memoria");
        exit;
    }

    newMovie->id = id;
    strncpy(newMovie->title, title, sizeof(newMovie->title) - 1);
    newMovie->title[sizeof(newMovie->title) - 1] = '\0';

    newMovie->neighbors = NULL;
    return newMovie;
}

int vizinhoExiste(movie *movie, int neighborId) {
    adjNode *current = movie->neighbors;
    while (current != NULL) {
        if (current->movieId == neighborId) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void addVizinhoFilme(movie *movie, int neighborId) {
    if (vizinhoExiste(movie, neighborId)) {
        return;
    }

    adjNode *newNode = (adjNode *)malloc(sizeof(adjNode));
    if (!newNode) {
        perror("Erro ao alocar memoria");
        exit(EXIT_FAILURE);
    }
    newNode->movieId = neighborId;
    newNode->next = movie->neighbors;
    movie->neighbors = newNode;
}

void freeFilme(movie *movie) {
    adjNode *current = movie->neighbors;

    while (current != NULL) {
        adjNode *temp = current;
        current = current->next;
        free(temp);
    }
    free(movie);
}
