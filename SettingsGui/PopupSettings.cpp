#include "PopupSettings.hpp"
#include <iostream>
#include <cmath>

PopupSettings::PopupSettings(int x, int y, int w, int h, Settings* sett) : Fl_Double_Window(x, y, w, h),X(x),Y(y),W(w),H(h), SettingsFileContent(sett) {
	box(FL_BORDER_BOX);

	//this->SettingsFileContent = ReadSettings();
	//------------------------------------------------
	this->ButtonXinput = new Fl_Value_Slider(w-50, h- this->SettingsFileContent->MaxYButtonHeight -120, 40, 100, "ButtonX");
	this->ButtonXinput->value(this->SettingsFileContent->ButtonX);
	this->ButtonXinput->maximum(this->SettingsFileContent->MinXButtonHeight);
	this->ButtonXinput->minimum(this->SettingsFileContent->MaxXButtonHeight);
	this->ButtonXinput->step(1);
	this->ButtonXinput->tooltip("Sets the X extent of the Button in the bottom right Corner");
	this->ButtonXinput->callback(ResizeButtonTextField, this);

	this->ButtonYinput = new Fl_Value_Slider(w-100, h- this->SettingsFileContent->MaxYButtonHeight -120, 40, 100, "ButtonY");
	this->ButtonYinput->value(this->SettingsFileContent->ButtonY);
	this->ButtonYinput->maximum(this->SettingsFileContent->MinYButtonHeight);
	this->ButtonYinput->minimum(this->SettingsFileContent->MaxYButtonHeight);
	this->ButtonYinput->step(1);
	this->ButtonYinput->tooltip("Sets the Y extent of the Button in the bottom right Corner");
	this->ButtonYinput->callback(ResizeButtonTextField, this);
	
	this->ButtonTextInput = new Fl_Input(w - this->SettingsFileContent->ButtonX, h- this->SettingsFileContent->ButtonY, this->SettingsFileContent->ButtonX, this->SettingsFileContent->ButtonY);
	this->ButtonTextInput->value(this->SettingsFileContent->ButtonText.c_str());
	this->ButtonTextInput->tooltip("Sets the Text on the Button in the bottom right Corner");
	//------------------------------------------------

	this->Fadex = 150;
	this->Fadey = 10;
	this->Fadew = 300;
	this->Fadeh = 60;

	this->PopupFadeOut = new Fl_Check_Button(Fadex, Fadey, 20, 20, "Fade Out");
	this->PopupFadeOut->align(FL_ALIGN_LEFT);
	this->PopupFadeOut->value(this->SettingsFileContent->PopupFadeOut);
	this->PopupFadeOut->tooltip("Enables Popup Fadeout after Lifetime runs out");
	this->PopupFadeOut->callback(ActivateFadeout, this);

	this->PopupFadeOutStepsSlider = new Fl_Hor_Slider(Fadex, Fadey+20, 220, 20, "Steps");
	this->PopupFadeOutStepsSlider->align(FL_ALIGN_LEFT);
	this->PopupFadeOutStepsSlider->value(this->SettingsFileContent->PopupFadeOutSteps);
	this->PopupFadeOutStepsSlider->tooltip("Sets the amount of Steps the Fade out takes to dissapear (one step takes one time");
	this->PopupFadeOutStepsSlider->step(1);
	this->PopupFadeOutStepsSlider->maximum(5000);
	this->PopupFadeOutStepsSlider->minimum(3);
	this->PopupFadeOutStepsSlider->callback(SetPopupFadeOutStepsInput, this);

	this->PopupFadeOutStepsInput = new Fl_Value_Input(Fadex+220, Fadey+20,50,20);
	this->PopupFadeOutStepsInput->value(this->SettingsFileContent->PopupFadeOutSteps);
	this->PopupFadeOutStepsInput->callback(SetPopupFadeOutStepsSlider, this);

	this->PopupFadeOutTimeSlider = new Fl_Hor_Slider(Fadex, Fadey+40, 220, 20, "Time");
	this->PopupFadeOutTimeSlider->align(FL_ALIGN_LEFT);
	this->PopupFadeOutTimeSlider->value(this->SettingsFileContent->PopupFadeOutTime);
	this->PopupFadeOutTimeSlider->tooltip("Sets the time the Fade out takes");
	this->PopupFadeOutTimeSlider->step(1);
	this->PopupFadeOutTimeSlider->maximum(10000);
	this->PopupFadeOutTimeSlider->minimum(3);
	this->PopupFadeOutTimeSlider->callback(SetPopupFadeOutTimeInput,this);

	this->PopupFadeOutTimeInput = new Fl_Value_Input(Fadex + 220, Fadey + 40, 50, 20);
	this->PopupFadeOutTimeInput->value(this->SettingsFileContent->PopupFadeOutTime);
	this->PopupFadeOutTimeInput->callback(SetPopupFadeOutTimeSlider,this);

	if (this->PopupFadeOut->value() == 0) {
		this->PopupFadeOutTimeSlider->deactivate();
		this->PopupFadeOutStepsSlider->deactivate();
		this->PopupFadeOutStepsInput->deactivate();
		this->PopupFadeOutTimeInput->deactivate();
	}
	//------------------------------------------------
	Opacityx = 150;
	Opacityy = 90;
	Opacityw = 0;
	Opacityh = 0;

	this->PopupOpacitySlider = new Fl_Hor_Slider(Opacityx, Opacityy,220,20,"Opacity");
	this->PopupOpacitySlider->value(this->SettingsFileContent->PopupOpacity);
	this->PopupOpacitySlider->align(FL_ALIGN_LEFT);
	this->PopupOpacitySlider->maximum(1);
	this->PopupOpacitySlider->minimum(0);
	this->PopupOpacitySlider->step(0.01);
	this->PopupOpacitySlider->tooltip("Sets the Opactity of the Popups");
	this->PopupOpacitySlider->callback(SetPopupOpacityInput,this);
	this->PopupOpacityInput = new Fl_Value_Input(Opacityx + 220, Opacityy, 50, 20);
	this->PopupOpacityInput->value(this->SettingsFileContent->PopupOpacity);
	this->PopupOpacityInput->callback(SetPopupOpacitySlider, this);
	
	this->Overlay = new Fl_Check_Button(Opacityx, Opacityy + 20, 20, 20,"Overlay");
	this->Overlay->value(this->SettingsFileContent->Overlay);
	this->Overlay->align(FL_ALIGN_LEFT);
	this->Overlay->tooltip("Makes Popups Overlays");

	//this->PopupOpacityInput = new Fl_Value_Input(Opacityx);

	//------------------------------------------------

	Lifespanx = 150;
	Lifespany = 70;
	Lifespanw = 0;
	Lifespanh = 0;

	this->PopupLifespanSlider = new Fl_Hor_Slider(Lifespanx+20, Lifespany, 200, 20);
	this->PopupLifespanSlider->minimum(0);
	this->PopupLifespanSlider->maximum(10000);
	this->PopupLifespanSlider->step(1);
	this->PopupLifespanSlider->value(this->SettingsFileContent->PopupLifespan);
	this->PopupLifespanSlider->callback(SetInputLifespan,this);

	this->PopupLifespanInput = new Fl_Value_Input(Lifespanx+220, Lifespany, 50, 20);
	this->PopupLifespanInput->value(this->SettingsFileContent->PopupLifespan);
	this->PopupLifespanInput->tooltip("Sets how long each Popup exists untill it begins to dissapear");
	this->PopupLifespanInput->callback(SetSliderLifespan, this);

	this->PopupLifespan = new Fl_Check_Button(Lifespanx, Lifespany, 20, 20, "Popup Lifespan");
	this->PopupLifespan->value(1);
	if (this->SettingsFileContent->PopupLifespan < 0) {
		this->PopupLifespan->value(0);
		this->PopupLifespanInput->deactivate();
		this->PopupLifespanSlider->deactivate();
	}
	this->PopupLifespan->align(FL_ALIGN_LEFT);
	this->PopupLifespan->callback(SetPopupLifespan, this);
	this->PopupLifespan->tooltip("Sets wether Popups have a lifespan, after which they dissapear.\n(Disabling this may cause the program to crash due to running out of memory!)");
	//------------------------------------------------

	FolderPathx = 150;
	FolderPathy = 170;
	FolderPathw = 0;
	FolderPathh = 0;

	this->ImageFolderPath = new Fl_Input(FolderPathx, FolderPathy, 700, 20, "Image Folder Path");
	this->ImageFolderPath->value(this->SettingsFileContent->ImageFolderPath.c_str());
	this->ImageFolderPath->tooltip("this is the path the program will get all its images from");
	//------------------------------------------------

	TimeBetweenx = 150;
	TimeBetweeny = 150;
	TimeBetweenw = 0;
	TimeBetweenh = 0;

	this->TimeBetweenPopupsInput = new Fl_Value_Input(TimeBetweenx+220, TimeBetweeny, 50, 20);
	this->TimeBetweenPopupsInput->value(this->SettingsFileContent->TimeBetweenPopups);
	this->TimeBetweenPopupsInput->tooltip("Sets the time between Popups, Min time is always image loading time");
	this->TimeBetweenPopupsInput->callback(SetTimeBetweenPopupsSlider, this);

	this->TimeBetweenPopupsSlider = new Fl_Hor_Slider(TimeBetweenx+20, TimeBetweeny, 200, 20);
	this->TimeBetweenPopupsSlider->minimum(0);
	this->TimeBetweenPopupsSlider->maximum(10000);
	this->TimeBetweenPopupsSlider->step(1);
	this->TimeBetweenPopupsSlider->value(this->SettingsFileContent->TimeBetweenPopups);
	this->TimeBetweenPopupsSlider->tooltip("Sets the time between Popups, Min time is always image loading time");
	this->TimeBetweenPopupsSlider->callback(SetTimeBetweenPopupsInput, this);

	this->TimeBetweenPopups = new Fl_Check_Button(TimeBetweenx, TimeBetweeny, 20, 20, "TimeBetweenPopups");
	this->TimeBetweenPopups->align(FL_ALIGN_LEFT);
	this->TimeBetweenPopups->tooltip("Sets the time between Popups, Min time is always image loading time");
	this->TimeBetweenPopups->callback(SetTimeBeteenPopups, this);
	this->TimeBetweenPopups->value(1);
	if (this->SettingsFileContent->TimeBetweenPopups < 0) {
		this->TimeBetweenPopups->value(0);
		this->TimeBetweenPopupsInput->deactivate();
		this->TimeBetweenPopupsSlider->deactivate();
	}
	//------------------------------------------------

	Burstx = 150;
	Bursty = 130;

	this->BurstAmountSlider = new Fl_Hor_Slider(Burstx, Bursty, 220, 20, "Multipop");
	this->BurstAmountSlider->align(FL_ALIGN_LEFT);
	this->BurstAmountSlider->value(this->SettingsFileContent->BurstAmt);
	this->BurstAmountSlider->bounds(1, 100);
	this->BurstAmountSlider->step(1);
	this->BurstAmountSlider->tooltip("Sets Amount of Popups per activation");
	this->BurstAmountSlider->callback(SetBurstAmountInput, this);

	this->BurstAmountInput = new Fl_Value_Input(Burstx + 220, Bursty, 50, 20 );
	this->BurstAmountInput->value(this->SettingsFileContent->BurstAmt);
	this->BurstAmountInput->tooltip("Sets Amount of Popups per activation");
	this->BurstAmountInput->callback(SetBurstAmountSlider, this);
	//------------------------------------------------

	Multiplicatorx = 150;
	Multiplicatory = 300;
	Multiplicatorw = 0;
	Multiplicatorh = 0;

	this->MultiplicatiorInput = new Fl_Value_Input(Multiplicatorx, Multiplicatory, 50, 20, "Multiplication");
	this->MultiplicatiorInput->value(this->SettingsFileContent->Multiplicator);
	this->MultiplicatiorInput->tooltip("Determines how many Popus will be created when closing a Popup");

	hide();
}

