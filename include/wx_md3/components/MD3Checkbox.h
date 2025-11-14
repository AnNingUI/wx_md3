#ifndef MD3CHECKBOX_H
#define MD3CHECKBOX_H

#include "wx_md3/core/MD3Control.h"
#include <wx/checkbox.h>

namespace wx_md3 {

    // MD3 Checkbox class
    class MD3Checkbox : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3Checkbox)

    public:
        // Constructors
        MD3Checkbox();
        MD3Checkbox(wxWindow* parent, wxWindowID id = wxID_ANY,
                    const wxString& label = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = wxCheckBoxNameStr);

        // Destructor
        virtual ~MD3Checkbox();

        // Checkbox properties
        void SetValue(bool value);
        bool GetValue() const { return m_checked; }

        void SetLabel(const wxString& label) override;
        wxString GetLabel() const override { return m_label; }

        // MD3-specific properties
        void SetCheckboxSize(int size) { m_size = size; }
        int GetCheckboxSize() const { return m_size; }

        void SetStrokeWidth(int width) { m_strokeWidth = width; }
        int GetStrokeWidth() const { return m_strokeWidth; }

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
        virtual wxColour GetCheckColor() const;
        virtual wxColour GetBorderColor() const;

        // Checkbox state properties
        wxString m_label;
        bool m_checked;
        bool m_hovered;
        int m_size;         // Checkbox size (typically 24px)
        int m_strokeWidth;  // Border stroke width
        float m_checkProgress; // Animation progress 0.0 to 1.0
        std::shared_ptr<MD3PropertyAnimation<float>> m_checkAnimation;  // Checkmark animation

    private:
        void Init();
        void DrawCheckmark(wxDC& dc, int x, int y, float progress);

        wxDECLARE_EVENT_TABLE();
    };

    // MD3 Checkbox event
    wxDECLARE_EVENT(wxEVT_MD3_CHECKBOX_TOGGLED, wxCommandEvent);

} // namespace wx_md3

#endif // MD3CHECKBOX_H
