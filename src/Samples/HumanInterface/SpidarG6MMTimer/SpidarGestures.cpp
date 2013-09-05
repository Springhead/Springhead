#include "SpidarGestures.h"

SpidarGestures::SpidarGestures()
{
	this->posz1 = 0.0;
	this->velz1 = 0.0;
	this->evel = 0.0;
	this->pepos = 0.0;
	this->nepos = 0.0;
	this->leftSideVFlag = false;
	this->rightSideVFlag = false;
	this->negSideFlag = false;
	this->posSideFlag = false;
	this->lowpass = 0.0;
	this->time = 0.0;

	this->velThreshold = 0.4;
	this->posThreshold = 0.05;
	this->negThreshold = -0.05;
	this->peakThreshold = 1.6;
	this->alpha = 0.9;
}

void SpidarGestures::detectLeftVel(double velz1, double velz2)
{
	if ((velz1 < velThreshold) && (velz2 >= velThreshold) )
	{
		leftSideVFlag = true;
	}
}

void SpidarGestures::detectRightVel(double velz1, double velz2)
{
	if (!leftSideVFlag)
		return;

	if ((velz1 >= velThreshold) && (velz2 < velThreshold))
	{
		rightSideVFlag = true;
	}
}

double SpidarGestures::eventVelTrigger()
{
	if (leftSideVFlag && rightSideVFlag)
	{
		leftSideVFlag = false;
		rightSideVFlag = false;
		return time;
	}
	else
		return 0;
}

void SpidarGestures::detectPos(double pz1, double pz2)
{
	if ((pz1 < posThreshold) && (pz2 >= posThreshold) )
	{
		posSideFlag = true;
	}
}

void SpidarGestures::detectNeg(double pz1, double pz2)
{
	if ((pz1 >= negThreshold) && (pz2 < negThreshold))
	{
		negSideFlag = true;
	}
}

double SpidarGestures::eventPosTrigger()
{
	if (posSideFlag)
	{
		posSideFlag = false;
		return time;
	}
	else
		return 0;
}

double SpidarGestures::eventNegTrigger()
{
	if (negSideFlag)
	{
		negSideFlag = false;
		return time;
	}
	else
		return 0;
}

void SpidarGestures::nextPoints(double posz, double velz)
{
	posz1 = posz;
	velz1 = velz;
}

int SpidarGestures::slideEventTrigger(double pz, double pvz, double time)
{
	this->time = time;
	lowpass = ((1.0 - alpha) * lowpass) + (alpha * pz);
	pz = lowpass;

	detectLeftVel(velz1, pvz);
	detectRightVel(velz1, pvz);

	detectPos(posz1, pz);
	detectNeg(posz1, pz);

	double tpepos = eventPosTrigger();
	if (tpepos != 0)
		pepos = tpepos;

	double tnepos = eventNegTrigger();
	if (tnepos != 0)
		nepos = tnepos;

	double tevel = eventVelTrigger();  
	if (tevel != 0)
		evel = tevel;

	short final = 0;
	if ((evel != 0) && (pepos != 0))
	{
		if (evel - pepos <= peakThreshold) 
			final = 1;

		pepos = 0;
		evel = 0;
	}
	else if ((evel != 0) && (nepos != 0))
	{
		if (evel - nepos <= peakThreshold) 
			final = -1;

		nepos = 0;
		evel = 0;
	}

	nextPoints(pz, pvz);

	return final;
}
