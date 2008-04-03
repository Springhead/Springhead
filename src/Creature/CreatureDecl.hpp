#define SPR_DECLMEMBEROF_CRBodyDesc \
protected:\
	enum Spr::CRBodyDesc::CRCreatureJointOrder	jointOrder;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		jointOrder = ((CRBodyDesc*)ptr)->jointOrder;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	int	soNSolids;	\
	int	joNBallJoints;	\
	int	joNHingeJoints;	\
	int	joNJoints;	\
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
	bool	dynamicalMode;	\
	double	totalMass;	\
	bool	flagFMax;	\
	bool	flagRange;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBody::SetDesc((CRBodyDesc*)(CRFourLegsAnimalBodyDesc*)ptr);	\
		soNSolids = ((CRFourLegsAnimalBodyDesc*)ptr)->soNSolids;	\
		joNBallJoints = ((CRFourLegsAnimalBodyDesc*)ptr)->joNBallJoints;	\
		joNHingeJoints = ((CRFourLegsAnimalBodyDesc*)ptr)->joNHingeJoints;	\
		joNJoints = ((CRFourLegsAnimalBodyDesc*)ptr)->joNJoints;	\
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
		dynamicalMode = ((CRFourLegsAnimalBodyDesc*)ptr)->dynamicalMode;	\
		totalMass = ((CRFourLegsAnimalBodyDesc*)ptr)->totalMass;	\
		flagFMax = ((CRFourLegsAnimalBodyDesc*)ptr)->flagFMax;	\
		flagRange = ((CRFourLegsAnimalBodyDesc*)ptr)->flagRange;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRBody::GetDesc((CRBodyDesc*)(CRFourLegsAnimalBodyDesc*)ptr);	\
		((CRFourLegsAnimalBodyDesc*)ptr)->soNSolids = soNSolids;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->joNBallJoints = joNBallJoints;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->joNHingeJoints = joNHingeJoints;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->joNJoints = joNJoints;	\
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
		((CRFourLegsAnimalBodyDesc*)ptr)->dynamicalMode = dynamicalMode;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->totalMass = totalMass;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->flagFMax = flagFMax;	\
		((CRFourLegsAnimalBodyDesc*)ptr)->flagRange = flagRange;	\
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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

#define SPR_DECLMEMBEROF_CRTrunkFootAnimalBodyDesc \
protected:\
	double	waistHeight;	\
	double	waistBreadth;	\
	double	waistThickness;	\
	double	chestHeight;	\
	double	chestBreadth;	\
	double	chestThickness;	\
	double	neckLength;	\
	float	headDiameter;	\
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
		CRBody::SetDesc((CRBodyDesc*)(CRTrunkFootAnimalBodyDesc*)ptr);	\
		waistHeight = ((CRTrunkFootAnimalBodyDesc*)ptr)->waistHeight;	\
		waistBreadth = ((CRTrunkFootAnimalBodyDesc*)ptr)->waistBreadth;	\
		waistThickness = ((CRTrunkFootAnimalBodyDesc*)ptr)->waistThickness;	\
		chestHeight = ((CRTrunkFootAnimalBodyDesc*)ptr)->chestHeight;	\
		chestBreadth = ((CRTrunkFootAnimalBodyDesc*)ptr)->chestBreadth;	\
		chestThickness = ((CRTrunkFootAnimalBodyDesc*)ptr)->chestThickness;	\
		neckLength = ((CRTrunkFootAnimalBodyDesc*)ptr)->neckLength;	\
		headDiameter = ((CRTrunkFootAnimalBodyDesc*)ptr)->headDiameter;	\
		footLength = ((CRTrunkFootAnimalBodyDesc*)ptr)->footLength;	\
		footBreadth = ((CRTrunkFootAnimalBodyDesc*)ptr)->footBreadth;	\
		footThickness = ((CRTrunkFootAnimalBodyDesc*)ptr)->footThickness;	\
		springWaistChest = ((CRTrunkFootAnimalBodyDesc*)ptr)->springWaistChest;	\
		damperWaistChest = ((CRTrunkFootAnimalBodyDesc*)ptr)->damperWaistChest;	\
		springChestHead = ((CRTrunkFootAnimalBodyDesc*)ptr)->springChestHead;	\
		damperChestHead = ((CRTrunkFootAnimalBodyDesc*)ptr)->damperChestHead;	\
		rangeWaistChest = ((CRTrunkFootAnimalBodyDesc*)ptr)->rangeWaistChest;	\
		rangeChestHead = ((CRTrunkFootAnimalBodyDesc*)ptr)->rangeChestHead;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRBody::GetDesc((CRBodyDesc*)(CRTrunkFootAnimalBodyDesc*)ptr);	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->waistHeight = waistHeight;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->waistBreadth = waistBreadth;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->waistThickness = waistThickness;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->chestHeight = chestHeight;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->chestBreadth = chestBreadth;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->chestThickness = chestThickness;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->neckLength = neckLength;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->headDiameter = headDiameter;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->footLength = footLength;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->footBreadth = footBreadth;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->footThickness = footThickness;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->springWaistChest = springWaistChest;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->damperWaistChest = damperWaistChest;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->springChestHead = springChestHead;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->damperChestHead = damperChestHead;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->rangeWaistChest = rangeWaistChest;	\
		((CRTrunkFootAnimalBodyDesc*)ptr)->rangeChestHead = rangeChestHead;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerState \
protected:\
	enum Spr::CREyeControllerState::ControlState	controlState;	\
