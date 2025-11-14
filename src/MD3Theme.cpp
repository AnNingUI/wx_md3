#include "wx_md3/core/MD3Theme.h"
#include <wx/window.h>
#include <wx/log.h>

namespace wx_md3 {

    // Define static member
    std::shared_ptr<MD3Theme> MD3Theme::s_currentTheme = nullptr;

    // Define event
    wxDEFINE_EVENT(wxEVT_MD3_THEME_CHANGED, wxCommandEvent);

    // Constructor
    MD3Theme::MD3Theme() : m_themeType(MD3ThemeType::Light), m_dynamicColors(false), m_colorMapBuilt(false) {
        InitializeLightColors();
    }

    MD3Theme::MD3Theme(MD3ThemeType type) : m_themeType(type), m_dynamicColors(false), m_colorMapBuilt(false) {
        if (type == MD3ThemeType::Light) {
            InitializeLightColors();
        } else {
            InitializeDarkColors();
        }
    }

    // Destructor
    MD3Theme::~MD3Theme() {
        // Cleanup resources
    }

    // Theme type management
    void MD3Theme::SetThemeType(MD3ThemeType type) {
        if (m_themeType != type) {
            m_themeType = type;
            if (type == MD3ThemeType::Light) {
                InitializeLightColors();
            } else {
                InitializeDarkColors();
            }

            // Send theme change event
            wxCommandEvent event(wxEVT_MD3_THEME_CHANGED, wxID_ANY);
            // Note: Static theme change event needs special handling
        }
    }

    // Get specific color
    wxColour MD3Theme::GetColor(const wxString& colorName) const {
        // Build color map (if not already built)
        if (!m_colorMapBuilt) {
            const_cast<MD3Theme*>(this)->BuildColorMap();
        }

        auto it = m_colorMap.find(colorName);
        if (it != m_colorMap.end()) {
            return it->second;
        }

        // 🔧 添加警告日志以便调试
        wxLogWarning("Color '%s' not found in theme, returning black as fallback", colorName);
        return wxColour(0, 0, 0);
    }

    // Apply theme to window
    void MD3Theme::ApplyToWindow(wxWindow* window) const {
        if (!window) return;

        // Set background color
        window->SetBackgroundColour(m_colorScheme.background);
        window->SetForegroundColour(m_colorScheme.onBackground);

        // Refresh window
        window->Refresh();
    }

    // Static methods to get default themes
    std::shared_ptr<MD3Theme> MD3Theme::GetDefaultLightTheme() {
        return std::make_shared<MD3Theme>(MD3ThemeType::Light);
    }

    std::shared_ptr<MD3Theme> MD3Theme::GetDefaultDarkTheme() {
        return std::make_shared<MD3Theme>(MD3ThemeType::Dark);
    }

    std::shared_ptr<MD3Theme> MD3Theme::GetCurrentTheme() {
        if (!s_currentTheme) {
            s_currentTheme = GetDefaultLightTheme();
        }
        return s_currentTheme;
    }

    void MD3Theme::SetCurrentTheme(std::shared_ptr<MD3Theme> theme) {
        s_currentTheme = theme;

        // Send theme change event
        wxCommandEvent event(wxEVT_MD3_THEME_CHANGED, wxID_ANY);
        // Note: Global theme change event needs special handling
    }

    // Material You support (dynamic colors)
    void MD3Theme::EnableDynamicColors(bool enable) {
        if (m_dynamicColors != enable) {
            m_dynamicColors = enable;
            if (enable) {
                InitializeDynamicColors();
            } else {
                // Restore to standard theme
                if (m_themeType == MD3ThemeType::Light) {
                    InitializeLightColors();
                } else {
                    InitializeDarkColors();
                }
            }
        }
    }

