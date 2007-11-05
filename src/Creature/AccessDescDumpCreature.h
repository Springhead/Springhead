//	Do not edit. MakeTypeDesc.bat will update this file.
#define ACCESS_DESC_CRBodyDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRBodyDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRHingeHumanBodyDesc(cls) \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double abdomenHeight ; \
	double abdomenBreadth ; \
	double abdomenThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double neckDiameter ; \
	double headDiameter ; \
	double upperArmLength ; \
	double upperArmDiameter ; \
	double lowerArmLength ; \
	double lowerArmDiameter ; \
	double handLength ; \
	double handBreadth ; \
	double handThickness ; \
	double upperLegLength ; \
	double upperLegDiameter ; \
	double interLegDistance ; \
	double lowerLegLength ; \
	double lowerLegDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double ankleToeDistance ; \
	double vertexToEyeHeight ; \
	double occiputToEyeDistance ; \
	double eyeDiameter ; \
	double interpupillaryBreadth ; \
	double springWaistAbdomen ; \
	double damperWaistAbdomen ; \
	double springAbdomenChest ; \
	double damperAbdomenChest ; \
	double springChestNeckX ; \
	double damperChestNeckX ; \
	double springChestNeckY ; \
	double damperChestNeckY ; \
	double springChestNeckZ ; \
	double damperChestNeckZ ; \
	double springNeckHeadX ; \
	double damperNeckHeadX ; \
	double springNeckHeadZ ; \
	double damperNeckHeadZ ; \
	double springShoulderZ ; \
	double damperShoulderZ ; \
	double springShoulderX ; \
	double damperShoulderX ; \
	double springShoulderY ; \
	double damperShoulderY ; \
	double springElbow ; \
	double damperElbow ; \
	double springWristY ; \
	double damperWristY ; \
	double springWristX ; \
	double damperWristX ; \
	double springWristZ ; \
	double damperWristZ ; \
	double springWaistLegZ ; \
	double damperWaistLegZ ; \
	double springWaistLegX ; \
	double damperWaistLegX ; \
	double springWaistLegY ; \
	double damperWaistLegY ; \
	double springKnee ; \
	double damperKnee ; \
	double springAnkleY ; \
	double damperAnkleY ; \
	double springAnkleX ; \
	double damperAnkleX ; \
	double springAnkleZ ; \
	double damperAnkleZ ; \
	double springEyeY ; \
	double damperEyeY ; \
	double springEyeX ; \
	double damperEyeX ; \
	Vec3d posRightUpperArm ; \
	Quaterniond oriRightUpperArm ; \
	Quaterniond oriRightLowerArm ; \
	Quaterniond oriRightHand ; \
	Vec2d rangeWaistAbdomen ; \
	Vec2d rangeAbdomenChest ; \
	Vec2d rangeChestNeckX ; \
	Vec2d rangeChestNeckY ; \
	Vec2d rangeChestNeckZ ; \
	Vec2d rangeNeckHeadX ; \
	Vec2d rangeNeckHeadZ ; \
	Vec2d rangeShoulderZ ; \
	Vec2d rangeShoulderX ; \
	Vec2d rangeShoulderY ; \
	Vec2d rangeElbow ; \
	Vec2d rangeWristY ; \
	Vec2d rangeWristX ; \
	Vec2d rangeWristZ ; \
	Vec2d rangeWaistLegZ ; \
	Vec2d rangeWaistLegX ; \
	Vec2d rangeWaistLegY ; \
	Vec2d rangeKnee ; \
	Vec2d rangeAnkleY ; \
	Vec2d rangeAnkleX ; \
	Vec2d rangeAnkleZ ; \
	Vec2d rangeRightEyeY ; \
	Vec2d rangeEyeX ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistHeight = ((CRHingeHumanBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRHingeHumanBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRHingeHumanBodyDesc*)desc)->waistThickness; \
		abdomenHeight = ((CRHingeHumanBodyDesc*)desc)->abdomenHeight; \
		abdomenBreadth = ((CRHingeHumanBodyDesc*)desc)->abdomenBreadth; \
		abdomenThickness = ((CRHingeHumanBodyDesc*)desc)->abdomenThickness; \
		chestHeight = ((CRHingeHumanBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRHingeHumanBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRHingeHumanBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRHingeHumanBodyDesc*)desc)->neckLength; \
		neckDiameter = ((CRHingeHumanBodyDesc*)desc)->neckDiameter; \
		headDiameter = ((CRHingeHumanBodyDesc*)desc)->headDiameter; \
		upperArmLength = ((CRHingeHumanBodyDesc*)desc)->upperArmLength; \
		upperArmDiameter = ((CRHingeHumanBodyDesc*)desc)->upperArmDiameter; \
		lowerArmLength = ((CRHingeHumanBodyDesc*)desc)->lowerArmLength; \
		lowerArmDiameter = ((CRHingeHumanBodyDesc*)desc)->lowerArmDiameter; \
		handLength = ((CRHingeHumanBodyDesc*)desc)->handLength; \
		handBreadth = ((CRHingeHumanBodyDesc*)desc)->handBreadth; \
		handThickness = ((CRHingeHumanBodyDesc*)desc)->handThickness; \
		upperLegLength = ((CRHingeHumanBodyDesc*)desc)->upperLegLength; \
		upperLegDiameter = ((CRHingeHumanBodyDesc*)desc)->upperLegDiameter; \
		interLegDistance = ((CRHingeHumanBodyDesc*)desc)->interLegDistance; \
		lowerLegLength = ((CRHingeHumanBodyDesc*)desc)->lowerLegLength; \
		lowerLegDiameter = ((CRHingeHumanBodyDesc*)desc)->lowerLegDiameter; \
		footLength = ((CRHingeHumanBodyDesc*)desc)->footLength; \
		footBreadth = ((CRHingeHumanBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRHingeHumanBodyDesc*)desc)->footThickness; \
		ankleToeDistance = ((CRHingeHumanBodyDesc*)desc)->ankleToeDistance; \
		vertexToEyeHeight = ((CRHingeHumanBodyDesc*)desc)->vertexToEyeHeight; \
		occiputToEyeDistance = ((CRHingeHumanBodyDesc*)desc)->occiputToEyeDistance; \
		eyeDiameter = ((CRHingeHumanBodyDesc*)desc)->eyeDiameter; \
		interpupillaryBreadth = ((CRHingeHumanBodyDesc*)desc)->interpupillaryBreadth; \
		springWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->springWaistAbdomen; \
		damperWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->damperWaistAbdomen; \
		springAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->springAbdomenChest; \
		damperAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->damperAbdomenChest; \
		springChestNeckX = ((CRHingeHumanBodyDesc*)desc)->springChestNeckX; \
		damperChestNeckX = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckX; \
		springChestNeckY = ((CRHingeHumanBodyDesc*)desc)->springChestNeckY; \
		damperChestNeckY = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckY; \
		springChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->springChestNeckZ; \
		damperChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckZ; \
		springNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->springNeckHeadX; \
		damperNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->damperNeckHeadX; \
		springNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->springNeckHeadZ; \
		damperNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->damperNeckHeadZ; \
		springShoulderZ = ((CRHingeHumanBodyDesc*)desc)->springShoulderZ; \
		damperShoulderZ = ((CRHingeHumanBodyDesc*)desc)->damperShoulderZ; \
		springShoulderX = ((CRHingeHumanBodyDesc*)desc)->springShoulderX; \
		damperShoulderX = ((CRHingeHumanBodyDesc*)desc)->damperShoulderX; \
		springShoulderY = ((CRHingeHumanBodyDesc*)desc)->springShoulderY; \
		damperShoulderY = ((CRHingeHumanBodyDesc*)desc)->damperShoulderY; \
		springElbow = ((CRHingeHumanBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeHumanBodyDesc*)desc)->damperElbow; \
		springWristY = ((CRHingeHumanBodyDesc*)desc)->springWristY; \
		damperWristY = ((CRHingeHumanBodyDesc*)desc)->damperWristY; \
		springWristX = ((CRHingeHumanBodyDesc*)desc)->springWristX; \
		damperWristX = ((CRHingeHumanBodyDesc*)desc)->damperWristX; \
		springWristZ = ((CRHingeHumanBodyDesc*)desc)->springWristZ; \
		damperWristZ = ((CRHingeHumanBodyDesc*)desc)->damperWristZ; \
		springWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->springWaistLegZ; \
		damperWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegZ; \
		springWaistLegX = ((CRHingeHumanBodyDesc*)desc)->springWaistLegX; \
		damperWaistLegX = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegX; \
		springWaistLegY = ((CRHingeHumanBodyDesc*)desc)->springWaistLegY; \
		damperWaistLegY = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegY; \
		springKnee = ((CRHingeHumanBodyDesc*)desc)->springKnee; \
		damperKnee = ((CRHingeHumanBodyDesc*)desc)->damperKnee; \
		springAnkleY = ((CRHingeHumanBodyDesc*)desc)->springAnkleY; \
		damperAnkleY = ((CRHingeHumanBodyDesc*)desc)->damperAnkleY; \
		springAnkleX = ((CRHingeHumanBodyDesc*)desc)->springAnkleX; \
		damperAnkleX = ((CRHingeHumanBodyDesc*)desc)->damperAnkleX; \
		springAnkleZ = ((CRHingeHumanBodyDesc*)desc)->springAnkleZ; \
		damperAnkleZ = ((CRHingeHumanBodyDesc*)desc)->damperAnkleZ; \
		springEyeY = ((CRHingeHumanBodyDesc*)desc)->springEyeY; \
		damperEyeY = ((CRHingeHumanBodyDesc*)desc)->damperEyeY; \
		springEyeX = ((CRHingeHumanBodyDesc*)desc)->springEyeX; \
		damperEyeX = ((CRHingeHumanBodyDesc*)desc)->damperEyeX; \
		posRightUpperArm = ((CRHingeHumanBodyDesc*)desc)->posRightUpperArm; \
		oriRightUpperArm = ((CRHingeHumanBodyDesc*)desc)->oriRightUpperArm; \
		oriRightLowerArm = ((CRHingeHumanBodyDesc*)desc)->oriRightLowerArm; \
		oriRightHand = ((CRHingeHumanBodyDesc*)desc)->oriRightHand; \
		rangeWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->rangeWaistAbdomen; \
		rangeAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->rangeAbdomenChest; \
		rangeChestNeckX = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckX; \
		rangeChestNeckY = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckY; \
		rangeChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckZ; \
		rangeNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadX; \
		rangeNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadZ; \
		rangeShoulderZ = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderZ; \
		rangeShoulderX = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderX; \
		rangeShoulderY = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderY; \
		rangeElbow = ((CRHingeHumanBodyDesc*)desc)->rangeElbow; \
		rangeWristY = ((CRHingeHumanBodyDesc*)desc)->rangeWristY; \
		rangeWristX = ((CRHingeHumanBodyDesc*)desc)->rangeWristX; \
		rangeWristZ = ((CRHingeHumanBodyDesc*)desc)->rangeWristZ; \
		rangeWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegZ; \
		rangeWaistLegX = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegX; \
		rangeWaistLegY = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegY; \
		rangeKnee = ((CRHingeHumanBodyDesc*)desc)->rangeKnee; \
		rangeAnkleY = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleY; \
		rangeAnkleX = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleX; \
		rangeAnkleZ = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleZ; \
		rangeRightEyeY = ((CRHingeHumanBodyDesc*)desc)->rangeRightEyeY; \
		rangeEyeX = ((CRHingeHumanBodyDesc*)desc)->rangeEyeX; \
		noLegs = ((CRHingeHumanBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRHingeHumanBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeHumanBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeHumanBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeHumanBodyDesc*)desc)->abdomenHeight = abdomenHeight; \
		((CRHingeHumanBodyDesc*)desc)->abdomenBreadth = abdomenBreadth; \
		((CRHingeHumanBodyDesc*)desc)->abdomenThickness = abdomenThickness; \
		((CRHingeHumanBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeHumanBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeHumanBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeHumanBodyDesc*)desc)->neckLength = neckLength; \
		((CRHingeHumanBodyDesc*)desc)->neckDiameter = neckDiameter; \
		((CRHingeHumanBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRHingeHumanBodyDesc*)desc)->upperArmLength = upperArmLength; \
		((CRHingeHumanBodyDesc*)desc)->upperArmDiameter = upperArmDiameter; \
		((CRHingeHumanBodyDesc*)desc)->lowerArmLength = lowerArmLength; \
		((CRHingeHumanBodyDesc*)desc)->lowerArmDiameter = lowerArmDiameter; \
		((CRHingeHumanBodyDesc*)desc)->handLength = handLength; \
		((CRHingeHumanBodyDesc*)desc)->handBreadth = handBreadth; \
		((CRHingeHumanBodyDesc*)desc)->handThickness = handThickness; \
		((CRHingeHumanBodyDesc*)desc)->upperLegLength = upperLegLength; \
		((CRHingeHumanBodyDesc*)desc)->upperLegDiameter = upperLegDiameter; \
		((CRHingeHumanBodyDesc*)desc)->interLegDistance = interLegDistance; \
		((CRHingeHumanBodyDesc*)desc)->lowerLegLength = lowerLegLength; \
		((CRHingeHumanBodyDesc*)desc)->lowerLegDiameter = lowerLegDiameter; \
		((CRHingeHumanBodyDesc*)desc)->footLength = footLength; \
		((CRHingeHumanBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRHingeHumanBodyDesc*)desc)->footThickness = footThickness; \
		((CRHingeHumanBodyDesc*)desc)->ankleToeDistance = ankleToeDistance; \
		((CRHingeHumanBodyDesc*)desc)->vertexToEyeHeight = vertexToEyeHeight; \
		((CRHingeHumanBodyDesc*)desc)->occiputToEyeDistance = occiputToEyeDistance; \
		((CRHingeHumanBodyDesc*)desc)->eyeDiameter = eyeDiameter; \
		((CRHingeHumanBodyDesc*)desc)->interpupillaryBreadth = interpupillaryBreadth; \
		((CRHingeHumanBodyDesc*)desc)->springWaistAbdomen = springWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistAbdomen = damperWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->springAbdomenChest = springAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->damperAbdomenChest = damperAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckX = springChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckX = damperChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckY = springChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckY = damperChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckZ = springChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckZ = damperChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->springNeckHeadX = springNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->damperNeckHeadX = damperNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->springNeckHeadZ = springNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->damperNeckHeadZ = damperNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderZ = springShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderZ = damperShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderX = springShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderX = damperShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderY = springShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderY = damperShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeHumanBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeHumanBodyDesc*)desc)->springWristY = springWristY; \
		((CRHingeHumanBodyDesc*)desc)->damperWristY = damperWristY; \
		((CRHingeHumanBodyDesc*)desc)->springWristX = springWristX; \
		((CRHingeHumanBodyDesc*)desc)->damperWristX = damperWristX; \
		((CRHingeHumanBodyDesc*)desc)->springWristZ = springWristZ; \
		((CRHingeHumanBodyDesc*)desc)->damperWristZ = damperWristZ; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegZ = springWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegZ = damperWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegX = springWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegX = damperWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegY = springWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegY = damperWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->springKnee = springKnee; \
		((CRHingeHumanBodyDesc*)desc)->damperKnee = damperKnee; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleY = springAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleY = damperAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleX = springAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleX = damperAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleZ = springAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleZ = damperAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->springEyeY = springEyeY; \
		((CRHingeHumanBodyDesc*)desc)->damperEyeY = damperEyeY; \
		((CRHingeHumanBodyDesc*)desc)->springEyeX = springEyeX; \
		((CRHingeHumanBodyDesc*)desc)->damperEyeX = damperEyeX; \
		((CRHingeHumanBodyDesc*)desc)->posRightUpperArm = posRightUpperArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightUpperArm = oriRightUpperArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightLowerArm = oriRightLowerArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightHand = oriRightHand; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistAbdomen = rangeWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->rangeAbdomenChest = rangeAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckX = rangeChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckY = rangeChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckZ = rangeChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadX = rangeNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadZ = rangeNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderZ = rangeShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderX = rangeShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderY = rangeShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristY = rangeWristY; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristX = rangeWristX; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristZ = rangeWristZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegZ = rangeWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegX = rangeWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegY = rangeWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->rangeKnee = rangeKnee; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleY = rangeAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleX = rangeAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleZ = rangeAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeRightEyeY = rangeRightEyeY; \
		((CRHingeHumanBodyDesc*)desc)->rangeEyeX = rangeEyeX; \
		((CRHingeHumanBodyDesc*)desc)->noLegs = noLegs; \
		return true; \
	} \

