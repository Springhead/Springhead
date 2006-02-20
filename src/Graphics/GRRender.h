/**
 *	@file GRRender.h
 *	@brief グラフィックスレンダラーの基本クラス　　
 */
#ifndef GRRENDER_H
#define GRRENDER_H

#include <SprGraphics.h>

namespace Spr{;

/**	@class	GRRenderBase
    @brief	グラフィックスレンダラーの基本クラス（Object派生クラスの実行時型情報を管理） */
class GRRenderBase: public InheritObject<GRRenderBaseIf, Object>{
	OBJECTDEFABST(GRRenderBase);
};

/**	@class	GRRender
    @brief	グラフィックスレンダラーの基本クラス（デバイスの切り分け）　 */
class GRRender: public InheritObject<GRRenderIf, GRRenderBase>{
	OBJECTDEF(GRRender);
protected:
	UTRef<GRDeviceIf> device;		///<	デバイス
	GRCamera camera;				///<	カメラ
public:
#define REDIRECTIMP_GRRENDERBASE(ptr)							\
		virtual void ClearBuffer(){ ptr ClearBuffer(); }		\
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
	virtual void DrawFont(Vec2f pos, const std::string str)								\
		{ ptr DrawFont(pos, str); }														\
    virtual void DrawFont(Vec3f pos, const std::string str)								\
		{ ptr DrawFont(pos, str); }														\
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont& font)			\
		{ ptr DrawFont(pos, str, font); }												\
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont& font)			\
		{ ptr DrawFont(pos, str, font); }												\
	virtual void SetMaterial(const GRMaterial& mat){ ptr SetMaterial(mat); }			\
	virtual void SetLineWidth(float w){ ptr SetLineWidth(w); }							\
	virtual void PushLight(const GRLight& light){ptr PushLight(light);}					\
	virtual void PopLight(){ptr PopLight(); }											\
	virtual void SetDepthWrite(bool b){ ptr SetDepthWrite(b); }							\
	virtual void SetDepthTest(bool b){ptr SetDepthTest(b); }							\
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){ ptr SetDepthFunc(f); }		\
	virtual void SetAlphaTest(bool b){ptr SetAlphaTest(b); }							\
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

/**	@class	GRDevice
    @brief	グラフィックス描画の実装　 */
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
