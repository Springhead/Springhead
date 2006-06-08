/**
 *	@file GRRender.h
 *	@brief グラフィックスレンダラーの基本クラス　　
 */
#ifndef GRRENDER_H
#define GRRENDER_H

#include <SprGraphics.h>

namespace Spr{;


class GRLight :public InheritSceneObject<GRLightIf, SceneObject>, public GRLightDesc{
public:
	OBJECT_DEF(GRLight);
	GRLight(){
		ambient = Vec4f(0.0, 0.0, 0.0, 1.0);
		diffuse = Vec4f(1.0, 1.0, 1.0, 1.0);
		specular = Vec4f(1.0, 1.0, 1.0, 1.0);
		position = Vec4f(0.0, 0.0, 1.0, 1.0);
		range = FLT_MAX;
		attenuation0 = 1.0f;
		attenuation1 = 0.0f;
		attenuation2 = 0.0f;
		spotDirection = Vec3f(0.0, -1.0, 0.0);
		spotFalloff  = 0.0f;
		spotInner    = 0.0f;
		spotCutoff   = 180.0f;
	}
};

/**	@brief	グラフィックスの材質 */
class GRMaterial :public InheritSceneObject<GRMaterialIf, SceneObject>, public GRMaterialDesc{
public:
	OBJECT_DEF(GRMaterial);
	GRMaterial(){}
	GRMaterial(Vec4f a, Vec4f d, Vec4f s, Vec4f e, float p){
		ambient = a;
		diffuse = d;
		specular = s;
		emissive = e;
		power = p;
	}
	GRMaterial(Vec4f c, float p){
		ambient = diffuse = specular = emissive = c;
		power = p;
	}
	GRMaterial(Vec4f c){
		ambient = diffuse = specular = emissive = c;		
	}
	/**	W()要素は、アルファ値(0.0～1.0で透明度を表す). 1.0が不透明を表す.
		materialのW()要素を判定して、不透明物体か、透明物体かを判定する. 
		透明なオブジェクトを描くとき、遠くのものから順番に描画しないと、意図に反した結果となる. */
	bool IsOpaque() const {		
		return ambient.W() >= 1.0 && diffuse.W() >= 1.0 && specular.W() >= 1.0 && emissive.W() >= 1.0;
	}
};


/**	@class	GRRenderBase
    @brief	グラフィックスレンダラーの基本クラス（Object派生クラスの実行時型情報を管理） */
class GRRenderBase: public InheritObject<GRRenderBaseIf, Object>{
public:
	OBJECT_DEF_ABST(GRRenderBase);
};

/**	@class	GRRender
    @brief	グラフィックスレンダラーの基本クラス（デバイスの切り分け）　 */
class GRRender: public InheritObject<GRRenderIf, GRRenderBase>{
	OBJECT_DEF(GRRender);
protected:
	UTRef<GRDeviceIf> device;		///<	デバイス
	GRCamera camera;				///<	カメラ
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
	virtual void SetVertexFormat(const GRVertexElement* f)		\
		{ ptr SetVertexFormat(f); }								\
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty,							\
		void* vtx, size_t ct, size_t st=0)												\
		{ ptr DrawDirect(ty, vtx, ct, st); }											\
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty,							\
		size_t* idx, void* vtx, size_t ct, size_t st=0)									\
		{ ptr DrawIndexed(ty, idx, vtx, ct, st); }										\
	virtual void DrawFont(Vec2f pos, const std::string str)								\
		{ ptr DrawFont(pos, str); }														\
    virtual void DrawFont(Vec3f pos, const std::string str)								\
		{ ptr DrawFont(pos, str); }														\
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont& font)			\
		{ ptr DrawFont(pos, str, font); }												\
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont& font)			\
		{ ptr DrawFont(pos, str, font); }												\
	virtual void SetMaterial(const GRMaterialDesc& mat){ ptr SetMaterial(mat); }		\
	virtual void SetMaterial(const GRMaterialIf* mat){									\
		if(mat) ptr SetMaterial((const GRMaterialDesc&)*DCAST(GRMaterial, mat)); }		\
	virtual void SetLineWidth(float w){ ptr SetLineWidth(w); }							\
	virtual void PushLight(const GRLightDesc& light){ptr PushLight(light);}				\
	virtual void PushLight(const GRLightIf* light){										\
		if(light) ptr PushLight((const GRLightDesc&)*DCAST(GRLight, light)); }			\
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
struct InheritGRRender:public InheritObject<intf, base>{
	REDIRECTIMP_GRRENDERBASE(base::device->)
	void SetDevice(GRDeviceIf* dev){
		base::SetDevice(dev);
	}
	virtual void SetCamera(GRCamera& cam){
		base::SetCamera(cam); 
	}
};

/**	@class	GRDevice
    @brief	グラフィックス描画の実装　 */
class GRDevice: public InheritObject<GRDeviceIf, GRRenderBase>{
public:
	OBJECT_DEF_ABST(GRDevice);
	virtual void SetMaterial(const GRMaterialDesc& mat)=0;
	virtual void SetMaterial(const GRMaterialIf* mat){
		if(mat) SetMaterial(*DCAST(GRMaterial, mat)); }
	virtual void PushLight(const GRLightDesc& light)=0;
    virtual void PushLight(const GRLightIf* light){
        if(light) PushLight(*DCAST(GRLight, light)); }
};
template <class intf, class base>
struct InheritGRDevice:public InheritObject<intf, base>{
	void SetDevice(GRDeviceIf* dev){ base::SetDevice(dev); }
	void SetMaterial(const GRMaterialIf* mat){ base::SetMaterial(mat); }
	void PushLight(const GRLightIf* light){ base::PushLight(light); }
};

}
#endif
