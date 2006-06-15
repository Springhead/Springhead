	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	(0.390f/2)		//	x方向の辺の長さ/2
	#define PY	(0.395f/2)		//	y方向の辺の長さ/2
	#define PZ	(0.405f/2)		//	z方向の辺の長さ/2
/*	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
*/
	#define GX	(0.065f/2)		//	x方向の辺の長さ/2
	#define GY	(0.065f/2)		//	y方向の辺の長さ/2
	Vec3f motorPos[8][2] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY,-PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f( GX,  0, 0)}
	};

	for(int i=4; i<8; ++i){
		spidar.motor[i].lengthPerPulse *= -1;
	}

	spidar.Init(devMan, 8, motorPos, 0.3f, 2.924062107079e-5f, 1.0f, 20.0f);
