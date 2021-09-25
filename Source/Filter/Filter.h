#pragma once


#include <JuceHeader.h>

class EkoFilter
{
public:

	EkoFilter() {}
	~EkoFilter() {}

	void init(float sampleRate, float samplesPerBlock);
	void reset();
	void setTypeLP();
	void setTypeHP();
	void setTypeBP();
	void setTypeVariable(float cutoffValue);
	void setCutoff(float cutoff);
	void setResonance(float resonance);
	float process(float sample);

private:

	static constexpr int mNumChannels = 1;
	juce::dsp::StateVariableTPTFilter<float> filter;
	juce::dsp::ProcessSpec spec;

};