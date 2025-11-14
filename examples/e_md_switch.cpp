#include <wx/wx.h>
#include "wx_md3/components/MD3Switch.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3SwitchExample : public wxFrame {
public:
    MD3SwitchExample() : wxFrame(nullptr, wxID_ANY, "MD3 Switch Example", wxDefaultPosition, wxSize(400, 300)) {
        // Create a panel to hold the switches
        wxPanel* panel = new wxPanel(this);

        // Create MD3 switches
        wx_md3::MD3Switch* switch1 = new wx_md3::MD3Switch(panel, wxID_ANY, "WiFi");
        wx_md3::MD3Switch* switch2 = new wx_md3::MD3Switch(panel, wxID_ANY, "Bluetooth");
        wx_md3::MD3Switch* switch3 = new wx_md3::MD3Switch(panel, wxID_ANY, "Location (Disabled)");
        switch3->Enable(false);

        // Set some switches as enabled
        switch1->SetValue(true);

        // Bind switch events
        switch1->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, &MD3SwitchExample::OnSwitchToggled, this);
        switch2->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, &MD3SwitchExample::OnSwitchToggled, this);

        // Create layout using MD3Layout
        wx_md3::MD3Column column(
            wx_md3::MD3MainAxisAlignment::Center,
            wx_md3::MD3CrossAxisAlignment::Center,
            16 // spacing
        );

        column.Add(switch1, 0, 8);
        column.Add(switch2, 0, 8);
        column.Add(switch3, 0, 8);

        // Set the sizer for the panel
        panel->SetSizer(column.GetSizer());

        // Center the frame
        Centre();
    }

private:
    void OnSwitchToggled(wxCommandEvent& event) {
        wxString message = wxString::Format("Switch toggled! State: %s", 
                                           event.GetInt() ? "On" : "Off");
        wxLogMessage(message);
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3SwitchExample* frame = new MD3SwitchExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
