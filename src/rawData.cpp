#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rawData.h"

rawData::rawData(int size, int step): dataStep(step) // size = taille initiale; step = valeur d'incrément lors du realloc
{
	dataAlloc=size+step;
	if(!dataAlloc) return;
	data=(char*)malloc(dataAlloc);
	if(!data) return;
	memset(data,0,dataAlloc);
}

rawData::~rawData()
{
	if(data) free(data);
}

int rawData::create(int size) // Fournit un nouveau bloc de données
{
//	printf("rawData::create size=%d\n",size);
	if(dataSize+size>dataAlloc)
	{
		dataAlloc=dataSize+size+dataStep;
		data=(char*)realloc(data, dataAlloc);
		if(!data) return(-1);	// Mémoire pleine
	}
	int index=dataSize;
	dataSize+=size;
	return(index);
}

const char* rawData::get(int index)
{
//	printf("rawData::get index=%d\n",index);
	return(data+index+sizeof(rawDataHeader));
}

int rawData::set(int index, const char* _data, int size)	// Met à jour les données; retourne la nouvelle valeur de index; si index=-1 -> création
{
//	printf("rawData::set index=%d size=%d data=[%s]\n",index,size,_data);
	rawDataHeader* header=(rawDataHeader*)(data+index);
	int blockSize=header->size;
	int blockAlloc=header->alloc;

	if((index<0)||(size+(int)sizeof(rawDataHeader)>blockSize))	// création / extension
	{
		int newIndex=create(size+sizeof(rawDataHeader));
		if(newIndex<0) return(-1);	// Mémoire pleine -> Les données ne seront pas mises à jour
		index=newIndex;
	}

	header=(rawDataHeader*)(data+index);
	header->size=size;
	header->alloc=size;	// !!! absurde et inutile

	memcpy(data+index+sizeof(rawDataHeader), _data, size);
	return(index);
}

int rawData::alloc(int size)
{
	data=(char*)realloc(data,dataAlloc+size+dataStep);
	return(data?0:-1);
}

