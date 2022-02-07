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

	float exp_log(const float& in, const float& shp);
	float crossFade(const float& in1, const float& in2, const float& x);
	float shaperStage(const float& in, const float& xB, const float& xA, const float& shp, const float& levA, const float& levB, float& greaterThan);
	float shaper(const float& time, const float& shape, const float& spread);

public:

	float process(const float& spr, const float& center, const float& time);

};