    // Initialize light colors
    void MD3Theme::InitializeLightColors() {
        // Material Design 3 light theme colors
        m_colorScheme.primary = wxColour(103, 80, 164); // #6750A4
        m_colorScheme.onPrimary = wxColour(255, 255, 255); // #FFFFFF
        m_colorScheme.primaryContainer = wxColour(234, 221, 255); // #EADDFF
        m_colorScheme.onPrimaryContainer = wxColour(33, 0, 93); // #21005D

        m_colorScheme.secondary = wxColour(98, 91, 113); // #625B71
        m_colorScheme.onSecondary = wxColour(255, 255, 255); // #FFFFFF
        m_colorScheme.secondaryContainer = wxColour(232, 222, 248); // #E8DEF8
        m_colorScheme.onSecondaryContainer = wxColour(30, 25, 43); // #1E192B

        m_colorScheme.tertiary = wxColour(125, 82, 96); // #7D5260
        m_colorScheme.onTertiary = wxColour(255, 255, 255); // #FFFFFF
        m_colorScheme.tertiaryContainer = wxColour(255, 216, 228); // #FFD8E4
        m_colorScheme.onTertiaryContainer = wxColour(55, 11, 30); // #370B1E

        m_colorScheme.error = wxColour(186, 26, 26); // #BA1A1A
        m_colorScheme.onError = wxColour(255, 255, 255); // #FFFFFF
        m_colorScheme.errorContainer = wxColour(255, 218, 214); // #FFDAD6
        m_colorScheme.onErrorContainer = wxColour(65, 0, 2); // #410002

        m_colorScheme.background = wxColour(255, 255, 255); // #FFFBFE
        m_colorScheme.onBackground = wxColour(28, 27, 31); // #1C1B1F
        m_colorScheme.surface = wxColour(255, 255, 255); // #FFFBFE
        m_colorScheme.onSurface = wxColour(28, 27, 31); // #1C1B1F
        m_colorScheme.surfaceVariant = wxColour(231, 224, 236); // #E7E0EC
        m_colorScheme.onSurfaceVariant = wxColour(73, 69, 79); // #49454F
        m_colorScheme.outline = wxColour(121, 116, 126); // #79747E
        m_colorScheme.outlineVariant = wxColour(202, 196, 208); // #CAC4D0
        m_colorScheme.shadow = wxColour(0, 0, 0); // #000000
        m_colorScheme.scrim = wxColour(0, 0, 0); // #000000

        m_colorScheme.surfaceTint = m_colorScheme.primary;
        m_colorScheme.inverseSurface = wxColour(49, 48, 51); // #313033
        m_colorScheme.inverseOnSurface = wxColour(245, 239, 244); // #F4EFF4
        m_colorScheme.inversePrimary = wxColour(208, 188, 255); // #D0BCFF

        // Mark color map as needing rebuild
        m_colorMapBuilt = false;
    }

    // Initialize dark colors
    void MD3Theme::InitializeDarkColors() {
        // Material Design 3 dark theme colors
        m_colorScheme.primary = wxColour(208, 188, 255); // #D0BCFF
        m_colorScheme.onPrimary = wxColour(55, 30, 115); // #371E73
        m_colorScheme.primaryContainer = wxColour(79, 55, 139); // #4F378B
        m_colorScheme.onPrimaryContainer = wxColour(234, 221, 255); // #EADDFF

        m_colorScheme.secondary = wxColour(197, 186, 213); // #CCC2DC
        m_colorScheme.onSecondary = wxColour(51, 45, 65); // #332D41
        m_colorScheme.secondaryContainer = wxColour(74, 68, 88); // #4A4458
        m_colorScheme.onSecondaryContainer = wxColour(232, 222, 248); // #E8DEF8

        m_colorScheme.tertiary = wxColour(243, 184, 202); // #EFB8C8
        m_colorScheme.onTertiary = wxColour(73, 37, 50); // #492532
        m_colorScheme.tertiaryContainer = wxColour(99, 59, 75); // #633B4B
        m_colorScheme.onTertiaryContainer = wxColour(255, 216, 228); // #FFD8E4

        m_colorScheme.error = wxColour(255, 180, 171); // #FFB4AB
        m_colorScheme.onError = wxColour(102, 0, 5); // #660005
        m_colorScheme.errorContainer = wxColour(140, 0, 9); // #8C0009
        m_colorScheme.onErrorContainer = wxColour(255, 218, 214); // #FFDAD6

        m_colorScheme.background = wxColour(19, 18, 22); // #141316
        m_colorScheme.onBackground = wxColour(231, 225, 229); // #E6E1E5
        m_colorScheme.surface = wxColour(19, 18, 22); // #141316
        m_colorScheme.onSurface = wxColour(231, 225, 229); // #E6E1E5
        m_colorScheme.surfaceVariant = wxColour(73, 69, 79); // #49454F
        m_colorScheme.onSurfaceVariant = wxColour(202, 196, 208); // #CAC4D0
        m_colorScheme.outline = wxColour(147, 143, 153); // #938F99
        m_colorScheme.outlineVariant = wxColour(73, 69, 79); // #49454F
        m_colorScheme.shadow = wxColour(0, 0, 0); // #000000
        m_colorScheme.scrim = wxColour(0, 0, 0); // #000000

        m_colorScheme.surfaceTint = m_colorScheme.primary;
        m_colorScheme.inverseSurface = wxColour(231, 225, 229); // #E6E1E5
        m_colorScheme.inverseOnSurface = wxColour(49, 48, 51); // #313033
        m_colorScheme.inversePrimary = wxColour(103, 80, 164); // #6750A4

        // Mark color map as needing rebuild
        m_colorMapBuilt = false;
    }

