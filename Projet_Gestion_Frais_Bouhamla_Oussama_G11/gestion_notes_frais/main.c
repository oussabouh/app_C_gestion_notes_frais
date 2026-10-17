#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Codes ANSI pour les couleurs

#define ROUGE  "\x1B[91m"
#define VERT   "\x1B[92m"
#define JAUNE  "\x1B[93m"
#define BLEU   "\x1B[96m"
#define RESET  "\x1B[0m"

typedef struct
{
    int jour,mois,annee;

} Date;

typedef struct
{
    Date date;
    float montant;
    char categorie[50];
    char description[100];

} InfoNote;

typedef struct NoteFrais
{
    InfoNote details;
    struct NoteFrais *suivant;
    struct NoteFrais *precedent;

} NoteFrais;

// Retourne 1 si d1 est strictement après d2, -1 si avant, 0 si égale

int comparerDates(Date d1, Date d2)
{
    if (d1.annee != d2.annee) return (d1.annee > d2.annee) ? 1 : -1;
    if (d1.mois != d2.mois)   return (d1.mois > d2.mois)   ? 1 : -1;
    if (d1.jour != d2.jour)   return (d1.jour > d2.jour)   ? 1 : -1;

    return 0;
}

int estDateValide(int j, int m, int a)
{
    // Vérification de base de l'année et du mois
    if (a < 2000 || a > 2090) return 0; // On accepte une plage raisonnable
    if (m < 1 || m > 12)      return 0;
    if (j < 1 || j > 31)      return 0;

    //Gestion des mois de 30 jours
    if ((m == 4 || m == 6 || m == 9 || m == 11) && j > 30) return 0;

    //Gestion spécifique de Février (Bissextile)
    if (m == 2)
    {
        // Une année est bissextile si divisible par 4 mais pas par 100, ou par 400
        int bissextile = (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);

        if (bissextile)
        {
            if (j > 29) return 0;
        }
        else
        {
            if (j > 28) return 0;
        }
    }

    return 1;
}

void SaisirInfoNote(InfoNote *pnote)
{
    int valide;
    int nbrlus;

    do
    {
        printf(BLEU"\n> Date (JJ MM AAAA) : "RESET);
        nbrlus=scanf("%d %d %d", &pnote->date.jour,&pnote->date.mois,&pnote->date.annee);

        while(getchar() != '\n');

        if(nbrlus!=3) valide=0;
        else valide=estDateValide(pnote->date.jour,pnote->date.mois,pnote->date.annee);

        if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

    }while(!valide);

    do
    {
        pnote->montant = -1; // On force une valeur invalide pour le test suivant si scanf() échoue

        printf(BLEU"> Montant : "RESET);
        scanf("%f", &pnote->montant);

        while(getchar() != '\n');

        valide=(pnote->montant>0) ? 1:0;

        if(!valide) printf(ROUGE"\a\nMontant invalide. Reessayez.\n\n"RESET);

    }while(!valide);

    printf(BLEU"> Categorie : "RESET);
    scanf(" %49[^\n]", pnote->categorie);

    while(getchar() != '\n');

    printf(BLEU"> Description : "RESET);
    scanf(" %99[^\n]", pnote->description);

    while(getchar() != '\n');

}
NoteFrais* CreerNote()
{
    NoteFrais *n = (NoteFrais*)malloc(sizeof(NoteFrais));

    if(n == NULL)
    {
        printf(ROUGE"\nErreur d'allocation memoire\n"RESET);
        return NULL;
    }

    printf("\n>>> Saisie des informations de la nouvelle note <<<\n");

    SaisirInfoNote(&n->details);

    n->suivant = NULL;
    n->precedent = NULL;

    return n;
}

void AjouterDebut(NoteFrais **tete)
{
    NoteFrais *n = CreerNote();

    if(n == NULL)
        return;

    if(*tete == NULL)
    {
        *tete = n;
    }
    else
    {
        n->suivant = *tete;
        (*tete)->precedent = n;
        *tete = n;
    }

    printf(VERT"\nNote ajoutee au debut avec succes.\n"RESET);
}

void AjouterFin(NoteFrais **tete)
{
    NoteFrais *n = CreerNote();

    if(n == NULL)   return;

    if(*tete == NULL)
    {
        *tete = n;
    }
    else
    {
        NoteFrais *temp = *tete;

        while(temp->suivant != NULL)
        {
            temp = temp->suivant;
        }

        temp->suivant = n;
        n->precedent = temp;
    }

     printf(VERT"\nNote ajoutee en fin avec succes.\n"RESET);
}

