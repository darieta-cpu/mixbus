#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include "Look.h"

namespace ui
{
// Meter di livello stereo a LED segmentati (20 segmenti da 3 dB, -57..0 dBFS) con peak hold.
// push() va chiamato dal timer della GUI a 60 Hz: salita immediata, caduta ~60 dB/s.
class LevelMeter : public juce::Component
{
public:
    static constexpr int kSegs = 20;

    void push (float linL, float linR)
    {
        update (0, linL);
        update (1, linR);
        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        auto scale = r.removeFromLeft (26.0f);
        const float gap = 5.0f, segGap = 2.0f;
        const float barW = (r.getWidth() - gap) * 0.5f;
        const float segH = (r.getHeight() - segGap * (float) (kSegs - 1)) / (float) kSegs;
        auto segY = [&] (int i) { return r.getBottom() - (float) (i + 1) * segH - (float) i * segGap; };

        g.setFont (10.0f);
        for (float db : { 0.0f, -6.0f, -12.0f, -24.0f, -36.0f, -48.0f })
        {
            const int i = (int) std::round ((db + 57.0f) / 3.0f);
            g.setColour (col::dim);
            g.drawText (juce::String ((int) db), scale.withY (segY (i) + segH * 0.5f - 6.0f).withHeight (12.0f),
                        juce::Justification::centredRight, false);
        }

        for (int ch = 0; ch < 2; ++ch)
        {
            const float x = r.getX() + (float) ch * (barW + gap);
            int holdSeg = -1;
            for (int i = 0; i < kSegs; ++i) if (holdDb[ch] >= threshold (i)) holdSeg = i;

            for (int i = 0; i < kSegs; ++i)
            {
                const juce::Colour base = i >= 19 ? col::ledRed : (i >= 16 ? col::ledYellow : col::ledGreen);
                const bool lit = levelDb[ch] >= threshold (i) || i == holdSeg;
                g.setColour (lit ? base : base.withAlpha (0.13f));
                g.fillRoundedRectangle (x, segY (i), barW, segH, 1.0f);
            }
        }
    }

private:
    static float threshold (int i) { return -57.0f + 3.0f * (float) i; }

    void update (int ch, float lin)
    {
        const float db = juce::Decibels::gainToDecibels (lin, -100.0f);
        levelDb[ch] = db > levelDb[ch] ? db : juce::jmax (db, levelDb[ch] - 1.0f);

        if (db >= holdDb[ch])            { holdDb[ch] = db; holdFrames[ch] = 60; }   // hold ~1 s
        else if (holdFrames[ch] > 0)     --holdFrames[ch];
        else                             holdDb[ch] = juce::jmax (levelDb[ch], holdDb[ch] - 1.0f);
    }

    float levelDb[2] { -100.0f, -100.0f }, holdDb[2] { -100.0f, -100.0f };
    int holdFrames[2] { 0, 0 };
};

// Meter di riduzione di guadagno a LED (20 segmenti, 0..maxDb), si accende dall'alto verso il basso
class GRMeter : public juce::Component
{
public:
    static constexpr int kSegs = 20;

    GRMeter (float maxDbIn, juce::Colour) : maxDb (maxDbIn) {}

    void push (float grDb)
    {
        gr = grDb > gr ? grDb : juce::jmax (grDb, gr - 0.5f);    // caduta ~30 dB/s a 60 Hz
        repaint();
    }

    void paint (juce::Graphics& g) override
    {
        auto r = getLocalBounds().toFloat();
        auto readout = r.removeFromBottom (18.0f);
        auto scale = r.removeFromLeft (28.0f);
        const float segGap = 2.0f;
        const float segH = (r.getHeight() - segGap * (float) (kSegs - 1)) / (float) kSegs;
        const float barW = juce::jmin (r.getWidth(), 22.0f);
        const float step = maxDb / (float) kSegs;
        auto segY = [&] (int i) { return r.getY() + (float) i * (segH + segGap); };

        g.setFont (10.0f);
        for (int k = 0; k <= 4; ++k)
        {
            const float yTop = r.getY() + (float) (k * 5) * (segH + segGap);
            const float y = juce::jlimit (r.getY(), r.getBottom() - 12.0f, yTop - 6.0f);
            g.setColour (col::dim);
            g.drawText (k == 0 ? juce::String ("0") : "-" + juce::String ((int) std::round (maxDb * (float) k / 4.0f)),
                        scale.withY (y).withHeight (12.0f), juce::Justification::centredRight, false);
        }

        for (int i = 0; i < kSegs; ++i)
        {
            const juce::Colour base = i >= 15 ? col::ledRed : col::ledAmber;
            const bool lit = gr > ((float) i + 0.3f) * step;
            g.setColour (lit ? base : base.withAlpha (0.13f));
            g.fillRoundedRectangle (r.getX(), segY (i), barW, segH, 1.0f);
        }

        g.setColour (col::text);
        g.setFont (11.0f);
        g.drawText ("-" + juce::String (gr, 1) + " dB", readout.withLeft (0.0f), juce::Justification::centred, false);
    }

private:
    float maxDb, gr = 0.0f;
};
} // namespace ui
