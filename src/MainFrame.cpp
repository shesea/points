#include "MainFrame.h"
#include "GetAudioRawData.h"
#include <wx/wx.h>
#include <mfidl.h>
#include <cpr/cpr.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
}

void MainFrame::CreateControls() {
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	button = new wxButton(panel, wxID_ANY, "ОК", wxPoint(270, 230), wxSize(100, 35));
	enterLinkTxt = new wxStaticText(panel, wxID_ANY, "Введите ссылку на встречу:", wxPoint(170, 80), wxSize(100, 35));
	link = new wxHyperlinkCtrl(panel, wxID_ANY, "Ссылка на встречу", "dummy", wxPoint(100, 120), wxSize(500, 50));
	link->Hide();
	waitForLink = new wxStaticText(panel, wxID_ANY, "Ссылка на результаты встречи готовится, подождите.", wxPoint(80, 120), wxSize(400, 40));
	waitForLink->Hide();
	hereLink = new wxStaticText(panel, wxID_ANY, "Ваша ссылка готова:", wxPoint(100, 80), wxSize(400, 40));
	hereLink->Hide();
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(130, 120), wxSize(400, 40));
	enterLinkTxt->SetFont(mainFont);
	inputField->SetFont(mainFont);
	link->SetFont(mainFont);
	waitForLink->SetFont(mainFont);
	hereLink->SetFont(mainFont);

}

void MainFrame::OnButtonClicked(wxCommandEvent& evt) {
	wxString gotLink = inputField->GetValue();
	std::string s = gotLink.ToStdString();
	std::string meetingId;
	int pwdIndex = s.find("pwd");
	int idStart = s.find("/j/") + 3;
	if (pwdIndex != -1) {
		meetingId = s.substr(idStart, pwdIndex - idStart - 1);
	}
	else {
		meetingId = s.substr(idStart);
	}
	std::string pwd = s.substr(pwdIndex + 4);
	wxString myString("meetingId is " + meetingId + ", pwd is: " + pwd);
	//wxLogStatus(myString);

	auto r = cpr::Post(cpr::Url{ "http://points-app.ru:4000" },
		cpr::Body{"{ \"meetingNumber\": " + meetingId + ",\n\"role\": 0}"});
	std::string token = "";
	if (!r.text.empty()) {
		int startToken = r.text.find("{\"signature\"") + 14;
		token = r.text.substr(startToken, r.text.length() - startToken - 2);
	}
	wxString ans(token);
	//wxLogStatus(ans);
	std::cout << r.status_code << std::endl;
	try {
		unsigned long long met = std::stoull(meetingId);
	}
	catch (std::invalid_argument e) {
		wxLogStatus("here");
	}

	button->Hide();
	enterLinkTxt->Hide();
	inputField->Hide();
	this->Hide();

	GetAudioRawData::startInMeetingProcess(token, meetingId, pwd);
	this->SetClientSize(700, 300);
	this->Center();
	this->Show();
	waitForLink->Show();


	r = cpr::Post(cpr::Url{ "https://points-app.ru:8002/api/meeting/upload" },
		cpr::Multipart{ {"file", cpr::File{"audio.pcm"}} });
	wxString code(std::to_string(r.status_code));
	if (!r.text.empty()) {
		link->SetURL("https://points-app.ru/summary/" + r.text);
		link->SetLabelText("https://points-app.ru/summary/" + r.text);
	}
	waitForLink->Hide();
	hereLink->Show();
	link->Show();
	//wxLogStatus(code);

}