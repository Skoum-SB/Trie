#include <iostream>
#include <fstream>
#include <unordered_map>
#include <dirent.h>
#include <string>
#include <chrono>
#include <memory>

using hmap = std::unordered_map<std::string, int>; //Table de hachage de la bibliothèque standard

typedef struct trie_unordered_map{ //Trie utilisant l'unordered_map
	std::unordered_map<char, std::unique_ptr<trie_unordered_map>> next_char_;
	int count_;
}trie_unordered_map;

typedef struct trie{ //Mon iplémentation du trie
	std::unique_ptr<trie> suiv[26];
	int nb;
}trie;


int main(int argc, char **argv){
	struct dirent *pDirent;
	DIR *pDir;

	if(argc != 2){
		printf("Usage: a.out <directory>\n");
		return 1;
	}

	pDir = opendir(argv[1]); //Ouvrir le répertoire passée en paramètres

	if(pDir == NULL){
		printf("Cannot open directory '%s'\n", argv[1]);
		return 1;
	}

	std::string directory; //Nom du répertoire
	std::string file; //Chemin vers le fichier
	directory.append(argv[1]); //Affecte à directory le nom du répertoire passée en paramètres

	std::string s; //String contenant le mot pour la méthode 1
	
	hmap tab; //Méthode 1
	std::unique_ptr<trie_unordered_map> arbre_unordered_map; //Méthode 2
	std::unique_ptr<trie> arbre; //Méthode 3

	std::chrono::time_point<std::chrono::system_clock> start, end; //Mesure du temps

//-------------------Méthode 1-------------------

	start = std::chrono::system_clock::now();
	while((pDirent = readdir(pDir))){ //Ouverture du fichier suivant
		file.append(pDirent->d_name); //Ajoute a file le nom du fichier courant
		std::ifstream ifs(file, std::ifstream::in); //Ouvre ce fichier
		char c = tolower(ifs.get()); //Contient un caractère
		while (ifs.good()) { //Tant que l'on a pas atteint la fin du fichier
			if(c >= 'a' && c <= 'z'){
				s.push_back(c); //Rajoute la lettre à la fin de la string
			}
			else if(!s.empty()){
				tab[s] += 1; //Ajoute 1 dans tab["mot"] s'il existe sinon cela le créé
				s.clear(); //Vider la string
			}
			c = tolower(ifs.get()); //Traiter le caractère suivant
		}
		ifs.close(); //Fermer le fichier
		file = directory; //File reçoit à nouveau le nom du repertoire
	}

	end = std::chrono::system_clock::now();
	std::cout << "Temps méthode 1 : " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " secondes" << '\n';
	std::cout << tab["tarbes"] << '\n';

	pDir = opendir(argv[1]);

//-------------------Méthode 2-------------------

	start = std::chrono::system_clock::now();
	while((pDirent = readdir(pDir))){
		file.append(pDirent->d_name);
		std::ifstream ifs(file, std::ifstream::in);
		char c = tolower(ifs.get());
		int b = 0; //Permet de gérer les autres caratères
		for (auto current = &arbre_unordered_map ; ifs.good() ; c = tolower(ifs.get())) {
			if(c >= 'a' && c <= 'z'){
				if (*current == nullptr) //Si l'on "construit" un nouveau mot, *current vaut nullptr car on a pas encore créé de trie à cette endroit de l'arbre
					*current = std::make_unique<trie_unordered_map>(); //Instancier un nouveau trie
				current = &(*current)->next_char_[c]; //L'itérateur reçoit l'adresse du trie suivant correspondant à la lettre courante
				b = 1;
			}
			else if(b == 1){ //b vaut 1 uniquement pour le premier caractère non valide, les suivants sont ignorés
				if (*current == nullptr) //Si c'est la première occurence du mot, *current vaut nullptr
					*current = std::make_unique<trie_unordered_map>(); //Il faut donc l'instancier
				(*current)->count_++; //Incrémente la variable count_ correspondante au mot
				current = &arbre_unordered_map;
				b = 0;
			}
		}
		ifs.close();
		file = directory;
	}

	end = std::chrono::system_clock::now();
	std::cout << "Temps méthode 2 : " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " secondes" << '\n';
	std::cout << arbre_unordered_map->next_char_['t']->next_char_['a']->next_char_['r']->next_char_['b']->next_char_['e']->next_char_['s']->count_ << '\n';

	pDir = opendir(argv[1]);

//-------------------Méthode 3-------------------

	start = std::chrono::system_clock::now();
	while((pDirent = readdir(pDir))){
		file.append(pDirent->d_name);
		std::ifstream ifs(file, std::ifstream::in);
		char c = tolower(ifs.get());
		int b = 0;
		for (auto current = &arbre ; ifs.good() ; c = tolower(ifs.get())) {
			if(c >= 'a' && c <= 'z'){
				if (*current == nullptr)
					*current = std::make_unique<trie>();
				current = &(*current)->suiv[c - 'a'];
				b = 1;
			}
			else if(b == 1){
				if (*current == nullptr)
					*current = std::make_unique<trie>();
				(*current)->nb++;
				current = &arbre;
				b = 0;
			}
		}
		ifs.close();
		file = directory;
	}

	end = std::chrono::system_clock::now();
	std::cout << "Temps méthode 3 : " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " secondes" << '\n';
	std::cout << arbre->suiv[19]->suiv[0]->suiv[17]->suiv[1]->suiv[4]->suiv[18]->nb << '\n';

	closedir(pDir); //Fermer le répertoire
	return 0;
}