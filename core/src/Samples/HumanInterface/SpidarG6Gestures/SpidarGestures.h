#ifndef SGESTURES_H
#define SGESTURES_H

class SpidarGestures
{
private:
	bool leftSideVFlag;
	bool rightSideVFlag;
	bool negSideFlag;
	bool posSideFlag;
	double evel;
	double pepos;
	double nepos;
	double posz1;
	double velz1;
	double lowpass;
	double time;
	
	void detectLeftVel(double velz1, double velz2);
	void detectRightVel(double velz1, double velz2);
	double eventVelTrigger();

	void detectPos(double pz1, double pz2);
	void detectNeg(double pz1, double pz2);
	double eventPosTrigger();
	double eventNegTrigger();

	void nextPoints(double posz, double velz);


public:
	double velThreshold;
	double posThreshold;
	double peakThreshold;
	double negThreshold;
	double alpha;

	SpidarGestures();
	int slideEventTrigger(double pz, double pvz, double time);

};


#endif