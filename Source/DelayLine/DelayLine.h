#pragma once

#include <vector>

class DelayLine
{
public:
	DelayLine() 
	{
		mCircularBufferSizeInSamples = 0;
		mCircularBufferPosition = 0;
		mSampleRate = 0.f;
	}
	~DelayLine() {}

	inline void init(const float& sampleRate, const float& bufferSizeInSeconds);
	inline void init(const uint32_t& bufferSizeInSamples, const float& sampleRate);
	inline float read(const float& readTime);
	inline float interpolate(const float& x, const float& x1, const float& fraction);
	inline void write(const float& sample);
	inline void nextPos();
	inline uint32_t getSize();

private:

	std::vector<float> mCircularBuffer;
	int32_t mCircularBufferSizeInSamples;
	int32_t mCircularBufferPosition;
	float mSampleRate;
};

inline void DelayLine::init(const float& sampleRate, const float& bufferSizeInSeconds)
{
	mSampleRate = sampleRate;
	mCircularBufferSizeInSamples = sampleRate * bufferSizeInSeconds;
	mCircularBuffer = std::vector<float>(mCircularBufferSizeInSamples);
	std::fill(mCircularBuffer.begin(), mCircularBuffer.end(), 0.f);
}

inline void DelayLine::init(const uint32_t& bufferSizeInSamples, const float& sampleRate)
{
	mSampleRate = sampleRate;
	mCircularBufferSizeInSamples = bufferSizeInSamples;
	mCircularBuffer = std::vector<float>(bufferSizeInSamples);
	std::fill(mCircularBuffer.begin(), mCircularBuffer.end(), 0.f);
}

inline void DelayLine::write(const float& sample)
{
	mCircularBuffer[mCircularBufferPosition] = sample;
	nextPos();
}

inline float DelayLine::read(const float& readTime)  // No Interpolation
{
	float readTimeInSamples = readTime * mSampleRate;
	int delayReadHead = mCircularBufferPosition - readTimeInSamples;
	if (delayReadHead < 0) delayReadHead += mCircularBufferSizeInSamples;

	return mCircularBuffer[delayReadHead];
}

inline float DelayLine::interpolate(const float& x, const float& x1, const float& fraction)
{
	return (1.f - fraction) * x + fraction * x1;
}

inline void DelayLine::nextPos()
{
	mCircularBufferPosition++;
	mCircularBufferPosition %= mCircularBufferSizeInSamples;
}

inline uint32_t DelayLine::getSize()
{
	return mCircularBufferSizeInSamples;
}




