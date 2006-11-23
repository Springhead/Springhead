//	Do not edit. MakeTypeDesc.bat will update this file.

template <class IF, class OBJ>
struct FWObjectIfStub: public NamedObjectIfStub<IF, OBJ> {
	virtual PHSolidIf *  GetPHSolid(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHSolid();
	}
	virtual GRFrameIf *  GetGRFrame(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRFrame();
	}
};

template <class IF, class OBJ>
struct FWSceneIfStub: public SceneIfStub<IF, OBJ> {
	virtual PHSceneIf *  GetPHScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHScene();
	}
	virtual GRSceneIf *  GetGRScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRScene();
	}
	virtual void Sync(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Sync();
	}
	virtual void Step(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Step();
	}
	virtual void Draw(GRRenderIf *  grRender, bool debug = false){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Draw(grRender, debug);
	}
	virtual void AddHumanInterface(HIForceDevice6D *  d){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddHumanInterface(d);
	}
};

template <class IF, class OBJ>
struct FWSdkIfStub: public ObjectIfStub<IF, OBJ> {
	virtual FWSceneIf *  CreateScene(const FWSceneDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene(desc);
	}
	virtual int NScene() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
	virtual FWSceneIf *  GetScene(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual PHSdkIf *  GetPHSdk(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHSdk();
	}
	virtual GRSdkIf *  GetGRSdk(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRSdk();
	}
	virtual FISdkIf *  GetFISdk(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetFISdk();
	}
};
