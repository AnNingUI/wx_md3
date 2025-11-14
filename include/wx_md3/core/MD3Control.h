#ifndef MD3CONTROL_H
#define MD3CONTROL_H

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/dc.h>
#include <array>
#include "wx_md3/core/MD3Animator.h"

namespace wx_md3 {

    // MD3 Control State Enum
    enum class MD3State {
        Normal,
        Hover,
        Pressed,
        Disabled,
        Focused,
        Error
    };

    // MD3 Control Base Class
    class MD3Control : public wxWindow {
        DECLARE_DYNAMIC_CLASS(MD3Control)

    public:
        // Constructor
        MD3Control();
        MD3Control(wxWindow* parent, wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxString& name = wxEmptyString);

        // Destructor
        virtual ~MD3Control();

        // State Management
        virtual void SetState(MD3State state);
        MD3State GetState() const { return m_state; }
        virtual bool IsEnabled() const { return m_state != MD3State::Disabled; }
        virtual bool Enable(bool enable = true);

        // Theme Support
        virtual void SetTheme(const wxString& theme);
        wxString GetTheme() const { return m_theme; }

        // Animation Support
        virtual void StartAnimation(MD3AnimationType animationType);
        virtual void StopAnimation(MD3AnimationType animationType);
        virtual bool IsAnimating(MD3AnimationType animationType) const;

        // Backward compatibility (string-based)
        virtual void StartAnimation(const wxString& animationType);
        virtual void StopAnimation(const wxString& animationType);
        virtual bool IsAnimating(const wxString& animationType) const;

        // Event Handling
        virtual void BindEvents();

    protected:
        // Drawing Functions
        virtual wxSize DoGetBestSize() const;
        virtual void OnPaint(wxPaintEvent& event);
        virtual void OnSize(wxSizeEvent& event);
        virtual void OnMouseEnter(wxMouseEvent& event);
        virtual void OnMouseLeave(wxMouseEvent& event);
        virtual void OnMouseLeftDown(wxMouseEvent& event);
        virtual void OnMouseLeftUp(wxMouseEvent& event);
        virtual void OnSetFocus(wxFocusEvent& event);
        virtual void OnKillFocus(wxFocusEvent& event);

        // State Variables
        MD3State m_state;
        wxString m_theme;
        std::array<bool, static_cast<size_t>(MD3AnimationType::Count)> m_animations;

        // Internal Methods
        virtual void UpdateState();
        virtual void Render(wxDC& dc); // Virtual function with default implementation

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

} // namespace wx_md3

#endif // MD3CONTROL_H