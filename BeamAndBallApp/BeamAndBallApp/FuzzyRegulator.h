#pragma once

#include "pch.h"

struct BandB_Data
{
public:
	BandB_Data() {
		beamLength = 5.0f;
		ballRadius = 0.5f;
		ballWeight = 12.1736715327f;
		servoRadius = 1.0f;
		servoMaxSpeed = 2.0f;
		gravitationalAcceleration = 9.81f;
	}

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

