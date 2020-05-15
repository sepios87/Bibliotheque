#include <windows.h>   // pour la couleur
using namespace std;
#include <iostream>
#include <string>
#include <conio.h> //pour le _getch() de la detection des touches
#include "outils.h"
#include <sstream> //pour fonction verif int
#include <ctime> //pour rendre le random vraiment random

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND hwnd = GetConsoleWindow(); //l'objet handle de la console
CONSOLE_CURSOR_INFO cursorInfo; //pour recup les info du curseur
CONSOLE_SCREEN_BUFFER_INFO cbsi; //pour recup la postion du curseur sur console
const string caractAccept = "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789/*-+.&(-_)=<>,;:!$€£%§.#{[|@]}"; //92 caractères possibles dans la generation d'un id

//---------------------------------------------------------------------------------------------------------------------------------------------
int tailleMotPlusGrand(t_liste<string> liste) {
	int maxi = 0;
	for (int i = 0; i < liste.nbElem; i++) { //diviser le menu en differentres chaines de caracteres en unsigned pour eviter avertissement
		if (liste.tab[i].length() > maxi) maxi = liste.tab[i].length(); //recup nb lettres dans mot le plus grand
	}
	return maxi;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void bloqueRedemConsole() {
	//LONG saveConsoleOrignale = GetWindowLong(hwnd, GWL_STYLE); //sauvegarder etat console
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); //bloquer redimensionnement console
	//SetWindowLong(hwnd, GWL_STYLE, saveConsoleOrignale); //recuperer redimensionnement console
}
//------
string chiffrement(string mot, string cle, bool chiffrer) { //en s'inspirant du chiffrement de vigenère
	int i = 0;
	if (mot != "") {
		while (i < mot.length()) {
			if (chiffrer) mot[i] = (mot[i] - cle[i % cle.length()]) + 'a';
			else mot[i] = (mot[i] + cle[i % cle.length()]) - 'a';
			i++;
		}
	}
	return mot;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void txtCouleur(string message, int couleur) {
	SetConsoleTextAttribute(hConsole, couleur);
	cout << message;
	SetConsoleTextAttribute(hConsole, 15); //couleur de base blanc
}
//---------------------------------------------------------------------------------------------------------------------------------------------
//positionner le curseur
void setPositionCurseur(COORD c) {
	SetConsoleCursorPosition(hConsole, c);
}
//-----
COORD getPositionCurseur() {
	if (GetConsoleScreenBufferInfo(hConsole, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else return { 0, 0 };
}
//---------------------------------------------------------------------------------------------------------------------------------------------
string maj(string nom) {
	for (unsigned int i = 0; i <= nom.length(); i++) {
		if (nom[i] >= 'a' && nom[i] <= 'z') nom[i] = nom[i] - 'a' + 'A';
	}
	return nom;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void setPosTailleConsole(int tailleX, int tailleY, int posX, int posY) {
	MoveWindow(hwnd, posX, posY, tailleX, tailleY, SWP_HIDEWINDOW); //pour modifier les attributs de la console
}
//---------------------------------------------------------------------------------------------------------------------------------------------
void afficherCurseur(bool aff) {
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = aff; // rendre curseur invisible
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}
//---------------------------------------------------------------------------------------------------------------------------------------------
int deplacementMenu(int nbElemList, char& ascii, int& select) {
	ascii = _getch();
	switch (ascii) {
	case KEY_DOWN: if (select < nbElemList - 1) select++; else select = 0;
		break;
	case KEY_UP:if (select > 0) select--; else select = nbElemList - 1;
		break;
	default:	if (ascii >= '1' && ascii <= '9')	if (((int)ascii - (int)'1') < nbElemList - 1)	select = ((int)ascii - (int)'1');
	}
	return select;
}
//---
string saisieMdp() {
	char ascii;
	string mdp;
	do {
		ascii = _getch();
		if (ascii != KEY_DELETE && ascii != KEY_ENTER) {
			cout << '*';
			mdp += ascii;
		}
		else if (mdp.length() > 0 && ascii == KEY_DELETE) {
			cout << "\b \b";
			mdp = mdp.substr(0, mdp.length() - 1);
		}
	} while (ascii != KEY_ENTER);
	return mdp;
}
//-----
string nouveauMdp(string mdp) {
	string tempo, tempoConfirm;
	cout << "Changer mdp, Taper nouveau mot de passe :";
	cin >> tempo;
	cout << "Retaper le nouveau mot de passe pour confirmer :";
	cin >> tempoConfirm;
	if (tempoConfirm == tempo) {
		mdp = tempoConfirm;
		txtCouleur("Le mot de passe vient de changer", jaune);
	}
	else txtCouleur("Les deux saisies sont diff\202rente, mot de passe inchang\202", rouge);
	return mdp;
}
void option(t_liste<string> liste, bool opt[]) {
	int select = 0, maxi; //max coorespond a la taille du plus grand mot
	char ascii;

	//conversion de la chaine en selection independante dans un tableau
	maxi = tailleMotPlusGrand(liste);

	afficherCurseur(false);

	setPositionCurseur({ 0, 10 });
	txtCouleur(string(6, ' ') + string(20, '='), jaune);
	setPositionCurseur({ 0, getPositionCurseur().Y + 6 });
	txtCouleur(string(6, ' ') + string(20, '='), jaune);

	string espace(maxi, ' '); //genere nb espaces mot le plus grand pour eviter pb d'affichage

	//le lambda se conporte comme une petite fonction ou procédure. [] = zone de capture
	auto lambda = [opt, liste, espace](int numList) {
		if (opt[numList]) {
			txtCouleur(liste.tab[numList], vert);
			cout << espace;
		}
		else txtCouleur(liste.tab[numList] + espace, rouge);
	};

	//affichage et deplacement dans le menu
	do {
		//affichage proposition du haut
		setPositionCurseur({ 10, 11 });
		(select == 0) ? lambda(liste.nbElem - 1) : lambda(select - 1); //= if (...) ... else ...

		//affichage proposition selectionné
		setPositionCurseur({ 13, 13 });
		if (opt[select]) {
			txtCouleur(" --->" + liste.tab[select], vert);
			cout << espace;
		}
		else txtCouleur(" --->" + liste.tab[select] + espace, rouge);
		cout << endl;

		//affichage proposition du bas
		setPositionCurseur({ 10, 15 });
		(select == liste.nbElem - 1) ? lambda(0) : lambda(select + 1);

		//gestion des touches
		deplacementMenu(liste.nbElem, ascii, select);
		if (select != liste.nbElem - 1 && ascii == KEY_ENTER) opt[select] = !opt[select];
	} while (!(ascii == KEY_ENTER && select == liste.nbElem - 1) && ascii != KEY_ESCAPE);

	if (ascii == KEY_ESCAPE) select = liste.nbElem - 1;

	afficherCurseur(true);
	setPositionCurseur({ 0, getPositionCurseur().Y + 2 });
}
//---------------------------------------------------------------------------------------------------------------------------------------------
bool controlSaisieInt(int& nb, int nbCaractMax) { //max 9 caract pr int pr ne pas crash
	char ascii;
	bool rempli = true; //false si chaine vide
	bool negatif = 0; //0 si positif 1 si negatif
	string mot = "";
	do {
		ascii = _getch();
		if (ascii != KEY_DELETE && ascii != KEY_ENTER && (ascii >= '0' && ascii <= '9' || (ascii == '-' && mot.length() == 0)) && mot.length() < nbCaractMax + negatif) {
			cout << ascii;
			mot += ascii;
			if (mot[0] == '-') negatif = 1;
			else negatif = 0;
		}

		if (mot.length() > 0 && ascii == KEY_DELETE) {
			cout << "\b \b"; //retour en arrière d'1 caract puis espace pour effecer puis retour encore pr simuler suppression classique
			mot = mot.substr(0, mot.length() - 1);
		}
	} while (ascii != KEY_ENTER);

	if (mot == "") {
		rempli = false;
		nb = 0;
	}
	else nb = stoi(mot);
	cout << endl;
	return rempli;
}
//-------------
string generateurCode(int nbCaract) {
	srand(time(NULL)); //pour rendre le random vraiment random
	string combi;
	//soit X^ N + X ^ (N - 1) + X ^ (N - 2) + ... + X + 1  : le nb de mots de passe avec X le nombre de caractères autorisés et N la logueur
	for (int i = 0; i < nbCaract; i++) {
		combi += caractAccept[rand() % caractAccept.length()]; //reste dans le champs des caractères assez courants (entre 0 et longueur de la chaine)
	}
	return combi;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
int menu(t_liste<string> liste, bool quitter) { //decomp : sur quel caractere decomposer le menu en tableau
	int select = 0, max; //max coorespond a la taille du plus grand mot
	char ascii;

	max = tailleMotPlusGrand(liste);

	afficherCurseur(false);

	setPositionCurseur({ 0, 9 });
	txtCouleur((string(7, ' ') + "**Menu d\202roulant**\n"), vert);
	txtCouleur(string(6, ' ') + string(20, '-'), jaune);
	setPositionCurseur({ 0, getPositionCurseur().Y + 6 });
	txtCouleur(string(6, ' ') + string(20, '-'), jaune);

	string espace(max, ' '); //genere nb espaces mot le plus grand pour eviter pb d'affichage

	//affichage et deplacement dans le menu
	do {
		//affichage proposition du haut
		setPositionCurseur({ 10, 11 });
		(select == 0) ? txtCouleur(liste.tab[liste.nbElem - 1] + espace, blanc) : txtCouleur(liste.tab[select - 1] + espace, blanc);

		//affichage proposition selectionné
		setPositionCurseur({ 13, 13 });
		txtCouleur(liste.tab[select] + espace, cyan);
		cout << endl;

		//affichage proposition du bas
		setPositionCurseur({ 10, 15 });
		(select == liste.nbElem - 1) ? txtCouleur(liste.tab[0] + espace, blanc) : txtCouleur(liste.tab[select + 1] + espace, blanc);

		//gestion des touches
		deplacementMenu(liste.nbElem, ascii, select);
	} while (ascii != KEY_ENTER && !(quitter && ascii == KEY_ESCAPE));

	if (quitter && ascii == KEY_ESCAPE) select = liste.nbElem - 1;

	afficherCurseur(true);
	setPositionCurseur({ 0, getPositionCurseur().Y + 2 });

	return select + 1;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
bool confirmation(string message) {
	bool test;
	string conf; //pour le passer dans la fonction maj, doit etre une chaine
	cout << endl;
	txtCouleur(message + " (o/n) :", 14);
	cin >> conf;
	conf = maj(conf);
	while (conf != "O" && conf != "N") {
		cout << endl;
		txtCouleur("Mauvaise saise," + message + " (o/n) :", 14);
		cin >> conf;
		conf = maj(conf);
	}
	(conf == "O") ? test = true : test = false;
	cin.ignore();
	return test;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
int controler(string signe, int nbControl, string message, int nbCaractMax) {
	int nbSaisie;
	controlSaisieInt(nbSaisie, nbCaractMax);

	if (signe == "==") while (nbSaisie != nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}
	if (signe == ">") while (nbSaisie <= nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}
	if (signe == ">=") while (nbSaisie < nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}
	if (signe == "<") while (nbSaisie >= nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}
	if (signe == "<=") while (nbSaisie > nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}
	if (signe == "!=") while (nbSaisie == nbControl) {
		txtCouleur(message, rouge);
		controlSaisieInt(nbSaisie, nbCaractMax);
	}

	return nbSaisie;
}