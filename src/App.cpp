#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include "icon.xpm"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Points bot");
	mainFrame->SetClientSize(700, 300);
	mainFrame->Center();
	mainFrame->SetIcon(icon);
	mainFrame->Show();
	return true;
}