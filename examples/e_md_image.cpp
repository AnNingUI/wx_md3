#include <wx/wx.h>
#include "wx_md3/components/MD3Image.h"
#include "wx_md3/components/MD3Card.h"
#include "wx_md3/core/MD3Theme.h"
#include "wx_md3/core/MD3Layout.h"

class MD3ImageExample : public wxFrame {
public:
    MD3ImageExample() : wxFrame(nullptr, wxID_ANY, "MD3 Image Example", wxDefaultPosition, wxSize(600, 500)) {
        // Create a panel to hold the images
        wxPanel* panel = new wxPanel(this);

        // Create a sample bitmap for demonstration
        wxBitmap sampleBitmap = CreateSampleBitmap();

        // Create cards to hold the images with labels
        wx_md3::MD3Card* rectCard = new wx_md3::MD3Card(panel, wxID_ANY);
        rectCard->SetVariant(wx_md3::MD3CardVariant::Elevated);

        wx_md3::MD3Card* roundedCard = new wx_md3::MD3Card(panel, wxID_ANY);
        roundedCard->SetVariant(wx_md3::MD3CardVariant::Elevated);

        wx_md3::MD3Card* circularCard = new wx_md3::MD3Card(panel, wxID_ANY);
        circularCard->SetVariant(wx_md3::MD3CardVariant::Elevated);

        // Create MD3 images with different shapes - parent is the card, not panel
        wx_md3::MD3Image* rectangularImage = new wx_md3::MD3Image(rectCard, wxID_ANY, sampleBitmap);
        rectangularImage->SetImageShape(wx_md3::MD3ImageShape::Rectangle);
        rectangularImage->SetMinSize(wxSize(150, 150));

        wx_md3::MD3Image* roundedImage = new wx_md3::MD3Image(roundedCard, wxID_ANY, sampleBitmap);
        roundedImage->SetImageShape(wx_md3::MD3ImageShape::Rounded);
        roundedImage->SetCornerRadius(12);
        roundedImage->SetMinSize(wxSize(150, 150));

        wx_md3::MD3Image* circularImage = new wx_md3::MD3Image(circularCard, wxID_ANY, sampleBitmap);
        circularImage->SetImageShape(wx_md3::MD3ImageShape::Circle);
        circularImage->SetMinSize(wxSize(150, 150));

        // Create sizers for each card
        wxBoxSizer* rectSizer = new wxBoxSizer(wxVERTICAL);
        rectSizer->Add(new wxStaticText(rectCard, wxID_ANY, "Rectangle"), 0, wxALIGN_CENTER | wxALL, 8);
        rectSizer->Add(rectangularImage, 0, wxALIGN_CENTER | wxALL, 8);
        rectCard->SetSizer(rectSizer);

        wxBoxSizer* roundedSizer = new wxBoxSizer(wxVERTICAL);
        roundedSizer->Add(new wxStaticText(roundedCard, wxID_ANY, "Rounded"), 0, wxALIGN_CENTER | wxALL, 8);
        roundedSizer->Add(roundedImage, 0, wxALIGN_CENTER | wxALL, 8);
        roundedCard->SetSizer(roundedSizer);

        wxBoxSizer* circularSizer = new wxBoxSizer(wxVERTICAL);
        circularSizer->Add(new wxStaticText(circularCard, wxID_ANY, "Circle"), 0, wxALIGN_CENTER | wxALL, 8);
        circularSizer->Add(circularImage, 0, wxALIGN_CENTER | wxALL, 8);
        circularCard->SetSizer(circularSizer);

        // Create layout using MD3Layout
        wx_md3::MD3Column* mainColumn = new wx_md3::MD3Column(
            wx_md3::MD3MainAxisAlignment::Start,
            wx_md3::MD3CrossAxisAlignment::Center,
            20 // spacing
        );

        wx_md3::MD3Row* imageRow = new wx_md3::MD3Row(
            wx_md3::MD3MainAxisAlignment::SpaceEvenly,
            wx_md3::MD3CrossAxisAlignment::Center,
            20 // spacing
        );

        imageRow->Add(rectCard, 0, 8);
        imageRow->Add(roundedCard, 0, 8);
        imageRow->Add(circularCard, 0, 8);

        mainColumn->Add(imageRow->GetSizer(), 0, wxALL, 16);

        // Set the sizer for the panel
        panel->SetSizer(mainColumn->GetSizer());

        // Center the frame
        Centre();
    }

private:
    wxBitmap CreateSampleBitmap() {
        // Create a simple gradient bitmap for demonstration
        wxBitmap bitmap(100, 100);
        wxMemoryDC dc;
        dc.SelectObject(bitmap);

        // Create a simple colored rectangle with gradient effect
        wxColour startColor(103, 80, 164); // MD3 primary color
        wxColour endColor(208, 188, 255);  // MD3 primary container color

        // Draw gradient background
        for (int i = 0; i < 100; i++) {
            int r = startColor.Red() + (endColor.Red() - startColor.Red()) * i / 100;
            int g = startColor.Green() + (endColor.Green() - startColor.Green()) * i / 100;
            int b = startColor.Blue() + (endColor.Blue() - startColor.Blue()) * i / 100;
            dc.SetPen(wxColour(r, g, b));
            dc.DrawLine(0, i, 100, i);
        }

        // Draw a simple icon in the center
        dc.SetPen(*wxWHITE_PEN);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawEllipse(30, 30, 40, 40);

        dc.SelectObject(wxNullBitmap);
        return bitmap;
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MD3ImageExample* frame = new MD3ImageExample();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);