void AjouterApres(NoteFrais *tete)
{
    if(tete == NULL)
    {
        printf(JAUNE"\nLa liste vide. Option annulee.\n"RESET);
        return;
    }

    InfoNote cible;

    printf("\n>>> Saisie des informations de la note cible <<<\n");

    SaisirInfoNote(&cible);

    NoteFrais *temp = tete;

    while(temp != NULL)
    {
        if(temp->details.date.jour == cible.date.jour &&
           temp->details.date.mois == cible.date.mois &&
           temp->details.date.annee == cible.date.annee &&
           temp->details.montant == cible.montant &&
           strcasecmp(temp->details.categorie, cible.categorie) == 0 &&
           strcasecmp(temp->details.description, cible.description) == 0)
        {
            break;
        }

        temp = temp->suivant;
    }

    if(temp == NULL)
    {
        printf(JAUNE"\nNote cible non trouvee.\n"RESET);
        return;
    }

    printf(VERT"\nNote trouvee.\n"RESET);

    NoteFrais *n = CreerNote();

    if(n == NULL)  return;

    n->suivant = temp->suivant;
    n->precedent = temp;

    if(temp->suivant != NULL)  temp->suivant->precedent = n;

    temp->suivant = n;

    printf(VERT"\nNote ajoutee avec succes.\n"RESET);
}

void AfficherListe(NoteFrais *tete)
{
    if(tete == NULL)
    {
        printf(JAUNE"\nLa liste est vide. Aucune note a afficher.\n"RESET);
        return;
    }

    NoteFrais *temp = tete;
    int i=1;

    printf("\n------------------------ Liste des notes ------------------------\n\n");

    while(temp != NULL)
    {
        printf("Numero de la note : %d\n",i);
        printf("Date : %d/%d/%d\n",
               temp->details.date.jour,
               temp->details.date.mois,
               temp->details.date.annee);

        printf("Montant : %.2f DH\n", temp->details.montant);

        printf("Categorie : %s\n", temp->details.categorie);

        printf("Description : %s\n", temp->details.description);

        printf("\n-----------------------------------------------------------------\n");

        temp = temp->suivant;
        i++;
    }


}

void rechercheSimple(NoteFrais *tete, int typeRecherche, InfoNote valeurCherchee)
{
    // Vérification si la liste est vide
    if (tete == NULL)
    {
        printf(JAUNE"\nLa liste est vide.\n"RESET);
        return;
    }

    NoteFrais *courant = tete;
    int compteur = 0;

    while (courant != NULL)
    {
        int correspond = 0;

        // On accède aux données via courant->details
        switch (typeRecherche)
        {
            case 1: // Recherche par Date
                if (courant->details.date.jour == valeurCherchee.date.jour &&
                    courant->details.date.mois == valeurCherchee.date.mois &&
                    courant->details.date.annee == valeurCherchee.date.annee)
                {
                    correspond = 1;
                }
                break;

            case 2: // Recherche par Montant
                if (courant->details.montant == valeurCherchee.montant)
                {
                    correspond = 1;
                }
                break;

            case 3: // Recherche par Categorie

                if (strcasecmp(courant->details.categorie, valeurCherchee.categorie) == 0)
                {
                    correspond = 1;
                }
                break;
        }

        // Affichage formaté si une correspondance est trouvée
        if (correspond)
        {
            if(compteur==0)
            {
                printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>> RESULTAT(S) DE LA RECHERCHE <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
                printf("\n" JAUNE "%-10s" RESET " | " JAUNE "%-15s" RESET " | " JAUNE "%-10s" RESET "  | " JAUNE "%s" RESET "\n", "DATE", "CATEGORIE", "MONTANT", "DESCRIPTION");
                printf("---------------------------------------------------------------------------------\n");
            }
            printf("%02d/%02d/%04d | %-15s | %-8.2f DH | %s\n",
                   courant->details.date.jour,
                   courant->details.date.mois,
                   courant->details.date.annee,
                   courant->details.categorie,
                   courant->details.montant,
                   courant->details.description);
            compteur++;
        }
        courant = courant->suivant;
    }

    if (compteur == 0)
    {
        printf(JAUNE"\nAucun resultat trouve pour ce critere.\n"RESET);
    }
    else
    {
        printf(VERT"\nTOTAL : %d correspondance(s) trouvee(s).\n"RESET, compteur);
    }
}

NoteFrais* rechercheAvancee(NoteFrais *tete, Date d, float m, char *cat, char *desc)
{
    // On commence la recherche à partir de la tête de liste

    if (tete == NULL)
    {
        printf(JAUNE"\nLa liste est vide.\n"RESET);
        return NULL;
    }

    NoteFrais *courant = tete;

    while (courant != NULL)
    {
        // On vérifie si tous les critères correspondent en accédant à courant->details
        if (courant->details.date.jour == d.jour &&
            courant->details.date.mois == d.mois &&
            courant->details.date.annee == d.annee &&
            courant->details.montant == m &&
            strcasecmp(courant->details.categorie, cat) == 0 &&
            strcasecmp(courant->details.description, desc) == 0)
        {
            printf(VERT"\nNote Trouvee.\n"RESET);

            return courant; // Retourne le pointeur vers la note trouvée
        }
        courant = courant->suivant;
    }

    printf(JAUNE"\nAucune note correspondante n'est trouvee.\n"RESET);

    return NULL;
}

