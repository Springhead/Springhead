#ifndef GRSCENE_H
#define GRSCENE_H
#include <Graphics/SprGRScene.h>
#include <Foundation/Scene.h>
#include <Graphics/GRFrame.h>

namespace Spr {;


/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
*/
class SPR_DLL GRScene:public InheritScene<GRSceneIf, Scene>, public GRSceneDesc{
	OBJECTDEF(GRScene);
protected:
	///	元のSDK
	GRSdkIf* sdk;
	///	ルートノード
	GRFrame* world;
	///	すべてのフレーム
	typedef std::vector< UTRef<GRFrame> > GRFrames;
	GRFrames frames;
public:
	///	コンストラクタ
	GRScene();
	GRScene(GRSdkIf* s, const GRSceneDesc& desc);
	void Init();
	///	デストラクタ
	~GRScene(){}

	GRSdkIf* GetSdk();

	///	シーンを空にする．
	void Clear();
	GRFrameIf* CreateFrame(const GRFrameDesc& desc);

	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
protected:
	void* GetDescAddress(){ return (GRSceneDesc*)this; }
};

}
#endif
