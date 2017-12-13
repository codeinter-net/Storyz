#include "table.h"

Table::TableColDef colNameList[20]=
{{"sequence","Découpage",80,64},{"date","Date",100,64},{"timing","Durée",60,64},{"place","Lieu",100,256},{"digest","Résumé",160,1024},{"description","Description",240,1024},{"cast","Personnages",100,256},{"dialog","Dialogue",240,1024},{"notes","Notes",100,256},{"storyboard","Storyboard",60,256},{0,0,0}};


char fileSelectorLoad[]="Ouvrir un document";
char fileSelectorSave[]="Sauver le document";
char fileSelectorFilter[]="Document Storyz\t*.syz\n";
char fileNotSaved[]="Le document en cours n'a pas été sauvé.";

char listItemDelete[]="Voulez-vous vraiment supprimer\n%s ?";
char listItemNew[]="Nouveau %d";
char listItemNoBlank[]="Le nom ne peut pas être vide !";
char listItemDouble[]="Le nom %s est en double !";

char choiceYes[]="Oui";
char choiceNo[]="Non";
char choiceContinue[]="Continuer";
char choiceCancel[]="Annuler";

char fileSelectorImageLoad[]="Choisir une image";
char fileSelectorImageFilter[]="Image\t*.{jpg,jpe,jpeg,png,gif}\n";

