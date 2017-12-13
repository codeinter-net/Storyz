#pragma once
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Preferences.H>

class Pref : public Fl_Preferences
{
private :
	void saveWindowPos(char* name, Fl_Double_Window* window);
	void loadWindowPos(char* name, Fl_Double_Window* window);
public :
	Pref():Fl_Preferences(Fl_Preferences::USER,"CodeInter.net","Storyz"){};
	void save();
	void load();
};
