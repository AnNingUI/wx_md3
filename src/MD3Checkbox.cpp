// MD3Checkbox.cpp
#include "wx_md3/components/MD3Checkbox.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Animator.h"

#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <algorithm>          // std::min, std::max

namespace wx_md3 {

    wxDEFINE_EVENT(wxEVT_MD3_CHECKBOX_TOGGLED, wxCommandEvent);

    // 空事件表（事件通过 Bind 处理）
    wxBEGIN_EVENT_TABLE(MD3Checkbox, MD3Control)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Checkbox, MD3Control)

    // Constructor
    MD3Checkbox::MD3Checkbox() {
        Init();
    }

    MD3Checkbox::MD3Checkbox(wxWindow* parent, wxWindowID id,
                             const wxString& label,
                             const wxPoint& pos, const wxSize& size,
                             long style, const wxValidator& validator,
                             const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        m_label = label;
        Init();
    }

    // Initialization
    void MD3Checkbox::Init() {
        m_checked = false;
        m_hovered = false;
        m_size = 24;        // MD3 standard checkbox size
        m_strokeWidth = 2;
        m_checkProgress = 0.0f;

        // 🔧 设置字体 - 确保文字能显示
        wxFont font = GetFont();
        font.SetPointSize(11);
        SetFont(font);

        // 使用透明背景策略，让我们复制父窗口真实绘制内容
        if (!GetHandle()) {
            SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
        } else {
            SetBackgroundStyle(wxBG_STYLE_PAINT);
        }
        UpdateAppearance();
        
        // ✅ 使用 Bind 动态绑定事件（优先于事件表）
        Bind(wxEVT_PAINT, &MD3Checkbox::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &MD3Checkbox::OnMouseLeftDown, this);
        Bind(wxEVT_LEFT_UP, &MD3Checkbox::OnMouseLeftUp, this);
        Bind(wxEVT_ENTER_WINDOW, &MD3Checkbox::OnMouseEnter, this);
        Bind(wxEVT_LEAVE_WINDOW, &MD3Checkbox::OnMouseLeave, this);
    }

    // Destructor
    MD3Checkbox::~MD3Checkbox() {
        // Cleanup resources
    }

    // Set checked state
    void MD3Checkbox::SetValue(bool value) {
        if (m_checked != value) {
            m_checked = value;
            
            // ✨ 使用 MD3Animator 创建流畅的勾线动画
            float targetProgress = value ? 1.0f : 0.0f;
            float startProgress = m_checkProgress;
            
            // 创建动画对象
            auto animator = &MD3Animator::GetInstance();
            m_checkAnimation = animator->CreatePropertyAnimation<float>(
                MD3AnimationType::ScaleFade,
                &m_checkProgress,
                startProgress,
                targetProgress,
                400,  // 400ms 动画时长 - 比 switch 稍长一些
                MD3Easing::EaseInOut
            );
            
            // 设置动画更新回调 - 每帧刷新界面
            m_checkAnimation->SetOnUpdateCallback([this]() {
                Refresh();
                Update();
            });
            
            // 动画完成回调
            m_checkAnimation->SetOnCompleteCallback([this]() {
                Refresh();
                Update();
            });
            
            // 启动动画
            animator->Start();
        }
    }

    void MD3Checkbox::SetLabel(const wxString& label) {
        m_label = label;
        Refresh();
    }

    // Override SetState
    void MD3Checkbox::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();
        Refresh();
    }

    wxSize MD3Checkbox::DoGetBestSize() const {
        wxSize size(m_size + 8, m_size + 8); // 4px padding on each side

        if (!m_label.IsEmpty()) {
            wxCoord textWidth = 0, textHeight = 0;
            // 在 const 环境下安全地使用屏幕 DC 来计算文字尺寸（兼容性高）
            wxScreenDC screenDC;
            screenDC.SetFont(GetFont());
            screenDC.GetTextExtent(m_label, &textWidth, &textHeight);
            size.x += textWidth + 12; // 8px spacing between checkbox and label
            size.y = std::max(size.y, textHeight + 8);
        }

        return size;
    }

    // Event handling
    void MD3Checkbox::OnPaint(wxPaintEvent& event) {
        // 双缓冲绘制
        wxBufferedPaintDC dc(this);
        Render(dc);
    }

    void MD3Checkbox::OnMouseLeftDown(wxMouseEvent& event) {
        if (IsEnabled()) {
            SetState(MD3State::Pressed);
            event.Skip(false);  // 不传播事件
        } else {
            event.Skip();
        }
    }

    void MD3Checkbox::OnMouseLeftUp(wxMouseEvent& event) {
        wxPoint pos = event.GetPosition();
        wxSize sz = GetClientSize();
        
        if (IsEnabled()) {
            // Check if mouse is still over the control
            if (pos.x >= 0 && pos.x < sz.GetWidth() &&
                pos.y >= 0 && pos.y < sz.GetHeight()) {
                
                // Toggle checkbox
                SetValue(!m_checked);
                
                // Send toggle event
                wxCommandEvent checkEvent(wx_md3::wxEVT_MD3_CHECKBOX_TOGGLED, GetId());
                checkEvent.SetInt(m_checked ? 1 : 0);
                checkEvent.SetEventObject(this);
                ProcessEvent(checkEvent);
                
                SetState(MD3State::Hover);
                
                event.Skip(false);  // 消费事件，不传播
                return;
            }
        }
        event.Skip();  // 如果不在范围内，传播事件
    }

    void MD3Checkbox::OnMouseEnter(wxMouseEvent& event) {
        if (IsEnabled()) {
            m_hovered = true;
            SetState(MD3State::Hover);
        }
        event.Skip();
    }

    void MD3Checkbox::OnMouseLeave(wxMouseEvent& event) {
        m_hovered = false;
        SetState(MD3State::Normal);
        event.Skip();
    }

    // Internal methods
    void MD3Checkbox::UpdateAppearance() {
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

        // 方案：从父窗口的客户端 DC 把对应区域 Blit 到内存位图，再绘制到目标 dc。
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

            // 尝试 blit：源点是控件在父窗口坐标系的位置
            // 注意：wxWidgets 在高 DPI 上可能需要考虑缩放；这里使用默认行为
            memDC.Blit(0, 0, w, h, &parentDC, topleftInParent.x, topleftInParent.y, wxCOPY, true);

            memDC.SelectObject(wxNullBitmap);
        }

        // 把位图绘制到目标 dc（相对于控件）
        dc.DrawBitmap(bmp, rect.GetX(), rect.GetY(), false);
    }

    void MD3Checkbox::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        auto theme = MD3Theme::GetCurrentTheme();

        // 关键：先把父窗口当前的可见内容绘制到我们的 dc（支持复杂父背景）
        wxRect rect(0, 0, size.GetWidth(), size.GetHeight());
        DrawParentBackgroundFallback(this, dc, rect);

        // 复选框位置
        int checkboxX = 4;
        int checkboxY = (size.GetHeight() - m_size) / 2;
        
        // 确定颜色（保持主题）
        wxColour checkColor = GetCheckColor();
        wxColour borderColor = GetBorderColor();
        wxColour checkboxBg;
        
        if (m_checked) {
            // ✅ 勾选：使用主题的 primary 色
            checkboxBg = checkColor;
        } else {
            // ❌ 未勾选：浅色或透明
            if (m_state == MD3State::Hover) {
                checkboxBg = theme->GetColor("surfaceVariant");
            } else {
                // 不再强制填成父背景色的单色（我们已经把父背景绘制到 DC），
                // 若需要透明效果直接使用父背景色作为 fallback
                if (GetParent()) {
                    checkboxBg = GetParent()->GetBackgroundColour();
                } else {
                    checkboxBg = theme->GetColor("surface"); // fallback
                }
            }
        }
        
        // 绘制复选框背景（圆角矩形）
        dc.SetBrush(wxBrush(checkboxBg));
        dc.SetPen(wxPen(borderColor, 2));
        dc.DrawRoundedRectangle(checkboxX, checkboxY, m_size, m_size, 2);
        
        // 绘制勾线（如果勾选或动画中）
        if (m_checked || m_checkProgress > 0.0f) {
            DrawCheckmark(dc, checkboxX, checkboxY, m_checkProgress);
        }
        
        // 绘制标签文本
        if (!m_label.IsEmpty()) {
            int labelX = checkboxX + m_size + 8;
            // 使用 DC 的字符高度来垂直居中
            int ch = dc.GetCharHeight();
            int labelY = (size.GetHeight() - ch) / 2;
            
            dc.SetTextForeground(theme->GetColor("onSurface"));
            dc.SetFont(GetFont());
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetPen(*wxTRANSPARENT_PEN);
            
            dc.DrawText(m_label, labelX, labelY);
        }
    }

    void MD3Checkbox::DrawCheckmark(wxDC& dc, int x, int y, float progress) {
        // 绘制动画勾线（保持主题色，但确保可见）
        auto theme = MD3Theme::GetCurrentTheme();
        
        // 勾线用 onPrimary 色，如果看不清就用黑色
        wxColour checkmarkColor = theme->GetColor("onPrimary");
        
        // 安全检查：确保勾线可见
        int r = checkmarkColor.Red();
        int g = checkmarkColor.Green();
        int b = checkmarkColor.Blue();
        int brightness = (r * 299 + g * 587 + b * 114) / 1000;
        
        // 如果太亮（> 200），用黑色；如果太暗，用白色
        if (brightness > 200) {
            checkmarkColor = *wxBLACK;
        } else if (brightness < 50) {
            checkmarkColor = *wxWHITE;
        }
        
        // Material Design 3 风格的勾 - 更优雅
        // 勾的起点、中点、终点坐标（相对于复选框内部）
        float cx = x + m_size / 2.0f;   // 中心 X
        float cy = y + m_size / 2.0f;   // 中心 Y
        float r_val = m_size / 2.2f;    // 半径
        
        // 起点（左下）
        float x1 = cx - r_val * 0.35f;
        float y1 = cy + r_val * 0.15f;
        
        // 中点（下方）
        float x2 = cx - r_val * 0.05f;
        float y2 = cy + r_val * 0.35f;
        
        // 终点（右上）
        float x3 = cx + r_val * 0.45f;
        float y3 = cy - r_val * 0.35f;
        
        // 用粗线绘制（3px），更精致
        dc.SetPen(wxPen(checkmarkColor, 3, wxPENSTYLE_SOLID));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        
        // 第一段：从起点到中点（进度 0-0.5）
        if (progress > 0.0f) {
            float p1 = std::min(1.0f, progress * 2.0f);  // 0 -> 1 when progress goes 0 -> 0.5
            
            float x1_current = x1 + (x2 - x1) * p1;
            float y1_current = y1 + (y2 - y1) * p1;
            dc.DrawLine((int)x1, (int)y1, (int)x1_current, (int)y1_current);
        }
        
        // 第二段：从中点到终点（进度 0.5-1.0）
        if (progress > 0.5f) {
            float p2 = std::min(1.0f, (progress - 0.5f) * 2.0f);  // 0 -> 1 when progress goes 0.5 -> 1.0
            
            float x2_current = x2 + (x3 - x2) * p2;
            float y2_current = y2 + (y3 - y2) * p2;
            dc.DrawLine((int)x2, (int)y2, (int)x2_current, (int)y2_current);
        }
    }

    wxColour MD3Checkbox::GetCheckColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        switch (m_state) {
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            case MD3State::Pressed: {
                wxColour primary = theme->GetColor("primary");
                return wxColour(
                    std::min(255, (int)primary.Red() + 30),
                    std::min(255, (int)primary.Green() + 30),
                    std::min(255, (int)primary.Blue() + 30)
                );
            }
            default: {
                // 返回亮蓝色而不是深蓝色
                wxColour primary = theme->GetColor("primary");
                int r = primary.Red();
                int g = primary.Green();
                int b = primary.Blue();
                // 增加亮度 50%
                return wxColour(
                    std::min(255, r + (255 - r) / 2),
                    std::min(255, g + (255 - g) / 2),
                    std::min(255, b + (255 - b) / 2)
                );
            }
        }
    }

    wxColour MD3Checkbox::GetBorderColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        
        if (m_checked) {
            return GetCheckColor();
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
