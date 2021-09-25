#pragma once

#include "Filter.h"

void EkoFilter::init(float sampleRate, float samplesPerBlock)
{
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = mNumChannels;

	filter.prepare(spec);
}

void EkoFilter::reset()
{
	filter.reset();
}

void EkoFilter::setTypeLP()
{
	filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

void EkoFilter::setTypeHP()
{
	filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
}

void EkoFilter::setTypeBP()
{
	filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
}

void EkoFilter::setTypeVariable(float cutoffValue)
{

	cutoffValue <= 0.f ? filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass) : filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);


}

void EkoFilter::setCutoff(float cutoff)
{
	filter.setCutoffFrequency(cutoff);
}

void EkoFilter::setResonance(float resonance)
{
	float res = resonance / 1.4142135623731;
	filter.setResonance(resonance);
}

float EkoFilter::process(float sample)
{
	return filter.processSample(0, sample);
}


