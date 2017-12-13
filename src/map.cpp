#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include "StoryzInterface.h"
#include "main.h"
#include "data.h"
#include "toolbox.h"

// Fênètre de gestion des maps

Fl_Text_Buffer mapTextBuffer;

Toolbox *mapToolbox;

void mapWindowCallback(Fl_Widget *widget, void *) // Appelé lors de la fermeture de la fenêtre
{
	Fl_Window *window = (Fl_Window *)widget;
	if(!mapToolbox->setData()) return;
	window->hide();
}

void mapToolboxInit()
{
	mapToolbox=new Toolbox(Data::NOTE,2,mainInterface->mapList,data);
	mapToolbox->setWidgetList(0,mainInterface->mapName,0,0,0);
	mapToolbox->setWidgetList(1,0,mainInterface->mapText,"text",&mapTextBuffer);
}

void mapListNew(class Fl_Button *b,void *e)
{
	mapToolbox->listNew(b,e);
}

void mapListDelete(class Fl_Button *b,void *e)
{
	mapToolbox->listDelete(b,e);
}

void mapListUp(class Fl_Button *b,void *e)
{
	mapToolbox->listUp(b,e);
}

void mapListDown(class Fl_Button *b,void *e)
{
	mapToolbox->listDown(b,e);
}

void mapListInit()
{
	if(!mapToolbox) mapToolboxInit();
	mapToolbox->listInit();
}

void openMapWindow() // Ouverture de la fenêtre + initialisations
{
	if(!mapToolbox) mapToolboxInit();
	mainInterface->mapWindow->show();
	mainInterface->mapWindow->callback(mapWindowCallback);
	mapToolbox->editOff();
	mapListInit();
}

void clearMapWindow()
{
	if(!mapToolbox) return;
	mapToolbox->editOff();
	mapToolbox->listInit();
}

void menuOpenMap(Fl_Menu_* , void*)
{
	openMapWindow();
}

void toolbarOpenMap(class Fl_Button *,void *)
{
	openMapWindow();
}

void mapListCallback(class Fl_Browser *list,void *data)
{
	if(!mapToolbox->setData()) return;
	int select=mapToolbox->getSelectedItem();
	if(!select)
	{
		mapToolbox->editOff();
		return;
	}
	mapToolbox->editOn();
	mapToolbox->getData();
}

void mapNameCallback(class Fl_Input *,void *)	// Initialise le champ nom
{
	int select=mapToolbox->getSelectedItem();
	if(!select) return;
	mainInterface->mapList->text(select,mainInterface->mapName->value());
}
