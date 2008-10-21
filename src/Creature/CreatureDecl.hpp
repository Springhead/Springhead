#define SPR_DECLMEMBEROF_CRBodyDesc \
protected:\
	enum Spr::CRBodyDesc::CRCreatureJointOrder	jointOrder;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		jointOrder = ((CRBodyDesc*)ptr)->jointOrder;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CRBodyDesc*)ptr)->jointOrder = jointOrder;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRHingeHumanBodyDesc \
protected:\
	double	bodyMass;	\
	double	waistHeight;	\
	double	waistBreadth;	\
	double	waistThickness;	\
	double	abdomenHeight;	\
	double	abdomenBreadth;	\
	double	abdomenThickness;	\
	double	chestHeight;	\
	double	chestBreadth;	\
	double	chestThickness;	\
	double	neckLength;	\
	double	neckDiameter;	\
	double	headDiameter;	\
	double	upperArmLength;	\
	double	upperArmDiameter;	\
	double	lowerArmLength;	\
	double	lowerArmDiameter;	\
	double	handLength;	\
	double	handBreadth;	\
	double	handThickness;	\
	double	upperLegLength;	\
	double	upperLegDiameter;	\
	double	interLegDistance;	\
	double	lowerLegLength;	\
	double	lowerLegDiameter;	\
	double	footLength;	\
	double	footBreadth;	\
	double	footThickness;	\
	double	ankleToeDistance;	\
	double	vertexToEyeHeight;	\
	double	occiputToEyeDistance;	\
	double	eyeDiameter;	\
	double	interpupillaryBreadth;	\
	double	spring;	\
	double	damper;	\
	double	springWaistAbdomen;	\
	double	damperWaistAbdomen;	\
	double	springAbdomenChest;	\
	double	damperAbdomenChest;	\
	double	springChestNeckX;	\
	double	damperChestNeckX;	\
	double	springChestNeckY;	\
	double	damperChestNeckY;	\
	double	springChestNeckZ;	\
	double	damperChestNeckZ;	\
	double	springNeckHeadX;	\
	double	damperNeckHeadX;	\
	double	springNeckHeadZ;	\
	double	damperNeckHeadZ;	\
	double	springShoulderZ;	\
	double	damperShoulderZ;	\
	double	springShoulderX;	\
	double	damperShoulderX;	\
	double	springShoulderY;	\
	double	damperShoulderY;	\
	double	springElbow;	\
	double	damperElbow;	\
	double	springWristY;	\
	double	damperWristY;	\
	double	springWristX;	\
	double	damperWristX;	\
	double	springWristZ;	\
	double	damperWristZ;	\
	double	springWaistLegZ;	\
	double	damperWaistLegZ;	\
	double	springWaistLegX;	\
	double	damperWaistLegX;	\
	double	springWaistLegY;	\
	double	damperWaistLegY;	\
	double	springKnee;	\
	double	damperKnee;	\
	double	springAnkleY;	\
	double	damperAnkleY;	\
	double	springAnkleX;	\
	double	damperAnkleX;	\
	double	springAnkleZ;	\
	double	damperAnkleZ;	\
	double	springEyeY;	\
	double	damperEyeY;	\
	double	springEyeX;	\
	double	damperEyeX;	\
	Vec3d	posRightUpperArm;	\
	Quaterniond	oriRightUpperArm;	\
	Quaterniond	oriRightLowerArm;	\
	Quaterniond	oriRightHand;	\
	Vec2d	rangeWaistAbdomen;	\
	Vec2d	rangeAbdomenChest;	\
	Vec2d	rangeChestNeckX;	\
	Vec2d	rangeChestNeckY;	\
	Vec2d	rangeChestNeckZ;	\
	Vec2d	rangeNeckHeadX;	\
	Vec2d	rangeNeckHeadZ;	\
	Vec2d	rangeShoulderZ;	\
	Vec2d	rangeShoulderX;	\
	Vec2d	rangeShoulderY;	\
	Vec2d	rangeElbow;	\
	Vec2d	rangeWristY;	\
	Vec2d	rangeWristX;	\
	Vec2d	rangeWristZ;	\
	Vec2d	rangeWaistLegZ;	\
	Vec2d	rangeWaistLegX;	\
	Vec2d	rangeWaistLegY;	\
	Vec2d	rangeKnee;	\
	Vec2d	rangeAnkleY;	\
	Vec2d	rangeAnkleX;	\
	Vec2d	rangeAnkleZ;	\
	Vec2d	rangeRightEyeY;	\
	Vec2d	rangeEyeX;	\
	bool	noLegs;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRHingeHumanBodyDesc*)ptr);	\
		bodyMass = ((CRHingeHumanBodyDesc*)ptr)->bodyMass;	\
		waistHeight = ((CRHingeHumanBodyDesc*)ptr)->waistHeight;	\
		waistBreadth = ((CRHingeHumanBodyDesc*)ptr)->waistBreadth;	\
		waistThickness = ((CRHingeHumanBodyDesc*)ptr)->waistThickness;	\
		abdomenHeight = ((CRHingeHumanBodyDesc*)ptr)->abdomenHeight;	\
		abdomenBreadth = ((CRHingeHumanBodyDesc*)ptr)->abdomenBreadth;	\
		abdomenThickness = ((CRHingeHumanBodyDesc*)ptr)->abdomenThickness;	\
		chestHeight = ((CRHingeHumanBodyDesc*)ptr)->chestHeight;	\
		chestBreadth = ((CRHingeHumanBodyDesc*)ptr)->chestBreadth;	\
		chestThickness = ((CRHingeHumanBodyDesc*)ptr)->chestThickness;	\
		neckLength = ((CRHingeHumanBodyDesc*)ptr)->neckLength;	\
		neckDiameter = ((CRHingeHumanBodyDesc*)ptr)->neckDiameter;	\
		headDiameter = ((CRHingeHumanBodyDesc*)ptr)->headDiameter;	\
		upperArmLength = ((CRHingeHumanBodyDesc*)ptr)->upperArmLength;	\
		upperArmDiameter = ((CRHingeHumanBodyDesc*)ptr)->upperArmDiameter;	\
		lowerArmLength = ((CRHingeHumanBodyDesc*)ptr)->lowerArmLength;	\
		lowerArmDiameter = ((CRHingeHumanBodyDesc*)ptr)->lowerArmDiameter;	\
		handLength = ((CRHingeHumanBodyDesc*)ptr)->handLength;	\
		handBreadth = ((CRHingeHumanBodyDesc*)ptr)->handBreadth;	\
		handThickness = ((CRHingeHumanBodyDesc*)ptr)->handThickness;	\
		upperLegLength = ((CRHingeHumanBodyDesc*)ptr)->upperLegLength;	\
		upperLegDiameter = ((CRHingeHumanBodyDesc*)ptr)->upperLegDiameter;	\
		interLegDistance = ((CRHingeHumanBodyDesc*)ptr)->interLegDistance;	\
		lowerLegLength = ((CRHingeHumanBodyDesc*)ptr)->lowerLegLength;	\
		lowerLegDiameter = ((CRHingeHumanBodyDesc*)ptr)->lowerLegDiameter;	\
		footLength = ((CRHingeHumanBodyDesc*)ptr)->footLength;	\
		footBreadth = ((CRHingeHumanBodyDesc*)ptr)->footBreadth;	\
		footThickness = ((CRHingeHumanBodyDesc*)ptr)->footThickness;	\
		ankleToeDistance = ((CRHingeHumanBodyDesc*)ptr)->ankleToeDistance;	\
		vertexToEyeHeight = ((CRHingeHumanBodyDesc*)ptr)->vertexToEyeHeight;	\
		occiputToEyeDistance = ((CRHingeHumanBodyDesc*)ptr)->occiputToEyeDistance;	\
		eyeDiameter = ((CRHingeHumanBodyDesc*)ptr)->eyeDiameter;	\
		interpupillaryBreadth = ((CRHingeHumanBodyDesc*)ptr)->interpupillaryBreadth;	\
		spring = ((CRHingeHumanBodyDesc*)ptr)->spring;	\
		damper = ((CRHingeHumanBodyDesc*)ptr)->damper;	\
		springWaistAbdomen = ((CRHingeHumanBodyDesc*)ptr)->springWaistAbdomen;	\
		damperWaistAbdomen = ((CRHingeHumanBodyDesc*)ptr)->damperWaistAbdomen;	\
		springAbdomenChest = ((CRHingeHumanBodyDesc*)ptr)->springAbdomenChest;	\
		damperAbdomenChest = ((CRHingeHumanBodyDesc*)ptr)->damperAbdomenChest;	\
		springChestNeckX = ((CRHingeHumanBodyDesc*)ptr)->springChestNeckX;	\
		damperChestNeckX = ((CRHingeHumanBodyDesc*)ptr)->damperChestNeckX;	\
		springChestNeckY = ((CRHingeHumanBodyDesc*)ptr)->springChestNeckY;	\
		damperChestNeckY = ((CRHingeHumanBodyDesc*)ptr)->damperChestNeckY;	\
		springChestNeckZ = ((CRHingeHumanBodyDesc*)ptr)->springChestNeckZ;	\
		damperChestNeckZ = ((CRHingeHumanBodyDesc*)ptr)->damperChestNeckZ;	\
		springNeckHeadX = ((CRHingeHumanBodyDesc*)ptr)->springNeckHeadX;	\
		damperNeckHeadX = ((CRHingeHumanBodyDesc*)ptr)->damperNeckHeadX;	\
		springNeckHeadZ = ((CRHingeHumanBodyDesc*)ptr)->springNeckHeadZ;	\
		damperNeckHeadZ = ((CRHingeHumanBodyDesc*)ptr)->damperNeckHeadZ;	\
		springShoulderZ = ((CRHingeHumanBodyDesc*)ptr)->springShoulderZ;	\
		damperShoulderZ = ((CRHingeHumanBodyDesc*)ptr)->damperShoulderZ;	\
		springShoulderX = ((CRHingeHumanBodyDesc*)ptr)->springShoulderX;	\
		damperShoulderX = ((CRHingeHumanBodyDesc*)ptr)->damperShoulderX;	\
		springShoulderY = ((CRHingeHumanBodyDesc*)ptr)->springShoulderY;	\
		damperShoulderY = ((CRHingeHumanBodyDesc*)ptr)->damperShoulderY;	\
		springElbow = ((CRHingeHumanBodyDesc*)ptr)->springElbow;	\
		damperElbow = ((CRHingeHumanBodyDesc*)ptr)->damperElbow;	\
		springWristY = ((CRHingeHumanBodyDesc*)ptr)->springWristY;	\
		damperWristY = ((CRHingeHumanBodyDesc*)ptr)->damperWristY;	\
		springWristX = ((CRHingeHumanBodyDesc*)ptr)->springWristX;	\
		damperWristX = ((CRHingeHumanBodyDesc*)ptr)->damperWristX;	\
		springWristZ = ((CRHingeHumanBodyDesc*)ptr)->springWristZ;	\
		damperWristZ = ((CRHingeHumanBodyDesc*)ptr)->damperWristZ;	\
		springWaistLegZ = ((CRHingeHumanBodyDesc*)ptr)->springWaistLegZ;	\
		damperWaistLegZ = ((CRHingeHumanBodyDesc*)ptr)->damperWaistLegZ;	\
		springWaistLegX = ((CRHingeHumanBodyDesc*)ptr)->springWaistLegX;	\
		damperWaistLegX = ((CRHingeHumanBodyDesc*)ptr)->damperWaistLegX;	\
		springWaistLegY = ((CRHingeHumanBodyDesc*)ptr)->springWaistLegY;	\
		damperWaistLegY = ((CRHingeHumanBodyDesc*)ptr)->damperWaistLegY;	\
		springKnee = ((CRHingeHumanBodyDesc*)ptr)->springKnee;	\
		damperKnee = ((CRHingeHumanBodyDesc*)ptr)->damperKnee;	\
		springAnkleY = ((CRHingeHumanBodyDesc*)ptr)->springAnkleY;	\
		damperAnkleY = ((CRHingeHumanBodyDesc*)ptr)->damperAnkleY;	\
		springAnkleX = ((CRHingeHumanBodyDesc*)ptr)->springAnkleX;	\
		damperAnkleX = ((CRHingeHumanBodyDesc*)ptr)->damperAnkleX;	\
		springAnkleZ = ((CRHingeHumanBodyDesc*)ptr)->springAnkleZ;	\
		damperAnkleZ = ((CRHingeHumanBodyDesc*)ptr)->damperAnkleZ;	\
		springEyeY = ((CRHingeHumanBodyDesc*)ptr)->springEyeY;	\
		damperEyeY = ((CRHingeHumanBodyDesc*)ptr)->damperEyeY;	\
		springEyeX = ((CRHingeHumanBodyDesc*)ptr)->springEyeX;	\
		damperEyeX = ((CRHingeHumanBodyDesc*)ptr)->damperEyeX;	\
		posRightUpperArm = ((CRHingeHumanBodyDesc*)ptr)->posRightUpperArm;	\
		oriRightUpperArm = ((CRHingeHumanBodyDesc*)ptr)->oriRightUpperArm;	\
		oriRightLowerArm = ((CRHingeHumanBodyDesc*)ptr)->oriRightLowerArm;	\
		oriRightHand = ((CRHingeHumanBodyDesc*)ptr)->oriRightHand;	\
		rangeWaistAbdomen = ((CRHingeHumanBodyDesc*)ptr)->rangeWaistAbdomen;	\
		rangeAbdomenChest = ((CRHingeHumanBodyDesc*)ptr)->rangeAbdomenChest;	\
		rangeChestNeckX = ((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckX;	\
		rangeChestNeckY = ((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckY;	\
		rangeChestNeckZ = ((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckZ;	\
		rangeNeckHeadX = ((CRHingeHumanBodyDesc*)ptr)->rangeNeckHeadX;	\
		rangeNeckHeadZ = ((CRHingeHumanBodyDesc*)ptr)->rangeNeckHeadZ;	\
		rangeShoulderZ = ((CRHingeHumanBodyDesc*)ptr)->rangeShoulderZ;	\
		rangeShoulderX = ((CRHingeHumanBodyDesc*)ptr)->rangeShoulderX;	\
		rangeShoulderY = ((CRHingeHumanBodyDesc*)ptr)->rangeShoulderY;	\
		rangeElbow = ((CRHingeHumanBodyDesc*)ptr)->rangeElbow;	\
		rangeWristY = ((CRHingeHumanBodyDesc*)ptr)->rangeWristY;	\
		rangeWristX = ((CRHingeHumanBodyDesc*)ptr)->rangeWristX;	\
		rangeWristZ = ((CRHingeHumanBodyDesc*)ptr)->rangeWristZ;	\
		rangeWaistLegZ = ((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegZ;	\
		rangeWaistLegX = ((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegX;	\
		rangeWaistLegY = ((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegY;	\
		rangeKnee = ((CRHingeHumanBodyDesc*)ptr)->rangeKnee;	\
		rangeAnkleY = ((CRHingeHumanBodyDesc*)ptr)->rangeAnkleY;	\
		rangeAnkleX = ((CRHingeHumanBodyDesc*)ptr)->rangeAnkleX;	\
		rangeAnkleZ = ((CRHingeHumanBodyDesc*)ptr)->rangeAnkleZ;	\
		rangeRightEyeY = ((CRHingeHumanBodyDesc*)ptr)->rangeRightEyeY;	\
		rangeEyeX = ((CRHingeHumanBodyDesc*)ptr)->rangeEyeX;	\
		noLegs = ((CRHingeHumanBodyDesc*)ptr)->noLegs;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBody::GetDesc((CRBodyDesc*)(CRHingeHumanBodyDesc*)ptr);	\
		((CRHingeHumanBodyDesc*)ptr)->bodyMass = bodyMass;	\
		((CRHingeHumanBodyDesc*)ptr)->waistHeight = waistHeight;	\
		((CRHingeHumanBodyDesc*)ptr)->waistBreadth = waistBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->waistThickness = waistThickness;	\
		((CRHingeHumanBodyDesc*)ptr)->abdomenHeight = abdomenHeight;	\
		((CRHingeHumanBodyDesc*)ptr)->abdomenBreadth = abdomenBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->abdomenThickness = abdomenThickness;	\
		((CRHingeHumanBodyDesc*)ptr)->chestHeight = chestHeight;	\
		((CRHingeHumanBodyDesc*)ptr)->chestBreadth = chestBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->chestThickness = chestThickness;	\
		((CRHingeHumanBodyDesc*)ptr)->neckLength = neckLength;	\
		((CRHingeHumanBodyDesc*)ptr)->neckDiameter = neckDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->headDiameter = headDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->upperArmLength = upperArmLength;	\
		((CRHingeHumanBodyDesc*)ptr)->upperArmDiameter = upperArmDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->lowerArmLength = lowerArmLength;	\
		((CRHingeHumanBodyDesc*)ptr)->lowerArmDiameter = lowerArmDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->handLength = handLength;	\
		((CRHingeHumanBodyDesc*)ptr)->handBreadth = handBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->handThickness = handThickness;	\
		((CRHingeHumanBodyDesc*)ptr)->upperLegLength = upperLegLength;	\
		((CRHingeHumanBodyDesc*)ptr)->upperLegDiameter = upperLegDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->interLegDistance = interLegDistance;	\
		((CRHingeHumanBodyDesc*)ptr)->lowerLegLength = lowerLegLength;	\
		((CRHingeHumanBodyDesc*)ptr)->lowerLegDiameter = lowerLegDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->footLength = footLength;	\
		((CRHingeHumanBodyDesc*)ptr)->footBreadth = footBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->footThickness = footThickness;	\
		((CRHingeHumanBodyDesc*)ptr)->ankleToeDistance = ankleToeDistance;	\
		((CRHingeHumanBodyDesc*)ptr)->vertexToEyeHeight = vertexToEyeHeight;	\
		((CRHingeHumanBodyDesc*)ptr)->occiputToEyeDistance = occiputToEyeDistance;	\
		((CRHingeHumanBodyDesc*)ptr)->eyeDiameter = eyeDiameter;	\
		((CRHingeHumanBodyDesc*)ptr)->interpupillaryBreadth = interpupillaryBreadth;	\
		((CRHingeHumanBodyDesc*)ptr)->spring = spring;	\
		((CRHingeHumanBodyDesc*)ptr)->damper = damper;	\
		((CRHingeHumanBodyDesc*)ptr)->springWaistAbdomen = springWaistAbdomen;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWaistAbdomen = damperWaistAbdomen;	\
		((CRHingeHumanBodyDesc*)ptr)->springAbdomenChest = springAbdomenChest;	\
		((CRHingeHumanBodyDesc*)ptr)->damperAbdomenChest = damperAbdomenChest;	\
		((CRHingeHumanBodyDesc*)ptr)->springChestNeckX = springChestNeckX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperChestNeckX = damperChestNeckX;	\
		((CRHingeHumanBodyDesc*)ptr)->springChestNeckY = springChestNeckY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperChestNeckY = damperChestNeckY;	\
		((CRHingeHumanBodyDesc*)ptr)->springChestNeckZ = springChestNeckZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperChestNeckZ = damperChestNeckZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springNeckHeadX = springNeckHeadX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperNeckHeadX = damperNeckHeadX;	\
		((CRHingeHumanBodyDesc*)ptr)->springNeckHeadZ = springNeckHeadZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperNeckHeadZ = damperNeckHeadZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springShoulderZ = springShoulderZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperShoulderZ = damperShoulderZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springShoulderX = springShoulderX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperShoulderX = damperShoulderX;	\
		((CRHingeHumanBodyDesc*)ptr)->springShoulderY = springShoulderY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperShoulderY = damperShoulderY;	\
		((CRHingeHumanBodyDesc*)ptr)->springElbow = springElbow;	\
		((CRHingeHumanBodyDesc*)ptr)->damperElbow = damperElbow;	\
		((CRHingeHumanBodyDesc*)ptr)->springWristY = springWristY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWristY = damperWristY;	\
		((CRHingeHumanBodyDesc*)ptr)->springWristX = springWristX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWristX = damperWristX;	\
		((CRHingeHumanBodyDesc*)ptr)->springWristZ = springWristZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWristZ = damperWristZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springWaistLegZ = springWaistLegZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWaistLegZ = damperWaistLegZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springWaistLegX = springWaistLegX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWaistLegX = damperWaistLegX;	\
		((CRHingeHumanBodyDesc*)ptr)->springWaistLegY = springWaistLegY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperWaistLegY = damperWaistLegY;	\
		((CRHingeHumanBodyDesc*)ptr)->springKnee = springKnee;	\
		((CRHingeHumanBodyDesc*)ptr)->damperKnee = damperKnee;	\
		((CRHingeHumanBodyDesc*)ptr)->springAnkleY = springAnkleY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperAnkleY = damperAnkleY;	\
		((CRHingeHumanBodyDesc*)ptr)->springAnkleX = springAnkleX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperAnkleX = damperAnkleX;	\
		((CRHingeHumanBodyDesc*)ptr)->springAnkleZ = springAnkleZ;	\
		((CRHingeHumanBodyDesc*)ptr)->damperAnkleZ = damperAnkleZ;	\
		((CRHingeHumanBodyDesc*)ptr)->springEyeY = springEyeY;	\
		((CRHingeHumanBodyDesc*)ptr)->damperEyeY = damperEyeY;	\
		((CRHingeHumanBodyDesc*)ptr)->springEyeX = springEyeX;	\
		((CRHingeHumanBodyDesc*)ptr)->damperEyeX = damperEyeX;	\
		((CRHingeHumanBodyDesc*)ptr)->posRightUpperArm = posRightUpperArm;	\
		((CRHingeHumanBodyDesc*)ptr)->oriRightUpperArm = oriRightUpperArm;	\
		((CRHingeHumanBodyDesc*)ptr)->oriRightLowerArm = oriRightLowerArm;	\
		((CRHingeHumanBodyDesc*)ptr)->oriRightHand = oriRightHand;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWaistAbdomen = rangeWaistAbdomen;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeAbdomenChest = rangeAbdomenChest;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckX = rangeChestNeckX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckY = rangeChestNeckY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeChestNeckZ = rangeChestNeckZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeNeckHeadX = rangeNeckHeadX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeNeckHeadZ = rangeNeckHeadZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeShoulderZ = rangeShoulderZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeShoulderX = rangeShoulderX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeShoulderY = rangeShoulderY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeElbow = rangeElbow;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWristY = rangeWristY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWristX = rangeWristX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWristZ = rangeWristZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegZ = rangeWaistLegZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegX = rangeWaistLegX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeWaistLegY = rangeWaistLegY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeKnee = rangeKnee;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeAnkleY = rangeAnkleY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeAnkleX = rangeAnkleX;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeAnkleZ = rangeAnkleZ;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeRightEyeY = rangeRightEyeY;	\
		((CRHingeHumanBodyDesc*)ptr)->rangeEyeX = rangeEyeX;	\
		((CRHingeHumanBodyDesc*)ptr)->noLegs = noLegs;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRFourLegsAnimalBodyDesc \
protected:\
	int	joNBallJoints;	\
	int	joNHingeJoints;	\
	double	waistBreadth;	\
	double	waistHeight;	\
	double	waistThickness;	\
	double	chestBreadth;	\
	double	chestHeight;	\
	double	chestThickness;	\
	double	tailBreadth;	\
	double	tailHeight;	\
	double	tailThickness;	\
	double	neckBreadth;	\
	double	neckHeight;	\
	double	neckThickness;	\
	double	headBreadth;	\
	double	headHeight;	\
	double	headThickness;	\
	double	breastboneBreadth;	\
	double	breastboneHeight;	\
	double	breastboneThickness;	\
	double	radiusBreadth;	\
	double	radiusHeight;	\
	double	radiusThickness;	\
	double	frontCannonBoneBreadth;	\
	double	frontCannonBoneHeight;	\
	double	frontCannonBoneThickness;	\
	double	frontToeBreadth;	\
	double	frontToeHeight;	\
	double	frontToeThickness;	\
	double	femurBreadth;	\
	double	femurHeight;	\
	double	femurThickness;	\
	double	tibiaBreadth;	\
	double	tibiaHeight;	\
	double	tibiaThickness;	\
	double	rearCannonBoneBreadth;	\
	double	rearCannonBoneHeight;	\
	double	rearCannonBoneThickness;	\
	double	rearToeBreadth;	\
	double	rearToeHeight;	\
	double	rearToeThickness;	\
	double	springWaistChest;	\
	double	damperWaistChest;	\
	double	springWaistTail;	\
	double	damperWaistTail;	\
	double	springTail;	\
	double	damperTail;	\
	double	springChestNeck;	\
	double	damperChestNeck;	\
	double	springNeckHead;	\
	double	damperNeckHead;	\
	double	springShoulder;	\
	double	damperShoulder;	\
	double	springFrontAnkle;	\
	double	damperFrontAnkle;	\
	double	springHip;	\
	double	damperHip;	\
	double	springRearAnkle;	\
	double	damperRearAnkle;	\
	double	springElbow;	\
	double	damperElbow;	\
	double	springFrontKnee;	\
	double	damperFrontKnee;	\
	double	springStifle;	\
	double	damperStifle;	\
	double	springRearKnee;	\
	double	damperRearKnee;	\
	Vec2d	rangeElbow;	\
	Vec2d	rangeFrontKnee;	\
	Vec2d	rangeStifle;	\
	Vec2d	rangeRearKnee;	\
	Quaterniond	goalWaistChest;	\
	Quaterniond	goalWaistTail;	\
	Quaterniond	goalTail;	\
	Quaterniond	goalChestNeck;	\
	Quaterniond	goalNeckHead;	\
	Quaterniond	goalShoulder;	\
	Quaterniond	goalFrontAnkle;	\
	Quaterniond	goalHip;	\
	Quaterniond	goalRearAnkle;	\
	double	originElbow;	\
	double	originFrontKnee;	\
	double	originStifle;	\
	double	originRearKnee;	\
	Vec3d	limitDirWaistChest;	\
	Vec3d	limitDirWaistTail;	\
	Vec3d	limitDirTail;	\
	Vec3d	limitDirChestNeck;	\
	Vec3d	limitDirNeckHead;	\
	Vec3d	limitDirShoulder;	\
	Vec3d	limitDirFrontAnkle;	\
	Vec3d	limitDirHip;	\
	Vec3d	limitDirRearAnkle;	\
	Vec2d	limitSwingWaistChest;	\
	Vec2d	limitSwingWaistTail;	\
	Vec2d	limitSwingTail;	\
	Vec2d	limitSwingChestNeck;	\
	Vec2d	limitSwingNeckHead;	\
	Vec2d	limitSwingShoulder;	\
	Vec2d	limitSwingFrontAnkle;	\
	Vec2d	limitSwingHip;	\
	Vec2d	limitSwingRearAnkle;	\
	Vec2d	limitTwistWaistChest;	\
	Vec2d	limitTwistWaistTail;	\
	Vec2d	limitTwistTail;	\
	Vec2d	limitTwistChestNeck;	\
	Vec2d	limitTwistNeckHead;	\
	Vec2d	limitTwistShoulder;	\
	Vec2d	limitTwistFrontAnkle;	\
	Vec2d	limitTwistHip;	\
	Vec2d	limitTwistRearAnkle;	\
	double	fMaxWaistChest;	\
	double	fMaxChestNeck;	\
	double	fMaxNeckHead;	\
	double	fMaxWaistTail;	\
	double	fMaxTail12;	\
	double	fMaxTail23;	\
	double	fMaxLeftShoulder;	\
	double	fMaxLeftElbow;	\
	double	fMaxLeftFrontKnee;	\
	double	fMaxLeftFrontAnkle;	\
	double	fMaxLeftHip;	\
	double	fMaxLeftStifle;	\
	double	fMaxLeftRearKnee;	\
	double	fMaxLeftRearAnkle;	\
	double	fMaxRightShoulder;	\
	double	fMaxRightElbow;	\
	double	fMaxRightFrontKnee;	\
	double	fMaxRightFrontAnkle;	\
	double	fMaxRightHip;	\
	double	fMaxRightStifle;	\
	double	fMaxRightRearKnee;	\
	double	fMaxRightRearAnkle;	\
	float	materialMu;	\
	bool	noLegs;	\
	bool	noHead;	\
	bool	dynamicalMode;	\
	double	totalMass;	\
	bool	flagFMax;	\
	bool	flagRange;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRFourLegsAnimalBodyDesc*)ptr);	\
		joNBallJoints = ((CRFourLegsAnimalBodyDesc*)ptr)->joNBallJoints;	\
		joNHingeJoints = ((CRFourLegsAnimalBodyDesc*)ptr)->joNHingeJoints;	\
		waistBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->waistBreadth;	\
		waistHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->waistHeight;	\
		waistThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->waistThickness;	\
		chestBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->chestBreadth;	\
		chestHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->chestHeight;	\
		chestThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->chestThickness;	\
		tailBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->tailBreadth;	\
		tailHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->tailHeight;	\
		tailThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->tailThickness;	\
		neckBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->neckBreadth;	\
		neckHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->neckHeight;	\
		neckThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->neckThickness;	\
		headBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->headBreadth;	\
		headHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->headHeight;	\
		headThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->headThickness;	\
		breastboneBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->breastboneBreadth;	\
		breastboneHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->breastboneHeight;	\
		breastboneThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->breastboneThickness;	\
		radiusBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->radiusBreadth;	\
		radiusHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->radiusHeight;	\
		radiusThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->radiusThickness;	\
		frontCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneBreadth;	\
		frontCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneHeight;	\
		frontCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneThickness;	\
		frontToeBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->frontToeBreadth;	\
		frontToeHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->frontToeHeight;	\
		frontToeThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->frontToeThickness;	\
		femurBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->femurBreadth;	\
		femurHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->femurHeight;	\
		femurThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->femurThickness;	\
		tibiaBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->tibiaBreadth;	\
		tibiaHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->tibiaHeight;	\
		tibiaThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->tibiaThickness;	\
		rearCannonBoneBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneBreadth;	\
		rearCannonBoneHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneHeight;	\
		rearCannonBoneThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneThickness;	\
		rearToeBreadth = ((CRFourLegsAnimalBodyDesc*)ptr)->rearToeBreadth;	\
		rearToeHeight = ((CRFourLegsAnimalBodyDesc*)ptr)->rearToeHeight;	\
		rearToeThickness = ((CRFourLegsAnimalBodyDesc*)ptr)->rearToeThickness;	\
		springWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->springWaistChest;	\
		damperWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->damperWaistChest;	\
		springWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->springWaistTail;	\
		damperWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->damperWaistTail;	\
		springTail = ((CRFourLegsAnimalBodyDesc*)ptr)->springTail;	\
		damperTail = ((CRFourLegsAnimalBodyDesc*)ptr)->damperTail;	\
		springChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->springChestNeck;	\
		damperChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->damperChestNeck;	\
		springNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->springNeckHead;	\
		damperNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->damperNeckHead;	\
		springShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->springShoulder;	\
		damperShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->damperShoulder;	\
		springFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->springFrontAnkle;	\
		damperFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->damperFrontAnkle;	\
		springHip = ((CRFourLegsAnimalBodyDesc*)ptr)->springHip;	\
		damperHip = ((CRFourLegsAnimalBodyDesc*)ptr)->damperHip;	\
		springRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->springRearAnkle;	\
		damperRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->damperRearAnkle;	\
		springElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->springElbow;	\
		damperElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->damperElbow;	\
		springFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->springFrontKnee;	\
		damperFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->damperFrontKnee;	\
		springStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->springStifle;	\
		damperStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->damperStifle;	\
		springRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->springRearKnee;	\
		damperRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->damperRearKnee;	\
		rangeElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->rangeElbow;	\
		rangeFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->rangeFrontKnee;	\
		rangeStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->rangeStifle;	\
		rangeRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->rangeRearKnee;	\
		goalWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->goalWaistChest;	\
		goalWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->goalWaistTail;	\
		goalTail = ((CRFourLegsAnimalBodyDesc*)ptr)->goalTail;	\
		goalChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->goalChestNeck;	\
		goalNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->goalNeckHead;	\
		goalShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->goalShoulder;	\
		goalFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->goalFrontAnkle;	\
		goalHip = ((CRFourLegsAnimalBodyDesc*)ptr)->goalHip;	\
		goalRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->goalRearAnkle;	\
		originElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->originElbow;	\
		originFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->originFrontKnee;	\
		originStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->originStifle;	\
		originRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->originRearKnee;	\
		limitDirWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirWaistChest;	\
		limitDirWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirWaistTail;	\
		limitDirTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirTail;	\
		limitDirChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirChestNeck;	\
		limitDirNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirNeckHead;	\
		limitDirShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirShoulder;	\
		limitDirFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirFrontAnkle;	\
		limitDirHip = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirHip;	\
		limitDirRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitDirRearAnkle;	\
		limitSwingWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingWaistChest;	\
		limitSwingWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingWaistTail;	\
		limitSwingTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingTail;	\
		limitSwingChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingChestNeck;	\
		limitSwingNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingNeckHead;	\
		limitSwingShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingShoulder;	\
		limitSwingFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingFrontAnkle;	\
		limitSwingHip = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingHip;	\
		limitSwingRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingRearAnkle;	\
		limitTwistWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistWaistChest;	\
		limitTwistWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistWaistTail;	\
		limitTwistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistTail;	\
		limitTwistChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistChestNeck;	\
		limitTwistNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistNeckHead;	\
		limitTwistShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistShoulder;	\
		limitTwistFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistFrontAnkle;	\
		limitTwistHip = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistHip;	\
		limitTwistRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistRearAnkle;	\
		fMaxWaistChest = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxWaistChest;	\
		fMaxChestNeck = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxChestNeck;	\
		fMaxNeckHead = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxNeckHead;	\
		fMaxWaistTail = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxWaistTail;	\
		fMaxTail12 = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxTail12;	\
		fMaxTail23 = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxTail23;	\
		fMaxLeftShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftShoulder;	\
		fMaxLeftElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftElbow;	\
		fMaxLeftFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftFrontKnee;	\
		fMaxLeftFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftFrontAnkle;	\
		fMaxLeftHip = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftHip;	\
		fMaxLeftStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftStifle;	\
		fMaxLeftRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftRearKnee;	\
		fMaxLeftRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftRearAnkle;	\
		fMaxRightShoulder = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightShoulder;	\
		fMaxRightElbow = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightElbow;	\
		fMaxRightFrontKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightFrontKnee;	\
		fMaxRightFrontAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightFrontAnkle;	\
		fMaxRightHip = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightHip;	\
		fMaxRightStifle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightStifle;	\
		fMaxRightRearKnee = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightRearKnee;	\
		fMaxRightRearAnkle = ((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightRearAnkle;	\
		materialMu = ((CRFourLegsAnimalBodyDesc*)ptr)->materialMu;	\
		noLegs = ((CRFourLegsAnimalBodyDesc*)ptr)->noLegs;	\
		noHead = ((CRFourLegsAnimalBodyDesc*)ptr)->noHead;	\
		dynamicalMode = ((CRFourLegsAnimalBodyDesc*)ptr)->dynamicalMode;	\
		totalMass = ((CRFourLegsAnimalBodyDesc*)ptr)->totalMass;	\
		flagFMax = ((CRFourLegsAnimalBodyDesc*)ptr)->flagFMax;	\
		flagRange = ((CRFourLegsAnimalBodyDesc*)ptr)->flagRange;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBody::GetDesc((CRBodyDesc*)(CRFourLegsAnimalBodyDesc*)ptr);	\
		((CRFourLegsAnimalBodyDesc*)ptr)->joNBallJoints = joNBallJoints;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->joNHingeJoints = joNHingeJoints;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->waistBreadth = waistBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->waistHeight = waistHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->waistThickness = waistThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->chestBreadth = chestBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->chestHeight = chestHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->chestThickness = chestThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tailBreadth = tailBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tailHeight = tailHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tailThickness = tailThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->neckBreadth = neckBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->neckHeight = neckHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->neckThickness = neckThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->headBreadth = headBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->headHeight = headHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->headThickness = headThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->breastboneBreadth = breastboneBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->breastboneHeight = breastboneHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->breastboneThickness = breastboneThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->radiusBreadth = radiusBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->radiusHeight = radiusHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->radiusThickness = radiusThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneBreadth = frontCannonBoneBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneHeight = frontCannonBoneHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontCannonBoneThickness = frontCannonBoneThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontToeBreadth = frontToeBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontToeHeight = frontToeHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->frontToeThickness = frontToeThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->femurBreadth = femurBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->femurHeight = femurHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->femurThickness = femurThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tibiaBreadth = tibiaBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tibiaHeight = tibiaHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->tibiaThickness = tibiaThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneBreadth = rearCannonBoneBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneHeight = rearCannonBoneHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearCannonBoneThickness = rearCannonBoneThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearToeBreadth = rearToeBreadth;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearToeHeight = rearToeHeight;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rearToeThickness = rearToeThickness;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springWaistChest = springWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperWaistChest = damperWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springWaistTail = springWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperWaistTail = damperWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springTail = springTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperTail = damperTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springChestNeck = springChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperChestNeck = damperChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springNeckHead = springNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperNeckHead = damperNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springShoulder = springShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperShoulder = damperShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springFrontAnkle = springFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperFrontAnkle = damperFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springHip = springHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperHip = damperHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springRearAnkle = springRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperRearAnkle = damperRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springElbow = springElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperElbow = damperElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springFrontKnee = springFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperFrontKnee = damperFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springStifle = springStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperStifle = damperStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->springRearKnee = springRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->damperRearKnee = damperRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rangeElbow = rangeElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rangeFrontKnee = rangeFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rangeStifle = rangeStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->rangeRearKnee = rangeRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalWaistChest = goalWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalWaistTail = goalWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalTail = goalTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalChestNeck = goalChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalNeckHead = goalNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalShoulder = goalShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalFrontAnkle = goalFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalHip = goalHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->goalRearAnkle = goalRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->originElbow = originElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->originFrontKnee = originFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->originStifle = originStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->originRearKnee = originRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirWaistChest = limitDirWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirWaistTail = limitDirWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirTail = limitDirTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirChestNeck = limitDirChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirNeckHead = limitDirNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirShoulder = limitDirShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirFrontAnkle = limitDirFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirHip = limitDirHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitDirRearAnkle = limitDirRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingWaistChest = limitSwingWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingWaistTail = limitSwingWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingTail = limitSwingTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingChestNeck = limitSwingChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingNeckHead = limitSwingNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingShoulder = limitSwingShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingFrontAnkle = limitSwingFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingHip = limitSwingHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitSwingRearAnkle = limitSwingRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistWaistChest = limitTwistWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistWaistTail = limitTwistWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistTail = limitTwistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistChestNeck = limitTwistChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistNeckHead = limitTwistNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistShoulder = limitTwistShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistFrontAnkle = limitTwistFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistHip = limitTwistHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->limitTwistRearAnkle = limitTwistRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxWaistChest = fMaxWaistChest;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxChestNeck = fMaxChestNeck;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxNeckHead = fMaxNeckHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxWaistTail = fMaxWaistTail;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxTail12 = fMaxTail12;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxTail23 = fMaxTail23;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftShoulder = fMaxLeftShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftElbow = fMaxLeftElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftFrontKnee = fMaxLeftFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftFrontAnkle = fMaxLeftFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftHip = fMaxLeftHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftStifle = fMaxLeftStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftRearKnee = fMaxLeftRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxLeftRearAnkle = fMaxLeftRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightShoulder = fMaxRightShoulder;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightElbow = fMaxRightElbow;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightFrontKnee = fMaxRightFrontKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightFrontAnkle = fMaxRightFrontAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightHip = fMaxRightHip;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightStifle = fMaxRightStifle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightRearKnee = fMaxRightRearKnee;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->fMaxRightRearAnkle = fMaxRightRearAnkle;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->materialMu = materialMu;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->noLegs = noLegs;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->noHead = noHead;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->dynamicalMode = dynamicalMode;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->totalMass = totalMass;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->flagFMax = flagFMax;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->flagRange = flagRange;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRManipulatorDesc \
protected:\
	int	soNSolids;	\
	int	joNBallJoints;	\
	int	joNHingeJoints;	\
	int	joNJoints;	\
	double	bRoot;	\
	double	bFinger00;	\
	double	lFinger00;	\
	double	tFinger00;	\
	double	bFinger01;	\
	double	lFinger01;	\
	double	tFinger01;	\
	double	bFinger02;	\
	double	lFinger02;	\
	double	tFinger02;	\
	double	bFinger10;	\
	double	lFigner10;	\
	double	tFinger10;	\
	double	bFinger11;	\
	double	lFinger11;	\
	double	tFinger11;	\
	double	bFinger12;	\
	double	lFigner12;	\
	double	tFinger12;	\
	double	bFinger13;	\
	double	lFinger13;	\
	double	tFigner13;	\
	double	bFinger20;	\
	double	lFigner20;	\
	double	tFinger20;	\
	double	bFinger21;	\
	double	lFinger21;	\
	double	tFinger21;	\
	double	bFinger22;	\
	double	lFigner22;	\
	double	tFinger22;	\
	double	bFinger23;	\
	double	lFinger23;	\
	double	tFigner23;	\
	double	bFinger30;	\
	double	lFigner30;	\
	double	tFinger30;	\
	double	bFinger31;	\
	double	lFinger31;	\
	double	tFinger31;	\
	double	bFinger32;	\
	double	lFigner32;	\
	double	tFinger32;	\
	double	bFinger33;	\
	double	lFinger33;	\
	double	tFigner33;	\
	double	bFinger40;	\
	double	lFigner40;	\
	double	tFinger40;	\
	double	bFinger41;	\
	double	lFinger41;	\
	double	tFinger41;	\
	double	bFinger42;	\
	double	lFigner42;	\
	double	tFinger42;	\
	double	bFinger43;	\
	double	lFinger43;	\
	double	tFigner43;	\
	double	spring00;	\
	double	damper00;	\
	double	spring01;	\
	double	damper01;	\
	double	spring02;	\
	double	damper02;	\
	double	spring10;	\
	double	damper10;	\
	double	spring11;	\
	double	damper11;	\
	double	spring12;	\
	double	damper12;	\
	double	spring13;	\
	double	damper13;	\
	double	spring20;	\
	double	damper20;	\
	double	spring21;	\
	double	damper21;	\
	double	spring22;	\
	double	damper22;	\
	double	spring23;	\
	double	damper23;	\
	double	spring30;	\
	double	damper30;	\
	double	spring31;	\
	double	damper31;	\
	double	spring32;	\
	double	damper32;	\
	double	spring33;	\
	double	damper33;	\
	double	spring40;	\
	double	damper40;	\
	double	spring41;	\
	double	damper41;	\
	double	spring42;	\
	double	damper42;	\
	double	spring43;	\
	double	damper43;	\
	Vec2d	range01;	\
	Vec2d	range02;	\
	Vec2d	range11;	\
	Vec2d	range12;	\
	Vec2d	range13;	\
	Vec2d	range21;	\
	Vec2d	range22;	\
	Vec2d	range23;	\
	Vec2d	range31;	\
	Vec2d	range32;	\
	Vec2d	range33;	\
	Vec2d	range41;	\
	Vec2d	range42;	\
	Vec2d	range43;	\
	Quaterniond	goalFinger0;	\
	Quaterniond	goalFinger1;	\
	Quaterniond	goalFinger2;	\
	Quaterniond	goalFigner3;	\
	Quaterniond	goalFinger4;	\
	Vec2d	limitSwing00;	\
	Vec2d	limitSwing10;	\
	Vec2d	limitSwing20;	\
	Vec2d	limitSwing30;	\
	Vec2d	limitSwing40;	\
	Vec2d	limitTwist00;	\
	Vec2d	limitTwist10;	\
	Vec2d	limitTwist20;	\
	Vec2d	limitTwist30;	\
	Vec2d	limitTwist40;	\
	double	fMaxFinger0;	\
	double	fMaxFinger1;	\
	double	fMaxFinger2;	\
	double	fMaxFinger3;	\
	double	fMaxFinger4;	\
	float	materialMu;	\
	bool	dynamicalMode;	\
	bool	flagFMax;	\
	bool	flagRange;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRManipulatorDesc*)ptr);	\
		soNSolids = ((CRManipulatorDesc*)ptr)->soNSolids;	\
		joNBallJoints = ((CRManipulatorDesc*)ptr)->joNBallJoints;	\
		joNHingeJoints = ((CRManipulatorDesc*)ptr)->joNHingeJoints;	\
		joNJoints = ((CRManipulatorDesc*)ptr)->joNJoints;	\
		bRoot = ((CRManipulatorDesc*)ptr)->bRoot;	\
		bFinger00 = ((CRManipulatorDesc*)ptr)->bFinger00;	\
		lFinger00 = ((CRManipulatorDesc*)ptr)->lFinger00;	\
		tFinger00 = ((CRManipulatorDesc*)ptr)->tFinger00;	\
		bFinger01 = ((CRManipulatorDesc*)ptr)->bFinger01;	\
		lFinger01 = ((CRManipulatorDesc*)ptr)->lFinger01;	\
		tFinger01 = ((CRManipulatorDesc*)ptr)->tFinger01;	\
		bFinger02 = ((CRManipulatorDesc*)ptr)->bFinger02;	\
		lFinger02 = ((CRManipulatorDesc*)ptr)->lFinger02;	\
		tFinger02 = ((CRManipulatorDesc*)ptr)->tFinger02;	\
		bFinger10 = ((CRManipulatorDesc*)ptr)->bFinger10;	\
		lFigner10 = ((CRManipulatorDesc*)ptr)->lFigner10;	\
		tFinger10 = ((CRManipulatorDesc*)ptr)->tFinger10;	\
		bFinger11 = ((CRManipulatorDesc*)ptr)->bFinger11;	\
		lFinger11 = ((CRManipulatorDesc*)ptr)->lFinger11;	\
		tFinger11 = ((CRManipulatorDesc*)ptr)->tFinger11;	\
		bFinger12 = ((CRManipulatorDesc*)ptr)->bFinger12;	\
		lFigner12 = ((CRManipulatorDesc*)ptr)->lFigner12;	\
		tFinger12 = ((CRManipulatorDesc*)ptr)->tFinger12;	\
		bFinger13 = ((CRManipulatorDesc*)ptr)->bFinger13;	\
		lFinger13 = ((CRManipulatorDesc*)ptr)->lFinger13;	\
		tFigner13 = ((CRManipulatorDesc*)ptr)->tFigner13;	\
		bFinger20 = ((CRManipulatorDesc*)ptr)->bFinger20;	\
		lFigner20 = ((CRManipulatorDesc*)ptr)->lFigner20;	\
		tFinger20 = ((CRManipulatorDesc*)ptr)->tFinger20;	\
		bFinger21 = ((CRManipulatorDesc*)ptr)->bFinger21;	\
		lFinger21 = ((CRManipulatorDesc*)ptr)->lFinger21;	\
		tFinger21 = ((CRManipulatorDesc*)ptr)->tFinger21;	\
		bFinger22 = ((CRManipulatorDesc*)ptr)->bFinger22;	\
		lFigner22 = ((CRManipulatorDesc*)ptr)->lFigner22;	\
		tFinger22 = ((CRManipulatorDesc*)ptr)->tFinger22;	\
		bFinger23 = ((CRManipulatorDesc*)ptr)->bFinger23;	\
		lFinger23 = ((CRManipulatorDesc*)ptr)->lFinger23;	\
		tFigner23 = ((CRManipulatorDesc*)ptr)->tFigner23;	\
		bFinger30 = ((CRManipulatorDesc*)ptr)->bFinger30;	\
		lFigner30 = ((CRManipulatorDesc*)ptr)->lFigner30;	\
		tFinger30 = ((CRManipulatorDesc*)ptr)->tFinger30;	\
		bFinger31 = ((CRManipulatorDesc*)ptr)->bFinger31;	\
		lFinger31 = ((CRManipulatorDesc*)ptr)->lFinger31;	\
		tFinger31 = ((CRManipulatorDesc*)ptr)->tFinger31;	\
		bFinger32 = ((CRManipulatorDesc*)ptr)->bFinger32;	\
		lFigner32 = ((CRManipulatorDesc*)ptr)->lFigner32;	\
		tFinger32 = ((CRManipulatorDesc*)ptr)->tFinger32;	\
		bFinger33 = ((CRManipulatorDesc*)ptr)->bFinger33;	\
		lFinger33 = ((CRManipulatorDesc*)ptr)->lFinger33;	\
		tFigner33 = ((CRManipulatorDesc*)ptr)->tFigner33;	\
		bFinger40 = ((CRManipulatorDesc*)ptr)->bFinger40;	\
		lFigner40 = ((CRManipulatorDesc*)ptr)->lFigner40;	\
		tFinger40 = ((CRManipulatorDesc*)ptr)->tFinger40;	\
		bFinger41 = ((CRManipulatorDesc*)ptr)->bFinger41;	\
		lFinger41 = ((CRManipulatorDesc*)ptr)->lFinger41;	\
		tFinger41 = ((CRManipulatorDesc*)ptr)->tFinger41;	\
		bFinger42 = ((CRManipulatorDesc*)ptr)->bFinger42;	\
		lFigner42 = ((CRManipulatorDesc*)ptr)->lFigner42;	\
		tFinger42 = ((CRManipulatorDesc*)ptr)->tFinger42;	\
		bFinger43 = ((CRManipulatorDesc*)ptr)->bFinger43;	\
		lFinger43 = ((CRManipulatorDesc*)ptr)->lFinger43;	\
		tFigner43 = ((CRManipulatorDesc*)ptr)->tFigner43;	\
		spring00 = ((CRManipulatorDesc*)ptr)->spring00;	\
		damper00 = ((CRManipulatorDesc*)ptr)->damper00;	\
		spring01 = ((CRManipulatorDesc*)ptr)->spring01;	\
		damper01 = ((CRManipulatorDesc*)ptr)->damper01;	\
		spring02 = ((CRManipulatorDesc*)ptr)->spring02;	\
		damper02 = ((CRManipulatorDesc*)ptr)->damper02;	\
		spring10 = ((CRManipulatorDesc*)ptr)->spring10;	\
		damper10 = ((CRManipulatorDesc*)ptr)->damper10;	\
		spring11 = ((CRManipulatorDesc*)ptr)->spring11;	\
		damper11 = ((CRManipulatorDesc*)ptr)->damper11;	\
		spring12 = ((CRManipulatorDesc*)ptr)->spring12;	\
		damper12 = ((CRManipulatorDesc*)ptr)->damper12;	\
		spring13 = ((CRManipulatorDesc*)ptr)->spring13;	\
		damper13 = ((CRManipulatorDesc*)ptr)->damper13;	\
		spring20 = ((CRManipulatorDesc*)ptr)->spring20;	\
		damper20 = ((CRManipulatorDesc*)ptr)->damper20;	\
		spring21 = ((CRManipulatorDesc*)ptr)->spring21;	\
		damper21 = ((CRManipulatorDesc*)ptr)->damper21;	\
		spring22 = ((CRManipulatorDesc*)ptr)->spring22;	\
		damper22 = ((CRManipulatorDesc*)ptr)->damper22;	\
		spring23 = ((CRManipulatorDesc*)ptr)->spring23;	\
		damper23 = ((CRManipulatorDesc*)ptr)->damper23;	\
		spring30 = ((CRManipulatorDesc*)ptr)->spring30;	\
		damper30 = ((CRManipulatorDesc*)ptr)->damper30;	\
		spring31 = ((CRManipulatorDesc*)ptr)->spring31;	\
		damper31 = ((CRManipulatorDesc*)ptr)->damper31;	\
		spring32 = ((CRManipulatorDesc*)ptr)->spring32;	\
		damper32 = ((CRManipulatorDesc*)ptr)->damper32;	\
		spring33 = ((CRManipulatorDesc*)ptr)->spring33;	\
		damper33 = ((CRManipulatorDesc*)ptr)->damper33;	\
		spring40 = ((CRManipulatorDesc*)ptr)->spring40;	\
		damper40 = ((CRManipulatorDesc*)ptr)->damper40;	\
		spring41 = ((CRManipulatorDesc*)ptr)->spring41;	\
		damper41 = ((CRManipulatorDesc*)ptr)->damper41;	\
		spring42 = ((CRManipulatorDesc*)ptr)->spring42;	\
		damper42 = ((CRManipulatorDesc*)ptr)->damper42;	\
		spring43 = ((CRManipulatorDesc*)ptr)->spring43;	\
		damper43 = ((CRManipulatorDesc*)ptr)->damper43;	\
		range01 = ((CRManipulatorDesc*)ptr)->range01;	\
		range02 = ((CRManipulatorDesc*)ptr)->range02;	\
		range11 = ((CRManipulatorDesc*)ptr)->range11;	\
		range12 = ((CRManipulatorDesc*)ptr)->range12;	\
		range13 = ((CRManipulatorDesc*)ptr)->range13;	\
		range21 = ((CRManipulatorDesc*)ptr)->range21;	\
		range22 = ((CRManipulatorDesc*)ptr)->range22;	\
		range23 = ((CRManipulatorDesc*)ptr)->range23;	\
		range31 = ((CRManipulatorDesc*)ptr)->range31;	\
		range32 = ((CRManipulatorDesc*)ptr)->range32;	\
		range33 = ((CRManipulatorDesc*)ptr)->range33;	\
		range41 = ((CRManipulatorDesc*)ptr)->range41;	\
		range42 = ((CRManipulatorDesc*)ptr)->range42;	\
		range43 = ((CRManipulatorDesc*)ptr)->range43;	\
		goalFinger0 = ((CRManipulatorDesc*)ptr)->goalFinger0;	\
		goalFinger1 = ((CRManipulatorDesc*)ptr)->goalFinger1;	\
		goalFinger2 = ((CRManipulatorDesc*)ptr)->goalFinger2;	\
		goalFigner3 = ((CRManipulatorDesc*)ptr)->goalFigner3;	\
		goalFinger4 = ((CRManipulatorDesc*)ptr)->goalFinger4;	\
		limitSwing00 = ((CRManipulatorDesc*)ptr)->limitSwing00;	\
		limitSwing10 = ((CRManipulatorDesc*)ptr)->limitSwing10;	\
		limitSwing20 = ((CRManipulatorDesc*)ptr)->limitSwing20;	\
		limitSwing30 = ((CRManipulatorDesc*)ptr)->limitSwing30;	\
		limitSwing40 = ((CRManipulatorDesc*)ptr)->limitSwing40;	\
		limitTwist00 = ((CRManipulatorDesc*)ptr)->limitTwist00;	\
		limitTwist10 = ((CRManipulatorDesc*)ptr)->limitTwist10;	\
		limitTwist20 = ((CRManipulatorDesc*)ptr)->limitTwist20;	\
		limitTwist30 = ((CRManipulatorDesc*)ptr)->limitTwist30;	\
		limitTwist40 = ((CRManipulatorDesc*)ptr)->limitTwist40;	\
		fMaxFinger0 = ((CRManipulatorDesc*)ptr)->fMaxFinger0;	\
		fMaxFinger1 = ((CRManipulatorDesc*)ptr)->fMaxFinger1;	\
		fMaxFinger2 = ((CRManipulatorDesc*)ptr)->fMaxFinger2;	\
		fMaxFinger3 = ((CRManipulatorDesc*)ptr)->fMaxFinger3;	\
		fMaxFinger4 = ((CRManipulatorDesc*)ptr)->fMaxFinger4;	\
		materialMu = ((CRManipulatorDesc*)ptr)->materialMu;	\
		dynamicalMode = ((CRManipulatorDesc*)ptr)->dynamicalMode;	\
		flagFMax = ((CRManipulatorDesc*)ptr)->flagFMax;	\
		flagRange = ((CRManipulatorDesc*)ptr)->flagRange;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBody::GetDesc((CRBodyDesc*)(CRManipulatorDesc*)ptr);	\
		((CRManipulatorDesc*)ptr)->soNSolids = soNSolids;	\
		((CRManipulatorDesc*)ptr)->joNBallJoints = joNBallJoints;	\
		((CRManipulatorDesc*)ptr)->joNHingeJoints = joNHingeJoints;	\
		((CRManipulatorDesc*)ptr)->joNJoints = joNJoints;	\
		((CRManipulatorDesc*)ptr)->bRoot = bRoot;	\
		((CRManipulatorDesc*)ptr)->bFinger00 = bFinger00;	\
		((CRManipulatorDesc*)ptr)->lFinger00 = lFinger00;	\
		((CRManipulatorDesc*)ptr)->tFinger00 = tFinger00;	\
		((CRManipulatorDesc*)ptr)->bFinger01 = bFinger01;	\
		((CRManipulatorDesc*)ptr)->lFinger01 = lFinger01;	\
		((CRManipulatorDesc*)ptr)->tFinger01 = tFinger01;	\
		((CRManipulatorDesc*)ptr)->bFinger02 = bFinger02;	\
		((CRManipulatorDesc*)ptr)->lFinger02 = lFinger02;	\
		((CRManipulatorDesc*)ptr)->tFinger02 = tFinger02;	\
		((CRManipulatorDesc*)ptr)->bFinger10 = bFinger10;	\
		((CRManipulatorDesc*)ptr)->lFigner10 = lFigner10;	\
		((CRManipulatorDesc*)ptr)->tFinger10 = tFinger10;	\
		((CRManipulatorDesc*)ptr)->bFinger11 = bFinger11;	\
		((CRManipulatorDesc*)ptr)->lFinger11 = lFinger11;	\
		((CRManipulatorDesc*)ptr)->tFinger11 = tFinger11;	\
		((CRManipulatorDesc*)ptr)->bFinger12 = bFinger12;	\
		((CRManipulatorDesc*)ptr)->lFigner12 = lFigner12;	\
		((CRManipulatorDesc*)ptr)->tFinger12 = tFinger12;	\
		((CRManipulatorDesc*)ptr)->bFinger13 = bFinger13;	\
		((CRManipulatorDesc*)ptr)->lFinger13 = lFinger13;	\
		((CRManipulatorDesc*)ptr)->tFigner13 = tFigner13;	\
		((CRManipulatorDesc*)ptr)->bFinger20 = bFinger20;	\
		((CRManipulatorDesc*)ptr)->lFigner20 = lFigner20;	\
		((CRManipulatorDesc*)ptr)->tFinger20 = tFinger20;	\
		((CRManipulatorDesc*)ptr)->bFinger21 = bFinger21;	\
		((CRManipulatorDesc*)ptr)->lFinger21 = lFinger21;	\
		((CRManipulatorDesc*)ptr)->tFinger21 = tFinger21;	\
		((CRManipulatorDesc*)ptr)->bFinger22 = bFinger22;	\
		((CRManipulatorDesc*)ptr)->lFigner22 = lFigner22;	\
		((CRManipulatorDesc*)ptr)->tFinger22 = tFinger22;	\
		((CRManipulatorDesc*)ptr)->bFinger23 = bFinger23;	\
		((CRManipulatorDesc*)ptr)->lFinger23 = lFinger23;	\
		((CRManipulatorDesc*)ptr)->tFigner23 = tFigner23;	\
		((CRManipulatorDesc*)ptr)->bFinger30 = bFinger30;	\
		((CRManipulatorDesc*)ptr)->lFigner30 = lFigner30;	\
		((CRManipulatorDesc*)ptr)->tFinger30 = tFinger30;	\
		((CRManipulatorDesc*)ptr)->bFinger31 = bFinger31;	\
		((CRManipulatorDesc*)ptr)->lFinger31 = lFinger31;	\
		((CRManipulatorDesc*)ptr)->tFinger31 = tFinger31;	\
		((CRManipulatorDesc*)ptr)->bFinger32 = bFinger32;	\
		((CRManipulatorDesc*)ptr)->lFigner32 = lFigner32;	\
		((CRManipulatorDesc*)ptr)->tFinger32 = tFinger32;	\
		((CRManipulatorDesc*)ptr)->bFinger33 = bFinger33;	\
		((CRManipulatorDesc*)ptr)->lFinger33 = lFinger33;	\
		((CRManipulatorDesc*)ptr)->tFigner33 = tFigner33;	\
		((CRManipulatorDesc*)ptr)->bFinger40 = bFinger40;	\
		((CRManipulatorDesc*)ptr)->lFigner40 = lFigner40;	\
		((CRManipulatorDesc*)ptr)->tFinger40 = tFinger40;	\
		((CRManipulatorDesc*)ptr)->bFinger41 = bFinger41;	\
		((CRManipulatorDesc*)ptr)->lFinger41 = lFinger41;	\
		((CRManipulatorDesc*)ptr)->tFinger41 = tFinger41;	\
		((CRManipulatorDesc*)ptr)->bFinger42 = bFinger42;	\
		((CRManipulatorDesc*)ptr)->lFigner42 = lFigner42;	\
		((CRManipulatorDesc*)ptr)->tFinger42 = tFinger42;	\
		((CRManipulatorDesc*)ptr)->bFinger43 = bFinger43;	\
		((CRManipulatorDesc*)ptr)->lFinger43 = lFinger43;	\
		((CRManipulatorDesc*)ptr)->tFigner43 = tFigner43;	\
		((CRManipulatorDesc*)ptr)->spring00 = spring00;	\
		((CRManipulatorDesc*)ptr)->damper00 = damper00;	\
		((CRManipulatorDesc*)ptr)->spring01 = spring01;	\
		((CRManipulatorDesc*)ptr)->damper01 = damper01;	\
		((CRManipulatorDesc*)ptr)->spring02 = spring02;	\
		((CRManipulatorDesc*)ptr)->damper02 = damper02;	\
		((CRManipulatorDesc*)ptr)->spring10 = spring10;	\
		((CRManipulatorDesc*)ptr)->damper10 = damper10;	\
		((CRManipulatorDesc*)ptr)->spring11 = spring11;	\
		((CRManipulatorDesc*)ptr)->damper11 = damper11;	\
		((CRManipulatorDesc*)ptr)->spring12 = spring12;	\
		((CRManipulatorDesc*)ptr)->damper12 = damper12;	\
		((CRManipulatorDesc*)ptr)->spring13 = spring13;	\
		((CRManipulatorDesc*)ptr)->damper13 = damper13;	\
		((CRManipulatorDesc*)ptr)->spring20 = spring20;	\
		((CRManipulatorDesc*)ptr)->damper20 = damper20;	\
		((CRManipulatorDesc*)ptr)->spring21 = spring21;	\
		((CRManipulatorDesc*)ptr)->damper21 = damper21;	\
		((CRManipulatorDesc*)ptr)->spring22 = spring22;	\
		((CRManipulatorDesc*)ptr)->damper22 = damper22;	\
		((CRManipulatorDesc*)ptr)->spring23 = spring23;	\
		((CRManipulatorDesc*)ptr)->damper23 = damper23;	\
		((CRManipulatorDesc*)ptr)->spring30 = spring30;	\
		((CRManipulatorDesc*)ptr)->damper30 = damper30;	\
		((CRManipulatorDesc*)ptr)->spring31 = spring31;	\
		((CRManipulatorDesc*)ptr)->damper31 = damper31;	\
		((CRManipulatorDesc*)ptr)->spring32 = spring32;	\
		((CRManipulatorDesc*)ptr)->damper32 = damper32;	\
		((CRManipulatorDesc*)ptr)->spring33 = spring33;	\
		((CRManipulatorDesc*)ptr)->damper33 = damper33;	\
		((CRManipulatorDesc*)ptr)->spring40 = spring40;	\
		((CRManipulatorDesc*)ptr)->damper40 = damper40;	\
		((CRManipulatorDesc*)ptr)->spring41 = spring41;	\
		((CRManipulatorDesc*)ptr)->damper41 = damper41;	\
		((CRManipulatorDesc*)ptr)->spring42 = spring42;	\
		((CRManipulatorDesc*)ptr)->damper42 = damper42;	\
		((CRManipulatorDesc*)ptr)->spring43 = spring43;	\
		((CRManipulatorDesc*)ptr)->damper43 = damper43;	\
		((CRManipulatorDesc*)ptr)->range01 = range01;	\
		((CRManipulatorDesc*)ptr)->range02 = range02;	\
		((CRManipulatorDesc*)ptr)->range11 = range11;	\
		((CRManipulatorDesc*)ptr)->range12 = range12;	\
		((CRManipulatorDesc*)ptr)->range13 = range13;	\
		((CRManipulatorDesc*)ptr)->range21 = range21;	\
		((CRManipulatorDesc*)ptr)->range22 = range22;	\
		((CRManipulatorDesc*)ptr)->range23 = range23;	\
		((CRManipulatorDesc*)ptr)->range31 = range31;	\
		((CRManipulatorDesc*)ptr)->range32 = range32;	\
		((CRManipulatorDesc*)ptr)->range33 = range33;	\
		((CRManipulatorDesc*)ptr)->range41 = range41;	\
		((CRManipulatorDesc*)ptr)->range42 = range42;	\
		((CRManipulatorDesc*)ptr)->range43 = range43;	\
		((CRManipulatorDesc*)ptr)->goalFinger0 = goalFinger0;	\
		((CRManipulatorDesc*)ptr)->goalFinger1 = goalFinger1;	\
		((CRManipulatorDesc*)ptr)->goalFinger2 = goalFinger2;	\
		((CRManipulatorDesc*)ptr)->goalFigner3 = goalFigner3;	\
		((CRManipulatorDesc*)ptr)->goalFinger4 = goalFinger4;	\
		((CRManipulatorDesc*)ptr)->limitSwing00 = limitSwing00;	\
		((CRManipulatorDesc*)ptr)->limitSwing10 = limitSwing10;	\
		((CRManipulatorDesc*)ptr)->limitSwing20 = limitSwing20;	\
		((CRManipulatorDesc*)ptr)->limitSwing30 = limitSwing30;	\
		((CRManipulatorDesc*)ptr)->limitSwing40 = limitSwing40;	\
		((CRManipulatorDesc*)ptr)->limitTwist00 = limitTwist00;	\
		((CRManipulatorDesc*)ptr)->limitTwist10 = limitTwist10;	\
		((CRManipulatorDesc*)ptr)->limitTwist20 = limitTwist20;	\
		((CRManipulatorDesc*)ptr)->limitTwist30 = limitTwist30;	\
		((CRManipulatorDesc*)ptr)->limitTwist40 = limitTwist40;	\
		((CRManipulatorDesc*)ptr)->fMaxFinger0 = fMaxFinger0;	\
		((CRManipulatorDesc*)ptr)->fMaxFinger1 = fMaxFinger1;	\
		((CRManipulatorDesc*)ptr)->fMaxFinger2 = fMaxFinger2;	\
		((CRManipulatorDesc*)ptr)->fMaxFinger3 = fMaxFinger3;	\
		((CRManipulatorDesc*)ptr)->fMaxFinger4 = fMaxFinger4;	\
		((CRManipulatorDesc*)ptr)->materialMu = materialMu;	\
		((CRManipulatorDesc*)ptr)->dynamicalMode = dynamicalMode;	\
		((CRManipulatorDesc*)ptr)->flagFMax = flagFMax;	\
		((CRManipulatorDesc*)ptr)->flagRange = flagRange;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTrunkFootHumanBodyDesc \
protected:\
	double	waistHeight;	\
	double	waistBreadth;	\
	double	waistThickness;	\
	double	chestHeight;	\
	double	chestBreadth;	\
	double	chestThickness;	\
	double	neckLength;	\
	double	headDiameter;	\
	double	footLength;	\
	double	footBreadth;	\
	double	footThickness;	\
	double	springWaistChest;	\
	double	damperWaistChest;	\
	double	springChestHead;	\
	double	damperChestHead;	\
	Vec2d	rangeWaistChest;	\
	Vec2d	rangeChestHead;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRTrunkFootHumanBodyDesc*)ptr);	\
		waistHeight = ((CRTrunkFootHumanBodyDesc*)ptr)->waistHeight;	\
		waistBreadth = ((CRTrunkFootHumanBodyDesc*)ptr)->waistBreadth;	\
		waistThickness = ((CRTrunkFootHumanBodyDesc*)ptr)->waistThickness;	\
		chestHeight = ((CRTrunkFootHumanBodyDesc*)ptr)->chestHeight;	\
		chestBreadth = ((CRTrunkFootHumanBodyDesc*)ptr)->chestBreadth;	\
		chestThickness = ((CRTrunkFootHumanBodyDesc*)ptr)->chestThickness;	\
		neckLength = ((CRTrunkFootHumanBodyDesc*)ptr)->neckLength;	\
		headDiameter = ((CRTrunkFootHumanBodyDesc*)ptr)->headDiameter;	\
		footLength = ((CRTrunkFootHumanBodyDesc*)ptr)->footLength;	\
		footBreadth = ((CRTrunkFootHumanBodyDesc*)ptr)->footBreadth;	\
		footThickness = ((CRTrunkFootHumanBodyDesc*)ptr)->footThickness;	\
		springWaistChest = ((CRTrunkFootHumanBodyDesc*)ptr)->springWaistChest;	\
		damperWaistChest = ((CRTrunkFootHumanBodyDesc*)ptr)->damperWaistChest;	\
		springChestHead = ((CRTrunkFootHumanBodyDesc*)ptr)->springChestHead;	\
		damperChestHead = ((CRTrunkFootHumanBodyDesc*)ptr)->damperChestHead;	\
		rangeWaistChest = ((CRTrunkFootHumanBodyDesc*)ptr)->rangeWaistChest;	\
		rangeChestHead = ((CRTrunkFootHumanBodyDesc*)ptr)->rangeChestHead;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBody::GetDesc((CRBodyDesc*)(CRTrunkFootHumanBodyDesc*)ptr);	\
		((CRTrunkFootHumanBodyDesc*)ptr)->waistHeight = waistHeight;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->waistBreadth = waistBreadth;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->waistThickness = waistThickness;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->chestHeight = chestHeight;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->chestBreadth = chestBreadth;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->chestThickness = chestThickness;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->neckLength = neckLength;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->headDiameter = headDiameter;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->footLength = footLength;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->footBreadth = footBreadth;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->footThickness = footThickness;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->springWaistChest = springWaistChest;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->damperWaistChest = damperWaistChest;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->springChestHead = springChestHead;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->damperChestHead = damperChestHead;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->rangeWaistChest = rangeWaistChest;	\
		((CRTrunkFootHumanBodyDesc*)ptr)->rangeChestHead = rangeChestHead;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRFourLegsTinyAnimalBodyDesc \
protected:\
	double	bodyHeight;	\
	double	bodyBreadth;	\
	double	bodyThickness;	\
	double	frontLegsBreadth;	\
	double	frontLegsHeight;	\
	double	frontLegsThickness;	\
	double	rearLegsBreadth;	\
	double	rearLegsHeight;	\
	double	rearLegsThickness;	\
	double	springFront;	\
	double	damperFront;	\
	double	springRear;	\
	double	damperRear;	\
	Vec2d	rangeFrontSwing;	\
	Vec2d	rangeFrontTwist;	\
	Vec2d	rangeRearSwing;	\
	Vec2d	rangeRearTwist;	\
	bool	noLegs;	\
	bool	noHead;	\
	bool	onlyOneLeg;	\
	bool	hingeDebug;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRFourLegsTinyAnimalBodyDesc*)ptr);	\
		bodyHeight = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyHeight;	\
		bodyBreadth = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyBreadth;	\
		bodyThickness = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyThickness;	\
		frontLegsBreadth = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsBreadth;	\
		frontLegsHeight = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsHeight;	\
		frontLegsThickness = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsThickness;	\
		rearLegsBreadth = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsBreadth;	\
		rearLegsHeight = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsHeight;	\
		rearLegsThickness = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsThickness;	\
		springFront = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->springFront;	\
		damperFront = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->damperFront;	\
		springRear = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->springRear;	\
		damperRear = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->damperRear;	\
		rangeFrontSwing = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeFrontSwing;	\
		rangeFrontTwist = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeFrontTwist;	\
		rangeRearSwing = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeRearSwing;	\
		rangeRearTwist = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeRearTwist;	\
		noLegs = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->noLegs;	\
		noHead = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->noHead;	\
		onlyOneLeg = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->onlyOneLeg;	\
		hingeDebug = ((CRFourLegsTinyAnimalBodyDesc*)ptr)->hingeDebug;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBody::GetDesc((CRBodyDesc*)(CRFourLegsTinyAnimalBodyDesc*)ptr);	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyHeight = bodyHeight;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyBreadth = bodyBreadth;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->bodyThickness = bodyThickness;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsBreadth = frontLegsBreadth;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsHeight = frontLegsHeight;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->frontLegsThickness = frontLegsThickness;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsBreadth = rearLegsBreadth;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsHeight = rearLegsHeight;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rearLegsThickness = rearLegsThickness;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->springFront = springFront;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->damperFront = damperFront;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->springRear = springRear;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->damperRear = damperRear;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeFrontSwing = rangeFrontSwing;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeFrontTwist = rangeFrontTwist;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeRearSwing = rangeRearSwing;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->rangeRearTwist = rangeRearTwist;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->noLegs = noLegs;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->noHead = noHead;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->onlyOneLeg = onlyOneLeg;	\
		((CRFourLegsTinyAnimalBodyDesc*)ptr)->hingeDebug = hingeDebug;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerState \
protected:\
	enum Spr::CREyeControllerState::ControlState	controlState;	\
public:\
	virtual void SetState(const void* ptr){ \
		controlState = ((CREyeControllerState*)ptr)->controlState;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((CREyeControllerState*)ptr)->controlState = controlState;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::SetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::GetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRNeckControllerState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRNeckControllerDesc \
protected:\
	float	lowerAttractiveness;	\
	float	upperAttractiveness;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRNeckControllerDesc*)ptr);	\
		CRNeckController::SetState((CRNeckControllerState*)(CRNeckControllerDesc*)ptr);	\
		lowerAttractiveness = ((CRNeckControllerDesc*)ptr)->lowerAttractiveness;	\
		upperAttractiveness = ((CRNeckControllerDesc*)ptr)->upperAttractiveness;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRNeckControllerDesc*)ptr);	\
		CRNeckController::GetState((CRNeckControllerState*)(CRNeckControllerDesc*)ptr);	\
		((CRNeckControllerDesc*)ptr)->lowerAttractiveness = lowerAttractiveness;	\
		((CRNeckControllerDesc*)ptr)->upperAttractiveness = upperAttractiveness;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRReachingControllerDesc \
protected:\
	Spr::PHSolidIf*	solid;	\
	Vec3f	reachPos;	\
	Quaterniond	fixOri;	\
	float	limitForce;	\
	float	springPos;	\
	float	damperPos;	\
	float	springOri;	\
	float	damperOri;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRReachingControllerDesc*)ptr);	\
		solid = ((CRReachingControllerDesc*)ptr)->solid;	\
		reachPos = ((CRReachingControllerDesc*)ptr)->reachPos;	\
		fixOri = ((CRReachingControllerDesc*)ptr)->fixOri;	\
		limitForce = ((CRReachingControllerDesc*)ptr)->limitForce;	\
		springPos = ((CRReachingControllerDesc*)ptr)->springPos;	\
		damperPos = ((CRReachingControllerDesc*)ptr)->damperPos;	\
		springOri = ((CRReachingControllerDesc*)ptr)->springOri;	\
		damperOri = ((CRReachingControllerDesc*)ptr)->damperOri;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRReachingControllerDesc*)ptr);	\
		((CRReachingControllerDesc*)ptr)->solid = solid;	\
		((CRReachingControllerDesc*)ptr)->reachPos = reachPos;	\
		((CRReachingControllerDesc*)ptr)->fixOri = fixOri;	\
		((CRReachingControllerDesc*)ptr)->limitForce = limitForce;	\
		((CRReachingControllerDesc*)ptr)->springPos = springPos;	\
		((CRReachingControllerDesc*)ptr)->damperPos = damperPos;	\
		((CRReachingControllerDesc*)ptr)->springOri = springOri;	\
		((CRReachingControllerDesc*)ptr)->damperOri = damperOri;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::SetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::GetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRAttentionControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRAttentionControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRAttentionControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTryStandingUpControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRTryStandingUpControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRTryStandingUpControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRWalkingControllerDesc \
