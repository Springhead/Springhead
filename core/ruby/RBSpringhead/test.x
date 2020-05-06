xof 0302txt 0064

FWScene fwScene{
	{grScene}
	{phScene}
	FWObject obj1{
		{soBlock1} {fr1}
	}
	FWObject obj2{
		{soBlock2} {fr2}
	}
	FWObject obj3{
		{soBlock3} {fr3}
	}
}

GRScene grScene{
	GRFrame {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,4,1;;
		GRFrame frCam{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1;;
		}
	}
	GRCamera cam{
		0.2; 0.0;;				#	size	スクリーンのサイズ
		0; 0;;					#	center	カメラからのスクリーンのずれ
		0.1; 500;				#	front back	視点からクリップ面までの相対距離（正の値で指定）
		{frCam}
	}
	GRLight light1{
		0.5; 0.2; 0.2; 1.0;;	#	ambient		環境光
		1.5;0.1;0.1;1;;	#	diffuse		拡散光
		1.0;0.1;0.1;1;;	#	emmisive	鏡面光
		1;1;1;0;;	#	position	光源位置（w=1.0で点光源、w=0.0で平行光源）
		100; 			#	range	光が届く範囲（deviceがDirectXの場合のみ利用可能）
		1;				#	attenuation0	一定減衰率
		0;				#	attenuation1	線形減衰率
		0;				#	attenuation2	2次減衰率
		0;0;0;;		#	spotDirection	スポットライトの向き
		0;				#	spotFalloff		スポットライトの円錐内での減衰率(大きいほど急峻) 0..∞
		0;				#	spotInner			スポットライトの中心部分(内部コーン)（deviceがDirectXの場合のみ利用可能） 0..spotCutoff
		0;				#	spotCutoff	スポットライトの広がり角度(度)(外部コーン) 0..π(pi)
	}
	GRMaterial mat{
		0.2; 0.2; 1.0; 1.0;;	#ambient
		0.0; 0.0; 1.0; 1.0;;	#diffuse
		0.0; 0.0; 1.0; 1.0;;	#specular
		0.1; 0.1; 0.1; 1.0;;	#emmisive
		4.0;									#power
		"";
	}
	GRFrame fr1{
		1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
		GRMesh{
			-2;0;0;,  2;0;0;,  0;2;0;;				#pos
			-1;0;1;,   0;0;1;,  1;0;1;;				#normal
			1;0;1;1;,   0;1;1;1;,  1;1;0;1;;		#color
			0;0;, 0;0;, 0;0;;						#tex
			0,1,2;
		}
	}
	GRFrame fr2{
		1,0,0,0,	0,1,0,0,	0,0,1,0,	1,1,0,1;;
		GRMesh{
			-2;0;0;,  2;0;0;,  0;2;0;;	#pos
			-1;0;1;,   0;0;1;,  1;0;1;;	#normal
			;		#color
			;		#tex
			0,1,2;
		}
	}
	GRFrame fr3{
		1,0,0,0,	0,1,0,0,	0,0,1,0,	1,2,0,1;;
		GRMesh{
			-2;0;0;,  2;0;0;,  0;2;0;;	#pos
			-1;0;1;,   0;0;1;,  1;0;1;;	#normal
			;		#color
			;		#tex
			0,1,2;
		}
	}			
}

CDConvexMesh meshFloor{
	1;
	0.2; 0.5; 0.6;;;
	10;  0;10;, 10;  0;-10;, -10;  0;10;, -10;  0;-10;, 
	10;-10;10;, 10;-10;-10;, -10;-10;10;, -10;-10;-10;;
}
CDConvexMesh meshBlock{
	1;
	0.2; 0.5; 0.6;;;
	0.5; 0.5; 0.5;, 0.5; 0.5;-0.5;, -0.5; 0.5;0.5;, -0.5; 0.5;-0.5;, 
	0.5;-0.5; 0.5;, 0.5;-0.5;-0.5;, -0.5;-0.5;0.5;, -0.5;-0.5;-0.5;;
}
PHScene phScene{
	0.01;			//	timeStep
	0;;				//	count
	0;-9.8;0;;		//	gravity
	15;
	PHSolid soFloor{
		0.0; 0.0; 0.0;;		#vel
		0.0; 0.0; 0.0;;		#angular val
		1.0; 0.0; 0.0; 0.0; 0.0; 0.0; 0.0;;	#pose		ET w,x,y,z; px, py, pz;
		0.0; 0.0; 0.0;;		#force		(read only)
		0.0; 0.0; 0.0;;		#torque		(read only)
		0.0; 0.0; 0.0;;		#newForce
		0.0; 0.0; 0.0;;;	#newTorque
		1e30;				#mass
		1e30, 0.0, 0.0,
		0.0, 1e30, 0.0,
		0.0, 0.0, 1e30;;	#inertia
		0.0; 0.0; 0.0;;		#center
		false;
		{meshFloor}
	}
	PHSolid soBlock1{
		0.0; 0.0; 0.0;;		#vel
		0.0; 0.0; 0.0;;		#angular val
		1.0; 0.0; 0.0; 0.0; 0.0; 1.0; 0.0;;	#pose		ET w,x,y,z; px, py, pz;
		0.0; 0.0; 0.0;;		#force		(read only)
		0.0; 0.0; 0.0;;		#torque		(read only)
		0.0; 0.0; 0.0;;		#newForce
		0.0; 0.0; 0.0;;;	#newTorque
		1;					#mass
		1, 0.0, 0.0,
		0.0, 1, 0.0,
		0.0, 0.0, 1;;		#inertia
		0.0; 0.0; 0.0;;		#center
		true;
		{meshBlock}
	}
	PHSolid soBlock2{
		0.0; 0.0; 0.0;;		#vel
		0.0; 0.0; 0.0;;		#angular val
		1.0; 0.0; 0.0; 0.0; 0.6; 3.0; 0.0;;	#pose		ET w,x,y,z; px, py, pz;

		0.0; 0.0; 0.0;;		#force		(read only)
		0.0; 0.0; 0.0;;		#torque		(read only)
		0.0; 0.0; 0.0;;		#newForce
		0.0; 0.0; 0.0;;;	#newTorque
		1;					#mass
		1, 0.0, 0.0,
		0.0, 1, 0.0,
		0.0, 0.0, 1;;		#inertia
		0.0; 0.0; 0.0;;		#center
		true;
		{meshBlock}
	}
	PHSolid soBlock3{
		0.0; 0.0; 0.0;;		#vel
		0.0; 0.0; 0.0;;		#angular val
		1.0; 0.0; 0.0; 0.0; 0.0; 5.0; 0.0;;	#pose		ET w,x,y,z; px, py, pz;
		0.0; 0.0; 0.0;;		#force		(read only)
		0.0; 0.0; 0.0;;		#torque		(read only)
		0.0; 0.0; 0.0;;		#newForce
		0.0; 0.0; 0.0;;;	#newTorque
		1;					#mass
		1, 0.0, 0.0,
		0.0, 1, 0.0,
		0.0, 0.0, 1;;		#inertia
		0.0; 0.0; 0.0;;		#center
		true;
		{meshBlock}
	}
}
