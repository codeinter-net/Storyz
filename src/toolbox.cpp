#include <FL/fl_ask.H>
#include "toolbox.h"
#include "locale.h"

// Gestion générique des fenêtres d'outillage

Toolbox::Toolbox(Data::Type _type, int size,Fl_Browser* _list, Data* _data)
{
	if(size<1) size=1;
	widgetListSize=size;
	widgetList=new WidgetList[size];
	list=_list;
	data=_data;
	type=_type;
}

Toolbox::~Toolbox()
{
	delete[] widgetList;
}

void Toolbox::setWidgetList(int index,Fl_Input* input,Fl_Text_Editor * textEditor,char* name,Fl_Text_Buffer* textBuffer) // Initialise les liens entre les widgets et les données
{
	widgetList[index].input=input;
	widgetList[index].textEditor=textEditor;
	widgetList[index].name=name;
	widgetList[index].textBuffer=textBuffer;
}

void Toolbox::getData()	// Renseigne les champs éditables à partir de l'arbre XML
{
	currentSelection=getSelectedItem();
	if(!currentSelection) return;
	const char* name=list->text(currentSelection);
	strncpy_s(currentName,sizeof(currentName),name,sizeof(currentName)-1);
	widgetList[0].input->value(name);

	// Lecture depuis l'arbre DOM XML
	int i=widgetListSize;
	for(WidgetList* widget=widgetList;--i>=0;widget++)
	{
		if(!widget->name) continue;
		if(widget->input)
			widget->input->value(data->getText(type,name,widget->name));
		if(widget->textEditor)
			widget->textBuffer->text(data->getText(type,name,widget->name));
	}
}

bool Toolbox::setData()	// Sauvegarde les champs éditables dans l'arbre XML
{
	if(!*currentName) return true;
	const char* newName=widgetList[0].input->value();
	if(!*newName)
	{
		list->select(currentSelection);
		fl_message(listItemNoBlank);
		return(false);
	}

	// Vérifie si le nom est en double
	for(int i=1;i<=list->size();i++)
		if((i!=currentSelection)&&!strcmp(list->text(i),list->text(currentSelection)))
		{
			list->select(currentSelection);
			fl_message(listItemDouble,list->text(currentSelection));
			return(false);
		}

	// Ecriture dans l'arbre DOM XML
	if(strcmp(newName,currentName)) // Le nom a été changé
		data->update(type,currentName,newName);
	int i=widgetListSize;
	for(WidgetList* widget=widgetList;--i>=0;widget++)
	{
		if(!widget->name) continue;
		if(widget->input)
			data->set(type,newName,widget->name,widget->input->value());
		if(widget->textEditor)
			data->set(type,newName,widget->name,widget->textBuffer->text());
	}
	strncpy_s(currentName,sizeof(currentName),newName,sizeof(currentName)-1);
	return true;
}

void Toolbox::editOn() // Active et vide les champs éditables
{
	int i=widgetListSize;
	for(WidgetList* widget=widgetList;--i>=0;widget++)
	{
		if(widget->input)
		{
			widget->input->activate();
			widget->input->value(0);
		}
		if(widget->textEditor)
		{
			widget->textEditor->activate();
			widget->textEditor->buffer(widget->textBuffer);
		}
		if(widget->textBuffer)
			widget->textBuffer->text(0);
	}
}

void Toolbox::editOff() // Désactive et vide les champs éditables
{
	int i=widgetListSize;
	for(WidgetList* widget=widgetList;--i>=0;widget++)
	{
		if(widget->input)
		{
			widget->input->deactivate();
			widget->input->value(0);
		}
		if(widget->textEditor)
			widget->textEditor->deactivate();
		if(widget->textBuffer)
			widget->textBuffer->text(0);
	}
	*currentName=0;
	currentSelection=0;
}

int Toolbox::getSelectedItem() // Retourne l'index de l'élément actif (base 1), zéro si non sélectionné
{
	for(int i=1;i<=list->size();i++)
		if(list->selected(i))
			return i;
	return 0;
}

void Toolbox::listNew(class Fl_Button *,void *) // Callback : ajoute un élément en fin de liste
{
char newName[32];

	for(int i=1;i<1000;i++)
	{
		sprintf_s(newName,sizeof(newName),listItemNew,i);	// TODO : paramétrage du nom du nouvel élément
		if(!list->size()) break; // Liste vide
		int j;
		for(j=1;j<=list->size();j++)
		{
			if(!strcmp(newName,list->text(j))) break; // Le nom existe déjà
		}
		if((j>list->size())||strcmp(newName,list->text(j))) break; // Le nom n'existe pas
	}
	list->add(newName);

	data->set(type,newName);
}

void Toolbox::listDelete(class Fl_Button *,void *) // Callback : efface un élément de la liste
{
	int select=getSelectedItem();
	if(!select) return;
	char text[256];
	sprintf_s(text,sizeof(text),listItemDelete,list->text(select));
	if(fl_choice(text,choiceNo,choiceYes,0)!=1) return;
	editOff();
	data->unset(type,list->text(select));
	list->remove(select);
}

void Toolbox::listUp(class Fl_Button *,void *) // Callback : monte un élément de la liste
{
	int select = getSelectedItem();
	if(select<=1) return;
	data->move(type, select-1, select);
	list->swap(select-1, select);
	list->redraw();
	list->select(select-1);
	currentSelection=getSelectedItem();
}

void Toolbox::listDown(class Fl_Button *,void *) // Callback : descend un élément de la liste
{
	int select = getSelectedItem();
	if(!select||(select==list->size())) return;
	data->move(type, select, select+1);
	list->swap(select, select+1);
	list->redraw();
	list->select(select+1);
	currentSelection=getSelectedItem();
}

void Toolbox::listInit() // Initialise la liste à partir de l'arbre XML
{
	if(!list||!data) return;
	list->clear();
	const char* text=data->getFirstIndex(type);
	while(text&&*text)
	{
		list->add(text);
		text=data->getNextIndex();
	}
	list->select(0);
}
