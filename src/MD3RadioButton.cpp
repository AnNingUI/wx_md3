#include "wx_md3/components/MD3RadioButton.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Animator.h"
#include <wx/dcbuffer.h>
#include <cmath>

namespace wx_md3 {

    wxDEFINE_EVENT(wxEVT_MD3_RADIOBUTTON_SELECTED, wxCommandEvent);

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3RadioButton, MD3Control)
        EVT_PAINT(MD3RadioButton::OnPaint)
        EVT_LEFT_DOWN(MD3RadioButton::OnMouseLeftDown)
        EVT_LEFT_UP(MD3RadioButton::OnMouseLeftUp)
        EVT_ENTER_WINDOW(MD3RadioButton::OnMouseEnter)
        EVT_LEAVE_WINDOW(MD3RadioButton::OnMouseLeave)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3RadioButton, MD3Control)

    // Constructor
    MD3RadioButton::MD3RadioButton() {
        Init();
    }

    MD3RadioButton::MD3RadioButton(wxWindow* parent, wxWindowID id,
                                   const wxString& label,
                                   const wxPoint& pos, const wxSize& size,
                                   long style, const wxValidator& validator,
                                   const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        m_label = label;
        Init();
    }

    // Initialization
    void MD3RadioButton::Init() {
        m_selected = false;
        m_hovered = false;
        m_size = 24;        // MD3 standard radio button size
        m_strokeWidth = 2;
        m_fillProgress = 0.0f;

        SetBackgroundStyle(wxBG_STYLE_PAINT);
        UpdateAppearance();
    }

    // Destructor
    MD3RadioButton::~MD3RadioButton() {
        // Cleanup resources
    }

    // Set selected state
    void MD3RadioButton::SetValue(bool value) {
        if (m_selected != value) {
            m_selected = value;
            
            // Animate the radio button fill
            if (value) {
                m_fillProgress = 0.0f;
                StartAnimation(MD3AnimationType::ScaleFade);
            } else {
                m_fillProgress = 0.0f;
            }
            
            Refresh();
        }
    }

    void MD3RadioButton::SetLabel(const wxString& label) {
        m_label = label;
        Refresh();
    }

    // Override SetState
    void MD3RadioButton::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();
        Refresh();
    }

    wxSize MD3RadioButton::DoGetBestSize() const {
        wxSize size(m_size + 8, m_size + 8); // 4px padding on each side

        if (!m_label.IsEmpty()) {
            wxCoord textWidth, textHeight;
            wxClientDC dc(const_cast<MD3RadioButton*>(this));
            dc.GetTextExtent(m_label, &textWidth, &textHeight);
            size.x += textWidth + 12; // 8px spacing between radio and label
            size.y = std::max(size.y, textHeight + 8);
        }

        return size;
    }

    // Event handling
    void MD3RadioButton::OnPaint(wxPaintEvent& event) {
        wxBufferedPaintDC dc(this);
        // 🔧 不在这里清除，在Render中绘制背景
        Render(dc);
    }

    void MD3RadioButton::OnMouseLeftDown(wxMouseEvent& event) {
        if (IsEnabled()) {
            SetState(MD3State::Pressed);
        }
        event.Skip();
    }

    void MD3RadioButton::OnMouseLeftUp(wxMouseEvent& event) {
        if (IsEnabled()) {
            // Check if mouse is still over the control
            wxPoint pos = event.GetPosition();
            if (pos.x >= 0 && pos.x < GetClientSize().GetWidth() &&
                pos.y >= 0 && pos.y < GetClientSize().GetHeight()) {
                
                // Select radio button
                if (!m_selected) {
                    SetValue(true);
                    
                    // Send selection event
                    wxCommandEvent radioEvent(wxEVT_MD3_RADIOBUTTON_SELECTED, GetId());
                    radioEvent.SetInt(1);
                    radioEvent.SetEventObject(this);
                    ProcessEvent(radioEvent);
                }
                
                SetState(MD3State::Hover);
            } else {
                SetState(MD3State::Normal);
            }
        }
        event.Skip();
    }

    void MD3RadioButton::OnMouseEnter(wxMouseEvent& event) {
        if (IsEnabled()) {
            m_hovered = true;
            SetState(MD3State::Hover);
        }
        event.Skip();
    }

    void MD3RadioButton::OnMouseLeave(wxMouseEvent& event) {
        m_hovered = false;
        SetState(MD3State::Normal);
        event.Skip();
    }

    // Internal methods
    void MD3RadioButton::UpdateAppearance() {
        // Update colors based on theme and state
        SetBackgroundColour(*wxWHITE);
    }

    void MD3RadioButton::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        auto theme = MD3Theme::GetCurrentTheme();
        
        // 🔧 首先绘制背景（清除之前的内容）
        wxColour clearColor = GetParent() ? GetParent()->GetBackgroundColour() : *wxWHITE;
        dc.SetBrush(wxBrush(clearColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

        // Draw radio button circle
        int radioX = 4;
        int radioY = (size.GetHeight() - m_size) / 2;
        int radioCenterX = radioX + m_size / 2;
        int radioCenterY = radioY + m_size / 2;
        
        wxColour borderColor = GetBorderColor();
        wxColour bgColor;
        
        if (m_selected) {
            bgColor = theme->GetColor("primary");
        } else {
            bgColor = (m_state == MD3State::Hover) ? theme->GetColor("surfaceVariant") : *wxWHITE;
        }
        
        // Draw radio button outer circle
        dc.SetBrush(wxBrush(bgColor));
        dc.SetPen(wxPen(borderColor, m_strokeWidth));
        dc.DrawCircle(radioCenterX, radioCenterY, m_size / 2);
        
        // Draw filled dot if selected
        if (m_selected) {
            wxColour dotColor = theme->GetColor("onPrimary");
            dc.SetBrush(wxBrush(dotColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            
            // Calculate dot size based on fill progress
            int dotRadius = std::max(1, static_cast<int>(m_size / 4.0f * m_fillProgress));
            dc.DrawCircle(radioCenterX, radioCenterY, dotRadius);
        }
        
        // Draw label
        if (!m_label.IsEmpty()) {
            int labelX = radioX + m_size + 8;
            int labelY = (size.GetHeight() - dc.GetCharHeight()) / 2;
            
            // 🔧 确保设置字体和文字颜色
            dc.SetTextForeground(theme->GetColor("onSurface"));
            dc.SetFont(GetFont());
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetPen(*wxTRANSPARENT_PEN);
            
            dc.DrawText(m_label, labelX, labelY);
        }
    }

    wxColour MD3RadioButton::GetRadioColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        switch (m_state) {
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            case MD3State::Pressed:
                return theme->Darken(theme->GetColor("primary"), 0.1f);
            default:
                return theme->GetColor("primary");
        }
    }

    wxColour MD3RadioButton::GetBorderColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        if (m_selected) {
            return GetRadioColor();
        }
        
        switch (m_state) {
            case MD3State::Hover:
                return theme->GetColor("onSurfaceVariant");
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            default:
                return theme->GetColor("outline");
        }
    }

} // namespace wx_md3
