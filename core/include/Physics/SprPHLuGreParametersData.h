#pragma once

#include<map>
#include<string>

namespace Spr {

	//LuGre���f�����̂̃p�����[�^��A���s�����ŕK�v�ȃp�����[�^���܂Ƃ߂��\����
	//�͊o�����_�����O�ɂ�����o�l�W����_���p�W����LuGre���f�����̂��̂̃p�����[�^�ł͂Ȃ����A
	//���������f���̋����ɉe����^���邽�߂����Ɋ܂߂Ă���
	struct PHLuGreParameters
	{
		//���̃p�����[�^�̕\���ގ�
		std::string material = "default";

		//LuGre���f�����̂̃p�����[�^
		double A = 1.0;						///< g(T) = A + B log( CT + 1)��A
		double B = 1.6;						///< g(T) = A + B log( CT + 1)��B
		double C = 40.0;					///< g(T) = A + B log( CT + 1)��C
		double bristlesSpringK = 4000.0;	///< LuGre���f���ɂ����鍄�т̃o�l�W��
		double bristlesDamperD = 4.0;		///< LuGre���f���ɂ����鍄�т̃_���p�W��
		double bristlesViscosityV = 0.4;	///< LuGre���f���ɂ����鍄�тɂ͂��炭�S����R�̌W��

		//�͊o�����_�����O�̒��Ŋ֘A����p�����[�^
		double frictionSpring = 1500.0;		///< �͊o�����_�����O�̃o�l�W��(�v���L�V�̃n�v�e�B�b�N�|�C���^�̊Ԃ̃o�l�̌W��)
		double frictionDamper = 0.0;		///< �͊o�����_�����O�̃_���p�W��(�v���L�V�̃n�v�e�B�b�N�|�C���^�̊Ԃ̃_���p�̌W��)
	};


	//�l�X�ȍގ���PHLuGreParameters���܂Ƃ߂��N���X
	class PHLuGreParametersData
	{
	public:

		//�g�p����p�����[�^��ύX����
		//����:
		//    material (std::string): �p�����[�^���\���ގ��̖���
		static void changeCurrentParameters(std::string material);

		//CSV�t�@�C������p�����[�^�̃f�[�^��ǂݍ��݁A�ǉ�����@�\
		//����:
		//    filename (std::string): �ǂݍ���CSV�t�@�C��
		static void addParametersFromFile(std::string filename);

		//���ݎg�p���Ă���p�����[�^�̒l���擾����
		static std::string getCurrentMaterial();		///< �ގ��̖���
		static double getCurrentA();					///< g(T) = A + B log( CT + 1)��A
		static double getCurrentB();					///< g(T) = A + B log( CT + 1)��B
		static double getCurrentC();					///< g(T) = A + B log( CT + 1)��C
		static double getCurrentBristlesSpringK();		///< LuGre���f���ɂ����鍄�т̃o�l�W��
		static double getCurrentBristlesDamperD();		///< LuGre���f���ɂ����鍄�т̃_���p�W��
		static double getCurrentBristlesViscosityV();	///< LuGre���f���ɂ����鍄�тɂ͂��炭�S����R�̌W��
		static double getCurrentFrictionSpring();		///< �͊o�����_�����O�̃o�l�W��(�v���L�V�̃n�v�e�B�b�N�|�C���^�̊Ԃ̃o�l�̌W��)
		static double getCurrentFrictionDamper();		///< �͊o�����_�����O�̃_���p�W��(�v���L�V�̃n�v�e�B�b�N�|�C���^�̊Ԃ̃_���p�̌W��)

	private:
		static PHLuGreParametersData* instance;//���̃N���X�̗B��̃C���X�^���X

		static PHLuGreParametersData* getInstance();//���̃N���X�̃C���X�^���X���擾����

		PHLuGreParameters currentParameters;//���ݎg�p���Ă���p�����[�^

		//�l�X�ȍގ���PHLuGreParameters���ގ��̖��̂��������悤�ɂ��Ă���
		std::map<std::string, PHLuGreParameters> lugreParametersMap;

		//�R���X�g���N�^(�f�t�H���g�̃p�����[�^��ێ����Ă���)
		PHLuGreParametersData();
	};
}

