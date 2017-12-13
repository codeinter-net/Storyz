#include "data.h"

// Gestion des donn�es
// R�alise l'interface entre les actions utilisateur et les donn�es en m�moire et sur disque
// Permet de s'adapter � une autre librairie XML si n�cessaire
// Les donn�es sont stock�es en m�moire dans un arbre XML ayant la m�me structure que le fichier disque
//
// L'arbre XML est structur� sur 4 niveaux :
// 1 : <storyz> : racine du document
// 2 : type: <timeline>|<character>|<place>|<action>|<note>|<map> : cat�gorie de donn�es
// 3 : <item id> : identifiant/index dans la liste des donn�es : num�ro de ligne ou nom
// 4 : variable : pr�d�finie en fonction du type

// BUG : improssible d'�diter apr�s un CTRL+N

Data::Data()
{
	xmlData=new XMLDocument;
	init();
};

void Data::init(void) // Cr�e l'arborescence de base de l'arbre XML
{
	XMLDeclaration* declaration = xmlData->NewDeclaration(0);
	xmlData->LinkEndChild(declaration);
	XMLElement* storyz=xmlData->NewElement("storyz");
	xmlData->LinkEndChild(storyz);
	XMLElement* timeline = xmlData->NewElement("timeline");
	storyz->LinkEndChild(timeline);
	XMLElement* character = xmlData->NewElement("character");
	storyz->LinkEndChild(character);
	XMLElement* place = xmlData->NewElement("place");
	storyz->LinkEndChild(place);
	XMLElement* action = xmlData->NewElement("action");
	storyz->LinkEndChild(action);
	XMLElement* note = xmlData->NewElement("note");
	storyz->LinkEndChild(note);
	XMLElement* map = xmlData->NewElement("map");
	storyz->LinkEndChild(map);
}

XMLElement* Data::getType(Type type) // Retourne la branche correspondant � un type
{
	XMLElement* storyz=xmlData->FirstChildElement("storyz");
	if(!storyz) return 0;
	switch(type)
	{
		case TIMELINE : return storyz->FirstChildElement("timeline");
		case CHARACTER : return storyz->FirstChildElement("character");
		case PLACE : return storyz->FirstChildElement("place");
		case ACTION : return storyz->FirstChildElement("action");
		case NOTE : return storyz->FirstChildElement("note");
		case MAP : return storyz->FirstChildElement("map");
	}
	return 0;
}

XMLElement* Data::getItem(Type type, const char* id) // Retourne l'item correspondant � une branche et un id
{
	XMLElement* branch = getType(type);
	if(!branch||!id) return 0;
	XMLElement* object = branch->FirstChildElement("item");
	while(object)
	{
		const char* value = object->Attribute("id");
		if(value&&!strcmp(value,id)) return object;
		object = object->NextSiblingElement();
	}
	return 0;
}

void Data::load(char *name) // Charge les donn�es depuis le disque
{
	xmlData->LoadFile(name);
}

void Data::save(char *name) // Enregistre les donn�es sur disque
{
//	xmlData->SetBOM(true);
	xmlData->SaveFile(name);
}

void Data::clear() // Efface toutes les donn�es stock�es en m�moire
{
	xmlData->Clear();
	init();
}

void Data::set(Type type, const char* id, const char* var, const char* val) // Cr�e ou met � jour un �l�ment
{
	XMLElement* branch = getType(type);
	if(!branch) return;
	XMLElement* object = getItem(type,id);
	if(!object)	// item absent
	{
		object = xmlData->NewElement("item");
		branch->LinkEndChild(object);
		object->SetAttribute("id",id);
	}
	if(!object||!var) return;
	XMLElement* node = object->FirstChildElement(var);
	if(!node) // objet absent
	{
		node = xmlData->NewElement(var);
		object->LinkEndChild(node);
	}
	if(!node||!val) return;
	node->SetText(val);
}

void Data::set(Type type, int id, const char* var, const char* val) // Cr�e ou met � jour un �l�ment
{
	char textId[16];
	sprintf_s(textId,sizeof(textId),"%d",id);
	set(type,textId,var,val);
}

void Data::unset(Type type, const char* id, const char* var) // Retire un �l�ment
{
	XMLElement* branch = getType(type);
	if(!branch) return;
	XMLElement* object = getItem(type,id);
	if(!object) return;
	if(!var)
	{
		branch->DeleteChild(object);
	}
	else
	{
		XMLElement* node = object->FirstChildElement(var);
		if(!node) return;
		object->DeleteChild(node);
	}
}

// BUG : ne fonctionne pas

void Data::update(Type type, const char* oldId, const char* newId) // Met � jour un identifiant d'item
{
	if(!oldId||!newId) return;
	XMLElement* object = getItem(type,oldId);
	if(!object) return;
	object->SetAttribute("id",newId); // non
}

void Data::move(Type type, int fromIndex, int toIndex)	// D�place un �l�ment dans l'arbre XML
{
	XMLElement* branch = getType(type);
	if(!branch) return;
	XMLElement* fromObject = 0;
	XMLElement* toObject = 0;
	XMLElement*	object = branch->FirstChildElement();
	for(int i=1; object; i++)
	{
		if(i==fromIndex) fromObject=object;
		if(i==toIndex) toObject=object;
		if(fromObject&&toObject)
		{
			branch->InsertAfterChild(toObject,fromObject);
			break;
		}
		object = object->NextSiblingElement();
	}
}

void Data::move(Type type, const char * fromId, const char * toId)	// D�place un �l�ment dans l'arbre XML
{
	XMLElement* branch = getType(type);
	if(!branch||!fromId||!toId) return;
	XMLElement* fromObject = getItem(type,fromId);
	XMLElement* toObject = getItem(type,toId);
	if(!fromObject||!toObject) return;
	branch->InsertAfterChild(toObject,fromObject);
}

const char* Data::getText(Type type, const char* id, const char* var) // Lit la valeur d'un �lement
{
	XMLElement* object = getItem(type,id);
	if(!object||!var) return 0;
	XMLElement* node = object->FirstChildElement(var);
	if(!node) return 0;
	return node->GetText();
}

const char* Data::getText(Type type, int id, const char* var) // Lit la valeur d'un �lement
{
	char textId[16];
	sprintf_s(textId,sizeof(textId),"%d",id);
	return getText(type,textId,var);
}

const char* Data::getFirstIndex(Type type) // Retourne l'identifiant du premier item du type demand� -> permet de cr�er les listes d'items
{
	XMLElement* branch = getType(type);
	if(!branch) return 0;
	currentIndexElement=branch->FirstChildElement();
	currentIndexNode=branch->FirstChild();
	if(!currentIndexNode) return 0;
	return currentIndexNode->ToElement()->Attribute("id");
}

const char* Data::getNextIndex() // Retourne l'identifiant de l'item suivant
{
	if(!currentIndexNode) return 0;
	currentIndexNode=currentIndexNode->NextSibling();
	if(!currentIndexNode) return 0;
	return currentIndexNode->ToElement()->Attribute("id");
}
