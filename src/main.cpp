// --------------
// STORYZ
// PBA 2016-10-18
// --------------

#include <FL/Fl.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Shared_Image.H>
#include "table.h"
#include "UserFile.h"
#include "pref.h"
#include "locale.h"
#include "main.h"
#include "character.h"
#include "note.h"
#include "plan.h"
#include "map.h"

int version[4]={0,2,0,0};

MainInterface *mainInterface;
UserFile *userFile;
UserData *userData;
Data *data;
Pref *pref;

int keyboardMove(int c, Fl_Text_Editor* e)
{
	int row, col;
	Table::EditTo to;
	userData->timeline->getEditRowCol(row,col);
	userData->timeline->editEnd();
	switch(c)
	{
	case FL_Left :
		to=Table::EditTo::END;
		col--;
		break;
	case FL_Up :
		to=Table::EditTo::END;
		row--;
		break;
	case FL_Right :
		to=Table::EditTo::START;
		col++;
		break;
	case FL_Down :
		to=Table::EditTo::START;
		row++;
		break;
	}
	userData->timeline->editBegin(row,col,to);
	return 1;
}

/*
void setMainTitle()
{
	char text[256];
	char* fileName=userFile->getName();
	sprintf_s(text,sizeof(text),"STORYZ %s %s",(fileName&&*fileName)?fileName:"(sans nom)",userFile->isSaved()?"":" (*) ");
	mainInterface->mainWindow->label(text);
}
*/

/*
void setChanged()
{
	int saved = userFile->setSaved(false);
	if(saved) setMainTitle();
}
*/

/*
void doLoad(const char* filename)
{
	userFile->load(userData,filename);
	mainInterface->mainView->redraw();
	characterListInit();
	mainInterface->characterWindow->redraw();
	setMainTitle();
}
*/

void menuNew(Fl_Menu_*, void*)
{
	if(!userFile->isSaved())
	{
		if(fl_choice(fileNotSaved,choiceCancel,choiceContinue,0)!=1) return;
	}
	userData->timeline->editEnd();
	userData->timeline->flush();
	data->clear();
	userData->timeline->redraw();
	clearCharacterWindow();
	clearNoteWindow();
	clearPlanWindow();
	clearMapWindow();
	userFile->clear();
}

void menuOpen(Fl_Menu_*, void*)
{
	userData->timeline->editEnd();
	userData->timeline->flush();
	data->clear();

	Fl_Native_File_Chooser fileSelector;
	fileSelector.title(fileSelectorLoad);
	fileSelector.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fileSelector.filter(fileSelectorFilter);

	if(!fileSelector.show())
	{
		userFile->load(userData,fileSelector.filename());
	}
}

void menuSave(Fl_Menu_* menu, void* param)
{
	userData->timeline->editEnd();
	userData->timeline->flush();
	if(*userFile->getName())
	{
		userFile->save(userData);
		userFile->setMainTitle();
	}
	else
		menuSaveAs(menu, param);
}

void menuSaveAs(Fl_Menu_* menu, void* param)
{
	Fl_Native_File_Chooser fileSelector;
	fileSelector.title(fileSelectorSave);
	fileSelector.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fileSelector.filter(fileSelectorFilter);
	fileSelector.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM|Fl_Native_File_Chooser::NEW_FOLDER);	// TODO : SAVEAS_CONFIRM est inopérant -> tester l'existence du fichier
	if(*userFile->getName())
		fileSelector.preset_file(userFile->getName());

	if(!fileSelector.show())
	{
		userFile->save(userData, fileSelector.filename());
		userFile->setMainTitle();
	}
}

void menuQuit(Fl_Menu_* menu, void* param)
{
}

int main(int argc, char **argv)
{
	fl_register_images();
	Fl::visual(FL_RGB);
	pref = new Pref();
	mainInterface = new MainInterface();
	mainInterface->mainWindow->resizable(mainInterface->mainView);
	mainInterface->mainView->begin();
	data = new Data;
	userData = new UserData;
	userData->timeline = new Table(mainInterface->mainView->x(), mainInterface->mainView->y(), mainInterface->mainView->w(), mainInterface->mainView->h(), colNameList, data);
	mainInterface->mainView->end();
	mainInterface->mainWindow->show(argc, argv);
	userFile = new UserFile();
	pref->load();
	int retCode=Fl::run();
	pref->save();
	return retCode;
}

