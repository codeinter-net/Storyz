#pragma once

#include "tinyxml2.h"

using namespace tinyxml2;

class Data
{
public:
	enum Type{TIMELINE=1, CHARACTER, PLACE, ACTION, NOTE, MAP};

	Data();
	void load(char *name);
	void save(char *name);
	void clear();
	void set(Type type, const char* id, const char* var=0, const char* val=0);
	void set(Type type, int id, const char* var=0, const char* val=0);
	void unset(Type type, const char* id, const char* var=0);
	void update(Type type, const char* oldName, const char* newName);
	void move(Type type, int fromIndex,int toIndex);
	void move(Type type, const char* fromName,const char* toName);
	const char* getText(Type type, const char* id, const char* var);
	const char* getText(Type type, int id, const char* var);
	const char* getFirstIndex(Type type);
	const char* getNextIndex();

private:
	tinyxml2::XMLDocument *xmlData;

	XMLNode* currentIndexNode;
	XMLElement* currentIndexElement;

	void init(void);
	XMLElement* getType(Type type);
	XMLElement* getItem(Type type, const char* Id);
};
