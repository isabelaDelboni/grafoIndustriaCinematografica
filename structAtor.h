#include <stdlib.h>

typedef struct node node;

struct node {
    int movieId;
    node *next;
};

typedef struct ator ator;

struct ator{
	int id, birth, death;
	char name[64], surname[64];
	node *movies;
};

ator *criarAtor(int id, const char *name, const char *surname, int birth, int death) {
    ator *newAtor = (ator *)malloc(sizeof(ator));
    if (!newAtor) {
        printf("Erro ao alocar memoria");
        exit;
    }
    
    newAtor->id = id;
    strncpy(newAtor->name, name, sizeof(newAtor->name) - 1);
    newAtor->name[sizeof(newAtor->name) - 1] = '\0';
    
    strncpy(newAtor->surname, surname, sizeof(newAtor->surname) - 1);
    newAtor->surname[sizeof(newAtor->surname) - 1] = '\0';
    
    newAtor->birth = birth;
    newAtor->death = death;
    newAtor->movies = NULL;
    return newAtor;
}

void adicionarFilmeAoAtor(ator *ator, int movieId) {
    node *newNode = (node *)malloc(sizeof(node));
    if (!newNode) {
        printf("Erro ao alocar memoria do node");
        exit;
    }
    
    newNode->movieId = movieId;
    newNode->next = ator->movies;
    ator->movies = newNode;
}

void liberarAtor(ator *ator) {
    node *current = ator->movies;
    
    while (current != NULL) {
        node *temp = current;
        current = current->next;
        free(temp);
    }
    
    free(ator);
}