#define ACCESS_DESC_CRHingeHumanBodyDesc_NOBASE \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double abdomenHeight ; \
	double abdomenBreadth ; \
	double abdomenThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double neckDiameter ; \
	double headDiameter ; \
	double upperArmLength ; \
	double upperArmDiameter ; \
	double lowerArmLength ; \
	double lowerArmDiameter ; \
	double handLength ; \
	double handBreadth ; \
	double handThickness ; \
	double upperLegLength ; \
	double upperLegDiameter ; \
	double interLegDistance ; \
	double lowerLegLength ; \
	double lowerLegDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double ankleToeDistance ; \
	double vertexToEyeHeight ; \
	double occiputToEyeDistance ; \
	double eyeDiameter ; \
	double interpupillaryBreadth ; \
	double springWaistAbdomen ; \
	double damperWaistAbdomen ; \
	double springAbdomenChest ; \
	double damperAbdomenChest ; \
	double springChestNeckX ; \
	double damperChestNeckX ; \
	double springChestNeckY ; \
	double damperChestNeckY ; \
	double springChestNeckZ ; \
	double damperChestNeckZ ; \
	double springNeckHeadX ; \
	double damperNeckHeadX ; \
	double springNeckHeadZ ; \
	double damperNeckHeadZ ; \
	double springShoulderZ ; \
	double damperShoulderZ ; \
	double springShoulderX ; \
	double damperShoulderX ; \
	double springShoulderY ; \
	double damperShoulderY ; \
	double springElbow ; \
	double damperElbow ; \
	double springWristY ; \
	double damperWristY ; \
	double springWristX ; \
	double damperWristX ; \
	double springWristZ ; \
	double damperWristZ ; \
	double springWaistLegZ ; \
	double damperWaistLegZ ; \
	double springWaistLegX ; \
	double damperWaistLegX ; \
	double springWaistLegY ; \
	double damperWaistLegY ; \
	double springKnee ; \
	double damperKnee ; \
	double springAnkleY ; \
	double damperAnkleY ; \
	double springAnkleX ; \
	double damperAnkleX ; \
	double springAnkleZ ; \
	double damperAnkleZ ; \
	double springEyeY ; \
	double damperEyeY ; \
	double springEyeX ; \
	double damperEyeX ; \
	Vec3d posRightUpperArm ; \
	Quaterniond oriRightUpperArm ; \
	Quaterniond oriRightLowerArm ; \
	Quaterniond oriRightHand ; \
	Vec2d rangeWaistAbdomen ; \
	Vec2d rangeAbdomenChest ; \
	Vec2d rangeChestNeckX ; \
	Vec2d rangeChestNeckY ; \
	Vec2d rangeChestNeckZ ; \
	Vec2d rangeNeckHeadX ; \
	Vec2d rangeNeckHeadZ ; \
	Vec2d rangeShoulderZ ; \
	Vec2d rangeShoulderX ; \
	Vec2d rangeShoulderY ; \
	Vec2d rangeElbow ; \
	Vec2d rangeWristY ; \
	Vec2d rangeWristX ; \
	Vec2d rangeWristZ ; \
	Vec2d rangeWaistLegZ ; \
	Vec2d rangeWaistLegX ; \
	Vec2d rangeWaistLegY ; \
	Vec2d rangeKnee ; \
	Vec2d rangeAnkleY ; \
	Vec2d rangeAnkleX ; \
	Vec2d rangeAnkleZ ; \
	Vec2d rangeRightEyeY ; \
	Vec2d rangeEyeX ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistHeight = ((CRHingeHumanBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRHingeHumanBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRHingeHumanBodyDesc*)desc)->waistThickness; \
		abdomenHeight = ((CRHingeHumanBodyDesc*)desc)->abdomenHeight; \
		abdomenBreadth = ((CRHingeHumanBodyDesc*)desc)->abdomenBreadth; \
		abdomenThickness = ((CRHingeHumanBodyDesc*)desc)->abdomenThickness; \
		chestHeight = ((CRHingeHumanBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRHingeHumanBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRHingeHumanBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRHingeHumanBodyDesc*)desc)->neckLength; \
		neckDiameter = ((CRHingeHumanBodyDesc*)desc)->neckDiameter; \
		headDiameter = ((CRHingeHumanBodyDesc*)desc)->headDiameter; \
		upperArmLength = ((CRHingeHumanBodyDesc*)desc)->upperArmLength; \
		upperArmDiameter = ((CRHingeHumanBodyDesc*)desc)->upperArmDiameter; \
		lowerArmLength = ((CRHingeHumanBodyDesc*)desc)->lowerArmLength; \
		lowerArmDiameter = ((CRHingeHumanBodyDesc*)desc)->lowerArmDiameter; \
		handLength = ((CRHingeHumanBodyDesc*)desc)->handLength; \
		handBreadth = ((CRHingeHumanBodyDesc*)desc)->handBreadth; \
		handThickness = ((CRHingeHumanBodyDesc*)desc)->handThickness; \
		upperLegLength = ((CRHingeHumanBodyDesc*)desc)->upperLegLength; \
		upperLegDiameter = ((CRHingeHumanBodyDesc*)desc)->upperLegDiameter; \
		interLegDistance = ((CRHingeHumanBodyDesc*)desc)->interLegDistance; \
		lowerLegLength = ((CRHingeHumanBodyDesc*)desc)->lowerLegLength; \
		lowerLegDiameter = ((CRHingeHumanBodyDesc*)desc)->lowerLegDiameter; \
		footLength = ((CRHingeHumanBodyDesc*)desc)->footLength; \
		footBreadth = ((CRHingeHumanBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRHingeHumanBodyDesc*)desc)->footThickness; \
		ankleToeDistance = ((CRHingeHumanBodyDesc*)desc)->ankleToeDistance; \
		vertexToEyeHeight = ((CRHingeHumanBodyDesc*)desc)->vertexToEyeHeight; \
		occiputToEyeDistance = ((CRHingeHumanBodyDesc*)desc)->occiputToEyeDistance; \
		eyeDiameter = ((CRHingeHumanBodyDesc*)desc)->eyeDiameter; \
		interpupillaryBreadth = ((CRHingeHumanBodyDesc*)desc)->interpupillaryBreadth; \
		springWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->springWaistAbdomen; \
		damperWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->damperWaistAbdomen; \
		springAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->springAbdomenChest; \
		damperAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->damperAbdomenChest; \
		springChestNeckX = ((CRHingeHumanBodyDesc*)desc)->springChestNeckX; \
		damperChestNeckX = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckX; \
		springChestNeckY = ((CRHingeHumanBodyDesc*)desc)->springChestNeckY; \
		damperChestNeckY = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckY; \
		springChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->springChestNeckZ; \
		damperChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->damperChestNeckZ; \
		springNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->springNeckHeadX; \
		damperNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->damperNeckHeadX; \
		springNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->springNeckHeadZ; \
		damperNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->damperNeckHeadZ; \
		springShoulderZ = ((CRHingeHumanBodyDesc*)desc)->springShoulderZ; \
		damperShoulderZ = ((CRHingeHumanBodyDesc*)desc)->damperShoulderZ; \
		springShoulderX = ((CRHingeHumanBodyDesc*)desc)->springShoulderX; \
		damperShoulderX = ((CRHingeHumanBodyDesc*)desc)->damperShoulderX; \
		springShoulderY = ((CRHingeHumanBodyDesc*)desc)->springShoulderY; \
		damperShoulderY = ((CRHingeHumanBodyDesc*)desc)->damperShoulderY; \
		springElbow = ((CRHingeHumanBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeHumanBodyDesc*)desc)->damperElbow; \
		springWristY = ((CRHingeHumanBodyDesc*)desc)->springWristY; \
		damperWristY = ((CRHingeHumanBodyDesc*)desc)->damperWristY; \
		springWristX = ((CRHingeHumanBodyDesc*)desc)->springWristX; \
		damperWristX = ((CRHingeHumanBodyDesc*)desc)->damperWristX; \
		springWristZ = ((CRHingeHumanBodyDesc*)desc)->springWristZ; \
		damperWristZ = ((CRHingeHumanBodyDesc*)desc)->damperWristZ; \
		springWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->springWaistLegZ; \
		damperWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegZ; \
		springWaistLegX = ((CRHingeHumanBodyDesc*)desc)->springWaistLegX; \
		damperWaistLegX = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegX; \
		springWaistLegY = ((CRHingeHumanBodyDesc*)desc)->springWaistLegY; \
		damperWaistLegY = ((CRHingeHumanBodyDesc*)desc)->damperWaistLegY; \
		springKnee = ((CRHingeHumanBodyDesc*)desc)->springKnee; \
		damperKnee = ((CRHingeHumanBodyDesc*)desc)->damperKnee; \
		springAnkleY = ((CRHingeHumanBodyDesc*)desc)->springAnkleY; \
		damperAnkleY = ((CRHingeHumanBodyDesc*)desc)->damperAnkleY; \
		springAnkleX = ((CRHingeHumanBodyDesc*)desc)->springAnkleX; \
		damperAnkleX = ((CRHingeHumanBodyDesc*)desc)->damperAnkleX; \
		springAnkleZ = ((CRHingeHumanBodyDesc*)desc)->springAnkleZ; \
		damperAnkleZ = ((CRHingeHumanBodyDesc*)desc)->damperAnkleZ; \
		springEyeY = ((CRHingeHumanBodyDesc*)desc)->springEyeY; \
		damperEyeY = ((CRHingeHumanBodyDesc*)desc)->damperEyeY; \
		springEyeX = ((CRHingeHumanBodyDesc*)desc)->springEyeX; \
		damperEyeX = ((CRHingeHumanBodyDesc*)desc)->damperEyeX; \
		posRightUpperArm = ((CRHingeHumanBodyDesc*)desc)->posRightUpperArm; \
		oriRightUpperArm = ((CRHingeHumanBodyDesc*)desc)->oriRightUpperArm; \
		oriRightLowerArm = ((CRHingeHumanBodyDesc*)desc)->oriRightLowerArm; \
		oriRightHand = ((CRHingeHumanBodyDesc*)desc)->oriRightHand; \
		rangeWaistAbdomen = ((CRHingeHumanBodyDesc*)desc)->rangeWaistAbdomen; \
		rangeAbdomenChest = ((CRHingeHumanBodyDesc*)desc)->rangeAbdomenChest; \
		rangeChestNeckX = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckX; \
		rangeChestNeckY = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckY; \
		rangeChestNeckZ = ((CRHingeHumanBodyDesc*)desc)->rangeChestNeckZ; \
		rangeNeckHeadX = ((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadX; \
		rangeNeckHeadZ = ((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadZ; \
		rangeShoulderZ = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderZ; \
		rangeShoulderX = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderX; \
		rangeShoulderY = ((CRHingeHumanBodyDesc*)desc)->rangeShoulderY; \
		rangeElbow = ((CRHingeHumanBodyDesc*)desc)->rangeElbow; \
		rangeWristY = ((CRHingeHumanBodyDesc*)desc)->rangeWristY; \
		rangeWristX = ((CRHingeHumanBodyDesc*)desc)->rangeWristX; \
		rangeWristZ = ((CRHingeHumanBodyDesc*)desc)->rangeWristZ; \
		rangeWaistLegZ = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegZ; \
		rangeWaistLegX = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegX; \
		rangeWaistLegY = ((CRHingeHumanBodyDesc*)desc)->rangeWaistLegY; \
		rangeKnee = ((CRHingeHumanBodyDesc*)desc)->rangeKnee; \
		rangeAnkleY = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleY; \
		rangeAnkleX = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleX; \
		rangeAnkleZ = ((CRHingeHumanBodyDesc*)desc)->rangeAnkleZ; \
		rangeRightEyeY = ((CRHingeHumanBodyDesc*)desc)->rangeRightEyeY; \
		rangeEyeX = ((CRHingeHumanBodyDesc*)desc)->rangeEyeX; \
		noLegs = ((CRHingeHumanBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRHingeHumanBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeHumanBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeHumanBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeHumanBodyDesc*)desc)->abdomenHeight = abdomenHeight; \
		((CRHingeHumanBodyDesc*)desc)->abdomenBreadth = abdomenBreadth; \
		((CRHingeHumanBodyDesc*)desc)->abdomenThickness = abdomenThickness; \
		((CRHingeHumanBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeHumanBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeHumanBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeHumanBodyDesc*)desc)->neckLength = neckLength; \
		((CRHingeHumanBodyDesc*)desc)->neckDiameter = neckDiameter; \
		((CRHingeHumanBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRHingeHumanBodyDesc*)desc)->upperArmLength = upperArmLength; \
		((CRHingeHumanBodyDesc*)desc)->upperArmDiameter = upperArmDiameter; \
		((CRHingeHumanBodyDesc*)desc)->lowerArmLength = lowerArmLength; \
		((CRHingeHumanBodyDesc*)desc)->lowerArmDiameter = lowerArmDiameter; \
		((CRHingeHumanBodyDesc*)desc)->handLength = handLength; \
		((CRHingeHumanBodyDesc*)desc)->handBreadth = handBreadth; \
		((CRHingeHumanBodyDesc*)desc)->handThickness = handThickness; \
		((CRHingeHumanBodyDesc*)desc)->upperLegLength = upperLegLength; \
		((CRHingeHumanBodyDesc*)desc)->upperLegDiameter = upperLegDiameter; \
		((CRHingeHumanBodyDesc*)desc)->interLegDistance = interLegDistance; \
		((CRHingeHumanBodyDesc*)desc)->lowerLegLength = lowerLegLength; \
		((CRHingeHumanBodyDesc*)desc)->lowerLegDiameter = lowerLegDiameter; \
		((CRHingeHumanBodyDesc*)desc)->footLength = footLength; \
		((CRHingeHumanBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRHingeHumanBodyDesc*)desc)->footThickness = footThickness; \
		((CRHingeHumanBodyDesc*)desc)->ankleToeDistance = ankleToeDistance; \
		((CRHingeHumanBodyDesc*)desc)->vertexToEyeHeight = vertexToEyeHeight; \
		((CRHingeHumanBodyDesc*)desc)->occiputToEyeDistance = occiputToEyeDistance; \
		((CRHingeHumanBodyDesc*)desc)->eyeDiameter = eyeDiameter; \
		((CRHingeHumanBodyDesc*)desc)->interpupillaryBreadth = interpupillaryBreadth; \
		((CRHingeHumanBodyDesc*)desc)->springWaistAbdomen = springWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistAbdomen = damperWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->springAbdomenChest = springAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->damperAbdomenChest = damperAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckX = springChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckX = damperChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckY = springChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckY = damperChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->springChestNeckZ = springChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->damperChestNeckZ = damperChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->springNeckHeadX = springNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->damperNeckHeadX = damperNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->springNeckHeadZ = springNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->damperNeckHeadZ = damperNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderZ = springShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderZ = damperShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderX = springShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderX = damperShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->springShoulderY = springShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->damperShoulderY = damperShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeHumanBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeHumanBodyDesc*)desc)->springWristY = springWristY; \
		((CRHingeHumanBodyDesc*)desc)->damperWristY = damperWristY; \
		((CRHingeHumanBodyDesc*)desc)->springWristX = springWristX; \
		((CRHingeHumanBodyDesc*)desc)->damperWristX = damperWristX; \
		((CRHingeHumanBodyDesc*)desc)->springWristZ = springWristZ; \
		((CRHingeHumanBodyDesc*)desc)->damperWristZ = damperWristZ; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegZ = springWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegZ = damperWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegX = springWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegX = damperWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->springWaistLegY = springWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->damperWaistLegY = damperWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->springKnee = springKnee; \
		((CRHingeHumanBodyDesc*)desc)->damperKnee = damperKnee; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleY = springAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleY = damperAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleX = springAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleX = damperAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->springAnkleZ = springAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->damperAnkleZ = damperAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->springEyeY = springEyeY; \
		((CRHingeHumanBodyDesc*)desc)->damperEyeY = damperEyeY; \
		((CRHingeHumanBodyDesc*)desc)->springEyeX = springEyeX; \
		((CRHingeHumanBodyDesc*)desc)->damperEyeX = damperEyeX; \
		((CRHingeHumanBodyDesc*)desc)->posRightUpperArm = posRightUpperArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightUpperArm = oriRightUpperArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightLowerArm = oriRightLowerArm; \
		((CRHingeHumanBodyDesc*)desc)->oriRightHand = oriRightHand; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistAbdomen = rangeWaistAbdomen; \
		((CRHingeHumanBodyDesc*)desc)->rangeAbdomenChest = rangeAbdomenChest; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckX = rangeChestNeckX; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckY = rangeChestNeckY; \
		((CRHingeHumanBodyDesc*)desc)->rangeChestNeckZ = rangeChestNeckZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadX = rangeNeckHeadX; \
		((CRHingeHumanBodyDesc*)desc)->rangeNeckHeadZ = rangeNeckHeadZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderZ = rangeShoulderZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderX = rangeShoulderX; \
		((CRHingeHumanBodyDesc*)desc)->rangeShoulderY = rangeShoulderY; \
		((CRHingeHumanBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristY = rangeWristY; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristX = rangeWristX; \
		((CRHingeHumanBodyDesc*)desc)->rangeWristZ = rangeWristZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegZ = rangeWaistLegZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegX = rangeWaistLegX; \
		((CRHingeHumanBodyDesc*)desc)->rangeWaistLegY = rangeWaistLegY; \
		((CRHingeHumanBodyDesc*)desc)->rangeKnee = rangeKnee; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleY = rangeAnkleY; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleX = rangeAnkleX; \
		((CRHingeHumanBodyDesc*)desc)->rangeAnkleZ = rangeAnkleZ; \
		((CRHingeHumanBodyDesc*)desc)->rangeRightEyeY = rangeRightEyeY; \
		((CRHingeHumanBodyDesc*)desc)->rangeEyeX = rangeEyeX; \
		((CRHingeHumanBodyDesc*)desc)->noLegs = noLegs; \
		return true; \
	} \

#define ACCESS_DESC_CRHingeAnimalBodyDesc(cls) \
protected: \
	double waistBreadth ; \
	double waistHeight ; \
	double waistThickness ; \
	double chestBreadth ; \
	double chestHeight ; \
	double chestThickness ; \
	double tailBreath ; \
	double tailHeight ; \
	double tailThickness ; \
	double neckBreath ; \
	double neckHeight ; \
	double neckThickness ; \
	double headBreath ; \
	double headHeight ; \
	double headThickness ; \
	double breathtboneBreath ; \
	double breathtboneHeight ; \
	double breathtboneThickness ; \
	double radiusBreath ; \
	double radiusHeight ; \
	double radiusThickness ; \
	double frontCannonBoneBreath ; \
	double frontCannonBoneHeight ; \
	double frontCannonBoneThickness ; \
	double frontToeBreath ; \
	double frontToeHeight ; \
	double frontToeThickness ; \
	double femurBreath ; \
	double femurHeight ; \
	double femurThickness ; \
	double tibiaBreath ; \
	double tibiaHeight ; \
	double tibiaThickness ; \
	double rearCannonBoneBreath ; \
	double rearCannonBoneHeight ; \
	double rearCannonBoneThickness ; \
	double rearToeBreath ; \
	double rearToeHeight ; \
	double rearToeThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springTailWaist ; \
	double damperTailWaist ; \
	double springTail ; \
	double damperTail ; \
	double springChestNeckXY ; \
	double damperChestNeckXY ; \
	double springChestNeckYZ ; \
	double damperChestNeckYZ ; \
	double springChestNeck ; \
	double damperChestNeck ; \
	double springNeckHead ; \
	double damperNeckHead ; \
	double springShoulder_X ; \
	double damperShoulder_X ; \
	double springShoulder_Y ; \
	double damperShoulder_Y ; \
	double springShoulder_Z ; \
	double damperShoulder_Z ; \
	double springElbow ; \
	double damperElbow ; \
	double springFrontKnee ; \
	double damperFrontKnee ; \
	double springFrontAnkle_X ; \
	double damperFrontAnkle_X ; \
	double springFrontAnkle_Y ; \
	double damperFrontAnkle_Y ; \
	double springFrontAnkle_Z ; \
	double damperFrontAnkle_Z ; \
	double springHip_X ; \
	double damperHip_X ; \
	double springHip_Y ; \
	double damperHip_Y ; \
	double springHip_Z ; \
	double damperHip_Z ; \
	double springStifle ; \
	double damperStifle ; \
	double springRearKnee ; \
	double damperRearKnee ; \
	double springRearAnkle_X ; \
	double damperRearAnkle_X ; \
	double springRearAnkle_Y ; \
	double damperRearAnkle_Y ; \
	double springRearAnkle_Z ; \
	double damperRearAnkle_Z ; \
	Vec2d rangeChestNeckXY ; \
	Vec2d rangeChestNeckYZ ; \
	Vec2d rangeChestNeck ; \
	Vec2d rangeNeckHead ; \
	Vec2d rangeShoulder_X ; \
	Vec2d rangeShoulder_Y ; \
	Vec2d rangeShoulder_Z ; \
	Vec2d rangeElbow ; \
	Vec2d rangeFrontKnee ; \
	Vec2d rangeFrontAnkle_X ; \
	Vec2d rangeFrontAnkle_Y ; \
	Vec2d rangeFrontAnkle_Z ; \
	Vec2d rangeHip_X ; \
	Vec2d rangeHip_Y ; \
	Vec2d rangeHip_Z ; \
	Vec2d rangeStifle ; \
	Vec2d rangeRearKnee ; \
	Vec2d rangeRearAnkle_X ; \
	Vec2d rangeRearAnkle_Y ; \
	Vec2d rangeRearAnkle_Z ; \
	Quaterniond oriTailWaist ; \
	Quaterniond oriTail ; \
	Quaterniond oriNeckHead ; \
	double rangeWaistChest ; \
	double rangeTailWaist ; \
	double rangeTail ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistBreadth = ((CRHingeAnimalBodyDesc*)desc)->waistBreadth; \
		waistHeight = ((CRHingeAnimalBodyDesc*)desc)->waistHeight; \
		waistThickness = ((CRHingeAnimalBodyDesc*)desc)->waistThickness; \
		chestBreadth = ((CRHingeAnimalBodyDesc*)desc)->chestBreadth; \
		chestHeight = ((CRHingeAnimalBodyDesc*)desc)->chestHeight; \
		chestThickness = ((CRHingeAnimalBodyDesc*)desc)->chestThickness; \
		tailBreath = ((CRHingeAnimalBodyDesc*)desc)->tailBreath; \
		tailHeight = ((CRHingeAnimalBodyDesc*)desc)->tailHeight; \
		tailThickness = ((CRHingeAnimalBodyDesc*)desc)->tailThickness; \
		neckBreath = ((CRHingeAnimalBodyDesc*)desc)->neckBreath; \
		neckHeight = ((CRHingeAnimalBodyDesc*)desc)->neckHeight; \
		neckThickness = ((CRHingeAnimalBodyDesc*)desc)->neckThickness; \
		headBreath = ((CRHingeAnimalBodyDesc*)desc)->headBreath; \
		headHeight = ((CRHingeAnimalBodyDesc*)desc)->headHeight; \
		headThickness = ((CRHingeAnimalBodyDesc*)desc)->headThickness; \
		breathtboneBreath = ((CRHingeAnimalBodyDesc*)desc)->breathtboneBreath; \
		breathtboneHeight = ((CRHingeAnimalBodyDesc*)desc)->breathtboneHeight; \
		breathtboneThickness = ((CRHingeAnimalBodyDesc*)desc)->breathtboneThickness; \
		radiusBreath = ((CRHingeAnimalBodyDesc*)desc)->radiusBreath; \
		radiusHeight = ((CRHingeAnimalBodyDesc*)desc)->radiusHeight; \
		radiusThickness = ((CRHingeAnimalBodyDesc*)desc)->radiusThickness; \
		frontCannonBoneBreath = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneBreath; \
		frontCannonBoneHeight = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneHeight; \
		frontCannonBoneThickness = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneThickness; \
		frontToeBreath = ((CRHingeAnimalBodyDesc*)desc)->frontToeBreath; \
		frontToeHeight = ((CRHingeAnimalBodyDesc*)desc)->frontToeHeight; \
		frontToeThickness = ((CRHingeAnimalBodyDesc*)desc)->frontToeThickness; \
		femurBreath = ((CRHingeAnimalBodyDesc*)desc)->femurBreath; \
		femurHeight = ((CRHingeAnimalBodyDesc*)desc)->femurHeight; \
		femurThickness = ((CRHingeAnimalBodyDesc*)desc)->femurThickness; \
		tibiaBreath = ((CRHingeAnimalBodyDesc*)desc)->tibiaBreath; \
		tibiaHeight = ((CRHingeAnimalBodyDesc*)desc)->tibiaHeight; \
		tibiaThickness = ((CRHingeAnimalBodyDesc*)desc)->tibiaThickness; \
		rearCannonBoneBreath = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneBreath; \
		rearCannonBoneHeight = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneHeight; \
		rearCannonBoneThickness = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneThickness; \
		rearToeBreath = ((CRHingeAnimalBodyDesc*)desc)->rearToeBreath; \
		rearToeHeight = ((CRHingeAnimalBodyDesc*)desc)->rearToeHeight; \
		rearToeThickness = ((CRHingeAnimalBodyDesc*)desc)->rearToeThickness; \
		springWaistChest = ((CRHingeAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRHingeAnimalBodyDesc*)desc)->damperWaistChest; \
		springTailWaist = ((CRHingeAnimalBodyDesc*)desc)->springTailWaist; \
		damperTailWaist = ((CRHingeAnimalBodyDesc*)desc)->damperTailWaist; \
		springTail = ((CRHingeAnimalBodyDesc*)desc)->springTail; \
		damperTail = ((CRHingeAnimalBodyDesc*)desc)->damperTail; \
		springChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->springChestNeckXY; \
		damperChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeckXY; \
		springChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->springChestNeckYZ; \
		damperChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeckYZ; \
		springChestNeck = ((CRHingeAnimalBodyDesc*)desc)->springChestNeck; \
		damperChestNeck = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeck; \
		springNeckHead = ((CRHingeAnimalBodyDesc*)desc)->springNeckHead; \
		damperNeckHead = ((CRHingeAnimalBodyDesc*)desc)->damperNeckHead; \
		springShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_X; \
		damperShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_X; \
		springShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_Y; \
		damperShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Y; \
		springShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_Z; \
		damperShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Z; \
		springElbow = ((CRHingeAnimalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeAnimalBodyDesc*)desc)->damperElbow; \
		springFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->springFrontKnee; \
		damperFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->damperFrontKnee; \
		springFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_X; \
		damperFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_X; \
		springFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Y; \
		damperFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Y; \
		springFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Z; \
		damperFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Z; \
		springHip_X = ((CRHingeAnimalBodyDesc*)desc)->springHip_X; \
		damperHip_X = ((CRHingeAnimalBodyDesc*)desc)->damperHip_X; \
		springHip_Y = ((CRHingeAnimalBodyDesc*)desc)->springHip_Y; \
		damperHip_Y = ((CRHingeAnimalBodyDesc*)desc)->damperHip_Y; \
		springHip_Z = ((CRHingeAnimalBodyDesc*)desc)->springHip_Z; \
		damperHip_Z = ((CRHingeAnimalBodyDesc*)desc)->damperHip_Z; \
		springStifle = ((CRHingeAnimalBodyDesc*)desc)->springStifle; \
		damperStifle = ((CRHingeAnimalBodyDesc*)desc)->damperStifle; \
		springRearKnee = ((CRHingeAnimalBodyDesc*)desc)->springRearKnee; \
		damperRearKnee = ((CRHingeAnimalBodyDesc*)desc)->damperRearKnee; \
		springRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_X; \
		damperRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_X; \
		springRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Y; \
		damperRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Y; \
		springRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Z; \
		damperRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Z; \
		rangeChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckXY; \
		rangeChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckYZ; \
		rangeChestNeck = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeck; \
		rangeNeckHead = ((CRHingeAnimalBodyDesc*)desc)->rangeNeckHead; \
		rangeShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_X; \
		rangeShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Y; \
		rangeShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Z; \
		rangeElbow = ((CRHingeAnimalBodyDesc*)desc)->rangeElbow; \
		rangeFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontKnee; \
		rangeFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_X; \
		rangeFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Y; \
		rangeFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Z; \
		rangeHip_X = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_X; \
		rangeHip_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_Y; \
		rangeHip_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_Z; \
		rangeStifle = ((CRHingeAnimalBodyDesc*)desc)->rangeStifle; \
		rangeRearKnee = ((CRHingeAnimalBodyDesc*)desc)->rangeRearKnee; \
		rangeRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_X; \
		rangeRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Y; \
		rangeRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Z; \
		oriTailWaist = ((CRHingeAnimalBodyDesc*)desc)->oriTailWaist; \
		oriTail = ((CRHingeAnimalBodyDesc*)desc)->oriTail; \
		oriNeckHead = ((CRHingeAnimalBodyDesc*)desc)->oriNeckHead; \
		rangeWaistChest = ((CRHingeAnimalBodyDesc*)desc)->rangeWaistChest; \
		rangeTailWaist = ((CRHingeAnimalBodyDesc*)desc)->rangeTailWaist; \
		rangeTail = ((CRHingeAnimalBodyDesc*)desc)->rangeTail; \
		noLegs = ((CRHingeAnimalBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRHingeAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeAnimalBodyDesc*)desc)->tailBreath = tailBreath; \
		((CRHingeAnimalBodyDesc*)desc)->tailHeight = tailHeight; \
		((CRHingeAnimalBodyDesc*)desc)->tailThickness = tailThickness; \
		((CRHingeAnimalBodyDesc*)desc)->neckBreath = neckBreath; \
		((CRHingeAnimalBodyDesc*)desc)->neckHeight = neckHeight; \
		((CRHingeAnimalBodyDesc*)desc)->neckThickness = neckThickness; \
		((CRHingeAnimalBodyDesc*)desc)->headBreath = headBreath; \
		((CRHingeAnimalBodyDesc*)desc)->headHeight = headHeight; \
		((CRHingeAnimalBodyDesc*)desc)->headThickness = headThickness; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneBreath = breathtboneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneHeight = breathtboneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneThickness = breathtboneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->radiusBreath = radiusBreath; \
		((CRHingeAnimalBodyDesc*)desc)->radiusHeight = radiusHeight; \
		((CRHingeAnimalBodyDesc*)desc)->radiusThickness = radiusThickness; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneBreath = frontCannonBoneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneHeight = frontCannonBoneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneThickness = frontCannonBoneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeBreath = frontToeBreath; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeHeight = frontToeHeight; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeThickness = frontToeThickness; \
		((CRHingeAnimalBodyDesc*)desc)->femurBreath = femurBreath; \
		((CRHingeAnimalBodyDesc*)desc)->femurHeight = femurHeight; \
		((CRHingeAnimalBodyDesc*)desc)->femurThickness = femurThickness; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaBreath = tibiaBreath; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaHeight = tibiaHeight; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaThickness = tibiaThickness; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneBreath = rearCannonBoneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneHeight = rearCannonBoneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneThickness = rearCannonBoneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeBreath = rearToeBreath; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeHeight = rearToeHeight; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeThickness = rearToeThickness; \
		((CRHingeAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->springTailWaist = springTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->damperTailWaist = damperTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->springTail = springTail; \
		((CRHingeAnimalBodyDesc*)desc)->damperTail = damperTail; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeckXY = springChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeckXY = damperChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeckYZ = springChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeckYZ = damperChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeck = springChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeck = damperChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->springNeckHead = springNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->damperNeckHead = damperNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_X = springShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_X = damperShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_Y = springShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Y = damperShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_Z = springShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Z = damperShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeAnimalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontKnee = springFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontKnee = damperFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_X = springFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_X = damperFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Y = springFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Y = damperFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Z = springFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Z = damperFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_X = springHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_X = damperHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_Y = springHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_Y = damperHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_Z = springHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_Z = damperHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springStifle = springStifle; \
		((CRHingeAnimalBodyDesc*)desc)->damperStifle = damperStifle; \
		((CRHingeAnimalBodyDesc*)desc)->springRearKnee = springRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearKnee = damperRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_X = springRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_X = damperRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Y = springRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Y = damperRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Z = springRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Z = damperRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckXY = rangeChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckYZ = rangeChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeck = rangeChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->rangeNeckHead = rangeNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_X = rangeShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Y = rangeShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Z = rangeShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontKnee = rangeFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_X = rangeFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Y = rangeFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Z = rangeFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_X = rangeHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_Y = rangeHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_Z = rangeHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeStifle = rangeStifle; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearKnee = rangeRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_X = rangeRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Y = rangeRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Z = rangeRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->oriTailWaist = oriTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->oriTail = oriTail; \
		((CRHingeAnimalBodyDesc*)desc)->oriNeckHead = oriNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->rangeTailWaist = rangeTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->rangeTail = rangeTail; \
		((CRHingeAnimalBodyDesc*)desc)->noLegs = noLegs; \
		return true; \
	} \

#define ACCESS_DESC_CRHingeAnimalBodyDesc_NOBASE \
protected: \
	double waistBreadth ; \
	double waistHeight ; \
	double waistThickness ; \
	double chestBreadth ; \
	double chestHeight ; \
	double chestThickness ; \
	double tailBreath ; \
	double tailHeight ; \
	double tailThickness ; \
	double neckBreath ; \
	double neckHeight ; \
	double neckThickness ; \
	double headBreath ; \
	double headHeight ; \
	double headThickness ; \
	double breathtboneBreath ; \
	double breathtboneHeight ; \
	double breathtboneThickness ; \
	double radiusBreath ; \
	double radiusHeight ; \
	double radiusThickness ; \
	double frontCannonBoneBreath ; \
	double frontCannonBoneHeight ; \
	double frontCannonBoneThickness ; \
	double frontToeBreath ; \
	double frontToeHeight ; \
	double frontToeThickness ; \
	double femurBreath ; \
	double femurHeight ; \
	double femurThickness ; \
	double tibiaBreath ; \
	double tibiaHeight ; \
	double tibiaThickness ; \
	double rearCannonBoneBreath ; \
	double rearCannonBoneHeight ; \
	double rearCannonBoneThickness ; \
	double rearToeBreath ; \
	double rearToeHeight ; \
	double rearToeThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springTailWaist ; \
	double damperTailWaist ; \
	double springTail ; \
	double damperTail ; \
	double springChestNeckXY ; \
	double damperChestNeckXY ; \
	double springChestNeckYZ ; \
	double damperChestNeckYZ ; \
	double springChestNeck ; \
	double damperChestNeck ; \
	double springNeckHead ; \
	double damperNeckHead ; \
	double springShoulder_X ; \
	double damperShoulder_X ; \
	double springShoulder_Y ; \
	double damperShoulder_Y ; \
	double springShoulder_Z ; \
	double damperShoulder_Z ; \
	double springElbow ; \
	double damperElbow ; \
	double springFrontKnee ; \
	double damperFrontKnee ; \
	double springFrontAnkle_X ; \
	double damperFrontAnkle_X ; \
	double springFrontAnkle_Y ; \
	double damperFrontAnkle_Y ; \
	double springFrontAnkle_Z ; \
	double damperFrontAnkle_Z ; \
	double springHip_X ; \
	double damperHip_X ; \
	double springHip_Y ; \
	double damperHip_Y ; \
	double springHip_Z ; \
	double damperHip_Z ; \
	double springStifle ; \
	double damperStifle ; \
	double springRearKnee ; \
	double damperRearKnee ; \
	double springRearAnkle_X ; \
	double damperRearAnkle_X ; \
	double springRearAnkle_Y ; \
	double damperRearAnkle_Y ; \
	double springRearAnkle_Z ; \
	double damperRearAnkle_Z ; \
	Vec2d rangeChestNeckXY ; \
	Vec2d rangeChestNeckYZ ; \
	Vec2d rangeChestNeck ; \
	Vec2d rangeNeckHead ; \
	Vec2d rangeShoulder_X ; \
	Vec2d rangeShoulder_Y ; \
	Vec2d rangeShoulder_Z ; \
	Vec2d rangeElbow ; \
	Vec2d rangeFrontKnee ; \
	Vec2d rangeFrontAnkle_X ; \
	Vec2d rangeFrontAnkle_Y ; \
	Vec2d rangeFrontAnkle_Z ; \
	Vec2d rangeHip_X ; \
	Vec2d rangeHip_Y ; \
	Vec2d rangeHip_Z ; \
	Vec2d rangeStifle ; \
	Vec2d rangeRearKnee ; \
	Vec2d rangeRearAnkle_X ; \
	Vec2d rangeRearAnkle_Y ; \
	Vec2d rangeRearAnkle_Z ; \
	Quaterniond oriTailWaist ; \
	Quaterniond oriTail ; \
	Quaterniond oriNeckHead ; \
	double rangeWaistChest ; \
	double rangeTailWaist ; \
	double rangeTail ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistBreadth = ((CRHingeAnimalBodyDesc*)desc)->waistBreadth; \
		waistHeight = ((CRHingeAnimalBodyDesc*)desc)->waistHeight; \
		waistThickness = ((CRHingeAnimalBodyDesc*)desc)->waistThickness; \
		chestBreadth = ((CRHingeAnimalBodyDesc*)desc)->chestBreadth; \
		chestHeight = ((CRHingeAnimalBodyDesc*)desc)->chestHeight; \
		chestThickness = ((CRHingeAnimalBodyDesc*)desc)->chestThickness; \
		tailBreath = ((CRHingeAnimalBodyDesc*)desc)->tailBreath; \
		tailHeight = ((CRHingeAnimalBodyDesc*)desc)->tailHeight; \
		tailThickness = ((CRHingeAnimalBodyDesc*)desc)->tailThickness; \
		neckBreath = ((CRHingeAnimalBodyDesc*)desc)->neckBreath; \
		neckHeight = ((CRHingeAnimalBodyDesc*)desc)->neckHeight; \
		neckThickness = ((CRHingeAnimalBodyDesc*)desc)->neckThickness; \
		headBreath = ((CRHingeAnimalBodyDesc*)desc)->headBreath; \
		headHeight = ((CRHingeAnimalBodyDesc*)desc)->headHeight; \
		headThickness = ((CRHingeAnimalBodyDesc*)desc)->headThickness; \
		breathtboneBreath = ((CRHingeAnimalBodyDesc*)desc)->breathtboneBreath; \
		breathtboneHeight = ((CRHingeAnimalBodyDesc*)desc)->breathtboneHeight; \
		breathtboneThickness = ((CRHingeAnimalBodyDesc*)desc)->breathtboneThickness; \
		radiusBreath = ((CRHingeAnimalBodyDesc*)desc)->radiusBreath; \
		radiusHeight = ((CRHingeAnimalBodyDesc*)desc)->radiusHeight; \
		radiusThickness = ((CRHingeAnimalBodyDesc*)desc)->radiusThickness; \
		frontCannonBoneBreath = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneBreath; \
		frontCannonBoneHeight = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneHeight; \
		frontCannonBoneThickness = ((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneThickness; \
		frontToeBreath = ((CRHingeAnimalBodyDesc*)desc)->frontToeBreath; \
		frontToeHeight = ((CRHingeAnimalBodyDesc*)desc)->frontToeHeight; \
		frontToeThickness = ((CRHingeAnimalBodyDesc*)desc)->frontToeThickness; \
		femurBreath = ((CRHingeAnimalBodyDesc*)desc)->femurBreath; \
		femurHeight = ((CRHingeAnimalBodyDesc*)desc)->femurHeight; \
		femurThickness = ((CRHingeAnimalBodyDesc*)desc)->femurThickness; \
		tibiaBreath = ((CRHingeAnimalBodyDesc*)desc)->tibiaBreath; \
		tibiaHeight = ((CRHingeAnimalBodyDesc*)desc)->tibiaHeight; \
		tibiaThickness = ((CRHingeAnimalBodyDesc*)desc)->tibiaThickness; \
		rearCannonBoneBreath = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneBreath; \
		rearCannonBoneHeight = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneHeight; \
		rearCannonBoneThickness = ((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneThickness; \
		rearToeBreath = ((CRHingeAnimalBodyDesc*)desc)->rearToeBreath; \
		rearToeHeight = ((CRHingeAnimalBodyDesc*)desc)->rearToeHeight; \
		rearToeThickness = ((CRHingeAnimalBodyDesc*)desc)->rearToeThickness; \
		springWaistChest = ((CRHingeAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRHingeAnimalBodyDesc*)desc)->damperWaistChest; \
		springTailWaist = ((CRHingeAnimalBodyDesc*)desc)->springTailWaist; \
		damperTailWaist = ((CRHingeAnimalBodyDesc*)desc)->damperTailWaist; \
		springTail = ((CRHingeAnimalBodyDesc*)desc)->springTail; \
		damperTail = ((CRHingeAnimalBodyDesc*)desc)->damperTail; \
		springChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->springChestNeckXY; \
		damperChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeckXY; \
		springChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->springChestNeckYZ; \
		damperChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeckYZ; \
		springChestNeck = ((CRHingeAnimalBodyDesc*)desc)->springChestNeck; \
		damperChestNeck = ((CRHingeAnimalBodyDesc*)desc)->damperChestNeck; \
		springNeckHead = ((CRHingeAnimalBodyDesc*)desc)->springNeckHead; \
		damperNeckHead = ((CRHingeAnimalBodyDesc*)desc)->damperNeckHead; \
		springShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_X; \
		damperShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_X; \
		springShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_Y; \
		damperShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Y; \
		springShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->springShoulder_Z; \
		damperShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Z; \
		springElbow = ((CRHingeAnimalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeAnimalBodyDesc*)desc)->damperElbow; \
		springFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->springFrontKnee; \
		damperFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->damperFrontKnee; \
		springFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_X; \
		damperFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_X; \
		springFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Y; \
		damperFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Y; \
		springFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Z; \
		damperFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Z; \
		springHip_X = ((CRHingeAnimalBodyDesc*)desc)->springHip_X; \
		damperHip_X = ((CRHingeAnimalBodyDesc*)desc)->damperHip_X; \
		springHip_Y = ((CRHingeAnimalBodyDesc*)desc)->springHip_Y; \
		damperHip_Y = ((CRHingeAnimalBodyDesc*)desc)->damperHip_Y; \
		springHip_Z = ((CRHingeAnimalBodyDesc*)desc)->springHip_Z; \
		damperHip_Z = ((CRHingeAnimalBodyDesc*)desc)->damperHip_Z; \
		springStifle = ((CRHingeAnimalBodyDesc*)desc)->springStifle; \
		damperStifle = ((CRHingeAnimalBodyDesc*)desc)->damperStifle; \
		springRearKnee = ((CRHingeAnimalBodyDesc*)desc)->springRearKnee; \
		damperRearKnee = ((CRHingeAnimalBodyDesc*)desc)->damperRearKnee; \
		springRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_X; \
		damperRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_X; \
		springRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Y; \
		damperRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Y; \
		springRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Z; \
		damperRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Z; \
		rangeChestNeckXY = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckXY; \
		rangeChestNeckYZ = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckYZ; \
		rangeChestNeck = ((CRHingeAnimalBodyDesc*)desc)->rangeChestNeck; \
		rangeNeckHead = ((CRHingeAnimalBodyDesc*)desc)->rangeNeckHead; \
		rangeShoulder_X = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_X; \
		rangeShoulder_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Y; \
		rangeShoulder_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Z; \
		rangeElbow = ((CRHingeAnimalBodyDesc*)desc)->rangeElbow; \
		rangeFrontKnee = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontKnee; \
		rangeFrontAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_X; \
		rangeFrontAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Y; \
		rangeFrontAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Z; \
		rangeHip_X = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_X; \
		rangeHip_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_Y; \
		rangeHip_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeHip_Z; \
		rangeStifle = ((CRHingeAnimalBodyDesc*)desc)->rangeStifle; \
		rangeRearKnee = ((CRHingeAnimalBodyDesc*)desc)->rangeRearKnee; \
		rangeRearAnkle_X = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_X; \
		rangeRearAnkle_Y = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Y; \
		rangeRearAnkle_Z = ((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Z; \
		oriTailWaist = ((CRHingeAnimalBodyDesc*)desc)->oriTailWaist; \
		oriTail = ((CRHingeAnimalBodyDesc*)desc)->oriTail; \
		oriNeckHead = ((CRHingeAnimalBodyDesc*)desc)->oriNeckHead; \
		rangeWaistChest = ((CRHingeAnimalBodyDesc*)desc)->rangeWaistChest; \
		rangeTailWaist = ((CRHingeAnimalBodyDesc*)desc)->rangeTailWaist; \
		rangeTail = ((CRHingeAnimalBodyDesc*)desc)->rangeTail; \
		noLegs = ((CRHingeAnimalBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRHingeAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeAnimalBodyDesc*)desc)->tailBreath = tailBreath; \
		((CRHingeAnimalBodyDesc*)desc)->tailHeight = tailHeight; \
		((CRHingeAnimalBodyDesc*)desc)->tailThickness = tailThickness; \
		((CRHingeAnimalBodyDesc*)desc)->neckBreath = neckBreath; \
		((CRHingeAnimalBodyDesc*)desc)->neckHeight = neckHeight; \
		((CRHingeAnimalBodyDesc*)desc)->neckThickness = neckThickness; \
		((CRHingeAnimalBodyDesc*)desc)->headBreath = headBreath; \
		((CRHingeAnimalBodyDesc*)desc)->headHeight = headHeight; \
		((CRHingeAnimalBodyDesc*)desc)->headThickness = headThickness; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneBreath = breathtboneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneHeight = breathtboneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->breathtboneThickness = breathtboneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->radiusBreath = radiusBreath; \
		((CRHingeAnimalBodyDesc*)desc)->radiusHeight = radiusHeight; \
		((CRHingeAnimalBodyDesc*)desc)->radiusThickness = radiusThickness; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneBreath = frontCannonBoneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneHeight = frontCannonBoneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->frontCannonBoneThickness = frontCannonBoneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeBreath = frontToeBreath; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeHeight = frontToeHeight; \
		((CRHingeAnimalBodyDesc*)desc)->frontToeThickness = frontToeThickness; \
		((CRHingeAnimalBodyDesc*)desc)->femurBreath = femurBreath; \
		((CRHingeAnimalBodyDesc*)desc)->femurHeight = femurHeight; \
		((CRHingeAnimalBodyDesc*)desc)->femurThickness = femurThickness; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaBreath = tibiaBreath; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaHeight = tibiaHeight; \
		((CRHingeAnimalBodyDesc*)desc)->tibiaThickness = tibiaThickness; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneBreath = rearCannonBoneBreath; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneHeight = rearCannonBoneHeight; \
		((CRHingeAnimalBodyDesc*)desc)->rearCannonBoneThickness = rearCannonBoneThickness; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeBreath = rearToeBreath; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeHeight = rearToeHeight; \
		((CRHingeAnimalBodyDesc*)desc)->rearToeThickness = rearToeThickness; \
		((CRHingeAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->springTailWaist = springTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->damperTailWaist = damperTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->springTail = springTail; \
		((CRHingeAnimalBodyDesc*)desc)->damperTail = damperTail; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeckXY = springChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeckXY = damperChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeckYZ = springChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeckYZ = damperChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->springChestNeck = springChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->damperChestNeck = damperChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->springNeckHead = springNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->damperNeckHead = damperNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_X = springShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_X = damperShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_Y = springShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Y = damperShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springShoulder_Z = springShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperShoulder_Z = damperShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeAnimalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontKnee = springFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontKnee = damperFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_X = springFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_X = damperFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Y = springFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Y = damperFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springFrontAnkle_Z = springFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperFrontAnkle_Z = damperFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_X = springHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_X = damperHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_Y = springHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_Y = damperHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springHip_Z = springHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperHip_Z = damperHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->springStifle = springStifle; \
		((CRHingeAnimalBodyDesc*)desc)->damperStifle = damperStifle; \
		((CRHingeAnimalBodyDesc*)desc)->springRearKnee = springRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearKnee = damperRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_X = springRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_X = damperRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Y = springRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Y = damperRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->springRearAnkle_Z = springRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->damperRearAnkle_Z = damperRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckXY = rangeChestNeckXY; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeckYZ = rangeChestNeckYZ; \
		((CRHingeAnimalBodyDesc*)desc)->rangeChestNeck = rangeChestNeck; \
		((CRHingeAnimalBodyDesc*)desc)->rangeNeckHead = rangeNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_X = rangeShoulder_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Y = rangeShoulder_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeShoulder_Z = rangeShoulder_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontKnee = rangeFrontKnee; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_X = rangeFrontAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Y = rangeFrontAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeFrontAnkle_Z = rangeFrontAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_X = rangeHip_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_Y = rangeHip_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeHip_Z = rangeHip_Z; \
		((CRHingeAnimalBodyDesc*)desc)->rangeStifle = rangeStifle; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearKnee = rangeRearKnee; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_X = rangeRearAnkle_X; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Y = rangeRearAnkle_Y; \
		((CRHingeAnimalBodyDesc*)desc)->rangeRearAnkle_Z = rangeRearAnkle_Z; \
		((CRHingeAnimalBodyDesc*)desc)->oriTailWaist = oriTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->oriTail = oriTail; \
		((CRHingeAnimalBodyDesc*)desc)->oriNeckHead = oriNeckHead; \
		((CRHingeAnimalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRHingeAnimalBodyDesc*)desc)->rangeTailWaist = rangeTailWaist; \
		((CRHingeAnimalBodyDesc*)desc)->rangeTail = rangeTail; \
		((CRHingeAnimalBodyDesc*)desc)->noLegs = noLegs; \
		return true; \
	} \

#define ACCESS_DESC_CRTrunkFootHumanBodyDesc(cls) \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double headDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springChestHead ; \
	double damperChestHead ; \
	Vec2d rangeWaistChest ; \
	Vec2d rangeChestHead ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistHeight = ((CRTrunkFootHumanBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootHumanBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootHumanBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootHumanBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootHumanBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootHumanBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootHumanBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootHumanBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRTrunkFootHumanBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootHumanBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootHumanBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootHumanBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootHumanBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootHumanBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootHumanBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
		return true; \
	} \

#define ACCESS_DESC_CRTrunkFootHumanBodyDesc_NOBASE \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double headDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springChestHead ; \
	double damperChestHead ; \
	Vec2d rangeWaistChest ; \
	Vec2d rangeChestHead ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistHeight = ((CRTrunkFootHumanBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootHumanBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootHumanBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootHumanBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootHumanBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootHumanBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootHumanBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootHumanBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootHumanBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootHumanBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootHumanBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRTrunkFootHumanBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootHumanBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootHumanBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootHumanBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootHumanBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootHumanBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootHumanBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootHumanBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootHumanBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootHumanBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
		return true; \
	} \

#define ACCESS_DESC_CRTrunkFootAnimalBodyDesc(cls) \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double headDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springChestHead ; \
	double damperChestHead ; \
	Vec2d rangeWaistChest ; \
	Vec2d rangeChestHead ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistHeight = ((CRTrunkFootAnimalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootAnimalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootAnimalBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootAnimalBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootAnimalBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootAnimalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootAnimalBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootAnimalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
		return true; \
	} \

#define ACCESS_DESC_CRTrunkFootAnimalBodyDesc_NOBASE \
protected: \
	double waistHeight ; \
	double waistBreadth ; \
	double waistThickness ; \
	double chestHeight ; \
	double chestBreadth ; \
	double chestThickness ; \
	double neckLength ; \
	double headDiameter ; \
	double footLength ; \
	double footBreadth ; \
	double footThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springChestHead ; \
	double damperChestHead ; \
	Vec2d rangeWaistChest ; \
	Vec2d rangeChestHead ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistHeight = ((CRTrunkFootAnimalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootAnimalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootAnimalBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootAnimalBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootAnimalBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootAnimalBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootAnimalBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootAnimalBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootAnimalBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootAnimalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootAnimalBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootAnimalBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootAnimalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootAnimalBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
		return true; \
	} \

#define ACCESS_DESC_CRControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CREyeControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CREyeControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRNeckControllerDesc(cls) \
protected: \
	float lowerAttractiveness ; \
	float upperAttractiveness ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		lowerAttractiveness = ((CRNeckControllerDesc*)desc)->lowerAttractiveness; \
		upperAttractiveness = ((CRNeckControllerDesc*)desc)->upperAttractiveness; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRNeckControllerDesc*)desc)->lowerAttractiveness = lowerAttractiveness; \
		((CRNeckControllerDesc*)desc)->upperAttractiveness = upperAttractiveness; \
		return true; \
	} \

#define ACCESS_DESC_CRNeckControllerDesc_NOBASE \
protected: \
	float lowerAttractiveness ; \
	float upperAttractiveness ; \
public: \
	virtual void SetDesc(const void* desc){ \
		lowerAttractiveness = ((CRNeckControllerDesc*)desc)->lowerAttractiveness; \
		upperAttractiveness = ((CRNeckControllerDesc*)desc)->upperAttractiveness; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRNeckControllerDesc*)desc)->lowerAttractiveness = lowerAttractiveness; \
		((CRNeckControllerDesc*)desc)->upperAttractiveness = upperAttractiveness; \
		return true; \
	} \

#define ACCESS_DESC_CRReachingControllerDesc(cls) \
protected: \
	PHSolidIf *solid ; \
	Vec3f reachPos ; \
	Quaterniond fixOri ; \
	float limitForce ; \
	float springPos ; \
	float damperPos ; \
	float springOri ; \
	float damperOri ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		solid = ((CRReachingControllerDesc*)desc)->solid; \
		reachPos = ((CRReachingControllerDesc*)desc)->reachPos; \
		fixOri = ((CRReachingControllerDesc*)desc)->fixOri; \
		limitForce = ((CRReachingControllerDesc*)desc)->limitForce; \
		springPos = ((CRReachingControllerDesc*)desc)->springPos; \
		damperPos = ((CRReachingControllerDesc*)desc)->damperPos; \
		springOri = ((CRReachingControllerDesc*)desc)->springOri; \
		damperOri = ((CRReachingControllerDesc*)desc)->damperOri; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRReachingControllerDesc*)desc)->solid = solid; \
		((CRReachingControllerDesc*)desc)->reachPos = reachPos; \
		((CRReachingControllerDesc*)desc)->fixOri = fixOri; \
		((CRReachingControllerDesc*)desc)->limitForce = limitForce; \
		((CRReachingControllerDesc*)desc)->springPos = springPos; \
		((CRReachingControllerDesc*)desc)->damperPos = damperPos; \
		((CRReachingControllerDesc*)desc)->springOri = springOri; \
		((CRReachingControllerDesc*)desc)->damperOri = damperOri; \
		return true; \
	} \

#define ACCESS_DESC_CRReachingControllerDesc_NOBASE \
protected: \
	PHSolidIf *solid ; \
	Vec3f reachPos ; \
	Quaterniond fixOri ; \
	float limitForce ; \
	float springPos ; \
	float damperPos ; \
	float springOri ; \
	float damperOri ; \
public: \
	virtual void SetDesc(const void* desc){ \
		solid = ((CRReachingControllerDesc*)desc)->solid; \
		reachPos = ((CRReachingControllerDesc*)desc)->reachPos; \
		fixOri = ((CRReachingControllerDesc*)desc)->fixOri; \
		limitForce = ((CRReachingControllerDesc*)desc)->limitForce; \
		springPos = ((CRReachingControllerDesc*)desc)->springPos; \
		damperPos = ((CRReachingControllerDesc*)desc)->damperPos; \
		springOri = ((CRReachingControllerDesc*)desc)->springOri; \
		damperOri = ((CRReachingControllerDesc*)desc)->damperOri; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRReachingControllerDesc*)desc)->solid = solid; \
		((CRReachingControllerDesc*)desc)->reachPos = reachPos; \
		((CRReachingControllerDesc*)desc)->fixOri = fixOri; \
		((CRReachingControllerDesc*)desc)->limitForce = limitForce; \
		((CRReachingControllerDesc*)desc)->springPos = springPos; \
		((CRReachingControllerDesc*)desc)->damperPos = damperPos; \
		((CRReachingControllerDesc*)desc)->springOri = springOri; \
		((CRReachingControllerDesc*)desc)->damperOri = damperOri; \
		return true; \
	} \

#define ACCESS_DESC_CRGazeControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRGazeControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRAttentionControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRAttentionControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRWalkingControllerDesc(cls) \
protected: \
	double paramLdx ; \
	double paramLdz ; \
	double maxHalfStride ; \
	double height ; \
	double minCycleRate ; \
	double maxDSRate ; \
	double FootLength ; \
	double MaxFootLength ; \
	double MinFootLength ; \
	double MaxRoGround ; \
	double MaxRoLandingSiteFront ; \
	double MaxRoLandingSiteSide ; \
	double MaxRoLandingSiteBack ; \
	double MaxRoConstraint ; \
	double MaxFootSpeedFront ; \
	double MaxFootSpeedSide ; \
	double MaxFootSpeedBack ; \
	double MaxFootAccelerationFront ; \
	double MaxFootAccelerationSide ; \
	double MaxFootAccelerationBack ; \
	double DoubleSupportLimitRate ; \
	double LimitChange ; \
	double pi ; \
	double footsize ; \
	double miu ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		paramLdx = ((CRWalkingControllerDesc*)desc)->paramLdx; \
		paramLdz = ((CRWalkingControllerDesc*)desc)->paramLdz; \
		maxHalfStride = ((CRWalkingControllerDesc*)desc)->maxHalfStride; \
		height = ((CRWalkingControllerDesc*)desc)->height; \
		minCycleRate = ((CRWalkingControllerDesc*)desc)->minCycleRate; \
		maxDSRate = ((CRWalkingControllerDesc*)desc)->maxDSRate; \
		FootLength = ((CRWalkingControllerDesc*)desc)->FootLength; \
		MaxFootLength = ((CRWalkingControllerDesc*)desc)->MaxFootLength; \
		MinFootLength = ((CRWalkingControllerDesc*)desc)->MinFootLength; \
		MaxRoGround = ((CRWalkingControllerDesc*)desc)->MaxRoGround; \
		MaxRoLandingSiteFront = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteFront; \
		MaxRoLandingSiteSide = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteSide; \
		MaxRoLandingSiteBack = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteBack; \
		MaxRoConstraint = ((CRWalkingControllerDesc*)desc)->MaxRoConstraint; \
		MaxFootSpeedFront = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedFront; \
		MaxFootSpeedSide = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedSide; \
		MaxFootSpeedBack = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedBack; \
		MaxFootAccelerationFront = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationFront; \
		MaxFootAccelerationSide = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationSide; \
		MaxFootAccelerationBack = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationBack; \
		DoubleSupportLimitRate = ((CRWalkingControllerDesc*)desc)->DoubleSupportLimitRate; \
		LimitChange = ((CRWalkingControllerDesc*)desc)->LimitChange; \
		pi = ((CRWalkingControllerDesc*)desc)->pi; \
		footsize = ((CRWalkingControllerDesc*)desc)->footsize; \
		miu = ((CRWalkingControllerDesc*)desc)->miu; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRWalkingControllerDesc*)desc)->paramLdx = paramLdx; \
		((CRWalkingControllerDesc*)desc)->paramLdz = paramLdz; \
		((CRWalkingControllerDesc*)desc)->maxHalfStride = maxHalfStride; \
		((CRWalkingControllerDesc*)desc)->height = height; \
		((CRWalkingControllerDesc*)desc)->minCycleRate = minCycleRate; \
		((CRWalkingControllerDesc*)desc)->maxDSRate = maxDSRate; \
		((CRWalkingControllerDesc*)desc)->FootLength = FootLength; \
		((CRWalkingControllerDesc*)desc)->MaxFootLength = MaxFootLength; \
		((CRWalkingControllerDesc*)desc)->MinFootLength = MinFootLength; \
		((CRWalkingControllerDesc*)desc)->MaxRoGround = MaxRoGround; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteFront = MaxRoLandingSiteFront; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteSide = MaxRoLandingSiteSide; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteBack = MaxRoLandingSiteBack; \
		((CRWalkingControllerDesc*)desc)->MaxRoConstraint = MaxRoConstraint; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedFront = MaxFootSpeedFront; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedSide = MaxFootSpeedSide; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedBack = MaxFootSpeedBack; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationFront = MaxFootAccelerationFront; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationSide = MaxFootAccelerationSide; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationBack = MaxFootAccelerationBack; \
		((CRWalkingControllerDesc*)desc)->DoubleSupportLimitRate = DoubleSupportLimitRate; \
		((CRWalkingControllerDesc*)desc)->LimitChange = LimitChange; \
		((CRWalkingControllerDesc*)desc)->pi = pi; \
		((CRWalkingControllerDesc*)desc)->footsize = footsize; \
		((CRWalkingControllerDesc*)desc)->miu = miu; \
		return true; \
	} \

#define ACCESS_DESC_CRWalkingControllerDesc_NOBASE \
protected: \
	double paramLdx ; \
	double paramLdz ; \
	double maxHalfStride ; \
	double height ; \
	double minCycleRate ; \
	double maxDSRate ; \
	double FootLength ; \
	double MaxFootLength ; \
	double MinFootLength ; \
	double MaxRoGround ; \
	double MaxRoLandingSiteFront ; \
	double MaxRoLandingSiteSide ; \
	double MaxRoLandingSiteBack ; \
	double MaxRoConstraint ; \
	double MaxFootSpeedFront ; \
	double MaxFootSpeedSide ; \
	double MaxFootSpeedBack ; \
	double MaxFootAccelerationFront ; \
	double MaxFootAccelerationSide ; \
	double MaxFootAccelerationBack ; \
	double DoubleSupportLimitRate ; \
	double LimitChange ; \
	double pi ; \
	double footsize ; \
	double miu ; \
public: \
	virtual void SetDesc(const void* desc){ \
		paramLdx = ((CRWalkingControllerDesc*)desc)->paramLdx; \
		paramLdz = ((CRWalkingControllerDesc*)desc)->paramLdz; \
		maxHalfStride = ((CRWalkingControllerDesc*)desc)->maxHalfStride; \
		height = ((CRWalkingControllerDesc*)desc)->height; \
		minCycleRate = ((CRWalkingControllerDesc*)desc)->minCycleRate; \
		maxDSRate = ((CRWalkingControllerDesc*)desc)->maxDSRate; \
		FootLength = ((CRWalkingControllerDesc*)desc)->FootLength; \
		MaxFootLength = ((CRWalkingControllerDesc*)desc)->MaxFootLength; \
		MinFootLength = ((CRWalkingControllerDesc*)desc)->MinFootLength; \
		MaxRoGround = ((CRWalkingControllerDesc*)desc)->MaxRoGround; \
		MaxRoLandingSiteFront = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteFront; \
		MaxRoLandingSiteSide = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteSide; \
		MaxRoLandingSiteBack = ((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteBack; \
		MaxRoConstraint = ((CRWalkingControllerDesc*)desc)->MaxRoConstraint; \
		MaxFootSpeedFront = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedFront; \
		MaxFootSpeedSide = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedSide; \
		MaxFootSpeedBack = ((CRWalkingControllerDesc*)desc)->MaxFootSpeedBack; \
		MaxFootAccelerationFront = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationFront; \
		MaxFootAccelerationSide = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationSide; \
		MaxFootAccelerationBack = ((CRWalkingControllerDesc*)desc)->MaxFootAccelerationBack; \
		DoubleSupportLimitRate = ((CRWalkingControllerDesc*)desc)->DoubleSupportLimitRate; \
		LimitChange = ((CRWalkingControllerDesc*)desc)->LimitChange; \
		pi = ((CRWalkingControllerDesc*)desc)->pi; \
		footsize = ((CRWalkingControllerDesc*)desc)->footsize; \
		miu = ((CRWalkingControllerDesc*)desc)->miu; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRWalkingControllerDesc*)desc)->paramLdx = paramLdx; \
		((CRWalkingControllerDesc*)desc)->paramLdz = paramLdz; \
		((CRWalkingControllerDesc*)desc)->maxHalfStride = maxHalfStride; \
		((CRWalkingControllerDesc*)desc)->height = height; \
		((CRWalkingControllerDesc*)desc)->minCycleRate = minCycleRate; \
		((CRWalkingControllerDesc*)desc)->maxDSRate = maxDSRate; \
		((CRWalkingControllerDesc*)desc)->FootLength = FootLength; \
		((CRWalkingControllerDesc*)desc)->MaxFootLength = MaxFootLength; \
		((CRWalkingControllerDesc*)desc)->MinFootLength = MinFootLength; \
		((CRWalkingControllerDesc*)desc)->MaxRoGround = MaxRoGround; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteFront = MaxRoLandingSiteFront; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteSide = MaxRoLandingSiteSide; \
		((CRWalkingControllerDesc*)desc)->MaxRoLandingSiteBack = MaxRoLandingSiteBack; \
		((CRWalkingControllerDesc*)desc)->MaxRoConstraint = MaxRoConstraint; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedFront = MaxFootSpeedFront; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedSide = MaxFootSpeedSide; \
		((CRWalkingControllerDesc*)desc)->MaxFootSpeedBack = MaxFootSpeedBack; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationFront = MaxFootAccelerationFront; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationSide = MaxFootAccelerationSide; \
		((CRWalkingControllerDesc*)desc)->MaxFootAccelerationBack = MaxFootAccelerationBack; \
		((CRWalkingControllerDesc*)desc)->DoubleSupportLimitRate = DoubleSupportLimitRate; \
		((CRWalkingControllerDesc*)desc)->LimitChange = LimitChange; \
		((CRWalkingControllerDesc*)desc)->pi = pi; \
		((CRWalkingControllerDesc*)desc)->footsize = footsize; \
		((CRWalkingControllerDesc*)desc)->miu = miu; \
		return true; \
	} \

#define ACCESS_DESC_CREseWalkingControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CREseWalkingControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRTravelControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRTravelControllerDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRGrabControllerDesc(cls) \
protected: \
	double rateSpringSoften ; \
	double rateDamperSoften ; \
	double rateSpringHarden ; \
	double rateDamperHarden ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		rateSpringSoften = ((CRGrabControllerDesc*)desc)->rateSpringSoften; \
		rateDamperSoften = ((CRGrabControllerDesc*)desc)->rateDamperSoften; \
		rateSpringHarden = ((CRGrabControllerDesc*)desc)->rateSpringHarden; \
		rateDamperHarden = ((CRGrabControllerDesc*)desc)->rateDamperHarden; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRGrabControllerDesc*)desc)->rateSpringSoften = rateSpringSoften; \
		((CRGrabControllerDesc*)desc)->rateDamperSoften = rateDamperSoften; \
		((CRGrabControllerDesc*)desc)->rateSpringHarden = rateSpringHarden; \
		((CRGrabControllerDesc*)desc)->rateDamperHarden = rateDamperHarden; \
		return true; \
	} \

