#include <wx/wx.h>
#include "wx_md3/components/MD3Checkbox.h"

class SimpleCheckboxFrame : public wxFrame {
public:
    SimpleCheckboxFrame() : wxFrame(nullptr, wxID_ANY, "Simple Checkbox Test", wxDefaultPosition, wxSize(300, 200)) {
        wxPanel* panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxWHITE);
        
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        
        // Simple checkbox without any layout
        wx_md3::MD3Checkbox* cb = new wx_md3::MD3Checkbox(panel, wxID_ANY, "Click me!");
        wxLogMessage("Created checkbox: %p", cb);
        
        sizer->Add(cb, 0, wxALL, 20);
        sizer->Add(new wxButton(panel, wxID_ANY, "Check checkbox value"), 0, wxALL, 10);
        
        panel->SetSizer(sizer);
        Centre();
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        SimpleCheckboxFrame* frame = new SimpleCheckboxFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
