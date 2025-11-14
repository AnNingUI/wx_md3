#include <wx/wx.h>
#include "wx_md3/components/MD3Checkbox.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3CheckboxExample : public wxFrame {
public:
    MD3CheckboxExample() : wxFrame(nullptr, wxID_ANY, "MD3 Checkbox Example", wxDefaultPosition, wxSize(400, 300)) {
        // Create a panel to hold the checkboxes
        wxPanel* panel = new wxPanel(this);

        // Create MD3 checkboxes
        wx_md3::MD3Checkbox* checkbox1 = new wx_md3::MD3Checkbox(panel, wxID_ANY, "Checkbox 1");
        wx_md3::MD3Checkbox* checkbox2 = new wx_md3::MD3Checkbox(panel, wxID_ANY, "Checkbox 2");
        wx_md3::MD3Checkbox* checkbox3 = new wx_md3::MD3Checkbox(panel, wxID_ANY, "Checkbox 3 (Disabled)");
        checkbox3->Enable(false);

        // Bind checkbox events
        checkbox1->Bind(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, &MD3CheckboxExample::OnCheckboxToggled, this);
        checkbox2->Bind(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, &MD3CheckboxExample::OnCheckboxToggled, this);

        // Create layout using MD3Layout
        wx_md3::MD3Column column(
            wx_md3::MD3MainAxisAlignment::Center,
            wx_md3::MD3CrossAxisAlignment::Center,
            16 // spacing
        );

        column.Add(checkbox1, 0, 8);
        column.Add(checkbox2, 0, 8);
        column.Add(checkbox3, 0, 8);

        // Set the sizer for the panel
        panel->SetSizer(column.GetSizer());

        // Center the frame
        Centre();
    }

private:
    void OnCheckboxToggled(wxCommandEvent& event) {
        wxString message = wxString::Format("Checkbox toggled! State: %s", 
                                           event.GetInt() ? "Checked" : "Unchecked");
        wxLogMessage(message);
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3CheckboxExample* frame = new MD3CheckboxExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
