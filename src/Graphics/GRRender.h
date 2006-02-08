#ifndef GRRENDER_H
#define GRRENDER_H

#include <SprGraphics.h>

namespace Spr{;

class GRRenderBase: public InheritObject<GRRenderBaseIf, Object>{
	OBJECTDEFABST(GRRenderBase);
};

/**	グラフィックスレンダラーの基本クラス	*/
class GRRender: public InheritObject<GRRenderIf, GRRenderBase>{
	OBJECTDEF(GRRender);
protected:
	UTRef<GRDeviceIf> device;		// デバイス
	GRCamera camera;				// カメラ
public:
#define REDIRECTIMP_GRRENDERBASE(ptr)							\
	virtual void ClearBuffer(){ ptr ClearBuffer(); }			\
	virtual void BeginScene(){ ptr BeginScene(); }				\
	virtual void EndScene(){ ptr EndScene(); }					\
	virtual void MultModelMatrix(const Affinef& afw)			\
		{ ptr MultModelMatrix(afw); }							\
	virtual void PushModelMatrix(){ptr PushModelMatrix(); }		\
	virtual void PopModelMatrix(){ptr PopModelMatrix(); }		\
	virtual void SetModelMatrix(const Affinef& afw)				\
		{ ptr SetModelMatrix(afw); }							\
	virtual void SetViewMatrix(const Affinef& afv)				\
		{ ptr SetViewMatrix(afv); }								\
	virtual void SetProjectionMatrix(const Affinef& afp)		\
		{ ptr SetProjectionMatrix(afp); }						\
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty,							\
												Vec3f* begin, Vec3f* end)				\
		{ ptr DrawDirect(ty, begin, end); }												\
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty,							\
												size_t* begin, size_t* end, Vec3f* vtx)	\
		{ ptr DrawIndexed(ty, begin, end, vtx); }										\
	virtual void DrawText(Vec2f pos, std::string str, const GRFont& font)				\
		{ ptr DrawText(pos, str, font); }												\
	virtual void SetMaterial(const GRMaterial& mat){ ptr SetMaterial(mat); }			\
	virtual void SetLineWidth(float w){ ptr SetLineWidth(w); }							\
	virtual void PushLight(const GRLight& m){ptr PushLight(m);}							\
	virtual void PopLight(){ptr PopLight(); }											\
	virtual void SetDepthWrite(bool b){ ptr SetDepthWrite(b); }							\
	virtual void SetDepthTest(bool b){ptr SetDepthTest(b); }							\
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){ ptr SetDepthFunc(f); }		\
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src,							\
							GRRenderBaseIf::TBlendFunc dest)							\
		{ptr SetAlphaMode(src, dest); }													\

	REDIRECTIMP_GRRENDERBASE(device->)

	virtual void SetDevice(GRDeviceIf* dev){ device = dev; }
	virtual void SetCamera(GRCamera& cam){ camera = cam; }
	virtual void Print(std::ostream& os) const;
};
template <class intf, class base>
struct InheritGRRenderBase:public InheritObject<intf, base>{
	REDIRECTIMP_GRRENDERBASE(base::)	
};
template <class intf, class base>
struct InheritGRRender:public InheritGRRenderBase<intf, base>{
	void SetDevice(GRDeviceIf* dev){
		base::SetDevice(dev);
	}
	virtual void SetCamera(GRCamera& cam){
		base::SetCamera(cam); 
	}
};


/**	グラフィックス描画の実装	*/
class GRDevice: public InheritObject<GRDeviceIf, Object>{
public:
	OBJECTDEFABST(GRDevice);
};
template <class intf, class base>
struct InheritGRDevice:public InheritObject<intf, base>{
	void SetDevice(GRDeviceIf* dev){
		base::SetDevice(dev);
	}
};

}
#endif
