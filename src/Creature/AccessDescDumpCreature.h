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
	double bodyMass ; \
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
	double spring ; \
	double damper ; \
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
		bodyMass = ((CRHingeHumanBodyDesc*)desc)->bodyMass; \
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
		spring = ((CRHingeHumanBodyDesc*)desc)->spring; \
		damper = ((CRHingeHumanBodyDesc*)desc)->damper; \
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
		((CRHingeHumanBodyDesc*)desc)->bodyMass = bodyMass; \
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
		((CRHingeHumanBodyDesc*)desc)->spring = spring; \
		((CRHingeHumanBodyDesc*)desc)->damper = damper; \
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
	double bodyMass ; \
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
	double spring ; \
	double damper ; \
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
		bodyMass = ((CRHingeHumanBodyDesc*)desc)->bodyMass; \
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
		spring = ((CRHingeHumanBodyDesc*)desc)->spring; \
		damper = ((CRHingeHumanBodyDesc*)desc)->damper; \
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
		((CRHingeHumanBodyDesc*)desc)->bodyMass = bodyMass; \
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
		((CRHingeHumanBodyDesc*)desc)->spring = spring; \
		((CRHingeHumanBodyDesc*)desc)->damper = damper; \
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

#define ACCESS_DESC_CRFourLegsAnimalBodyDesc(cls) \
protected: \
	double waistBreadth ; \
	double waistHeight ; \
	double waistThickness ; \
	double chestBreadth ; \
	double chestHeight ; \
	double chestThickness ; \
	double tailBreadth ; \
	double tailHeight ; \
	double tailThickness ; \
	double neckBreadth ; \
	double neckHeight ; \
	double neckThickness ; \
	double headBreadth ; \
	double headHeight ; \
	double headThickness ; \
	double breastboneBreadth ; \
	double breastboneHeight ; \
	double breastboneThickness ; \
	double radiusBreadth ; \
	double radiusHeight ; \
	double radiusThickness ; \
	double frontCannonBoneBreadth ; \
	double frontCannonBoneHeight ; \
	double frontCannonBoneThickness ; \
	double frontToeBreadth ; \
	double frontToeHeight ; \
	double frontToeThickness ; \
	double femurBreadth ; \
	double femurHeight ; \
	double femurThickness ; \
	double tibiaBreadth ; \
	double tibiaHeight ; \
	double tibiaThickness ; \
	double rearCannonBoneBreadth ; \
	double rearCannonBoneHeight ; \
	double rearCannonBoneThickness ; \
	double rearToeBreadth ; \
	double rearToeHeight ; \
	double rearToeThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springWaistTail ; \
	double damperWaistTail ; \
	double springTail ; \
	double damperTail ; \
	double springChestNeck ; \
	double damperChestNeck ; \
	double springNeckHead ; \
	double damperNeckHead ; \
	double springShoulder ; \
	double damperShoulder ; \
	double springFrontAnkle ; \
	double damperFrontAnkle ; \
	double springHip ; \
	double damperHip ; \
	double springRearAnkle ; \
	double damperRearAnkle ; \
	double springElbow ; \
	double damperElbow ; \
	double springFrontKnee ; \
	double damperFrontKnee ; \
	double springStifle ; \
	double damperStifle ; \
	double springRearKnee ; \
	double damperRearKnee ; \
	Vec2d rangeElbow ; \
	Vec2d rangeFrontKnee ; \
	Vec2d rangeStifle ; \
	Vec2d rangeRearKnee ; \
	Quaterniond goalWaistChest ; \
	Quaterniond goalWaistTail ; \
	Quaterniond goalTail ; \
	Quaterniond goalChestNeck ; \
	Quaterniond goalNeckHead ; \
	Quaterniond goalShoulder ; \
	Quaterniond goalFrontAnkle ; \
	Quaterniond goalHip ; \
	Quaterniond goalRearAnkle ; \
	double originElbow ; \
	double originFrontKnee ; \
	double originStifle ; \
	double originRearKnee ; \
	Vec3d limitDirWaistChest ; \
	Vec3d limitDirWaistTail ; \
	Vec3d limitDirTail ; \
	Vec3d limitDirChestNeck ; \
	Vec3d limitDirNeckHead ; \
	Vec3d limitDirShoulder ; \
	Vec3d limitDirFrontAnkle ; \
	Vec3d limitDirHip ; \
	Vec3d limitDirRearAnkle ; \
	Vec2d limitSwingWaistChest ; \
	Vec2d limitSwingWaistTail ; \
	Vec2d limitSwingTail ; \
	Vec2d limitSwingChestNeck ; \
	Vec2d limitSwingNeckHead ; \
	Vec2d limitSwingShoulder ; \
	Vec2d limitSwingFrontAnkle ; \
	Vec2d limitSwingHip ; \
	Vec2d limitSwingRearAnkle ; \
	Vec2d limitTwistWaistChest ; \
	Vec2d limitTwistWaistTail ; \
	Vec2d limitTwistTail ; \
	Vec2d limitTwistChestNeck ; \
	Vec2d limitTwistNeckHead ; \
	Vec2d limitTwistShoulder ; \
	Vec2d limitTwistFrontAnkle ; \
	Vec2d limitTwistHip ; \
	Vec2d limitTwistRearAnkle ; \
	bool noLegs ; \
	bool dynamicalMode ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		waistBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->waistBreadth; \
		waistHeight = ((CRFourLegsAnimalBodyDesc*)desc)->waistHeight; \
		waistThickness = ((CRFourLegsAnimalBodyDesc*)desc)->waistThickness; \
		chestBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->chestBreadth; \
		chestHeight = ((CRFourLegsAnimalBodyDesc*)desc)->chestHeight; \
		chestThickness = ((CRFourLegsAnimalBodyDesc*)desc)->chestThickness; \
		tailBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->tailBreadth; \
		tailHeight = ((CRFourLegsAnimalBodyDesc*)desc)->tailHeight; \
		tailThickness = ((CRFourLegsAnimalBodyDesc*)desc)->tailThickness; \
		neckBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->neckBreadth; \
		neckHeight = ((CRFourLegsAnimalBodyDesc*)desc)->neckHeight; \
		neckThickness = ((CRFourLegsAnimalBodyDesc*)desc)->neckThickness; \
		headBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->headBreadth; \
		headHeight = ((CRFourLegsAnimalBodyDesc*)desc)->headHeight; \
		headThickness = ((CRFourLegsAnimalBodyDesc*)desc)->headThickness; \
		breastboneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneBreadth; \
		breastboneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneHeight; \
		breastboneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneThickness; \
		radiusBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->radiusBreadth; \
		radiusHeight = ((CRFourLegsAnimalBodyDesc*)desc)->radiusHeight; \
		radiusThickness = ((CRFourLegsAnimalBodyDesc*)desc)->radiusThickness; \
		frontCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneBreadth; \
		frontCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneHeight; \
		frontCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneThickness; \
		frontToeBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeBreadth; \
		frontToeHeight = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeHeight; \
		frontToeThickness = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeThickness; \
		femurBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->femurBreadth; \
		femurHeight = ((CRFourLegsAnimalBodyDesc*)desc)->femurHeight; \
		femurThickness = ((CRFourLegsAnimalBodyDesc*)desc)->femurThickness; \
		tibiaBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaBreadth; \
		tibiaHeight = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaHeight; \
		tibiaThickness = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaThickness; \
		rearCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneBreadth; \
		rearCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneHeight; \
		rearCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneThickness; \
		rearToeBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeBreadth; \
		rearToeHeight = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeHeight; \
		rearToeThickness = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeThickness; \
		springWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->damperWaistChest; \
		springWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->springWaistTail; \
		damperWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->damperWaistTail; \
		springTail = ((CRFourLegsAnimalBodyDesc*)desc)->springTail; \
		damperTail = ((CRFourLegsAnimalBodyDesc*)desc)->damperTail; \
		springChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->springChestNeck; \
		damperChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->damperChestNeck; \
		springNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->springNeckHead; \
		damperNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->damperNeckHead; \
		springShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->springShoulder; \
		damperShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->damperShoulder; \
		springFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->springFrontAnkle; \
		damperFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->damperFrontAnkle; \
		springHip = ((CRFourLegsAnimalBodyDesc*)desc)->springHip; \
		damperHip = ((CRFourLegsAnimalBodyDesc*)desc)->damperHip; \
		springRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->springRearAnkle; \
		damperRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->damperRearAnkle; \
		springElbow = ((CRFourLegsAnimalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRFourLegsAnimalBodyDesc*)desc)->damperElbow; \
		springFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->springFrontKnee; \
		damperFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->damperFrontKnee; \
		springStifle = ((CRFourLegsAnimalBodyDesc*)desc)->springStifle; \
		damperStifle = ((CRFourLegsAnimalBodyDesc*)desc)->damperStifle; \
		springRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->springRearKnee; \
		damperRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->damperRearKnee; \
		rangeElbow = ((CRFourLegsAnimalBodyDesc*)desc)->rangeElbow; \
		rangeFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->rangeFrontKnee; \
		rangeStifle = ((CRFourLegsAnimalBodyDesc*)desc)->rangeStifle; \
		rangeRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->rangeRearKnee; \
		goalWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->goalWaistChest; \
		goalWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->goalWaistTail; \
		goalTail = ((CRFourLegsAnimalBodyDesc*)desc)->goalTail; \
		goalChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->goalChestNeck; \
		goalNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->goalNeckHead; \
		goalShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->goalShoulder; \
		goalFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->goalFrontAnkle; \
		goalHip = ((CRFourLegsAnimalBodyDesc*)desc)->goalHip; \
		goalRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->goalRearAnkle; \
		originElbow = ((CRFourLegsAnimalBodyDesc*)desc)->originElbow; \
		originFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->originFrontKnee; \
		originStifle = ((CRFourLegsAnimalBodyDesc*)desc)->originStifle; \
		originRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->originRearKnee; \
		limitDirWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistChest; \
		limitDirWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistTail; \
		limitDirTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirTail; \
		limitDirChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirChestNeck; \
		limitDirNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirNeckHead; \
		limitDirShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirShoulder; \
		limitDirFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirFrontAnkle; \
		limitDirHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirHip; \
		limitDirRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirRearAnkle; \
		limitSwingWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistChest; \
		limitSwingWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistTail; \
		limitSwingTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingTail; \
		limitSwingChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingChestNeck; \
		limitSwingNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingNeckHead; \
		limitSwingShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingShoulder; \
		limitSwingFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingFrontAnkle; \
		limitSwingHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingHip; \
		limitSwingRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingRearAnkle; \
		limitTwistWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistChest; \
		limitTwistWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistTail; \
		limitTwistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistTail; \
		limitTwistChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistChestNeck; \
		limitTwistNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistNeckHead; \
		limitTwistShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistShoulder; \
		limitTwistFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistFrontAnkle; \
		limitTwistHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistHip; \
		limitTwistRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistRearAnkle; \
		noLegs = ((CRFourLegsAnimalBodyDesc*)desc)->noLegs; \
		dynamicalMode = ((CRFourLegsAnimalBodyDesc*)desc)->dynamicalMode; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRFourLegsAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailBreadth = tailBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailHeight = tailHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailThickness = tailThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckBreadth = neckBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckHeight = neckHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckThickness = neckThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->headBreadth = headBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->headHeight = headHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->headThickness = headThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneBreadth = breastboneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneHeight = breastboneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneThickness = breastboneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusBreadth = radiusBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusHeight = radiusHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusThickness = radiusThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneBreadth = frontCannonBoneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneHeight = frontCannonBoneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneThickness = frontCannonBoneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeBreadth = frontToeBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeHeight = frontToeHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeThickness = frontToeThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurBreadth = femurBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurHeight = femurHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurThickness = femurThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaBreadth = tibiaBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaHeight = tibiaHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaThickness = tibiaThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneBreadth = rearCannonBoneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneHeight = rearCannonBoneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneThickness = rearCannonBoneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeBreadth = rearToeBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeHeight = rearToeHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeThickness = rearToeThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->springWaistTail = springWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperWaistTail = damperWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->springTail = springTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperTail = damperTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->springChestNeck = springChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperChestNeck = damperChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->springNeckHead = springNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperNeckHead = damperNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->springShoulder = springShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperShoulder = damperShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->springFrontAnkle = springFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperFrontAnkle = damperFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springHip = springHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperHip = damperHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->springRearAnkle = springRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperRearAnkle = damperRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springElbow = springElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->springFrontKnee = springFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperFrontKnee = damperFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->springStifle = springStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperStifle = damperStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springRearKnee = springRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperRearKnee = damperRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeFrontKnee = rangeFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeStifle = rangeStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeRearKnee = rangeRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalWaistChest = goalWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalWaistTail = goalWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalTail = goalTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalChestNeck = goalChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalNeckHead = goalNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalShoulder = goalShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalFrontAnkle = goalFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalHip = goalHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalRearAnkle = goalRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->originElbow = originElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->originFrontKnee = originFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->originStifle = originStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->originRearKnee = originRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistChest = limitDirWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistTail = limitDirWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirTail = limitDirTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirChestNeck = limitDirChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirNeckHead = limitDirNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirShoulder = limitDirShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirFrontAnkle = limitDirFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirHip = limitDirHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirRearAnkle = limitDirRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistChest = limitSwingWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistTail = limitSwingWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingTail = limitSwingTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingChestNeck = limitSwingChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingNeckHead = limitSwingNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingShoulder = limitSwingShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingFrontAnkle = limitSwingFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingHip = limitSwingHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingRearAnkle = limitSwingRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistChest = limitTwistWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistTail = limitTwistWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistTail = limitTwistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistChestNeck = limitTwistChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistNeckHead = limitTwistNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistShoulder = limitTwistShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistFrontAnkle = limitTwistFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistHip = limitTwistHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistRearAnkle = limitTwistRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->noLegs = noLegs; \
		((CRFourLegsAnimalBodyDesc*)desc)->dynamicalMode = dynamicalMode; \
		return true; \
	} \

