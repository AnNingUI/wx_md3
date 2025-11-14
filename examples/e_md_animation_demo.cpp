#include <wx/wx.h>
#include "wx_md3/components/MD3Button.h"
#include "wx_md3/components/MD3Checkbox.h"
#include "wx_md3/components/MD3Switch.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3AnimationDemo : public wxFrame {
public:
    MD3AnimationDemo() : wxFrame(nullptr, wxID_ANY, "MD3 Animation Effects Demo", 
                                 wxDefaultPosition, wxSize(600, 700)) {
        // Create main panel
        wxPanel* mainPanel = new wxPanel(this);
        mainPanel->SetBackgroundColour(*wxWHITE);
        
        // Create main sizer
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        
        // ============ BUTTON ANIMATIONS ============
        wxStaticBox* buttonStaticBox = new wxStaticBox(mainPanel, wxID_ANY, "Button Animations");
        wxStaticBoxSizer* buttonBox = new wxStaticBoxSizer(buttonStaticBox, wxVERTICAL);
        
        // Title - 必须是 StaticBox 的子窗口
        wxStaticText* buttonTitle = new wxStaticText(buttonStaticBox, wxID_ANY, 
            "✨ Button Features: Ripple + Elevation\n"
            "Click buttons to see ripple animation expand from click point");
        buttonTitle->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        buttonBox->Add(buttonTitle, 0, wxALL | wxEXPAND, 8);
        
        // Create different button variants
        wx_md3::MD3Button* elevatedBtn = new wx_md3::MD3Button(buttonStaticBox, wxID_ANY, "Elevated Button");
        elevatedBtn->SetVariant(wx_md3::MD3ButtonVariant::Elevated);
        
        wx_md3::MD3Button* filledBtn = new wx_md3::MD3Button(buttonStaticBox, wxID_ANY, "Filled Button");
        filledBtn->SetVariant(wx_md3::MD3ButtonVariant::Filled);
        
        wx_md3::MD3Button* outlinedBtn = new wx_md3::MD3Button(buttonStaticBox, wxID_ANY, "Outlined Button");
        outlinedBtn->SetVariant(wx_md3::MD3ButtonVariant::Outlined);
        
        wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        btnSizer->Add(elevatedBtn, 0, wxALL, 6);
        btnSizer->Add(filledBtn, 0, wxALL, 6);
        btnSizer->Add(outlinedBtn, 0, wxALL, 6);
        buttonBox->Add(btnSizer, 0, wxALL | wxEXPAND, 4);
        
        // ============ CHECKBOX ANIMATIONS ============
        wxStaticBox* checkboxStaticBox = new wxStaticBox(mainPanel, wxID_ANY, "Checkbox Animations");
        wxStaticBoxSizer* checkboxBox = new wxStaticBoxSizer(checkboxStaticBox, wxVERTICAL);
        
        wxStaticText* checkboxTitle = new wxStaticText(checkboxStaticBox, wxID_ANY,
            "✨ Checkbox Features: Smooth Check Animation\n"
            "Watch the checkmark appear/disappear smoothly (400ms animation)");
        checkboxTitle->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        checkboxBox->Add(checkboxTitle, 0, wxALL | wxEXPAND, 8);
        
        wx_md3::MD3Checkbox* check1 = new wx_md3::MD3Checkbox(checkboxStaticBox, wxID_ANY, "Enable Notifications");
        wx_md3::MD3Checkbox* check2 = new wx_md3::MD3Checkbox(checkboxStaticBox, wxID_ANY, "Accept Terms");
        check1->SetValue(false);
        check2->SetValue(true);
        
        wxBoxSizer* checkSizer = new wxBoxSizer(wxVERTICAL);
        checkSizer->Add(check1, 0, wxALL, 6);
        checkSizer->Add(check2, 0, wxALL, 6);
        checkboxBox->Add(checkSizer, 0, wxALL | wxEXPAND, 4);
        
        // ============ SWITCH ANIMATIONS ============
        wxStaticBox* switchStaticBox = new wxStaticBox(mainPanel, wxID_ANY, "Switch Animations");
        wxStaticBoxSizer* switchBox = new wxStaticBoxSizer(switchStaticBox, wxVERTICAL);
        
        wxStaticText* switchTitle = new wxStaticText(switchStaticBox, wxID_ANY,
            "✨ Switch Features: Smooth Thumb Slide\n"
            "Watch the thumb slide from left to right (300ms animation)");
        switchTitle->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        switchBox->Add(switchTitle, 0, wxALL | wxEXPAND, 8);
        
        wx_md3::MD3Switch* switch1 = new wx_md3::MD3Switch(switchStaticBox, wxID_ANY, "WiFi");
        wx_md3::MD3Switch* switch2 = new wx_md3::MD3Switch(switchStaticBox, wxID_ANY, "Bluetooth");
        wx_md3::MD3Switch* switch3 = new wx_md3::MD3Switch(switchStaticBox, wxID_ANY, "Location (Disabled)");
        
        // 不要在初始化时设置值，让用户点击观看动画
        // switch1->SetValue(true);
        // switch2->SetValue(false);
        // switch3->Enable(false);
        
        wxBoxSizer* switchSizer = new wxBoxSizer(wxVERTICAL);
        switchSizer->Add(switch1, 0, wxALL, 6);
        switchSizer->Add(switch2, 0, wxALL, 6);
        switchSizer->Add(switch3, 0, wxALL, 6);
        switchBox->Add(switchSizer, 0, wxALL | wxEXPAND, 4);
        
        // ============ INFO BOX ============
        wxStaticBox* infoStaticBox = new wxStaticBox(mainPanel, wxID_ANY, "Animation System Info");
        wxStaticBoxSizer* infoBox = new wxStaticBoxSizer(infoStaticBox, wxVERTICAL);
        
        wxStaticText* info = new wxStaticText(infoStaticBox, wxID_ANY,
            "✨ Material Design 3 Animations Enabled:\n\n"
            "🎯 Button Ripple: Expands from click point (300ms, Linear)\n"
            "🎯 Button Elevation: Shadow transitions on hover/press (200ms, EaseInOut)\n"
            "🎯 Checkbox: Checkmark draws smoothly (400ms, EaseInOut)\n"
            "🎯 Switch: Thumb slides left/right (300ms, EaseInOut)\n\n"
            "All animations use MD3Animator for smooth 60 FPS rendering");
        info->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        infoBox->Add(info, 1, wxALL | wxEXPAND, 8);
        
        // Add all boxes to main sizer
        mainSizer->Add(buttonBox, 0, wxALL | wxEXPAND, 8);
        mainSizer->Add(checkboxBox, 0, wxALL | wxEXPAND, 8);
        mainSizer->Add(switchBox, 0, wxALL | wxEXPAND, 8);
        mainSizer->Add(infoBox, 1, wxALL | wxEXPAND, 8);
        
        // Set panel sizer
        mainPanel->SetSizer(mainSizer);
        
        // Center and show the frame
        Centre();
        Show();
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        new MD3AnimationDemo();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
