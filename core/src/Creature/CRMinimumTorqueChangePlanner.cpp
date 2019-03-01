/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

#include <Creature/CRMinimumTorqueChangePlanner.h>
#include <Creature/CRMinimumJerkTrajectory.h>

namespace Spr {;

ViaPoint::ViaPoint() {}

ViaPoint::ViaPoint(Posed p, double t) {}

ViaPoint::ViaPoint(Posed p, SpatialVector v, SpatialVector a, double t) {}

//N-Simple Moving Average LPF
template<class T>
#ifdef _MSC_VER
static PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
#else
PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::NSMA(PTM::VMatrixRow<T> input, int n, double mag, PTM::VVector<T> s) {
#endif
	PTM::VMatrixRow<T> output;
	//DSTR << input.height() << " " << input.width() << std::endl;
	output.resize(input.height(), input.width());
	output.clear();
	DSTR << n << std::endl;
	//std::ofstream filterRecord("C:/Users/hirohitosatoh/Desktop/logs/filter.csv");

	if (input.width() < (size_t)n) {
		return input;
	}
	else {
		for (size_t i = 0; i < input.height(); i++) {
			size_t j = 0;
			T sum = s[i] * n;
			DSTR << s[i] << std::endl;
			for (; j < (size_t)n; j++) {
				sum = sum + input[i][j] - s[i];
				output[i][j] = sum / n;
			}
			for (; j < input.width(); j++) {
				sum = sum + input[i][j] - input[i][j - n];
				output[i][j] = mag * sum / n;
			}
		}
		/*
		for (size_t i = 0; i < input.width(); i++) {
		for (size_t j = 0; j < input.height(); j++) {
		filterRecord << input[j][i] << "," << output[j][i] << ",";
		}
		filterRecord << std::endl;
		}
		*/
	}
	return output;
}

template<class T>
#ifdef _MSC_VER
static PTM::VVector<T> CRTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
#else
PTM::VVector<T> CRTrajectoryPlanner::LPF::centerNSMAv(PTM::VVector<T> input, int n, double mag, T initial) {
#endif
	PTM::VVector<T> output;
	output.resize(input.size());

	if ((n % 2) == 0) n++;
	int half = n / 2;

	size_t j = 0;
	T sum = initial * (half + 1);
	for (int k = 0; k < half; k++) {
		sum += input[k];
	}
	DSTR << initial << std::endl;
	for (; j < (size_t)(half + 1); j++) {  //初期トルクが残ってる部分のループ
		sum = sum + input[j + half] - initial;
		output[j] = mag * sum / n;
	}
	for (; j < input.size() - half; j++) {
		sum = sum + input[j + half] - input[j - half - 1];
		output[j] = mag * sum / n;
	}
	for (; j < input.size(); j++) {
		sum = sum + input[input.size() - 1] - input[j - half - 1];
		output[j] = mag * sum / n;
	}
	return output;
}

template<class T>
#ifdef _MSC_VER
static PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
#else
PTM::VMatrixRow<T> CRTrajectoryPlanner::LPF::weighted(PTM::VMatrixRow<T> input, PTM::VVector<T> s, PTM::VVector<double> w) {
#endif
	PTM::VMatrixRow<T> output;
	//DSTR << input.height() << " " << input.width() << std::endl;
	output.resize(input.height(), input.width());
	output.clear();

	if (input.width() < 2) {
		return input;
	}
	for (size_t i = 0; i < input.height(); i++) {
		double weight = 0.3 * w[i];
		output[i][0] = input[i][0] + weight * (s[i] + input[i][1] - 2 * input[i][0]);
		for (size_t j = 1; j < input.width() - 1; j++) {
			output[i][j] = input[i][j] + weight * (input[i][j - 1] + input[i][j + 1] - 2 * input[i][j]);
		}
		output[i][input.width() - 1] = input[i][input.width() - 1] + weight * (input[i][input.width() - 2] - input[i][input.width() - 1]);
	}
	return output;
}

template <class T>
#ifdef _MSC_VER
static PTM::VVector<T> CRTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
#else
PTM::VVector<T> CRTrajectoryPlanner::LPF::weightedv(PTM::VVector<T> input, T initial, double w, double r) {
#endif
	PTM::VVector<T> output;
	output.resize(input.size());

	if (input.size() < 2) {
		return input;
	}
	double weight = 0.3 * w * r;
	//double weight = 0.3 * r;
	output[0] = input[0] + weight * (initial + input[1] - 2 * input[0]);
	for (int j = 1; j < (int)input.size() - 1; j++) {
		output[j] = input[j] + weight * (input[j - 1] + input[j + 1] - 2 * input[j]);
	}
	output[input.size() - 1] = input[input.size() - 1] + weight * (input[input.size() - 2] - input[input.size() - 1]);
	return output;
}

}