#ifndef MD3RADIOBUTTON_H
#define MD3RADIOBUTTON_H

#include "wx_md3/core/MD3Control.h"
#include <wx/radiobut.h>

namespace wx_md3 {

    // MD3 RadioButton class
    class MD3RadioButton : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3RadioButton)

    public:
        // Constructors
        MD3RadioButton();
        MD3RadioButton(wxWindow* parent, wxWindowID id = wxID_ANY,
                       const wxString& label = wxEmptyString,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = 0,
                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxRadioButtonNameStr);

        // Destructor
        virtual ~MD3RadioButton();

        // RadioButton properties
        void SetValue(bool value);
        bool GetValue() const { return m_selected; }

        void SetLabel(const wxString& label) override;
        wxString GetLabel() const override { return m_label; }

        // MD3-specific properties
        void SetRadioSize(int size) { m_size = size; }
        int GetRadioSize() const { return m_size; }

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
        virtual wxColour GetRadioColor() const;
        virtual wxColour GetBorderColor() const;

        // RadioButton state properties
        wxString m_label;
        bool m_selected;
        bool m_hovered;
        int m_size;         // Radio button size (typically 24px)
        int m_strokeWidth;  // Border stroke width
        float m_fillProgress; // Animation progress 0.0 to 1.0

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

    // MD3 RadioButton event
    wxDECLARE_EVENT(wxEVT_MD3_RADIOBUTTON_SELECTED, wxCommandEvent);

} // namespace wx_md3

#endif // MD3RADIOBUTTON_H
