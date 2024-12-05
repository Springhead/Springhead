

/*
	3次元のWorld座標系と、摩擦が生じる平面上の座標系の変換を行う
*/
#include <Springhead.h>

namespace Spr {

	//接触面の法線を与えると、World座標系から接触面上の座標系への変換を行うためのPoseを返す
	//引数:
	//    normal: 接触面の法線方向をWorld座標系で表したもの
	//    origin: 平面上の座標系の原点の座標のWorld座標系での表現
	//戻り値:
	//    接触平面上の座標系を表すPose型の変数
	Posed getWorldToPlanePose(Vec3d normal, Vec3d origin);

	//接触面の法線の方向が変化したときに、
	//それに応じてWorld座標系から接触面上の座標系への変換を行うためのクォータニオンを更新する関数
	//引数:
	//    oldPose: 以前の状態での接触平面上の座標系を表すPose型変数
	//    newNormal: 現在の接触面のWorld座標系おける法線方向
	//    newOrigin: 現在の接触面での平面上の座標系の原点を表すWorld座標系での座標
	//戻り値:
	//    接触平面上の座標系を表すPose型変数
	Posed updateWorldToPlanePose(Posed oldPose, Vec3d newNormal, Vec3d newOrigin);

	//World座標系の座標から、接触平面上の座標に変換する関数
	//引数:
	//    r: World座標系における座標 
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    接触平面上における座標 (z成分は法線方向成分)
	Vec3d convertWorldToPlanePos(Vec3d r, Posed p);

	//World座標系のベクトルから、接触平面上のベクトルに変換する関数
	//速度など原点の位置を考慮しない変換をする場合に用いる
	//引数:
	//    r: World座標系でのベクトル 
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    接触平面上でのベクトル (z成分は法線方向成分)
	Vec3d convertWorldToPlaneVec(Vec3d r, Posed p);

	//接触平面上の座標から、World座標系の座標に変換する関数
	//引数:
	//    s: 接触平面上における座標
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    World座標系における座標
	Vec3d convertPlaneToWorldPos(Vec3d s, Posed p);

	//接触平面上のベクトルから、World座標系のベクトルに変換する関数
	//速度など原点の位置を考慮しない変換をする場合に用いる
	//引数:
	//    s: 接触平面上でのベクトル
	//    p: 平面上の座標系を表すPose型変数(関数GetWorldToPlanePose()で生成する)
	//戻り値:
	//    World座標系でのベクトル
	Vec3d convertPlaneToWorldVec(Vec3d s, Posed p);
}