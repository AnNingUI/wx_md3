#ifndef MD3EVENTS_H
#define MD3EVENTS_H

#include <wx/event.h>

namespace wx_md3 {

    // MD3 Event types
    wxDECLARE_EVENT(wxEVT_MD3_CLICK, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_HOVER_ENTER, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_HOVER_LEAVE, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_PRESS, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_RELEASE, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_FOCUS_GAINED, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_FOCUS_LOST, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_STATE_CHANGED, wxCommandEvent);
    wxDECLARE_EVENT(wxEVT_MD3_THEME_CHANGED, wxCommandEvent);

    // Simple macro for easy event binding
    #define MD3_BIND_EVENT(window, eventType, handler) \
        window->Bind(eventType, [this](wxCommandEvent& event) { handler(event); })

    #define MD3_BIND_CLICK(window, handler) \
        MD3_BIND_EVENT(window, wxEVT_MD3_CLICK, handler)

    #define MD3_BIND_HOVER_ENTER(window, handler) \
        MD3_BIND_EVENT(window, wxEVT_MD3_HOVER_ENTER, handler)

    #define MD3_BIND_HOVER_LEAVE(window, handler) \
        MD3_BIND_EVENT(window, wxEVT_MD3_HOVER_LEAVE, handler)

} // namespace wx_md3

#endif // MD3EVENTS_H