#define ACCESS_DESC_CRGrabControllerDesc_NOBASE \
protected: \
	double rateSpringSoften ; \
	double rateDamperSoften ; \
	double rateSpringHarden ; \
	double rateDamperHarden ; \
public: \
	virtual void SetDesc(const void* desc){ \
		rateSpringSoften = ((CRGrabControllerDesc*)desc)->rateSpringSoften; \
		rateDamperSoften = ((CRGrabControllerDesc*)desc)->rateDamperSoften; \
		rateSpringHarden = ((CRGrabControllerDesc*)desc)->rateSpringHarden; \
		rateDamperHarden = ((CRGrabControllerDesc*)desc)->rateDamperHarden; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRGrabControllerDesc*)desc)->rateSpringSoften = rateSpringSoften; \
		((CRGrabControllerDesc*)desc)->rateDamperSoften = rateDamperSoften; \
		((CRGrabControllerDesc*)desc)->rateSpringHarden = rateSpringHarden; \
		((CRGrabControllerDesc*)desc)->rateDamperHarden = rateDamperHarden; \
		return true; \
	} \

#define ACCESS_DESC_CRCreatureDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRCreatureDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRInternalSceneObjectDesc(cls) \
protected: \
	char *type ; \
	PHSolidIf *solid ; \
	Vec3f position ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		type = ((CRInternalSceneObjectDesc*)desc)->type; \
		solid = ((CRInternalSceneObjectDesc*)desc)->solid; \
		position = ((CRInternalSceneObjectDesc*)desc)->position; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRInternalSceneObjectDesc*)desc)->type = type; \
		((CRInternalSceneObjectDesc*)desc)->solid = solid; \
		((CRInternalSceneObjectDesc*)desc)->position = position; \
		return true; \
	} \

