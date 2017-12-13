#include <string.h>
#include "UserFile.h"
#include "tinyxml2.h"

// TODO : effectuer le merge de userFile et data dans une seule classe -> ou pas ?

void UserFile::setMainTitle()
{
	char text[256];
	char* fileName=getName();
	sprintf_s(text,sizeof(text),"STORYZ %s %s",(fileName&&*fileName)?fileName:"(sans nom)",isSaved()?"":" (*) ");
	mainInterface->mainWindow->label(text);
}

void UserFile::setChanged()
{
	int saved = setSaved(false);
	if(saved) setMainTitle();
}

void UserFile::clear()
{
	*name=0;
	saved=true;
	setMainTitle();
}

void UserFile::load(UserData *userData, const char* _name)
{
	if(!*_name) return;
	if(strlen(_name)>sizeof(name)+1) return; // Buffer trop petit
//	syzLoad(data, _name);
	strcpy_s(name,sizeof(name),_name);
	data->load(name);
	saved=true;

	mainInterface->mainView->redraw(); // TODO : à placer ailleurs
//	characterListInit();
	mainInterface->characterWindow->redraw(); // TODO : à placer ailleurs
	setMainTitle();
}

void UserFile::save(UserData *userData, const char* _name)
{
	if(_name&&*_name)
	{
		if(strlen(_name)>sizeof(name)+5) return; // Buffer trop petit (4 en plus pour ajouter l'extension si nécessaire)
		strcpy_s(name,sizeof(name),_name);
	}
	if(!*name) return;
	char *ext=strrchr(name,'.');
	if(!ext||stricmp(ext+1,"syz")) // Ajoute l'extension par défaut si elle est absente ou différente
		strcat_s(name,sizeof(name),".syz");
//	syzSave(data, name);
	data->save(name);

	saved=true;
}
