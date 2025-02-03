#include <Physics/SprPHLuGreParametersData.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <springhead.h>

namespace Spr {

	//���̃N���X�̗B��̃C���X�^���X
	PHLuGreParametersData* PHLuGreParametersData::instance = nullptr;

	//�g�p����p�����[�^��ύX����
	//����:
	//    material (std::string): �p�����[�^���\���ގ��̖���
	void PHLuGreParametersData::changeCurrentParameters(std::string material) {
		std::map<std::string, PHLuGreParameters>* data = &(getInstance()->lugreParametersMap);
		if (data->find(material) != data->end()) {
			//�w�肳�ꂽ�ގ��̃p�����[�^�����݂���Ƃ�
			getInstance()->currentParameters = data->at(material);
		}
		else {
			//�w�肳�ꂽ�ގ��̃p�����[�^�����݂��Ȃ��Ƃ�
			DSTR << "�w�肳�ꂽ�ގ������݂��܂���!!!" << std::endl;
		}
	}

	//CSV�t�@�C������p�����[�^�̃f�[�^��ǂݍ��݁A�ǉ����Ă����@�\
	void PHLuGreParametersData::addParametersFromFile(std::string filename) {

		std::string thisFilePath = std::string(__FILE__);//���̃t�@�C���̃p�X���擾
		std::string thisDirectoryPath = thisFilePath.erase(thisFilePath.find_last_of("\\") + 1);//���̃t�@�C���̑��݂���f�B���N�g�����擾(���Ɉˑ��������Ȃ̂Œ���)
		std::ifstream file(thisDirectoryPath + filename);//�t�@�C�����J��
		if (!file.is_open())return;//�t�@�C�����J���Ȃ������ꍇ

		std::vector<std::string> parameterNames;//�p�����[�^���̔z��
		bool isFirstLine = true;//�ŏ��̍s�ł��邩�ǂ���
		std::string line;
		while (std::getline(file, line)) {
			//1�s���̕�����line�ɓ����Ă���

			if (line.empty()) continue;//��̍s�͌��Ȃ�


			std::stringstream ss(line);
			std::string item;

			if (isFirstLine) {
				while (std::getline(ss, item, ',')) {
					//�ŏ��̍s�̏ꍇ�̓p�����[�^���������Ă���̂ŁA�����ێ����Ă���
					parameterNames.push_back(item);
				}
				isFirstLine = false;
				continue;
			}

			//�ŏ��̍s�ȊO�̏ꍇ�ɂ́A���̍ގ��̃p�����[�^��ۑ����Ă���
			PHLuGreParameters parameters = PHLuGreParameters();
			int index = -1;//���ڂ̃p�����[�^�����Ă��邩��\��
			while (std::getline(ss, item, ',')) {
				index++;
				std::string parameterName = parameterNames[index];

				if (parameterName == "Material") {
					parameters.material = item;
					continue;
				}

				double num = std::stod(item);//string����double�ɕϊ�
				if (parameterName == "A") {
					parameters.A = num;
				}
				else if (parameterName == "B") {
					parameters.B = num;
				}
				else if (parameterName == "C") {
					parameters.C = num;
				}
				else if (parameterName == "bristlesSpringK") {
					parameters.bristlesSpringK = num;
				}
				else if (parameterName == "bristlesDamperD") {
					parameters.bristlesDamperD = num;
				}
				else if (parameterName == "bristlesViscosityV") {
					parameters.bristlesViscosityV = num;
				}
				else if (parameterName == "frictionSpring") {
					parameters.frictionSpring = num;
				}
				else if (parameterName == "frictionDamper") {
					parameters.frictionDamper = num;
				}
			}
			getInstance()->lugreParametersMap[parameters.material] = parameters;
		}

		file.close();//�t�@�C�������
	}

	//���ݎg�p���Ă���p�����[�^�̒l���擾����
	std::string PHLuGreParametersData::getCurrentMaterial() { return getInstance()->currentParameters.material; };
	double PHLuGreParametersData::getCurrentA() { return getInstance()->currentParameters.A; };
	double PHLuGreParametersData::getCurrentB() { return getInstance()->currentParameters.B; };
	double PHLuGreParametersData::getCurrentC() { return getInstance()->currentParameters.C; };
	double PHLuGreParametersData::getCurrentBristlesSpringK() { return getInstance()->currentParameters.bristlesSpringK; };
	double PHLuGreParametersData::getCurrentBristlesDamperD() { return getInstance()->currentParameters.bristlesDamperD; };
	double PHLuGreParametersData::getCurrentBristlesViscosityV() { return getInstance()->currentParameters.bristlesViscosityV; };
	double PHLuGreParametersData::getCurrentFrictionSpring() { return getInstance()->currentParameters.frictionSpring; };
	double PHLuGreParametersData::getCurrentFrictionDamper() { return getInstance()->currentParameters.frictionDamper; };

	//���������������
	void PHLuGreParametersData::release() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	//���̃N���X�̃C���X�^���X��Ԃ�
	//�Ȃ��ꍇ�ɂ͐�������
	PHLuGreParametersData* PHLuGreParametersData::getInstance() {

		if (!instance) {
			instance = new PHLuGreParametersData();
			addParametersFromFile("PHLuGreParametersData.csv");//�t�@�C���̃f�[�^��ǂݍ���
			//�����ƃt�@�C�����ǂݍ��߂Ă�����ŏ��̓x�[�N���C�g�̃p�����[�^�ɂ��Ă���
			//instance->changeCurrentParameters("Bakelite");
			//instance->changeCurrentParameters("NewMaterial");
			instance->changeCurrentParameters("Default");
		}

		return instance;
	}

	PHLuGreParametersData::PHLuGreParametersData() {
		//�f�t�H���g�̃p�����[�^���쐬���āA�ێ����Ă���
		PHLuGreParameters defaultParameters = PHLuGreParameters();
		lugreParametersMap[defaultParameters.material] = defaultParameters;
		currentParameters = defaultParameters;
	}
}
	
