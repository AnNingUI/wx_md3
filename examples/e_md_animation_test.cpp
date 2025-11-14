#include <wx/wx.h>
#include "wx_md3/components/MD3Switch.h"
#include "wx_md3/core/MD3Animator.h"

class AnimationTestFrame : public wxFrame {
public:
    AnimationTestFrame() : wxFrame(nullptr, wxID_ANY, "MD3 Animation Test", 
                                   wxDefaultPosition, wxSize(400, 300)) {
        wxPanel* panel = new wxPanel(this);
        panel->SetBackgroundColour(*wxWHITE);
        
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        
        // 添加文本信息
        wxStaticText* info = new wxStaticText(panel, wxID_ANY,
            "Testing MD3 Animation System:\n\n"
            "1. Click the switch below\n"
            "2. You should see smooth thumb animation\n"
            "3. (300ms EaseInOut)\n\n"
            "Check console for debug output");
        sizer->Add(info, 0, wxALL | wxEXPAND, 20);
        
        // 创建开关
        m_switch = new wx_md3::MD3Switch(panel, wxID_ANY, "Test Switch");
        sizer->Add(m_switch, 0, wxALL | wxCENTER, 20);
        
        // 绑定开关事件
        m_switch->Bind(wx_md3::wxEVT_MD3_SWITCH_TOGGLED, 
                      [this](wxCommandEvent& evt) {
                          wxLogMessage("Switch toggled! State: %s", 
                                      m_switch->GetValue() ? "ON" : "OFF");
                      });
        
        // 添加按钮来手动测试动画
        wxButton* testBtn = new wxButton(panel, wxID_ANY, "Test Animation Directly");
        testBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
            wxLogMessage("Testing animation directly...");
            wxLogMessage("Current switch state: %s", 
                        m_switch->GetValue() ? "ON" : "OFF");
            wxLogMessage("Setting to: %s", 
                        m_switch->GetValue() ? "OFF" : "ON");
            m_switch->SetValue(!m_switch->GetValue());
        });
        sizer->Add(testBtn, 0, wxALL | wxCENTER, 10);
        
        panel->SetSizer(sizer);
        Centre();
        Show();
    }

private:
    wx_md3::MD3Switch* m_switch;
};

class TestApp : public wxApp {
public:
    bool OnInit() override {
        wxLogWindow* logWin = new wxLogWindow(nullptr, "Debug Log", false);
        wxLog::SetActiveTarget(logWin);
        
        new AnimationTestFrame();
        return true;
    }
};

wxIMPLEMENT_APP(TestApp);
