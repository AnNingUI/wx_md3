#include "wx_md3/components/MD3Image.h"
#include <wx/dcbuffer.h>
#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include <wx/log.h>
#include <algorithm>

namespace wx_md3 {

    // Event Table
    wxBEGIN_EVENT_TABLE(MD3Image, MD3Control)
        EVT_PAINT(MD3Image::OnPaint)
        EVT_SIZE(MD3Image::OnSize)
    wxEND_EVENT_TABLE()

    IMPLEMENT_DYNAMIC_CLASS(MD3Image, MD3Control)

    // Constructor
    MD3Image::MD3Image() {
        Init();
    }

    MD3Image::MD3Image(wxWindow* parent, wxWindowID id,
                      const wxBitmap& bitmap,
                      const wxPoint& pos, const wxSize& size,
                      long style, const wxString& name)
        : MD3Control(parent, id, pos, size, style, name) {
        m_bitmap = bitmap;
        Init();
    }

    // Initialization Function
    void MD3Image::Init() {
        m_shape = MD3ImageShape::Rectangle;
        m_cornerRadius = 8; // Default MD3 corner radius for images
        m_scaleToFit = true;
        m_quality = MD3ImageQuality::High;
        m_cacheMaxSize = 10; // Default cache size

        // Set window style - always use wxBG_STYLE_PAINT for consistent behavior
        // wxBG_STYLE_PAINT ensures we have full control over the painting process
        SetBackgroundStyle(wxBG_STYLE_PAINT);

        // Initialize appearance
        UpdateAppearance();
    }

    // Destructor
    MD3Image::~MD3Image() {
        // Cleanup resources
    }

    // Image properties
    void MD3Image::SetBitmap(const wxBitmap& bitmap) {
        if (!m_bitmap.IsSameAs(bitmap)) {
            m_bitmap = bitmap;
            InvalidateCache();
            Refresh();
        }
    }

    void MD3Image::SetImageShape(MD3ImageShape shape) {
        if (m_shape != shape) {
            m_shape = shape;
            InvalidateCache();
            Refresh();
        }
    }

    void MD3Image::SetCornerRadius(int radius) {
        if (m_cornerRadius != radius) {
            m_cornerRadius = radius;
            InvalidateCache();
            Refresh();
        }
    }

    // Cache invalidation
    void MD3Image::InvalidateCache() {
        m_cache.clear();
        m_cacheOrder.clear();
    }

    // Override MD3Control methods
    void MD3Image::SetState(MD3State state) {
        MD3Control::SetState(state);
        UpdateAppearance();
        Refresh();
    }

    wxSize MD3Image::DoGetBestSize() const {
        if (m_bitmap.IsOk()) {
            // Return the natural size of the bitmap
            return m_bitmap.GetSize();
        }

        // Default size if no bitmap is set
        return wxSize(100, 100);
    }

    // Event handling
    void MD3Image::OnPaint(wxPaintEvent& event) {
        // Use wxAutoBufferedPaintDC to eliminate flickering
        wxAutoBufferedPaintDC dc(this);
        Render(dc);
    }

    void MD3Image::OnSize(wxSizeEvent& event) {
        InvalidateCache();
        Refresh();
        event.Skip();
    }

    // Internal methods
    void MD3Image::UpdateAppearance() {
        // Update colors based on theme
        SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    }

