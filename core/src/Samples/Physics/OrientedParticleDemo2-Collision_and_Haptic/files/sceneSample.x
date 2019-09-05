xof 0302txt 0064

#FWモデルの作成
#FwObjectにGRFrame型とPHSolid型のモデルを関連付けすることで，GRモデルがPHモデルに同期される
FWScene fwScene{
	{grScene}
	{phScene}
	
	FWObject fwKuma{
		{grFrameKuma}
		{soKumaBody}
	}
	FWObject fwFloor{
		{grFrameFloor}
		{soFloor}
	}
}
#phモデルの作成
Import{"phKumaBody.x"}
PHScene phScene{									#SprPHScene.h PHSceneDesc
	#SprPHScene.h PHSceneState (inherited)
		0.01;										#double		timeStep
		0;											#unsigned	count
	;
	0.0; -9.8; 0;;									#Vec3f		gravity
	0.99;											#air resist
	15;												#int		numIteration
	
	PHSolid soKumaBody{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; -0.5; 10.0; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;
		1.0;										#double		mass
		10.0,	  0.00,		0.00,					#Matrix3d	inertia	
		0.00,	  1.0,		0.00,
		0.00,	  0.0,		10.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		true;										#bool		dynamical
		true;										#bool		integrate
		PHFrame{
			1,0,0,0,0,0,0;;
			{phKumaBody}
//			{grKuma}
		}
	}
	PHSolid soFloor{
		#SprPHSolid.h PHSolidState (inherited)
			0.0; 0.0; 0.0;;							#Vec3d		velocity
			0.0; 0.0; 0.0;;							#Vec3d		angVelocity
			1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;	#Posed		pose
			0.0; 0.0; 0.0;;							#Vec3d		force
			0.0; 0.0; 0.0;;							#Vec3d		torque
			0.0; 0.0; 0.0;;							#Vec3d		nextForce
			0.0; 0.0; 0.0;;							#Vec3d		nextTorque
		;
		1.0;										#double		mass
		10.0,	  0.00,		0.00,					#Matrix3d	inertia	
		0.00,	  1.0,		0.00,
		0.00,	  0.0,		10.0;
		0.0; 0.0; 0.0;;								#Vec3d		center
		false;										#bool		dynamical
		false;										#bool		integrate
		{grFloor}
	}
	
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
	GRLight light1{							#SprGRRender.h GRMaterialDesc
		0.9; 0.9; 0.9; 1.0;;					#Vec4f ambient
		0.5; 0.5; 0.5; 1.9;;					#Vec4f diffuse
		0.1; 0.1; 0.1; 1.0;;					#Vec4f specular
		1.0; 1.0; 1.0; 0.0;;					#Vec4f position
		10000.0;						#float range
		1.0;							#float attenuation0
		0.0;							#float attenuation1
		0.0;							#float attenuation2
		0.0; 0.0; 0.0;;						#Vec3f spotDirection
		0;							#float spotFalloff
		0;							#float spotInner
		0;							#flaot spotCutoff
	}
	
	GRFrame grFrameKuma{
		#Affinef transform
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;;
		{grKuma}
	}
	
	GRFrame grFrameFloor{
		#Affinef transform
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0;;
		{grFloor}
	}
}

#GRモデルの作成
Import{"grKuma.x";}		#生成したxファイルのパスを指定
Import{"grFloor.x";}

