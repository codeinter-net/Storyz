#include <FL/Fl.H>
#include <FL/Fl_Draw.H>
#include "table.h"
#include "main.h"
#include "userFile.h"

#define ROW_BASE 100
#define ROW_STEP 100
#define DATA_BASE 65536
#define TEXT_BASE 4096

extern UserFile *userFile;

Table::Table(int x, int y, int w, int h, TableColDef *_colName, Data *_data) : Fl_Table(x,y,w,h,0)
{
	colName=_colName;
	data=_data;
	for(colNameSize=0; colNameSize<50; colNameSize++)
	{
		if(!colName[colNameSize].name) break;
	}

	rows(ROW_BASE);
	row_header(1);
	row_header_width(50);
	row_resize(1);
	row_height_all(50);

	cols(colNameSize);
	col_header(1);
	col_header_height(25);
	col_resize(1);
	for(int col=0; col<colNameSize; col++)
		col_width(col,colName[col].colSize);

	callback(&event_callback, (void*)this);
	end();

//	data=new TableData(colNameSize,ROW_BASE,ROW_STEP,0,DATA_BASE);

	editBuffer = new Fl_Text_Buffer(TEXT_BASE,TEXT_BASE);

	tableEditor = new Fl_Text_Editor(0,0,0,0);
	tableEditor->hide();
	tableEditor->callback(tableEditor_callback, (void*)this);
	tableEditor->when(FL_WHEN_CHANGED|FL_WHEN_NOT_CHANGED);
	tableEditor->buffer(editBuffer);
	tableEditor->scrollbar_width(0);
	tableEditor->clip_children(1);
	tableEditor->wrap_mode(tableEditor->WRAP_AT_BOUNDS,0);
	tableEditor->textsize(12);
	tableEditor->add_key_binding(FL_Left, FL_ALT, keyboardMove);
	tableEditor->add_key_binding(FL_Up, FL_ALT, keyboardMove);
	tableEditor->add_key_binding(FL_Right, FL_ALT, keyboardMove);
	tableEditor->add_key_binding(FL_Down, FL_ALT, keyboardMove);
}

void Table::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H)
{
	const char* cellData;
	static char text[40];
	switch(context)
	{
	case CONTEXT_STARTPAGE:
		fl_font(FL_HELVETICA, 12);
		return;
	case CONTEXT_COL_HEADER:
		if((C>=colNameSize)||!colName[C].name) return;
		fl_push_clip(X, Y, W, H);
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, col_header_color());
		fl_color(FL_BLACK);
		fl_draw(colName[C].name, X, Y, W, H, FL_ALIGN_CENTER);
		fl_pop_clip();
		return;
	case CONTEXT_ROW_HEADER:
		sprintf_s(text, sizeof(text), "%d", R+1);
		fl_push_clip(X, Y, W, H);
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
		fl_color(FL_BLACK);
		fl_draw(text, X, Y, W, H, FL_ALIGN_CENTER);
		fl_pop_clip();
		return;
	case CONTEXT_CELL:
 		fl_push_clip(X, Y, W, H);
		// BG COLOR
		fl_color(255);
		fl_rectf(X, Y, W, H);
		// TEXT
		fl_color(0);
//		cellData=_data->get(R, C);
// PROBLEME
// id correspond au numéro de ligne
// Alors que le numéro de ligne est dans l'attribut num
//		char row[16];
//		sprintf_s(row,sizeof(row),"line_%d",R);
//		cellData=data->getText(Data::TIMELINE, row, colName[C].id);
		cellData=data->getText(Data::TIMELINE, R, colName[C].id);

//		printf("%d %d\n",R,C);
		if(cellData)
			fl_draw(cellData, X+5, Y+3, W-9, H-3, FL_ALIGN_TOP_LEFT|FL_ALIGN_WRAP); // +5 +3 -9 -3 : Valeurs arbitraires pour faire cadrer le texte avec le champ éditable
		// BORDER
		fl_color(color()); 
		fl_rect(X, Y, W, H);
		fl_pop_clip();
		return;
	case CONTEXT_TABLE:
		return;
	case CONTEXT_ENDPAGE:
	case CONTEXT_RC_RESIZE:
	case CONTEXT_NONE:
		return;
	}
}

void Table::event_callback(Fl_Widget*, void *data)
{
	Table *o = (Table*)data;
	o->event_callback2();
}

