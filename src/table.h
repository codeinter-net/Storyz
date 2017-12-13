#pragma once
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Text_Editor.H>
//#include "TableData.h"
#include "data.h"

class Table : public Fl_Table
{
public:

	enum EditTo
	{
		START,
		END
	};
	struct TableColDef
	{
		char	*id;
		char	*name;
		int	colSize;
		int	textSize;
	};

private:
	Fl_Text_Editor *tableEditor;
	Fl_Text_Buffer *editBuffer;

//	TableData *data;
	TableColDef *colName;
	int colNameSize;
	int editRow;
	int editCol;

protected:
	void draw_cell(TableContext context, int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0);
	static void event_callback(Fl_Widget*, void*);
	void event_callback2();				// callback for table events
	static void tableEditor_callback(Fl_Widget*,void* v);

public:
	Table(int x, int y, int w, int h, TableColDef *_colName, Data *_data);
	~Table() { };
//	TableData* getData() {return data;};
	char *getColId(int col) {return colName[col].id;};
	void editBegin(int R, int C,EditTo to);
	void editEnd();
	void flush();
	void getEditRowCol(int &R, int &C) {R=editRow; C=editCol;};
};
