#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TableData.h"

TableData::TableData(int _cols, int _rows, int _rowsStep, int _dataSize, int _dataStep): cols(_cols), rows(_rows),rowsStep(_rowsStep)
{
}

TableData::~TableData()
{
}

const char* TableData::get(int _row, int _col)	// Retourne le contenu d'une cellule - à recopier avant toute modification - retourne null si la cellule est vide
{
//	printf("TableData::get %d %d\n",_row,_col);
	tableCellStruct* cell=getCell(_row,_col);
//	printf("TableData::get index=%d size=%d allocSize=%d\n",cell->index,cell->size,cell->allocSize);
	if(!cell->size) return(0);
	return(data->get(cell->index));
}

int TableData::set(int _row, int _col, const char* _data) // Met à jour le contenu d'une cellule
{
//	printf("TableData::set %d %d [%s]\n",_row,_col,_data);
	tableCellStruct* cell=getCell(_row,_col);
	int dataSize=strlen(_data)+1;
	int newIndex=data->set(cell->size?cell->index:-1,_data,dataSize);
	if(newIndex<0) return(-1);
	cell->index=newIndex;
	cell->size=dataSize;
	return(0);
}


/*--

TableData::TableData(int _cols, int _rows, int _rowsStep, int _dataSize, int _dataStep): cols(_cols), rows(_rows),rowsStep(_rowsStep)
{
	// Création du tableau
	rowsAlloc=rows?rows:rowsStep;
	int tableSize=cols*rowsAlloc*sizeof(tableCellStruct);
	if(!tableSize) return;
	tableCell=(tableCellStruct*)malloc(tableSize);
	if(!tableCell) return;
	memset(tableCell,0,tableSize);
	data = new rawData(_dataSize,_dataStep);
}

TableData::~TableData()
{
	if(tableCell) free(tableCell);
}

const char* TableData::get(int _row, int _col)	// Retourne le contenu d'une cellule - à recopier avant toute modification - retourne null si la cellule est vide
{
//	printf("TableData::get %d %d\n",_row,_col);
	tableCellStruct* cell=getCell(_row,_col);
//	printf("TableData::get index=%d size=%d allocSize=%d\n",cell->index,cell->size,cell->allocSize);
	if(!cell->size) return(0);
	return(data->get(cell->index));
}

int TableData::set(int _row, int _col, const char* _data) // Met à jour le contenu d'une cellule
{
//	printf("TableData::set %d %d [%s]\n",_row,_col,_data);
	tableCellStruct* cell=getCell(_row,_col);
	int dataSize=strlen(_data)+1;
	int newIndex=data->set(cell->size?cell->index:-1,_data,dataSize);
	if(newIndex<0) return(-1);
	cell->index=newIndex;
	cell->size=dataSize;
	return(0);
}

--*/
