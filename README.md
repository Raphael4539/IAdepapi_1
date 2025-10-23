

﻿(en construction)
Bonjour,
 
Mon nom est Bernard Ribordy, ancien ingénieur(ETS Fribourg 67)  télécommunication, électronique, micro et mini ordinateurs, hardware, software (assembleur, fortran, pascal, COBOL). A la retraite depuis de nombreuses années, je débute avec Github.
J'habite dans le canton de Genève.
A part les nombreuses activités de retraité (golf, lecture, Cinéma, conférence à l'uni des seniors, (siestes :), etc) je me suis intéressé aux réseaux de neurones artificiels. 

J'ai commencé à développer un programme en Visual basic, que j'ai abandonné au profit du C++, plus rapide.

Ainsi, j'ai réalisé un programme qui, à l'aide de couches de neurones artificiels (perceptron multicouches),
permet par exemple de résoudre la fonction XOR en fonction de x et y. - ou de simuler un décodeur binaire/décimal (0..9).
(Le contraire également, ou à partir d'une entrée entre 0 et 9, on obtient le chiffre binaire correspondant (4 bits). Le taux de réussite atteint 100%.

En poussant plus loin, le réseau de neurones «full connected» a été précédé de couches de convolution permettant l'apprentissage à l'aide d'une grande quantité d'images monocolor (jusqu’à 60’000) représentant des chiffres 0 à 9 écrits à la main (data set MNIST), de dire en sortie de quel chiffre il s'agit, ceci avec un taux de réussite de 95% environ.
Actuellement, les spécialistes obtiennent de meilleures résultats en complexifiant l’architecture des réseaux.
Les techniques de convolutions et des réseaux de neurones sont décrites abondamment sur internet(voir réf. ci-après).
Le fichier des labels de MNIST m'a servi à construire les fichiers d'entraînement pour XOR, binair/décimal etc.

A part les liaisons entre couches (fully connected), j'ai ajouté aussi des liaison sautant (à pieds joints) sur les couches que j'appelle ironiquement "hyperconnected".

Le programme permet aussi, à partir d’un ancien apprentissage qui auront fait évoluer le poids des liaisons entre neurones, de continuer l’entraînement, ceci à partir de fichiers préenregistrés ayant le suffixe _net.txt(si < 100 % de réussite) ou _netok.txt (si 100 % de réussite). 

						                   	_________________________________________
								

<strong>Procédure :</strong>

1.  créer deux répertoire "IAdePapi_1" et "IAdePapi_installation" et copier les fichier .cpp et .h depuis les repository.
Charger l’outil de développement Qt (https://www.qt.io/product/development-tools) si vous ne l’avez pas (il existe en version open source),
 

2.  Dans Qt, Fichier/Nouveau projet..., créer deux projets : IAdePapi_installation  et IAdePapi_1 et  y ajouter les fichiers .cpp et .h correspondants à chaque repository.
(Choisissez: Application Qt avec Widgets, dans "class parent" sélectionner QWidget, ne pas générer de formulaire, langue: aucun, appuyer sur TERMINER. Supprimer les fichier maim.cpp, Widget.cpp et Widget.h créés automatiquement qui seront remplacé par les équivalents du projet.

Pour ajouter les fichiers de nos projets, il est plus simple de faire un clic droit sur "Header files" / "Ajouter des fichiers existants" puis sélectionner tous les fichiers xxx.h et xxx.cpp de nos projets qui viendront s'ajouter à nos projets.


3.  Dans le menu vertical de gauche, cliquer sur Projets/Compiler/Paramètre de compilation ajouter si nécessaire «Release» en plus de «Debug» (afin de diminuer les temps d’apprentissage).  

4.	 Maintenant que les deux projets sont créés, choisir un dossier, par exemple: ./IAdePapi_1_data/ et y mettre les deux fichiers MNIST (60’000 images et 60’000 labels); Pour ceci:
aller sur GitHub – fgnt/mnist, charger et extraire les fichiers :

	    https://raw.githubusercontent.com/fgnt/mnist/master/train-images-idx3-ubyte.gz
  	 
	    https://raw.githubusercontent.com/fgnt/mnist/master/train-labels-idx1-ubyte.gz


6.	 Dans Qt, exécuter le projet "IadePapi_installation", afin de créer des sous dossiers (./IAdePapi_1_data/data_XOR, ./IAdePapi_1_data/data_MNIST/.. etc comprenant les fichiers d’apprentissage (par ex. XOR.txt, MNISTxxxx.txt).
Ces sous-dossiers comprendront aussi les fichiers de résultats d'apprentissage.
IadePapi_installation ne s'exécute qu'une fois!

7.	 Enfin dans Qt, exécuter le projet IAdePapi_1 :
Le programme vous demande de choisir un fichier, soit p.ex. : /IAdePapi_1_data/data_BcdToOutput/BcdToOutput.txt.
Le programme demande si l’on veut recréer le fichier. Cela permet de 	changer le nombre d’exemples d’apprentissage.		(p.exemple 500).
Le nombre d’entrées ici 4 (la couche Q0 aura 4 neurones = C0 à C3) et le nombre de sorties ici 10 sont indiqués.
	
8.	 Cliquer sur «nouveau réseau».

9.	 Si l’on désir ajouter des couches intermédiaires (cachées), on pourra choisir un nombre entre -3 et +3. Les valeurs négatives permettent  de créer le réseau Hyperconnecté (voir plus haut),
ajoutons le nombre de neurones par couches intermédiaires. Choisissons p.ex : -1 couche de 3 neurones

On peut choisir ici les poids initiaux des branches, mais laisser plutôt «rd» afin de d’y attribuer des valeurs aléatoires.

9.	 Cliquer sur «1:Construisons la tables» : la table apparaît.

10.  cliquer sur «2:Construisons le réseau» : le réseau apparaît sous forme de graphe. Avec les neurones et leur biais. Délai d’attente.

11.  Laissons p.ex. le «taux d’apprentissage» de 0.5, la «différence acceptable» sur 0.2. C’est.à dire que la différence entre  les valeurs cibles (0 ou 1) et les sorties de la dernière couche ne doit pas dépasser 0.2.

12.  Choisissons un nombre de boucle (epoch)  = 10.	

  (Cliquons par exemple sur «Afficher les détails» puis sur «Pas à pas» plusieurs fois : on peut voir le changement des poids sur les liaisons entre neurones.)

13.  Cliquons sur «3:Apprentissage», une fenêtre s’ouvre montrant qu’après 10 boucles nous avons 100% de réussite.

Ces résultats apparaissent aussi dans un fichier se trouvant dans le sous-dossier
/IadePapi_1/data_BcdToOutput/BcdToOutput_tauxMax_de_réussite_100%.txt, affichant les paramètres ainsi que les résultats des 500 derniers exemples de la dernière boucle. Si 100% de réussite, l’apprentissage s’arrête.
Le nombre d’exemples à afficher peut être modifier dans le fichier :    	reseauDeNeurones_variablesConstantes.h  nberOfResultOutput(500).

Pour augmenter le taux de réussite, on peut appuyer plusieurs fois sur «Apprentissage» ce qui refera à chaque fois
10 boucles supplémentaires.  On peut voir qu’en cas de 100 % de réussite et en affichant les détails,
des Pas-à-pas supplémentaires ne feront pas évoluer les poids des liaisons.

Il est possible de modifier les couches cachées et de repartir de «1: construisons la tables» etc.

<strong>MNIST :</strong>

Si l'on choisi par exemple le fichier ./IAdePapi_1_data/data_MNIST/MNIST_4x3x3.txt, une fenêtre apparaît avec les paramètres suivant:

Nombre d’image max 59999 	    Nombre de pixel 28 (inchangé)	Nombre de convolutions 0..3
si Nombre de convolutions = 0 les pixels de l’image (28x28) seront transmises à l’entrée du réseau «full connected».

Padding (same) si coché, un nombre « zéro padding » sera ajouté pour que la sortie ait la même taille que l’image d’entée. 

Stride(pas) =1 en général pour la convolution.

Nombre de filtres.

Type de filtre 3x3 ou 5x5.

Max pool qui consiste, après convolution, à ne prendre que les valeurs maximum dans un carré de 2x2 si « Stride pooling = 2. 

RELU à cocher en général pour ne prendre que les valeurs positives de sortie.

Pooling pair  à cocher en général sauf pour la dernière convolution afin de rendre « pair » la sortie (feature) qui sera transmise à la convolution suivante.

Ces paramètres serons à remplir pour toutes les convolutions.
Ensuite, si changement, on fera un save.
Tous ces termes utilisé pour les convolutions sont bien expliqués sur internet .

Continuation au point 7 ci-dessus



Références :

https://helios2.mi.parisdescartes.fr/~bouzy/Doc/AA1/ReseauxDeNeurones1.pdf

https://alp.developpez.com/tutoriels/intelligence-artificielle/reseaux-de-neurones/#LIV-3
voir chapitre VIII Apprentissage du perceptron multicouches

https://fr.wikipedia.org/wiki/R%C3%A9seau_neuronal_convolutif

https://datapeaker.com/fr/Big-Data/un-guide-pratique-pour-construire-votre-premier-mod%C3%A8le-de-r%C3%A9seau-de-neurones-convolutifs/

https://openclassrooms.com/fr/

Merci au professeur Yann LeCun qui permet d'utiliser MNIST
