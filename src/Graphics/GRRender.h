/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file GRRender.h
 *	@brief グラフィックスレンダラーの基本クラス　　
 */
#ifndef GRRENDER_H
#define GRRENDER_H

#include <Graphics/SprGRRender.h>
#include <Graphics/GRFrame.h>

namespace Spr{;

class GRCamera:public GRVisual, public GRCameraDesc{
public:
	SPR_OBJECTDEF(GRCamera);
	ACCESS_DESC(GRCamera);
	UTRef<GRFrameIf> frame;
	GRCamera(const GRCameraDesc& desc=GRCameraDesc()):GRCameraDesc(desc), frame(NULL){}
	virtual size_t NChildObject() const ;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual GRFrameIf* GetFrame(){ return frame; }
	virtual void SetFrame(GRFrameIf* fr){ frame = fr; }
	virtual bool AddChildObject(ObjectIf* o);
	virtual void Render(GRRenderIf* render);	
};

class GRLight :public GRVisual, public GRLightDesc{
public:
	SPR_OBJECTDEF(GRLight);
	ACCESS_DESC(GRLight);
	GRLight(const GRLightDesc& desc = GRLightDesc()):GRLightDesc(desc){}
	virtual void Render(GRRenderIf* render);
	virtual void Rendered(GRRenderIf* render);
};

/**	@brief	グラフィックスの材質 */
class GRMaterial :public GRVisual, public GRMaterialDesc{
public:
	UTString texnameAbs;			//	テクスチャファイルの絶対パスが入っていることがある。入ってない場合は、texnameが使われる
	SPR_OBJECTDEF(GRMaterial);
	ACCESS_DESC(GRMaterial);
	GRMaterial(const GRMaterialDesc& desc=GRMaterialDesc()):GRMaterialDesc(desc){}
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
	void Render(GRRenderIf* render);
};

class GRShader : public Object{
public:
	SPR_OBJECTDEF(GRShader);
	
	int programId;
	int vertShaderId;
	int fragShaderId;
};

class GRShadowLight : public Object{
public:
	SPR_OBJECTDEF(GRShadowLight);
	
};

/**	@class	GRRenderBase
    @brief	グラフィックスレンダラー/デバイスの基本クラス　 */
class GRRenderBase: public Object{
public:
	SPR_OBJECTDEF_ABST(GRRenderBase);
	