#define ACCESS_DESC_CRInternalSceneObjectDesc_NOBASE \
protected: \
	char *type ; \
	PHSolidIf *solid ; \
	Vec3f position ; \
public: \
	virtual void SetDesc(const void* desc){ \
		type = ((CRInternalSceneObjectDesc*)desc)->type; \
		solid = ((CRInternalSceneObjectDesc*)desc)->solid; \
		position = ((CRInternalSceneObjectDesc*)desc)->position; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRInternalSceneObjectDesc*)desc)->type = type; \
		((CRInternalSceneObjectDesc*)desc)->solid = solid; \
		((CRInternalSceneObjectDesc*)desc)->position = position; \
		return true; \
	} \

#define ACCESS_DESC_CRISAttractiveObjectDesc(cls) \
protected: \
	float bottomupAttr ; \
	float topdownAttr ; \
	float unknownAttr ; \
	float uncertainty ; \
	float uncertaintyIncRate ; \
	float uncertaintyDecRate ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		bottomupAttr = ((CRISAttractiveObjectDesc*)desc)->bottomupAttr; \
		topdownAttr = ((CRISAttractiveObjectDesc*)desc)->topdownAttr; \
		unknownAttr = ((CRISAttractiveObjectDesc*)desc)->unknownAttr; \
		uncertainty = ((CRISAttractiveObjectDesc*)desc)->uncertainty; \
		uncertaintyIncRate = ((CRISAttractiveObjectDesc*)desc)->uncertaintyIncRate; \
		uncertaintyDecRate = ((CRISAttractiveObjectDesc*)desc)->uncertaintyDecRate; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRISAttractiveObjectDesc*)desc)->bottomupAttr = bottomupAttr; \
		((CRISAttractiveObjectDesc*)desc)->topdownAttr = topdownAttr; \
		((CRISAttractiveObjectDesc*)desc)->unknownAttr = unknownAttr; \
		((CRISAttractiveObjectDesc*)desc)->uncertainty = uncertainty; \
		((CRISAttractiveObjectDesc*)desc)->uncertaintyIncRate = uncertaintyIncRate; \
		((CRISAttractiveObjectDesc*)desc)->uncertaintyDecRate = uncertaintyDecRate; \
		return true; \
	} \

