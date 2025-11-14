#include "wx_md3/core/MD3Events.h"

namespace wx_md3 {

    // Define events
    wxDEFINE_EVENT(wxEVT_MD3_CLICK, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_HOVER_ENTER, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_HOVER_LEAVE, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_PRESS, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_RELEASE, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_FOCUS_GAINED, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_FOCUS_LOST, wxCommandEvent);
    wxDEFINE_EVENT(wxEVT_MD3_STATE_CHANGED, wxCommandEvent);
    // Note: wxEVT_MD3_THEME_CHANGED is defined in MD3Theme.cpp to avoid multiple definition errors

} // namespace wx_md3