    // Draw parent background - improved for Win32 compatibility
    void MD3Image::DrawParentBackground(wxDC& dc, const wxRect& rect) {
        wxWindow* parent = GetParent();
        if (!parent) {
            // No parent window: fill with default background color
            dc.SetBrush(wxBrush(GetBackgroundColour()));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
            return;
        }

        // Get parent's background color
        wxColour parentBgColor = parent->GetBackgroundColour();
        
        // If parent has a valid background color, use it
        if (parentBgColor.IsOk()) {
            dc.SetBrush(wxBrush(parentBgColor));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
        } else {
            // Fallback to system background color
            dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)));
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(rect);
        }
    }

    // Create processed bitmap with scaling and shape applied using wxGraphicsContext for smooth edges
    wxBitmap MD3Image::CreateProcessedBitmap(const wxBitmap& source, const wxSize& targetSize, MD3ImageShape shape, int cornerRadius, MD3ImageQuality quality) {
        if (!source.IsOk() || targetSize.GetWidth() <= 0 || targetSize.GetHeight() <= 0) {
            return wxNullBitmap;
        }

        // Convert quality enum to wxImage quality
        wxImageResizeQuality wxQuality = wxIMAGE_QUALITY_HIGH;
        switch (quality) {
            case MD3ImageQuality::Fast:
                wxQuality = wxIMAGE_QUALITY_NORMAL;
                break;
            case MD3ImageQuality::High:
                wxQuality = wxIMAGE_QUALITY_HIGH;
                break;
            case MD3ImageQuality::Best:
                wxQuality = wxIMAGE_QUALITY_BICUBIC;
                break;
        }

        // Create result bitmap with proper initialization
        wxBitmap result(targetSize.GetWidth(), targetSize.GetHeight(), 32); // 32-bit for alpha
        
        // Initialize the bitmap with transparent/white background to avoid black artifacts
        {
            wxMemoryDC memDC;
            memDC.SelectObject(result);
            
            // Clear with parent's background color or white
            wxWindow* parent = GetParent();
            wxColour bgColor = parent ? parent->GetBackgroundColour() : *wxWHITE;
            if (!bgColor.IsOk()) {
                bgColor = *wxWHITE;
            }
            
            memDC.SetBackground(wxBrush(bgColor));
            memDC.Clear();
            memDC.SelectObject(wxNullBitmap);
        }

        // Create graphics context for smooth rendering
        wxMemoryDC memDC;
        memDC.SelectObject(result);
        
        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        if (!gc) {
            // Fallback to basic bitmap scaling if graphics context fails
            memDC.SelectObject(wxNullBitmap);
            wxImage scaledImage = source.ConvertToImage().Rescale(targetSize.GetWidth(), targetSize.GetHeight(), wxQuality);
            return wxBitmap(scaledImage);
        }

        // Scale the source bitmap to target size
        wxImage scaledImage = source.ConvertToImage().Rescale(targetSize.GetWidth(), targetSize.GetHeight(), wxQuality);
        wxBitmap scaledBitmap(scaledImage);

        // Apply shape using mask-based approach for better Win32 compatibility
        if (shape == MD3ImageShape::Rounded && cornerRadius > 0) {
            // Create a mask bitmap for rounded rectangle
            wxBitmap maskBitmap(targetSize.GetWidth(), targetSize.GetHeight(), 1); // 1-bit bitmap
            wxMemoryDC maskDC;
            maskDC.SelectObject(maskBitmap);
            maskDC.SetBackground(*wxBLACK_BRUSH); // Black = transparent
            maskDC.Clear();
            maskDC.SetBrush(*wxWHITE_BRUSH); // White = opaque
            maskDC.SetPen(*wxWHITE_PEN);
            maskDC.DrawRoundedRectangle(0, 0, targetSize.GetWidth(), targetSize.GetHeight(), cornerRadius);
            maskDC.SelectObject(wxNullBitmap);

            // Apply the mask to the bitmap
            wxMask* mask = new wxMask(maskBitmap);
            scaledBitmap.SetMask(mask);
            
            // Draw the masked bitmap
            gc->DrawBitmap(scaledBitmap, 0, 0, targetSize.GetWidth(), targetSize.GetHeight());
        } else if (shape == MD3ImageShape::Circle) {
            // Create a mask bitmap for circular shape
            wxBitmap maskBitmap(targetSize.GetWidth(), targetSize.GetHeight(), 1); // 1-bit bitmap
            wxMemoryDC maskDC;
            maskDC.SelectObject(maskBitmap);
            maskDC.SetBackground(*wxBLACK_BRUSH); // Black = transparent
            maskDC.Clear();
            maskDC.SetBrush(*wxWHITE_BRUSH); // White = opaque
            maskDC.SetPen(*wxWHITE_PEN);
            int radius = std::min(targetSize.GetWidth(), targetSize.GetHeight()) / 2;
            int centerX = targetSize.GetWidth() / 2;
            int centerY = targetSize.GetHeight() / 2;
            maskDC.DrawCircle(centerX, centerY, radius);
            maskDC.SelectObject(wxNullBitmap);

            // Apply the mask to the bitmap
            wxMask* mask = new wxMask(maskBitmap);
            scaledBitmap.SetMask(mask);
            
            // Draw the masked bitmap
            gc->DrawBitmap(scaledBitmap, 0, 0, targetSize.GetWidth(), targetSize.GetHeight());
        } else {
            // Rectangle - no mask needed
            gc->DrawBitmap(scaledBitmap, 0, 0, targetSize.GetWidth(), targetSize.GetHeight());
        }

        delete gc;
        memDC.SelectObject(wxNullBitmap);
        
        return result;
    }

    void MD3Image::Render(wxDC& dc) {
        wxSize size = GetClientSize();
        if (size.GetWidth() <= 0 || size.GetHeight() <= 0) {
            return;
        }

        // Draw parent background first
        wxRect rect(0, 0, size.GetWidth(), size.GetHeight());
        DrawParentBackground(dc, rect);

        if (!m_bitmap.IsOk()) {
            // No bitmap to draw, just show background
            return;
        }

        // Calculate target draw size
        wxSize targetSize = size;
        wxPoint drawPos(0, 0);

        if (m_scaleToFit) {
            // Scale to fit within the control bounds while maintaining aspect ratio
            wxSize bitmapSize = m_bitmap.GetSize();
            if (bitmapSize.GetWidth() > 0 && bitmapSize.GetHeight() > 0) {
                double scaleX = static_cast<double>(size.GetWidth()) / bitmapSize.GetWidth();
                double scaleY = static_cast<double>(size.GetHeight()) / bitmapSize.GetHeight();
                double scale = std::min(scaleX, scaleY);

                targetSize.SetWidth(static_cast<int>(bitmapSize.GetWidth() * scale));
                targetSize.SetHeight(static_cast<int>(bitmapSize.GetHeight() * scale));

                // Center the image
                drawPos.x = (size.GetWidth() - targetSize.GetWidth()) / 2;
                drawPos.y = (size.GetHeight() - targetSize.GetHeight()) / 2;
            }
        }

        // Create cache key for current parameters
        CacheKey key{targetSize, m_shape, m_cornerRadius};

        wxBitmap* cachedBitmap = nullptr;

        // Check if we have a cached bitmap
        auto it = m_cache.find(key);
        if (it != m_cache.end()) {
            // Move to front of LRU list (most recently used)
            m_cacheOrder.remove(key);
            m_cacheOrder.push_front(key);
            cachedBitmap = &it->second;
        } else {
            // Create new processed bitmap
            wxBitmap newBitmap = CreateProcessedBitmap(m_bitmap, targetSize, m_shape, m_cornerRadius, m_quality);

            // Manage LRU cache size
            if (m_cache.size() >= m_cacheMaxSize && m_cacheMaxSize > 0) {
                // Remove least recently used item
                if (!m_cacheOrder.empty()) {
                    CacheKey lruKey = m_cacheOrder.back();
                    m_cache.erase(lruKey);
                    m_cacheOrder.pop_back();
                }
            }

            // Add new item to cache
            if (m_cacheMaxSize > 0) {
                m_cache[key] = newBitmap;
                m_cacheOrder.push_front(key);
                cachedBitmap = &m_cache[key];
            } else {
                // Cache disabled, use the new bitmap directly
                cachedBitmap = &newBitmap;
            }
        }

        // Draw the cached processed bitmap
        if (cachedBitmap && cachedBitmap->IsOk()) {
            dc.DrawBitmap(*cachedBitmap, drawPos.x, drawPos.y, true);
        }
    }

} // namespace wx_md3