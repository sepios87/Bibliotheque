#include <iostream>
#include <string>
#include "outils.h"
#include "biblioLivre.h"
using namespace std;

int idLivreRecherche = -1, choix;
bool session = false; //session pour savoir si un mode est choisi
char mode; //user ou admin
string motdp;

t_liste<string> optUtilisateur = { {" [1] Admin", "  [2] Invit\202", " [3] Quitter"}, 3 };
t_liste<string> optionsMenu = { { " [1]:Ajouter un livre", " [2]:Afficher tous les livres", " [3]:Supprimer un livre", " [4]:Rechercher un livre", " [5]:Trier les livres", " [6]:Modifier / Empreinter - Rendre un livre", " [7]:D\202tails sur le dernier livre touch\202", " [8]:Parametres", " [9]:R\202initialiser la biblioteque", " [10]:Sauvegarder et quitter"}, 10 };
t_liste<string> parametres = { {"[1]:Gestion des infos affich\202es", "[2]:Changer mot de passe", "[3]:Quitter"}, 3 };

t_biblio bibliotheque{}; //{} pour initialiser correctement la structure
t_liste_auteur auteur{};

int main() {
	setPosTailleConsole(1480, 750);//Positionne la console a 10 pixel du bord gauche et aussi du bord haut de l'ecran par defaut, et la console prend la taille : 1280x750px
	bloqueRedemConsole();

	motdp = lireFicBiblio(bibliotheque, auteur); //récupère le mdp dans le fichier
	if (motdp == "") motdp = "admin"; //si aucun mdp, mdp par defaut = admin

	do {
		cout << titre;
		choix = menu(optUtilisateur, false);
		switch (choix) {
		case 1: txtCouleur("Saisir mdp : ", rouge); //Connexion pour admin
			if (saisieMdp() == motdp) {
				session = true;
				mode = 'a';
			}
			else {
				txtCouleur("\nMauvais mot de passe !\n", rouge);
				system("Pause");
			}
			break;
		case 2: session = true; //Connexion invité
			mode = 'u';
			break;
		case 3: txtCouleur("\n A  +", 13);
			system("pause");
			break;
		default: break;
		}
		system("CLS");
	} while (!session && choix != 3);

	//menu du "logiciel"
	if (choix != 3) {
		do {
			//affichage permanent avec titre et queslques informations
			cout << titre;
			setPositionCurseur({ 35, 7 }); //curseur positionner à la 7eme ligne et au 45eme espace
			if (mode == 'a') txtCouleur("Mode Admin        ", jaune);
			else txtCouleur("Mode Invit\202        ", jaune);
			if (bibliotheque.nbLivre > 1) txtCouleur("Nombre de livres : ", cyan);
			else txtCouleur("Nombre de livre : ", cyan);
			cout << bibliotheque.nbLivre;
			//--
			choix = menu(optionsMenu, false);
			switch (choix) {
			case 1: if (mode == 'a') ajoutLivre(bibliotheque, auteur); //controle d'administrateur
				  else txtCouleur("Il faut etre administrateur pour ajouter un livre", rouge);
				break;
			case 2: (bibliotheque.nbLivre > 0) ? afficheLivres(bibliotheque, auteur) : txtCouleur("Aucun livre", rouge);
				//operateur ternaire = if (>0) affiche else "aucun livre"
				break;
			case 3: if (mode == 'a') (bibliotheque.nbLivre > 0) ? suppLivre(bibliotheque, auteur) : txtCouleur("Aucun livre", rouge);
				  else txtCouleur("Il faut etre administrateur pour supprimer un livre", rouge);
				break;
			case 4: if (bibliotheque.nbLivre > 1) idLivreRecherche = rechercheLivre(bibliotheque, auteur, false);
				  else txtCouleur("il faut minimum 2 livres", rouge);
				break;
			case 5: if (bibliotheque.nbLivre > 1) { //controle de nombre de livres dans la bibliotheque
				triBulle(bibliotheque);
			}
				  else txtCouleur("il faut minimum 2 livres", rouge);
				break;
			case 6: if (mode == 'a') {
				if (bibliotheque.nbLivre > 0) {
					system("CLS");
					setPositionCurseur({ 0, 0 });
					cout << titre;
					idLivreRecherche = modifLivre(bibliotheque, auteur); //mettre variable pour savoir quel ouvrage a ete créé ou modif en dernier avec idLivreRecheche
				}
				else txtCouleur("Aucun livre", rouge);
			}
				  else txtCouleur("Il faut etre administrateur pour modifier un livre", rouge);
				break;
			case 7: if (idLivreRecherche != -1 || bibliotheque.nbLivre == 1) {
				if (bibliotheque.nbLivre == 1) idLivreRecherche = 0;
				setPositionCurseur({ 0, 18 });
				dessinAffichage();
				afficheLivre(bibliotheque.tabLivre[idLivreRecherche], auteur.tabAuteur[bibliotheque.tabLivre[idLivreRecherche].idAuteur]);
			}
				  else {
				txtCouleur("Aucun livre seul recherch\202 derni\x8Arement", rouge);
			}
				  break;
			case 8: system("CLS");
				cout << titre;
				switch (menu(parametres)) {
				case 1: setTab();
					break;
				case 2: if (mode == 'a') motdp = nouveauMdp(motdp);
					  else txtCouleur("Il faut etre administrateur pour modifier le mot de passe", rouge);
				default:
					break;
				}
				break;
			case 9: if (mode == 'a') {
				if (bibliotheque.nbLivre > 0) {
					if (confirmation("Supprimer tous les livres ?")) {
						bibliotheque.nbLivre = 0; //pour "reinitialiser" la biblioteque mais en realité les nouveaux livres écraserons les anciens
						auteur.nbAuteur = 0;
						txtCouleur("Biblioth\x8Aque reinitialis\202e", rouge + foncer);
					}
				}
				else txtCouleur("Aucun livre", rouge);
			}
				  else txtCouleur("Il faut etre administrateur pour r\202initialiser la bibliotheque", rouge);
				break;
			case 10: txtCouleur("\n A  +", 13);
				ecrireFicBiblio(bibliotheque, auteur, motdp);
				break;
			default: break;
			}
			cout << endl;
			setPositionCurseur({ 75, 10 });
			system("Pause");
			system("CLS"); //effacer l'affichage de la console
		} while (choix != 10);
	}
	return 0;
}