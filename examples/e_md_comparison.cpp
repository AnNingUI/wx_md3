#include <wx/wx.h>
#include "wx_md3/components/MD3Button.h"
#include "wx_md3/components/MD3Checkbox.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3ComparisonExample : public wxFrame {
public:
    MD3ComparisonExample() : wxFrame(nullptr, wxID_ANY, "MD3 Button vs Checkbox Comparison", wxDefaultPosition, wxSize(500, 400)) {
        // Create a panel to hold the controls
        wxPanel* panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxWHITE);

        // Test 1: Simple Button
        auto* simpleBtn = new wx_md3::MD3Button(panel, wxID_ANY, "Simple Button");
        simpleBtn->SetVariant(wx_md3::MD3ButtonVariant::Elevated);

        // Test 2: Simple Checkbox
        auto* simpleCb = new wx_md3::MD3Checkbox(panel, wxID_ANY, "Simple Checkbox");
        simpleCb->SetValue(false);

        // Test 3: Filled Button
        auto* filledBtn = new wx_md3::MD3Button(panel, wxID_ANY, "Filled Button");
        filledBtn->SetVariant(wx_md3::MD3ButtonVariant::Filled);

        // Test 4: Outlined Button
        auto* outlinedBtn = new wx_md3::MD3Button(panel, wxID_ANY, "Outlined Button");
        outlinedBtn->SetVariant(wx_md3::MD3ButtonVariant::Outlined);

        // Create layout using MD3Layout
        wx_md3::MD3Column column(
            wx_md3::MD3MainAxisAlignment::Center,
            wx_md3::MD3CrossAxisAlignment::Center,
            16
        );

        column.Add(simpleBtn, 0, 8);
        column.Add(simpleCb, 0, 8);
        column.Add(filledBtn, 0, 8);
        column.Add(outlinedBtn, 0, 8);

        panel->SetSizer(column.GetSizer());
        Centre();
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3ComparisonExample* frame = new MD3ComparisonExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
