include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

void creer_matrice(int **matrice, int dimension);
void sauvegarder_matrice_binaire(int **matrice, int dimension, const char *nom_fichier);
void sauvegarder_matrice_texte(int **matrice, int dimension, const char *nom_fichier);
void charger_et_afficher_matrice_binaire(const char *nom_fichier, int dimension);
void charger_et_afficher_matrice_texte(const char *nom_fichier, int dimension);

int main(int argc, char *argv[]) {
    int dimension = 0;
    int creer_flag = 0, afficher_flag = 0, mode_binaire = 1;
    char *nom_fichier = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "cad:btf:")) != -1) {
        switch (opt) {
            case 'c':
                creer_flag = 1;
                break;
            case 'a':
                afficher_flag = 1;
                break;
            case 'd':
                dimension = atoi(optarg);
                break;
            case 'b':
                mode_binaire = 1;
                break;
            case 't':
                mode_binaire = 0;
                break;
            case 'f':
                nom_fichier = optarg;
                break;
            default:
                fprintf(stderr, "Utilisation : %s [-c] [-a] [-d dimension] [-b|-t] [-f nom_fichier]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (dimension <= 0 || nom_fichier == NULL) {
        fprintf(stderr, "La dimension et le nom du fichier doivent être spécifiés.\n");
        exit(EXIT_FAILURE);
    }

    // Allocation dynamique de la matrice
    int **matrice = malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++) {
        matrice[i] = malloc(dimension * sizeof(int));
    }

    if (creer_flag) {
        creer_matrice(matrice, dimension);
        if (mode_binaire) {
            sauvegarder_matrice_binaire(matrice, dimension, nom_fichier);
        } else {
            sauvegarder_matrice_texte(matrice, dimension, nom_fichier);
        }
    }

    if (afficher_flag) {
        if (mode_binaire) {
            charger_et_afficher_matrice_binaire(nom_fichier, dimension);
        } else {
            charger_et_afficher_matrice_texte(nom_fichier, dimension);
        }
    }

    // Libération de la mémoire
    for (int i = 0; i < dimension; i++) {
        free(matrice[i]);
    }
    free(matrice);

    return 0;
}

// Fonction pour remplir la matrice avec des valeurs aléatoires
void creer_matrice(int **matrice, int dimension) {
    srand(time(NULL));
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            matrice[i][j] = rand() % 100;  // Valeurs entre 0 et 99
        }
    }
}

// Fonction pour enregistrer la matrice en mode binaire
void sauvegarder_matrice_binaire(int **matrice, int dimension, const char *nom_fichier) {
    int fd = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dimension; i++) {
        write(fd, matrice[i], dimension * sizeof(int));
    }
    close(fd);
}

// Fonction pour enregistrer la matrice en mode texte
void sauvegarder_matrice_texte(int **matrice, int dimension, const char *nom_fichier) {
    int fd = open(nom_fichier, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    char tampon[20];
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int len = snprintf(tampon, sizeof(tampon), "%d ", matrice[i][j]);
            write(fd, tampon, len);  // Écrit l'entier formaté dans le fichier

        }
        write(fd, "\n", 1);  // Ajoute un saut de ligne
    }
    close(fd);
}

// Fonction pour lire et afficher la matrice en mode binaire
void charger_et_afficher_matrice_binaire(const char *nom_fichier, int dimension) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }
    int *tampon = malloc(dimension * sizeof(int));
    for (int i = 0; i < dimension; i++) {
        read(fd, tampon, dimension * sizeof(int));
        for (int j = 0; j < dimension; j++) {
            printf("%d ", tampon[j]);
        }
        printf("\n");
    }
    free(tampon);
    close(fd);
}

// Fonction pour lire et afficher la matrice en mode texte
void charger_et_afficher_matrice_texte(const char *nom_fichier, int dimension) {
    int fd = open(nom_fichier, O_RDONLY);
    if (fd == -1) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    char tampon[20];
    int valeur;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int k = 0;
            while (read(fd, &tampon[k], 1) > 0 && tampon[k] != ' ' && tampon[k] != '\n') {
                k++;
            }
            tampon[k] = '\0';
            valeur = atoi(tampon);
            printf("%d ", valeur);
        }
        printf("\n");
    }
    close(fd);
}

