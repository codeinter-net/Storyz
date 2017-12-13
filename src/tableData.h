#pragma once

#include "rawData.h"

// Gestion d'un tabeau de données
// Fonctionne avec deux blocs mémoire : un pour le tableau et un pour les données
// Le nombre de colonnes est fixe et est déterminé à la création
// Le nombre de colonnes peut être augmenté par un incrément

class TableData
{
	struct tableCellStruct
	{
		int	index;	// Index vers les données
		int	size;	// Taille des données
		int	allocSize;	// Taille par défaut du bloc de texte
	} ;

private:
	tableCellStruct*	tableCell; // Le tableau de données
	rawData *data; // Les données du tableau

	int	cols;
	int	rows;	// Nombre de rangées actives
	int	rowsAlloc;	// Nombre de rangées allouées : rowsAlloc >= rows
	int	rowsStep;	// Incrément en cas d'augmentation

	inline tableCellStruct* getCell(int _row,int _col) {return(tableCell+((_row*cols+_col)));} // Retourne le pointeur sur la structure d'une cellule du tableau

public:

	TableData(int _cols, int _rows, int _rowsStep, int _dataSize, int _dataStep);
	~TableData();
	const char* get(int _row, int _col);
	int set(int _row, int _col, const char* _data);
	int getRows() {return rows;};
	int getCols() {return cols;};
};
