#pragma once

#include "rawData.h"

// Gestion d'un tabeau de donn�es
// Fonctionne avec deux blocs m�moire : un pour le tableau et un pour les donn�es
// Le nombre de colonnes est fixe et est d�termin� � la cr�ation
// Le nombre de colonnes peut �tre augment� par un incr�ment

class TableData
{
	struct tableCellStruct
	{
		int	index;	// Index vers les donn�es
		int	size;	// Taille des donn�es
		int	allocSize;	// Taille par d�faut du bloc de texte
	} ;

private:
	tableCellStruct*	tableCell; // Le tableau de donn�es
	rawData *data; // Les donn�es du tableau

	int	cols;
	int	rows;	// Nombre de rang�es actives
	int	rowsAlloc;	// Nombre de rang�es allou�es : rowsAlloc >= rows
	int	rowsStep;	// Incr�ment en cas d'augmentation

	inline tableCellStruct* getCell(int _row,int _col) {return(tableCell+((_row*cols+_col)));} // Retourne le pointeur sur la structure d'une cellule du tableau

public:

	TableData(int _cols, int _rows, int _rowsStep, int _dataSize, int _dataStep);
	~TableData();
	const char* get(int _row, int _col);
	int set(int _row, int _col, const char* _data);
	int getRows() {return rows;};
	int getCols() {return cols;};
};
