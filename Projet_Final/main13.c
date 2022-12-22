 // WACRENIER Titouan  ---- PROJET C Reseau électrique 	------					PEI2

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

/// Fonction pour ajouter du delai
void delay(int i)
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC);
}

void Color(int couleurDuTexte,int couleurDeFond) // fonction d'affichage de couleurs
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}

// Lit les caratere pour et leur attribut un nombre utilise dans l'affichage

int lireCaract(){

    char c=10;
    int fnt=0;

    c=getch();
    fnt=c;
    if (c==-32){c=getch();fnt = 400+c;}
    if (c==0)  {c=getch();fnt = 500+c;}
    return fnt;
}

////////////////////////////////// TERMINAL ////////////////////////////////////////////

// Fonction qui permet de se deplacer sur l'ecran du terminal

void gotoLigCol( int lig, int col )
{
    // ressources
    COORD mycoord;
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
}

// Fonction qui permet de se déplacer et de supprimer un nombre de carctère voulue sur une ligne
void  gotoDel(int x, int y,int taille){

    int i=0;
    //On va a  la position demandÃ©e
    gotoLigCol(x, y);
    //On efface avec des espaces
    while(i<taille)
    {
        printf(" ");
        i++;
    }

    gotoLigCol(x, y);
}

/////////////////////////////////////////////////////////////////////////////////// DEFINITION DES STRUCTURES ///////////////////////////////////////////////////////////////

/////////  Liste des villes  ////////////////////////

// Creation de la structure ville contenant le code postale et le pointeur vers la ville suivante
typedef struct ville {
    int codePostal;
    struct ville * villeSuivante;
} Tville;

typedef Tville * PTville;

/////////  Liste des lignes Centrale-Ville  ////////////////////////

typedef struct lignesElectrique {
    int puissance;
    PTville villeDesservie;
    struct lignesElectrique * ligneSuivante ;
} TligneElectrique;

typedef TligneElectrique * PTligneElectrique;

//////////  Liste des centrales electriques ////////

typedef struct centrale{
    int codeCentrale;
    int puissance_MAX;
    // Pointeur sur la liste des lignes
    PTligneElectrique villeDependante;
    // Liste doublement chainee
    struct centrale * ptsuivant;
    struct centrale * ptprecedent;
}Tcentrale;

typedef Tcentrale * PTcentrale;

/////////////////////////////////////////////////////////////////////////////////// INSERSION D'ELEMENT ////////////////////////////////////////////////////////////////////////////////

// Alloue la mémoire a une nouvelle case de la taille struct Centrale

void insertion_Centrale (PTcentrale p){

    p->ptsuivant->ptprecedent= (PTcentrale)malloc(sizeof(Tcentrale));
    p->ptsuivant->ptprecedent->ptsuivant= p->ptsuivant;
    p->ptsuivant->ptprecedent->ptprecedent =p;
    p->ptsuivant = p->ptsuivant->ptprecedent;
}

// Alloue la mémoire a une nouvelle case de la taille struct Ville

void insertion_Ville (PTville p){

    p->villeSuivante=(PTville)malloc(sizeof(Tville));
    p->villeSuivante->villeSuivante=NULL;

}

// Alloue la mémoire à une nouvelle case de la taille struct ligne

void insertion_Ligne (PTligneElectrique p){

    p->ligneSuivante=(PTligneElectrique)malloc(sizeof(TligneElectrique));
    p->ligneSuivante->ligneSuivante=NULL;

}

/////////////////////////////////////////////////////////////////////////////////// PARCOURS DE LISTE ////////////////////////////////////////////////////////////////////////////////

// Fonction qui recupere l'adresse de la centrale recherche avec son Code en verifiant son existance

PTcentrale getCentraleCode (int CodeC, PTcentrale pdebut)
{
    PTcentrale pc;
    pc = pdebut;
    while(pc->codeCentrale!=CodeC)
    {
        pc=pc->ptsuivant;
        if(pc==NULL){Color(12,0);printf("Le code n'est pas bon");Color(15,0);}
    }
    return pc; // retrour d'une adresse
}

// Fonction qui recuperer l'adresse de la ville grace a son code postale et verifie son existance

PTville getVilleCode (int CodeP, PTville pdebut)
{
    PTville pv;
    pv = pdebut->villeSuivante;
    while(pv->codePostal!=CodeP)
    {
        pv=pv->villeSuivante;
        if(pv==NULL){Color(12,0);printf("Le code n'est pas bon");Color(15,0);}
    }
    return pv;
}

//Fonction qui recuperer l'adresse qui precede la ville dont on a saisi le code postal et verifie son existance

PTville getprecVille(int CodeP, PTville pdebut)
{
    PTville pv;
    pv = pdebut;
    while(pv->villeSuivante->codePostal!=CodeP)
    {
        pv=pv->villeSuivante;
        if(pv==NULL){Color(12,0);printf("Le code n'est pas bon");Color(15,0);}
    }
    return pv;
}

//Fonction qui recuperer l'adresse qui precede la ligne, avec un code postal.

PTligneElectrique getprecLigne(int codeC, int CodeP, PTcentrale pCdebut)
{
    PTligneElectrique pe;
    PTcentrale  pc;
    pc = getCentraleCode(codeC, pCdebut);
    pe = pc->villeDependante;
    while(pe->ligneSuivante->villeDesservie->codePostal!=CodeP)
    {
        pe=pe->ligneSuivante;
        if(pe==NULL){Color(12,0);printf("Le code n'est pas bon");Color(15,0);}
    }
    return pe;
}

/////////////////////////////////////////////////////////////////////////////////// COMPTEUR LISTE ////////////////////////////////////////////////////////////////////////////////

// Fonction qui permet de compter le nombre de ville dans la liste ville
int comptVille(PTville pdebut)
{
    PTville pv;
    int k=0;

    pv=pdebut;
    while(pv->villeSuivante!=NULL)
    {
        pv=pv->villeSuivante;
        k++;
    }

    return k;
}

// Fonction qui permet de compter le nombre de centrale dans la liste centrale
int comptCentrale(PTcentrale pdebut)
{
    PTcentrale pc;
    int k=0;

    pc=pdebut->ptsuivant;
    while(pc->ptsuivant!=NULL)
    {
        pc=pc->ptsuivant;
        k++;
    }

    return k;
}

// FOnction qui permet de compter le nombre de ville dans la liste ligne d'une centrale

