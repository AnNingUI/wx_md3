#include <wx/wx.h>
#include "wx_md3/components/MD3RadioButton.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3RadioButtonExample : public wxFrame {
public:
    MD3RadioButtonExample() : wxFrame(nullptr, wxID_ANY, "MD3 RadioButton Example", wxDefaultPosition, wxSize(400, 300)) {
        // Create a panel to hold the radio buttons
        wxPanel* panel = new wxPanel(this);

        // Create MD3 radio buttons
        wx_md3::MD3RadioButton* radio1 = new wx_md3::MD3RadioButton(panel, wxID_ANY, "Option 1");
        wx_md3::MD3RadioButton* radio2 = new wx_md3::MD3RadioButton(panel, wxID_ANY, "Option 2");
        wx_md3::MD3RadioButton* radio3 = new wx_md3::MD3RadioButton(panel, wxID_ANY, "Option 3");
        wx_md3::MD3RadioButton* radio4 = new wx_md3::MD3RadioButton(panel, wxID_ANY, "Option 4 (Disabled)");
        radio4->Enable(false);

        // Set first radio as selected
        radio1->SetValue(true);

        // Bind radio button events
        radio1->Bind(wx_md3::wxEVT_MD3_RADIOBUTTON_SELECTED, &MD3RadioButtonExample::OnRadioSelected, this);
        radio2->Bind(wx_md3::wxEVT_MD3_RADIOBUTTON_SELECTED, &MD3RadioButtonExample::OnRadioSelected, this);
        radio3->Bind(wx_md3::wxEVT_MD3_RADIOBUTTON_SELECTED, &MD3RadioButtonExample::OnRadioSelected, this);

        // Store radio buttons for group management
        m_radios = {radio1, radio2, radio3, radio4};

        // Create layout using MD3Layout
        wx_md3::MD3Column column(
            wx_md3::MD3MainAxisAlignment::Center,
            wx_md3::MD3CrossAxisAlignment::Center,
            16 // spacing
        );

        column.Add(radio1, 0, 8);
        column.Add(radio2, 0, 8);
        column.Add(radio3, 0, 8);
        column.Add(radio4, 0, 8);

        // Set the sizer for the panel
        panel->SetSizer(column.GetSizer());

        // Center the frame
        Centre();
    }

private:
    std::vector<wx_md3::MD3RadioButton*> m_radios;

    void OnRadioSelected(wxCommandEvent& event) {
        // Deselect other radio buttons in the group
        wx_md3::MD3RadioButton* selected = nullptr;
        for (auto radio : m_radios) {
            if (radio->HasCapture() || (event.GetEventObject() == radio)) {
                selected = radio;
            }
        }

        if (selected) {
            for (auto radio : m_radios) {
                if (radio != selected) {
                    radio->SetValue(false);
                }
            }
            wxLogMessage("Radio button selected");
        }
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3RadioButtonExample* frame = new MD3RadioButtonExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