void modifierNote(NoteFrais *note)
{
    if (note == NULL)
    {
        printf(ROUGE"Note inexistante.\n"RESET);
        return;
    }

    int choix, valide;

    do
    {
        choix=0;
        printf("\n------------- Options de modification -------------\n\n");
        printf("1. Modifier la date\n");
        printf("2. Modifier le montant\n");
        printf("3. Modifier la categorie\n");
        printf("4. Modifier la description\n");
        printf("5. Retour\n");
        printf(BLEU"\n> Choix : "RESET);

        scanf("%d", &choix);
        while(getchar() != '\n');

        switch (choix)
        {
            case 1:

                do
                {
                    printf(BLEU"\n> Nouvelle date (JJ MM AAAA) : "RESET);
                    int nbrlus=scanf("%d %d %d", &note->details.date.jour,&note->details.date.mois,&note->details.date.annee);
                    while(getchar() != '\n');

                    if(nbrlus!=3) valide=0;
                    else valide=estDateValide(note->details.date.jour,note->details.date.mois,note->details.date.annee);

                    if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

                }while(!valide);

                break;

            case 2:

                do
                {
                    note->details.montant=-1;
                    printf(BLEU"\n> Nouveau montant : "RESET);
                    scanf("%f", &note->details.montant);
                    while(getchar() != '\n');

                    if (note->details.montant > 0) valide = 1;

                    else
                    {
                        valide = 0;
                        printf(ROUGE"\a\nMontant invalide. Reessayez.\n"RESET);
                    }
                }while (!valide);

                break;

            case 3:

                printf(BLEU"\n> Nouvelle categorie : "RESET);
                scanf(" %49[^\n]",note->details.categorie);
                while(getchar() != '\n');

                break;

            case 4:

                printf(BLEU"\n> Nouvelle description : "RESET);
                scanf(" %99[^\n]",note->details.description);
                while(getchar() != '\n');

                break;

            case 5:

                printf(JAUNE"\nModification annulee.\n"RESET);
                break;

            default:

                printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
                break;
        }

        if(choix>=1&&choix<=4) printf(VERT"\nNote modifiee avec succes.\n"RESET);

    }while(choix!=5);

}

void supprimerDebut(NoteFrais **tete)
{
    // 1. Vérifier si la liste est vide
    if (*tete == NULL)
    {
        printf(JAUNE"\nLa liste est deja vide.\n"RESET);
        return;
    }

    // 2. Identifier le nœud à supprimer (l'actuel premier)
    NoteFrais *aSupprimer = *tete;

    // 3. Décaler la tête vers le deuxième élément
    *tete = (*tete)->suivant;

    // 4. Si la liste n'est pas devenue vide, mettre à jour le nouveau premier
    if (*tete != NULL)
    {
        (*tete)->precedent = NULL;
    }

    // 5. Libérer la mémoire
    free(aSupprimer);

    if(*tete!=NULL) printf(VERT"\nLa premiere note a ete supprimee avec succes.\n"RESET);
    else printf(VERT"\nL'unique note a ete supprimee avec succes.\n"RESET);
}

void supprimerFin(NoteFrais **tete)
{
    // 1. Vérifier si la liste est vide
    if (*tete == NULL)
    {
        printf(JAUNE"\nLa liste est deja vide.\n"RESET);
        return;
    }

    NoteFrais *courant = *tete;

    // 2. Cas particulier : un seul élément dans la liste
    if (courant->suivant == NULL)
    {
        free(courant);
        *tete = NULL;
        printf(VERT"\nL'unique note a ete supprimee avec succes.\n"RESET);
        return;
    }

    // 3. Parcourir jusqu'au dernier élément
    while (courant->suivant != NULL)
    {
        courant = courant->suivant;
    }

    // 4. Détacher le dernier élément
    // 'courant' est le dernier, 'courant->precedent' devient le nouveau dernier
    courant->precedent->suivant = NULL;

    // 5. Libérer la mémoire
    free(courant);

    printf(VERT"\nLa derniere note a ete supprimee avec succes.\n"RESET);
}

