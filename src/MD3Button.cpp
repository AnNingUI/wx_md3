#include "wx_md3/components/MD3Button.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include <cmath>
#include <algorithm>

namespace wx_md3 {

    wxDEFINE_EVENT(wxEVT_MD3_BUTTON_CLICKED, wxCommandEvent);

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3Button, MD3Control)
        EVT_PAINT(MD3Button::OnPaint)
        EVT_LEFT_DOWN(MD3Button::OnMouseLeftDown)
        EVT_LEFT_UP(MD3Button::OnMouseLeftUp)
        EVT_ENTER_WINDOW(MD3Button::OnMouseEnter)
        EVT_LEAVE_WINDOW(MD3Button::OnMouseLeave)
        EVT_SIZE(MD3Button::OnSize)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Button, MD3Control)

    // Constructor
    MD3Button::MD3Button() {
        Init();
    }

    MD3Button::MD3Button(wxWindow* parent, wxWindowID id,
                        const wxString& label,
                        const wxPoint& pos, const wxSize& size,
                        long style, const wxValidator& validator,
                        const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        Init();           // 🔧 先调用 Init
        m_label = label;  // 🔧 再设置 label（这样不会被 Init 覆盖）
    }

    // Initialization Function
    void MD3Button::Init() {
        m_label = wxEmptyString;
        m_variant = MD3ButtonVariant::Elevated;
        m_cornerRadius = 4; // Default MD3 corner radius
        m_elevation = 1;    // Default elevation
        m_currentElevation = m_elevation;
        m_iconBeforeText = true;
        m_rippleRadius = 0.0f;  // ✨ 初始化涟漪
        m_rippleCenter = wxPoint(0, 0);

        // Set window style
        SetBackgroundStyle(wxBG_STYLE_PAINT);

        // 🔧 设置字体 - 确保文字能显示
        wxFont font = GetFont();
        font.SetPointSize(12);  // 增大字体从 14 改为 12（因为之前可能太大）
        font.SetWeight(wxFONTWEIGHT_BOLD);
        SetFont(font);

        // Initialize appearance
        UpdateAppearance();
    }

    // Destructor
    MD3Button::~MD3Button() {
        // Cleanup resources
    }

    // Button properties
    void MD3Button::SetLabel(const wxString& label) {
        m_label = label;
        Refresh();
    }

    void MD3Button::SetVariant(MD3ButtonVariant variant) {
        if (m_variant != variant) {
            m_variant = variant;
            UpdateAppearance();
            Refresh();
        }
    }

    void MD3Button::SetIcon(const wxBitmap& icon) {
        m_icon = icon;
        Refresh();
    }

    // Override MD3Control methods
    void MD3Button::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();

        // ✨ 使用动画处理阴影变化
        auto animator = &MD3Animator::GetInstance();
        int targetElevation = m_elevation;
        
        // Handle elevation changes based on state
        switch (state) {
            case MD3State::Pressed:
                targetElevation = std::max(0, m_elevation - 1);
                break;
            case MD3State::Hover:
                targetElevation = m_elevation + 1;
                break;
            case MD3State::Disabled:
                targetElevation = 0;
                break;
            default:
                targetElevation = m_elevation;
                break;
        }
        
        // 如果目标阴影值与当前不同，创建动画
        if (targetElevation != m_currentElevation) {
            m_elevationAnimation = animator->CreatePropertyAnimation<int>(
                MD3AnimationType::Elevation,
                &m_currentElevation,
                m_currentElevation,
                targetElevation,
                200,  // 200ms 阴影过渡
                MD3Easing::EaseInOut
            );
            
            m_elevationAnimation->SetOnUpdateCallback([this]() {
                Refresh();
            });
            
            animator->Start();
        }

        Refresh();
    }

    wxSize MD3Button::DoGetBestSize() const {
        // Start with a reasonable default size
        wxSize size(80, 40); // MD3 standard button size (稍高一些)

        // Add space for icon if present
        if (m_icon.IsOk()) {
            size.x += m_icon.GetWidth() + 8; // 8px spacing between icon and text
        }

        // Add space for label
        if (!m_label.IsEmpty()) {
            wxCoord textWidth, textHeight;
            wxScreenDC screenDC;  // 🔧 用 wxScreenDC 代替 wxClientDC，避免窗口未创建的问题
            screenDC.SetFont(GetFont());
            screenDC.GetTextExtent(m_label, &textWidth, &textHeight);
            size.x = std::max(size.x, textWidth + 32); // 16px padding on each side
            size.y = std::max(size.y, textHeight + 16); // 8px padding on top and bottom
        }

        return size;
    }

    // Event handling
    void MD3Button::OnPaint(wxPaintEvent& event) {
        // 尝试用普通 wxPaintDC 而非 wxBufferedPaintDC
        wxPaintDC dc(this);
        Render(dc);
    }

    void MD3Button::OnSize(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }

    void MD3Button::OnMouseLeftDown(wxMouseEvent& event) {
        if (IsEnabled()) {
            // ✨ 创建涟漪动画
            m_rippleCenter = event.GetPosition();
            m_rippleRadius = 0.0f;
            
            auto animator = &MD3Animator::GetInstance();
            m_rippleAnimation = animator->CreatePropertyAnimation<float>(
                MD3AnimationType::Ripple,
                &m_rippleRadius,
                0.0f,
                1.0f,
                300,  // 300ms 涟漪扩散
                MD3Easing::Linear
            );
            
            m_rippleAnimation->SetOnUpdateCallback([this]() {
                Refresh();
            });
            
            m_rippleAnimation->SetOnCompleteCallback([this]() {
                m_rippleRadius = 0.0f;  // 重置涟漪
                Refresh();
            });
            
            animator->Start();
            
            MD3Control::OnMouseLeftDown(event);

            // Send button clicked event
            wxCommandEvent buttonEvent(wxEVT_MD3_BUTTON_CLICKED, GetId());
            buttonEvent.SetEventObject(this);
            ProcessEvent(buttonEvent);
        }
        event.Skip(false); // Don't propagate to parent
    }

    void MD3Button::OnMouseLeftUp(wxMouseEvent& event) {
        if (IsEnabled()) {
            MD3Control::OnMouseLeftUp(event);
        }
        event.Skip(false);
    }

    void MD3Button::OnMouseEnter(wxMouseEvent& event) {
        if (IsEnabled()) {
            MD3Control::OnMouseEnter(event);
        }
        event.Skip(false);
    }

    void MD3Button::OnMouseLeave(wxMouseEvent& event) {
        MD3Control::OnMouseLeave(event);
        event.Skip(false);
    }

    // Internal methods
    void MD3Button::UpdateAppearance() {
        // Update colors based on theme and variant
        SetBackgroundColour(GetBackgroundColor());
        SetForegroundColour(GetForegroundColor());
    }

    void MD3Button::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        auto theme = MD3Theme::GetCurrentTheme();
        
        // Get the current button appearance properties
        wxColour bgColor = GetBackgroundColor();
        wxColour fgColor = GetForegroundColor();
        wxColour borderColor = GetBorderColor();

        // 🔧 首先绘制背景（清除之前的内容）
        wxColour clearColor = GetParent() ? GetParent()->GetBackgroundColour() : *wxWHITE;
        dc.SetBrush(wxBrush(clearColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

        // Draw button background with rounded corners using DC
        if (bgColor.IsOk() && bgColor.Alpha() > 0) {
            dc.SetBrush(wxBrush(bgColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), m_cornerRadius);
        }

        // ✨ 绘制涟漪效果（在背景上，文字下面）
        if (m_rippleRadius > 0.0f && m_rippleRadius < 1.0f && bgColor.IsOk() && bgColor.Alpha() > 0) {
            // 涟漪颜色：使用背景色与前景色的混合
            // 这样文字在涟漪区域会自然地与涟漪融合
            wxColour bgColor_for_ripple = bgColor;
            wxColour rippleColor = fgColor;
            
            // 计算混合颜色（背景 + 半透明前景）
            int alpha = static_cast<int>(255 * (1.0f - m_rippleRadius) * 0.5f);  // 50% 透明度
            
            // 混合算法：最终颜色 = 背景 + 前景 * alpha
            int r = static_cast<int>(bgColor_for_ripple.Red() * (1.0f - alpha/255.0f) + rippleColor.Red() * (alpha/255.0f));
            int g = static_cast<int>(bgColor_for_ripple.Green() * (1.0f - alpha/255.0f) + rippleColor.Green() * (alpha/255.0f));
            int b = static_cast<int>(bgColor_for_ripple.Blue() * (1.0f - alpha/255.0f) + rippleColor.Blue() * (alpha/255.0f));
            
            wxColour finalRippleColor(
                std::min(255, r),
                std::min(255, g),
                std::min(255, b)
            );
            
            // 计算涟漪的最大半径（从中心到角）
            float dx1 = static_cast<float>(m_rippleCenter.x);
            float dy1 = static_cast<float>(m_rippleCenter.y);
            float maxRadius = std::sqrt(dx1 * dx1 + dy1 * dy1);
            
            float dx2 = static_cast<float>(size.GetWidth() - m_rippleCenter.x);
            float dy2 = static_cast<float>(size.GetHeight() - m_rippleCenter.y);
            float maxRadius2 = std::sqrt(dx2 * dx2 + dy2 * dy2);
            
            maxRadius = std::max(maxRadius, maxRadius2);
            
            // 绘制涟漪圆形
            int currentRadius = static_cast<int>(maxRadius * m_rippleRadius);
            if (currentRadius > 0) {
                dc.SetBrush(wxBrush(finalRippleColor));
                dc.SetPen(*wxTRANSPARENT_PEN);
                dc.DrawCircle(m_rippleCenter.x, m_rippleCenter.y, currentRadius);
            }
        }

        // Draw button border for outlined variant
        if (m_variant == MD3ButtonVariant::Outlined) {
            dc.SetPen(wxPen(borderColor, 1));
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), m_cornerRadius);
        }

        // 🔧 绘制所有形状后，重新设置文字的DC状态（因为前面的操作可能污染了状态）
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(*wxTRANSPARENT_PEN);

        // Draw icon and text
        int textX = 12; // Left padding
        int centerY = size.GetHeight() / 2;

        if (m_icon.IsOk()) {
            int iconY = centerY - m_icon.GetHeight() / 2;
            if (m_iconBeforeText) {
                dc.DrawBitmap(m_icon, textX, iconY, true);
                textX += m_icon.GetWidth() + 8; // 8px spacing
            }
        }

        if (!m_label.IsEmpty()) {
            // 🔧 确保文字颜色有效 - 强制使用可见颜色
            if (!fgColor.IsOk() || fgColor.Alpha() < 50) {
                fgColor = *wxBLACK;  // 直接用黑色作为后备
            }
            
            // 🔧 确保DC状态正确
            dc.SetFont(GetFont());
            dc.SetTextForeground(fgColor);
            
            wxCoord textWidth, textHeight;
            dc.GetTextExtent(m_label, &textWidth, &textHeight);
            int textY = centerY - (textHeight / 2);

            // Ensure text position is within bounds
            if (textX < 0) textX = 0;
            if (textY < 0) textY = 0;

            dc.DrawText(m_label, textX, textY);
        }
    }

    wxColour MD3Button::GetBackgroundColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        wxColour bgColor;

        switch (m_variant) {
            case MD3ButtonVariant::Filled:
                bgColor = theme->GetColor("primary");
                break;
            case MD3ButtonVariant::Elevated:
                bgColor = theme->GetColor("surface");
                break;
            case MD3ButtonVariant::Outlined:
            case MD3ButtonVariant::Text:
            default:
                // Outlined and Text buttons should have transparent background
                // Use surface color with alpha for hover/pressed states
                if (m_state == MD3State::Hover || m_state == MD3State::Pressed) {
                    bgColor = theme->GetColor("surface");
                    // Make it slightly transparent for hover/pressed states
                    bgColor = theme->AdjustAlpha(bgColor, 0.08);
                } else {
                    // Fully transparent for normal state
                    bgColor = wxColour(0, 0, 0, 0);
                }
                return bgColor;
        }

        // Adjust color based on state for Filled and Elevated buttons
        switch (m_state) {
            case MD3State::Pressed:
                return theme->Darken(bgColor, 0.2);
            case MD3State::Hover:
                return theme->Lighten(bgColor, 0.1);
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            default:
                return bgColor;
        }
    }

    wxColour MD3Button::GetForegroundColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        wxColour color;

        switch (m_variant) {
            case MD3ButtonVariant::Filled:
                color = theme->GetColor("onPrimary");
                break;
            case MD3ButtonVariant::Elevated:
                color = theme->GetColor("onSurface");
                break;
            case MD3ButtonVariant::Outlined:
            case MD3ButtonVariant::Text:
            default:
                // For outlined and text buttons, use primary color for normal state
                // and on-surface color for disabled state
                if (m_state == MD3State::Disabled) {
                    color = theme->GetColor("onSurfaceVariant");
                } else {
                    color = theme->GetColor("primary");
                }
                break;
        }

        // 🔧 确保颜色有效且可见 - 加强检查
        if (!color.IsOk() || 
            (color.Red() == 0 && color.Green() == 0 && color.Blue() == 0 && color.Alpha() == 0) ||
            color.Alpha() < 50) {  // 如果透明度太低也不行
            // 回退到确定有效的颜色
            if (m_variant == MD3ButtonVariant::Filled) {
                color = *wxWHITE;
            } else {
                // 🔧 对于非填充按钮，直接使用黑色，而不是依赖主题
                color = *wxBLACK;
            }
        }

        return color;
    }

    wxColour MD3Button::GetBorderColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        return theme->GetColor("outline");
    }

} // namespace wx_md3