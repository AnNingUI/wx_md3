#ifndef MD3THEME_H
#define MD3THEME_H

#include <wx/colour.h>
#include <wx/string.h>
#include <wx/window.h>
#include <map>
#include <memory>

namespace wx_md3 {

    // Material Design 3 Color System
    struct MD3ColorScheme {
        // Primary colors
        wxColour primary;
        wxColour onPrimary;
        wxColour primaryContainer;
        wxColour onPrimaryContainer;

        // Secondary colors
        wxColour secondary;
        wxColour onSecondary;
        wxColour secondaryContainer;
        wxColour onSecondaryContainer;

        // Tertiary colors
        wxColour tertiary;
        wxColour onTertiary;
        wxColour tertiaryContainer;
        wxColour onTertiaryContainer;

        // Error colors
        wxColour error;
        wxColour onError;
        wxColour errorContainer;
        wxColour onErrorContainer;

        // Background and surface
        wxColour background;
        wxColour onBackground;
        wxColour surface;
        wxColour onSurface;
        wxColour surfaceVariant;
        wxColour onSurfaceVariant;
        wxColour outline;
        wxColour outlineVariant;
        wxColour shadow;
        wxColour scrim;

        // State colors
        wxColour surfaceTint;
        wxColour inverseSurface;
        wxColour inverseOnSurface;
        wxColour inversePrimary;
    };

    // Theme type enum
    enum class MD3ThemeType {
        Light,
        Dark
    };

    // MD3 Theme Manager
    class MD3Theme {
    public:
        // Constructor
        MD3Theme();
        MD3Theme(MD3ThemeType type);

        // Destructor
        virtual ~MD3Theme();

        // Theme type management
        void SetThemeType(MD3ThemeType type);
        MD3ThemeType GetThemeType() const { return m_themeType; }

        // Get color scheme
        const MD3ColorScheme& GetColorScheme() const { return m_colorScheme; }

        // Get specific color
        wxColour GetColor(const wxString& colorName) const;

        // Apply theme to window
        void ApplyToWindow(wxWindow* window) const;

        // Static methods to get default themes
        static std::shared_ptr<MD3Theme> GetDefaultLightTheme();
        static std::shared_ptr<MD3Theme> GetDefaultDarkTheme();
        static std::shared_ptr<MD3Theme> GetCurrentTheme();
        static void SetCurrentTheme(std::shared_ptr<MD3Theme> theme);

        // Material You support (dynamic colors)
        void EnableDynamicColors(bool enable);
        bool IsDynamicColorsEnabled() const { return m_dynamicColors; }

        // Color conversion helper functions (public for external use)
        wxColour Lighten(const wxColour& color, double factor) const;
        wxColour Darken(const wxColour& color, double factor) const;
        wxColour AdjustAlpha(const wxColour& color, double alpha) const;

    protected:
        // Initialize color schemes
        void InitializeLightColors();
        void InitializeDarkColors();
        void InitializeDynamicColors();

    private:
        MD3ThemeType m_themeType;
        MD3ColorScheme m_colorScheme;
        bool m_dynamicColors;

        // Global current theme
        static std::shared_ptr<MD3Theme> s_currentTheme;

        // Color map (for easy color access by name)
        std::map<wxString, wxColour> m_colorMap;
        bool m_colorMapBuilt;
        void BuildColorMap();
    };

    // Theme change event
    wxDECLARE_EVENT(wxEVT_MD3_THEME_CHANGED, wxCommandEvent);

} // namespace wx_md3

#endif // MD3THEME_H