#include <wx/wx.h>
#include "wx_md3/components/MD3Button.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3ButtonExample : public wxFrame {
public:
    MD3ButtonExample() : wxFrame(nullptr, wxID_ANY, "MD3 Button Example", wxDefaultPosition, wxSize(400, 300)) {
        // Create a panel to hold the buttons
        wxPanel* panel = new wxPanel(this);

        // Create MD3 buttons with different variants
        wx_md3::MD3Button* elevatedButton = new wx_md3::MD3Button(panel, wxID_ANY, "Elevated Button");
        elevatedButton->SetVariant(wx_md3::MD3ButtonVariant::Elevated);
        // elevatedButton->SetMinSize(wxSize(150, 40));  // 不强制设置，让 DoGetBestSize 自动计算

        wx_md3::MD3Button* filledButton = new wx_md3::MD3Button(panel, wxID_ANY, "Filled Button");
        filledButton->SetVariant(wx_md3::MD3ButtonVariant::Filled);
        // filledButton->SetMinSize(wxSize(150, 40));    // 不强制设置，让 DoGetBestSize 自动计算

        wx_md3::MD3Button* outlinedButton = new wx_md3::MD3Button(panel, wxID_ANY, "Outlined Button");
        outlinedButton->SetVariant(wx_md3::MD3ButtonVariant::Outlined);
        // outlinedButton->SetMinSize(wxSize(150, 40));  // 不强制设置，让 DoGetBestSize 自动计算

        wx_md3::MD3Button* textButton = new wx_md3::MD3Button(panel, wxID_ANY, "Text Button");
        textButton->SetVariant(wx_md3::MD3ButtonVariant::Text);
        // textButton->SetMinSize(wxSize(150, 40));      // 不强制设置，让 DoGetBestSize 自动计算

        // Bind button click events
        elevatedButton->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3ButtonExample::OnButtonClicked, this);
        filledButton->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3ButtonExample::OnButtonClicked, this);
        outlinedButton->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3ButtonExample::OnButtonClicked, this);
        textButton->Bind(wx_md3::wxEVT_MD3_BUTTON_CLICKED, &MD3ButtonExample::OnButtonClicked, this);

        // Create layout using MD3Layout
        // 🔧 创建列布局，所有权由panel的sizer管理
        wx_md3::MD3Column column(
            wx_md3::MD3MainAxisAlignment::Center,
            wx_md3::MD3CrossAxisAlignment::Center,
            16 // spacing
        );

        column.Add(elevatedButton, 0, 12);
        column.Add(filledButton, 0, 12);
        column.Add(outlinedButton, 0, 12);
        column.Add(textButton, 0, 12);

        // 🔧 正确设置sizer（sizer的所有权转移给panel）
        panel->SetSizer(column.GetSizer());

        // Center the frame
        Centre();
    }

private:
    void OnButtonClicked(wxCommandEvent& event) {
        wxMessageBox("Button clicked!", "MD3 Button Example", wxOK | wxICON_INFORMATION);
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3ButtonExample* frame = new MD3ButtonExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);