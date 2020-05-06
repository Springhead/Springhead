xof 0302txt 0064

FWScene fwScene{
	{grScene}
	{phScene}

	FWObject fwCookie{
		{grFrameCookie}
		{soCookie}
	}
	FWObject fwTable{
		{grFrameTable}
		{soTable}	
	}
}

Import{"grCookie.x"}
Import{"grTable.x"}	

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
	GRFrame grFrameCookie{
		#Affinef transform
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;;
		{grCookie}	
	}
	GRFrame grFrameTable{
		#Affinef transform
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;;
		{grTable}
	}
}

Import{"phCookie.x"}
Import{"phTable.x"}

PHScene phScene{									#SprPHScene.h PHSceneDesc
	#SprPHScene.h PHSceneState (inherited)
		0.01;										#double		timeStep
		0;											#unsigned	count
	;
	0.0; -9.8; 0;;									#Vec3f		gravity
	0.0;											#double		airResistanceRate
	50;												#int		numIteration
	
	PHSolid soCookie{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			1.2; 0.1; 0.1;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.04; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;
		0.008;										#double		mass
		0.001, 0.000, 0.000,						#Matrix3d	inertia	
		0.000, 0.010, 0.000,
		0.000, 0.000, 0.001;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		true;										#bool		integrate
		{phCookie}
	}
	PHSolid soTable{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;		#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;
		20.0;										#double		mass
		1.0, 0.0, 0.0,								#Matrix3d	inertia	
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		false;										#bool		dynamical
		false;
		{phTable}
	}
}
