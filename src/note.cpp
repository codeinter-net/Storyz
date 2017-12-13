#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include "StoryzInterface.h"
#include "main.h"
#include "data.h"
#include "toolbox.h"

// Fênètre de gestion des notes

Fl_Text_Buffer noteTextBuffer;

Toolbox *noteToolbox;

void noteWindowCallback(Fl_Widget *widget, void *) // Appelé lors de la fermeture de la fenêtre
{
	Fl_Window *window = (Fl_Window *)widget;
	if(!noteToolbox->setData()) return;
	window->hide();
}

void noteToolboxInit()
{
	noteToolbox=new Toolbox(Data::NOTE,2,mainInterface->noteList,data);
	noteToolbox->setWidgetList(0,mainInterface->noteName,0,0,0);
	noteToolbox->setWidgetList(1,0,mainInterface->noteText,"text",&noteTextBuffer);
}

void noteListNew(class Fl_Button *b,void *e)
{
	noteToolbox->listNew(b,e);
}

void noteListDelete(class Fl_Button *b,void *e)
{
	noteToolbox->listDelete(b,e);
}

void noteListUp(class Fl_Button *b,void *e)
{
	noteToolbox->listUp(b,e);
}

void noteListDown(class Fl_Button *b,void *e)
{
	noteToolbox->listDown(b,e);
}

void noteListInit()
{
	if(!noteToolbox) noteToolboxInit();
	noteToolbox->listInit();
}

void openNoteWindow() // Ouverture de la fenêtre + initialisations
{
	if(!noteToolbox) noteToolboxInit();
	mainInterface->noteWindow->show();
	mainInterface->noteWindow->callback(noteWindowCallback);
	noteToolbox->editOff();
	noteListInit();
}

void clearNoteWindow()
{
	if(!noteToolbox) return;
	noteToolbox->editOff();
	noteToolbox->listInit();
}

void menuOpenNote(Fl_Menu_* , void*)
{
	openNoteWindow();
}

void toolbarOpenNote(class Fl_Button *,void *)
{
	openNoteWindow();
}

void noteListCallback(class Fl_Browser *list,void *data)
{
	if(!noteToolbox->setData()) return;
	int select=noteToolbox->getSelectedItem();
	if(!select)
	{
		noteToolbox->editOff();
		return;
	}
	noteToolbox->editOn();
	noteToolbox->getData();
}

void noteNameCallback(class Fl_Input *,void *)	// Initialise le champ nom
{
	int select=noteToolbox->getSelectedItem();
	if(!select) return;
	mainInterface->noteList->text(select,mainInterface->noteName->value());
}
