#include <wx/wx.h>
#include "wx_md3/components/MD3Card.h"
#include "wx_md3/components/MD3Button.h"
#include "wx_md3/components/MD3Checkbox.h"
#include "wx_md3/components/MD3Switch.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3CardExample : public wxFrame {
public:
    MD3CardExample() : wxFrame(nullptr, wxID_ANY, "MD3 Card Example", wxDefaultPosition, wxSize(600, 500)) {
        // Create a panel to hold the cards
        wxPanel* panel = new wxPanel(this);

        // Create the main layout column
        wx_md3::MD3Column* mainColumn = new wx_md3::MD3Column(
            wx_md3::MD3MainAxisAlignment::Start,
            wx_md3::MD3CrossAxisAlignment::Center,
            20 // spacing
        );

        // Create elevated card section with buttons
        wx_md3::MD3Card* elevatedCard = new wx_md3::MD3Card(panel, wxID_ANY);
        elevatedCard->SetVariant(wx_md3::MD3CardVariant::Elevated);

        // Create a sizer for the elevated card content
        wxBoxSizer* elevatedSizer = new wxBoxSizer(wxVERTICAL);
        elevatedSizer->Add(new wxStaticText(elevatedCard, wxID_ANY, "Elevated Card - Buttons"), 0, wxALL, 12);

        wx_md3::MD3Button* elevatedBtn1 = new wx_md3::MD3Button(elevatedCard, wxID_ANY, "Primary");
        elevatedBtn1->SetVariant(wx_md3::MD3ButtonVariant::Filled);

        wx_md3::MD3Button* elevatedBtn2 = new wx_md3::MD3Button(elevatedCard, wxID_ANY, "Secondary");
        elevatedBtn2->SetVariant(wx_md3::MD3ButtonVariant::Outlined);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(elevatedBtn1, 0, wxLEFT | wxRIGHT, 8);
        buttonSizer->Add(elevatedBtn2, 0, wxLEFT | wxRIGHT, 8);

        elevatedSizer->Add(buttonSizer, 0, wxALL, 8);
        elevatedCard->SetSizer(elevatedSizer);

        // Create filled card section with checkboxes
        wx_md3::MD3Card* filledCard = new wx_md3::MD3Card(panel, wxID_ANY);
        filledCard->SetVariant(wx_md3::MD3CardVariant::Filled);

        wxBoxSizer* filledSizer = new wxBoxSizer(wxVERTICAL);
        filledSizer->Add(new wxStaticText(filledCard, wxID_ANY, "Filled Card - Checkboxes"), 0, wxALL, 12);

        wx_md3::MD3Checkbox* checkbox1 = new wx_md3::MD3Checkbox(filledCard, wxID_ANY, "Option 1");
        wx_md3::MD3Checkbox* checkbox2 = new wx_md3::MD3Checkbox(filledCard, wxID_ANY, "Option 2");
        wx_md3::MD3Checkbox* checkbox3 = new wx_md3::MD3Checkbox(filledCard, wxID_ANY, "Option 3");

        filledSizer->Add(checkbox1, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        filledSizer->Add(checkbox2, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        filledSizer->Add(checkbox3, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        filledCard->SetSizer(filledSizer);

        // Create outlined card section with switches
        wx_md3::MD3Card* outlinedCard = new wx_md3::MD3Card(panel, wxID_ANY);
        outlinedCard->SetVariant(wx_md3::MD3CardVariant::Outlined);

        wxBoxSizer* outlinedSizer = new wxBoxSizer(wxVERTICAL);
        outlinedSizer->Add(new wxStaticText(outlinedCard, wxID_ANY, "Outlined Card - Switches"), 0, wxALL, 12);

        wx_md3::MD3Switch* switch1 = new wx_md3::MD3Switch(outlinedCard, wxID_ANY, "WiFi");
        wx_md3::MD3Switch* switch2 = new wx_md3::MD3Switch(outlinedCard, wxID_ANY, "Bluetooth");
        wx_md3::MD3Switch* switch3 = new wx_md3::MD3Switch(outlinedCard, wxID_ANY, "Location");

        outlinedSizer->Add(switch1, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        outlinedSizer->Add(switch2, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        outlinedSizer->Add(switch3, 0, wxLEFT | wxRIGHT | wxBOTTOM, 8);
        outlinedCard->SetSizer(outlinedSizer);

        // Bind events for demonstration
        elevatedBtn1->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3CardExample::OnButtonClicked, this);
        elevatedBtn2->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3CardExample::OnButtonClicked, this);

        checkbox1->Bind(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, &MD3CardExample::OnCheckboxToggled, this);
        checkbox2->Bind(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, &MD3CardExample::OnCheckboxToggled, this);
        checkbox3->Bind(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, &MD3CardExample::OnCheckboxToggled, this);

        switch1->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, &MD3CardExample::OnSwitchToggled, this);
        switch2->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, &MD3CardExample::OnSwitchToggled, this);
        switch3->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, &MD3CardExample::OnSwitchToggled, this);

        // Add cards to main layout
        mainColumn->Add(elevatedCard, 0, 12);
        mainColumn->Add(filledCard, 0, 12);
        mainColumn->Add(outlinedCard, 0, 12);

        // Set the sizer for the panel
        panel->SetSizer(mainColumn->GetSizer());

        // Center the frame
        Centre();
    }

private:
    void OnButtonClicked(wxCommandEvent& event) {
        wxMessageBox("Button clicked!", "MD3 Card Example", wxOK | wxICON_INFORMATION);
    }

    void OnCheckboxToggled(wxCommandEvent& event) {
        bool checked = event.GetInt() != 0;
        wxString message = wxString::Format("Checkbox %s", checked ? "checked" : "unchecked");
        wxMessageBox(message, "MD3 Card Example", wxOK | wxICON_INFORMATION);
    }

    void OnSwitchToggled(wxCommandEvent& event) {
        bool checked = event.GetInt() != 0;
        wxString message = wxString::Format("Switch %s", checked ? "turned on" : "turned off");
        wxMessageBox(message, "MD3 Card Example", wxOK | wxICON_INFORMATION);
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3CardExample* frame = new MD3CardExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);