#ifndef GRSCENE_H
#define GRSCENE_H
#include <Graphics/SprGRScene.h>
#include <Foundation/Scene.h>
#include <Graphics/GRFrame.h>

namespace Spr {;

class GRCamera;
/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
*/
class SPR_DLL GRScene:public InheritScene<GRSceneIf, Scene>, public GRSceneDesc{
	OBJECT_DEF(GRScene);
protected:
	///	ルートノード
	GRFrame* world;
	typedef std::vector< UTRef<GRFrame> > GRFrames;
	///	すべてのフレーム
	GRFrames frames;
	///	カメラ
	UTRef<GRCamera> camera;
public:
	///	コンストラクタ
	GRScene(const GRSceneDesc& desc=GRSceneDesc());
	void Init();
	///	デストラクタ
	~GRScene(){}

	GRSdkIf* GetSdk();

	///	シーンを空にする．
	void Clear();

	virtual ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual GRFrameIf* CreateFrame(const GRFrameDesc& desc);
	virtual GRFrameIf* GetWorld(){return world;}
	virtual bool AddChildObject(ObjectIf* o);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual void Render(GRRenderIf* r);
protected:
	void* GetDescAddress(){ return (GRSceneDesc*)this; }
	friend class GRFrame;
};

}
#endif
