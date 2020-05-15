#pragma once
#ifndef BLIBLIO_outils_H
#define BLIBLIO_outils_H
#include <iostream>
#include <string>
#include "biblioDate.h"
#include <windows.h>   // pour la couleur

#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESCAPE 27
#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_DELETE 8
using namespace std;

const int foncer = -8; //faire bleu + fonc pour bleu foncé par exemple
const int gris = 8; // = noir en foncé
const int bleu = 9;
const int vert = 10;
const int cyan = 11;
const int rouge = 12;
const int violet = 13;
const int jaune = 14;
const int blanc = 15;

const int foncer_fond = -128;
const int fond_blanc = 240;
const int fond_jaune = 224;
const int fond_violet = 208;
const int fond_rouge = 192;
const int fond_cyan = 176;
const int fond_vert = 160;
const int fond_bleu = 144;
const int fond_gris = 128;

const int MAX_ELEM_LISTE = 100;

template<typename T> //on pourra ainsi utiliser n'importe quel type pour les listes (il faudra juste le préciser à "l'instanciation")
struct t_liste {
	T tab[MAX_ELEM_LISTE];
	unsigned int nbElem = 0; //ne peux pas etre <0 donc on profite de tout l'espace avec le unsigned 
};

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Procedure permettant de bloquer la taille de la console en empêchant son redimensionnement
//
// Entrée : Aucune variable
//
// Sortie : Aucune variable
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
void bloqueRedemConsole();

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Procedure permettant de mettre en couleur un texte 
//
// Entrée : Une chaine (message) et un entier (couleur)
//
// Sortie : Aucune variable
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
void txtCouleur(string message, int couleur);

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Procedure permettant de positionner a une certaine coordonnée sur la console
//
// Entrée : un type COORD (c)
//
// Sortie : Aucune variable
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
void setPositionCurseur(COORD c);

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Fonction permettant de mettre en majuscule
//
// Entrée : Une chaine (nom)
//
// Retourne une chaine
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
string maj(string nom);

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Procedure permettant de rechercher un livre par une caracteristique spécifique (id du livre, nom du livre etc ...)
//
// Entrée : Quatre entiers (tailleX et tailleY qui definissent la taille de la console ainsi que posX et posY qui definissent la position de la console)
//
// Sortie : Aucune variable
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
void setPosTailleConsole(int tailleX, int tailleY, int posX = 10, int posY = 10); // valeurs par defaut : 10px

// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
// 
// Procedure permettant de cacher le curseur sur la console
//
// Entrée : Un booléen (aff)
//
// Sortie : Aucune variable
//
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
void afficherCurseur(bool aff);


void option(t_liste<string> liste, bool opt[]);

bool controlSaisieInt(int& nb, int nbCaractMax = 9);//9 = max caract dans un int

string generateurCode(int nbCaract);

string chiffrement(string mot, string cle, bool chiffrer = true); //le chiffrer = true pour valeur par defaut si rien saisie

string saisieMdp();

string nouveauMdp(string mdp);

int menu(t_liste<string> liste, bool quitter = true);

bool confirmation(string message);

int controler(string signe, int nbControl, string message, int nbCaractMax = 9);


#endif // !BLIBLIO_outils_H