int comptVilleCentrale(int cc, PTcentrale pCdebut)
{
    PTligneElectrique pe;
    PTcentrale pc;
    int k=0;

    pc = getCentraleCode(cc, pCdebut);
    pe = pc->villeDependante->ligneSuivante;
    while(pe!=NULL)
    {
        pe = pe->ligneSuivante;
        k++;
    }

    return k;


}
// Fonction qui permet de compter la puissance totale alloue  par une centrale
int comptPuiss(int cc, PTcentrale pCdebut)
{
    PTligneElectrique pe;
    PTcentrale pc;
    int puissTot=0;

    pc = getCentraleCode(cc, pCdebut);
    pe = pc->villeDependante;
    while(pe->ligneSuivante!=NULL)
    {
        puissTot= puissTot + pe->puissance;
        pe = pe->ligneSuivante;

    }

    return puissTot;

}

// Fonction qui permet de compter la puissance recu par une ville

int comptPuissVille(int codeP, PTcentrale pCdebut)
{
    PTligneElectrique pe;
    PTcentrale pc;
    pc = pCdebut;
    int pTotVille=0;


    while(pc->ptsuivant!=NULL)
    {
        pe=pc->villeDependante->ligneSuivante;

        while(pe->ligneSuivante!=NULL)
        {
            if(pe->villeDesservie->codePostal==codeP)
            {
                pTotVille = pTotVille + pe->puissance;
            }
            pe=pe->ligneSuivante;
        }

        pc=pc->ptsuivant;
    }
    return pTotVille;

}

/////////////////////////////////////////////////////////////////////////////////// VERIFICATION D'EXISTANCE ////////////////////////////////////////////////////////////////////////////////

// Fonction qui verifie l'existance ou non d'une ville

int existeVille(PTville pdebut)
{
    PTville px;
    int i = 1;

    r:
    px = pdebut;
    printf("\tEntrer le code postal de la ville : \t");
    scanf("%d",&i);

    while(px!=NULL)
    {
        if(px->codePostal==i)
        {
            return i;

        }
        px=px->villeSuivante;
    }

    Color(12,0);printf("\tLe code n'existe pas\t Reessayer... \n");Color(15,0);
    goto r;

}

// Fonction qui verifie l'existance ou non d'une centrale
int existeCentrale(PTcentrale pdebut)
{
    PTcentrale px;
    int i = 1;

    r:
    px = pdebut;
    printf("\tEntrer le code de la centrale : \t");
    scanf("%d",&i);

    while(px!=NULL)
    {
        if(px->codeCentrale==i)
        {
            return i;
        }
        px=px->ptsuivant;
    }

    Color(12,0);printf("\n\tLe code n'existe pas\t Reessayer... \n");Color(15,0);
    goto r;

}


/////////////////////////////////////////////////////////////////////////////////// VERIFICATION GLOBALE ////////////////////////////////////////////////////////////////////////////////

// Fonction qui lit les entiers saisis par l'utilisateur et verifie qu'il n'y a pas de double
int saisiVille(PTville pdebut)
{
    PTville px;
    px = pdebut;
    int i = 1;

    printf("\tEntrer le code postal de la ville :\t");
    scanf("%d",&i);

    while(px!=NULL && i!=0)
    {
        if(i==px->codePostal){Color(12,0);printf("\tLe code postal est deja utilise ! \t Reessayer ...\n");Color(15,0);
            printf("\tEntrer le code postal de la ville : \t");scanf("%d",&i);
        }
        px=px->villeSuivante;
    }
    return i;
}

// Fonction qui lit les entiers saisis par l'utilisateur et verifie qu'il n'y a pas de double
int saisiCentrale(int lig, int col,PTcentrale pdebut)
{
    PTcentrale px;
    px = pdebut;
    int i = 1;

    printf("\tEntrer le code de la Centrale : \t");
    scanf("%d",&i);   // Si i n'est pas un entier boucle infini ?

    while(px->ptsuivant!=NULL && i!=0 )
    {
        if(i==px->codeCentrale){Color(12,0);printf("\tLe code centrale est deja utilise !! \t Reessayer ...\n");Color(15,0);
            printf("\tEntrer le code de la Centrale : \t");scanf("%d",&i);
        }
        px=px->ptsuivant;
    }
    return i;
}


/////////////////////////////////////////////////////////////////////////////////// LECTURES DES LISTES ////////////////////////////////////////////////////////////////////////////////

// FOnction qui creer la liste centrale et permet le remplisage de celle-ci jusqu'a que l'utilisateur saisisse 0
void lire_Centrale(PTcentrale pdebut){

    PTcentrale px;
    PTligneElectrique pdebutElec;
    int codeC=1;
    px = pdebut;
    c:
    while(codeC!=0){
        codeC = saisiCentrale(3,1,pdebut);

        if(codeC!=0){
            insertion_Centrale(px);
            px=px->ptsuivant;
            px->codeCentrale = codeC;
            px->puissance_MAX=0;

            /// Allocation d'une case pour ligne elec
            pdebutElec = (PTligneElectrique) malloc(sizeof(TligneElectrique));
            pdebutElec->ligneSuivante = NULL;
            px->villeDependante = pdebutElec;
        }
    }
}

// Fonction qui creer la liste ville et permet le remplisage de celle-ci jusqu'a que l'utilisateur saisisse 0
void lire_Ville(PTville pdebut){

    PTville px;
    px = pdebut;
    int codeP=1;

    while(codeP!=0){
        codeP = saisiVille(pdebut);
        if(codeP!=0){
            insertion_Ville(px);
            px=px->villeSuivante;
            px->codePostal= codeP;
        }
    }

}

// FOnction qui  permet le remplisage de la liste Ligne jusqu'a que l'utilisateur saisisse 0 ou qu'il n'y ai plus de ville a saisir

void lire_ligne(int cc,int cp, int puiss,PTcentrale pCdebut,PTville pVdebut)
{
    PTcentrale pc;
    PTligneElectrique pe;
    PTville pv;

    pc = getCentraleCode(cc,pCdebut);
    pe = pc->villeDependante;

    while(pe->ligneSuivante!=NULL)
    {
        pe=pe->ligneSuivante;
    }

    pv = getVilleCode(cp,pVdebut);
    insertion_Ligne(pe);
    pe = pe->ligneSuivante;
    pe->villeDesservie = pv;
    pe->puissance = puiss;

}


/////////////////////////////////////////////////////////////////////////////////// ENREGISTREMENT ET LECTURE DE FICHIER ////////////////////////////////////////////////////////////////////////////////


void login_CodeVille(PTville pdebut)
{
    PTville px;
    int codeP=1;
    px = pdebut;

    FILE * fichierCodeVille = NULL;
    fichierCodeVille=fopen("CodeVille.txt","r");

    if(fichierCodeVille==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }
    while(codeP!=0)
    {
        fscanf(fichierCodeVille, "%d", &codeP);
        if(codeP!=0){
            insertion_Ville(px);
            px=px->villeSuivante;
            px->codePostal= codeP;
        }
    }
    fclose(fichierCodeVille);
}

