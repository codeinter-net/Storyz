#pragma once
#include "StoryzInterface.h"
#include "table.h"
#include "data.h"

struct UserData	// Regroupe toutes les structures de données du projet;
{
	Table *timeline;
};

extern MainInterface *mainInterface;
extern Data *data;

//void setChanged();
int keyboardMove(int c, Fl_Text_Editor* e);
