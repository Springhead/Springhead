#include "PHFemEngine.h"
namespace Spr {;

struct CondVtx;					//	Conduction　Vertexの意味？伝導候補頂点？	Companioned Vertex？の略？ のどちらか。
struct CondVtxs :public std::vector<CondVtx> {
	//PHFemMesh* pmesh;
	//
	PHFemMeshNew* pmesh;
	std::vector<int> vtx2Cond;
};
struct CondVtx {
	int vid;
	Vec3d pos;	//	x,yは接触面上での位置、zは接触面からの距離
	double area;
	double assign;
	CondVtx() :vid(-1), area(0), assign(0) {}
	struct Companion {
		int id;
		double area;
		Companion(int i, double a) :id(i), area(a) {}
	};
	std::vector<Companion> companions;
	struct Less {
		int axis;
		Less(int a) :axis(a) {}
		bool operator() (const CondVtx& a, const CondVtx& b) const { return a.pos[axis] < b.pos[axis]; }
	};
};
inline static double dist2D2(const Vec3d& a, const Vec3d& b) {
	return Square(a.x - b.x) + Square(a.y - b.y);
}

inline static double dist2Dxz(const Vec3d& a, const Vec3d& b) {
	return sqrt(Square(a.x - b.x) + Square(a.z - b.z));
}
//	condVtxsのなかから、fromsの隣の割り当てに空きがある頂点を列挙し、posに近い順にソートして返す。
struct Dist2Less {
	Vec3d pos;
	CondVtxs& condVtxs;
	Dist2Less(Vec3d p, CondVtxs& c) :pos(p), condVtxs(c) {}
	bool operator () (int a, int b)const {
		return dist2D2(condVtxs[a].pos, pos) < dist2D2(condVtxs[b].pos, pos);
	}
};
double FindNearest(const Vec3d& pos, CondVtxs& condVtxs, int& found) {
	if (condVtxs.size() == 0) {
		found = -1;
		return DBL_MAX;
	}
	int minId = found;
	double minDist2;
	int cid;
	do {
		cid = minId;
		minDist2 = dist2D2(condVtxs[cid].pos, pos);		//距離の2乗 
		int vid = condVtxs[cid].vid;
		for (unsigned e = 0; e < condVtxs.pmesh->vertices[vid].edgeIDs.size(); ++e) {
			FemEdge& edge = condVtxs.pmesh->edges[condVtxs.pmesh->vertices[vid].edgeIDs[e]];
			int next = edge.vertexIDs[0] == vid ? edge.vertexIDs[1] : edge.vertexIDs[0];
			int cnext = condVtxs.vtx2Cond[next];			//edge pair
			if (cnext >= 0) {
				double d2 = dist2D2(condVtxs[cnext].pos, pos);
				if (d2 < minDist2) {
					minDist2 = d2;
					minId = cnext;
				}
			}
		}
	} while (cid != minId);
	found = cid;
	return sqrt(minDist2);
}
void DeleteNeighbor(int id, std::vector<int> &neighbor) {

	//idと一致する要素を削除
	int delNUM = -1;	//削除する要素は何番目か
	for (unsigned n = 0; n<neighbor.size(); ++n) {
		if (neighbor[n] == id) {
			delNUM = n;
			break;
		}
	}
	neighbor.erase(neighbor.begin() + delNUM);
}
void AddCompanion(CondVtxs& v1, int id1, CondVtxs& v2, int id2, double a) {
	assert(a > 0);
	v1[id1].assign += a;
	v2[id2].assign += a;
	assert(v1[id1].area - v1[id1].assign > -1e8);
	assert(v2[id2].area - v2[id2].assign > -1e8);
	unsigned i;
	for (i = 0; i<v1[id1].companions.size(); ++i) {
		if (v1[id1].companions[i].id == id2) {		// すでにcompanionに登録されている頂点を再Addの時、?
			v1[id1].companions[i].area += a;
			break;
		}
	}
	if (i == v1[id1].companions.size()) {
		v1[id1].companions.push_back(CondVtx::Companion(id2, a));
	}
	for (i = 0; i<v2[id2].companions.size(); ++i) {
		if (v2[id2].companions[i].id == id1) {
			v2[id2].companions[i].area += a;
			break;
		}
	}
	if (i == v2[id2].companions.size()) {
		v2[id2].companions.push_back(CondVtx::Companion(id1, a));
	}
}

void RenewNeighbor(CondVtxs& condvtx, int id, std::vector<int> &neighbor) {
	//idと一致する要素を削除
	DeleteNeighbor(id, neighbor);

	//idと隣接する要素（点）をneighborに追加
	for (unsigned e = 0; e<condvtx.pmesh->vertices[condvtx[id].vid].edgeIDs.size(); ++e) {
		//表面エッジでなければ以下処理スキップ
		if (condvtx.pmesh->edges[condvtx.pmesh->vertices[condvtx[id].vid].edgeIDs[e]] < (int)condvtx.pmesh->nSurfaceEdge)
			continue;
		FemEdge& edge = condvtx.pmesh->edges[condvtx.pmesh->vertices[condvtx[id].vid].edgeIDs[e]];	//idが一点のedgeを求め
		int next = edge.vertexIDs[0] == condvtx[id].vid ? edge.vertexIDs[1] : edge.vertexIDs[0];				//もう一方の点を求める
		if (condvtx.pmesh->GetPHFemThermo()->GetVertexToofar(next) == false) { //遠くて隣接点には加えない点ではなければ
			int cnext = condvtx.vtx2Cond[next];		//condvtxに変換
			if (cnext >= 0 && cnext<(int)condvtx.size())
			{	//その点が接触候補であるとき()(condvtxのサイズ内であるとき：サイズより大きい場合はcondvtxs)
				if (condvtx[cnext].assign == 0) {		//まだ割り当てられていなければ
					int exist = 0;
					for (unsigned n = 0; n<neighbor.size(); ++n) {
						if (neighbor[n] == cnext) {
							exist = 1;	//すでにneighborに値があるときは代入しない
							break;
						}
					}
					if (exist == 0)
						neighbor.push_back(cnext);	//neighborに追加
				}
			}
		}
	}
}

void MakeVtxPair(CondVtxs(&condVtxs)[2], std::vector<int>(&neighbor)[2], int baseMesh, int current_id) {
	int companion_id;//初期化
	double dist = DBL_MAX;

	while (1) { //neighbor同士の距離（対応を取った点同士)の距離が一定より大きくなったら終了

		double overlapArea; //重なっている部分の面積
		companion_id = -1;//初期化

						  //neighborの中から基準点に一番近い点を探し、companion_idにする
		double mini_length = DBL_MAX;	//edgeの長さ
		for (unsigned n = 0; n<neighbor[1 - baseMesh].size(); ++n) {
			//neighbor[相手側のメッシュ][n]とcondVtxs[基準メッシュ][current_id]の距離を求める
			double length = dist2D2(condVtxs[1 - baseMesh][neighbor[1 - baseMesh][n]].pos, condVtxs[baseMesh][current_id].pos);
			if (length < mini_length) {
				mini_length = length;
				companion_id = neighbor[1 - baseMesh][n];
			}
		}

		if (companion_id == -1) {//neighborが空のときはループ終了。neighborに要素が入っているとき、companion_idは-1にならない。
			return;
		}

		//距離判定（もし2点の距離が一定より大きかったらループ終了）
		dist = dist2Dxz(condVtxs[baseMesh][current_id].pos, condVtxs[1 - baseMesh][companion_id].pos); //現在、調べている2頂点の距離
		if (dist > 0.1/*1.14499*/) {
			condVtxs[baseMesh].pmesh->GetPHFemThermo()->SetVertexToofar(condVtxs[baseMesh][current_id].vid, true);//距離が遠い点をマーク
			DeleteNeighbor(current_id, neighbor[baseMesh]);//neighborから要素削除
														   //condVtxs[1-baseMesh].pmesh->vertices[condVtxs[1-baseMesh][companion_id].vid].toofar = true;//距離が遠い点をマーク
														   //DeleteNeighbor(companion_id,neighbor[1-baseMesh]);//neighborから要素削除

														   //まだneighborに要素が入っているのであれば、再起的に呼び出す。
			if (neighbor[0].size() != 0 && neighbor[1].size() != 0) {
				//	DSTR << "0サイズ：" << neighbor[0].size() << endl;
				//	for(unsigned n=0; n<neighbor[0].size(); n++){
				//		DSTR << "0残り：" << condVtxs[0][neighbor[0][n]].vid << endl;
				//	}
				//	DSTR << "1サイズ：" << neighbor[1].size() << endl;
				//	for(unsigned n=0; n<neighbor[1].size(); n++){
				//		DSTR << "1残り：" << condVtxs[1][neighbor[1][n]].vid << endl;
				//	}
				MakeVtxPair(condVtxs, neighbor, 1 - baseMesh, neighbor[1 - baseMesh][0]);
			}
			return;
		}

		if (condVtxs[baseMesh][current_id].area - condVtxs[baseMesh][current_id].assign > condVtxs[1 - baseMesh][companion_id].area - condVtxs[1 - baseMesh][companion_id].assign) {
			//基準点の面積＞相手の面積
			overlapArea = condVtxs[1 - baseMesh][companion_id].area - condVtxs[1 - baseMesh][companion_id].assign; //相手側（小さい方）の面積がoverlapArea
			AddCompanion(condVtxs[baseMesh], current_id, condVtxs[1 - baseMesh], companion_id, overlapArea);
			condVtxs[1 - baseMesh].pmesh->GetPHFemThermo()->SetVertexBeCondVtxs(condVtxs[1 - baseMesh][companion_id].vid, true);	//対応付け終わった点はtrue

																																	//相手側の面積を全て割り当てたので、対応点を削除し、その隣接点をneighborに追加
			RenewNeighbor(condVtxs[1 - baseMesh], companion_id, neighbor[1 - baseMesh]);

			//基準点の方が割り当てられる面積が多いので、基準点の残りの面積を割り当てるため、相手のneighborからさがしに行く。
		}
		else {	//基準点の面積＜=相手の面積
				//基準点（小さい方）の面積がoverlapArea
			overlapArea = condVtxs[baseMesh][current_id].area - condVtxs[baseMesh][current_id].assign;
			AddCompanion(condVtxs[baseMesh], current_id, condVtxs[1 - baseMesh], companion_id, overlapArea);
			condVtxs[baseMesh].pmesh->GetPHFemThermo()->SetVertexBeCondVtxs(condVtxs[baseMesh][current_id].vid, true);	//対応付け終わった点はtrue

																														//基準点の面積を全て割り当てたので、対応点を削除し、その隣接点をneighborに追加
			RenewNeighbor(condVtxs[baseMesh], current_id, neighbor[baseMesh]);

			//基準点を相手側に移す
			baseMesh = 1 - baseMesh;
			current_id = companion_id;	//基準点を相手側に移す
		}
	}
}


void PHFemEngine::ThermalTransfer() {
	for (int i = 0; i < (int) meshPairs.size(); ++i) {
		HeatTrans(meshPairs[i]);
	}
}
void PHFemEngine::HeatTrans(PHFemMeshPair* mp) {
	PHSolid* phs0 = mp->mesh[0]->GetPHSolid()->Cast();
	PHSolid* phs1 = mp->mesh[1]->GetPHSolid()->Cast();
	PHFemMeshNew* fmesh0 = mp->mesh[0];
	PHFemMeshNew* fmesh1 = mp->mesh[1];

	/*heattrans関数引用------------------------------------------------------------------------ここから*/
	PHSolid* solids[2];
	solids[0] = phs0;
	solids[1] = phs1;

	PHFemMeshNew* fmesh[2];
	fmesh[0] = fmesh0;
	fmesh[1] = fmesh1;
	PHScene* scene = solids[0]->GetScene()->Cast();
	//	接触ペアを見つけて、未判定なら判定する。
	bool bSwap;
	PHSolidPairForLCP* pair = scene->GetSolidPair(solids[0]->Cast(), solids[1]->Cast(), bSwap)->Cast();
	PHShapePairForLCP* sp = pair->GetShapePair(0, 0)->Cast();			/// 1st shape of wvery object is used as shape pair (= heatrans pair) 

	if (sp->lastContactCount != (scene->GetCount() - 1)) {		//今接触していなければ。接触	は他所で計算。CDDetectorImp::ContactDetect()のStateの仕様：最後接触時のカウントを保持のため 
																//	未接触なので、GJKを呼ぶ
		sp->shapePoseW[0] = solids[0]->GetPose() * solids[0]->GetShapePose(0);
		sp->shapePoseW[1] = solids[1]->GetPose() * solids[1]->GetShapePose(0);
		if (bSwap) std::swap(sp->shapePoseW[0], sp->shapePoseW[1]);
		Vec3d sep;
		double dist = FindClosestPoints(sp->shape[0], sp->shape[1], sp->shapePoseW[0], sp->shapePoseW[1], sep, sp->closestPoint[0], sp->closestPoint[1]);
		if (dist < 1e-10) {
			//	かなり近いので、法線が怪しいので、警告をだしておく。
			DSTR << "２物体が非常に近いが、接触しはしていない微妙な状態";
			DSTR << " dist : " << dist << std::endl;
			if (dist < 1e-14) {
				DSTR << "注意" << std::endl;
			}
		}
		sp->depth = -dist;
		sp->normal = (sp->shapePoseW[1] * sp->closestPoint[1] - sp->shapePoseW[0] * sp->closestPoint[0]);	//後の伝熱の計算必要。面→面の向き：法線から伝熱計算
		sp->normal.unitize();
	}

	//	距離:dist
	//	距離が近ければ伝熱の処理
	/*	熱伝達率 α [W/(m^2 K)] を用いると、境界上で q = α(T-Tc) (T:接点温度 Tc:周囲の流体等の温度)
	２物体の接触だと、T1-α1->Tc-α2->T2 となると考えられる。
	q = α1(T1-Tc) = α2(Tc-T2) より (α1+α2)Tc = α1T1 + α2T2
\	Tc = (α1T1 + α2T2)/(α1+α2)
	q = α1(T1-(α1T1 + α2T2)/(α1+α2)) = α1T1 - α1(α1T1 + α2T2)/(α1+α2)
	= (α1α2T1 - α1α2T2)/(α1+α2) = α'(T1-T2)  α' = α1α2/(α1+α2)	*/
	/*	qとQについての考察
	qは単位面積あたりなので、頂点間の熱の移動量Qに直すには、頂点が代表する面積を掛ける必要がある。
	本来は、三角形の重なりと形状関数から求めるべきもの。[

	しかし、重なり具合は毎ステップ変わるので、この計算は大変。簡略化を考える。
	頂点は頂点を含む三角形に勢力を持つ。三角形の重なりより、頂点の距離の意味が大きい。
	距離が近いものを割り当てていくが、後で飛び地が出てはいけない。
	そこで、１点から初めて徐々に割り当て領域を大きくして行く。これでずれは起きても飛び地はでない。

	頂点の面積は、頂点を含む三角形達の面積の和の1/3。
	*/
	const double isoLen = 0.005;	//	0.1にすると、反転後も、同じ面が加熱される。0.01にすると、反対面は加熱されないが、反対面に近いエッジが加熱される
	const double isoLen2 = 0.07;	//	condVtxsペアがこれ以上離れたら、対応していないことにする係数
	if (sp->depth > -isoLen) {		//	衝突の深さが
		CondVtxs condVtxs[2];
		condVtxs[0].pmesh = fmesh[bSwap ? 1 : 0]->Cast();
		condVtxs[1].pmesh = fmesh[bSwap ? 0 : 1]->Cast();
		//	距離が近い頂点を列挙。ついでに法線に垂直な平面上での座標を求めておく。
		Matrix3d coords;
		if (std::abs(sp->normal.x) < std::abs(sp->normal.y)) coords = Matrix3d::Rot(sp->normal, Vec3d(1, 0, 0), 'z');		//	物体間の共有面上でのx-y平面に物体の上下物体の頂点を置き換えるための座標変換をするために、z軸を垂直方面に   //射影準備？？
		else coords = Matrix3d::Rot(sp->normal, Vec3d(0, 1, 0), 'z');
		Matrix3d coords_inv = coords.inv();
		Vec3d normalL[2];

		for (int i = 0; i<2; ++i) {
			normalL[i] = sp->shapePoseW[i].Ori().Inv() * sp->normal * (i == 0 ? 1 : -1);		//判定の意味:? 1,-1の順が必要なのか？			//world座標を基準の法線から、ローカルの法線の向きを計算: W座標変換 *  normalL(ocal) =  normalW(orld)
																								//DSTR << std::endl << "NL:" << normalL[i]<< std::endl;
			for (unsigned v = 0; v < condVtxs[i].pmesh->surfaceVertices.size(); ++v) {
				double vd = (sp->closestPoint[i] - condVtxs[i].pmesh->vertices[condVtxs[i].pmesh->surfaceVertices[v]].pos) * normalL[i];
				vd -= sp->depth;
				if (vd < isoLen) {
					CondVtx c;
					c.vid = condVtxs[i].pmesh->surfaceVertices[v];
					c.pos = coords_inv * (sp->shapePoseW[i] * condVtxs[i].pmesh->vertices[c.vid].pos);
					c.pos.z = vd + (sp->depth / 2);	//	中面からの距離にしておく。
													// sp->normalはどちらかの物体からの法線なので、これの向きがcondvtxsのbswapによって？正負ありうる。
													// if(bSwap == true)	condVtxs[0] == fmesh[1]		// sp->normal法線と反対向きのみ
													// else(bSwap == false)	condVtxs[0] == fmesh[0]		// sp->normal法線と同じ向き

					double judgeN = normalL[i] *
						condVtxs[i].pmesh->GetPHFemThermo()->GetVertexNormal(condVtxs[i].pmesh->surfaceVertices[v]);

					if (judgeN > 0.0) {		// <= 以前の符号方向
						condVtxs[i].push_back(c);		//.　l.828でいれているcondVtxsは同じ？。condVtxsのメンバ変数を設定したcondVtxを作る。
					}
				}
			}
		}
		for (unsigned i = 0; i<2; i++) {
			if (condVtxs[i].size() == 0) {
				DSTR << "00 Can not find companion vertex on object " << i << std::endl;
			}
		}
		/*heattrans関数引用--------------------------------------------------------------ここまで*/

		const double isoLen3 = 0.003;		//:BoundingBoxの周囲何[m]までペア探索範囲とするか
											//	Lessを基に(接触面上のx,z軸で)ソート
		Vec2d bboxMin, bboxMax;
		for (int axis = 0; axis<2; ++axis) {
			for (int i = 0; i<2; ++i) {
				std::sort(condVtxs[i].begin(), condVtxs[i].end(), CondVtx::Less(axis)); //x(y)軸について両物体の頂点をソート
			}
			//	軸ごとに、2次元AABBのための最小値/最大値を求める
			//	boundingboxチェック 重ならない部分は捨てる
			//	軸ごとに、共通範囲の座標を入れる。最小から。
			bboxMin[axis] = condVtxs[0].size() ? condVtxs[0].front().pos[axis] : DBL_MIN;                           //condVtxs[0]のサイズがあれば、front().posを代入
			bboxMin[axis] = std::max(bboxMin[axis], (condVtxs[1].size() ? condVtxs[1].front().pos[axis] : DBL_MIN));  //condVtxs[1]のサイズがあれば、front().posを代入 ↑と比較して大きいほうを代入（重なっている部分の最小値）
			bboxMin[axis] -= isoLen3; //<-isoLen,	
			for (int i = 0; i<2; ++i) {		//	condVtxsのどちらも、isoLen3より外側の点は候補から除外する
				CondVtx tmp;
				tmp.pos[axis] = bboxMin[axis];
				CondVtxs::iterator it = std::lower_bound(condVtxs[i].begin(), condVtxs[i].end(), tmp, CondVtx::Less(axis));
				condVtxs[i].erase(condVtxs[i].begin(), it);
			}
			bboxMax[axis] = condVtxs[0].size() ? condVtxs[0].back().pos[axis] : DBL_MAX;
			bboxMax[axis] = std::min(bboxMax[axis], (condVtxs[1].size() ? condVtxs[1].back().pos[axis] : DBL_MAX));
			bboxMax[axis] += isoLen3;
			for (int i = 0; i<2; ++i) {
				CondVtx tmp;
				tmp.pos[axis] = bboxMax[axis];
				CondVtxs::iterator it = std::upper_bound(condVtxs[i].begin(), condVtxs[i].end(), tmp, CondVtx::Less(axis));		//	Lessで良いのか
				condVtxs[i].erase(it, condVtxs[i].end());
			}
		}

		//対応する頂点のない頂点の削除
		for (int i = 0; i<2; ++i) {
			condVtxs[i].vtx2Cond.resize(condVtxs[i].pmesh->vertices.size(), -1);
			for (unsigned j = 0; j<condVtxs[i].size(); ++j) {
				condVtxs[i].vtx2Cond[condVtxs[i][j].vid] = j;
			}
		}
		//	対応する頂点が見つからない頂点を、削除のためにマーク。
		std::vector<int> eraseVtxs[2];
		for (int i = 0; i<2; ++i) {
			int from = 0;
			for (unsigned j = 0; j<condVtxs[i].size(); ++j) {
				int found = from;
				double dist = FindNearest(condVtxs[i][j].pos, condVtxs[1 - i], found);	// condVtxs[i][j].posとcondVtxs[1-i](spのペア)の内、一番短いやつがisoLenより長かったら、熱伝導しない。けど、最初に、isoLenで判定してからペア作っているんだから、そのあとに、distを計算する必要はあるのだろうか？condVtxsが毎ステップ更新されないのなら、必要かも
				if (dist > isoLen2) {	//	平面距離がisoLen以上の頂点は削除のためリストout
					eraseVtxs[i].push_back(j);
				}
				else {
					from = found;
				}
			}
		}
		//	マークした頂点を削除
		for (int i = 0; i<2; ++i) {
			std::sort(eraseVtxs[i].begin(), eraseVtxs[i].end());
			for (int j = (int)eraseVtxs[i].size() - 1; j>0; --j) {
				condVtxs[i].erase(condVtxs[i].begin() + eraseVtxs[i][j]);
			}
			if (condVtxs[i].size() == 0) {
				return;
			}
		}

		//頂点の担当する面積の計算(実際には計算しているのではなく呼び出して代入してるだけ)上と同操作
		for (int i = 0; i<2; ++i) {
			for (unsigned j = 0; j<condVtxs[i].size(); ++j) {
				condVtxs[i][j].area = condVtxs[i].pmesh->GetPHFemThermo()->GetVertexArea(condVtxs[i][j].vid);
			}
		}
		//2物体が重なっている部分の中心を探す
		double xCenter = 0.5*(bboxMin.x + bboxMax.x); //重なっている部分の中心
		int centerVtx[2] = { -1, -1 };	//初期化(-1,-1)を入れとく //condVtxsで前から何番目か
		for (int i = 0; i<2; ++i) {
			CondVtx tmp;
			tmp.pos[1] = 0.5*(bboxMin.y + bboxMax.y);
			CondVtxs::iterator it = std::lower_bound(condVtxs[i].begin(), condVtxs[i].end(), tmp, CondVtx::Less(1)); //condVtxs[i]（begin~endで）指定した値（tmp)以上の値が最初に現れる位置を返す。
			int cit = (int)(it - condVtxs[i].begin()); //tmpより大きい値(要素を持った)が出た場所-最初の要素の場所
			double minDistX = DBL_MAX;	//初期化（doubleの上限値）
			for (int y = 0; y<5; ++y) {
				if (cit - y >= 0 && cit - y < (int)condVtxs[i].size()) {
					double dist = std::abs(condVtxs[i][cit - y].pos.x - xCenter);
					if (dist < minDistX) {
						centerVtx[i] = cit - y;
						minDistX = dist;
					}
				}
				if (cit + y < (int)condVtxs[i].size()) {
					double dist = std::abs(condVtxs[i][cit + y].pos.x - xCenter);
					if (dist < minDistX) {
						centerVtx[i] = cit + y;
						minDistX = dist;
					}
				}
			}
			assert(centerVtx[i] >= 0); //論理チェック centerVtxは0より大きい（0より小さいと診断メッセージ）
		}

		//	centerVtx[i]と一番近い頂点を探す
		int baseMesh = 0; //0or1、メッシュの対応を探す時、メッシュ０と１のどちらを基準に探しているか
		int current_id = centerVtx[0]; //基準の点ID。こいつの対応点を探している。
		int companion_id = centerVtx[1];

		// baseMesh(=centerVtx[0])を基準にcenterVtx[1]のfaces[0]から内部判定。対応させるcompanion_idを返す。

		/*//9.14 while文で対応づけるように全点対応づけるようにループさせる
		割り当てた頂点の数 = condVtxsの数になったら、全点対応したということでloopから出る。
		2013.9.20
		隣接点を探しに行く処理⇒隣接点の面積が全ていっぱいだったときの処理がかけていない。
		*/
		/*2013.9.20
		TriInOutJudgeの第三引数でfaceを渡すのだが、表面faceかを確認せずに渡していたのでバグが出た。
		TriInoutJudgeで返していた変数は頂点IDだったが、返すべきはそれをVtx2condで変換するやつだった。
		現在は修正済み
		*/
		/*
		2013.10.8
		終了（ループ）条件はcoundVtxs(接触している点)の数と対応付けが終わった（面積が０になった）点の数を比較し、それが同じ数だった場合に全点対応にする。
		対応付けが終わったときに、countMesh[0or1]をカウントアップする。

		2013.10.22
		終了条件の変更。⇒接触面積のだいたいを対応付けたら接触終了。
		そのためcounMeshのカウントアップは削除。
		neighbor変数を追加し、ここから次の対応を選ぶようにする。Addneighbor関数追加。

		2014
		終了条件は頂点ペアの線分距離にした		*/

		//	ここから対応を広げていく
		std::vector<int> neighbor[2];		//頂点の探索候補
		neighbor[0].push_back(current_id);
		neighbor[1].push_back(companion_id);

		//対応点を取る
		MakeVtxPair(condVtxs, neighbor, baseMesh, current_id);
		double areaSUM[2] = { 0.0, 0.0 };


		/// 2物体の間で熱伝達境界条件利用による熱伝達計算を行う
		for (unsigned i = 0; i < condVtxs[0].size(); i++) {
			//	対応する節点(companions[j])の温度を使って熱伝達の計算を行う
			double areasum = 0.0;
			for (unsigned j = 0; j < condVtxs[0][i].companions.size(); j++) {
				// 熱伝達のペアになっている者同士を、同じ色で描画、どの色で描画するか、ランダムで決定？
				//i=0から、１つずつチェックしていく。重複している場合には、色が上書きされてしまうので。
				condVtxs[0].pmesh->GetPHFemThermo()->SetVertexHeatTransRatio(condVtxs[0][i].vid, mp->heatTransferRatio);	// フライパン-空気間で伝熱しない：物体間伝達時のみ適用化すべきだが、とりあえず。
																													//>		isoLen3==0の時、judgeN直後と入っている点の数は同じなのに、熱伝達されていない点があるように見える。
				double dqdt = mp->heatTransferRatio * (condVtxs[0].pmesh->GetPHFemThermo()->GetVertexTemp(condVtxs[0][i].vid)			//	0,1どちらがどちらか変わる可能性ありだが、これでいいのか。
					- condVtxs[1].pmesh->GetPHFemThermo()->GetVertexTemp(condVtxs[1][condVtxs[0][i].companions[j].id].vid)) * condVtxs[0][i].companions[j].area;
				//	熱伝達対象頂点では、熱放射を計算しない
				//DSTR << condVtxs[0].pmesh->vertices[condVtxs[0][i].vid].beRadiantHeat << "," <<
				//	condVtxs[1].pmesh->vertices[condVtxs[1][condVtxs[0][i].companions[j].id].vid].beRadiantHeat <<std::endl;
				condVtxs[0].pmesh->GetPHFemThermo()->SetVertexBeRadiantHeat(condVtxs[0][i].vid, false);
				condVtxs[1].pmesh->GetPHFemThermo()->SetVertexBeRadiantHeat(condVtxs[1][condVtxs[0][i].companions[j].id].vid, false);

				// condvtx[0]のVecf にdqdt を足す
				// condVtx[1]のcompanion.id番目のVecfから引く
				condVtxs[0].pmesh->GetPHFemThermo()->AddvecFAll(condVtxs[0][i].vid, -dqdt);//-
				condVtxs[1].pmesh->GetPHFemThermo()->AddvecFAll(condVtxs[1][condVtxs[0][i].companions[j].id].vid, dqdt);//+
			}
		}
	}
}

}
