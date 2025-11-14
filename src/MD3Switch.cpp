// MD3Switch.cpp
#include "wx_md3/components/MD3Switch.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Animator.h"

#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <algorithm>

namespace wx_md3 {

    wxDEFINE_EVENT(wxEVT_MD3_SWITCH_TOGGLED, wxCommandEvent);

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3Switch, MD3Control)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Switch, MD3Control)

    // Constructor
    MD3Switch::MD3Switch() {
        Init();
    }

    MD3Switch::MD3Switch(wxWindow* parent, wxWindowID id,
                         const wxString& label,
                         const wxPoint& pos, const wxSize& size,
                         long style, const wxValidator& validator,
                         const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        m_label = label;
        Init();
    }

    // Initialization
    void MD3Switch::Init() {
        m_enabled = false;
        m_hovered = false;
        m_thumbSize = 24;
        m_trackHeight = 28;
        m_slideProgress = 0.0f;

        // 使用透明背景策略，让控件显示父窗口真实内容
        if (!GetHandle()) {
            SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
        } else {
            SetBackgroundStyle(wxBG_STYLE_PAINT);
        }
        UpdateAppearance();
        
        // ✅ 使用 Bind 动态绑定事件（优先于事件表）
        Bind(wxEVT_PAINT, &MD3Switch::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &MD3Switch::OnMouseLeftDown, this);
        Bind(wxEVT_LEFT_UP, &MD3Switch::OnMouseLeftUp, this);
        Bind(wxEVT_ENTER_WINDOW, &MD3Switch::OnMouseEnter, this);
        Bind(wxEVT_LEAVE_WINDOW, &MD3Switch::OnMouseLeave, this);
    }

    // Destructor
    MD3Switch::~MD3Switch() {
        // Cleanup resources
    }

    // Set switch state
    void MD3Switch::SetValue(bool value) {
        if (m_enabled != value) {
            m_enabled = value;
            
            // ✨ 使用 MD3Animator 创建流畅的动画过渡
            float targetProgress = value ? 1.0f : 0.0f;
            float startProgress = m_slideProgress;
            
            // 创建动画对象
            auto animator = &MD3Animator::GetInstance();
            m_slideAnimation = animator->CreatePropertyAnimation<float>(
                MD3AnimationType::ThumbSlide,
                &m_slideProgress,
                startProgress,
                targetProgress,
                300,  // 300ms 动画时长
                MD3Easing::EaseInOut
            );
            
            // 设置动画更新回调 - 每帧刷新界面
            m_slideAnimation->SetOnUpdateCallback([this]() {
                Refresh();
                Update();
            });
            
            // 动画完成回调
            m_slideAnimation->SetOnCompleteCallback([this]() {
                Refresh();
                Update();
            });
            
            // 启动动画
            animator->Start();
        }
    }

    void MD3Switch::SetLabel(const wxString& label) {
        m_label = label;
        Refresh();
    }

    // Override SetState
    void MD3Switch::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();
        Refresh();
    }

    wxSize MD3Switch::DoGetBestSize() const {
        // Switch track width is approximately 2 times track height
        int switchWidth = m_trackHeight * 2;
        wxSize size(switchWidth + 8, m_trackHeight + 8); // 4px padding

        if (!m_label.IsEmpty()) {
            wxCoord textWidth = 0, textHeight = 0;
            wxScreenDC screenDC;
            screenDC.SetFont(GetFont());
            screenDC.GetTextExtent(m_label, &textWidth, &textHeight);
            size.x += textWidth + 12; // 8px spacing between switch and label
            size.y = std::max(size.y, textHeight + 8);
        }

        return size;
    }

    // Event handling
    void MD3Switch::OnPaint(wxPaintEvent& event) {
        wxBufferedPaintDC dc(this);
        Render(dc);
    }

    void MD3Switch::OnMouseLeftDown(wxMouseEvent& event) {
        if (IsEnabled()) {
            SetState(MD3State::Pressed);
            event.Skip(false);  // 不传播事件
        } else {
            event.Skip();
        }
    }

    void MD3Switch::OnMouseLeftUp(wxMouseEvent& event) {
        wxPoint pos = event.GetPosition();
        wxSize sz = GetClientSize();
        
        if (IsEnabled()) {
            // Check if mouse is still over the control
            if (pos.x >= 0 && pos.x < sz.GetWidth() &&
                pos.y >= 0 && pos.y < sz.GetHeight()) {
                
                // Toggle switch
                SetValue(!m_enabled);
                
                // Send toggle event
                wxCommandEvent switchEvent(wxEVT_MD3_SWITCH_TOGGLED, GetId());
                switchEvent.SetInt(m_enabled ? 1 : 0);
                switchEvent.SetEventObject(this);
                ProcessEvent(switchEvent);
                
                SetState(MD3State::Hover);
                
                event.Skip(false);  // 消费事件，不传播
                return;
            }
        }
        event.Skip();  // 如果不在范围内，传播事件
    }

    void MD3Switch::OnMouseEnter(wxMouseEvent& event) {
        if (IsEnabled()) {
            m_hovered = true;
            SetState(MD3State::Hover);
        }
        event.Skip();
    }

    void MD3Switch::OnMouseLeave(wxMouseEvent& event) {
        m_hovered = false;
        SetState(MD3State::Normal);
        event.Skip();
    }

    // Internal methods
    void MD3Switch::UpdateAppearance() {
        // Update colors based on theme and state
        // 不设置硬编码背景色，让控件透明显示父容器的背景
    }

    // Helper: draw parent's visible content into our DC (fallback when DrawParentBackground is unavailable)
    static void DrawParentBackgroundFallback(wxWindow* win, wxDC& dc, const wxRect& rect) {
        if (!win) return;

        wxWindow* parent = win->GetParent();
        if (!parent) {
            // 没有父窗口：用窗口默认背景色填充
            dc.SetBrush(wxBrush(win->GetBackgroundColour()));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
            return;
        }

        // 计算控件在父窗口中的位置
        wxPoint topleftInParent = win->GetPosition();
        int w = rect.GetWidth();
        int h = rect.GetHeight();

        if (w <= 0 || h <= 0) {
            return;
        }

        // 创建位图并把父窗口内容拷贝进来
        wxBitmap bmp(w, h);
        {
            wxMemoryDC memDC;
            memDC.SelectObject(bmp);

            // 使用父窗口的 ClientDC 作为源 DC
            wxClientDC parentDC(parent);

            // Blit: 源点是控件在父窗口坐标系的位置
            memDC.Blit(0, 0, w, h, &parentDC, topleftInParent.x, topleftInParent.y, wxCOPY, true);

            memDC.SelectObject(wxNullBitmap);
        }

        // 把位图绘制到目标 dc（相对于控件）
        dc.DrawBitmap(bmp, rect.GetX(), rect.GetY(), false);
    }

    void MD3Switch::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        // 先绘制父窗口当前的可见内容到我们的 DC（支持复杂背景）
        wxRect rect(0, 0, size.GetWidth(), size.GetHeight());
        DrawParentBackgroundFallback(this, dc, rect);

        auto theme = MD3Theme::GetCurrentTheme();
        
        // Draw switch track
        int switchX = 4;
        int switchY = (size.GetHeight() - m_trackHeight) / 2;
        int trackWidth = m_trackHeight * 2;
        
        wxColour trackColor = GetTrackColor();
        wxColour thumbColor = GetThumbColor();
        
        // Draw track background
        dc.SetBrush(wxBrush(trackColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRoundedRectangle(switchX, switchY, trackWidth, m_trackHeight, m_trackHeight / 2.0f);
        
        // Calculate thumb position
        // m_slideProgress: 0 = 圆点在左（关闭），1 = 圆点在右（打开）
        int thumbX = switchX + static_cast<int>((trackWidth - m_thumbSize) * m_slideProgress);
        int thumbY = switchY + (m_trackHeight - m_thumbSize) / 2;
        
        // Draw thumb circle
        dc.SetBrush(wxBrush(thumbColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawCircle(thumbX + m_thumbSize / 2, thumbY + m_thumbSize / 2, m_thumbSize / 2);
        
        // Draw label
        if (!m_label.IsEmpty()) {
            int labelX = switchX + trackWidth + 8;
            int labelY = (size.GetHeight() - dc.GetCharHeight()) / 2;
            
            // 🔧 确保设置字体和文字颜色
            dc.SetTextForeground(theme->GetColor("onSurface"));
            dc.SetFont(GetFont());
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetPen(*wxTRANSPARENT_PEN);
            
            dc.DrawText(m_label, labelX, labelY);
        }
    }

    wxColour MD3Switch::GetTrackColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        if (m_enabled) {
            return theme->GetColor("primary");
        }
        
        switch (m_state) {
            case MD3State::Hover:
                return theme->GetColor("surfaceVariant");
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            default:
                return theme->GetColor("surfaceVariant");
        }
    }

    wxColour MD3Switch::GetThumbColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        if (m_enabled) {
            return theme->GetColor("onPrimary");
        }
        
        switch (m_state) {
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            default:
                return theme->GetColor("outline");
        }
    }

} // namespace wx_md3
