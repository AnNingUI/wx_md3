#ifndef MD3CARD_H
#define MD3CARD_H

#include "wx_md3/core/MD3Control.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Animator.h"
#include <wx/panel.h>

namespace wx_md3 {

    // MD3 Card variants
    enum class MD3CardVariant {
        Elevated,    // Default elevated card with shadow
        Filled,      // Filled card (surface color)
        Outlined     // Outlined card with border
    };

    // MD3 Card class
    class MD3Card : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3Card)

    public:
        // Constructors
        MD3Card();
        MD3Card(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = "md3Card");

        // Destructor
        virtual ~MD3Card();

        // Card properties
        void SetVariant(MD3CardVariant variant);
        MD3CardVariant GetVariant() const { return m_variant; }

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
        virtual void OnMouseEnter(wxMouseEvent& event) override;
        virtual void OnMouseLeave(wxMouseEvent& event) override;

    protected:
        // Internal methods
        virtual void UpdateAppearance();
        virtual wxColour GetBackgroundColor() const;
        virtual wxColour GetBorderColor() const;

        // Card state properties
        MD3CardVariant m_variant;
        int m_cornerRadius;
        int m_elevation;
        int m_currentElevation;

        // Animation support
        std::shared_ptr<MD3PropertyAnimation<int>> m_elevationAnimation;

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

} // namespace wx_md3

#endif // MD3CARD_H