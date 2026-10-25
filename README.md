Application console développée en langage C pour gérer des notes de frais : ajout, modification, suppression, recherche, tri, calcul des dépenses par catégorie et génération de rapports détaillés. Les données sont stockées en mémoire dans une liste doublement chaînée et peuvent être sauvegardées dans un fichier binaire.

Fonctionnalités : 
* Ajout d'une note : au début, à la fin, ou après une note précise
* Modification d'une note : date, montant, catégorie ou description
* Suppression : au début, à la fin, après une note précise, ou d'une note spécifique
* Recherche
  - simple : par date, par montant ou par catégorie
  - avancée : par combinaison de tous les critères
* Affichage de la liste, avec tri optionnel (par montant, date ou catégorie, en ordre croissant ou décroissant)
* Calcul du total des dépenses d'une catégorie sur une période donnée
* Rapport détaillé des dépenses regroupées par catégorie, avec totaux par catégorie et total général
* Sauvegarde et chargement des notes dans le fichier notes.bin
* Validation des saisies : dates (années bissextiles incluses, années entre 2000 et 2090), montants strictement positifs, intervalles de dates cohérents
* Interface colorée dans le terminal grâce aux codes ANSI
