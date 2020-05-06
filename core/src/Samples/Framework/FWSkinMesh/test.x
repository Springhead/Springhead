xof 0302txt 0064

FWScene fwScene{
	{grScene}
}

GRScene grScene{
	Import{"human.x";}
		GRFrame frCam{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,5,1;;
		}
	GRCamera cam{
		0.2; 0.0;;				#	size	スクリーンのサイズ
		0; 0;;					#	center	カメラからのスクリーンのずれ
		0.1; 500;				#	front back	視点からクリップ面までの相対距離（正の値で指定）
		{frCam}
	}
	
}
