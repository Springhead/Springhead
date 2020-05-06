#define SPR_OVERRIDEMEMBERFUNCOF_FWFemMeshIf(base)	\
	PHFemMeshIf* GetPHMesh(){	return	base::GetPHMesh();}	\
	void DrawIHBorderXZPlane(bool sw){	base::DrawIHBorderXZPlane(sw);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWHapticPointerIf(base)	\
	void SetPHHapticPointer(PHHapticPointerIf* hpGlobal){	base::SetPHHapticPointer(hpGlobal);}	\
	PHHapticPointerIf* GetPHHapticPointer(){	return	base::GetPHHapticPointer();}	\
	void SetHumanInterface(HIBaseIf* hi){	base::SetHumanInterface(hi);}	\
	HIBaseIf* GetHumanInterface(){	return	base::GetHumanInterface();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWObjectIf(base)	\
	Spr::PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(Spr::PHSolidIf* s){	base::SetPHSolid(s);}	\
	Spr::GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(Spr::GRFrameIf* f){	base::SetGRFrame(f);}	\
	Spr::PHJointIf* GetPHJoint(){	return	base::GetPHJoint();}	\
	void SetPHJoint(Spr::PHJointIf* j){	base::SetPHJoint(j);}	\
	Spr::GRFrameIf* GetChildFrame(){	return	base::GetChildFrame();}	\
	void SetChildFrame(Spr::GRFrameIf* f){	base::SetChildFrame(f);}	\
	void SetSyncSource(Spr::FWObjectDesc::FWObjectSyncSource syncSrc){	base::SetSyncSource(syncSrc);}	\
	Spr::FWObjectDesc::FWObjectSyncSource GetSyncSource(){	return	base::GetSyncSource();}	\
	void EnableAbsolute(bool bAbs){	base::EnableAbsolute(bAbs);}	\
	bool IsAbsolute(){	return	base::IsAbsolute();}	\
	bool LoadMesh(const char* filename, const IfInfo* ii, Spr::GRFrameIf* frame){	return	base::LoadMesh(filename, ii, frame);}	\
	void GenerateCDMesh(Spr::GRFrameIf* frame, const PHMaterial& mat){	base::GenerateCDMesh(frame, mat);}	\
	void Sync(){	base::Sync();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWSceneIf(base)	\
	Spr::PHSceneIf* GetPHScene(){	return	base::GetPHScene();}	\
	void SetPHScene(Spr::PHSceneIf* s){	base::SetPHScene(s);}	\
	Spr::GRSceneIf* GetGRScene(){	return	base::GetGRScene();}	\
	void SetGRScene(Spr::GRSceneIf* s){	base::SetGRScene(s);}	\
	Spr::FWObjectIf* CreateFWObject(){	return	base::CreateFWObject();}	\
	int NObject()const{	return	base::NObject();}	\
	Spr::FWObjectIf** GetObjects(){	return	base::GetObjects();}	\
	void Sync(){	base::Sync();}	\
	void Step(){	base::Step();}	\
	void Draw(Spr::GRRenderIf* grRender, bool debug){	base::Draw(grRender, debug);}	\
	void DrawPHScene(Spr::GRRenderIf* render){	base::DrawPHScene(render);}	\
	void DrawSolid(Spr::GRRenderIf* render, Spr::PHSolidIf* solid, bool solid_or_wire){	base::DrawSolid(render, solid, solid_or_wire);}	\
	void DrawShape(Spr::GRRenderIf* render, Spr::CDShapeIf* shape, bool solid){	base::DrawShape(render, shape, solid);}	\
	void DrawConstraint(Spr::GRRenderIf* render, Spr::PHConstraintIf* con){	base::DrawConstraint(render, con);}	\
	void DrawContact(Spr::GRRenderIf* render, Spr::PHContactPointIf* con){	base::DrawContact(render, con);}	\
	void DrawIK(Spr::GRRenderIf* render, Spr::PHIKEngineIf* ikEngine){	base::DrawIK(render, ikEngine);}	\
	void DrawLimit(Spr::GRRenderIf* render, Spr::PHConstraintIf* con){	base::DrawLimit(render, con);}	\
	void DrawHaptic(Spr::GRRenderIf* render, Spr::PHHapticEngineIf* hapticEngine){	base::DrawHaptic(render, hapticEngine);}	\
	void SetRenderMode(bool solid, bool wire){	base::SetRenderMode(solid, wire);}	\
	void EnableRender(Spr::ObjectIf* obj, bool enable){	base::EnableRender(obj, enable);}	\
	void SetSolidMaterial(int mat, Spr::PHSolidIf* solid){	base::SetSolidMaterial(mat, solid);}	\
	void SetWireMaterial(int mat, Spr::PHSolidIf* solid){	base::SetWireMaterial(mat, solid);}	\
	void EnableRenderAxis(bool world, bool solid, bool con){	base::EnableRenderAxis(world, solid, con);}	\
	void SetAxisMaterial(int matX, int matY, int matZ){	base::SetAxisMaterial(matX, matY, matZ);}	\
	void SetAxisScale(float scaleWorld, float scaleSolid, float scaleCon){	base::SetAxisScale(scaleWorld, scaleSolid, scaleCon);}	\
	void SetAxisStyle(int style){	base::SetAxisStyle(style);}	\
	void EnableRenderForce(bool solid, bool constraint){	base::EnableRenderForce(solid, constraint);}	\
	void SetForceMaterial(int matForce, int matMoment){	base::SetForceMaterial(matForce, matMoment);}	\
	void SetForceScale(float scaleForce, float scaleMoment){	base::SetForceScale(scaleForce, scaleMoment);}	\
	void EnableRenderContact(bool enable){	base::EnableRenderContact(enable);}	\
	void SetContactMaterial(int mat){	base::SetContactMaterial(mat);}	\
	void EnableRenderGrid(bool x, bool y, bool z){	base::EnableRenderGrid(x, y, z);}	\
	void SetGridOption(char axis, float offset, float size, int slice){	base::SetGridOption(axis, offset, size, slice);}	\
	void SetGridMaterial(int matX, int matY, int matZ){	base::SetGridMaterial(matX, matY, matZ);}	\
	void EnableRenderIK(bool enable){	base::EnableRenderIK(enable);}	\
	void SetIKMaterial(int mat){	base::SetIKMaterial(mat);}	\
	void SetIKScale(float scale){	base::SetIKScale(scale);}	\
	void EnableRenderLimit(bool enable){	base::EnableRenderLimit(enable);}	\
	void EnableRenderHaptic(bool enable){	base::EnableRenderHaptic(enable);}	\
	void AddHumanInterface(Spr::HIForceDevice6D* d){	base::AddHumanInterface(d);}	\
	Spr::FWHapticPointerIf* CreateHapticPointer(){	return	base::CreateHapticPointer();}	\
	void UpdateHapticPointers(){	base::UpdateHapticPointers();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWSdkIf(base)	\
	Spr::FWSceneIf* CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){	return	base::CreateScene(phdesc, grdesc);}	\
	bool LoadScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){	return	base::LoadScene(filename, ex, ii, objs);}	\
	bool SaveScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){	return	base::SaveScene(filename, ex, ii, objs);}	\
	int NScene()const{	return	base::NScene();}	\
	Spr::FWSceneIf* GetScene(int index){	return	base::GetScene(index);}	\
	void MergeScene(Spr::FWSceneIf* scene0, Spr::FWSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\
	PHSdkIf* GetPHSdk(){	return	base::GetPHSdk();}	\
	GRSdkIf* GetGRSdk(){	return	base::GetGRSdk();}	\
	Spr::FISdkIf* GetFISdk(){	return	base::GetFISdk();}	\
	Spr::HISdkIf* GetHISdk(){	return	base::GetHISdk();}	\
	bool GetDebugMode(){	return	base::GetDebugMode();}	\
	void SetDebugMode(bool debug){	base::SetDebugMode(debug);}	\
	void Step(){	base::Step();}	\
	void Draw(){	base::Draw();}	\
	Spr::GRRenderIf* GetRender(){	return	base::GetRender();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWWinBaseIf(base)	\
	int GetID(){	return	base::GetID();}	\
	Vec2i GetPosition(){	return	base::GetPosition();}	\
	void SetPosition(int left, int top){	base::SetPosition(left, top);}	\
	Vec2i GetSize(){	return	base::GetSize();}	\
	void SetSize(int width, int height){	base::SetSize(width, height);}	\
	UTString GetTitle(){	return	base::GetTitle();}	\
	void SetTitle(UTString title){	base::SetTitle(title);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWControlIf(base)	\
	UTString GetLabel(){	return	base::GetLabel();}	\
	void SetLabel(UTString l){	base::SetLabel(l);}	\
	void SetAlign(int align){	base::SetAlign(align);}	\
	int GetStyle(){	return	base::GetStyle();}	\
	void SetStyle(int style){	base::SetStyle(style);}	\
	int GetInt(){	return	base::GetInt();}	\
	void SetInt(int val){	base::SetInt(val);}	\
	float GetFloat(){	return	base::GetFloat();}	\
	void SetFloat(float val){	base::SetFloat(val);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWPanelIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWButtonIf(base)	\
	void SetChecked(bool on){	base::SetChecked(on);}	\
	bool IsChecked(){	return	base::IsChecked();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWStaticTextIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWTextBoxIf(base)	\
	void SetIntRange(int rmin, int rmax){	base::SetIntRange(rmin, rmax);}	\
	void GetIntRange(int& rmin, int& rmax){	base::GetIntRange(rmin, rmax);}	\
	void SetFloatRange(float rmin, float rmax){	base::SetFloatRange(rmin, rmax);}	\
	void GetFloatRange(float& rmin, float& rmax){	base::GetFloatRange(rmin, rmax);}	\
	const char* GetString(){	return	base::GetString();}	\
	void SetString(char* str){	base::SetString(str);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWListBoxIf(base)	\
	void AddItem(UTString label){	base::AddItem(label);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWRotationControlIf(base)	\
	Matrix3f GetRotation(){	return	base::GetRotation();}	\
	void SetRotation(const Matrix3f& rot){	base::SetRotation(rot);}	\
	float GetDamping(){	return	base::GetDamping();}	\
	void SetDamping(float d){	base::SetDamping(d);}	\
	void Reset(){	base::Reset();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWTranslationControlIf(base)	\
	Vec3f GetTranslation(){	return	base::GetTranslation();}	\
	void SetTranslation(Vec3f p){	base::SetTranslation(p);}	\
	float GetSpeed(){	return	base::GetSpeed();}	\
	void SetSpeed(float sp){	base::SetSpeed(sp);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWDialogIf(base)	\
	Spr::FWControlIf* CreateControl(const IfInfo* ii, const Spr::FWControlDesc& desc, Spr::FWPanelIf* parent){	return	base::CreateControl(ii, desc, parent);}	\
	Spr::FWButtonIf* CreatePushButton(UTString label, Spr::FWPanelIf* parent){	return	base::CreatePushButton(label, parent);}	\
	Spr::FWButtonIf* CreateCheckButton(UTString label, bool checked, Spr::FWPanelIf* parent){	return	base::CreateCheckButton(label, checked, parent);}	\
	Spr::FWButtonIf* CreateRadioButton(UTString label, Spr::FWPanelIf* parent){	return	base::CreateRadioButton(label, parent);}	\
	Spr::FWStaticTextIf* CreateStaticText(UTString text, Spr::FWPanelIf* parent){	return	base::CreateStaticText(text, parent);}	\
	Spr::FWTextBoxIf* CreateTextBox(UTString label, UTString text, int style, Spr::FWPanelIf* parent){	return	base::CreateTextBox(label, text, style, parent);}	\
	Spr::FWPanelIf* CreatePanel(UTString label, int style, Spr::FWPanelIf* parent){	return	base::CreatePanel(label, style, parent);}	\
	Spr::FWPanelIf* CreateRadioGroup(Spr::FWPanelIf* parent){	return	base::CreateRadioGroup(parent);}	\
	Spr::FWRotationControlIf* CreateRotationControl(UTString label, Spr::FWPanelIf* parent){	return	base::CreateRotationControl(label, parent);}	\
	Spr::FWTranslationControlIf* CreateTranslationControl(UTString label, int style, Spr::FWPanelIf* parent){	return	base::CreateTranslationControl(label, style, parent);}	\
	Spr::FWListBoxIf* CreateListBox(UTString label, Spr::FWPanelIf* parent){	return	base::CreateListBox(label, parent);}	\
	void CreateColumn(bool sep, Spr::FWPanelIf* parent){	base::CreateColumn(sep, parent);}	\
	void CreateSeparator(Spr::FWPanelIf* parent){	base::CreateSeparator(parent);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWWinIf(base)	\
	void SetFullScreen(){	base::SetFullScreen();}	\
	bool GetFullScreen(){	return	base::GetFullScreen();}	\
	Spr::GRRenderIf* GetRender(){	return	base::GetRender();}	\
	void SetRender(Spr::GRRenderIf* data){	base::SetRender(data);}	\
	Spr::FWSceneIf* GetScene(){	return	base::GetScene();}	\
	void SetScene(Spr::FWSceneIf* s){	base::SetScene(s);}	\
	Spr::DVKeyMouseIf* GetKeyMouse(){	return	base::GetKeyMouse();}	\
	void SetKeyMouse(Spr::DVKeyMouseIf* dv){	base::SetKeyMouse(dv);}	\
	Spr::DVJoyStickIf* GetJoyStick(){	return	base::GetJoyStick();}	\
	void SetJoyStick(Spr::DVJoyStickIf* dv){	base::SetJoyStick(dv);}	\
	Spr::HITrackballIf* GetTrackball(){	return	base::GetTrackball();}	\
	void SetTrackball(Spr::HITrackballIf* dv){	base::SetTrackball(dv);}	\
	void SetDebugMode(bool ph_or_gr){	base::SetDebugMode(ph_or_gr);}	\
	bool GetDebugMode(){	return	base::GetDebugMode();}	\
	void Display(){	base::Display();}	\
	Spr::FWDialogIf* CreateDialog(const Spr::FWDialogDesc& desc){	return	base::CreateDialog(desc);}	\
	void CalcViewport(int& left, int& top, int& width, int& height){	base::CalcViewport(left, top, width, height);}	\