protected:\
	double	paramLdx;	\
	double	paramLdz;	\
	double	maxHalfStride;	\
	double	height;	\
	double	minCycleRate;	\
	double	maxDSRate;	\
	double	FootLength;	\
	double	MaxFootLength;	\
	double	MinFootLength;	\
	double	MaxRoGround;	\
	double	MaxRoLandingSiteFront;	\
	double	MaxRoLandingSiteSide;	\
	double	MaxRoLandingSiteBack;	\
	double	MaxRoConstraint;	\
	double	MaxFootSpeedFront;	\
	double	MaxFootSpeedSide;	\
	double	MaxFootSpeedBack;	\
	double	MaxFootAccelerationFront;	\
	double	MaxFootAccelerationSide;	\
	double	MaxFootAccelerationBack;	\
	double	DoubleSupportLimitRate;	\
	double	LimitChange;	\
	double	pi;	\
	double	footsize;	\
	double	miu;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRWalkingControllerDesc*)ptr);	\
		paramLdx = ((CRWalkingControllerDesc*)ptr)->paramLdx;	\
		paramLdz = ((CRWalkingControllerDesc*)ptr)->paramLdz;	\
		maxHalfStride = ((CRWalkingControllerDesc*)ptr)->maxHalfStride;	\
		height = ((CRWalkingControllerDesc*)ptr)->height;	\
		minCycleRate = ((CRWalkingControllerDesc*)ptr)->minCycleRate;	\
		maxDSRate = ((CRWalkingControllerDesc*)ptr)->maxDSRate;	\
		FootLength = ((CRWalkingControllerDesc*)ptr)->FootLength;	\
		MaxFootLength = ((CRWalkingControllerDesc*)ptr)->MaxFootLength;	\
		MinFootLength = ((CRWalkingControllerDesc*)ptr)->MinFootLength;	\
		MaxRoGround = ((CRWalkingControllerDesc*)ptr)->MaxRoGround;	\
		MaxRoLandingSiteFront = ((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteFront;	\
		MaxRoLandingSiteSide = ((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteSide;	\
		MaxRoLandingSiteBack = ((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteBack;	\
		MaxRoConstraint = ((CRWalkingControllerDesc*)ptr)->MaxRoConstraint;	\
		MaxFootSpeedFront = ((CRWalkingControllerDesc*)ptr)->MaxFootSpeedFront;	\
		MaxFootSpeedSide = ((CRWalkingControllerDesc*)ptr)->MaxFootSpeedSide;	\
		MaxFootSpeedBack = ((CRWalkingControllerDesc*)ptr)->MaxFootSpeedBack;	\
		MaxFootAccelerationFront = ((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationFront;	\
		MaxFootAccelerationSide = ((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationSide;	\
		MaxFootAccelerationBack = ((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationBack;	\
		DoubleSupportLimitRate = ((CRWalkingControllerDesc*)ptr)->DoubleSupportLimitRate;	\
		LimitChange = ((CRWalkingControllerDesc*)ptr)->LimitChange;	\
		pi = ((CRWalkingControllerDesc*)ptr)->pi;	\
		footsize = ((CRWalkingControllerDesc*)ptr)->footsize;	\
		miu = ((CRWalkingControllerDesc*)ptr)->miu;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRWalkingControllerDesc*)ptr);	\
		((CRWalkingControllerDesc*)ptr)->paramLdx = paramLdx;	\
		((CRWalkingControllerDesc*)ptr)->paramLdz = paramLdz;	\
		((CRWalkingControllerDesc*)ptr)->maxHalfStride = maxHalfStride;	\
		((CRWalkingControllerDesc*)ptr)->height = height;	\
		((CRWalkingControllerDesc*)ptr)->minCycleRate = minCycleRate;	\
		((CRWalkingControllerDesc*)ptr)->maxDSRate = maxDSRate;	\
		((CRWalkingControllerDesc*)ptr)->FootLength = FootLength;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootLength = MaxFootLength;	\
		((CRWalkingControllerDesc*)ptr)->MinFootLength = MinFootLength;	\
		((CRWalkingControllerDesc*)ptr)->MaxRoGround = MaxRoGround;	\
		((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteFront = MaxRoLandingSiteFront;	\
		((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteSide = MaxRoLandingSiteSide;	\
		((CRWalkingControllerDesc*)ptr)->MaxRoLandingSiteBack = MaxRoLandingSiteBack;	\
		((CRWalkingControllerDesc*)ptr)->MaxRoConstraint = MaxRoConstraint;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootSpeedFront = MaxFootSpeedFront;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootSpeedSide = MaxFootSpeedSide;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootSpeedBack = MaxFootSpeedBack;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationFront = MaxFootAccelerationFront;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationSide = MaxFootAccelerationSide;	\
		((CRWalkingControllerDesc*)ptr)->MaxFootAccelerationBack = MaxFootAccelerationBack;	\
		((CRWalkingControllerDesc*)ptr)->DoubleSupportLimitRate = DoubleSupportLimitRate;	\
		((CRWalkingControllerDesc*)ptr)->LimitChange = LimitChange;	\
		((CRWalkingControllerDesc*)ptr)->pi = pi;	\
		((CRWalkingControllerDesc*)ptr)->footsize = footsize;	\
		((CRWalkingControllerDesc*)ptr)->miu = miu;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREseWalkingControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CREseWalkingControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CREseWalkingControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTravelControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRTravelControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRTravelControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGrabControllerDesc \
protected:\
	double	rateSpringSoften;	\
	double	rateDamperSoften;	\
	double	rateSpringHarden;	\
	double	rateDamperHarden;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRGrabControllerDesc*)ptr);	\
		rateSpringSoften = ((CRGrabControllerDesc*)ptr)->rateSpringSoften;	\
		rateDamperSoften = ((CRGrabControllerDesc*)ptr)->rateDamperSoften;	\
		rateSpringHarden = ((CRGrabControllerDesc*)ptr)->rateSpringHarden;	\
		rateDamperHarden = ((CRGrabControllerDesc*)ptr)->rateDamperHarden;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRGrabControllerDesc*)ptr);	\
		((CRGrabControllerDesc*)ptr)->rateSpringSoften = rateSpringSoften;	\
		((CRGrabControllerDesc*)ptr)->rateDamperSoften = rateDamperSoften;	\
		((CRGrabControllerDesc*)ptr)->rateSpringHarden = rateSpringHarden;	\
		((CRGrabControllerDesc*)ptr)->rateDamperHarden = rateDamperHarden;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRCreatureDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRInternalSceneObjectDesc \
protected:\
	char*	type;	\
	Spr::PHSolidIf*	solid;	\
	Vec3f	position;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		type = ((CRInternalSceneObjectDesc*)ptr)->type;	\
		solid = ((CRInternalSceneObjectDesc*)ptr)->solid;	\
		position = ((CRInternalSceneObjectDesc*)ptr)->position;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CRInternalSceneObjectDesc*)ptr)->type = type;	\
		((CRInternalSceneObjectDesc*)ptr)->solid = solid;	\
		((CRInternalSceneObjectDesc*)ptr)->position = position;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRISAttractiveObjectDesc \
protected:\
	float	bottomupAttr;	\
	float	topdownAttr;	\
	float	unknownAttr;	\
	float	uncertainty;	\
	float	uncertaintyIncRate;	\
	float	uncertaintyDecRate;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRInternalSceneObject::SetDesc((CRInternalSceneObjectDesc*)(CRISAttractiveObjectDesc*)ptr);	\
		bottomupAttr = ((CRISAttractiveObjectDesc*)ptr)->bottomupAttr;	\
		topdownAttr = ((CRISAttractiveObjectDesc*)ptr)->topdownAttr;	\
		unknownAttr = ((CRISAttractiveObjectDesc*)ptr)->unknownAttr;	\
		uncertainty = ((CRISAttractiveObjectDesc*)ptr)->uncertainty;	\
		uncertaintyIncRate = ((CRISAttractiveObjectDesc*)ptr)->uncertaintyIncRate;	\
		uncertaintyDecRate = ((CRISAttractiveObjectDesc*)ptr)->uncertaintyDecRate;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRInternalSceneObject::GetDesc((CRInternalSceneObjectDesc*)(CRISAttractiveObjectDesc*)ptr);	\
		((CRISAttractiveObjectDesc*)ptr)->bottomupAttr = bottomupAttr;	\
		((CRISAttractiveObjectDesc*)ptr)->topdownAttr = topdownAttr;	\
		((CRISAttractiveObjectDesc*)ptr)->unknownAttr = unknownAttr;	\
		((CRISAttractiveObjectDesc*)ptr)->uncertainty = uncertainty;	\
		((CRISAttractiveObjectDesc*)ptr)->uncertaintyIncRate = uncertaintyIncRate;	\
		((CRISAttractiveObjectDesc*)ptr)->uncertaintyDecRate = uncertaintyDecRate;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRISTravelPotentialObjectDesc \
protected:\
	Vec2f	strength;	\
	Vec2f	decay;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRInternalSceneObject::SetDesc((CRInternalSceneObjectDesc*)(CRISTravelPotentialObjectDesc*)ptr);	\
		strength = ((CRISTravelPotentialObjectDesc*)ptr)->strength;	\
		decay = ((CRISTravelPotentialObjectDesc*)ptr)->decay;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRInternalSceneObject::GetDesc((CRInternalSceneObjectDesc*)(CRISTravelPotentialObjectDesc*)ptr);	\
		((CRISTravelPotentialObjectDesc*)ptr)->strength = strength;	\
		((CRISTravelPotentialObjectDesc*)ptr)->decay = decay;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRInternalSceneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CROpticalSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRSensor::SetDesc((CRSensorDesc*)(CROpticalSensorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRSensor::GetDesc((CRSensorDesc*)(CROpticalSensorDesc*)ptr);	\
		return true;	\
	}\

