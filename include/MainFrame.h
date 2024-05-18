#pragma once
#include <wx/wx.h>
#include <wx/hyperlink.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void OnButtonClicked(wxCommandEvent& evt);

	wxPanel* panel;
	wxButton* button;
	wxTextCtrl* inputField;
	wxStaticText* enterLinkTxt;
	wxStaticText* waitForLink;
	wxStaticText* hereLink;
	wxHyperlinkCtrl* link;
};