	virtual void SetViewport(Vec2f pos, Vec2f sz){}
	virtual Vec2f GetViewportPos(){ return Vec2f(); }
	virtual Vec2f GetViewportSize(){ return Vec2f(); }
	virtual void ClearBuffer(bool color, bool depth){}
	///	バッファの入れ替え（表示）
	virtual void SwapBuffers(){}
	/// 背景色の取得
	virtual void GetClearColor(Vec4f& color){}
	virtual void SetClearColor(const Vec4f& color){}
	virtual void BeginScene(){}
	virtual void EndScene(){}
	virtual void SetViewMatrix(const Affinef& afv){}
	virtual void GetViewMatrix(Affinef& afv){}
	///	カレントの投影行列をafpで置き換える
	virtual void SetProjectionMatrix(const Affinef& afp){}
	virtual void GetProjectionMatrix(Affinef& afp){}
	virtual void SetModelMatrix(const Affinef& afw){}
	virtual void GetModelMatrix(Affinef& afw){}
	///	カレントのモデル行列に対してafwを掛ける
	virtual void MultModelMatrix(const Affinef& afw){}
	virtual void PushModelMatrix(){}
	virtual void PopModelMatrix(){}
	virtual void ClearBlendMatrix(){}
	virtual bool SetBlendMatrix(const Affinef& afb, unsigned int id=0){return 0;}	
	virtual void SetVertexFormat(const GRVertexElement* e){}
	//virtual void SetVertexShader(void* shader){}
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride=0){}
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride=0){}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, GRVertexArray* arrays, size_t count){}
 	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, GRVertexArray* arrays, size_t count){}
	virtual void DrawPoint(Vec3f p){}
	virtual void DrawLine(Vec3f p0, Vec3f p1){}
	virtual void DrawSpline(Vec3f p0, Vec3f v0, Vec3f p1, Vec3f v1, int ndiv){}
	virtual void DrawArrow(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid){}
	virtual void DrawBox(float sx, float sy, float sz, bool solid=true){}
	virtual void DrawSphere(float radius, int slices, int stacks, bool solid=true){}
 	virtual void DrawCone(float radius, float height, int slice, bool solid=true){}
	virtual void DrawCylinder(float radius, float height, int slice, bool solid=true, bool cap=true){}
	virtual void DrawDisk(float radius, int slice, bool solid=true){}
	virtual void DrawCapsule(float radius, float height, int slice=20, bool solid=true){}
	virtual void DrawRoundCone(float rbottom, float rtop, float height, int slice=20, bool solid=true){}
	virtual void DrawCurve(const Curve3f& curve){}
	virtual void DrawGrid(float size, int slice, float lineWidth){}
	virtual int	 StartList(){return 0;}
	virtual void EndList(){}
	virtual void DrawList(int i){}
	virtual void ReleaseList(int i){}
	virtual void SetFont(const GRFont& font)=0;
	virtual void DrawFont(Vec2f pos, const std::string str){}
	virtual void DrawFont(Vec3f pos, const std::string str){}
	virtual void SetMaterial(const GRMaterialDesc& mat){}
	virtual void SetMaterial(const GRMaterialIf* mat){}
	virtual void SetMaterial(int matname){}
	virtual void SetPointSize(float sz, bool smooth = false){}
	virtual void SetLineWidth(float w, bool smooth = false){}
	virtual void PushLight(const GRLightDesc& light){}
	virtual void PushLight(const GRLightIf* light){}
	virtual void PopLight(){}
	virtual int  NLights(){ return 0; }
	virtual void SetDepthWrite(bool b){}
	virtual void SetDepthTest(bool b){}
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){}
	virtual void SetAlphaTest(bool b){}
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){}
	virtual void SetLighting(bool l){}
	virtual void SetTexture2D(bool b){}
	virtual void SetTexture3D(bool b){}
	virtual unsigned int LoadTexture(const std::string filename){return 0;}
	virtual void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, const char* tb){}
	//virtual void InitShader(){}
	//virtual void SetShaderFormat(GRShaderFormat::ShaderType type){}	
	//virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader){return 0;}
	//virtual GRHandler CreateShader(){return 0;}
	//virtual bool ReadShaderSource(GRHandler shader, std::string file){return 0;}	
	//virtual void GetShaderLocation(GRHandler shader, void* location){}
	virtual GRShaderIf* CreateShader(const GRShaderDesc& sd){ return 0; }
	virtual void SetShader(GRShaderIf* sh){}
	virtual void SetShadowLight(const GRShadowLightDesc& sld){}
	virtual void EnterShadowMapGeneration(){}
	virtual void LeaveShadowMapGeneration(){}
};

/**	@class	GRRender
    @brief	グラフィックスレンダラーの基本クラス（デバイスの切り分け）　 */
class GRRender: public GRRenderBase{
	SPR_OBJECTDEF(GRRender);
protected:
	UTRef<GRDeviceIf> device;		///<	デバイス
	GRCameraDesc camera;			///<	カメラ
	Affinef	affViewTmp, affModelTmp, affProjTmp;	///< 退避用アフィン行列
	bool	screenCoord;			///<	画面座標モードか

