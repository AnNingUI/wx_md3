#include "wx_md3/core/MD3Layout.h"
#include <wx/log.h>

namespace wx_md3 {

    // MD3 Row implementation
    MD3Row::MD3Row() {
        m_sizer = new wxBoxSizer(wxHORIZONTAL);
        m_mainAxisAlignment = MD3MainAxisAlignment::Start;
        m_crossAxisAlignment = MD3CrossAxisAlignment::Center;
        m_spacing = 0;
        m_items.clear();
    }

    MD3Row::MD3Row(MD3MainAxisAlignment mainAxis, MD3CrossAxisAlignment crossAxis, int spacing) {
        m_sizer = new wxBoxSizer(wxHORIZONTAL);
        m_mainAxisAlignment = mainAxis;
        m_crossAxisAlignment = crossAxis;
        m_spacing = spacing;
        m_items.clear();
        m_sizer->SetMinSize(wxSize(-1, -1));
        ApplyAlignment();
    }

    void MD3Row::SetMainAxisAlignment(MD3MainAxisAlignment alignment) {
        m_mainAxisAlignment = alignment;
        ApplyAlignment();
    }

    void MD3Row::SetCrossAxisAlignment(MD3CrossAxisAlignment alignment) {
        m_crossAxisAlignment = alignment;
        ApplyAlignment();
    }

    void MD3Row::SetSpacing(int spacing) {
        m_spacing = spacing;
        m_sizer->SetMinSize(wxSize(-1, -1));
        ApplyAlignment();
    }

    wxSizerItem* MD3Row::Add(wxWindow* window, int flex, int border, int flags) {
        // Convert MD3 alignment to wxWidgets flags
        int wxFlags = 0;
        switch (m_crossAxisAlignment) {
            case MD3CrossAxisAlignment::Start:
                wxFlags |= wxALIGN_TOP;
                break;
            case MD3CrossAxisAlignment::Center:
                wxFlags |= wxALIGN_CENTER_VERTICAL;
                break;
            case MD3CrossAxisAlignment::End:
                wxFlags |= wxALIGN_BOTTOM;
                break;
            case MD3CrossAxisAlignment::Stretch:
                wxFlags |= wxEXPAND;
                break;
        }
        wxFlags |= flags;

        // Store item information for ApplyAlignment
        m_items.emplace_back(window, flex, border, wxFlags);

        return m_sizer->Add(window, flex, wxFlags, border);
    }

    wxSizerItem* MD3Row::Add(wxSizer* sizer, int flex, int border, int flags) {
        // Convert MD3 alignment to wxWidgets flags
        int wxFlags = 0;
        switch (m_crossAxisAlignment) {
            case MD3CrossAxisAlignment::Start:
                wxFlags |= wxALIGN_TOP;
                break;
            case MD3CrossAxisAlignment::Center:
                wxFlags |= wxALIGN_CENTER_VERTICAL;
                break;
            case MD3CrossAxisAlignment::End:
                wxFlags |= wxALIGN_BOTTOM;
                break;
            case MD3CrossAxisAlignment::Stretch:
                wxFlags |= wxEXPAND;
                break;
        }
        wxFlags |= flags;

        // Store item information for ApplyAlignment
        m_items.emplace_back(sizer, flex, border, wxFlags);

        return m_sizer->Add(sizer, flex, wxFlags, border);
    }

    void MD3Row::ApplyAlignment() {
        // Store current items since we need to clear and rebuild the sizer
        auto itemsToRebuild = m_items;

        // Clear the sizer without deleting the windows/sizers
        m_sizer->Clear(false);

        // Re-add items with new alignment and spacing
        for (size_t i = 0; i < itemsToRebuild.size(); ++i) {
            const auto& itemInfo = itemsToRebuild[i];
            int border = m_spacing;
            int flags = itemInfo.flags;  // 🔧 先赋值原始flags，而不是初始化为0

            // Apply cross axis alignment
            switch (m_crossAxisAlignment) {
                case MD3CrossAxisAlignment::Start:
                    flags |= wxALIGN_TOP;
                    break;
                case MD3CrossAxisAlignment::Center:
                    flags |= wxALIGN_CENTER_VERTICAL;
                    break;
                case MD3CrossAxisAlignment::End:
                    flags |= wxALIGN_BOTTOM;
                    break;
                case MD3CrossAxisAlignment::Stretch:
                    flags |= wxEXPAND;
                    break;
            }

            // Add spacing between items (not after the last item)
            if (i < itemsToRebuild.size() - 1) {
                flags |= wxRIGHT;
            }

            // Add the item back to the sizer
            if (itemInfo.type == ItemInfo::WindowItem) {
                m_sizer->Add(itemInfo.window, itemInfo.proportion, flags, border);
            } else if (itemInfo.type == ItemInfo::SizerItem) {
                m_sizer->Add(itemInfo.sizer, itemInfo.proportion, flags, border);
            }
        }
    }

