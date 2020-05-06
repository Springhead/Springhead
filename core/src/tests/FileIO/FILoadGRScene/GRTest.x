xof 0302txt 0064

GRSdk{
	GRScene scene{
		GRCamera cam{
			0.2; 0.0;;			#	size	スクリーンのサイズ
			0; 0;;					#	center	カメラからのスクリーンのずれ
			0.1; 500;				#	front back	視点からクリップ面までの相対距離（正の値で指定）
			GRFrame frCam{
				1,0,0,0,0,1,0,0,0,0,1,0,0,0,4,1;;
			}
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
		GRFrame fr1{
			1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
			GRMesh{
				-2;0;0;,  2;0;0;,  0;2;0;;				#pos
				3; 2, 1, 0;;							#faces
				-1;0;1;,   0;0;1;,  1;0;1;;				#normal
				3; 2, 1, 0;;							#faceNormal
				1;0;1;1;,   0;1;1;1;,  1;1;0;1;;		#colors
				0;0;, 0;0;, 0;0;;						#texCoords
				0,0,0;									#materialList
			}
		}
		GRMaterial mat{
			0.2; 0.2; 1.2; 1.0;;	#ambient
			0.0; 0.0; 1.0; 1.0;;	#diffuse
			0.0; 0.0; 1.0; 1.0;;	#specular
			0.1; 0.1; 0.1; 1.0;;	#emmisive
			4.0;					#power
			"texture.ppm";
		}
		GRFrame fr2{
			1,0,0,0,	0,1,0,0,	0,0,1,0,	1,1,0,1;;
			GRMesh{
				-2;0;0;,  2;0;0;,  0;2;0;;	#pos
				3; 2, 1, 0;;				#faces
				-1;0;1;,   0;0;1;,  1;0;1;;	#normal
				3; 2, 1, 0;;							#faceNormal
				1;0;1;1;,   0;1;1;1;,  1;1;0;1;;		#colors
				0;0;, 1;1;, 1;0;;						#texCoords
				0,0,0;									#materialList
			}
		}		
	}
}
