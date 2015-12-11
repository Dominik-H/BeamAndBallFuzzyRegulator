#pragma once

#include "pch.h"

struct BandB_Data
{
public:
	float beamLength;

	float ballRadius;
	float ballWeight;

	float servoRadius;
	float servoMaxSpeed; // in radians per second

	float gravitationalAcceleration;
};

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

