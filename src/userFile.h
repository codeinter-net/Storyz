#pragma once
#include <stdio.h>
#include "main.h"

class UserFile
{
private:
	char name[1024]; // TODO : taille dynamique
	bool saved;

//	int syzLoad(UserData *userData, const char* name);
//	int syzSave(UserData *userData, const char* name);

public:
	UserFile::UserFile() {*name=0; saved=true;};
	void load(UserData *userData, const char* _name);
	void save(UserData *userData, const char* _name = 0);
	void clear();
	bool isSaved() {return saved;};
	bool setSaved(bool _saved=true) {bool oldSaved=saved; saved=_saved; return oldSaved;};
	char *getName() {return name;};
	void setMainTitle();
	void setChanged();
};