	std::vector<GRMaterialDesc> matSample;		/// レンダラーで用意してある材質(24種類)
	
public:
#define REDIRECTIMP_GRRENDERBASE(ptr)																		\
	virtual void SetViewport(Vec2f p, Vec2f s){ ptr SetViewport(p, s); }									\
	virtual Vec2f GetViewportPos(){ return ptr GetViewportPos(); }                                          \
	virtual Vec2f GetViewportSize(){ return ptr GetViewportSize(); }                                        \
	virtual void ClearBuffer(bool color, bool depth){ ptr ClearBuffer(color, depth); }						\
	virtual void SwapBuffers(){ ptr SwapBuffers(); }														\
	virtual void GetClearColor(Vec4f& color){ ptr GetClearColor(color); }									\
	virtual void SetClearColor(const Vec4f& color){ ptr SetClearColor(color); }								\
	virtual void BeginScene(){ ptr BeginScene(); }															\
	virtual void EndScene(){ ptr EndScene(); }																\
	virtual void SetViewMatrix(const Affinef& afv){ ptr SetViewMatrix(afv); }								\
	virtual void GetViewMatrix(Affinef& afv){ ptr GetViewMatrix(afv); }										\
	virtual void SetProjectionMatrix(const Affinef& afp){ ptr SetProjectionMatrix(afp); }					\
	virtual void GetProjectionMatrix(Affinef& afp){ ptr GetProjectionMatrix(afp); }							\
	virtual void SetModelMatrix(const Affinef& afw){ ptr SetModelMatrix(afw); }								\
	virtual void GetModelMatrix(Affinef& afw){ ptr SetModelMatrix(afw); }									\
	virtual void MultModelMatrix(const Affinef& afw){ ptr MultModelMatrix(afw); }							\
	virtual void PushModelMatrix(){ ptr PushModelMatrix(); }												\
	virtual void PopModelMatrix(){ ptr PopModelMatrix(); }													\
	virtual bool SetBlendMatrix(const Affinef& afb){ return ptr SetBlendMatrix(afb); }						\
	virtual bool SetBlendMatrix(const Affinef& afb, unsigned int id){ return ptr SetBlendMatrix(afb, id); }	\
	virtual void ClearBlendMatrix(){ ptr ClearBlendMatrix(); }												\
	virtual void SetVertexFormat(const GRVertexElement* f){ ptr SetVertexFormat(f); }						\
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t ct, size_t st=0)			\
		{ ptr DrawDirect(ty, vtx, ct, st); }																\
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty,												\
		size_t* idx, void* vtx, size_t ct, size_t st=0)														\
		{ ptr DrawIndexed(ty, idx, vtx, ct, st); }															\
	virtual void DrawPoint(Vec3f p){ ptr DrawPoint(p); }													\
	virtual void DrawLine(Vec3f p0, Vec3f p1){ ptr DrawLine(p0, p1); }										\
	virtual void DrawSpline(Vec3f p0, Vec3f v0, Vec3f p1, Vec3f v1, int ndiv)								\
		{ ptr DrawSpline(p0, v0, p1, v1, ndiv); }															\
	virtual void DrawArrow(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid)	\
		{ ptr DrawArrow(p0, p1, rbar, rhead, lhead, slice, solid); }										\
	virtual void DrawBox(float sx, float sy, float sz, bool solid=true)										\
		{ ptr DrawBox(sx, sy, sz, solid); }																	\
	virtual void DrawSphere(float radius, int stacks, int slice, bool solid = true)							\
		{ ptr DrawSphere(radius, stacks, slice, solid); }													\
	virtual void DrawCone(float radius, float height, int slice, bool solid=true)							\
		{ ptr DrawCone(radius, height, slice, solid); }														\
	virtual void DrawCylinder(float radius, float height, int slice, bool solid=true, bool cap=true)		\
		{ ptr DrawCylinder(radius, height, slice, solid, cap); }											\
	virtual void	DrawDisk(float radius, int slice, bool solid=true)										\
		{ ptr DrawDisk(radius, slice, solid); }																\
	virtual void DrawCapsule(float radius, float height, int slice=20, bool solid=true)						\
		{ ptr DrawCapsule(radius, height, slice, solid); }													\
	virtual void DrawRoundCone(float rbottom, float rtop, float height, int slice=20, bool solid=true)		\
		{ ptr DrawRoundCone(rbottom, rtop, height, slice, solid); }											\
	virtual void DrawCurve(const Curve3f& curve)															\
		{ ptr DrawCurve(curve); }																			\
	virtual void DrawGrid(float size, int slice, float lineWidth)											\
		{ ptr DrawGrid(size, slice, lineWidth); }															\
	virtual int StartList()																					\
		{ return ptr StartList(); }																			\
	virtual void EndList()																					\
		{ ptr EndList(); }																					\
	virtual void DrawList(int i){ ptr DrawList(i); }														\
	virtual void ReleaseList(int i){ ptr ReleaseList(i); }													\
	virtual void SetFont(const GRFont& font){ ptr SetFont(font); }											\
	virtual void DrawFont(Vec2f pos, const std::string str){ ptr DrawFont(pos, str); }						\
    virtual void DrawFont(Vec3f pos, const std::string str){ ptr DrawFont(pos, str); }						\
	virtual void SetMaterial(const GRMaterialDesc& mat){ ptr SetMaterial(mat); }							\
	virtual void SetMaterial(const GRMaterialIf* mat){ ptr SetMaterial(mat); }								\
	virtual void SetPointSize(float sz, bool smooth){ ptr SetPointSize(sz, smooth); }						\
	virtual void SetLineWidth(float w, bool smooth){ ptr SetLineWidth(w, smooth); }							\
	virtual void PushLight(const GRLightDesc& light){ ptr PushLight(light);}								\
	virtual void PushLight(const GRLightIf* light){ ptr PushLight(light);}									\
	virtual void PopLight(){ ptr PopLight(); }																\
	virtual int  NLights(){ return ptr NLights(); }															\
	virtual void SetDepthWrite(bool b){ ptr SetDepthWrite(b); }												\
	virtual void SetDepthTest(bool b){ptr SetDepthTest(b); }												\
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){ ptr SetDepthFunc(f); }							\
	virtual void SetAlphaTest(bool b){ptr SetAlphaTest(b); }												\
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest)				\
		{ ptr SetAlphaMode(src, dest); }																	\
	virtual void SetLighting(bool l) { ptr SetLighting(l); }												\
	virtual void SetTexture2D(bool b){ ptr SetTexture2D(b); }												\
	virtual void SetTexture3D(bool b){ ptr SetTexture3D(b); }												\
	virtual unsigned int LoadTexture(const std::string filename){ return ptr LoadTexture(filename); }		\
	/*virtual void InitShader(){ ptr InitShader(); }													\
	virtual void SetShaderFormat(GRShaderFormat::ShaderType type){ ptr SetShaderFormat(type); }				\
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader)			\
		{ return ptr CreateShader(vShaderFile, fShaderFile, shader); }										\
	virtual GRHandler CreateShader(){ return ptr CreateShader(); }											\
	virtual bool ReadShaderSource(GRHandler shader, std::string file)										\
		{ return ptr ReadShaderSource(shader, file); }														\
	virtual void GetShaderLocation(GRHandler shader, void* location)										\
		{ ptr GetShaderLocation(shader, location); }													*/\
	virtual GRShaderIf* CreateShader(const GRShaderDesc& sd){ return ptr CreateShader(sd); }				\
	virtual void SetShader(GRShaderIf* sh){ ptr SetShader(sh); }											\
	virtual void SetShadowLight(const GRShadowLightDesc& sld){ ptr SetShadowLight(sld); }					\
	virtual void EnterShadowMapGeneration()  { ptr EnterShadowMapGeneration(); }							\
	virtual void LeaveShadowMapGeneration()  { ptr LeaveShadowMapGeneration(); }							\

	REDIRECTIMP_GRRENDERBASE(device->)

	virtual void SetDevice(GRDeviceIf* dev){ device = dev; }
	virtual GRDeviceIf* GetDevice(){ return device; }
	
	virtual void Print(std::ostream& os) const;
	
	void SetCamera(const GRCameraDesc& c);
	const GRCameraDesc& GetCamera(){ return camera; }
	
	virtual void Reshape(Vec2f pos, Vec2f sz);

	virtual void SetMaterial(int matname);
	Vec4f	GetReservedColor(int matname){ return matSample[matname].diffuse; }
	
	Vec2f GetPixelSize();
	Vec3f ScreenToCamera(int x, int y, float depth, bool LorR = false);

	void EnterScreenCoordinate();
	void LeaveScreenCoordinate();
	
	GRRender();

};

/**	@class	GRDevice
    @brief	グラフィックス描画の実装　 */
class GRDevice: public GRRenderBase{
protected:
	Vec4f	clearColor;		///< 背景色
public:
	SPR_OBJECTDEF_ABST(GRDevice);
	virtual void Init(){}
	virtual void GetClearColor(Vec4f& color){ color = clearColor; }
	virtual void SetClearColor(const Vec4f& color){ clearColor = color; }
	virtual void SetMaterial(const GRMaterialDesc& mat){}
	virtual void SetMaterial(const GRMaterialIf* mat){}
	virtual void PushLight(const GRLightDesc& light){}
    virtual void PushLight(const GRLightIf* light){ if(light) PushLight(*DCAST(GRLight, light)); }
};
}
#endif