void login_CodeCentrale(PTcentrale pdebut)
{
    PTcentrale px;
    px=pdebut;
    PTligneElectrique pdebutElec;

    int codeC=1;int puiss=0;

    FILE * fCodeCentrale = NULL;
    fCodeCentrale=fopen("CodeCentrale.txt","r");

    if(fCodeCentrale==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }

    while(codeC!=0)
    {
        fscanf(fCodeCentrale, "%d %d", &codeC,&puiss);
        if(codeC!=0){
            insertion_Centrale(px);
            px=px->ptsuivant;
            px->codeCentrale= codeC;
            px->puissance_MAX=puiss;

            pdebutElec = (PTligneElectrique) malloc(sizeof(TligneElectrique));
            pdebutElec->ligneSuivante = NULL;
            px->villeDependante = pdebutElec;
        }

    }

    fclose(fCodeCentrale);
}

void login_Ligne(PTcentrale pCdebut, PTville pVdebut)
{
    int code, codeC, codeP, puiss;

    FILE * fLigneElec = NULL;
    fLigneElec=fopen("Ligne.txt","r");

    if(fLigneElec==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }

    while(codeC!=0)
    {

        fscanf(fLigneElec, "%d %d %d", &codeC, &codeP,&puiss);
        if(codeC!=0){
            lire_ligne(codeC,codeP,puiss,pCdebut,pVdebut);
        }


    }

    fclose(fLigneElec);
}

//ENREGISTREMENT DES LISTES

void EnregisVille(PTville pVdebut)
{
    PTville pv;//PTcentrale pc;PTligneElectrique pe;
    pv=pVdebut->villeSuivante;  
    
    int i=0;
    FILE * Ville = NULL;
    Ville=fopen("CodeVille.txt","w+");

    if(Ville==NULL){
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);}


    while(i<comptVille(pVdebut))
    {
        fprintf(Ville, "%d", pv->codePostal);
        fprintf(Ville, " ");
        pv=pv->villeSuivante;
        i++;

    }
    fprintf(Ville, "0 \n");
    fclose(Ville);
}

void EnregisCentrale(PTcentrale pCdebut)
{
	PTcentrale pc;
	pc=pCdebut->ptsuivant;
	int i=0;
    FILE * Centrale = NULL;
    Centrale = fopen("CodeCentrale.txt","w+");
	
	
    while(i<comptCentrale(pCdebut))
    {
        fprintf(Centrale, "%d %d", pc->codeCentrale, pc->puissance_MAX);
        fprintf(Centrale, " ");
        pc=pc->ptsuivant;
        i++;

    }
    fprintf(Centrale, "0 0\n");
    fclose(Centrale);
}

void EnregisLigne(PTville pVdebut,PTcentrale pCdebut)
{
	int i=0;
	PTcentrale pc;
	pc=pCdebut->ptsuivant;
	PTligneElectrique pe;
	
	FILE * Ligne = NULL;
    Ligne = fopen("Ligne.txt","w+");
		
    while(pc->ptsuivant!=NULL)
    {
        pc=pCdebut->ptsuivant;
        pe=pc->villeDependante;
        printf("YO");
        
        while(pe->ligneSuivante!=NULL)
        {
            fprintf(Ligne, "%d %d %d", pc->codeCentrale, pe->villeDesservie->codePostal,pe->puissance);
            fprintf(Ligne, " ");
            pe=pe->ligneSuivante;
            printf("Y2");
        }
        pc=pc->ptsuivant;
        
    }

    fprintf(Ligne, "0 0 0\n");
    fclose(Ligne);
}


// Fonction qui cree un cadre de taille et a l'emplacement voulu

void cadre(int x, int y, int haut, int larg){
    int i,j;
    char c,cvertical,  	chorizontal,   	cangleGH,    	cangleGB,    	cangleDH,    	cangleDB;
    cvertical=186;chorizontal=205;cangleGH=201;cangleGB=200;cangleDH=187;cangleDB=188;

    //ligne haut
    gotoLigCol(x,y);printf("%c",cangleGH);
    i=larg;
    while(i>0)	{
        printf("%c",chorizontal);
        i=i-1;
    };
    printf("%c",cangleDH);

    // lignes intermediaires
    j= haut;

    while(j>0){
        gotoLigCol(x+j,y);
        printf("%c",cvertical);
        i=larg;
        while(i>0)	{
            printf("%c",' ');
            i=i-1;
        };
        j=j-1;
        printf("%c",cvertical);
    }
    //ligne bas
    gotoLigCol(x+haut,y);printf("%c",cangleGB);
    i=larg;
    while(i>0)	{
        printf("%c",chorizontal);
        i=i-1;
    };
    printf("%c",cangleDB);
}


/////////////////////////////////////////////////////////////////////////////////// FONCTION D'AJOUT ////////////////////////////////////////////////////////////////////////////////

//Fonction qui permet d'ajouter une ou plusieurs villes a la liste ville precedement cree

void ajouterVille(int Lig,int Col,PTville pdebut){
    int i,nb=0,compt=0;
    PTville px;

    i=1;
    gotoLigCol(Lig+3,Col-3);printf("Combien de villes : ");
    scanf("%d",&nb);

    while(compt<nb){
        px=pdebut;
        while(px->villeSuivante!=NULL)
        {
            px=px->villeSuivante;
        }
        gotoDel(Lig+3,Col-4,44);
        i = saisiVille(pdebut);
        insertion_Ville(px);
        px=px->villeSuivante;
        px->codePostal= i;
        gotoDel(Lig+3,Col-4,44);
        compt++;

    }

}

//Fonction qui permet d'ajouter une ou plusieurs centrales a la liste centrale precedement cree

void ajouterCentrale(int Lig, int Col,PTcentrale pdebut){

    PTcentrale px,py;
    PTligneElectrique pdebutElec;
    int codeC,nb=0,compt=0;


    gotoLigCol(Lig+3,Col-3);printf("Combien de centrale :\t");
    scanf("%d",&nb);
    c:
    while(compt!=nb){
        px = pdebut;
        while (px->ptsuivant->ptsuivant!=NULL) {
            px = px->ptsuivant;
        }
        gotoDel(Lig+3,0,90);
        codeC = saisiCentrale(Lig+3,1,pdebut);
        if(codeC==0){goto c;}
        insertion_Centrale(px);
        px=px->ptsuivant;
        px->codeCentrale = codeC;
        px->puissance_MAX=0;

        /// Allocation d'une case pour ligne elec
        pdebutElec = (PTligneElectrique) malloc(sizeof(TligneElectrique));
        pdebutElec->ligneSuivante = NULL;
        px->villeDependante = pdebutElec;
        compt++;

    }
    gotoDel(Lig+3,0,90);
}



