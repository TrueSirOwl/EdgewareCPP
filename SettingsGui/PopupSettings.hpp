#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/Fl_Hor_Slider.H>
#include <FL/Fl_Check_Button.H>
#include "../shared/Settings.hpp"
#include <string>
#include <algorithm>

class PopupSettings : public Fl_Double_Window
{
private:

	Settings* SettingsFileContent;

public:
	int X, Y , W, H;

	//------------------------------------------------
	int Buttonx, Buttony, Buttonw, Buttonh;
	Fl_Value_Slider* ButtonXinput;
	Fl_Value_Slider* ButtonYinput;
	Fl_Input* ButtonTextInput;
	//------------------------------------------------
	int Fadex, Fadey, Fadew, Fadeh;
	Fl_Check_Button* PopupFadeOut;
	Fl_Hor_Slider* PopupFadeOutStepsSlider;
	Fl_Hor_Slider* PopupFadeOutTimeSlider;
	Fl_Value_Input* PopupFadeOutStepsInput;
	Fl_Value_Input* PopupFadeOutTimeInput;
	//------------------------------------------------
	int Opacityx, Opacityy, Opacityw, Opacityh;
	Fl_Hor_Slider* PopupOpacitySlider;
	Fl_Value_Input* PopupOpacityInput;
	Fl_Check_Button* Overlay;
	//------------------------------------------------
	int Lifespanx, Lifespany, Lifespanw, Lifespanh;
	Fl_Check_Button* PopupLifespan;
	Fl_Value_Input* PopupLifespanInput;
	Fl_Hor_Slider* PopupLifespanSlider;
	//------------------------------------------------
	int TimeBetweenx, TimeBetweeny, TimeBetweenw, TimeBetweenh;
	Fl_Check_Button* TimeBetweenPopups;
	Fl_Hor_Slider* TimeBetweenPopupsSlider;
	Fl_Value_Input* TimeBetweenPopupsInput;
	//------------------------------------------------
	int Multiplicatorx, Multiplicatory, Multiplicatorw, Multiplicatorh;
	Fl_Value_Input* MultiplicatiorInput;
	//------------------------------------------------
	int FolderPathx, FolderPathy, FolderPathw, FolderPathh;
	Fl_Input* ImageFolderPath;
	//------------------------------------------------
	int Burstx, Bursty;

	Fl_Hor_Slider* BurstAmountSlider;
	Fl_Value_Input* BurstAmountInput;

	static void ResizeButtonTextField(Fl_Widget* w, void* data);
	static void SetTimeBeteenPopups(Fl_Widget* w, void* data);
	static void SetTimeBetweenPopupsSlider(Fl_Widget* w, void* data);
	static void SetTimeBetweenPopupsInput(Fl_Widget* w, void* data);
	static void SetPopupLifespan(Fl_Widget* w, void* data);
	static void SetInputLifespan(Fl_Widget* w, void* data);
	static void SetSliderLifespan(Fl_Widget* w, void* data);
	static void ActivateFadeout(Fl_Widget* w, void* data);
	static void SetPopupFadeOutStepsInput(Fl_Widget* w, void* data);
	static void SetPopupFadeOutTimeInput(Fl_Widget* w, void* data);
	static void SetPopupFadeOutStepsSlider(Fl_Widget* w, void* data);
	static void SetPopupFadeOutTimeSlider(Fl_Widget* w, void* data);
	static void SetPopupOpacitySlider(Fl_Widget* w, void* data);
	static void SetPopupOpacityInput(Fl_Widget* w, void* data);
	static void SetBurstModeActivity(Fl_Widget* w, void* data);
	static void SetBurstAmountSlider(Fl_Widget* w, void* data);
	static void SetBurstAmountInput(Fl_Widget* w, void* data);

	PopupSettings(int x, int y, int w, int h, Settings* sett);
	~PopupSettings();
};

