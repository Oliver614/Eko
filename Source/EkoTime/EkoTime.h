#pragma once

/*
* Class based off the rounds reverb reflektor panel.
* Designed for shaping incoming times to even disperse reverb diffusion. 
*/

#include <stdint.h>

class EkoTime
{
public:

	EkoTime() {}
	~EkoTime() {}

private:

	inline void diffShape(const float& diff, float* diffArray);
	inline float exp_log(const float& in, const float& shp);
	inline float crossFade(const float& in1, const float& in2, const float& x);
	inline float shaperStage(const float& in, const float& xB, const float& xA, const float& shp, const float& levA, const float& levB, float& greaterThan);
	inline float shaper(const float& time, const float& shape, const float& spread);

public:

	inline float shapeTimes(const float& spr, const float& center, const float& time);

};

/*
* Returns the diffusion times for even diffusion and preventing echoing
*/
inline void EkoTime::diffShape(const float& diff, float* diffArray)
{
	diffArray[0] = 0.66f * diff;
	diffArray[1] = 0.0286714f + ((0.66f - 0.0286714f) * diff);
	diffArray[2] = 0.0571429f + ((0.66f - 0.0571429f) * diff);
	diffArray[3] = 0.0857143f + ((0.66f - 0.0857143f) * diff);
	diffArray[4] = 0.0857143f + ((0.66f - 0.0857143f) * diff);
	diffArray[5] = 0.0571429f + ((0.66f - 0.0571429f) * diff);
	diffArray[6] = 0.0286714f + ((0.66f - 0.0286714f) * diff);
	diffArray[7] = 0.66f * diff;
}

/*
* Finds the exponent of log, similar to f (x) = x ^ 1 / n but
* symmetrical to the axis f (x) = 1-x
*/
inline float EkoTime::exp_log(const float& in, const float& shp)
{
	
	float strongness = shp > 0.f ? (1 / (1 - shp)) + -1 : shp;
	float negStrongness = strongness * -1;

	float xIn = in > 1e-9f ? in : 0.f;

	float a = (1 / strongness) + 1;

	float b = xIn + a;
	float c = b * negStrongness;
	float d = c + strongness;
	float e = a / d;
	float f = e + a;
	float h = in > 1e-9f ? f : 0.f;

	return h;
}

/*
* Crossfades the input x between the two input levels
*/
inline float EkoTime::crossFade(const float& in1, const float& in2, const float& x)
{
	float a = (in2 - in1);
	float b = a * x;
	float c = in1 + b;
	return c;
}

/*
* Shapes the time as long as the input (in) is greater than xB
* else it passes to the next shaper
*/
inline float EkoTime::shaperStage(const float& in, const float& xB, const float& xA, const float& shp, const float& levA, const float& levB, float& greaterThan)
{
	if (in >= xB)
	{
		greaterThan = in;
		return 0.f;
	}
	greaterThan = 0.f;

	float a = in >= xB ? 0.f : in;
	float b = a - xA;
	float c = 1.f/(xB - xA);
	float d = b * c;
	float e = exp_log(d, shp);
	return crossFade(levA, levB, e);
}

/*
* if the first shaper fails or passes the input to the greater than output it then moves onto the second stage of shaping.
*/
inline float EkoTime::shaper(const float& time, const float& X, const float& spread)
{
	float greaterThanStageA = 0.f, greaterThatStageB = 0.f;
	float a = shaperStage(time, X, 0.f, spread, 0.f, X, greaterThanStageA);
	if (greaterThanStageA == 0.f) return a;
	float b = shaperStage(greaterThanStageA, 1.f, X, spread * -1, X, 1.f, greaterThatStageB);
	return b;
}

/*
* Translates spr to a value between 0.3 and 0.999 then executes the shaper function.
*/
inline float EkoTime::shapeTimes(const float& shape /*Spread*/, const float& spr /*Center*/, const float& time)
{
	float sprSquared = (spr * spr);
	float spread = ((0.3f - 0.999f) * sprSquared) + 0.999f;

	return shaper(time, shape, spread);


}