    // Initialize dynamic colors (Material You)
    void MD3Theme::InitializeDynamicColors() {
        // In actual implementation, this would generate dynamic color scheme based on system theme color
        // Currently we just use default colors
        if (m_themeType == MD3ThemeType::Light) {
            InitializeLightColors();
        } else {
            InitializeDarkColors();
        }
    }

    // Color conversion helper functions
    wxColour MD3Theme::Lighten(const wxColour& color, double factor) const {
        int r = color.Red() + (255 - color.Red()) * factor;
        int g = color.Green() + (255 - color.Green()) * factor;
        int b = color.Blue() + (255 - color.Blue()) * factor;

        r = r > 255 ? 255 : r;
        g = g > 255 ? 255 : g;
        b = b > 255 ? 255 : b;

        return wxColour(r, g, b, color.Alpha());
    }

    wxColour MD3Theme::Darken(const wxColour& color, double factor) const {
        int r = color.Red() * (1 - factor);
        int g = color.Green() * (1 - factor);
        int b = color.Blue() * (1 - factor);

        return wxColour(r, g, b, color.Alpha());
    }

    wxColour MD3Theme::AdjustAlpha(const wxColour& color, double alpha) const {
        int a = alpha * 255;
        a = a > 255 ? 255 : (a < 0 ? 0 : a);
        return wxColour(color.Red(), color.Green(), color.Blue(), a);
    }

    // Build color map
    void MD3Theme::BuildColorMap() {
        m_colorMap.clear();

        m_colorMap["primary"] = m_colorScheme.primary;
        m_colorMap["onPrimary"] = m_colorScheme.onPrimary;
        m_colorMap["primaryContainer"] = m_colorScheme.primaryContainer;
        m_colorMap["onPrimaryContainer"] = m_colorScheme.onPrimaryContainer;

        m_colorMap["secondary"] = m_colorScheme.secondary;
        m_colorMap["onSecondary"] = m_colorScheme.onSecondary;
        m_colorMap["secondaryContainer"] = m_colorScheme.secondaryContainer;
        m_colorMap["onSecondaryContainer"] = m_colorScheme.onSecondaryContainer;

        m_colorMap["tertiary"] = m_colorScheme.tertiary;
        m_colorMap["onTertiary"] = m_colorScheme.onTertiary;
        m_colorMap["tertiaryContainer"] = m_colorScheme.tertiaryContainer;
        m_colorMap["onTertiaryContainer"] = m_colorScheme.onTertiaryContainer;

        m_colorMap["error"] = m_colorScheme.error;
        m_colorMap["onError"] = m_colorScheme.onError;
        m_colorMap["errorContainer"] = m_colorScheme.errorContainer;
        m_colorMap["onErrorContainer"] = m_colorScheme.onErrorContainer;

        m_colorMap["background"] = m_colorScheme.background;
        m_colorMap["onBackground"] = m_colorScheme.onBackground;
        m_colorMap["surface"] = m_colorScheme.surface;
        m_colorMap["onSurface"] = m_colorScheme.onSurface;
        m_colorMap["surfaceVariant"] = m_colorScheme.surfaceVariant;
        m_colorMap["onSurfaceVariant"] = m_colorScheme.onSurfaceVariant;
        m_colorMap["outline"] = m_colorScheme.outline;
        m_colorMap["outlineVariant"] = m_colorScheme.outlineVariant;
        m_colorMap["shadow"] = m_colorScheme.shadow;
        m_colorMap["scrim"] = m_colorScheme.scrim;

        m_colorMap["surfaceTint"] = m_colorScheme.surfaceTint;
        m_colorMap["inverseSurface"] = m_colorScheme.inverseSurface;
        m_colorMap["inverseOnSurface"] = m_colorScheme.inverseOnSurface;
        m_colorMap["inversePrimary"] = m_colorScheme.inversePrimary;

        m_colorMapBuilt = true;
    }

} // namespace wx_md3