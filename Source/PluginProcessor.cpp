/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EkoAudioProcessor::EkoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mSizeSmooth = 0.f;
    mScaledFeedback = 0.f;
    mSpreadSmooth = 0.f;
    mCenterSmooth = 0.f;
    mDelayTimeSmooth = 0.f;

    addParameter(mPreDelayTime = new juce::AudioParameterFloat("predelaytime", "Pre Delay Time", 0.f, 2.f, 0.f));
    addParameter(mPreDelayFeedback = new juce::AudioParameterFloat("predelayFeedback", "Pre Delay Feedback", 0.f, 1.f, 0.f));
    addParameter(mDiffusion = new juce::AudioParameterFloat("diffusion", "Diffusion", 0.f, 1.f, 0.5f));
    addParameter(mFeedback = new juce::AudioParameterFloat("feedback", "Feedback", 0.f, 1.f, 0.5f));
    addParameter(mSize = new juce::AudioParameterFloat("size", "Size", 0.f, 1.f, 0.5f));
    addParameter(mSpread = new juce::AudioParameterFloat("spread", "Spread", 0.f, 1.f, 0.5f));
    addParameter(mCenter = new juce::AudioParameterFloat("center", "Center", 0.f, 1.f, 0.5f));
    addParameter(mLP = new juce::AudioParameterFloat("lp", "LP", 0.f, 1.f, 1.f));
    addParameter(mHP = new juce::AudioParameterFloat("hp", "HP", 0.f, 1.f, 0.f));
    addParameter(mColourCutoff = new juce::AudioParameterFloat("colourCutoff", "Colour", -1.f, 1.f, 0.f));
    addParameter(mColourEmphasis = new juce::AudioParameterFloat("colourEmphasis", "Emphasis", 0.f, 1.f, 0.f));
    addParameter(mMix = new juce::AudioParameterFloat("mix", "Mix", 0.f, 1.f, 0.5f));
    addParameter(mMute = new juce::AudioParameterBool("mute", "Mute", false));

}

EkoAudioProcessor::~EkoAudioProcessor()
{
}

//==============================================================================
const juce::String EkoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EkoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EkoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EkoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EkoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EkoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EkoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EkoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EkoAudioProcessor::getProgramName (int index)
{
    return {};
}

void EkoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EkoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mEko.init(sampleRate, samplesPerBlock);
    
}

void EkoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EkoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EkoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {

        mSizeSmooth = mSizeSmooth - 0.001f * (mSizeSmooth - *mSize);
        mSpreadSmooth = mSpreadSmooth - 0.001f * (mSpreadSmooth - *mSpread);
        mCenterSmooth = mCenterSmooth - 0.001f * (mCenterSmooth - *mCenter);
        mDelayTimeSmooth = mDelayTimeSmooth - 0.001f * (mDelayTimeSmooth - *mPreDelayTime);
        mScaledFeedback = *mFeedback * -0.95f;

        float reverbInArrayL[8] = { 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f };
        float reverbInArrayR[8] = { 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f };

        for (int i = 0; i < 8; i++)
        {
            reverbInArrayL[i] = mEkoTime.shapeTimes(mSpreadSmooth, mCenterSmooth, mTimeArrayL[i]);
            reverbInArrayR[i] = mEkoTime.shapeTimes(mSpreadSmooth, mCenterSmooth, mTimeArrayR[i]);
        }


        

        float lout, rout;
        mEko.processReverb(leftChannel[i], rightChannel[i], reverbInArrayL, reverbInArrayR, *mDiffusion, mSizeSmooth, *mLP, *mHP, mScaledFeedback, *mMix, *mMute, mDelayTimeSmooth, *mFeedback, *mColourCutoff, *mColourEmphasis, lout, rout);

        buffer.setSample(0, i, *mMute ? leftChannel[i] : lout);
        buffer.setSample(1, i, *mMute ? rightChannel[i] : rout);

    }
}

//==============================================================================
bool EkoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EkoAudioProcessor::createEditor()
{
    return new EkoAudioProcessorEditor (*this);
}

//==============================================================================
void EkoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EkoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EkoAudioProcessor();
}