// Fonction qui permet de determiner la puisance maximale qu'une centrale peut fournir

void ajouterPuissCentrale(int cc,int puiss, PTcentrale pCdebut)
{
    PTcentrale pc;
    pc = getCentraleCode(cc,pCdebut);
    pc->puissance_MAX = puiss;

}


/////////////////////////////////////////////////////////////////////////////////// SUPPRESSION  ////////////////////////////////////////////////////////////////////////////////

//Fonction qui supprimme un element de la liste centrale
void suppCentrale(int p,PTcentrale pdebut){
    PTcentrale px, ps;


    px = getCentraleCode(p,pdebut);
    px=px->ptprecedent;
    px->ptsuivant->ptprecedent =px;
    px->ptsuivant = px->ptsuivant->ptsuivant;
    //free(ps);
}

//Fonction qui supprimme un element de la liste ville

void suppVille(int p,PTville pdebut){
    PTville px;

    px = getprecVille(p,pdebut);
    px->villeSuivante = px->villeSuivante ->villeSuivante;
    //free(ps);
}

//Fonction qui supprimme un element de la liste ligne

void suppLigne(int cc, int cp, PTcentrale pCdebut)
{
    PTligneElectrique pe;
    pe = getprecLigne(cc,cp,pCdebut);
    pe->ligneSuivante = pe->ligneSuivante ->ligneSuivante;
}

/////////////////////////////////////////////////////////////////////////////////// AFFICHAGE IHM ////////////////////////////////////////////////////////////////////////////////

///////////////////////////// LOGO

void lecture_image(int i)
{
    char copie;
    FILE * logo = NULL;
    FILE * image = NULL;
    
	if(i==2)
    {
        image=fopen("EsterEgg.txt","r");
        while(copie!='!'){
            fscanf(image, "%c", &copie);
            printf("%c",copie);
        }
    }



}

////////////////////////////////////////////////////////////////////////////////// CADRE IHM ////////////////////////////////////////////////////////////////////////////////



/////////// Petit + affichage des puissances centrale ////////////////////////////////////////////////////////////

void affichegraphique(int lig, int col,PTcentrale pdebut)
{
    PTcentrale pc;
    int carre=254;
    int i=0;int b=0; int nbc=2;
    pc=pdebut->ptsuivant;
    gotoLigCol(lig+1,col);

    while(pc->ptsuivant!=NULL)
    {
        i=0;
        printf("Centrale %d \t",pc->codeCentrale);
        while(i<pc->puissance_MAX && i<100)
        {
            Color(2,0);printf("%c",carre);
            i++;
        }
        Color(15,0);printf("  %d kW", i);
        gotoLigCol(lig+nbc,col);
        pc=pc->ptsuivant;
        nbc++;
    }


}


// Fonction qui affiche la liste centrale

void affiche_Centrale(PTcentrale pdebut) {
    PTcentrale px;

    printf("\n\n Affichage de la liste Centrale \n");
    px = pdebut->ptsuivant;
    while (px->ptsuivant->ptsuivant!=NULL) {
        printf(" - %d : PMAX = %d \t ",px->codeCentrale,px->puissance_MAX);
        px = px->ptsuivant;
    }
}

// Fonction qui affiche la liste ville

void affiche_Ville(PTville pdebut ) {
    PTville px;

    //printf("\n\n Affichage de la liste Ville \n");
    px = pdebut->villeSuivante;
    while (px!=NULL){
        printf("\t%d",px->codePostal);
        px = px->villeSuivante;
    }
}

// Fonction qui affiche la liste ligne

void affiche_ligne(int cc,PTcentrale pCdebut)
{
    PTligneElectrique pe;
    PTcentrale pc;

    pc = getCentraleCode(cc, pCdebut);
    pe = pc->villeDependante->ligneSuivante;
    printf(" \n\nAffichage des lignes electriques\n");
    while(pe!=NULL)
    {
        printf(" \tLa puissance est %d pour la ville %d depuis la centrale %d\n",pe->puissance, pe->villeDesservie->codePostal,pc->codeCentrale);
        pe = pe->ligneSuivante;
    }

}

//Fonction qui permet de dï¿½finir la puissance alloue a une ou plusieurs villes depuis une centrale

void ajouterPuissVille(int Lig, int Col,int cc,PTcentrale pCdebut, PTville pVdebut){

    PTcentrale pc;
    PTligneElectrique pe;
    PTville pvx,pvy;
    int p=1,puiss=0, nb=0;
    char i=0;

    pvx = pVdebut->villeSuivante;
    pc = getCentraleCode(cc, pCdebut);
    pe = pc->villeDependante;

    

    gotoLigCol(6,20);printf("Selectionner la ville ... (Entree)");
    cadre(7, 1, 2, 60);

    while (i!= 27)
    {
        gotoLigCol(8, 5);
        printf("%d          ", pvx->codePostal);
        
        i = lireCaract();
        		
        if (i == 13) {
        	
        	while(pe->ligneSuivante!=NULL)
			    {
			        pe = pe->ligneSuivante;
			    }
    
	        insertion_Ligne(pe);
            pe = pe->ligneSuivante;
            pe->villeDesservie = pvx;
            gotoLigCol(10, 5);printf(" Entrer la puissance ville : \t");
            scanf("%d",&p);
            gotoLigCol(11, 5);printf(" (Echap) pour terminer l'ajout... \t");
            pe->puissance = p;
            

        }
        
        if (i==-37){if(pvx!=pVdebut->villeSuivante){ // fleche doite
                pvy=pVdebut;
                while(pvy->villeSuivante!=pvx)pvy=pvy->villeSuivante;};
            pvx=pvy;}

        else if (i==-35){if (pvx->villeSuivante!=NULL)pvx=pvx->villeSuivante;}
        else {}
    }
}

// Fonction qui affcihe la ville avec un cadre et les fleches pour parcourir la liste

