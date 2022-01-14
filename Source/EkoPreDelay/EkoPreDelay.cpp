#pragma once

#include "EkoPreDelay.h"

EkoPreDelay::EkoPreDelay()
{
	mFeedbackLeft = 0.f;
	mFeedbackRight = 0.f;
	mSampleRate = 0.f;
}

EkoPreDelay::~EkoPreDelay()
{

}

void EkoPreDelay::init(const float& sampleRate, const float& maxDelayTime)
{
	uint32_t bufferSizeInSamples = sampleRate * maxDelayTime;
	mDelayLineLeft.init(bufferSizeInSamples, sampleRate);
	mDelayLineRight.init(bufferSizeInSamples, sampleRate);
	mSampleRate = sampleRate;
}

void EkoPreDelay::process(const float& sampleLeft, const float& sampleRight, const float& delayTime, const float& feedback, float& returnLeft, float& returnRight)
{
	float feedbackAmount = delayTime > 0.f ? feedback * 0.95f : 0.f;
	float lessOneSample = 1.f / (mSampleRate);
	mDelayLineLeft.write(sampleLeft + mFeedbackLeft);
	mDelayLineRight.write(sampleRight + mFeedbackRight);

	float left = mDelayLineLeft.read(delayTime + lessOneSample);
	float right = mDelayLineRight.read(delayTime + lessOneSample);

	mFeedbackLeft = left * feedbackAmount;
	mFeedbackRight = right * feedbackAmount;

	returnLeft = left;
	returnRight = right;
}
