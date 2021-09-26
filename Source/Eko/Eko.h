#pragma once

#include "../DelayLine/DelayLine.h"
#include "../Filter/Filter.h"
#include "../EkoPreDelay/EkoPreDelay.h"

struct Diffuser
{
	Diffuser()
	{
		mMaxDiffusionTime = 0.f;
		mSampleRate = 0.f;
		mToWrite = 0.f;
		mZ = 0.f;
		mOut = 0.f;

	}

	~Diffuser() {}
	void init(const float& sampleRate, const float& maxDiffusuionTime, float samplesPerBlock);

	void diffuse(const float& input, const float& feedback, float& toWrite, const float& z, float& out);
	float twoPassDiffuse(const float& input, const float& time, const float& feedback, float& outputArray);

	DelayLine mDelayLine;

	float mMaxDiffusionTime;
	float mSampleRate;
	float mToWrite, mZ, mOut;

};

//==============================================================================================================================

struct MultiTapDiffusion
{
	MultiTapDiffusion() {}
	~MultiTapDiffusion() {}

	void init(const float& sampleRate, const float& samplesPerBlock);
	void processMultiTapDiffusion(const float& in, float& feedback, float* t, float* df, float* out);

	static constexpr uint32_t mNumberOfDiffusionLines = 8;
	const float mMaxDiffuserTime = 2.f;
	float mSampleRate = 0.f;

	Diffuser mDiffuserArray[mNumberOfDiffusionLines];
};

//==============================================================================================================================

class Eko
{
public:
	Eko();
	~Eko() {}

	void init(const float& sampleRate, const float& samplesPerBlock);

private:

	void spreadParams(const float& spread);
	void diffusionCalcs(const float& diff);

	void phase();
	float sclClip(const float& ctl, const float& min, const float& max);
	void scl(const float& tx, const float& tPlus, float& tx2, float& lx);
	float lfoStage(const float& i, const float& phase, const float& n, const float& t, const float& tx, const float& lx);
	void lfo(const float& txIn, const float& lfoAmt, float* t1, float* t2);

	void eq(const float& hpCutoff, const float& lpCutoff, float& outL, float& outR);
	void fb(const float& LIn, const float& RIn, const float& fb, float& returnLeft, float& feedbackLeft, float& returnRight, float& feedbackRight);
	void read(float& L, float& R, const float& diffusion);

	void colour(const float& sampleLeft, const float& sampleRight, const float& cutoff, const float& emphasis, float& returnLeft, float& returnRight);
	void mix(const float& l0, const float& r0, const float& l1, const float& r1, const float& fb, const float& mix, float& L, float& R);

public:

	void processReverb(const float& lIn, const float& rIn, float* lArray, float* rArray, const float& diffusion, const float& size, const float& LPAmmount, const float& HPAmmount, 
		const float& fbck, const float& mix, const float& preDelayTime, const float& preDelayFeedback, const float& colourCutoff, const float& colourEmphasis, float& lOut, float& rOut);

private:

	EkoPreDelay mPreDelay;
	MultiTapDiffusion mMultiTapDelayL, mMultiTapDelayR;
	EkoFilter mEqFilterHighpassLeft, mEqFilterHighpassRight, mEqFilterLowpassLeft, mEqFilterLowpassRight;
	EkoFilter mColourFilterLeft, mColourFilterRight;

	static constexpr uint32_t mMaxDiffusionSteps = 8;
	float mPhase;

	float mPassThroughLeft, mPassThroughRight;
	float mSampleLeft, mSampleRight;

	bool mSwitchLfoDirectionUp;

	float mHpCutoff, mLpCutoff, mLfoAmount, mTimeVariation, mDiffusionAmount, mSampleRate, mDiffusionFeedbackLeft, mDiffusionFeedbackRight;
	float tL[mMaxDiffusionSteps], tR[mMaxDiffusionSteps], df[mMaxDiffusionSteps];
	float mLtoScanX8[mMaxDiffusionSteps], mRtoScanX8[mMaxDiffusionSteps];
	float mDiffusionArray[mMaxDiffusionSteps];
};

