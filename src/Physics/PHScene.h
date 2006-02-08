#ifndef PHSCENE_H
#define PHSCENE_H
#include <Physics/SprPHScene.h>
#include <Foundation/Scene.h>
#include <Physics/PHEngine.h>

namespace Spr {;

class CDShape;
struct CDShapeDesc;
class PHSolidContainer;

/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
	OpenGLのシーングラフをD3Dに変換するためには，一度Documentに
	セーブして，D3D形式でロードしなければならない．	*/
class SPR_DLL PHScene:public InheritScene<PHSceneIf, Scene>, public PHSceneDesc{
	OBJECTDEF(PHScene);
public:
	PHEngines engines;
protected:
	/// 積分ステップ
	double timeStep;
	/// 積分した回数
	unsigned int count;
	///	元のSDK
	PHSdkIf* sdk;
	PHSolidContainer* solids;
public:
	///	コンストラクタ
	PHScene();
	PHScene(PHSdkIf* s, const PHSceneDesc& desc);
	void Init();
	///	デストラクタ
	~PHScene(){Clear();}

	PHSolidIf* CreateSolid();
	PHSolidIf* CreateSolid(const PHSolidDesc& desc);
	CDShapeIf* CreateShape(const CDShapeDesc& desc);

	void SetGravity(const Vec3d& g);
	PHSdkIf* GetSdk();

	/// 積分ステップを返す
	double GetTimeStep()const{return timeStep;}
	/// 積分ステップを設定する
	void SetTimeStep(double dt);
	/// カウント数を返す
	unsigned GetCount()const{return count;}
	/// カウント数を設定する
	void SetCount(unsigned c){count = c;}
	///	シーンの時刻を進める ClearForce(); GenerateForce(); Integrate(); と同じ
	void Step();
	///	シーンの時刻を進める（力のクリア）
	void ClearForce();
	///	シーンの時刻を進める（力の生成）
	void GenerateForce();
	///	シーンの時刻を進める（力と速度を積分して，速度と位置を更新）
	void Integrate();
	///	シーンを空にする．
	void Clear();
	///	名前表から，参照されていないオブジェクトを削除する．
	void ClearName();
	///	デバッグ用
	void Print(std::ostream& os) const { names.Print(os); }

	virtual void SetGravity(Vec3f accel);
	virtual Vec3f GetGravity();

	virtual int GetNSolids();
	virtual PHSolidIf** GetSolids();
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);

protected:
	friend class Object;
};

}
#endif