void PopupSettings::ResizeButtonTextField(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->ButtonTextInput->resize(Gui->W - Gui->ButtonXinput->value(), Gui->H - Gui->ButtonYinput->value(), Gui->ButtonXinput->value(), Gui->ButtonYinput->value());
	std::cout << (int)Gui->TimeBetweenPopups->value();
	Gui->redraw();
}

void PopupSettings::SetTimeBeteenPopups(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	if ((int)Gui->TimeBetweenPopups->value() == 1) {
		Gui->TimeBetweenPopupsInput->activate();
		Gui->TimeBetweenPopupsSlider->activate();
		Gui->TimeBetweenPopupsInput->value(Gui->TimeBetweenPopupsSlider->value());
	}
	else {
		Gui->TimeBetweenPopupsInput->value(-1);
		Gui->TimeBetweenPopupsInput->deactivate();
		Gui->TimeBetweenPopupsSlider->deactivate();
	}
}

void PopupSettings::SetPopupLifespan(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	if ((int)Gui->PopupLifespan->value() == 1) {
		Gui->PopupLifespanInput->activate();
		Gui->PopupLifespanSlider->activate();
		Gui->PopupLifespanInput->value(Gui->PopupLifespanSlider->value());
	}
	else {
		Gui->PopupLifespanInput->value(-1);
		Gui->PopupLifespanInput->deactivate();
		Gui->PopupLifespanSlider->deactivate();
	}
}

