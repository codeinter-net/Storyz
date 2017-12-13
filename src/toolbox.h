#pragma once
#include <FL/Fl_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include "data.h"

class Toolbox
{
public:
	struct WidgetList // Liste des objets éditables, L'objet d'index zéro est celui qui correspond à la sélection en cours
	{
		Fl_Input* input;
		Fl_Text_Editor * textEditor;
		char* name;
		Fl_Text_Buffer* textBuffer;
	};

	Toolbox(Data::Type _type, int size,Fl_Browser* _list, Data* _data);
	~Toolbox();
	void setWidgetList(int index,Fl_Input* input,Fl_Text_Editor * textEditor,char* name,Fl_Text_Buffer* textBuffer);
	void listNew(class Fl_Button *,void *);
	void listDelete(class Fl_Button *,void *);
	void listUp(class Fl_Button *,void *);
	void listDown(class Fl_Button *,void *);
	void listInit();
	void editOn();
	void editOff();
	void getData();
	bool setData();
	int getSelectedItem();

private:
	Data::Type type;
	WidgetList* widgetList;
	int widgetListSize;
	Data *data;
	Fl_Browser *list;
	int currentSelection;
	char currentName[256];
};
