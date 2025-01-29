
/*
	3次元のWorld座標系と、摩擦が生じる接触平面上の座標系の変換を行う
	ここでは、接触平面上の座標系を表すPose型の変数を生成しているが、これは、
	1 位置は、接触平面状の座標系の原点のWorld座標系上での表現
	2 向きを表すクォータニオンは、World座標系から接触面上の座標系への回転の変換を行うためのクォータニオン
	の位置・向きの情報をもつ変数となっている
*/

#include <Springhead.h>
#include <cmath>
#include "coordinateConverter.h"

namespace Spr{

	//接触面の法線を与えると、World座標系から接触面上の座標系への変換を行うためのPoseを返す
	//引数:
	//    normal: 接触面の法線方向をWorld座標系で表したもの
	//    origin: 平面上の座標系の原点の座標のWorld座標系での表現
	//戻り値:
	//    接触平面上の座標系を表すPose型の変数
	Posed getWorldToPlanePose(Vec3d normal, Vec3d origin) {

		//戻り値のための変数を用意
		Posed p;

		//原点座標の設定
		p.Pos() = origin;

		//以下は、World座標系から接触面上の座標系への回転の変換を行うためのクォータニオンを求めてる
		//念のため、normの長さを1に補正しておく
		normal = normal.unit();

		//接触面上のx軸方向の単位ベクトルexと、接触面上のy軸方向の単位ベクトルeyを求める
		Vec3d ex, ey;
		if (normal[1] == 0 && normal[2] == 0) {
			ey = Vec3d(0, 1, 0);
			if (normal[0] < 0) {
				ex = Vec3d(0, 0, 1);
			}
			else {
				ex = Vec3d(0, 0, -1);
			}

		} else {
			//eyを計算する ey = (0, eyy, eyz)とする
			double denominator = std::sqrt(normal[1] * normal[1] + normal[2] * normal[2]);
			double eyy = normal[2] / denominator;
			double eyz = -normal[1] / denominator;
			ey = Vec3d(0, eyy, eyz);
			
			//exを計算する
			ex = ey % normal;//%は外積
		}

		//ex, ey, normalを用いて、接触面上の座標系からWorld座標系への変換を行う座標変換行列を求める
		Matrix3d matrix = Matrix3d(ex, ey, normal);

		//座標変換行列からクォータニオンに変換
		Quaterniond q;
		q.FromMatrix(matrix);//接触面上の座標系からWorld座標系への変換を行うクォータニオン

		//World座標系から接触面上の座標系への変換を行うためのクォータニオンを設定
		p.Ori() = q.Inv();

		return p;
	}

	//接触面の法線とx軸方向、原点座標を与えると、World座標系から接触面上の座標系への変換を行うためのPoseを返す
	//引数:
	//    normal: World座標系における現在の接触面の法線方向
	//	  xAxis: World座標系における現在の接触面のx軸方向(またはx軸の向きにしたい方向)
	//           normalと直交しない場合には、normalに直交するように射影ベクトルをとって使用することとなる
	//           また、xAxisにゼロベクトルが代入された場合や、xAxisがnormalと全く同じ方向の場合にはxAxisの値は無視され、
	//           getWorldToPlanePose(Vec3d normal, Vec3d origin)と同じ結果を返す
	//    origin: 現在の接触面での平面上の座標系の原点を表すWorld座標系での座標
	//戻り値:
	//    接触平面上の座標系を表すPose型変数
	Posed getWorldToPlanePose(Vec3d normal, Vec3d xAxis, Vec3d origin) {

		//戻り値のためのPose型変数の用意
		Posed newPose;

		//新しい平面上の座標系での原点の設定
		newPose.Pos() = origin;

		//xAxisがゼロベクトルの場合を考慮する
		if (xAxis == Vec3d()) {
			return getWorldToPlanePose(normal, origin);
		}

		//newNormal, newXAxisの長さを1に補正しておく
		normal = normal.unit();
		xAxis = xAxis.unit();

		//normalとxAxisが直交するように、xAxisを補正
		//補正方法について
		//直交する <=> dot(normal, xAxis) = 0    (dotは内積とする)
		//そこで、ある定数aを用いて、
		//xAxis <- xAxis - a * normal
		//とすることで、直交する状態にすることを考える
		//ここで、normalが単位ベクトルなので、dot(normal, normal) = 1であることに注意すると
		//dot(normal, xAxis - a * normal) = dot(normal, xAxis) - a * dot(normal, normal) = dot(normal, xAxis) - a
		//となるので、これを0にするためには、a = dot(normal, xAxis)
		//そのため、
		//xAxis <- xAxis - dot(normal, xAxis) * normal
		//により補正することとする
		xAxis = xAxis - dot(normal, xAxis) * normal;

		//xAxisがnormalと同じ方向の場合(この時点でxAxisがゼロベクトルとなっている場合)を考慮する
		if (xAxis == Vec3d()) {
			return getWorldToPlanePose(normal, origin);
		}

		//新しいy軸方向を計算
		Vec3d yAxis = normal % xAxis;//%は外積

		//接触面上の座標系からWorld座標系に変換する回転行列を用意
		Matrix3d matrix = Matrix3d(xAxis, yAxis, normal);

		//座標変換行列からクォータニオンに変換
		Quaterniond q;
		q.FromMatrix(matrix);//接触面上の座標系からWorld座標系への変換を行うクォータニオン

		//World座標系から接触面上の座標系への変換を行うためのクォータニオンを設定
		newPose.Ori() = q.Inv();

		return newPose;
	}

