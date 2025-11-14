#include "wx_md3/core/MD3Control.h"
#include <wx/dcbuffer.h>
#include <wx/log.h>

namespace wx_md3 {

    wxDEFINE_EVENT(wxEVT_MD3_STATE_CHANGED, wxCommandEvent);

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3Control, wxWindow)
        EVT_PAINT(MD3Control::OnPaint)
        EVT_SIZE(MD3Control::OnSize)
        EVT_ENTER_WINDOW(MD3Control::OnMouseEnter)
        EVT_LEAVE_WINDOW(MD3Control::OnMouseLeave)
        EVT_LEFT_DOWN(MD3Control::OnMouseLeftDown)
        EVT_LEFT_UP(MD3Control::OnMouseLeftUp)
        EVT_SET_FOCUS(MD3Control::OnSetFocus)
        EVT_KILL_FOCUS(MD3Control::OnKillFocus)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Control, wxWindow)

    // Constructor
    MD3Control::MD3Control() {
        Init();
    }

    MD3Control::MD3Control(wxWindow* parent, wxWindowID id,
                          const wxPoint& pos, const wxSize& size,
                          long style, const wxString& name)
        : wxWindow(parent, id, pos, size, style, name) {
        Init();
    }

    // Initialization Function
    void MD3Control::Init() {
        m_state = MD3State::Normal;
        m_theme = "light"; // Default theme

        // Initialize animation array to false
        m_animations.fill(false);

        BindEvents();

        // Set window style
        SetBackgroundStyle(wxBG_STYLE_PAINT);
    }

    // Destructor
    MD3Control::~MD3Control() {
        // Cleanup resources
    }

    // State Management
    void MD3Control::SetState(MD3State state) {
        if (m_state != state) {
            m_state = state;
            UpdateState();

            // Send state change event
            wxCommandEvent event(wxEVT_MD3_STATE_CHANGED, GetId());
            event.SetEventObject(this);
            ProcessEvent(event);

            Refresh();
        }
    }

    bool MD3Control::Enable(bool enable) {
        if (enable && m_state == MD3State::Disabled) {
            SetState(MD3State::Normal);
        } else if (!enable && m_state != MD3State::Disabled) {
            SetState(MD3State::Disabled);
        }
        return wxWindow::Enable(enable);
    }

    // Theme Support
    void MD3Control::SetTheme(const wxString& theme) {
        if (m_theme != theme) {
            m_theme = theme;
            Refresh();
        }
    }

    // Animation Support (enum-based - optimized)
    void MD3Control::StartAnimation(MD3AnimationType animationType) {
        if (static_cast<size_t>(animationType) < m_animations.size()) {
            m_animations[static_cast<size_t>(animationType)] = true;
            // Here you can add actual animation logic using MD3Animator
        }
    }

    void MD3Control::StopAnimation(MD3AnimationType animationType) {
        if (static_cast<size_t>(animationType) < m_animations.size()) {
            m_animations[static_cast<size_t>(animationType)] = false;
        }
    }

    bool MD3Control::IsAnimating(MD3AnimationType animationType) const {
        if (static_cast<size_t>(animationType) < m_animations.size()) {
            return m_animations[static_cast<size_t>(animationType)];
        }
        return false;
    }

    // Backward compatibility (string-based)
    void MD3Control::StartAnimation(const wxString& animationType) {
        // Convert string to enum (basic mapping for common animations)
        MD3AnimationType type = MD3AnimationType::Custom;
        if (animationType == "ripple") type = MD3AnimationType::Ripple;
        else if (animationType == "elevation") type = MD3AnimationType::Elevation;
        else if (animationType == "hover") type = MD3AnimationType::Hover;
        else if (animationType == "label_float") type = MD3AnimationType::LabelFloat;
        else if (animationType == "thumb_slide") type = MD3AnimationType::ThumbSlide;
        else if (animationType == "scale_fade") type = MD3AnimationType::ScaleFade;
        else if (animationType == "indicator_slide") type = MD3AnimationType::IndicatorSlide;
        else if (animationType == "cursor_blink") type = MD3AnimationType::CursorBlink;
        else if (animationType == "progress_animate") type = MD3AnimationType::ProgressAnimate;

        if (type != MD3AnimationType::Custom) {
            StartAnimation(type);
        } else {
            // For custom animations, we can't store them in the array
            // This is a limitation of the optimization, but custom animations
            // should be rare and can be handled differently
            wxLogWarning("Custom animation '%s' not supported in optimized storage", animationType);
        }
    }

    void MD3Control::StopAnimation(const wxString& animationType) {
        MD3AnimationType type = MD3AnimationType::Custom;
        if (animationType == "ripple") type = MD3AnimationType::Ripple;
        else if (animationType == "elevation") type = MD3AnimationType::Elevation;
        else if (animationType == "hover") type = MD3AnimationType::Hover;
        else if (animationType == "label_float") type = MD3AnimationType::LabelFloat;
        else if (animationType == "thumb_slide") type = MD3AnimationType::ThumbSlide;
        else if (animationType == "scale_fade") type = MD3AnimationType::ScaleFade;
        else if (animationType == "indicator_slide") type = MD3AnimationType::IndicatorSlide;
        else if (animationType == "cursor_blink") type = MD3AnimationType::CursorBlink;
        else if (animationType == "progress_animate") type = MD3AnimationType::ProgressAnimate;

        if (type != MD3AnimationType::Custom) {
            StopAnimation(type);
        } else {
            wxLogWarning("Custom animation '%s' not supported in optimized storage", animationType);
        }
    }

    bool MD3Control::IsAnimating(const wxString& animationType) const {
        MD3AnimationType type = MD3AnimationType::Custom;
        if (animationType == "ripple") type = MD3AnimationType::Ripple;
        else if (animationType == "elevation") type = MD3AnimationType::Elevation;
        else if (animationType == "hover") type = MD3AnimationType::Hover;
        else if (animationType == "label_float") type = MD3AnimationType::LabelFloat;
        else if (animationType == "thumb_slide") type = MD3AnimationType::ThumbSlide;
        else if (animationType == "scale_fade") type = MD3AnimationType::ScaleFade;
        else if (animationType == "indicator_slide") type = MD3AnimationType::IndicatorSlide;
        else if (animationType == "cursor_blink") type = MD3AnimationType::CursorBlink;
        else if (animationType == "progress_animate") type = MD3AnimationType::ProgressAnimate;

        if (type != MD3AnimationType::Custom) {
            return IsAnimating(type);
        } else {
            return false; // Custom animations not supported in optimized storage
        }
    }

    // Event Handling
    void MD3Control::BindEvents() {
        // Default event binding is already done in the event table
        // Subclasses can override this function to add more events
    }

    // Drawing Functions
    wxSize MD3Control::DoGetBestSize() const {
        // Subclasses should override this function to provide best size
        return wxWindow::DoGetBestSize();
    }

    void MD3Control::OnPaint(wxPaintEvent& event) {
        wxAutoBufferedPaintDC dc(this);
        Render(dc);
    }

    void MD3Control::OnSize(wxSizeEvent& event) {
        Refresh();
        event.Skip();
    }

    void MD3Control::OnMouseEnter(wxMouseEvent& event) {
        if (m_state == MD3State::Normal) {
            SetState(MD3State::Hover);
        }
        event.Skip();
    }

    void MD3Control::OnMouseLeave(wxMouseEvent& event) {
        if (m_state == MD3State::Hover) {
            SetState(MD3State::Normal);
        }
        event.Skip();
    }

    void MD3Control::OnMouseLeftDown(wxMouseEvent& event) {
        if (IsEnabled()) {
            SetState(MD3State::Pressed);
        }
        event.Skip();
    }

    void MD3Control::OnMouseLeftUp(wxMouseEvent& event) {
        if (IsEnabled()) {
            SetState(m_state == MD3State::Pressed ? MD3State::Hover : m_state);
        }
        event.Skip();
    }

    void MD3Control::OnSetFocus(wxFocusEvent& event) {
        if (IsEnabled()) {
            SetState(MD3State::Focused);
        }
        event.Skip();
    }

    void MD3Control::OnKillFocus(wxFocusEvent& event) {
        if (m_state == MD3State::Focused) {
            SetState(MD3State::Normal);
        }
        event.Skip();
    }

    // Internal Methods
    void MD3Control::UpdateState() {
        // State update logic
        // Subclasses can override this function to add specific state handling
    }

    // Render function with default implementation
    void MD3Control::Render(wxDC& dc) {
        // Default rendering - just clear the background
        dc.SetBackground(GetBackgroundColour());
        dc.Clear();

        // Subclasses should override this function to provide custom rendering
    }

} // namespace wx_md3