#define ACCESS_DESC_CRFourLegsAnimalBodyDesc_NOBASE \
protected: \
	double waistBreadth ; \
	double waistHeight ; \
	double waistThickness ; \
	double chestBreadth ; \
	double chestHeight ; \
	double chestThickness ; \
	double tailBreadth ; \
	double tailHeight ; \
	double tailThickness ; \
	double neckBreadth ; \
	double neckHeight ; \
	double neckThickness ; \
	double headBreadth ; \
	double headHeight ; \
	double headThickness ; \
	double breastboneBreadth ; \
	double breastboneHeight ; \
	double breastboneThickness ; \
	double radiusBreadth ; \
	double radiusHeight ; \
	double radiusThickness ; \
	double frontCannonBoneBreadth ; \
	double frontCannonBoneHeight ; \
	double frontCannonBoneThickness ; \
	double frontToeBreadth ; \
	double frontToeHeight ; \
	double frontToeThickness ; \
	double femurBreadth ; \
	double femurHeight ; \
	double femurThickness ; \
	double tibiaBreadth ; \
	double tibiaHeight ; \
	double tibiaThickness ; \
	double rearCannonBoneBreadth ; \
	double rearCannonBoneHeight ; \
	double rearCannonBoneThickness ; \
	double rearToeBreadth ; \
	double rearToeHeight ; \
	double rearToeThickness ; \
	double springWaistChest ; \
	double damperWaistChest ; \
	double springWaistTail ; \
	double damperWaistTail ; \
	double springTail ; \
	double damperTail ; \
	double springChestNeck ; \
	double damperChestNeck ; \
	double springNeckHead ; \
	double damperNeckHead ; \
	double springShoulder ; \
	double damperShoulder ; \
	double springFrontAnkle ; \
	double damperFrontAnkle ; \
	double springHip ; \
	double damperHip ; \
	double springRearAnkle ; \
	double damperRearAnkle ; \
	double springElbow ; \
	double damperElbow ; \
	double springFrontKnee ; \
	double damperFrontKnee ; \
	double springStifle ; \
	double damperStifle ; \
	double springRearKnee ; \
	double damperRearKnee ; \
	Vec2d rangeElbow ; \
	Vec2d rangeFrontKnee ; \
	Vec2d rangeStifle ; \
	Vec2d rangeRearKnee ; \
	Quaterniond goalWaistChest ; \
	Quaterniond goalWaistTail ; \
	Quaterniond goalTail ; \
	Quaterniond goalChestNeck ; \
	Quaterniond goalNeckHead ; \
	Quaterniond goalShoulder ; \
	Quaterniond goalFrontAnkle ; \
	Quaterniond goalHip ; \
	Quaterniond goalRearAnkle ; \
	double originElbow ; \
	double originFrontKnee ; \
	double originStifle ; \
	double originRearKnee ; \
	Vec3d limitDirWaistChest ; \
	Vec3d limitDirWaistTail ; \
	Vec3d limitDirTail ; \
	Vec3d limitDirChestNeck ; \
	Vec3d limitDirNeckHead ; \
	Vec3d limitDirShoulder ; \
	Vec3d limitDirFrontAnkle ; \
	Vec3d limitDirHip ; \
	Vec3d limitDirRearAnkle ; \
	Vec2d limitSwingWaistChest ; \
	Vec2d limitSwingWaistTail ; \
	Vec2d limitSwingTail ; \
	Vec2d limitSwingChestNeck ; \
	Vec2d limitSwingNeckHead ; \
	Vec2d limitSwingShoulder ; \
	Vec2d limitSwingFrontAnkle ; \
	Vec2d limitSwingHip ; \
	Vec2d limitSwingRearAnkle ; \
	Vec2d limitTwistWaistChest ; \
	Vec2d limitTwistWaistTail ; \
	Vec2d limitTwistTail ; \
	Vec2d limitTwistChestNeck ; \
	Vec2d limitTwistNeckHead ; \
	Vec2d limitTwistShoulder ; \
	Vec2d limitTwistFrontAnkle ; \
	Vec2d limitTwistHip ; \
	Vec2d limitTwistRearAnkle ; \
	bool noLegs ; \
	bool dynamicalMode ; \
