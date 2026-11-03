#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace ui
{
namespace col
{
    // Mascherina e serigrafia
    inline const juce::Colour bg       { 0xff17181b };
    inline const juce::Colour panelTop { 0xff35373d };
    inline const juce::Colour panelBot { 0xff25272c };
    inline const juce::Colour panel    { 0xff2b2d33 };
    inline const juce::Colour edge     { 0xff4a4d55 };
    inline const juce::Colour text     { 0xffeae7dc };   // serigrafia chiara
    inline const juce::Colour dim      { 0xff9d9c96 };

    // Strisce colorate dei titoli
    inline const juce::Colour input   { 0xff5fcf8a };
    inline const juce::Colour tape    { 0xffd98c5f };
    inline const juce::Colour filters { 0xff4fb6c9 };
    inline const juce::Colour eq      { 0xffe0a43a };
    inline const juce::Colour comp    { 0xff6aa7ff };
    inline const juce::Colour limiter { 0xffe5584f };

    // LED
    inline const juce::Colour ledRed    { 0xffff3b30 };
    inline const juce::Colour ledGreen  { 0xff35d46a };
    inline const juce::Colour ledYellow { 0xffe6d33a };
    inline const juce::Colour ledAmber  { 0xffffa31a };
}

class Look : public juce::LookAndFeel_V4
{
public:
    Look()
    {
        setColour (juce::Slider::textBoxTextColourId,       col::text);
        setColour (juce::Slider::textBoxOutlineColourId,    juce::Colours::transparentBlack);
        setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Label::textColourId,               col::text);
        setColour (juce::ComboBox::backgroundColourId,      juce::Colour (0xff15161a));
        setColour (juce::ComboBox::textColourId,            col::text);
        setColour (juce::ComboBox::outlineColourId,         juce::Colour (0xff5a5d66));
        setColour (juce::ComboBox::arrowColourId,           col::text);
        setColour (juce::PopupMenu::backgroundColourId,     juce::Colour (0xff1d1e22));
        setColour (juce::PopupMenu::textColourId,           col::text);
        setColour (juce::PopupMenu::highlightedBackgroundColourId, juce::Colour (0xff4a4d55));
        setColour (juce::PopupMenu::highlightedTextColourId, col::text);
    }

    // Manopola da hardware: cappuccio nero con indice bianco e scala di tacche
    void drawRotarySlider (juce::Graphics& g, int x, int y, int w, int h, float pos,
                           float startAngle, float endAngle, juce::Slider&) override
    {
        auto area = juce::Rectangle<int> (x, y, w, h).toFloat().reduced (3.0f);
        const float r = juce::jmin (area.getWidth(), area.getHeight()) * 0.5f;
        const auto c = area.getCentre();
        const float angle = startAngle + pos * (endAngle - startAngle);

        // scala: 11 tacche (quella centrale e le estreme più lunghe)
        g.setColour (col::dim);
        for (int i = 0; i <= 10; ++i)
        {
            const float a = startAngle + (endAngle - startAngle) * (float) i / 10.0f;
            const float sx = std::sin (a), sy = -std::cos (a);
            const float len = (i % 5 == 0) ? 6.0f : 3.5f;
            g.drawLine (c.x + sx * (r - len), c.y + sy * (r - len), c.x + sx * r, c.y + sy * r,
                        (i % 5 == 0) ? 1.6f : 1.0f);
        }

        const float br = r - 9.0f;
        g.setColour (juce::Colours::black.withAlpha (0.55f));
        g.fillEllipse (c.x - br, c.y - br + 2.5f, 2.0f * br, 2.0f * br);

        juce::ColourGradient cap (juce::Colour (0xff53555c), c.x - br * 0.5f, c.y - br * 0.8f,
                                  juce::Colour (0xff0e0f12), c.x + br * 0.6f, c.y + br * 0.9f, false);
        g.setGradientFill (cap);
        g.fillEllipse (c.x - br, c.y - br, 2.0f * br, 2.0f * br);
        g.setColour (juce::Colours::white.withAlpha (0.14f));
        g.drawEllipse (c.x - br, c.y - br, 2.0f * br, 2.0f * br, 1.0f);
        g.setColour (juce::Colour (0xff08090b));
        g.drawEllipse (c.x - br * 0.7f, c.y - br * 0.7f, 1.4f * br, 1.4f * br, 1.0f);

        juce::Path p;
        p.addRoundedRectangle (-1.7f, -br + 2.0f, 3.4f, br * 0.58f, 1.3f);
        g.setColour (col::text);
        g.fillPath (p, juce::AffineTransform::rotation (angle).translated (c.x, c.y));
    }

    // Interruttore ON: LED rosso con alone
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& b, bool, bool) override
    {
        auto r = b.getLocalBounds().toFloat();
        const bool on = b.getToggleState();
        const float d = 11.0f;
        const juce::Rectangle<float> led = juce::Rectangle<float> (d, d).withCentre ({ r.getRight() - d * 0.5f - 3.0f, r.getCentreY() });

        g.setColour (juce::Colour (0xff0a0b0d));
        g.fillEllipse (led.expanded (2.0f));
        if (on)
        {
            juce::ColourGradient glow (col::ledRed.withAlpha (0.5f), led.getCentreX(), led.getCentreY(),
                                       juce::Colours::transparentBlack, led.getCentreX() + d * 1.5f, led.getCentreY(), true);
            g.setGradientFill (glow);
            g.fillEllipse (led.expanded (d));
            g.setColour (col::ledRed);
        }
        else
            g.setColour (juce::Colour (0xff4a1612));
        g.fillEllipse (led);
        if (on)
        {
            g.setColour (juce::Colours::white.withAlpha (0.55f));
            g.fillEllipse (led.getX() + d * 0.2f, led.getY() + d * 0.12f, d * 0.32f, d * 0.26f);
        }

        g.setColour (col::text);
        g.setFont (10.0f);
        g.drawText (b.getButtonText(), r.withTrimmedRight (d + 8.0f), juce::Justification::centredRight, false);
    }
};
} // namespace ui