void PopupSettings::SetTimeBetweenPopupsSlider(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->TimeBetweenPopupsSlider->value(Gui->TimeBetweenPopupsInput->value());
}

void PopupSettings::SetTimeBetweenPopupsInput(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->TimeBetweenPopupsInput->value(Gui->TimeBetweenPopupsSlider->value());
}

void PopupSettings::SetInputLifespan(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupLifespanInput->value(Gui->PopupLifespanSlider->value());
}

void PopupSettings::SetSliderLifespan(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupLifespanSlider->value(Gui->PopupLifespanInput->value());
}

void PopupSettings::ActivateFadeout(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	if (Gui->PopupFadeOut->value() == 0) {
		Gui->PopupFadeOutTimeSlider->deactivate();
		Gui->PopupFadeOutStepsSlider->deactivate();
		Gui->PopupFadeOutStepsInput->deactivate();
		Gui->PopupFadeOutTimeInput->deactivate();
	}
	else {
		Gui->PopupFadeOutTimeSlider->activate();
		Gui->PopupFadeOutStepsSlider->activate();
		Gui->PopupFadeOutStepsInput->activate();
		Gui->PopupFadeOutTimeInput->activate();
	}
}

void PopupSettings::SetPopupFadeOutStepsInput(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupFadeOutStepsInput->value(Gui->PopupFadeOutStepsSlider->value());
	/*
	if (Gui->PopupFadeOutStepsInput->value() > Gui->PopupFadeOutTimeInput->value()) {
		Gui->PopupFadeOutTimeInput->value(Gui->PopupFadeOutStepsSlider->value());
		Gui->PopupFadeOutTimeSlider->value(Gui->PopupFadeOutStepsSlider->value());
	}
	*/
}