	//接触面の法線の方向が変化したときに、
	//それに応じてWorld座標系から接触面上の座標系への変換を行うためのクォータニオンを更新する関数
	//引数:
	//    oldPose: 以前の状態での接触平面上の座標系を表すPose型変数
	//    newNormal: 現在の接触面のWorld座標系おける法線方向
	//    newOrigin: 現在の接触面での平面上の座標系の原点を表すWorld座標系での座標
	//戻り値:
	//    接触平面上の座標系を表すPose型変数
	Posed updateWorldToPlanePose(Posed oldPose, Vec3d newNormal, Vec3d newOrigin) {

		//戻り値のためのPose型変数の用意
		Posed newPose;

		//新しい平面上の座標系での原点の設定
		newPose.Pos() = newOrigin;

		//以下は、World座標系から接触面上の座標系への回転の変換を行うためのクォータニオンを求めてる
		//念のため、newNormalの長さを1に補正しておく
		newNormal = newNormal.unit();

		//以前の状態でのWorld座標系での法線方向
		Vec3d oldNormal = convertPlaneToWorldVec(Vec3d(0, 0, 1), oldPose);
		
		//以前の状態での法線方向から現在の状態での法線方向に変換するためのクォータニオン
		Quaterniond transformQuaternion;
		transformQuaternion.RotationArc(oldNormal, newNormal);
		
		//以前の「接触面上の座標系への回転を表すクォータニオン」から、
		//新しい「接触面上の座標系への回転を表すクォータニオン」( = newQuaternion)を求める。
		//計算の仕組み:
		//transformQuaternion * oldNormal = newNormal   <- transformQuaternionの定義
		//oldPose.Ori().Inv() * Vector3(0, 0, 1) = oldNormal   <- World座標でのoldNormalが以前の接触面上の座標系での(0, 0, 1)にあたる
		//newQuaternion.Inv() * Vector3(0, 0, 1) = newNormal   <- World座標でのnewNormalが以前の接触面上の座標系での(0, 0, 1)にあたる
		//この３つの式を満たすnewQuaternionの1つとして、newQuaternion = oldPose.Ori() * transformQuaternion.Inv()が導かれる。
		Quaterniond newQuaternion = oldPose.Ori() * transformQuaternion.Inv();

		//新しい接触面上の座標系への回転を表すクォータニオンをPose型変数に設定
		newPose.Ori() = newQuaternion;
		
		return newPose;
	}

	//World座標系の座標から、接触平面上の座標に変換する関数
	//引数:
	//    r: World座標系における座標 
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    接触平面上における座標 (z成分は法線方向成分)
	Vec3d convertWorldToPlanePos(Vec3d r, Posed p) {
		Vec3d s = p.Ori() * (r - p.Pos());//接触平面上における座標(z成分は、法線方向成分を表す)
		return s;
	}

	//World座標系のベクトルから、接触平面上のベクトルに変換する関数
	//速度など原点の位置を考慮しない変換をする場合に用いる
	//引数:
	//    r: World座標系でのベクトル 
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    接触平面上でのベクトル (z成分は法線方向成分)
	Vec3d convertWorldToPlaneVec(Vec3d r, Posed p) {
		Vec3d s = p.Ori() * r;//接触平面上でのベクトル(z成分は、法線方向成分を表す)
		return s;
	}

	//接触平面上の座標から、World座標系の座標に変換する関数
	//引数:
	//    s: 接触平面上における座標
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    World座標系における座標
	Vec3d convertPlaneToWorldPos(Vec3d s, Posed p) {
		//p.Ori()はWorld座標系から接触面上の座標系への変換をするクォータニオンなので、
		//接触面上の座標系からWorld座標系への変換を行うためには逆にしなくてはならないことに注意
		return p.Ori().Inv() * s + p.Pos();
	}

	//接触平面上のベクトルから、World座標系のベクトルに変換する関数
	//速度など原点の位置を考慮しない変換をする場合に用いる
	//引数:
	//    s: 接触平面上でのベクトル
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    World座標系でのベクトル
	Vec3d convertPlaneToWorldVec(Vec3d s, Posed p) {
		//p.Ori()はWorld座標系から接触面上の座標系への変換をするクォータニオンなので、
		//接触面上の座標系からWorld座標系への変換を行うためには逆にしなくてはならないことに注意
		return p.Ori().Inv() * s;
	}
}


