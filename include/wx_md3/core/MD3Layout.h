#ifndef MD3LAYOUT_H
#define MD3LAYOUT_H

#include <wx/wx.h>
#include <wx/sizer.h>
#include <vector>

namespace wx_md3 {

    // Layout alignment options
    enum class MD3Alignment {
        Start,
        Center,
        End,
        SpaceBetween,
        SpaceAround,
        SpaceEvenly
    };

    // Main axis alignment
    enum class MD3MainAxisAlignment {
        Start = static_cast<int>(MD3Alignment::Start),
        Center = static_cast<int>(MD3Alignment::Center),
        End = static_cast<int>(MD3Alignment::End),
        SpaceBetween = static_cast<int>(MD3Alignment::SpaceBetween),
        SpaceAround = static_cast<int>(MD3Alignment::SpaceAround),
        SpaceEvenly = static_cast<int>(MD3Alignment::SpaceEvenly)
    };

    // Cross axis alignment
    enum class MD3CrossAxisAlignment {
        Start = static_cast<int>(MD3Alignment::Start),
        Center = static_cast<int>(MD3Alignment::Center),
        End = static_cast<int>(MD3Alignment::End),
        Stretch
    };

    // Structure to store item information
    struct ItemInfo {
        enum ItemType {
            WindowItem,
            SizerItem
        };

        ItemType type;
        wxWindow* window;
        wxSizer* sizer;
        int proportion;
        int border;
        int flags;

        ItemInfo(wxWindow* w, int p, int b, int f) : type(WindowItem), window(w), sizer(nullptr), proportion(p), border(b), flags(f) {}
        ItemInfo(wxSizer* s, int p, int b, int f) : type(SizerItem), window(nullptr), sizer(s), proportion(p), border(b), flags(f) {}
    };

    // MD3 Row layout (horizontal) - uses wxBoxSizer internally
    class MD3Row {
    public:
        MD3Row();
        MD3Row(MD3MainAxisAlignment mainAxis, MD3CrossAxisAlignment crossAxis = MD3CrossAxisAlignment::Center, int spacing = 0);

        // Set main axis alignment
        void SetMainAxisAlignment(MD3MainAxisAlignment alignment);

        // Set cross axis alignment
        void SetCrossAxisAlignment(MD3CrossAxisAlignment alignment);

        // Set spacing between children
        void SetSpacing(int spacing);

        // Add child with flex factor
        wxSizerItem* Add(wxWindow* window, int flex = 0, int border = 0, int flags = 0);
        wxSizerItem* Add(wxSizer* sizer, int flex = 0, int border = 0, int flags = 0);

        // Get the underlying wxSizer
        wxSizer* GetSizer() const { return m_sizer; }

    private:
        wxBoxSizer* m_sizer;
        std::vector<ItemInfo> m_items; // Store item information for ApplyAlignment
        MD3MainAxisAlignment m_mainAxisAlignment;
        MD3CrossAxisAlignment m_crossAxisAlignment;
        int m_spacing;

        // Apply alignment settings
        void ApplyAlignment();
    };

    // MD3 Column layout (vertical) - uses wxBoxSizer internally
    class MD3Column {
    public:
        MD3Column();
        MD3Column(MD3MainAxisAlignment mainAxis, MD3CrossAxisAlignment crossAxis = MD3CrossAxisAlignment::Start, int spacing = 0);

        // Set main axis alignment
        void SetMainAxisAlignment(MD3MainAxisAlignment alignment);

        // Set cross axis alignment
        void SetCrossAxisAlignment(MD3CrossAxisAlignment alignment);

        // Set spacing between children
        void SetSpacing(int spacing);

        // Add child with flex factor
        wxSizerItem* Add(wxWindow* window, int flex = 0, int border = 0, int flags = 0);
        wxSizerItem* Add(wxSizer* sizer, int flex = 0, int border = 0, int flags = 0);

        // Get the underlying wxSizer
        wxSizer* GetSizer() const { return m_sizer; }

    private:
        wxBoxSizer* m_sizer;
        std::vector<ItemInfo> m_items; // Store item information for ApplyAlignment
        MD3MainAxisAlignment m_mainAxisAlignment;
        MD3CrossAxisAlignment m_crossAxisAlignment;
        int m_spacing;

        // Apply alignment settings
        void ApplyAlignment();
    };

} // namespace wx_md3

#endif // MD3LAYOUT_H