public: \
	virtual void SetDesc(const void* desc){ \
		waistBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->waistBreadth; \
		waistHeight = ((CRFourLegsAnimalBodyDesc*)desc)->waistHeight; \
		waistThickness = ((CRFourLegsAnimalBodyDesc*)desc)->waistThickness; \
		chestBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->chestBreadth; \
		chestHeight = ((CRFourLegsAnimalBodyDesc*)desc)->chestHeight; \
		chestThickness = ((CRFourLegsAnimalBodyDesc*)desc)->chestThickness; \
		tailBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->tailBreadth; \
		tailHeight = ((CRFourLegsAnimalBodyDesc*)desc)->tailHeight; \
		tailThickness = ((CRFourLegsAnimalBodyDesc*)desc)->tailThickness; \
		neckBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->neckBreadth; \
		neckHeight = ((CRFourLegsAnimalBodyDesc*)desc)->neckHeight; \
		neckThickness = ((CRFourLegsAnimalBodyDesc*)desc)->neckThickness; \
		headBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->headBreadth; \
		headHeight = ((CRFourLegsAnimalBodyDesc*)desc)->headHeight; \
		headThickness = ((CRFourLegsAnimalBodyDesc*)desc)->headThickness; \
		breastboneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneBreadth; \
		breastboneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneHeight; \
		breastboneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->breastboneThickness; \
		radiusBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->radiusBreadth; \
		radiusHeight = ((CRFourLegsAnimalBodyDesc*)desc)->radiusHeight; \
		radiusThickness = ((CRFourLegsAnimalBodyDesc*)desc)->radiusThickness; \
		frontCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneBreadth; \
		frontCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneHeight; \
		frontCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneThickness; \
		frontToeBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeBreadth; \
		frontToeHeight = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeHeight; \
		frontToeThickness = ((CRFourLegsAnimalBodyDesc*)desc)->frontToeThickness; \
		femurBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->femurBreadth; \
		femurHeight = ((CRFourLegsAnimalBodyDesc*)desc)->femurHeight; \
		femurThickness = ((CRFourLegsAnimalBodyDesc*)desc)->femurThickness; \
		tibiaBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaBreadth; \
		tibiaHeight = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaHeight; \
		tibiaThickness = ((CRFourLegsAnimalBodyDesc*)desc)->tibiaThickness; \
		rearCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneBreadth; \
		rearCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneHeight; \
		rearCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneThickness; \
		rearToeBreadth = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeBreadth; \
		rearToeHeight = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeHeight; \
		rearToeThickness = ((CRFourLegsAnimalBodyDesc*)desc)->rearToeThickness; \
		springWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->springWaistChest; \
		damperWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->damperWaistChest; \
		springWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->springWaistTail; \
		damperWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->damperWaistTail; \
		springTail = ((CRFourLegsAnimalBodyDesc*)desc)->springTail; \
		damperTail = ((CRFourLegsAnimalBodyDesc*)desc)->damperTail; \
		springChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->springChestNeck; \
		damperChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->damperChestNeck; \
		springNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->springNeckHead; \
		damperNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->damperNeckHead; \
		springShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->springShoulder; \
		damperShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->damperShoulder; \
		springFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->springFrontAnkle; \
		damperFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->damperFrontAnkle; \
		springHip = ((CRFourLegsAnimalBodyDesc*)desc)->springHip; \
		damperHip = ((CRFourLegsAnimalBodyDesc*)desc)->damperHip; \
		springRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->springRearAnkle; \
		damperRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->damperRearAnkle; \
		springElbow = ((CRFourLegsAnimalBodyDesc*)desc)->springElbow; \
		damperElbow = ((CRFourLegsAnimalBodyDesc*)desc)->damperElbow; \
		springFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->springFrontKnee; \
		damperFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->damperFrontKnee; \
		springStifle = ((CRFourLegsAnimalBodyDesc*)desc)->springStifle; \
		damperStifle = ((CRFourLegsAnimalBodyDesc*)desc)->damperStifle; \
		springRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->springRearKnee; \
		damperRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->damperRearKnee; \
		rangeElbow = ((CRFourLegsAnimalBodyDesc*)desc)->rangeElbow; \
		rangeFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->rangeFrontKnee; \
		rangeStifle = ((CRFourLegsAnimalBodyDesc*)desc)->rangeStifle; \
		rangeRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->rangeRearKnee; \
		goalWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->goalWaistChest; \
		goalWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->goalWaistTail; \
		goalTail = ((CRFourLegsAnimalBodyDesc*)desc)->goalTail; \
		goalChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->goalChestNeck; \
		goalNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->goalNeckHead; \
		goalShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->goalShoulder; \
		goalFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->goalFrontAnkle; \
		goalHip = ((CRFourLegsAnimalBodyDesc*)desc)->goalHip; \
		goalRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->goalRearAnkle; \
		originElbow = ((CRFourLegsAnimalBodyDesc*)desc)->originElbow; \
		originFrontKnee = ((CRFourLegsAnimalBodyDesc*)desc)->originFrontKnee; \
		originStifle = ((CRFourLegsAnimalBodyDesc*)desc)->originStifle; \
		originRearKnee = ((CRFourLegsAnimalBodyDesc*)desc)->originRearKnee; \
		limitDirWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistChest; \
		limitDirWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistTail; \
		limitDirTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirTail; \
		limitDirChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirChestNeck; \
		limitDirNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirNeckHead; \
		limitDirShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirShoulder; \
		limitDirFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirFrontAnkle; \
		limitDirHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirHip; \
		limitDirRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitDirRearAnkle; \
		limitSwingWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistChest; \
		limitSwingWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistTail; \
		limitSwingTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingTail; \
		limitSwingChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingChestNeck; \
		limitSwingNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingNeckHead; \
		limitSwingShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingShoulder; \
		limitSwingFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingFrontAnkle; \
		limitSwingHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingHip; \
		limitSwingRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitSwingRearAnkle; \
		limitTwistWaistChest = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistChest; \
		limitTwistWaistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistTail; \
		limitTwistTail = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistTail; \
		limitTwistChestNeck = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistChestNeck; \
		limitTwistNeckHead = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistNeckHead; \
		limitTwistShoulder = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistShoulder; \
		limitTwistFrontAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistFrontAnkle; \
		limitTwistHip = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistHip; \
		limitTwistRearAnkle = ((CRFourLegsAnimalBodyDesc*)desc)->limitTwistRearAnkle; \
		noLegs = ((CRFourLegsAnimalBodyDesc*)desc)->noLegs; \
		dynamicalMode = ((CRFourLegsAnimalBodyDesc*)desc)->dynamicalMode; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRFourLegsAnimalBodyDesc*)desc)->waistBreadth = waistBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->waistHeight = waistHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->waistThickness = waistThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestBreadth = chestBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestHeight = chestHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->chestThickness = chestThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailBreadth = tailBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailHeight = tailHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->tailThickness = tailThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckBreadth = neckBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckHeight = neckHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->neckThickness = neckThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->headBreadth = headBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->headHeight = headHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->headThickness = headThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneBreadth = breastboneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneHeight = breastboneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->breastboneThickness = breastboneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusBreadth = radiusBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusHeight = radiusHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->radiusThickness = radiusThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneBreadth = frontCannonBoneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneHeight = frontCannonBoneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontCannonBoneThickness = frontCannonBoneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeBreadth = frontToeBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeHeight = frontToeHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->frontToeThickness = frontToeThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurBreadth = femurBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurHeight = femurHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->femurThickness = femurThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaBreadth = tibiaBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaHeight = tibiaHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->tibiaThickness = tibiaThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneBreadth = rearCannonBoneBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneHeight = rearCannonBoneHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearCannonBoneThickness = rearCannonBoneThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeBreadth = rearToeBreadth; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeHeight = rearToeHeight; \
		((CRFourLegsAnimalBodyDesc*)desc)->rearToeThickness = rearToeThickness; \
		((CRFourLegsAnimalBodyDesc*)desc)->springWaistChest = springWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperWaistChest = damperWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->springWaistTail = springWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperWaistTail = damperWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->springTail = springTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperTail = damperTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->springChestNeck = springChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperChestNeck = damperChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->springNeckHead = springNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperNeckHead = damperNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->springShoulder = springShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperShoulder = damperShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->springFrontAnkle = springFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperFrontAnkle = damperFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springHip = springHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperHip = damperHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->springRearAnkle = springRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperRearAnkle = damperRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springElbow = springElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperElbow = damperElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->springFrontKnee = springFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperFrontKnee = damperFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->springStifle = springStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperStifle = damperStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->springRearKnee = springRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->damperRearKnee = damperRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeElbow = rangeElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeFrontKnee = rangeFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeStifle = rangeStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->rangeRearKnee = rangeRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalWaistChest = goalWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalWaistTail = goalWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalTail = goalTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalChestNeck = goalChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalNeckHead = goalNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalShoulder = goalShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalFrontAnkle = goalFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalHip = goalHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->goalRearAnkle = goalRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->originElbow = originElbow; \
		((CRFourLegsAnimalBodyDesc*)desc)->originFrontKnee = originFrontKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->originStifle = originStifle; \
		((CRFourLegsAnimalBodyDesc*)desc)->originRearKnee = originRearKnee; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistChest = limitDirWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirWaistTail = limitDirWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirTail = limitDirTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirChestNeck = limitDirChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirNeckHead = limitDirNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirShoulder = limitDirShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirFrontAnkle = limitDirFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirHip = limitDirHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitDirRearAnkle = limitDirRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistChest = limitSwingWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingWaistTail = limitSwingWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingTail = limitSwingTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingChestNeck = limitSwingChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingNeckHead = limitSwingNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingShoulder = limitSwingShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingFrontAnkle = limitSwingFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingHip = limitSwingHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitSwingRearAnkle = limitSwingRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistChest = limitTwistWaistChest; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistWaistTail = limitTwistWaistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistTail = limitTwistTail; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistChestNeck = limitTwistChestNeck; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistNeckHead = limitTwistNeckHead; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistShoulder = limitTwistShoulder; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistFrontAnkle = limitTwistFrontAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistHip = limitTwistHip; \
		((CRFourLegsAnimalBodyDesc*)desc)->limitTwistRearAnkle = limitTwistRearAnkle; \
		((CRFourLegsAnimalBodyDesc*)desc)->noLegs = noLegs; \
		((CRFourLegsAnimalBodyDesc*)desc)->dynamicalMode = dynamicalMode; \
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
	float headDiameter ; \
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
	float headDiameter ; \
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

