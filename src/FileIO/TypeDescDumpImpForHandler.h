//	Do not edit. MakeTypeDescForHandler.bat will update this file.
	
	Frame* pFrame = NULL;
	desc = DBG_NEW UTTypeDesc("Frame");
	desc->size = sizeof(Frame);
	desc->access = DBG_NEW UTAccess<Frame>;
	db->RegisterDesc(desc);
	
	FrameTransformMatrix* pFrameTransformMatrix = NULL;
	desc = DBG_NEW UTTypeDesc("FrameTransformMatrix");
	desc->size = sizeof(FrameTransformMatrix);
	desc->access = DBG_NEW UTAccess<FrameTransformMatrix>;
	field = desc->AddField("", "Affinef", "matrix", "");
	field->offset = (char*)&(pFrameTransformMatrix->matrix) - (char*)pFrameTransformMatrix;
	db->RegisterDesc(desc);