#define ACCESS_DESC_CRISAttractiveObjectDesc_NOBASE \
protected: \
	float bottomupAttr ; \
	float topdownAttr ; \
	float unknownAttr ; \
	float uncertainty ; \
	float uncertaintyIncRate ; \
	float uncertaintyDecRate ; \
public: \
	virtual void SetDesc(const void* desc){ \
		bottomupAttr = ((CRISAttractiveObjectDesc*)desc)->bottomupAttr; \
		topdownAttr = ((CRISAttractiveObjectDesc*)desc)->topdownAttr; \
		unknownAttr = ((CRISAttractiveObjectDesc*)desc)->unknownAttr; \
		uncertainty = ((CRISAttractiveObjectDesc*)desc)->uncertainty; \
		uncertaintyIncRate = ((CRISAttractiveObjectDesc*)desc)->uncertaintyIncRate; \
		uncertaintyDecRate = ((CRISAttractiveObjectDesc*)desc)->uncertaintyDecRate; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRISAttractiveObjectDesc*)desc)->bottomupAttr = bottomupAttr; \
		((CRISAttractiveObjectDesc*)desc)->topdownAttr = topdownAttr; \
		((CRISAttractiveObjectDesc*)desc)->unknownAttr = unknownAttr; \
		((CRISAttractiveObjectDesc*)desc)->uncertainty = uncertainty; \
		((CRISAttractiveObjectDesc*)desc)->uncertaintyIncRate = uncertaintyIncRate; \
		((CRISAttractiveObjectDesc*)desc)->uncertaintyDecRate = uncertaintyDecRate; \
		return true; \
	} \

#define ACCESS_DESC_CRISTravelPotentialObjectDesc(cls) \
protected: \
	Vec2f strength ; \
	Vec2f decay ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		strength = ((CRISTravelPotentialObjectDesc*)desc)->strength; \
		decay = ((CRISTravelPotentialObjectDesc*)desc)->decay; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRISTravelPotentialObjectDesc*)desc)->strength = strength; \
		((CRISTravelPotentialObjectDesc*)desc)->decay = decay; \
		return true; \
	} \

#define ACCESS_DESC_CRISTravelPotentialObjectDesc_NOBASE \
protected: \
	Vec2f strength ; \
	Vec2f decay ; \
public: \
	virtual void SetDesc(const void* desc){ \
		strength = ((CRISTravelPotentialObjectDesc*)desc)->strength; \
		decay = ((CRISTravelPotentialObjectDesc*)desc)->decay; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRISTravelPotentialObjectDesc*)desc)->strength = strength; \
		((CRISTravelPotentialObjectDesc*)desc)->decay = decay; \
		return true; \
	} \

#define ACCESS_DESC_CRInternalSceneDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRInternalSceneDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRSensorDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRSensorDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CROpticalSensorDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CROpticalSensorDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

