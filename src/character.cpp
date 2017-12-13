#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include "StoryzInterface.h"
#include "main.h"
#include "data.h"
#include "toolbox.h"

// Fênètre de gestion des personnages

// TODO
// Ajouter :
// - Photo
// - Sexe
// - Age
//	- Taille
// - Poids
// Liens :
// - Actions : vers la timetine
// - Relations : avec autres personnages
// - Habite / travaille / ... : cartes

Fl_Text_Buffer characterDescriptionBuffer;
Fl_Text_Buffer characterActionsBuffer;
Fl_Text_Buffer characterGoalsBuffer;
Fl_Text_Buffer characterLinksBuffer;

Toolbox *characterToolbox;

void characterWindowCallback(Fl_Widget *widget, void *) // Appelé lors de la fermeture de la fenêtre
{
	Fl_Window *window = (Fl_Window *)widget;
	if(!characterToolbox->setData()) return;
	window->hide();
}

void characterToolboxInit()
{
	characterToolbox=new Toolbox(Data::CHARACTER,10,mainInterface->characterList,data);
	characterToolbox->setWidgetList(0,mainInterface->characterName,0,0,0);
	characterToolbox->setWidgetList(1,mainInterface->characterFullName,0,"fullName",0);
	characterToolbox->setWidgetList(2,mainInterface->characterGender,0,"gender",0);
	characterToolbox->setWidgetList(3,mainInterface->characterAge,0,"age",0);
	characterToolbox->setWidgetList(4,mainInterface->characterSize,0,"size",0);
	characterToolbox->setWidgetList(5,mainInterface->characterWeight,0,"weight",0);
	characterToolbox->setWidgetList(6,0,mainInterface->characterDescription,"description",&characterDescriptionBuffer);
	characterToolbox->setWidgetList(7,0,mainInterface->characterActions,"actions",&characterActionsBuffer);
	characterToolbox->setWidgetList(8,0,mainInterface->characterGoals,"goals",&characterGoalsBuffer);
	characterToolbox->setWidgetList(9,0,mainInterface->characterLinks,"links",&characterLinksBuffer);
}

void characterListNew(class Fl_Button *b,void *e)
{
	characterToolbox->listNew(b,e);
}

void characterListDelete(class Fl_Button *b,void *e)
{
	characterToolbox->listDelete(b,e);
}

void characterListUp(class Fl_Button *b,void *e)
{
	characterToolbox->listUp(b,e);
}

void characterListDown(class Fl_Button *b,void *e)
{
	characterToolbox->listDown(b,e);
}

void characterListInit()
{
	if(!characterToolbox) characterToolboxInit();
	characterToolbox->listInit();
}

void openCharacterWindow() // Ouverture de la fenêtre + initialisations
{
	if(!characterToolbox) characterToolboxInit();
	mainInterface->characterWindow->show();
	mainInterface->characterWindow->callback(characterWindowCallback);
	characterToolbox->editOff();
	characterListInit();
}

void clearCharacterWindow()
{
	if(!characterToolbox) return;
	characterToolbox->editOff();
	characterToolbox->listInit();
}

void menuOpenCharacter(Fl_Menu_* , void*)
{
	openCharacterWindow();
}

void toolbarOpenCharacter(class Fl_Button *,void *)
{
	openCharacterWindow();
}

void characterListCallback(class Fl_Browser *list,void *data)
{
	if(!characterToolbox->setData()) return;
	int select=characterToolbox->getSelectedItem();
	if(!select)
	{
		characterToolbox->editOff();
		return;
	}
	characterToolbox->editOn();
	characterToolbox->getData();
}

void characterNameCallback(class Fl_Input *,void *)	// Initialise le champ nom
{
	int select=characterToolbox->getSelectedItem();
	if(!select) return;
	mainInterface->characterList->text(select,mainInterface->characterName->value());
}