public:\
	virtual void SetState(const void* ptr){ \
		controlState = ((CREyeControllerState*)ptr)->controlState;	\
	}\
	virtual bool GetState(const void* ptr){ \
		((CREyeControllerState*)ptr)->controlState = controlState;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::SetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRController::GetDesc((CRControllerDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::GetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRNeckControllerState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	virtual bool GetState(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::SetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRController::GetDesc((CRControllerDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::GetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRAttentionControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRAttentionControllerDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRController::GetDesc((CRControllerDesc*)(CRAttentionControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTryStandingUpControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRTryStandingUpControllerDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRController::GetDesc((CRControllerDesc*)(CREseWalkingControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTravelControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRTravelControllerDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
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
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRInternalSceneObject::GetDesc((CRInternalSceneObjectDesc*)(CRISTravelPotentialObjectDesc*)ptr);	\
		((CRISTravelPotentialObjectDesc*)ptr)->strength = strength;	\
		((CRISTravelPotentialObjectDesc*)ptr)->decay = decay;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRInternalSceneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKControlDesc \
protected:\
	Spr::PHSolidIf*	solid;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		solid = ((CRIKControlDesc*)ptr)->solid;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		((CRIKControlDesc*)ptr)->solid = solid;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKControlPosDesc \
protected:\
	Vec3d	pos;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKControl::SetDesc((CRIKControlDesc*)(CRIKControlPosDesc*)ptr);	\
		pos = ((CRIKControlPosDesc*)ptr)->pos;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKControl::GetDesc((CRIKControlDesc*)(CRIKControlPosDesc*)ptr);	\
		((CRIKControlPosDesc*)ptr)->pos = pos;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKControlOriDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKControl::SetDesc((CRIKControlDesc*)(CRIKControlOriDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKControl::GetDesc((CRIKControlDesc*)(CRIKControlOriDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovableDesc \
protected:\
	float	bias;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bias = ((CRIKMovableDesc*)ptr)->bias;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		((CRIKMovableDesc*)ptr)->bias = bias;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovableSolidPosDesc \
protected:\
	Spr::PHSolidIf*	solid;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKMovable::SetDesc((CRIKMovableDesc*)(CRIKMovableSolidPosDesc*)ptr);	\
		solid = ((CRIKMovableSolidPosDesc*)ptr)->solid;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKMovable::GetDesc((CRIKMovableDesc*)(CRIKMovableSolidPosDesc*)ptr);	\
		((CRIKMovableSolidPosDesc*)ptr)->solid = solid;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovableSolidOriDesc \
protected:\
	Spr::PHSolidIf*	solid;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKMovable::SetDesc((CRIKMovableDesc*)(CRIKMovableSolidOriDesc*)ptr);	\
		solid = ((CRIKMovableSolidOriDesc*)ptr)->solid;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKMovable::GetDesc((CRIKMovableDesc*)(CRIKMovableSolidOriDesc*)ptr);	\
		((CRIKMovableSolidOriDesc*)ptr)->solid = solid;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovableBallJointOriDesc \
protected:\
	PHBallJointIf*	joint;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKMovable::SetDesc((CRIKMovableDesc*)(CRIKMovableBallJointOriDesc*)ptr);	\
		joint = ((CRIKMovableBallJointOriDesc*)ptr)->joint;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKMovable::GetDesc((CRIKMovableDesc*)(CRIKMovableBallJointOriDesc*)ptr);	\
		((CRIKMovableBallJointOriDesc*)ptr)->joint = joint;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovable3HingeJointOriDesc \
protected:\
	Spr::PHHingeJointIf*	joint1;	\
	Spr::PHHingeJointIf*	joint2;	\
	Spr::PHHingeJointIf*	joint3;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKMovable::SetDesc((CRIKMovableDesc*)(CRIKMovable3HingeJointOriDesc*)ptr);	\
		joint1 = ((CRIKMovable3HingeJointOriDesc*)ptr)->joint1;	\
		joint2 = ((CRIKMovable3HingeJointOriDesc*)ptr)->joint2;	\
		joint3 = ((CRIKMovable3HingeJointOriDesc*)ptr)->joint3;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKMovable::GetDesc((CRIKMovableDesc*)(CRIKMovable3HingeJointOriDesc*)ptr);	\
		((CRIKMovable3HingeJointOriDesc*)ptr)->joint1 = joint1;	\
		((CRIKMovable3HingeJointOriDesc*)ptr)->joint2 = joint2;	\
		((CRIKMovable3HingeJointOriDesc*)ptr)->joint3 = joint3;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKMovableHingeJointOriDesc \
protected:\
	Spr::PHHingeJointIf*	joint;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRIKMovable::SetDesc((CRIKMovableDesc*)(CRIKMovableHingeJointOriDesc*)ptr);	\
		joint = ((CRIKMovableHingeJointOriDesc*)ptr)->joint;	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRIKMovable::GetDesc((CRIKMovableDesc*)(CRIKMovableHingeJointOriDesc*)ptr);	\
		((CRIKMovableHingeJointOriDesc*)ptr)->joint = joint;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CROpticalSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRSensor::SetDesc((CRSensorDesc*)(CROpticalSensorDesc*)ptr);	\
	}\
	virtual bool GetDesc(const void* ptr){ \
		CRSensor::GetDesc((CRSensorDesc*)(CROpticalSensorDesc*)ptr);	\
		return true;	\
	}\

