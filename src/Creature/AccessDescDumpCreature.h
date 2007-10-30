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

#define ACCESS_DESC_CRHingeMammalBodyDesc(cls) \
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
	double frontUpperLegLength ; \
	double frontUpperLegDiameter ; \
	double frontLowerLegLength ; \
	double frontLowerLegDiameter ; \
	double frontFootLength ; \
	double frontFootBreadth ; \
	double frontFootThickness ; \
	double rearUpperLegLength ; \
	double rearUpperLegDiameter ; \
	double rearInterLegDistance ; \
	double rearLowerLegLength ; \
	double rearLowerLegDiameter ; \
	double rearFootLength ; \
	double rearFootBreadth ; \
	double rearFootThickness ; \
	double rearAnkleToeDistance ; \
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
	double springFrontAnkleY ; \
	double damperFrontAnkleY ; \
	double springFrontAnkleX ; \
	double damperFrontAnkleX ; \
	double springFrontAnkleZ ; \
	double damperFrontAnkleZ ; \
	double springWaistLegZ ; \
	double damperWaistLegZ ; \
	double springWaistLegX ; \
	double damperWaistLegX ; \
	double springWaistLegY ; \
	double damperWaistLegY ; \
	double springKnee ; \
	double damperKnee ; \
	double springRearAnkleY ; \
	double damperRearAnkleY ; \
	double springRearAnkleX ; \
	double damperRearAnkleX ; \
	double springRearAnkleZ ; \
	double damperRearAnkleZ ; \
	double springEyeY ; \
	double damperEyeY ; \
	double springEyeX ; \
	double damperEyeX ; \
	Vec3d posRightFrontUpperLeg ; \
	Quaterniond oriRightFrontUpperLeg ; \
	Quaterniond oriRightFrontLowerLeg ; \
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
	Vec2d rangeFrontAnkleY ; \
	Vec2d rangeFrontAnkleX ; \
	Vec2d rangeFrontAnkleZ ; \
	Vec2d rangeWaistLegZ ; \
	Vec2d rangeWaistLegX ; \
	Vec2d rangeWaistLegY ; \
	Vec2d rangeKnee ; \
	Vec2d rangeRearAnkleY ; \
	Vec2d rangeRearAnkleX ; \
	Vec2d rangeRearAnkleZ ; \
	Vec2d rangeRightEyeY ; \
	Vec2d rangeEyeX ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistHeight = ((CRHingeMammalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRHingeMammalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRHingeMammalBodyDesc*)desc)->waistThickness; \
		abdomenHeight = ((CRHingeMammalBodyDesc*)desc)->abdomenHeight; \
		abdomenBreadth = ((CRHingeMammalBodyDesc*)desc)->abdomenBreadth; \
		abdomenThickness = ((CRHingeMammalBodyDesc*)desc)->abdomenThickness; \
		chestHeight = ((CRHingeMammalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRHingeMammalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRHingeMammalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRHingeMammalBodyDesc*)desc)->neckLength; \
		neckDiameter = ((CRHingeMammalBodyDesc*)desc)->neckDiameter; \
		headDiameter = ((CRHingeMammalBodyDesc*)desc)->headDiameter; \
		frontUpperLegLength = ((CRHingeMammalBodyDesc*)desc)->frontUpperLegLength; \
		frontUpperLegDiameter = ((CRHingeMammalBodyDesc*)desc)->frontUpperLegDiameter; \
		frontLowerLegLength = ((CRHingeMammalBodyDesc*)desc)->frontLowerLegLength; \
		frontLowerLegDiameter = ((CRHingeMammalBodyDesc*)desc)->frontLowerLegDiameter; \
		frontFootLength = ((CRHingeMammalBodyDesc*)desc)->frontFootLength; \
		frontFootBreadth = ((CRHingeMammalBodyDesc*)desc)->frontFootBreadth; \
		frontFootThickness = ((CRHingeMammalBodyDesc*)desc)->frontFootThickness; \
		rearUpperLegLength = ((CRHingeMammalBodyDesc*)desc)->rearUpperLegLength; \
		rearUpperLegDiameter = ((CRHingeMammalBodyDesc*)desc)->rearUpperLegDiameter; \
		rearInterLegDistance = ((CRHingeMammalBodyDesc*)desc)->rearInterLegDistance; \
		rearLowerLegLength = ((CRHingeMammalBodyDesc*)desc)->rearLowerLegLength; \
		rearLowerLegDiameter = ((CRHingeMammalBodyDesc*)desc)->rearLowerLegDiameter; \
		rearFootLength = ((CRHingeMammalBodyDesc*)desc)->rearFootLength; \
		rearFootBreadth = ((CRHingeMammalBodyDesc*)desc)->rearFootBreadth; \
		rearFootThickness = ((CRHingeMammalBodyDesc*)desc)->rearFootThickness; \
		rearAnkleToeDistance = ((CRHingeMammalBodyDesc*)desc)->rearAnkleToeDistance; \
		vertexToEyeHeight = ((CRHingeMammalBodyDesc*)desc)->vertexToEyeHeight; \
		occiputToEyeDistance = ((CRHingeMammalBodyDesc*)desc)->occiputToEyeDistance; \
		eyeDiameter = ((CRHingeMammalBodyDesc*)desc)->eyeDiameter; \
		interpupillaryBreadth = ((CRHingeMammalBodyDesc*)desc)->interpupillaryBreadth; \
		springWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->springWaistAbdomen; \
		damperWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->damperWaistAbdomen; \
		springAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->springAbdomenChest; \
		damperAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->damperAbdomenChest; \
		springChestNeckX = ((CRHingeMammalBodyDesc*)desc)->springChestNeckX; \
		damperChestNeckX = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckX; \
		springChestNeckY = ((CRHingeMammalBodyDesc*)desc)->springChestNeckY; \
		damperChestNeckY = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckY; \
		springChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->springChestNeckZ; \
		damperChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckZ; \
		springNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->springNeckHeadX; \
		damperNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->damperNeckHeadX; \
		springNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->springNeckHeadZ; \
		damperNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->damperNeckHeadZ; \
		springShoulderZ = ((CRHingeMammalBodyDesc*)desc)->springShoulderZ; \
		damperShoulderZ = ((CRHingeMammalBodyDesc*)desc)->damperShoulderZ; \
		springShoulderX = ((CRHingeMammalBodyDesc*)desc)->springShoulderX; \
		damperShoulderX = ((CRHingeMammalBodyDesc*)desc)->damperShoulderX; \
		springShoulderY = ((CRHingeMammalBodyDesc*)desc)->springShoulderY; \
		damperShoulderY = ((CRHingeMammalBodyDesc*)desc)->damperShoulderY; \
		springElbow = ((CRHingeMammalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeMammalBodyDesc*)desc)->damperElbow; \
		springFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleY; \
		damperFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleY; \
		springFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleX; \
		damperFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleX; \
		springFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleZ; \
		damperFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleZ; \
		springWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->springWaistLegZ; \
		damperWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegZ; \
		springWaistLegX = ((CRHingeMammalBodyDesc*)desc)->springWaistLegX; \
		damperWaistLegX = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegX; \
		springWaistLegY = ((CRHingeMammalBodyDesc*)desc)->springWaistLegY; \
		damperWaistLegY = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegY; \
		springKnee = ((CRHingeMammalBodyDesc*)desc)->springKnee; \
		damperKnee = ((CRHingeMammalBodyDesc*)desc)->damperKnee; \
		springRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleY; \
		damperRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleY; \
		springRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleX; \
		damperRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleX; \
		springRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleZ; \
		damperRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleZ; \
		springEyeY = ((CRHingeMammalBodyDesc*)desc)->springEyeY; \
		damperEyeY = ((CRHingeMammalBodyDesc*)desc)->damperEyeY; \
		springEyeX = ((CRHingeMammalBodyDesc*)desc)->springEyeX; \
		damperEyeX = ((CRHingeMammalBodyDesc*)desc)->damperEyeX; \
		posRightFrontUpperLeg = ((CRHingeMammalBodyDesc*)desc)->posRightFrontUpperLeg; \
		oriRightFrontUpperLeg = ((CRHingeMammalBodyDesc*)desc)->oriRightFrontUpperLeg; \
		oriRightFrontLowerLeg = ((CRHingeMammalBodyDesc*)desc)->oriRightFrontLowerLeg; \
		oriRightHand = ((CRHingeMammalBodyDesc*)desc)->oriRightHand; \
		rangeWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->rangeWaistAbdomen; \
		rangeAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->rangeAbdomenChest; \
		rangeChestNeckX = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckX; \
		rangeChestNeckY = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckY; \
		rangeChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckZ; \
		rangeNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadX; \
		rangeNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadZ; \
		rangeShoulderZ = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderZ; \
		rangeShoulderX = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderX; \
		rangeShoulderY = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderY; \
		rangeElbow = ((CRHingeMammalBodyDesc*)desc)->rangeElbow; \
		rangeFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleY; \
		rangeFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleX; \
		rangeFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleZ; \
		rangeWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegZ; \
		rangeWaistLegX = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegX; \
		rangeWaistLegY = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegY; \
		rangeKnee = ((CRHingeMammalBodyDesc*)desc)->rangeKnee; \
		rangeRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleY; \
		rangeRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleX; \
		rangeRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleZ; \
		rangeRightEyeY = ((CRHingeMammalBodyDesc*)desc)->rangeRightEyeY; \
		rangeEyeX = ((CRHingeMammalBodyDesc*)desc)->rangeEyeX; \
		noLegs = ((CRHingeMammalBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRHingeMammalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeMammalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeMammalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeMammalBodyDesc*)desc)->abdomenHeight = abdomenHeight; \
		((CRHingeMammalBodyDesc*)desc)->abdomenBreadth = abdomenBreadth; \
		((CRHingeMammalBodyDesc*)desc)->abdomenThickness = abdomenThickness; \
		((CRHingeMammalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeMammalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeMammalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeMammalBodyDesc*)desc)->neckLength = neckLength; \
		((CRHingeMammalBodyDesc*)desc)->neckDiameter = neckDiameter; \
		((CRHingeMammalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontUpperLegLength = frontUpperLegLength; \
		((CRHingeMammalBodyDesc*)desc)->frontUpperLegDiameter = frontUpperLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontLowerLegLength = frontLowerLegLength; \
		((CRHingeMammalBodyDesc*)desc)->frontLowerLegDiameter = frontLowerLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontFootLength = frontFootLength; \
		((CRHingeMammalBodyDesc*)desc)->frontFootBreadth = frontFootBreadth; \
		((CRHingeMammalBodyDesc*)desc)->frontFootThickness = frontFootThickness; \
		((CRHingeMammalBodyDesc*)desc)->rearUpperLegLength = rearUpperLegLength; \
		((CRHingeMammalBodyDesc*)desc)->rearUpperLegDiameter = rearUpperLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->rearInterLegDistance = rearInterLegDistance; \
		((CRHingeMammalBodyDesc*)desc)->rearLowerLegLength = rearLowerLegLength; \
		((CRHingeMammalBodyDesc*)desc)->rearLowerLegDiameter = rearLowerLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->rearFootLength = rearFootLength; \
		((CRHingeMammalBodyDesc*)desc)->rearFootBreadth = rearFootBreadth; \
		((CRHingeMammalBodyDesc*)desc)->rearFootThickness = rearFootThickness; \
		((CRHingeMammalBodyDesc*)desc)->rearAnkleToeDistance = rearAnkleToeDistance; \
		((CRHingeMammalBodyDesc*)desc)->vertexToEyeHeight = vertexToEyeHeight; \
		((CRHingeMammalBodyDesc*)desc)->occiputToEyeDistance = occiputToEyeDistance; \
		((CRHingeMammalBodyDesc*)desc)->eyeDiameter = eyeDiameter; \
		((CRHingeMammalBodyDesc*)desc)->interpupillaryBreadth = interpupillaryBreadth; \
		((CRHingeMammalBodyDesc*)desc)->springWaistAbdomen = springWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistAbdomen = damperWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->springAbdomenChest = springAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->damperAbdomenChest = damperAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckX = springChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckX = damperChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckY = springChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckY = damperChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckZ = springChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckZ = damperChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->springNeckHeadX = springNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->damperNeckHeadX = damperNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->springNeckHeadZ = springNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->damperNeckHeadZ = damperNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderZ = springShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderZ = damperShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderX = springShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderX = damperShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderY = springShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderY = damperShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeMammalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleY = springFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleY = damperFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleX = springFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleX = damperFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleZ = springFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleZ = damperFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegZ = springWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegZ = damperWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegX = springWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegX = damperWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegY = springWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegY = damperWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->springKnee = springKnee; \
		((CRHingeMammalBodyDesc*)desc)->damperKnee = damperKnee; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleY = springRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleY = damperRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleX = springRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleX = damperRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleZ = springRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleZ = damperRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->springEyeY = springEyeY; \
		((CRHingeMammalBodyDesc*)desc)->damperEyeY = damperEyeY; \
		((CRHingeMammalBodyDesc*)desc)->springEyeX = springEyeX; \
		((CRHingeMammalBodyDesc*)desc)->damperEyeX = damperEyeX; \
		((CRHingeMammalBodyDesc*)desc)->posRightFrontUpperLeg = posRightFrontUpperLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightFrontUpperLeg = oriRightFrontUpperLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightFrontLowerLeg = oriRightFrontLowerLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightHand = oriRightHand; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistAbdomen = rangeWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->rangeAbdomenChest = rangeAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckX = rangeChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckY = rangeChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckZ = rangeChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadX = rangeNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadZ = rangeNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderZ = rangeShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderX = rangeShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderY = rangeShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleY = rangeFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleX = rangeFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleZ = rangeFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegZ = rangeWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegX = rangeWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegY = rangeWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->rangeKnee = rangeKnee; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleY = rangeRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleX = rangeRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleZ = rangeRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeRightEyeY = rangeRightEyeY; \
		((CRHingeMammalBodyDesc*)desc)->rangeEyeX = rangeEyeX; \
		((CRHingeMammalBodyDesc*)desc)->noLegs = noLegs; \
		return true; \
	} \

#define ACCESS_DESC_CRHingeMammalBodyDesc_NOBASE \
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
	double frontUpperLegLength ; \
	double frontUpperLegDiameter ; \
	double frontLowerLegLength ; \
	double frontLowerLegDiameter ; \
	double frontFootLength ; \
	double frontFootBreadth ; \
	double frontFootThickness ; \
	double rearUpperLegLength ; \
	double rearUpperLegDiameter ; \
	double rearInterLegDistance ; \
	double rearLowerLegLength ; \
	double rearLowerLegDiameter ; \
	double rearFootLength ; \
	double rearFootBreadth ; \
	double rearFootThickness ; \
	double rearAnkleToeDistance ; \
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
	double springFrontAnkleY ; \
	double damperFrontAnkleY ; \
	double springFrontAnkleX ; \
	double damperFrontAnkleX ; \
	double springFrontAnkleZ ; \
	double damperFrontAnkleZ ; \
	double springWaistLegZ ; \
	double damperWaistLegZ ; \
	double springWaistLegX ; \
	double damperWaistLegX ; \
	double springWaistLegY ; \
	double damperWaistLegY ; \
	double springKnee ; \
	double damperKnee ; \
	double springRearAnkleY ; \
	double damperRearAnkleY ; \
	double springRearAnkleX ; \
	double damperRearAnkleX ; \
	double springRearAnkleZ ; \
	double damperRearAnkleZ ; \
	double springEyeY ; \
	double damperEyeY ; \
	double springEyeX ; \
	double damperEyeX ; \
	Vec3d posRightFrontUpperLeg ; \
	Quaterniond oriRightFrontUpperLeg ; \
	Quaterniond oriRightFrontLowerLeg ; \
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
	Vec2d rangeFrontAnkleY ; \
	Vec2d rangeFrontAnkleX ; \
	Vec2d rangeFrontAnkleZ ; \
	Vec2d rangeWaistLegZ ; \
	Vec2d rangeWaistLegX ; \
	Vec2d rangeWaistLegY ; \
	Vec2d rangeKnee ; \
	Vec2d rangeRearAnkleY ; \
	Vec2d rangeRearAnkleX ; \
	Vec2d rangeRearAnkleZ ; \
	Vec2d rangeRightEyeY ; \
	Vec2d rangeEyeX ; \
	bool noLegs ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistHeight = ((CRHingeMammalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRHingeMammalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRHingeMammalBodyDesc*)desc)->waistThickness; \
		abdomenHeight = ((CRHingeMammalBodyDesc*)desc)->abdomenHeight; \
		abdomenBreadth = ((CRHingeMammalBodyDesc*)desc)->abdomenBreadth; \
		abdomenThickness = ((CRHingeMammalBodyDesc*)desc)->abdomenThickness; \
		chestHeight = ((CRHingeMammalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRHingeMammalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRHingeMammalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRHingeMammalBodyDesc*)desc)->neckLength; \
		neckDiameter = ((CRHingeMammalBodyDesc*)desc)->neckDiameter; \
		headDiameter = ((CRHingeMammalBodyDesc*)desc)->headDiameter; \
		frontUpperLegLength = ((CRHingeMammalBodyDesc*)desc)->frontUpperLegLength; \
		frontUpperLegDiameter = ((CRHingeMammalBodyDesc*)desc)->frontUpperLegDiameter; \
		frontLowerLegLength = ((CRHingeMammalBodyDesc*)desc)->frontLowerLegLength; \
		frontLowerLegDiameter = ((CRHingeMammalBodyDesc*)desc)->frontLowerLegDiameter; \
		frontFootLength = ((CRHingeMammalBodyDesc*)desc)->frontFootLength; \
		frontFootBreadth = ((CRHingeMammalBodyDesc*)desc)->frontFootBreadth; \
		frontFootThickness = ((CRHingeMammalBodyDesc*)desc)->frontFootThickness; \
		rearUpperLegLength = ((CRHingeMammalBodyDesc*)desc)->rearUpperLegLength; \
		rearUpperLegDiameter = ((CRHingeMammalBodyDesc*)desc)->rearUpperLegDiameter; \
		rearInterLegDistance = ((CRHingeMammalBodyDesc*)desc)->rearInterLegDistance; \
		rearLowerLegLength = ((CRHingeMammalBodyDesc*)desc)->rearLowerLegLength; \
		rearLowerLegDiameter = ((CRHingeMammalBodyDesc*)desc)->rearLowerLegDiameter; \
		rearFootLength = ((CRHingeMammalBodyDesc*)desc)->rearFootLength; \
		rearFootBreadth = ((CRHingeMammalBodyDesc*)desc)->rearFootBreadth; \
		rearFootThickness = ((CRHingeMammalBodyDesc*)desc)->rearFootThickness; \
		rearAnkleToeDistance = ((CRHingeMammalBodyDesc*)desc)->rearAnkleToeDistance; \
		vertexToEyeHeight = ((CRHingeMammalBodyDesc*)desc)->vertexToEyeHeight; \
		occiputToEyeDistance = ((CRHingeMammalBodyDesc*)desc)->occiputToEyeDistance; \
		eyeDiameter = ((CRHingeMammalBodyDesc*)desc)->eyeDiameter; \
		interpupillaryBreadth = ((CRHingeMammalBodyDesc*)desc)->interpupillaryBreadth; \
		springWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->springWaistAbdomen; \
		damperWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->damperWaistAbdomen; \
		springAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->springAbdomenChest; \
		damperAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->damperAbdomenChest; \
		springChestNeckX = ((CRHingeMammalBodyDesc*)desc)->springChestNeckX; \
		damperChestNeckX = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckX; \
		springChestNeckY = ((CRHingeMammalBodyDesc*)desc)->springChestNeckY; \
		damperChestNeckY = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckY; \
		springChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->springChestNeckZ; \
		damperChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->damperChestNeckZ; \
		springNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->springNeckHeadX; \
		damperNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->damperNeckHeadX; \
		springNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->springNeckHeadZ; \
		damperNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->damperNeckHeadZ; \
		springShoulderZ = ((CRHingeMammalBodyDesc*)desc)->springShoulderZ; \
		damperShoulderZ = ((CRHingeMammalBodyDesc*)desc)->damperShoulderZ; \
		springShoulderX = ((CRHingeMammalBodyDesc*)desc)->springShoulderX; \
		damperShoulderX = ((CRHingeMammalBodyDesc*)desc)->damperShoulderX; \
		springShoulderY = ((CRHingeMammalBodyDesc*)desc)->springShoulderY; \
		damperShoulderY = ((CRHingeMammalBodyDesc*)desc)->damperShoulderY; \
		springElbow = ((CRHingeMammalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRHingeMammalBodyDesc*)desc)->damperElbow; \
		springFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleY; \
		damperFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleY; \
		springFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleX; \
		damperFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleX; \
		springFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->springFrontAnkleZ; \
		damperFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleZ; \
		springWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->springWaistLegZ; \
		damperWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegZ; \
		springWaistLegX = ((CRHingeMammalBodyDesc*)desc)->springWaistLegX; \
		damperWaistLegX = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegX; \
		springWaistLegY = ((CRHingeMammalBodyDesc*)desc)->springWaistLegY; \
		damperWaistLegY = ((CRHingeMammalBodyDesc*)desc)->damperWaistLegY; \
		springKnee = ((CRHingeMammalBodyDesc*)desc)->springKnee; \
		damperKnee = ((CRHingeMammalBodyDesc*)desc)->damperKnee; \
		springRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleY; \
		damperRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleY; \
		springRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleX; \
		damperRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleX; \
		springRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->springRearAnkleZ; \
		damperRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->damperRearAnkleZ; \
		springEyeY = ((CRHingeMammalBodyDesc*)desc)->springEyeY; \
		damperEyeY = ((CRHingeMammalBodyDesc*)desc)->damperEyeY; \
		springEyeX = ((CRHingeMammalBodyDesc*)desc)->springEyeX; \
		damperEyeX = ((CRHingeMammalBodyDesc*)desc)->damperEyeX; \
		posRightFrontUpperLeg = ((CRHingeMammalBodyDesc*)desc)->posRightFrontUpperLeg; \
		oriRightFrontUpperLeg = ((CRHingeMammalBodyDesc*)desc)->oriRightFrontUpperLeg; \
		oriRightFrontLowerLeg = ((CRHingeMammalBodyDesc*)desc)->oriRightFrontLowerLeg; \
		oriRightHand = ((CRHingeMammalBodyDesc*)desc)->oriRightHand; \
		rangeWaistAbdomen = ((CRHingeMammalBodyDesc*)desc)->rangeWaistAbdomen; \
		rangeAbdomenChest = ((CRHingeMammalBodyDesc*)desc)->rangeAbdomenChest; \
		rangeChestNeckX = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckX; \
		rangeChestNeckY = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckY; \
		rangeChestNeckZ = ((CRHingeMammalBodyDesc*)desc)->rangeChestNeckZ; \
		rangeNeckHeadX = ((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadX; \
		rangeNeckHeadZ = ((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadZ; \
		rangeShoulderZ = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderZ; \
		rangeShoulderX = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderX; \
		rangeShoulderY = ((CRHingeMammalBodyDesc*)desc)->rangeShoulderY; \
		rangeElbow = ((CRHingeMammalBodyDesc*)desc)->rangeElbow; \
		rangeFrontAnkleY = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleY; \
		rangeFrontAnkleX = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleX; \
		rangeFrontAnkleZ = ((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleZ; \
		rangeWaistLegZ = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegZ; \
		rangeWaistLegX = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegX; \
		rangeWaistLegY = ((CRHingeMammalBodyDesc*)desc)->rangeWaistLegY; \
		rangeKnee = ((CRHingeMammalBodyDesc*)desc)->rangeKnee; \
		rangeRearAnkleY = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleY; \
		rangeRearAnkleX = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleX; \
		rangeRearAnkleZ = ((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleZ; \
		rangeRightEyeY = ((CRHingeMammalBodyDesc*)desc)->rangeRightEyeY; \
		rangeEyeX = ((CRHingeMammalBodyDesc*)desc)->rangeEyeX; \
		noLegs = ((CRHingeMammalBodyDesc*)desc)->noLegs; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRHingeMammalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRHingeMammalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRHingeMammalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRHingeMammalBodyDesc*)desc)->abdomenHeight = abdomenHeight; \
		((CRHingeMammalBodyDesc*)desc)->abdomenBreadth = abdomenBreadth; \
		((CRHingeMammalBodyDesc*)desc)->abdomenThickness = abdomenThickness; \
		((CRHingeMammalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRHingeMammalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRHingeMammalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRHingeMammalBodyDesc*)desc)->neckLength = neckLength; \
		((CRHingeMammalBodyDesc*)desc)->neckDiameter = neckDiameter; \
		((CRHingeMammalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontUpperLegLength = frontUpperLegLength; \
		((CRHingeMammalBodyDesc*)desc)->frontUpperLegDiameter = frontUpperLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontLowerLegLength = frontLowerLegLength; \
		((CRHingeMammalBodyDesc*)desc)->frontLowerLegDiameter = frontLowerLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->frontFootLength = frontFootLength; \
		((CRHingeMammalBodyDesc*)desc)->frontFootBreadth = frontFootBreadth; \
		((CRHingeMammalBodyDesc*)desc)->frontFootThickness = frontFootThickness; \
		((CRHingeMammalBodyDesc*)desc)->rearUpperLegLength = rearUpperLegLength; \
		((CRHingeMammalBodyDesc*)desc)->rearUpperLegDiameter = rearUpperLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->rearInterLegDistance = rearInterLegDistance; \
		((CRHingeMammalBodyDesc*)desc)->rearLowerLegLength = rearLowerLegLength; \
		((CRHingeMammalBodyDesc*)desc)->rearLowerLegDiameter = rearLowerLegDiameter; \
		((CRHingeMammalBodyDesc*)desc)->rearFootLength = rearFootLength; \
		((CRHingeMammalBodyDesc*)desc)->rearFootBreadth = rearFootBreadth; \
		((CRHingeMammalBodyDesc*)desc)->rearFootThickness = rearFootThickness; \
		((CRHingeMammalBodyDesc*)desc)->rearAnkleToeDistance = rearAnkleToeDistance; \
		((CRHingeMammalBodyDesc*)desc)->vertexToEyeHeight = vertexToEyeHeight; \
		((CRHingeMammalBodyDesc*)desc)->occiputToEyeDistance = occiputToEyeDistance; \
		((CRHingeMammalBodyDesc*)desc)->eyeDiameter = eyeDiameter; \
		((CRHingeMammalBodyDesc*)desc)->interpupillaryBreadth = interpupillaryBreadth; \
		((CRHingeMammalBodyDesc*)desc)->springWaistAbdomen = springWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistAbdomen = damperWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->springAbdomenChest = springAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->damperAbdomenChest = damperAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckX = springChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckX = damperChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckY = springChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckY = damperChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->springChestNeckZ = springChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->damperChestNeckZ = damperChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->springNeckHeadX = springNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->damperNeckHeadX = damperNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->springNeckHeadZ = springNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->damperNeckHeadZ = damperNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderZ = springShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderZ = damperShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderX = springShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderX = damperShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->springShoulderY = springShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->damperShoulderY = damperShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->springElbow = springElbow; \
		((CRHingeMammalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleY = springFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleY = damperFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleX = springFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleX = damperFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->springFrontAnkleZ = springFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->damperFrontAnkleZ = damperFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegZ = springWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegZ = damperWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegX = springWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegX = damperWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->springWaistLegY = springWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->damperWaistLegY = damperWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->springKnee = springKnee; \
		((CRHingeMammalBodyDesc*)desc)->damperKnee = damperKnee; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleY = springRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleY = damperRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleX = springRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleX = damperRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->springRearAnkleZ = springRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->damperRearAnkleZ = damperRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->springEyeY = springEyeY; \
		((CRHingeMammalBodyDesc*)desc)->damperEyeY = damperEyeY; \
		((CRHingeMammalBodyDesc*)desc)->springEyeX = springEyeX; \
		((CRHingeMammalBodyDesc*)desc)->damperEyeX = damperEyeX; \
		((CRHingeMammalBodyDesc*)desc)->posRightFrontUpperLeg = posRightFrontUpperLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightFrontUpperLeg = oriRightFrontUpperLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightFrontLowerLeg = oriRightFrontLowerLeg; \
		((CRHingeMammalBodyDesc*)desc)->oriRightHand = oriRightHand; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistAbdomen = rangeWaistAbdomen; \
		((CRHingeMammalBodyDesc*)desc)->rangeAbdomenChest = rangeAbdomenChest; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckX = rangeChestNeckX; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckY = rangeChestNeckY; \
		((CRHingeMammalBodyDesc*)desc)->rangeChestNeckZ = rangeChestNeckZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadX = rangeNeckHeadX; \
		((CRHingeMammalBodyDesc*)desc)->rangeNeckHeadZ = rangeNeckHeadZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderZ = rangeShoulderZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderX = rangeShoulderX; \
		((CRHingeMammalBodyDesc*)desc)->rangeShoulderY = rangeShoulderY; \
		((CRHingeMammalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleY = rangeFrontAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleX = rangeFrontAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->rangeFrontAnkleZ = rangeFrontAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegZ = rangeWaistLegZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegX = rangeWaistLegX; \
		((CRHingeMammalBodyDesc*)desc)->rangeWaistLegY = rangeWaistLegY; \
		((CRHingeMammalBodyDesc*)desc)->rangeKnee = rangeKnee; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleY = rangeRearAnkleY; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleX = rangeRearAnkleX; \
		((CRHingeMammalBodyDesc*)desc)->rangeRearAnkleZ = rangeRearAnkleZ; \
		((CRHingeMammalBodyDesc*)desc)->rangeRightEyeY = rangeRightEyeY; \
		((CRHingeMammalBodyDesc*)desc)->rangeEyeX = rangeEyeX; \
		((CRHingeMammalBodyDesc*)desc)->noLegs = noLegs; \
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

#define ACCESS_DESC_CRTrunkFootMammalBodyDesc(cls) \
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
		waistHeight = ((CRTrunkFootMammalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootMammalBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootMammalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootMammalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootMammalBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootMammalBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootMammalBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootMammalBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRTrunkFootMammalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootMammalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootMammalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootMammalBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootMammalBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootMammalBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootMammalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
		return true; \
	} \

#define ACCESS_DESC_CRTrunkFootMammalBodyDesc_NOBASE \
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
		waistHeight = ((CRTrunkFootMammalBodyDesc*)desc)->waistHeight; \
		waistBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->waistBreadth; \
		waistThickness = ((CRTrunkFootMammalBodyDesc*)desc)->waistThickness; \
		chestHeight = ((CRTrunkFootMammalBodyDesc*)desc)->chestHeight; \
		chestBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->chestBreadth; \
		chestThickness = ((CRTrunkFootMammalBodyDesc*)desc)->chestThickness; \
		neckLength = ((CRTrunkFootMammalBodyDesc*)desc)->neckLength; \
		headDiameter = ((CRTrunkFootMammalBodyDesc*)desc)->headDiameter; \
		footLength = ((CRTrunkFootMammalBodyDesc*)desc)->footLength; \
		footBreadth = ((CRTrunkFootMammalBodyDesc*)desc)->footBreadth; \
		footThickness = ((CRTrunkFootMammalBodyDesc*)desc)->footThickness; \
		springWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->damperWaistChest; \
		springChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->springChestHead; \
		damperChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->damperChestHead; \
		rangeWaistChest = ((CRTrunkFootMammalBodyDesc*)desc)->rangeWaistChest; \
		rangeChestHead = ((CRTrunkFootMammalBodyDesc*)desc)->rangeChestHead; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRTrunkFootMammalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRTrunkFootMammalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->neckLength = neckLength; \
		((CRTrunkFootMammalBodyDesc*)desc)->headDiameter = headDiameter; \
		((CRTrunkFootMammalBodyDesc*)desc)->footLength = footLength; \
		((CRTrunkFootMammalBodyDesc*)desc)->footBreadth = footBreadth; \
		((CRTrunkFootMammalBodyDesc*)desc)->footThickness = footThickness; \
		((CRTrunkFootMammalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->springChestHead = springChestHead; \
		((CRTrunkFootMammalBodyDesc*)desc)->damperChestHead = damperChestHead; \
		((CRTrunkFootMammalBodyDesc*)desc)->rangeWaistChest = rangeWaistChest; \
		((CRTrunkFootMammalBodyDesc*)desc)->rangeChestHead = rangeChestHead; \
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
	int solidNo ; \
	Vec3f reachPos ; \
	float limitForce ; \
	float springPos ; \
	float damperPos ; \
	float springOri ; \
	float damperOri ; \
	float softenRate ; \
	float hardenRate ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		solid = ((CRReachingControllerDesc*)desc)->solid; \
		solidNo = ((CRReachingControllerDesc*)desc)->solidNo; \
		reachPos = ((CRReachingControllerDesc*)desc)->reachPos; \
		limitForce = ((CRReachingControllerDesc*)desc)->limitForce; \
		springPos = ((CRReachingControllerDesc*)desc)->springPos; \
		damperPos = ((CRReachingControllerDesc*)desc)->damperPos; \
		springOri = ((CRReachingControllerDesc*)desc)->springOri; \
		damperOri = ((CRReachingControllerDesc*)desc)->damperOri; \
		softenRate = ((CRReachingControllerDesc*)desc)->softenRate; \
		hardenRate = ((CRReachingControllerDesc*)desc)->hardenRate; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRReachingControllerDesc*)desc)->solid = solid; \
		((CRReachingControllerDesc*)desc)->solidNo = solidNo; \
		((CRReachingControllerDesc*)desc)->reachPos = reachPos; \
		((CRReachingControllerDesc*)desc)->limitForce = limitForce; \
		((CRReachingControllerDesc*)desc)->springPos = springPos; \
		((CRReachingControllerDesc*)desc)->damperPos = damperPos; \
		((CRReachingControllerDesc*)desc)->springOri = springOri; \
		((CRReachingControllerDesc*)desc)->damperOri = damperOri; \
		((CRReachingControllerDesc*)desc)->softenRate = softenRate; \
		((CRReachingControllerDesc*)desc)->hardenRate = hardenRate; \
		return true; \
	} \

#define ACCESS_DESC_CRReachingControllerDesc_NOBASE \
protected: \
	PHSolidIf *solid ; \
	int solidNo ; \
	Vec3f reachPos ; \
	float limitForce ; \
	float springPos ; \
	float damperPos ; \
	float springOri ; \
	float damperOri ; \
	float softenRate ; \
	float hardenRate ; \
public: \
	virtual void SetDesc(const void* desc){ \
		solid = ((CRReachingControllerDesc*)desc)->solid; \
		solidNo = ((CRReachingControllerDesc*)desc)->solidNo; \
		reachPos = ((CRReachingControllerDesc*)desc)->reachPos; \
		limitForce = ((CRReachingControllerDesc*)desc)->limitForce; \
		springPos = ((CRReachingControllerDesc*)desc)->springPos; \
		damperPos = ((CRReachingControllerDesc*)desc)->damperPos; \
		springOri = ((CRReachingControllerDesc*)desc)->springOri; \
		damperOri = ((CRReachingControllerDesc*)desc)->damperOri; \
		softenRate = ((CRReachingControllerDesc*)desc)->softenRate; \
		hardenRate = ((CRReachingControllerDesc*)desc)->hardenRate; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRReachingControllerDesc*)desc)->solid = solid; \
		((CRReachingControllerDesc*)desc)->solidNo = solidNo; \
		((CRReachingControllerDesc*)desc)->reachPos = reachPos; \
		((CRReachingControllerDesc*)desc)->limitForce = limitForce; \
		((CRReachingControllerDesc*)desc)->springPos = springPos; \
		((CRReachingControllerDesc*)desc)->damperPos = damperPos; \
		((CRReachingControllerDesc*)desc)->springOri = springOri; \
		((CRReachingControllerDesc*)desc)->damperOri = damperOri; \
		((CRReachingControllerDesc*)desc)->softenRate = softenRate; \
		((CRReachingControllerDesc*)desc)->hardenRate = hardenRate; \
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

