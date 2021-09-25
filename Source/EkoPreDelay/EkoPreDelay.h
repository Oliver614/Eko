#pragma once

#include "../DelayLine/DelayLine.h"

class EkoPreDelay
{
public:

	EkoPreDelay();
	~EkoPreDelay();

	void init(const float& sampleRate, const float& maxDelayTime);
	void process(const float& sampleLeft, const float& sampleRight, const float& delayTime, const float& feedback, float & returnLeft, float& returnRight);

private:

	float mFeedbackLeft, mFeedbackRight, mSampleRate;
	DelayLine mDelayLineRight, mDelayLineLeft;

};
