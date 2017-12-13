#include <stdlib.h>
#include "pref.h"
#include "userFile.h"

extern UserFile *userFile;
extern UserData *userData;

void Pref::saveWindowPos(char* name, Fl_Double_Window* window)
{
	char param[256];
	sprintf_s(param,sizeof(param),"%d;%d;%d;%d;%d",window->x(),window->y(),window->w(),window->h(),window->visible());
	set(name, param);
}

void Pref::loadWindowPos(char* name, Fl_Double_Window* window)
{
	char param[256];
	if(get(name, param,"",sizeof(param)-1))
	{
		int x,y,w,h,v;
		sscanf_s(param,"%d;%d;%d;%d;%d",&x,&y,&w,&h,&v);
		window->resize(x,y,w,h);
		if(v) window->show();
	}
}

void Pref::save()	// Sauve l'environnement 
{
	set("filename", userFile->getName());
	saveWindowPos("window_main",mainInterface->mainWindow);
	saveWindowPos("window_character",mainInterface->characterWindow);
	flush();
}

void Pref::load()	// Restaure l'environnement de travail
{
	char* filename=0;
	if(get("filename", filename, ""))
		userFile->load(userData,filename);
	if(filename) free(filename);
	userFile->setMainTitle();
	loadWindowPos("window_main",mainInterface->mainWindow);
	loadWindowPos("window_character",mainInterface->characterWindow);
}
