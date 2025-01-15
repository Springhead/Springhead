#pragma once

#include<map>
#include<string>

namespace Spr {

	//LuGreモデル自体のパラメータや、実行する上で必要なパラメータをまとめた構造体
	//力覚レンダリングにおけるバネ係数やダンパ係数はLuGreモデルそのもののパラメータではないが、
	//それらもモデルの挙動に影響を与えるためここに含めている
	struct PHLuGreParameters
	{
		//このパラメータの表す材質
		std::string material = "default";

		//LuGreモデル自体のパラメータ
		double A = 1.0;						///< g(T) = A + B log( CT + 1)のA
		double B = 1.6;						///< g(T) = A + B log( CT + 1)のB
		double C = 40.0;					///< g(T) = A + B log( CT + 1)のC
		double bristlesSpringK = 4000.0;	///< LuGreモデルにおける剛毛のバネ係数
		double bristlesDamperD = 4.0;		///< LuGreモデルにおける剛毛のダンパ係数
		double bristlesViscosityV = 0.4;	///< LuGreモデルにおける剛毛にはたらく粘性抵抗の係数

		//力覚レンダリングの中で関連するパラメータ
		double frictionSpring = 1500.0;		///< 力覚レンダリングのバネ係数(プロキシのハプティックポインタの間のバネの係数)
		double frictionDamper = 0.0;		///< 力覚レンダリングのダンパ係数(プロキシのハプティックポインタの間のダンパの係数)
	};


	//様々な材質のPHLuGreParametersをまとめたクラス
	class PHLuGreParametersData
	{
	public:

		//使用するパラメータを変更する
		//引数:
		//    material (std::string): パラメータが表す材質の名称
		static void changeCurrentParameters(std::string material);

		//CSVファイルからパラメータのデータを読み込み、追加する機能
		//引数:
		//    filename (std::string): 読み込むCSVファイル
		static void addParametersFromFile(std::string filename);

		//現在使用しているパラメータの値を取得する
		static std::string getCurrentMaterial();		///< 材質の名称
		static double getCurrentA();					///< g(T) = A + B log( CT + 1)のA
		static double getCurrentB();					///< g(T) = A + B log( CT + 1)のB
		static double getCurrentC();					///< g(T) = A + B log( CT + 1)のC
		static double getCurrentBristlesSpringK();		///< LuGreモデルにおける剛毛のバネ係数
		static double getCurrentBristlesDamperD();		///< LuGreモデルにおける剛毛のダンパ係数
		static double getCurrentBristlesViscosityV();	///< LuGreモデルにおける剛毛にはたらく粘性抵抗の係数
		static double getCurrentFrictionSpring();		///< 力覚レンダリングのバネ係数(プロキシのハプティックポインタの間のバネの係数)
		static double getCurrentFrictionDamper();		///< 力覚レンダリングのダンパ係数(プロキシのハプティックポインタの間のダンパの係数)

	private:
		static PHLuGreParametersData* instance;//このクラスの唯一のインスタンス

		static PHLuGreParametersData* getInstance();//このクラスのインスタンスを取得する

		PHLuGreParameters currentParameters;//現在使用しているパラメータ

		//様々な材質のPHLuGreParametersを材質の名称から引けるようにしておく
		std::map<std::string, PHLuGreParameters> lugreParametersMap;

		//コンストラクタ(デフォルトのパラメータを保持しておく)
		PHLuGreParametersData();
	};
}

