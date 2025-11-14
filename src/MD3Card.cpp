#include "wx_md3/components/MD3Card.h"
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <wx/log.h>
#include <cmath>
#include <algorithm>

namespace wx_md3 {

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3Card, MD3Control)
        EVT_PAINT(MD3Card::OnPaint)
        EVT_ENTER_WINDOW(MD3Card::OnMouseEnter)
        EVT_LEAVE_WINDOW(MD3Card::OnMouseLeave)
        EVT_SIZE(MD3Card::OnSize)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Card, MD3Control)

    // Constructor
    MD3Card::MD3Card() {
        Init();
    }

    MD3Card::MD3Card(wxWindow* parent, wxWindowID id,
                    const wxPoint& pos, const wxSize& size,
                    long style, const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        Init();
    }

    // Initialization Function
    void MD3Card::Init() {
        m_variant = MD3CardVariant::Elevated;
        m_cornerRadius = 12; // Default MD3 card corner radius
        m_elevation = 1;     // Default elevation
        m_currentElevation = m_elevation;

        // Set window style - always use wxBG_STYLE_PAINT for consistent behavior
        // wxBG_STYLE_PAINT ensures we have full control over the painting process
        SetBackgroundStyle(wxBG_STYLE_PAINT);

        // Initialize appearance
        UpdateAppearance();
    }

    // Destructor
    MD3Card::~MD3Card() {
        // Cleanup resources
    }

    // Card properties
    void MD3Card::SetVariant(MD3CardVariant variant) {
        if (m_variant != variant) {
            m_variant = variant;
            UpdateAppearance();
            Refresh();
        }
    }

    // Override MD3Control methods
    void MD3Card::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();

        // Handle elevation changes based on state
        auto animator = &MD3Animator::GetInstance();
        int targetElevation = m_elevation;

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

        // If target elevation is different from current, create animation
        if (targetElevation != m_currentElevation) {
            m_elevationAnimation = animator->CreatePropertyAnimation<int>(
                MD3AnimationType::Elevation,
                &m_currentElevation,
                m_currentElevation,
                targetElevation,
                200,  // 200ms elevation transition
                MD3Easing::EaseInOut
            );

            m_elevationAnimation->SetOnUpdateCallback([this]() {
                Refresh();
            });

            animator->Start();
        }

        Refresh();
    }

    wxSize MD3Card::DoGetBestSize() const {
        // If we have a sizer, use its best size
        if (GetSizer()) {
            return GetSizer()->GetMinSize();
        }

        // Otherwise, return a reasonable default size for a card
        wxSize size(200, 120); // MD3 standard card size
        return size;
    }

    // Helper: draw parent background - improved for Win32 compatibility
    static void DrawParentBackgroundFallback(wxWindow* win, wxDC& dc, const wxRect& rect) {
        if (!win) return;

        wxWindow* parent = win->GetParent();
        if (!parent) {
            // No parent window: fill with default background color
            dc.SetBrush(wxBrush(win->GetBackgroundColour()));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
            return;
        }

        // Get parent's background color
        wxColour parentBgColor = parent->GetBackgroundColour();
        
        // If parent has a valid background color, use it
        if (parentBgColor.IsOk()) {
            dc.SetBrush(wxBrush(parentBgColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
        } else {
            // Fallback to system background color
            dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
        }
    }

    // Event handling
    void MD3Card::OnPaint(wxPaintEvent& event) {
        // Use wxAutoBufferedPaintDC to eliminate flickering
        wxAutoBufferedPaintDC dc(this);
        Render(dc);
    }

    void MD3Card::OnSize(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }

    void MD3Card::OnMouseEnter(wxMouseEvent& event) {
        if (IsEnabled()) {
            MD3Control::OnMouseEnter(event);
        }
        event.Skip();
    }

    void MD3Card::OnMouseLeave(wxMouseEvent& event) {
        MD3Control::OnMouseLeave(event);
        event.Skip();
    }

    // Internal methods
    void MD3Card::UpdateAppearance() {
        // Update colors based on theme and variant
        SetBackgroundColour(GetBackgroundColor());
        SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    }

    void MD3Card::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        auto theme = MD3Theme::GetCurrentTheme();

        // First draw parent background (clear previous content)
        wxRect rect(0, 0, size.GetWidth(), size.GetHeight());
        DrawParentBackgroundFallback(this, dc, rect);

        // Get card appearance properties
        wxColour bgColor = GetBackgroundColor();
        wxColour borderColor = GetBorderColor();

        // Draw card background with rounded corners
        if (bgColor.IsOk() && bgColor.Alpha() > 0) {
            dc.SetBrush(wxBrush(bgColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), m_cornerRadius);
        }

        // Draw card border for outlined variant
        if (m_variant == MD3CardVariant::Outlined) {
            dc.SetPen(wxPen(borderColor, 1));
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRoundedRectangle(0, 0, size.GetWidth(), size.GetHeight(), m_cornerRadius);
        }
    }

    wxColour MD3Card::GetBackgroundColor() const {
        auto theme = MD3Theme::GetCurrentTheme();
        wxColour bgColor;

        switch (m_variant) {
            case MD3CardVariant::Filled:
                bgColor = theme->GetColor("surface");
                break;
            case MD3CardVariant::Elevated:
                bgColor = theme->GetColor("surface");
                break;
            case MD3CardVariant::Outlined:
            default:
                // Outlined cards should have transparent background
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

        // Adjust color based on state
        switch (m_state) {
            case MD3State::Pressed:
                return theme->Darken(bgColor, 0.1);
            case MD3State::Hover:
                return theme->Lighten(bgColor, 0.05);
            case MD3State::Disabled:
                return theme->GetColor("surfaceVariant");
            default:
                return bgColor;
        }
    }

    wxColour MD3Card::GetBorderColor() const {
        auto theme = MD3Theme::GetCurrentTheme();

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