void PopupSettings::SetPopupFadeOutTimeInput(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupFadeOutTimeInput->value(Gui->PopupFadeOutTimeSlider->value());
	/*
	if (Gui->PopupFadeOutStepsSlider->value() > Gui->PopupFadeOutTimeInput->value()) {
		Gui->PopupFadeOutStepsInput->value(Gui->PopupFadeOutTimeInput->value());
		Gui->PopupFadeOutStepsSlider->value(Gui->PopupFadeOutTimeInput->value());
	}
	*/
}

void PopupSettings::SetPopupFadeOutStepsSlider(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupFadeOutStepsSlider->value(Gui->PopupFadeOutStepsInput->value());
	/*
	if (Gui->PopupFadeOutStepsSlider->value() > Gui->PopupFadeOutTimeInput->value()) {
		Gui->PopupFadeOutTimeInput->value(Gui->PopupFadeOutStepsInput->value());
		Gui->PopupFadeOutTimeSlider->value(Gui->PopupFadeOutStepsInput->value());
	}
	if (Gui->PopupFadeOutStepsInput->value() < 3) {
		Gui->PopupFadeOutStepsSlider->value(3);
		Gui->PopupFadeOutStepsInput->value(3);
	}
	if (Gui->PopupFadeOutTimeInput->value() < 3) {
		Gui->PopupFadeOutTimeSlider->value(3);
		Gui->PopupFadeOutTimeInput->value(3);
	}
	*/
}

