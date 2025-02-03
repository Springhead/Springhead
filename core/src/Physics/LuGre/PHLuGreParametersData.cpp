#include <Physics/SprPHLuGreParametersData.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <springhead.h>

namespace Spr {

	//このクラスの唯一のインスタンス
	PHLuGreParametersData* PHLuGreParametersData::instance = nullptr;

	//使用するパラメータを変更する
	//引数:
	//    material (std::string): パラメータが表す材質の名称
	void PHLuGreParametersData::changeCurrentParameters(std::string material) {
		std::map<std::string, PHLuGreParameters>* data = &(getInstance()->lugreParametersMap);
		if (data->find(material) != data->end()) {
			//指定された材質のパラメータが存在するとき
			getInstance()->currentParameters = data->at(material);
		}
		else {
			//指定された材質のパラメータが存在しないとき
			DSTR << "指定された材質が存在しません!!!" << std::endl;
		}
	}

	//CSVファイルからパラメータのデータを読み込み、追加しておく機能
	void PHLuGreParametersData::addParametersFromFile(std::string filename) {

		std::string thisFilePath = std::string(__FILE__);//このファイルのパスを取得
		std::string thisDirectoryPath = thisFilePath.erase(thisFilePath.find_last_of("\\") + 1);//このファイルの存在するディレクトリを取得(環境に依存しそうなので注意)
		std::ifstream file(thisDirectoryPath + filename);//ファイルを開く
		if (!file.is_open())return;//ファイルを開けなかった場合

		std::vector<std::string> parameterNames;//パラメータ名の配列
		bool isFirstLine = true;//最初の行であるかどうか
		std::string line;
		while (std::getline(file, line)) {
			//1行分の文字列がlineに入っている

			if (line.empty()) continue;//空の行は見ない


			std::stringstream ss(line);
			std::string item;

			if (isFirstLine) {
				while (std::getline(ss, item, ',')) {
					//最初の行の場合はパラメータ名が入っているので、それを保持しておく
					parameterNames.push_back(item);
				}
				isFirstLine = false;
				continue;
			}

			//最初の行以外の場合には、その材質のパラメータを保存していく
			PHLuGreParameters parameters = PHLuGreParameters();
			int index = -1;//何個目のパラメータを見ているかを表す
			while (std::getline(ss, item, ',')) {
				index++;
				std::string parameterName = parameterNames[index];

				if (parameterName == "Material") {
					parameters.material = item;
					continue;
				}

				double num = std::stod(item);//stringからdoubleに変換
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

		file.close();//ファイルを閉じる
	}

	//現在使用しているパラメータの値を取得する
	std::string PHLuGreParametersData::getCurrentMaterial() { return getInstance()->currentParameters.material; };
	double PHLuGreParametersData::getCurrentA() { return getInstance()->currentParameters.A; };
	double PHLuGreParametersData::getCurrentB() { return getInstance()->currentParameters.B; };
	double PHLuGreParametersData::getCurrentC() { return getInstance()->currentParameters.C; };
	double PHLuGreParametersData::getCurrentBristlesSpringK() { return getInstance()->currentParameters.bristlesSpringK; };
	double PHLuGreParametersData::getCurrentBristlesDamperD() { return getInstance()->currentParameters.bristlesDamperD; };
	double PHLuGreParametersData::getCurrentBristlesViscosityV() { return getInstance()->currentParameters.bristlesViscosityV; };
	double PHLuGreParametersData::getCurrentFrictionSpring() { return getInstance()->currentParameters.frictionSpring; };
	double PHLuGreParametersData::getCurrentFrictionDamper() { return getInstance()->currentParameters.frictionDamper; };

	//メモリを解放する
	void PHLuGreParametersData::release() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	//このクラスのインスタンスを返す
	//ない場合には生成する
	PHLuGreParametersData* PHLuGreParametersData::getInstance() {

		if (!instance) {
			instance = new PHLuGreParametersData();
			addParametersFromFile("PHLuGreParametersData.csv");//ファイルのデータを読み込み
			//ちゃんとファイルが読み込めていたら最初はベークライトのパラメータにしておく
			//instance->changeCurrentParameters("Bakelite");
			//instance->changeCurrentParameters("NewMaterial");
			instance->changeCurrentParameters("Default");
		}

		return instance;
	}

	PHLuGreParametersData::PHLuGreParametersData() {
		//デフォルトのパラメータを作成して、保持しておく
		PHLuGreParameters defaultParameters = PHLuGreParameters();
		lugreParametersMap[defaultParameters.material] = defaultParameters;
		currentParameters = defaultParameters;
	}
}
	