void supprimerApres(NoteFrais *tete)
{
    if (tete == NULL)
    {
        printf(JAUNE"\nLa liste est deja vide.\n"RESET);
        return;
    }

    InfoNote cible;

    printf("\n>>> Saisie des informations de la note AVANT celle a supprimer <<<\n");

    SaisirInfoNote(&cible);

    // 1. Rechercher la note cible
    NoteFrais *temp = tete;
    while (temp != NULL)
    {
        if (temp->details.date.jour == cible.date.jour &&
            temp->details.date.mois == cible.date.mois &&
            temp->details.date.annee == cible.date.annee &&
            temp->details.montant == cible.montant &&
            strcasecmp(temp->details.categorie, cible.categorie) == 0 &&
            strcasecmp(temp->details.description, cible.description) == 0)
        {
            break; // Note cible trouvée
        }
        temp = temp->suivant;
    }

    // 2. Vérifications
    if (temp == NULL)
    {
        printf(JAUNE"\nNote cible non trouvee.\n"RESET);
        return;
    }

    if (temp->suivant == NULL)
    {
        printf(JAUNE"\nIl n'y a aucune note apres celle-ci a supprimer.\n"RESET);
        return;
    }

    // 3. Procéder à la suppression de la note suivante
    NoteFrais *aSupprimer = temp->suivant;

    // On relie la note cible au "petit-fils" (celui après la note à supprimer)
    temp->suivant = aSupprimer->suivant;

    // Si le "petit-fils" existe, on met à jour son pointeur précédent
    if (aSupprimer->suivant != NULL)
    {
        aSupprimer->suivant->precedent = temp;
    }

    free(aSupprimer);
    printf(VERT"\nLa note suivante a ete supprimee avec succes.\n"RESET);
}

void supprimerNoteSpecifique(NoteFrais **tete)
{
    if (*tete == NULL)
    {
        printf(JAUNE "\nLa liste est vide. Option annulee.\n" RESET);
        return;
    }

    InfoNote cible;
    printf("\n>>> Saisie des informations de la note a supprimer <<<\n");
    SaisirInfoNote(&cible);

    NoteFrais *aSupprimer = rechercheAvancee(*tete, cible.date, cible.montant, cible.categorie, cible.description);

    if (aSupprimer == NULL)
    {
        // Le message "Aucune note correspondante" est déjà géré par rechercheAvancee
        return;
    }

    // CAS 1 : Si c'est la tête de liste
    if (aSupprimer == *tete)
    {
        *tete = aSupprimer->suivant;
    }

    // CAS 2 : Si ce n'est pas le dernier élément (on met à jour le lien vers l'arrière du suivant)
    if (aSupprimer->suivant != NULL)
    {
        aSupprimer->suivant->precedent = aSupprimer->precedent;
    }

    // CAS 3 : Si ce n'est pas le premier élément (on met à jour le lien vers l'avant du précédent)
    if (aSupprimer->precedent != NULL)
    {
        aSupprimer->precedent->suivant = aSupprimer->suivant;
    }

    free(aSupprimer);
    printf(VERT "\nLa note a ete supprimee avec succes.\n" RESET);
}

void calculerTotalParCategorie(NoteFrais *tete)
{
    if (tete == NULL)
    {
        printf(JAUNE"\nLa liste est vide. Option annulee.\n"RESET);
        return;
    }

    Date debut, fin;
    char catCherchee[50];
    float total = 0.0;
    int trouve = 0,valide;

    // 1. Saisie des critères
    printf("\n>>> Saisie des criteres du calcul <<<\n");
    printf(BLEU"\n> Categorie recherchee : "RESET);
    scanf(" %49[^\n]", catCherchee);
    while(getchar() != '\n');

    do
    {
        do
        {
            printf(BLEU"\n> Date de debut (JJ MM AAAA) : "RESET);
            int nbrlus=scanf("%d %d %d", &debut.jour,&debut.mois,&debut.annee);
            while(getchar() != '\n');

            if(nbrlus!=3) valide=0;
            else valide=estDateValide(debut.jour,debut.mois,debut.annee);

            if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

        }while(!valide);

        do
        {
            printf(BLEU"\n> Date de fin (JJ MM AAAA) : "RESET);
            int nbrlus=scanf("%d %d %d", &fin.jour,&fin.mois,&fin.annee);
            while(getchar() != '\n');

            if(nbrlus!=3) valide=0;
            else valide=estDateValide(fin.jour,fin.mois,fin.annee);

            if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

        }while(!valide);

        valide=(comparerDates(debut, fin) <= 0)?1:0;

        if(!valide)
        {
            printf(ROUGE"\a\nIntervalle invalide. Reessayez.\n"RESET);
        }

    }while(!valide);

    // 2. Parcours et calcul
    NoteFrais *courant = tete;
    while (courant != NULL)
    {
        // Vérifier si la catégorie correspond
        if (strcasecmp(courant->details.categorie, catCherchee) == 0)
        {
            // Vérifier si la date est dans l'intervalle [debut, fin]
            if (comparerDates(courant->details.date, debut) >= 0 && comparerDates(courant->details.date, fin) <= 0)
            {
                total += courant->details.montant;
                trouve = 1;
            }
        }
        courant = courant->suivant;
    }

    // 3. Affichage du résultat
    if (trouve)
    {
        printf(VERT"\nTotal pour la categorie '%s' du %02d/%02d/%d au %02d/%02d/%d : %.2f DH\n"RESET,
               catCherchee, debut.jour, debut.mois, debut.annee, fin.jour, fin.mois, fin.annee,total);
    }
    else
    {
        printf(JAUNE"\nAucune depense trouvee pour cette categorie sur cette periode.\n"RESET);
    }
}