void affichageVille(PTville pdebut, int lig, int col,PTcentrale pCdebut){

    int i=0, codeP=0; char v; int m=1;
    PTville px,py;
    px=pdebut->villeSuivante;
        
    while (i!=27){
    	
    	Color(15,0);
   		gotoLigCol(lig-2,col+25);printf("VILLES");
    	gotoLigCol(lig+7,col-3);printf("Suivant : Fleche droite >>");
    	gotoLigCol(lig+8,col-3);printf("Precedent : Fleche gauche << ");
    	
    	gotoLigCol(lig+10,col-3);printf("Regarder puissance");
	    gotoLigCol(lig+11,col-3);printf("Ajouter villes");
	    gotoLigCol(lig+12,col-3);printf("Supprimer villes");
	    gotoLigCol(lig+13,col-3);printf("Nombre de villes");
	    gotoLigCol(lig+14,col-3);printf("Affichage complet");
	    gotoLigCol(lig+16,col-3);printf("Quitter : Echap");
	    
    	gotoLigCol(lig,col);        
        if(comptVille(pdebut)!=0)
        {
            Color(15,0);printf("%d          ",px->codePostal);
        }
        else{
            Color(12,0);printf("Aucun element");delay(1);exit(1);
        }
        
        if(i==480)
	        {
	        	if(m<6){m=m+1;}
	       		 
			}
	    else if(i== 472)
	        {
	        	
	    		if(m>1){m=m-1;}
				
			}
		else if (i==475){if(px!=pdebut->villeSuivante){ // fleche doite
                py=pdebut;
                while(py->villeSuivante!=px){
				py=py->villeSuivante;};
            px=py;}}

        else if(i==477){if (px->villeSuivante!=NULL)px=px->villeSuivante;} // fleche gauche	
                
        if (m==1){Color(11,0);gotoLigCol(lig+10,col-3);printf("Regarder puissance");if(i==13){}}//gotoLigCol(lig+3,col);printf("La puissance totale accorde a cette ville est : %d",comptPuissVille(px->codePostal,pCdebut));}} 
                     
        else if (m==4){
        	Color(11,0);gotoLigCol(lig+13,col-3);printf("Nombre de villes");
			if(i==13){Color(15,0);gotoLigCol(lig+4,col);printf("Il y a : %d villes",comptVille(pdebut));}} // touche F2
            
        else if(m==5){Color(11,0);gotoLigCol(lig+14,col-3);printf("Affichage complet");if (i==13){gotoDel(lig+4,col,80);Color(15,0);(affiche_Ville(pdebut));}}
        else if(m==2){Color(11,0);gotoLigCol(lig+11,col-3);printf("Ajouter villes");if (i==13){Color(15,0);ajouterVille(lig+1,col,pdebut);}}
        else if(m==3){Color(11,0);gotoLigCol(lig+12,col-3);printf("Supprimer villes");if (i==13){Color(15,0);gotoLigCol(lig+4,col);
            if(comptVille(pdebut)!=0){
                gotoLigCol(lig+4,col);codeP=px->codePostal;printf("Voulez-vous supprimer, la ville %d   (Y/N) ",px->codePostal);v=getch();
                if(v=='y')
                {
                    if(px!=pdebut->villeSuivante)
                    {px=getprecVille(codeP,pdebut);suppVille(codeP,pdebut);gotoDel(lig+4,col,45);}
                    else
                    {
                        px=px->villeSuivante;suppVille(codeP,pdebut);gotoDel(lig+4,col,45);
                    }
                }else{
                    gotoDel(lig+4,col,45);
                }}
            else{
                gotoDel(lig+4,col,45);printf("Le dernier element ne peut etre supprimer");
            }
        }}
        //else if(m==6){if (i== 13){gotoDel(lig+4,col,100);printf("La puissance accorder a cette ville est %d",comptPuissVille(px->codePostal,pCdebut));delay(1);}}
        else if(m==6){Color(12,0);gotoLigCol(lig+16,col-3);printf("Quitter : Echap"); if (i== 13){i=27;}}
        else if(i== 27){}				// touche ESC sortir du programme
        i=lireCaract();
    }
}

void affichageCentrale(PTcentrale pdebut, int lig, int col){
    PTcentrale px,py;
    int i, codeC, puiss, v=0, x=0, m=1;

    px=pdebut->ptsuivant;
    i=0;
    gotoLigCol(lig-2,col+25);printf("CENTRALE");
    gotoLigCol(lig+7,col);printf("Suivant : Fleche droite >>");
    gotoLigCol(lig+8,col);printf("Precedent : Fleche gauche <<");
    gotoLigCol(lig+10,col);printf("Puissance MAX");
    gotoLigCol(lig+11,col);printf("Ajouter centrale");
    gotoLigCol(lig+12,col);printf("Supprimer centrale");
    gotoLigCol(lig+13,col);printf("Nombre de centrale");
    gotoLigCol(lig+14,col);printf("Affichage complet");
    gotoLigCol(lig+16,col);printf("Quitter : Echap");


    while (i!=27){
    	
       	Color(15,0);
    	gotoLigCol(lig+10,col);printf("Puissance MAX");
    	gotoLigCol(lig+11,col);printf("Ajouter centrale");
    	gotoLigCol(lig+12,col);printf("Supprimer centrale");
    	gotoLigCol(lig+13,col);printf("Nombre de centrale");
    	gotoLigCol(lig+14,col);printf("Affichage complet");
    	gotoLigCol(lig+16,col);printf("Quitter : Echap");
    	
        gotoLigCol(lig,col);
		
        if(comptCentrale(pdebut)!=0)
        {
            printf(" %d : PMAX = %d   ",px->codeCentrale,px->puissance_MAX);
        }
        else{
            Color(12,0);printf("Aucun element");Color(15,0);
        }
	
      
      
        if (i==475){if(px!=pdebut->ptsuivant){
                py=pdebut->ptsuivant;
                while(py->ptsuivant!=px){py=py->ptsuivant;};
            px=py;}}// fleche haut
        else if (i==477){if (px->ptsuivant->ptsuivant!=NULL)px=px->ptsuivant;} // fleche bas
        if(i==480)
	        {
	        	if(m<6){m=m+1;}
	       		 
			}
	    else if(i== 472)
	        {
	        	
	    		if(m>1){m=m-1;}
				
			}
		if (m==1){Color(11,0);gotoLigCol(lig+10,col);printf("Puissance MAX ");if(i== 13){Color(15,0);gotoLigCol(lig+3,col);printf("Quelle puissance Max pour la centrale %d : ",px->codeCentrale);scanf("%d",&puiss);ajouterPuissCentrale(px->codeCentrale,puiss, pdebut);gotoDel(lig+3,col,50);}}
        else if(m==4){Color(11,0);gotoLigCol(lig+13,col);printf("Nombre de centrale");if(i==13){Color(15,0);gotoLigCol(lig+2,1);printf("Il y a : %d centrales",comptCentrale(pdebut));
            affichegraphique(lig+2,col,pdebut);delay(2);while(x-1<comptCentrale(pdebut)){gotoDel(lig+2+x,0,80);x++;}x=0;}} // touche F2
        else if(m==5){Color(11,0);gotoLigCol(lig+14,col);printf("Affichage complet");if(i==13){Color(15,0);gotoLigCol(lig+4,col-15);(affiche_Centrale(pdebut));}}
        else if(m==2){Color(11,0);gotoLigCol(lig+11,col);printf("Ajouter centrale");if (i==13){Color(15,0);ajouterCentrale(lig+1,col,pdebut);}}
        else if(m==3){Color(11,0);gotoLigCol(lig+12,col);printf("Supprimer centrale");if (i==13){Color(15,0);gotoLigCol(lig+4,col);codeC=px->codeCentrale;
		printf("Voulez-vous supprimer la centrale %d   (Y/N) ",px->codeCentrale);v=getch();
                if(v=='y')
                {
                	if(px!=pdebut->ptsuivant)
                	{
                	px=getCentraleCode(codeC,pdebut)->ptprecedent;suppCentrale(codeC,pdebut);gotoDel(lig+4,col,45);	
					}
                    else{
						px=getCentraleCode(codeC,pdebut)->ptsuivant;suppCentrale(codeC,pdebut);gotoDel(lig+4,col,50);
					}
                }else{
                    gotoDel(lig+4,col,45);
                }
            
        }}
        else if(m==6){Color(12,0);gotoLigCol(lig+16,col);printf("Quitter : Echap");if(i==13){ i=27;}}
        else if (i== 27){} 					// touche ESC sortir du programme
        else {
        }
		i=lireCaract();
    }
}

