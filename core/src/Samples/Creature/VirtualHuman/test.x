xof 0302txt 0064

FWScene fwScene{
	{grScene}
	{phScene}
	FWBoneObject fbWaist { true; true; true; true; {Waist} {Chest}   {soWaist} {joWaist} }
	FWBoneObject fbChest { true; true; true; true; {Chest} {Neck}    {soChest} {joChest} }
	FWBoneObject fbNeck  { true; true; true; true; {Neck}  {NeckEnd} {soHead}  {joNeck}  }

	FWBoneObject fbUArmR { true; true; true; true; {RightShoulder} {RightElbow}  {soUArmR} {joShoulderR} }
	FWBoneObject fbUArmL { true; true; true; true; {LeftShoulder}  {LeftElbow}   {soUArmL} {joShoulderL} }
	FWBoneObject fbLArmR { true; true; true; true; {RightElbow}    {RightArmEnd} {soLArmR} {joElbowR}    }
	FWBoneObject fbLArmL { true; true; true; true; {LeftElbow}     {LeftArmEnd}  {soLArmL}  {joElbowL}   }

	FWBoneObject fbLegR  { true; true; true; true; {RightLeg} {RightLegEnd} {soLegR} {joLegR} }
	FWBoneObject fbLegL  { true; true; true; true; {LeftLeg}  {LeftLegEnd}  {soLegL} {joLegL} }
}

GRScene grScene{
	GRFrame {		#SprGRFrame.h GRFrameDesc
		#Affinef transform
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,1,1;;
		GRFrame grFrameWorld{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1;;
		}
	}
	GRLight light1{									#SprGRRender.h GRMaterialDesc
		0.9; 0.9; 0.9; 1.0;;						#Vec4f ambient
		0.5; 0.5; 0.5; 1.9;;						#Vec4f diffuse
		0.1; 0.1; 0.1; 1.0;;						#Vec4f specular
		1.0; 1.0; 1.0; 0.0;;						#Vec4f position
		10000.0;									#float range
		1.0;										#float attenuation0
		0.0;										#float attenuation1
		0.0;										#float attenuation2
		0.0; 0.0; 0.0;;								#Vec3f spotDirection
		0;											#float spotFalloff
		0;											#float spotInner
		0;											#flaot spotCutoff
	}
	
	Import{"teddy.x"}
	GRFrame grTeddy{
		#Affinef transform
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;;
		{Root}
	}
}

PHScene phScene{									#SprPHScene.h PHSceneDesc
	#SprPHScene.h PHSceneState (inherited)
		1.0e-2;										#double		timeStep
		0;											#unsigned	count
	;
	0; 0; 0;;									#Vec3f		gravity
	50;												#int		numIteration

	#
	# --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
	#

	PHSolid soBase{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		false;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.0;									#float		length
		}
	}

	PHSolid soWaist{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}
	
	PHSolid soChest{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}

	PHSolid soHead{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}
	PHIKPosCtl ikcpHead{
		;
		0; 0; -0.25;;  # pos
		{soHead}
	}

	PHSolid soUArmL{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}

	PHSolid soUArmR{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}

	PHSolid soLArmL{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}
	PHIKPosCtl ikcpLArmL{
		;
		0; 0; -0.25;;  # pos
		{soLArmL}
	}

	PHSolid soLArmR{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}
	PHIKPosCtl ikcpLArmR{
		;
		0; 0; -0.25;;  # pos
		{soLArmR}
	}

	PHSolid soLegL{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}

	PHSolid soLegR{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#-0.358368; 0.0; 0.93358; 0.0; 0.0; 0.5; 0.0;;	#1000.0; 0.0; 0.0; 0.0; 0.0; 0.5; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;

		1.0;										#double		mass
		1.0,      0.0,		0.0,					#Matrix3d	inertia	
		0.0,	  1.0,      0.0,
		0.0,	  0.0,		1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		CDRoundCone {
			#SprCDShape.h CDShapeDesc
				#SprCDShape.h PHMaterial
					0.4;							#float		mu
					0.4;							#float		mu0
					0.0;							#float		e
					0.0;							#float      density    
					0.0;							#float      springK
					0.0;							#float      damperD
					0.0;							#float		vibA
					0.0;							#float		vibB
					0.0;							#float		vibW
				;
			;
			0.2; 0.2;;								#Vec2d		radius
			0.5;									#float		length
		}
	}

	#
	# --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
	#

	PHBallJoint joWaist{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soBase}
		{soWaist}
	}
	PHIKBallJoint {
		3.0; 1000.0; 100.0; ; # bias, spring, damper
		{joWaist}
		{ikcpHead}
		{ikcpLArmL}
		{ikcpLArmR}
	}

	PHBallJoint joChest{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soWaist}
		{soChest}
	}
	PHIKBallJoint {
		2.0; 1000.0; 100.0; ; # bias, spring, damper
		{joChest}
		{ikcpHead}
		{ikcpLArmL}
		{ikcpLArmR}
	}

	PHBallJoint joNeck{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soChest}
		{soHead}
	}
	PHIKBallJoint {
		1.0; 1000.0; 100.0; ; # bias, spring, damper
		{joNeck}
		{ikcpHead}
	}

	PHBallJoint joShoulderL{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soChest}
		{soUArmL}
	}
	PHIKBallJoint {
		2.0; 1000.0; 100.0; ; # bias, spring, damper
		{joShoulderL}
		{ikcpLArmL}
	}

	PHBallJoint joShoulderR{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soChest}
		{soUArmR}
	}
	PHIKBallJoint {
		2.0; 1000.0; 100.0; ; # bias, spring, damper
		{joShoulderR}
		{ikcpLArmR}
	}

	PHBallJoint joElbowL{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soUArmL}
		{soLArmL}
	}
	PHIKBallJoint {
		1.0; 1000.0; 100.0; ; # bias, spring, damper
		{joElbowL}
		{ikcpLArmL}
	}

	PHBallJoint joElbowR{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soUArmR}
		{soLArmR}
	}
	PHIKBallJoint {
		1.0; 1000.0; 100.0; ; # bias, spring, damper
		{joElbowR}
		{ikcpLArmR}
	}
	
	PHBallJoint joLegL{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soBase}
		{soLegL}
	}

	PHBallJoint joLegR{
		true; # enabled
		1; 0; 0; 0; 0; 0; 0;; # poseSocket (To be Overwritten by FWBoneObject)
		1; 0; 0; 0; 0; 0; 0;; # posePlug   (To be Overwritten by FWBoneObject)
		;
		1; # mode
		;
		100.0;  1.0;   # spring, damper
		-3.2e+38; 3.0e+38;; # limitSwing
		-3.2e+38; 3.0e+38;; # limitTwist
		0; 0; 1;; # limitDir
		0; 0; 0; 1;; # goal
		0; 0; 0;; # desiredVel
		0; 0; 0;; # offset
		0; 0; 0;; # torque
		3.2e+38; # fMax
		-3.2e+38; # fMin
		3.2e+38; 3.2e+38;; # PoleTwist

		{soBase}
		{soLegR}
	}
}

CRCreature creature1 {
	{phScene}
	
	CRBody creature1Body {
		CRIKSolid { "LeftHand";  ; {soLArmL} {ikcpLArmL} }
		CRIKSolid { "RightHand"; ; {soLArmR} {ikcpLArmR} }
	}

	CRReachingController reachLH {
	}
	CRReachingController reachRH {
	}
}