void genererRapportDetaille(NoteFrais *tete, Date debut, Date fin)
{
    if (tete == NULL)
    {
        printf(JAUNE"\nLa liste est vide.\n"RESET);
        return;
    }

    printf("\n=========================================================================\n");
    printf("                      RAPPORT DETAILLE DES DEPENSES\n");
    printf("                       Du %02d/%02d/%d Au %02d/%02d/%d\n", debut.jour, debut.mois, debut.annee, fin.jour, fin.mois, fin.annee);
    printf("=========================================================================\n");

    NoteFrais *externe = tete;
    float totalGlobal = 0.0;

    while (externe != NULL)
    {
        // 1. On vérifie si la note actuelle est dans la période
        if (comparerDates(externe->details.date, debut) >= 0 &&
            comparerDates(externe->details.date, fin) <= 0)
        {
            // 2. Vérifier si on a déjà traité cette catégorie auparavant dans la boucle
            int dejaTraite = 0;
            NoteFrais *verif = tete;
            while (verif != externe)
            {
                if (strcasecmp(verif->details.categorie, externe->details.categorie) == 0 &&
                    comparerDates(verif->details.date, debut) >= 0 &&
                    comparerDates(verif->details.date, fin) <= 0)
                {
                    dejaTraite = 1;
                    break;
                }
                verif = verif->suivant;
            }

            // 3. Si c'est une "nouvelle" catégorie pour ce rapport
            if (!dejaTraite)
            {
                float totalCat = 0.0;
                printf(JAUNE"\nCATEGORIE : %s\n"RESET,externe->details.categorie);
                printf("-------------------------------------------------------------------------\n");
                printf("%-12s | %-40s | %10s\n", "DATE", "DESCRIPTION", "MONTANT");
                printf("-------------------------------------------------------------------------\n");

                // 4. Deuxième passage pour afficher toutes les notes de CETTE catégorie
                NoteFrais *interne = tete;
                while (interne != NULL)
                {
                    if (strcasecmp(interne->details.categorie, externe->details.categorie) == 0 &&
                        comparerDates(interne->details.date, debut) >= 0 &&
                        comparerDates(interne->details.date, fin) <= 0)
                    {
                        printf("%02d/%02d/%04d   | %-40s | %8.2f DH\n",
                               interne->details.date.jour, interne->details.date.mois, interne->details.date.annee,
                               interne->details.description, interne->details.montant);
                        totalCat += interne->details.montant;
                    }
                    interne = interne->suivant;
                }

                printf("-------------------------------------------------------------------------\n");
                printf(VERT"TOTAL CATEGORIE '%s' : %.2f DH\n"RESET, externe->details.categorie, totalCat);
                totalGlobal += totalCat;
            }
        }
        externe = externe->suivant;
    }

    printf("\n=========================================================================\n");
    printf(VERT"TOTAL GENERAL DE LA PERIODE : %.2f DH\n"RESET,totalGlobal);
    printf("=========================================================================\n");
}

// Fonction auxiliaire pour échanger les données de deux nœuds
void echangerData(NoteFrais *a, NoteFrais *b)
{
    InfoNote temp = a->details;
    a->details = b->details;
    b->details = temp;
}

void trierListe(NoteFrais *tete, int typeTri, int croissant)
{
    // Si la liste est vide ou n'a qu'un seul élément, rien à trier
    if (tete == NULL || tete->suivant == NULL) return;

    int permute;
    NoteFrais *courant;

    do
    {
        permute = 0;
        courant = tete;

        while (courant->suivant != NULL)
        {
            int doitEchanger = 0;
            int comp = 0;

            // Comparaison basée sur le membre 'details'
            switch (typeTri)
            {
                case 1: // Tri par Montant
                    if (courant->details.montant != courant->suivant->details.montant)
                    {
                        comp = (courant->details.montant > courant->suivant->details.montant) ? 1 : -1;
                    }
                    break;
                case 2: // Tri par Date
                    comp = comparerDates(courant->details.date, courant->suivant->details.date);
                    break;
                case 3: // Tri par Catégorie
                    comp = strcasecmp(courant->details.categorie, courant->suivant->details.categorie);
                    break;
            }

            // Détermination de l'ordre (croissant : comp > 0, décroissant : comp < 0)
            if (croissant)
            {
                if (comp > 0) doitEchanger = 1;
            }
            else
            {
                if (comp < 0) doitEchanger = 1;
            }

            if (doitEchanger)
            {
                echangerData(courant, courant->suivant);
                permute = 1;
            }
            courant = courant->suivant;
        }
    }
    while (permute);

    printf(VERT"\nLa liste a ete triee avec succes.\n"RESET);
}