void Table::event_callback2()
{
	int R = callback_row(), C = callback_col();
	TableContext context = callback_context();
//	printf("'%s' callback: ", (label() ? label() : "?"));
//	printf("Row=%d Col=%d Context=%d Event=%d InteractiveResize? %d\n", R, C, (int)context, (int)Fl::event(), (int)is_interactive_resize());

	switch (context)
	{
	case CONTEXT_CELL:
		{				// A table event occurred on a cell
			switch (Fl::event())
			{
			case FL_PUSH:	// Mouse click
				editEnd();
				editBegin(R,C,EditTo::END);
				return;
			case FL_KEYBOARD:
				editEnd();
//				if (C==cols()-1 || R==rows()-1) return;	// no editing of totals column
				editBegin(R,C,EditTo::END);
				return;
			}
			return;
		}
	case CONTEXT_TABLE:
	case CONTEXT_ROW_HEADER:
	case CONTEXT_COL_HEADER:
		editEnd();
		return;
	default:
		return;
	}
}

void Table::tableEditor_callback(Fl_Widget* widget,void* _table)
{
	Table* table = (Table*)_table;

	int fontHeight=fl_height(table->tableEditor->textfont(), table->tableEditor->textsize());
	int newHeight=(table->tableEditor->count_lines(0,262144,1)+1)*fontHeight+(fontHeight>>1);
	int currentHeight=table->row_height(table->editRow);

	if(newHeight>currentHeight)
		table->row_height(table->editRow,newHeight);

	int X,Y,W,H;
	table->find_cell(CONTEXT_CELL, table->editRow, table->editCol, X, Y, W, H);
	table->tableEditor->resize(X,Y,W,H);
}

void Table::editBegin(int R, int C,EditTo to)
{
	if(R<0) R=0;
	if(C<0) C=0;
//	if(R>=_data->getRows()) R=_data->getRows()-1;
//	if(C>=_data->getCols()) C=_data->getCols()-1;
	if(R>=rows()) R=rows()-1;
	if(C>=rows()) C=rows()-1;

	editRow=R;
	editCol=C;
	set_selection(R,C,R,C);
	move_cursor(R,C,0);
	int X,Y,W,H;
	find_cell(CONTEXT_CELL, R,C, X,Y,W,H);
//	printf("Cell xywh = %d %d %d %d | Table xywh = %d %d %d %d | ColH = %d | RowW = %d | ColPos = %d | RowPos = %d\n",X,Y,W,H,x(),y(),w(),h(),col_header_height(),row_header_width(),col_position(),row_position());

// TODO : forcer le scrolling pour replacer le champ éditable dans la fenêtre

//	const char* cellData=_data->get(editRow, editCol);
// PROBLEME
// id correspond au numéro de ligne
// Alors que le numéro de ligne est dans l'attribut num
//	char row[16];
//	sprintf_s(row,sizeof(row),"line_%d",editRow);
//	const char* cellData=data->getText(Data::TIMELINE, row, colName[editCol].id);
	const char* cellData=data->getText(Data::TIMELINE, editRow, colName[editCol].id);

	editBuffer->text(cellData?cellData:"");
	tableEditor->resize(X,Y,W,H);
	tableEditor->insert_position(to==EditTo::END?100000:0);
	tableEditor->redraw();
	tableEditor->show();
	tableEditor->take_focus();
	tableEditor->scroll(0,0);
	userFile->setChanged();
}

void Table::flush()
{
	if((editRow>=0)&&(editCol>=0))
	{
//		_data->set(editRow, editCol, editBuffer->text());
// PROBLEME
// id correspond au numéro de ligne
// Alors que le numéro de ligne est dans l'attribut num
//		char row[16];
//		sprintf_s(row,sizeof(row),"line_%d",editRow);
//		data->set(Data::TIMELINE, row, colName[editCol].id, editBuffer->text());
		data->set(Data::TIMELINE, editRow, colName[editCol].id, editBuffer->text());
	}
}

void Table::editEnd()
{
	if (tableEditor->visible())
	{
//		if((editRow>=0)&&(editCol>=0))
//		{
//			_data->set(editRow, editCol, editBuffer->text());
//		}
		flush();
		tableEditor->hide();
		window()->cursor(FL_CURSOR_DEFAULT);
	}
	editRow=editCol=-1;
}

