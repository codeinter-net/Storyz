#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Shared_Image.H>
#include "StoryzInterface.h"
#include "main.h"
#include "data.h"
#include "toolbox.h"
#include "locale.h"
#include "plan.h"

// Fênètre de gestion des plans

Fl_Text_Buffer planTextBuffer;
Toolbox *planToolbox;
Fl_Shared_Image *planImage;
Fl_Image *planDisplay;

void planWindowCallback(Fl_Widget *widget, void *) // Appelé lors de la fermeture de la fenêtre
{
	Fl_Window *window = (Fl_Window *)widget;
	if(!planToolbox->setData()) return;
	window->hide();
}

void planToolboxInit()
{
	planToolbox=new Toolbox(Data::NOTE,2,mainInterface->planList,data);
	planToolbox->setWidgetList(0,mainInterface->planName,0,0,0);
	planToolbox->setWidgetList(1,0,mainInterface->planText,"text",&planTextBuffer);
}

void planListNew(class Fl_Button *b,void *e)
{
	planToolbox->listNew(b,e);
}

void planListDelete(class Fl_Button *b,void *e)
{
	planToolbox->listDelete(b,e);
}

void planListUp(class Fl_Button *b,void *e)
{
	planToolbox->listUp(b,e);
}

void planListDown(class Fl_Button *b,void *e)
{
	planToolbox->listDown(b,e);
}

void planListInit()
{
	if(!planToolbox) planToolboxInit();
	planToolbox->listInit();
}

void openPlanWindow() // Ouverture de la fenêtre + initialisations
{
	if(!planToolbox) planToolboxInit();
	mainInterface->planWindow->show();
	mainInterface->planWindow->callback(planWindowCallback);
	planToolbox->editOff();
	planListInit();
}

void clearPlanWindow()
{
	if(!planToolbox) return;
	planToolbox->editOff();
	planToolbox->listInit();
}

void menuOpenPlan(Fl_Menu_* , void*)
{
	openPlanWindow();
}

void toolbarOpenPlan(class Fl_Button *,void *)
{
	openPlanWindow();
}

void planListCallback(class Fl_Browser *list,void *data)
{
	if(!planToolbox->setData()) return;
	int select=planToolbox->getSelectedItem();
	if(!select)
	{
		planToolbox->editOff();
		return;
	}
	planToolbox->editOn();
	planToolbox->getData();
}

void planNameCallback(class Fl_Input *,void *)	// Initialise le champ nom
{
	int select=planToolbox->getSelectedItem();
	if(!select) return;
	mainInterface->planList->text(select,mainInterface->planName->value());
}

void planImageZoomIn(class Fl_Button *,void *)
{
}

void planImageZoomOut(class Fl_Button *,void *)
{
}

void planImageFull(class Fl_Button *,void *)
{
}

void planImageClear(class Fl_Button *,void *)
{
}

void planImageLoad(class Fl_Button *,void *)
{
	Fl_Native_File_Chooser fileSelector;
	fileSelector.title(fileSelectorImageLoad);
	fileSelector.type(Fl_Native_File_Chooser::BROWSE_FILE);
	fileSelector.filter(fileSelectorImageFilter);

	if(!fileSelector.show())
	{
		const char* imageName = fileSelector.filename();
		if(fl_filename_isdir(imageName)) return;
		if(planImage) planImage->release();
		planImage = Fl_Shared_Image::get(imageName);
		if(planImage)
		{
			mainInterface->planImageBox->image(planImage);
			mainInterface->planImageBox->size(planImage->w(),planImage->h());
			mainInterface->planImageBox->redraw();
		}
	}
}