void sauvegarderListe(NoteFrais *tete)
{
    FILE *f = fopen("notes.bin", "wb");

    if (f == NULL)
    {
        printf(ROUGE"\nErreur lors de l'ouverture du fichier.\n"RESET);
        return;
    }

    NoteFrais *temp = tete;

    while (temp != NULL)
    {
        fwrite(&temp->details, sizeof(InfoNote), 1, f);
        temp = temp->suivant;
    }

    fclose(f);
    printf(VERT"\nSauvegarde effectuee avec succes.\n"RESET);
}

void libererListe(NoteFrais **tete)
{
    // Si la liste est déjà vide, rien à faire
    if (tete == NULL || *tete == NULL)
    {
        return;
    }

    NoteFrais *courant = *tete;
    NoteFrais *aSupprimer;

    while (courant != NULL)
    {
        aSupprimer = courant;       // On garde une trace du nœud actuel
        courant = courant->suivant; // On passe au suivant AVANT de libérer
        free(aSupprimer);           // On libère la mémoire
    }

    // on remet la tête à NULL pour éviter un pointeur fou (dangling pointer)
    *tete = NULL;

    printf(VERT"\nMemoire liberee avec succes.\n"RESET);
}

void chargerListe(NoteFrais **tete)
{
    FILE *f = fopen("notes.bin", "rb");

    if (f == NULL)
    {
        printf(JAUNE"\nAucune sauvegarde trouvee.\n"RESET);
        return;
    }

    InfoNote tempInfo;

    while (fread(&tempInfo, sizeof(InfoNote), 1, f))
    {
        NoteFrais *n = (NoteFrais*)malloc(sizeof(NoteFrais));

        if (n == NULL)
        {
            printf(ROUGE"\nErreur memoire.\n"RESET);

            // Nettoyer la mémoire partiellement allouée en cas d'échec
            libererListe(tete);
            fclose(f);
            printf(JAUNE"\nChargement annule.\n"RESET);

            return;
        }

        n->details = tempInfo;
        n->suivant = NULL;
        n->precedent = NULL;

        // Ajout à la fin
        if (*tete == NULL)
        {
            *tete = n;
        }
        else
        {
            NoteFrais *t = *tete;
            while (t->suivant != NULL)
                t = t->suivant;

            t->suivant = n;
            n->precedent = t;
        }
    }

    fclose(f);
    printf(VERT"\nChargement effectue avec succes.\n"RESET);
}

int fichierEstVide(FILE *f)
{
    fseek(f, 0, SEEK_END);   // Aller à la fin du fichier
    long taille = ftell(f);  // Obtenir la position (taille)
    rewind(f);               // Revenir au début

    return (taille == 0);
}