#define ACCESS_DESC_CRTryStandingUpControllerDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRTryStandingUpControllerDesc_NOBASE \
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

#define ACCESS_DESC_CRIKControlDesc(cls) \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		solid = ((CRIKControlDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKControlDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKControlDesc_NOBASE \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		solid = ((CRIKControlDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKControlDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKControlPosDesc(cls) \
protected: \
	Vec3d pos ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		pos = ((CRIKControlPosDesc*)desc)->pos; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKControlPosDesc*)desc)->pos = pos; \
		return true; \
	} \

#define ACCESS_DESC_CRIKControlPosDesc_NOBASE \
protected: \
	Vec3d pos ; \
public: \
	virtual void SetDesc(const void* desc){ \
		pos = ((CRIKControlPosDesc*)desc)->pos; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKControlPosDesc*)desc)->pos = pos; \
		return true; \
	} \

#define ACCESS_DESC_CRIKControlOriDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_CRIKControlOriDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableDesc(cls) \
protected: \
	float bias ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		bias = ((CRIKMovableDesc*)desc)->bias; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKMovableDesc*)desc)->bias = bias; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableDesc_NOBASE \
protected: \
	float bias ; \
public: \
	virtual void SetDesc(const void* desc){ \
		bias = ((CRIKMovableDesc*)desc)->bias; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKMovableDesc*)desc)->bias = bias; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableSolidPosDesc(cls) \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		solid = ((CRIKMovableSolidPosDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKMovableSolidPosDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableSolidPosDesc_NOBASE \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		solid = ((CRIKMovableSolidPosDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKMovableSolidPosDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableSolidOriDesc(cls) \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		solid = ((CRIKMovableSolidOriDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKMovableSolidOriDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableSolidOriDesc_NOBASE \
protected: \
	PHSolidIf *solid ; \
public: \
	virtual void SetDesc(const void* desc){ \
		solid = ((CRIKMovableSolidOriDesc*)desc)->solid; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKMovableSolidOriDesc*)desc)->solid = solid; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableBallJointOriDesc(cls) \
protected: \
	PHBallJointIf *joint ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		joint = ((CRIKMovableBallJointOriDesc*)desc)->joint; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKMovableBallJointOriDesc*)desc)->joint = joint; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableBallJointOriDesc_NOBASE \
protected: \
	PHBallJointIf *joint ; \
public: \
	virtual void SetDesc(const void* desc){ \
		joint = ((CRIKMovableBallJointOriDesc*)desc)->joint; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKMovableBallJointOriDesc*)desc)->joint = joint; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableHingeJointOriDesc(cls) \
protected: \
	PHHingeJointIf *joint ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		joint = ((CRIKMovableHingeJointOriDesc*)desc)->joint; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((CRIKMovableHingeJointOriDesc*)desc)->joint = joint; \
		return true; \
	} \

#define ACCESS_DESC_CRIKMovableHingeJointOriDesc_NOBASE \
protected: \
	PHHingeJointIf *joint ; \
public: \
	virtual void SetDesc(const void* desc){ \
		joint = ((CRIKMovableHingeJointOriDesc*)desc)->joint; \
	} \
	virtual bool GetDesc(void* desc){ \
		((CRIKMovableHingeJointOriDesc*)desc)->joint = joint; \
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