void PopupSettings::SetPopupFadeOutTimeSlider(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupFadeOutTimeSlider->value(Gui->PopupFadeOutTimeInput->value());
	/* 
	if (Gui->PopupFadeOutStepsSlider->value() > Gui->PopupFadeOutTimeInput->value()) {
		Gui->PopupFadeOutStepsInput->value(Gui->PopupFadeOutTimeInput->value());
		Gui->PopupFadeOutStepsSlider->value(Gui->PopupFadeOutTimeInput->value());
	}
	if (Gui->PopupFadeOutTimeInput->value() < 3) {
		Gui->PopupFadeOutTimeSlider->value(3);
		Gui->PopupFadeOutTimeInput->value(3);
	}
	if (Gui->PopupFadeOutStepsInput->value() < 3) {
		Gui->PopupFadeOutStepsSlider->value(3);
		Gui->PopupFadeOutStepsInput->value(3);
	}
	*/
}

void PopupSettings::SetPopupOpacitySlider(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupOpacitySlider->value(Gui->PopupOpacityInput->value());
}

void PopupSettings::SetPopupOpacityInput(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->PopupOpacityInput->value(Gui->PopupOpacitySlider->value());
}

void PopupSettings::SetBurstAmountSlider(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->BurstAmountSlider->value(Gui->BurstAmountInput->value());
}


void PopupSettings::SetBurstAmountInput(Fl_Widget* w, void* data) {
	PopupSettings* Gui = static_cast<PopupSettings*>(data);
	Gui->BurstAmountInput->value(Gui->BurstAmountSlider->value());
}

PopupSettings::~PopupSettings() {
	delete (this->ButtonXinput);
	delete (this->ButtonYinput);
	delete (this->ButtonTextInput);
	delete (this->PopupFadeOut);
	delete (this->PopupFadeOutStepsSlider);
	delete (this->PopupFadeOutTimeSlider);
	delete (this->PopupFadeOutStepsInput);
	delete (this->PopupFadeOutTimeInput);
	delete (this->PopupOpacitySlider);
	delete (this->PopupOpacityInput);
	delete (this->Overlay);
	delete (this->PopupLifespan);
	delete (this->PopupLifespanInput);
	delete (this->PopupLifespanSlider);
	delete (this->TimeBetweenPopups);
	delete (this->TimeBetweenPopupsSlider);
	delete (this->TimeBetweenPopupsInput);
	delete (this->MultiplicatiorInput);
	delete (this->ImageFolderPath);
	delete (this->BurstAmountInput);
	delete (this->BurstAmountSlider);
}