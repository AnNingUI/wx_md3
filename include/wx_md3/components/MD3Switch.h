#ifndef MD3SWITCH_H
#define MD3SWITCH_H

#include "wx_md3/core/MD3Control.h"
#include <wx/radiobut.h>

namespace wx_md3 {

    // MD3 Switch class
    class MD3Switch : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3Switch)

    public:
        // Constructors
        MD3Switch();
        MD3Switch(wxWindow* parent, wxWindowID id = wxID_ANY,
                  const wxString& label = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxRadioButtonNameStr);

        // Destructor
        virtual ~MD3Switch();

        // Switch properties
        void SetValue(bool value);
        bool GetValue() const { return m_enabled; }

        void SetLabel(const wxString& label) override;
        wxString GetLabel() const override { return m_label; }

        // MD3-specific properties
        void SetThumbSize(int size) { m_thumbSize = size; }
        int GetThumbSize() const { return m_thumbSize; }

        void SetTrackHeight(int height) { m_trackHeight = height; }
        int GetTrackHeight() const { return m_trackHeight; }

        // Override MD3Control methods
        virtual void SetState(MD3State state) override;
        virtual void Render(wxDC& dc) override;
        virtual wxSize DoGetBestSize() const override;

        // Event handling
        virtual void OnPaint(wxPaintEvent& event) override;
        virtual void OnMouseLeftDown(wxMouseEvent& event) override;
        virtual void OnMouseLeftUp(wxMouseEvent& event) override;
        virtual void OnMouseEnter(wxMouseEvent& event) override;
        virtual void OnMouseLeave(wxMouseEvent& event) override;

    protected:
        // Internal methods
        virtual void UpdateAppearance();
        virtual wxColour GetTrackColor() const;
        virtual wxColour GetThumbColor() const;

        // Switch state properties
        wxString m_label;
        bool m_enabled;
        bool m_hovered;
        int m_thumbSize;    // Thumb circle size (typically 24px)
        int m_trackHeight;  // Track height (typically 28px)
        float m_slideProgress; // Animation progress 0.0 to 1.0
        std::shared_ptr<MD3PropertyAnimation<float>> m_slideAnimation;  // Thumb slide animation

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

    // MD3 Switch event
    wxDECLARE_EVENT(wxEVT_MD3_SWITCH_TOGGLED, wxCommandEvent);

} // namespace wx_md3

#endif // MD3SWITCH_H
