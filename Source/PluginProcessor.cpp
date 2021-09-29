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
                       ), parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    mSizeSmooth = 0.f;
    mScaledFeedback = 0.f;
    mSpreadSmooth = 0.f;
    mCenterSmooth = 0.f;
    mDelayTimeSmooth = 0.f;

    mPreDelayTime       = parameters.getRawParameterValue("predelaytime");
    mPreDelayFeedback = parameters.getRawParameterValue("predelayFeedback");
    mDiffusion = parameters.getRawParameterValue("diffusion");
    mFeedback = parameters.getRawParameterValue("feedback");
    mSize = parameters.getRawParameterValue("size");
    mSpread = parameters.getRawParameterValue("spread");
    mCenter = parameters.getRawParameterValue("center");
    mLP = parameters.getRawParameterValue("lp");
    mHP = parameters.getRawParameterValue("hp");
    mMix = parameters.getRawParameterValue("colourCutoff");
    mColourCutoff = parameters.getRawParameterValue("colourEmphasis");
    mColourEmphasis = parameters.getRawParameterValue("mix");
    
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

    mPreDelayTime = parameters.getRawParameterValue("predelaytime");
    mPreDelayFeedback = parameters.getRawParameterValue("predelayFeedback");
    mDiffusion = parameters.getRawParameterValue("diffusion");
    mFeedback = parameters.getRawParameterValue("feedback");
    mSize = parameters.getRawParameterValue("size");
    mSpread = parameters.getRawParameterValue("spread");
    mCenter = parameters.getRawParameterValue("center");
    mLP = parameters.getRawParameterValue("lp");
    mHP = parameters.getRawParameterValue("hp");
    mMix = parameters.getRawParameterValue("mix");
    mColourCutoff = parameters.getRawParameterValue("colourCutoff");
    mColourEmphasis = parameters.getRawParameterValue("colourEmphasis");

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

        float mutatedTimingArrayLeft [8] = { 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f };
        float mutatedTimingArrayRight[8] = { 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f };

        for (int i = 0; i < 8; i++)
        {
            mutatedTimingArrayLeft[i] = mEkoTime.process(mSpreadSmooth, mCenterSmooth, mTimeArrayL[i]);
            mutatedTimingArrayRight[i] = mEkoTime.process(mSpreadSmooth, mCenterSmooth, mTimeArrayR[i]);
        }

        float lout, rout;
        mEko.process(leftChannel[i], rightChannel[i], mutatedTimingArrayLeft, mutatedTimingArrayRight, *mDiffusion, mSizeSmooth, *mLP, *mHP, mScaledFeedback, *mMix, mDelayTimeSmooth, *mPreDelayFeedback, *mColourCutoff, *mColourEmphasis, lout, rout);

        buffer.setSample(0, i, lout);
        buffer.setSample(1, i, rout);

    }
}

//==============================================================================
bool EkoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EkoAudioProcessor::createEditor()
{
    return new EkoAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void EkoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void EkoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout EkoAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat>("predelaytime"       , "Pre Delay Time", 0.f, 2.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("predelayFeedback"   , "Pre Delay Feedback", 0.f, 1.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("diffusion"          , "Diffusion", 0.f, 1.f, 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("feedback"           , "Feedback", 0.f, 1.f, 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("size"               , "Size", 0.f, 1.f, 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("spread"             , "Spread", 0.f, 1.f, 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("center"             , "Center", 0.f, 1.f, 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("lp"                 , "LP", 0.f, 1.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("hp"                 , "HP", 0.f, 1.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("colourCutoff"       , "Colour", -1.f, 1.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("colourEmphasis"     , "Emphasis", 0.f, 1.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("mix"                , "Mix", 0.f, 1.f, 0.5f));
    return params;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EkoAudioProcessor();
}
