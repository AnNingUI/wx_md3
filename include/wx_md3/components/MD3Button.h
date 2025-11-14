#ifndef MD3BUTTON_H
#define MD3BUTTON_H

#include "wx_md3/core/MD3Control.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Animator.h"
#include <wx/button.h>

namespace wx_md3 {

    // MD3 Button variants
    enum class MD3ButtonVariant {
        Elevated,    // Default elevated button
        Filled,      // Filled button (primary color)
        Outlined,    // Outlined button
        Text         // Text-only button
    };

    // MD3 Button class
    class MD3Button : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3Button)

    public:
        // Constructors
        MD3Button();
        MD3Button(wxWindow* parent, wxWindowID id = wxID_ANY,
                  const wxString& label = wxEmptyString,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxButtonNameStr);

        // Destructor
        virtual ~MD3Button();

        // Button properties
        void SetLabel(const wxString& label) override;
        wxString GetLabel() const override { return m_label; };

        void SetVariant(MD3ButtonVariant variant);
        MD3ButtonVariant GetVariant() const { return m_variant; }

        void SetIcon(const wxBitmap& icon);
        wxBitmap GetIcon() const { return m_icon; }

        void SetIconPosition(bool iconBeforeText = true) { m_iconBeforeText = iconBeforeText; }
        bool GetIconPosition() const { return m_iconBeforeText; }

        // MD3-specific properties
        void SetCornerRadius(int radius) { m_cornerRadius = radius; }
        int GetCornerRadius() const { return m_cornerRadius; }

        void SetElevation(int elevation) { m_elevation = elevation; }
        int GetElevation() const { return m_elevation; }

        // Override MD3Control methods
        virtual void SetState(MD3State state) override;
        virtual void Render(wxDC& dc) override;
        virtual wxSize DoGetBestSize() const override;

        // Event handling
        virtual void OnPaint(wxPaintEvent& event) override;
        virtual void OnSize(wxSizeEvent& event) override;
        virtual void OnMouseLeftDown(wxMouseEvent& event) override;
        virtual void OnMouseLeftUp(wxMouseEvent& event) override;
        virtual void OnMouseEnter(wxMouseEvent& event) override;
        virtual void OnMouseLeave(wxMouseEvent& event) override;

    protected:
        // Internal methods
        virtual void UpdateAppearance();
        virtual wxColour GetBackgroundColor() const;
        virtual wxColour GetForegroundColor() const;
        virtual wxColour GetBorderColor() const;

        // Button state properties
        wxString m_label;
        wxBitmap m_icon;
        bool m_iconBeforeText;
        MD3ButtonVariant m_variant;
        int m_cornerRadius;
        int m_elevation;
        int m_currentElevation;
        
        // ✨ 涟漪动画相关
        float m_rippleRadius;  // 涟漪半径（0.0 - 1.0）
        wxPoint m_rippleCenter;  // 涟漪中心
        std::shared_ptr<MD3PropertyAnimation<float>> m_rippleAnimation;  // 涟漪动画
        std::shared_ptr<MD3PropertyAnimation<int>> m_elevationAnimation;  // 阴影动画

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

    // MD3 Button event
    wxDECLARE_EVENT(wxEVT_MD3_BUTTON_CLICKED, wxCommandEvent);

} // namespace wx_md3

#endif // MD3BUTTON_H