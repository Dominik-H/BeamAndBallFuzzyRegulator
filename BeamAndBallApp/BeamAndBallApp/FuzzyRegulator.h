#pragma once

#include "pch.h"

class FuzzyRegulator
{
public:
	FuzzyRegulator();
	~FuzzyRegulator();

	void Init();
	float getAngle(float odchylka, float rozdiel);

private:
	fl::Engine* engine;
	fl::InputVariable* odchylka;
	fl::InputVariable* deltaE;
	fl::OutputVariable* angle;
};