void affichageLigne(int lig, int col,PTcentrale pCdebut,PTville pVdebut){

    int i=0,codeP,puiss,codeC,d,m=1;
	char v;
    PTcentrale pc, pcx, pcy;
    PTligneElectrique pex,pey, pedebut;
    pcx = pCdebut->ptsuivant;

    while(i!=27){

        gotoLigCol(1,20);printf("Selectionner la centrale ... (Entree)");
        cadre(2, 1, 2, 60);
        while (i != 27) {
            gotoLigCol(3, 5);

            printf(" %d : PMAX = %d   ", pcx->codeCentrale, pcx->puissance_MAX);

            i=lireCaract();

            if(i == 13) {
                codeC = pcx->codeCentrale;
                pc = getCentraleCode(codeC, pCdebut);
                pedebut = pc->villeDependante;
                pex = pedebut->ligneSuivante;
                if (comptVilleCentrale(codeC, pCdebut) != 0) {
                    retouraffiche:
                    system("cls");

                    Color(15, 0);
                    gotoLigCol(lig - 2, col + 20);printf("LIGNE CENTRALE %d", codeC);
                    gotoLigCol(lig + 7, col);printf("Suivant : Fleche Bas");
                    gotoLigCol(lig + 8, col);printf("Precedent : Fleche haut");
                    gotoLigCol(lig + 15, col);printf("Quitter : Echap");
					gotoLigCol(lig + 14, col);printf("Nombre ligne");
	                gotoLigCol(lig + 13, col);printf("Affichage complet");
	                gotoLigCol(lig + 12, col);printf("Statistique");
	                gotoLigCol(lig + 10, col);printf("Ajouter ligne");
	                gotoLigCol(lig + 11, col);printf("Supprimer ligne");
                    cadre(2, 1, 2, 60);
					i=1;
                    while (i != 27) {
                    	
                    	Color(15,0);
					   gotoLigCol(lig + 15, col);printf("Quitter : Echap");
						gotoLigCol(lig + 14, col);printf("Nombre ligne");
	                	gotoLigCol(lig + 13, col);printf("Affichage complet");
	                	gotoLigCol(lig + 12, col);printf("Statistique");
	                	gotoLigCol(lig + 10, col);printf("Ajouter ligne");
	                	gotoLigCol(lig + 11, col);printf("Supprimer ligne");
                        gotoLigCol(3, 5);

                        if (comptVilleCentrale(codeC, pCdebut) != 0) {
                            printf("%d : %d wh       ", pex->villeDesservie->codePostal, pex->puissance);
                        } else {
                            printf("Aucun element");
                        }

                        if (i == 475) {
                            if (pex != pedebut->ligneSuivante) {
                                pey = pedebut;
                                while (pey->ligneSuivante != pex){pey = pey->ligneSuivante;
                            };
                            pex = pey;}
                        } else if (i == 477) { if (pex->ligneSuivante != NULL)pex = pex->ligneSuivante; } // fleche bas
                        
                        if(i==480)
					        {
					        	if(m<6){m=m+1;}
					       		 
							}
					    else if(i== 472)
					        {
					        	
					    		if(m>1){m=m-1;}
								
							}
                                      
                        if (m==5) {Color(11,0);gotoLigCol(lig + 14, col);printf("Nombre ligne");if(i==13){
                            gotoDel(lig + 4, col - 7, 80);
                            printf("Il y a : %d lignes depuis la centrale %d", comptVilleCentrale(codeC, pCdebut),
                                   codeC);
                        } }
                        else if(m==4) {Color(11,0);gotoLigCol(lig + 13, col);printf("Affichage complet");if(i==13){
                            gotoLigCol(lig + 4, col - 15);
                            (affiche_ligne(codeC, pCdebut));
                        }}
                        else if(m==3) {Color(11,0);gotoLigCol(lig + 12, col);printf("Statistique");if(i==13){
                            gotoLigCol(lig + 4, col);
                            printf("Il y a %d villes dependantes", comptVilleCentrale(codeC, pCdebut));
                            printf("\tAvec un total de %d Wh", comptPuiss(codeC, pCdebut));
                            printf("\tAvec une moyenne de %d Wh par Ville",
                                   (comptPuiss(codeC, pCdebut) / comptVilleCentrale(codeC, pCdebut)));}

                        }else if (m==1) {Color(11,0);gotoLigCol(lig + 10, col);printf("Ajouter ligne");if(i==13){d=7; while(d<15){gotoDel(lig+d,col,50); d++;}ajouterPuissVille(lig + 1, col, codeC, pCdebut, pVdebut);}}
                        else if(m==2) {Color(11,0);gotoLigCol(lig + 11, col);printf("Supprimer ligne");if(i==13){
                            gotoLigCol(lig + 4, col);
                            codeP = pex->villeDesservie->codePostal;
                            printf("Voulez-vous supprimer, la ligne de la ville %d   (Y/N) ",
                                   pex->villeDesservie->codePostal);
                            v = getch();
                            if (v == 'y') {
                                pex = getprecLigne(codeC, codeP, pCdebut);
                                suppLigne(codeC, codeP, pCdebut);
                                gotoDel(lig + 4, col, 55);
                            }
                            else { gotoDel(lig + 4, col, 70); }
                        }}
                            //else if (i=='s'){gotoLigCol(lig+4,col);codeP = existeVille(pVdebut);suppLigne(codeC,codeP,pCdebut);}
                        else if(m==6) {Color(12,0);gotoLigCol(lig + 15, col);printf("Quitter : Echap");if(i==13){i=27;}}
						else if (i == 27) {}                    // touche ESC sortir du programme
                        else {
                        }
                        i = lireCaract();
                        
                    }}
                else {
                    system("cls");
                    printf("La liste est vide");
                    delay(1);
                    system("cls");
                    i = 27;
                }
            }
            else if (i == 475){if (pcx != pCdebut->ptsuivant) {
                    pcy = pCdebut->ptsuivant;
                    while (pcy->ptsuivant != pcx){pcy = pcy->ptsuivant;
                };pcx = pcy;}} // fleche haut
            else if (i == 477) { if (pcx->ptsuivant->ptsuivant != NULL)pcx = pcx->ptsuivant; } // fleche bas
            else {}
        }
    }
}

