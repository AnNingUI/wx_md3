#ifndef MD3IMAGE_H
#define MD3IMAGE_H

#include "wx_md3/core/MD3Control.h"
#include "wx_md3/core/MD3Theme.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/graphics.h>
#include <unordered_map>
#include <list>
#include <utility>

namespace wx_md3 {

    // MD3 Image shape variants
    enum class MD3ImageShape {
        Rectangle,   // Default rectangular shape
        Circle,      // Circular/cropped shape
        Rounded      // Rounded rectangle with custom radius
    };

    // MD3 Image scaling quality options
    enum class MD3ImageQuality {
        Fast,        // wxIMAGE_QUALITY_NORMAL - faster but lower quality
        High,        // wxIMAGE_QUALITY_HIGH - slower but higher quality
        Best         // wxIMAGE_QUALITY_BICUBIC - best quality but slowest
    };

    // MD3 Image class
    class MD3Image : public MD3Control {
        DECLARE_DYNAMIC_CLASS(MD3Image)

    public:
        // Constructors
        MD3Image();
        MD3Image(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxBitmap& bitmap = wxNullBitmap,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = "md3Image");

        // Destructor
        virtual ~MD3Image();

        // Image properties
        void SetBitmap(const wxBitmap& bitmap);
        wxBitmap GetBitmap() const { return m_bitmap; }

        void SetImageShape(MD3ImageShape shape);
        MD3ImageShape GetImageShape() const { return m_shape; }

        void SetCornerRadius(int radius);
        int GetCornerRadius() const { return m_cornerRadius; }

        void SetScaleType(bool scaleToFit = true) { m_scaleToFit = scaleToFit; }
        bool GetScaleType() const { return m_scaleToFit; }

        void SetImageQuality(MD3ImageQuality quality) { m_quality = quality; InvalidateCache(); }
        MD3ImageQuality GetImageQuality() const { return m_quality; }

        void SetCacheSize(size_t maxSize) { m_cacheMaxSize = maxSize; }
        size_t GetCacheSize() const { return m_cacheMaxSize; }

        // Override MD3Control methods
        virtual void SetState(MD3State state) override;
        virtual void Render(wxDC& dc) override;
        virtual wxSize DoGetBestSize() const override;

        // Event handling
        virtual void OnPaint(wxPaintEvent& event) override;
        virtual void OnSize(wxSizeEvent& event) override;

    protected:
        // Cache key structure for LRU cache
        struct CacheKey {
            wxSize size;
            MD3ImageShape shape;
            int cornerRadius;

            bool operator==(const CacheKey& other) const {
                return size == other.size &&
                       shape == other.shape &&
                       cornerRadius == other.cornerRadius;
            }
        };

        struct CacheKeyHash {
            std::size_t operator()(const CacheKey& k) const {
                return std::hash<int>()(k.size.GetWidth()) ^
                       std::hash<int>()(k.size.GetHeight()) ^
                       std::hash<int>()(static_cast<int>(k.shape)) ^
                       std::hash<int>()(k.cornerRadius);
            }
        };

        // Internal methods
        virtual void UpdateAppearance();
        void InvalidateCache();
        wxBitmap CreateProcessedBitmap(const wxBitmap& source, const wxSize& targetSize, MD3ImageShape shape, int cornerRadius, MD3ImageQuality quality = MD3ImageQuality::High);
        void DrawParentBackground(wxDC& dc, const wxRect& rect);

        // Image state properties
        wxBitmap m_bitmap;
        MD3ImageShape m_shape;
        int m_cornerRadius;
        bool m_scaleToFit;
        MD3ImageQuality m_quality;

        // LRU Cache properties
        using CacheMap = std::unordered_map<CacheKey, wxBitmap, CacheKeyHash>;
        using CacheList = std::list<CacheKey>;

        CacheMap m_cache;
        CacheList m_cacheOrder;
        size_t m_cacheMaxSize;

    private:
        void Init();

        wxDECLARE_EVENT_TABLE();
    };

} // namespace wx_md3

#endif // MD3IMAGE_H