    // MD3 Column implementation
    MD3Column::MD3Column() {
        m_sizer = new wxBoxSizer(wxVERTICAL);
        m_mainAxisAlignment = MD3MainAxisAlignment::Start;
        m_crossAxisAlignment = MD3CrossAxisAlignment::Start;
        m_spacing = 0;
        m_items.clear();
    }

    MD3Column::MD3Column(MD3MainAxisAlignment mainAxis, MD3CrossAxisAlignment crossAxis, int spacing) {
        m_sizer = new wxBoxSizer(wxVERTICAL);
        m_mainAxisAlignment = mainAxis;
        m_crossAxisAlignment = crossAxis;
        m_spacing = spacing;
        m_items.clear();
        m_sizer->SetMinSize(wxSize(-1, -1));
        ApplyAlignment();
    }

    void MD3Column::SetMainAxisAlignment(MD3MainAxisAlignment alignment) {
        m_mainAxisAlignment = alignment;
        ApplyAlignment();
    }

    void MD3Column::SetCrossAxisAlignment(MD3CrossAxisAlignment alignment) {
        m_crossAxisAlignment = alignment;
        ApplyAlignment();
    }

    void MD3Column::SetSpacing(int spacing) {
        m_spacing = spacing;
        m_sizer->SetMinSize(wxSize(-1, -1));
        ApplyAlignment();
    }

    wxSizerItem* MD3Column::Add(wxWindow* window, int flex, int border, int flags) {
        // Convert MD3 alignment to wxWidgets flags
        int wxFlags = 0;
        switch (m_crossAxisAlignment) {
            case MD3CrossAxisAlignment::Start:
                wxFlags |= wxALIGN_LEFT;
                break;
            case MD3CrossAxisAlignment::Center:
                wxFlags |= wxALIGN_CENTER_HORIZONTAL;
                break;
            case MD3CrossAxisAlignment::End:
                wxFlags |= wxALIGN_RIGHT;
                break;
            case MD3CrossAxisAlignment::Stretch:
                wxFlags |= wxEXPAND;
                break;
        }
        wxFlags |= flags;

        // Store item information for ApplyAlignment
        m_items.emplace_back(window, flex, border, wxFlags);

        return m_sizer->Add(window, flex, wxFlags, border);
    }

    wxSizerItem* MD3Column::Add(wxSizer* sizer, int flex, int border, int flags) {
        // Convert MD3 alignment to wxWidgets flags
        int wxFlags = 0;
        switch (m_crossAxisAlignment) {
            case MD3CrossAxisAlignment::Start:
                wxFlags |= wxALIGN_LEFT;
                break;
            case MD3CrossAxisAlignment::Center:
                wxFlags |= wxALIGN_CENTER_HORIZONTAL;
                break;
            case MD3CrossAxisAlignment::End:
                wxFlags |= wxALIGN_RIGHT;
                break;
            case MD3CrossAxisAlignment::Stretch:
                wxFlags |= wxEXPAND;
                break;
        }
        wxFlags |= flags;

        // Store item information for ApplyAlignment
        m_items.emplace_back(sizer, flex, border, wxFlags);

        return m_sizer->Add(sizer, flex, wxFlags, border);
    }

    void MD3Column::ApplyAlignment() {
        // Store current items since we need to clear and rebuild the sizer
        auto itemsToRebuild = m_items;

        // Clear the sizer without deleting the windows/sizers
        m_sizer->Clear(false);

        // Re-add items with new alignment and spacing
        for (size_t i = 0; i < itemsToRebuild.size(); ++i) {
            const auto& itemInfo = itemsToRebuild[i];
            int border = m_spacing;
            int flags = itemInfo.flags;  // 🔧 先赋值原始flags，而不是初始化为0

            // Apply cross axis alignment
            switch (m_crossAxisAlignment) {
                case MD3CrossAxisAlignment::Start:
                    flags |= wxALIGN_LEFT;
                    break;
                case MD3CrossAxisAlignment::Center:
                    flags |= wxALIGN_CENTER_HORIZONTAL;
                    break;
                case MD3CrossAxisAlignment::End:
                    flags |= wxALIGN_RIGHT;
                    break;
                case MD3CrossAxisAlignment::Stretch:
                    flags |= wxEXPAND;
                    break;
            }

            // Add spacing between items (not after the last item)
            if (i < itemsToRebuild.size() - 1) {
                flags |= wxBOTTOM;
            }

            // Add the item back to the sizer
            if (itemInfo.type == ItemInfo::WindowItem) {
                m_sizer->Add(itemInfo.window, itemInfo.proportion, flags, border);
            } else if (itemInfo.type == ItemInfo::SizerItem) {
                m_sizer->Add(itemInfo.sizer, itemInfo.proportion, flags, border);
            }
        }
    }

} // namespace wx_md3