void creation_ligne(PTcentrale pCdebut, PTville pVdebut)
{
    PTcentrale pcx, pcy;
    PTville pvx, pvy;
    int i = 0, puiss = 0, Totp=0, PMAX=0;

    pcx = pCdebut->ptsuivant;
    pvx = pVdebut->villeSuivante;
    

    gotoLigCol(1,20);printf("Selectionner la centrale ... (Entree)");
    cadre(2, 1, 2, 60);
    while (i != 27) {
        gotoLigCol(3, 5);

        printf(" %d : PMAX = %d   ", pcx->codeCentrale, pcx->puissance_MAX);

        i = lireCaract();
        if (i == 13) {
        	PMAX = pcx->puissance_MAX;
        	gotoLigCol(6,20);printf("Selectionner la ville ... (Entree)");
            cadre(7, 1, 2, 60);
            
            while (i != 27) {
                
                gotoLigCol(8, 5);

                printf("%d          ", pvx->codePostal);

                i = lireCaract();
                if (i == 13) {
                    gotoDel(10, 5,55);printf("Entrer la puissance :\t");
                    scanf("%d", &puiss);
                    Totp= Totp + puiss;
                    if(Totp>PMAX)
			        {
			            Color(12,0);gotoDel(12, 5,55);printf("Puissance = %d", Totp);gotoDel(13, 5,55);printf("ATTENTION SEUIL MAXIMAL DEPASSE");delay(1);gotoDel(12, 5,70);gotoDel(11, 5,70);Color(15,0);
			        }
                    lire_ligne(pcx->codeCentrale, pvx->codePostal, puiss, pCdebut, pVdebut);
                    gotoDel(16, 5,55);affiche_ligne(pcx->codeCentrale,pCdebut);
                } else if (i == 475) {
                    if (pvx != pVdebut->villeSuivante) { // fleche doite
                        pvy = pVdebut;
                        while (pvy->villeSuivante != pvx)pvy = pvy->villeSuivante;
                    };
                    pvx = pvy;
                } else if (i == 477) { if (pvx->villeSuivante != NULL)pvx = pvx->villeSuivante; }
                else {}
            }
        } else if (i == 475) {
            if (pcx != pCdebut->ptsuivant) {
                pcy = pCdebut->ptsuivant;
                while (pcy->ptsuivant != pcx){pcy = pcy->ptsuivant;};
            pcx = pcy;}
        } // fleche haut
        else if (i == 477) { if (pcx->ptsuivant->ptsuivant != NULL)pcx = pcx->ptsuivant; } // fleche bas
        else {}
    }
}


/////////////////////////////////////////////////////////////////////////////////// MENU IHM ////////////////////////////////////////////////////////////////////////////////

	// Fonction qui affiche le menu centrale
	void afficheMenuCentrale(int Lig, int Col,int nb)
	{
	    if(nb==4){
		gotoDel(Lig,Col,30);printf("Creation  centrales");	
		}
	    if(nb==5){
	    gotoDel(Lig+1,Col,30);printf("Afficher centrales");
		}
	}
	
	// Fonction qui affiche le menu ville
	void afficheMenuVille(int Lig, int Col,int nb)
	{
		if(nb==1){
		gotoDel(Lig,Col,30);printf("Creation villes");	
		}
	    else if(nb==2){
	    gotoDel(Lig+1,Col,30);printf("Afficher villes");
		}
	   
	}
	
	// Fonction qui affiche le menu ligne
	void afficheMenuLigne(int Lig, int Col, int nb)
	{
		if(nb==7){
		gotoDel(Lig,Col,30);printf("Creation lignes");	
		}
	    else if(nb==8){
	    gotoDel(Lig+1,Col,30);printf("Afficher lignes");
		}
	
	}

	void afficheMenuAutre (int Lig, int Col, int nb)
	{
		if(nb==1){
		gotoDel(Lig,Col,30);printf("Charger reseaux");	
		}
	    else if(nb==2){
	    gotoDel(Lig+1,Col,30);printf("Enregistrer reseaux");
		}
	    else if(nb==3){
	    gotoDel(Lig+2,Col,30);printf("Echap - Quitter");
		}  
	
	}


/////////////////////////////////////////////////////////////////////////////////// MAIN ////////////////////////////////////////////////////////////////////////////////