int main()
{
    // Initialisation du pointeur de tête
    NoteFrais *maListe = NULL;

    int choixPrincipal=0, choixGestion=0, choixSous=0,valide,estvide;
    char reponse;

    printf("\nBienvenue! \n");
    printf(VERT"\nInitialisation du programme...\n"RESET);

    FILE *f = fopen("notes.bin", "rb");

    if (f != NULL)
    {
        estvide = fichierEstVide(f);
        fclose(f);

        if (estvide == 0)
        {
            printf("\nSouhaitez-vous charger les notes sauvegardees en 'notes.bin' ? (O/N)\n");

            do
            {
                printf(BLEU"\n> Choix : "RESET);
                scanf(" %c", &reponse);
                while(getchar()!='\n');

                valide = (reponse=='o'||reponse=='O'||reponse=='n'||reponse=='N');

                if (!valide) printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);

            } while (!valide);

            if (reponse=='o'||reponse=='O') chargerListe(&maListe);
            else printf(JAUNE"\nChargement annule.\n"RESET);

        }
        else
        {
            printf(JAUNE"\nFichier vide. Aucune note a charger.\n"RESET);
        }
    }

    else
    {
        printf(JAUNE"\nAucune sauvegarde trouvee.\n"RESET);
    }

    do
    {
        choixPrincipal=0; //On force une valeur invalide pour tomber en 'else' (en cas d'echec de scanf)

        printf("\n================================================== MENU PRINCIPAL ==================================================\n\n");
        printf("                                           1 -> Gestion des notes de frais\n\n");
        printf("                                           2 -> Quitter\n");
        printf(BLEU"\n> Choix : "RESET);

        scanf("%d", &choixPrincipal);
        while(getchar() != '\n'); // Vide le tampon d'entrée pour éviter une boucle infinie en cas de saisie invalide(caractere)

        if (choixPrincipal == 1)
        {
            do
            {
                choixGestion=0;

                printf("\n-------------------------------------------- Gestion des notes de frais -------------------------------------------\n\n");
                printf("                                    1 -> Ajout de notes de frais\n\n");
                printf("                                    2 -> Modification de notes de frais\n\n");
                printf("                                    3 -> Suppression de notes de frais\n\n");
                printf("                                    4 -> Recherche de notes de frais\n\n");
                printf("                                    5 -> Affichage de notes de frais (avec tri optionnel)\n\n");
                printf("                                    6 -> Calcul des depenses totales par categorie\n\n");
                printf("                                    7 -> Generation de rapports de depenses\n\n");
                printf("                                    8 -> MENU principal\n");
                printf(BLEU"\n> Choix : "RESET);
                scanf("%d", &choixGestion);
                while(getchar() != '\n');

                switch (choixGestion)
                {
                    case 1: // AJOUT
                        do{
                            choixSous=0;
                            printf("\n------------- Options d'ajout -------------\n");
                            printf("\n1. Ajout au debut\n2. Ajout a la fin\n3. Ajout apres une note specifique\n4. Retour\n\n"BLEU"> Choix : "RESET);
                            scanf("%d", &choixSous);
                            while(getchar() != '\n');
                            if (choixSous == 1) AjouterDebut(&maListe);
                            else if (choixSous == 2) AjouterFin(&maListe);
                            else if (choixSous == 3) AjouterApres(maListe);
                            else if(choixSous == 4) printf(JAUNE"\nOption annulee.\n"RESET);
                            else printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
                        }while(choixSous!=4);

                        break;

                    case 2: // MODIFICATION
                        if (maListe == NULL) printf(JAUNE"\nLa liste est vide. Option annulee.\n"RESET);
                        else
                        {
                            InfoNote note_modif;

                            printf("\n>>> Identification de la note a modifier <<<\n");

                            SaisirInfoNote(&note_modif);

                            NoteFrais *cible = rechercheAvancee(maListe, note_modif.date, note_modif.montant, note_modif.categorie, note_modif.description);

                            if (cible != NULL) modifierNote(cible);
                        }
                        break;

                    case 3: // SUPPRESSION
                        if(maListe==NULL) printf(JAUNE"\nLa liste est vide. Option annulee.\n"RESET);
                        else
                        {
                            do{
                                choixSous=0;
                                printf("\n------------- Options de suppression -------------\n");
                                printf("\n1.Suppression au debut\n2.Suppression a la fin\n3.Suppression apres une note specifique\n4.Suppression d'une note precise\n5.Retour\n"BLEU"\n> Choix : "RESET);
                                scanf("%d", &choixSous);
                                while(getchar() != '\n');
                                if (choixSous == 1) supprimerDebut(&maListe);
                                else if (choixSous == 2) supprimerFin(&maListe);
                                else if (choixSous == 3) supprimerApres(maListe);
                                else if (choixSous == 4) supprimerNoteSpecifique(&maListe);
                                else if(choixSous == 5) printf(JAUNE"\nOption annulee.\n"RESET );
                                else printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
                            }while(choixSous!=5);
                        }

                        break;

                    case 4: // RECHERCHE
                        if(maListe==NULL) printf(JAUNE"\nLa liste est vide. Option annulee.\n"RESET);
                        else
                        {
                         do{
                            choixSous=0;
                            printf("\n------------- Options de recherche -------------\n");
                            printf("\n1. Par date\n2. Par montant\n3. Par categorie\n4. Recherche avancee\n5. Retour\n"BLEU"\n> Choix : "RESET);
                            scanf("%d", &choixSous);
                            while(getchar() != '\n');

                            if (choixSous >= 1 && choixSous <= 3)
                            {
                                InfoNote critere;
                                int valide;

                                if (choixSous == 1)
                                {
                                    do
                                    {
                                        printf(BLEU"\n> Date (JJ MM AAAA) : "RESET);
                                        int nbrlus=scanf("%d %d %d", &critere.date.jour,&critere.date.mois,&critere.date.annee);
                                        while(getchar() != '\n');

                                        if(nbrlus!=3) valide=0;
                                        else valide=estDateValide(critere.date.jour,critere.date.mois,critere.date.annee);

                                        if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

                                    }while(!valide);
                                }

                                else if (choixSous == 2)
                                {
                                    do
                                    {
                                        critere.montant=-1;
                                        printf(BLEU"\n> Montant : "RESET);
                                        scanf("%f", &critere.montant);
                                        while(getchar() != '\n');
                                        valide=(critere.montant>0) ? 1:0;

                                        if(!valide) printf(ROUGE"\a\nMontant invalide. Reesayez.\n"RESET);

                                    }while(!valide);
                                }

                                else
                                {
                                    printf(BLEU"\n> Categorie : "RESET);
                                    scanf(" %49[^\n]", critere.categorie);
                                    while(getchar() != '\n');
                                }

                                (void)rechercheSimple(maListe, choixSous, critere); //On caste en void pour ignorer la valeur retournée
                            }

                            else if(choixSous == 4)
                            {
                                InfoNote critere;

                                SaisirInfoNote(&critere);

                                rechercheAvancee(maListe,critere.date,critere.montant,critere.categorie,critere.description);

                            }

                            else if(choixSous == 5)
                            {
                                printf(JAUNE"\nOption annulee.\n"RESET);
                            }

                            else
                            {
                                printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
                            }

                        }while(choixSous!=5);

                        }

                        break;

                    case 5: // AFFICHAGE (AVEC OPTION DE TRI)
                        if (maListe == NULL)
                        {
                            AfficherListe(maListe); // Affichera "Aucune note"
                        }
                        else
                        {
                            char reponse;
                            int valide;

                            printf("\nSouhaitez-vous trier la liste avant l'affichage ? (O/N)\n");

                            do
                            {
                                printf(BLEU"\n> Choix : "RESET);
                                scanf(" %c", &reponse);
                                while(getchar()!='\n');

                                valide=(reponse=='o'||reponse=='O'||reponse=='n'||reponse=='N')?1:0;

                                if(!valide) printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);

                            }while(!valide);

                            if (reponse == 'o' || reponse == 'O')
                            {
                                int typeTri=0, ordre=0;

                                printf("\n------------- Options du critere de tri -------------\n");
                                printf("1. Par montant\n2. Par date\n3. Par categorie\n");

                                do
                                {
                                    typeTri=0;
                                    printf(BLEU"\n> Choix : "RESET);
                                    scanf("%d", &typeTri);
                                    while(getchar() != '\n');

                                    valide=(typeTri>=1&&typeTri<=3)?1:0;

                                    if(!valide) printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);

                                }while(!valide);

                                printf("\n------------- Options d'ordre de tri -------------\n");
                                printf("1. Croissant\n2. Decroissant\n");

                                do
                                {   ordre=0;
                                    printf(BLEU"\n> Choix : "RESET);
                                    scanf("%d", &ordre);
                                    while(getchar() != '\n');

                                    valide=(ordre==1||ordre==2)?1:0;

                                    if(!valide) printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);

                                }while(!valide);

                                // Appel de la fonction de tri (croissant est vrai si ordre == 1)
                                trierListe(maListe, typeTri, (ordre == 1));
                            }

                            else
                            {
                                printf(JAUNE"\nTri annule.\n"RESET);
                            }

                            // Affichage final (trié ou non selon le choix)
                            AfficherListe(maListe);
                        }
                        break;

                    case 6: // CALCUL TOTAL
                        calculerTotalParCategorie(maListe);
                        break;

                    case 7: // RAPPORT
                        if (maListe == NULL) printf(JAUNE"\nLa liste est vide. Option annulee.\n"RESET);
                        else
                        {
                            Date d1, d2;
                            int valide;

                            do
                            {
                                do
                                {
                                    printf(BLEU"\n> Date de debut (JJ MM AAAA) : "RESET);
                                    int nbrlus=scanf("%d %d %d", &d1.jour,&d1.mois,&d1.annee);
                                    while(getchar() != '\n');

                                    if(nbrlus!=3) valide=0;
                                    else valide=estDateValide(d1.jour,d1.mois,d1.annee);

                                    if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

                                }while(!valide);

                                do
                                {
                                    printf(BLEU"\n> Date de fin (JJ MM AAAA) : "RESET);
                                    int nbrlus=scanf("%d %d %d", &d2.jour,&d2.mois,&d2.annee);
                                    while(getchar() != '\n');

                                    if(nbrlus!=3) valide=0;
                                    else valide=estDateValide(d2.jour,d2.mois,d2.annee);

                                    if(!valide) printf(ROUGE"\a\nDate invalide. Reessayez.\n"RESET);

                                }while(!valide);

                                valide=(comparerDates(d1, d2) <= 0)?1:0;

                                if(!valide)
                                {
                                    printf(ROUGE"\a\nIntervalle invalide. Reessayez.\n"RESET);
                                }

                            }while(!valide);

                            genererRapportDetaille(maListe, d1, d2);
                        }
                        break;

                    case 8: // RETOUR MENU PRINCIPAL
                        break;

                    default:
                        printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
                        break;
                }
            } while (choixGestion != 8);
        }
        else if(choixPrincipal!=2)
        {
            printf(ROUGE"\a\nChoix inconnu. Reessayez.\n"RESET);
        }
    } while (choixPrincipal != 2);


    if (maListe != NULL)
    {
        printf("\nSouhaitez-vous sauvegarder les notes en 'notes.bin' avant de quitter ? (O/N)\n");

        do
        {
            printf(BLEU"\n> Choix : "RESET);
            scanf(" %c", &reponse);
            while(getchar()!='\n');

            valide = (reponse=='o'||reponse=='O'||reponse=='n'||reponse=='N');

            if (!valide) printf(ROUGE"\a\nChoix invalide. Reessayez.\n"RESET);

        } while (!valide);

        if (reponse == 'o' || reponse == 'O')
        {
            sauvegarderListe(maListe);
        }
        else
        {
            printf(JAUNE"\nSauvegarde annulee.\n"RESET);
        }
    }
    else
    {
        printf(JAUNE"\nAucune donnee a sauvegarder.\n"RESET);
    }

    libererListe(&maListe);

    printf(VERT"\nFermeture du programme...\n"RESET);

    printf("\nA bientot !\n");

    return 0;
}