int main() {

    //// initialisation des variables ///////

    PTcentrale pdebutCentrale, pfinCentrale;
    PTville pdebutVille;
    int nb, codeP, codeC,puiss, compt, i=1, m=1;
    char c=1; char nom[30];

    ///// initilaisation des listes //////////

    pdebutVille = (PTville) malloc(sizeof(Tville));
    pdebutVille->villeSuivante = NULL;

    pdebutCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pfinCentrale = (PTcentrale) malloc(sizeof(Tcentrale));
    pdebutCentrale->ptprecedent = NULL;
    pfinCentrale->ptsuivant = NULL;
    pdebutCentrale->ptsuivant = pfinCentrale;
    pfinCentrale->ptprecedent = pdebutCentrale;

    //////// MENU ///////////


    ////////// Intro ///////////
    
    gotoLigCol(35,52);printf("PROJET CENTRALE");
    gotoLigCol(50,0);printf("...");
    delay(1);
    Color(3,0);gotoLigCol(37,46);printf("Cree par Titouan WACRENIER");Color(15,0);
    gotoLigCol(50,0);printf("...");
    delay(1); 
    
    debut:
    system("cls");
    
    gotoLigCol(1,23); Color(11,0);printf("VILLE");Color(15,0);
    cadre(2,5,3,40); afficheMenuVille(3,8,1);afficheMenuVille(3,8,2);
    gotoLigCol(6,22); Color(11,0);printf("CENTRALE");Color(15,0);
    cadre(7,5,3,40);afficheMenuCentrale(8,8,4);afficheMenuCentrale(8,8,5);
    gotoLigCol(11,23); Color(11,0);printf("LIGNE");Color(15,0);
    cadre(12,5,3,40);afficheMenuLigne(13,8,7);afficheMenuLigne(13,8,8);
    cadre(17,5,4,40);afficheMenuAutre(18,8,1);afficheMenuAutre(18,8,2);afficheMenuAutre(18,8,3);
    //gotoLigCol(24,5);printf("%d %d",comptCentrale(pdebutCentrale),comptVille(pdebutVille));
    
    i=1;
    
    while(i!= 27){
    	    	     	    	
	        if(i==480)
	        {
	        	if(m<9){m=m+1;}
	       		
			}
	        else if(i== 472)
	        {
	        	
	    		if(m>1){m=m-1;}
				
			}

		        
        //////////////////////////////////////////// VILLE ////////////////////////////////////////
		if(m==1){
        	Color(11,0);afficheMenuVille(3,8,1);
			if(i==13){
				Color(15,0);
	            if(comptVille(pdebutVille)!=0){
	                gotoDel(23,5,45); printf("La liste existe deja");delay(1);gotoDel(23,5,44);
	            }
	            else{
	                system("cls");cadre(0,24,2,60); gotoLigCol(1,42);printf("Creation de liste Ville");
	                Color(11,0);gotoLigCol(3,5);printf("Entrer les codes postaux");
	                gotoLigCol(4,5);printf("Terminer avec 0");Color(15,0);
	                gotoLigCol(7,5);lire_Ville(pdebutVille);system("cls");goto debut;
	        	}
        		}
    	}
        else if(m==2){
        	Color(11,0);afficheMenuVille(3,8,2);
			if(i==13){
				Color(15,0);
	            if(comptVille(pdebutVille)==0){
	                gotoDel(23,5,44);Color(12,0);printf("La liste est vide");delay(1);gotoDel(23,5,44);Color(15,0);
	            }
	            else{
	                system("cls");cadre(2,1,2,60);affichageVille(pdebutVille,3,5,pdebutCentrale);system("cls");goto debut;
	            }
        	}
		}
	
        else if(m==7){Color(11,0);afficheMenuAutre(18,8,1);if(i==13){Color(15,0);login_CodeVille(pdebutVille);login_CodeCentrale(pdebutCentrale);login_Ligne(pdebutCentrale, pdebutVille);
		gotoDel(23,5,80);Color(2,0);printf("Le fichier a bien ete charger");Color(15,0);delay(1);gotoDel(23,5,80);}}
        else if(m==8){Color(11,0);afficheMenuAutre(18,8,2);if(i==13){Color(15,0);EnregisVille(pdebutVille);EnregisCentrale(pdebutCentrale);login_Ligne(pdebutCentrale,pdebutVille);gotoDel(23,5,80);Color(2,0);printf("Le fichier a bien ete enregistrer");Color(15,0);delay(1);gotoDel(23,5,80);}}
        
	
            //////////////////////////////////////////// CENTRAL ////////////////////////////////////////

        else if(m==3){
        	Color(11,0);afficheMenuCentrale(8,8,4);
			if (i==13){
				Color(15,0);
	            if(comptCentrale(pdebutCentrale)!=0){
	                gotoDel(23,5,45);printf("La liste existe deja");delay(1);gotoDel(23,5,80);}
	            else{
	                system("cls");cadre(0,24,2,60); gotoLigCol(1,42);printf("Creation de liste Centrale ");
	                Color(11,0);gotoLigCol(3,5);printf("Entrer les codes de la centrale");
	                gotoLigCol(4,5);printf("Terminer avec 0");Color(15,0);
	                gotoLigCol(7,5);lire_Centrale(pdebutCentrale);system("cls");goto debut;
	            }
	        }
		}
		
        else if(m==4){
        	Color(11,0);afficheMenuCentrale(8,8,5);
			if(i==13){
				Color(15,0);
	            if(comptCentrale(pdebutCentrale)==0){
	                gotoDel(23,5,44);Color(12,0);printf("La liste est vide");delay(1);gotoDel(23,5,80);Color(15,0);}
	
	            else{system("cls");cadre(2,1,2,60);affichageCentrale(pdebutCentrale,3,5);system("cls");goto debut;
	            }
	        }
		}
            //////////////////////////////////////////// LIGNE ////////////////////////////////////////

        else if(m==5){
        	Color(11,0);afficheMenuLigne(13,8,7);
			if (i==13){
				Color(15,0);
	            if(comptVille(pdebutVille) ==0){
	                gotoDel(23,5,44);Color(12,0);printf("Il n'y  pas d'element dans la liste Ville");
	                gotoDel(24,5,44);printf("Creation liste Ligne impossible");Color(15,0);
	                delay(2);system("cls");goto debut;}
	            if(comptCentrale(pdebutCentrale) ==0){
	                gotoDel(23,5,44);Color(12,0);printf("Il n'y  pas d'element dans la liste Centrale");
	                gotoDel(24,5,44);printf("Creation liste Ligne impossible");Color(15,0);
	                delay(2);system("cls");goto debut;
	            }else{system("cls");creation_ligne(pdebutCentrale,pdebutVille);system("cls");goto debut;}}
			}
	            
        else if(m==6){
        	Color(11,0);afficheMenuLigne(13,8,8);
			if(i==13) {
				Color(15,0);
	            if (comptCentrale(pdebutCentrale) == 0) {
	                gotoDel(23, 5, 44);Color(12,0);
	                printf("La liste est vide");
	                delay(1);Color(15,0);
	                gotoDel(23, 5, 80);goto debut;
	            } else {
	                system("cls");
	                cadre(2, 1, 2, 60);
	                affichageLigne(3, 5, pdebutCentrale, pdebutVille);
	                system("cls");goto debut;
	            }
	        }
		}
		else if(m==9){Color(12,0);afficheMenuAutre(18,8,3);if(i==13) {Color(15,0);gotoLigCol(25,0);exit(1);}}
		if(i=='s' || i=='S'){system("cls");Color(10,0);lecture_image(2);delay(2);system("cls");Color(15,0);goto debut;}
        
		Color(11,0);gotoDel(23,5,80);printf("Selectionner un element ... \n");Color(15,0);
		i = lireCaract();
		Color(15,0);afficheMenuLigne(13,8,8);afficheMenuLigne(13,8,7);afficheMenuCentrale(8,8,5);afficheMenuCentrale(8,8,4);
		afficheMenuVille(3,8,2);afficheMenuVille(3,8,1);afficheMenuAutre(18,8,1);afficheMenuAutre(18,8,2);afficheMenuAutre(18,8,3);
		
						
    }
    
  	
}



















