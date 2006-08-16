#include "TDiffFile.h"
#include <Graphics/GRRender.h>
#include <sstream>

#include <float.h>
using namespace Spr;
inline bool IsValid(const Vec3f& v){
	return v.square() < Square(900);
}

float TDiffRecord::LoadField(std::istream& is){
	char ch;
	is >> ch;
	if (!is.good() || ch!=','){
		is.setf(std::ios::badbit);
		return FLT_MAX;
	}
	float rv;
	is >> rv;
	return rv;
}
bool TDiffRecord::Load(std::istream& isAll){
	char line[10*1024];
	isAll.getline(line, sizeof(line));
	std::istringstream is(std::string(line, line+strlen(line)));
	char ch;
	is >> ch;
	if (!is.good()) return false;
	assert(ch == '"');
	char buf[256];
	is.getline(buf, sizeof(buf), '"');
	type = buf;
	time = LoadField(is);

	//	座標系をSpringheadに合わせる．
	static Affinef afw = Affinef::Rot(Rad(-90), 'x');
	if (type.compare("FRF") == 0){
		for(int i=0; i<3; i++){
			Vec3f v;
			for(int i=0; i<3; ++i){
				v[i] = LoadField(is);
			}
			v = afw * v;
			assert(is.good());
			data.push_back(v);
		}
		while(is.good()){
			Vec3f v;
			for(int i=0; i<2; ++i){
				v[i] = LoadField(is);
			}
			if (is.good()){
				v = afw * v;
				data.push_back(v);
			}
		}
		is.clear();
	}else{
		while(is.good()){
			Vec3f v;
			for(int i=0; i<3; ++i){
				v[i] = LoadField(is);
			}
			if (is.good()){
				v = afw * v;
				v.Z() -= 3.1f;
				v.X() += 0.1f;
				data.push_back(v);
			}
		}
		is.clear();
	}
	return true;
}
bool TDiffFile::Load(std::istream& is){
	while(is.good()){
		TDiffRecord rec;
		rec.Load(is);
		if (rec.type.compare("G1L") == 0) g1[0].push_back(rec);
		if (rec.type.compare("G1R") == 0) g1[1].push_back(rec);
		if (rec.type.compare("G2L") == 0) g2[0].push_back(rec);
		if (rec.type.compare("G2R") == 0) g2[1].push_back(rec);
		if (rec.type.compare("FRF") == 0) frf.push_back(rec);
	}
	bLoaded = true;
	return true;
}
void TDiffRecord::Print(std::ostream& os){
	os << type.c_str() << " " << time << " : ";
	for(TData::iterator it = data.begin(); it != data.end(); ++it){
		os << *it << " ";
	}
	os << std::endl;
}
void TDiffFile::Print(std::ostream& os){
	g1[0].Print(os);
	g1[1].Print(os);
	g2[0].Print(os);
	g2[1].Print(os);
	frf.Print(os);
}
void TDiffFile::TRecords::Print(std::ostream& os){
	std::string type;
	if (size()) type = front().type;
	DSTR << type << ":" << size() << " records." << std::endl;
/*	for(iterator it = begin(); it != end(); ++it){
		it->Print(os);
	}
*/
}
bool TDiffFile::TRecords::GetRecord(TDiffRecord& pos, TDiffRecord& vel ,float time){
	if (!size()) return false;
	if (curIndex == size()) curIndex = size()-1;
	for(; curIndex > 0 && at(curIndex).time > time; --curIndex);
	for(; curIndex < size() && at(curIndex).time < time; ++curIndex);
	if (curIndex == 0){
		pos = at(curIndex);
		vel.data.resize(pos.data.size());
		for(int i=0; i<vel.data.size(); ++i) vel.data[i] = Vec3f(0,0,0);
		vel.time = time;
		return true;
	}else if (curIndex == size()){
		pos = at(curIndex-1);
		vel.data.resize(pos.data.size());
		for(int i=0; i<vel.data.size(); ++i) vel.data[i] = Vec3f(0,0,0);
		vel.time = time;
		return false;
	}else{
		float dt = at(curIndex).time - at(curIndex-1).time;
		float t = (time - at(curIndex-1).time) / dt;
		pos.data.resize(at(curIndex).data.size());
		vel.data.resize(pos.data.size());
		for(int i=0; i<pos.data.size(); ++i){
			Vec3f pos1 = at(curIndex-1).data[i];
			Vec3f pos2 = at(curIndex).data[i];
			vel.data[i] = Vec3f();
			if (IsValid(pos1) && IsValid(pos2)){
				pos.data[i] = (1-t)*pos1 + t*pos2;
				vel.data[i] = (at(curIndex).data[i] - at(curIndex-1).data[i]) / dt;
			}else if (IsValid(pos1)){
				pos.data[i] = pos1;
			}else if (IsValid(pos2)){
				pos.data[i] = pos2;
			}else{
				pos.data[i] = Vec3f(9999.9f,9999.9f,9999.9f);
			}
		}
		pos.time = vel.time = time;
		return true;
	}
}

Spr::GRMaterialDesc Mat(Vec4f c){
	return Spr::GRMaterialDesc(c, c, c,	c, 0.0f);
}
void TDiffFile::Draw(GRRender* render, THuman& human, float time, bool bDrawLine){
	if (!IsLoaded()) return;
	if (!human.IsLoaded()) return;
	for(int lr=0; lr<2; ++lr){
		//	DIFFファイルの表示
		static Vec3f pos[14];
		static Vec3f vel[14];
		GetRecord(pos, vel, lr==0, time);
		if (lr==0) render->SetMaterial(Mat(Vec4f(1,0,0,1)));
		else render->SetMaterial(Mat(Vec4f(0,1,0,1)));
		render->SetLineWidth(5);
		for(int i=0; i<13; ++i){
			if (IsValid(pos[i])){
				render->DrawDirect(GRRender::POINTS, pos+i, 1);
			}
			render->SetLineWidth(3);
		}
		if (bDrawLine){
			render->SetLineWidth(1);
			for(int i=0; i<12; ++i){
				if (IsValid(pos[i]) && IsValid(pos[i+1])){
					render->DrawDirect(GRRender::LINES, pos+i, 2);
				}
			}
		}
		//	バネの端点の表示
		if (lr==0) render->SetMaterial(Mat(Vec4f(1,0,1,1)));
		else render->SetMaterial(Mat(Vec4f(0,1,1,1)));
		render->SetLineWidth(5);
		for(unsigned i=0; i<human.springs[lr].size() && human.springs[lr][i].solid; ++i){
			Vec3f pos = human.springs[lr][i].GetPos();
			render->DrawDirect(GRRender::POINTS, &pos, 1);
		}
		render->SetLineWidth(1);
		if (bDrawLine){
			for(unsigned i=0; i+1<human.springs[lr].size() && human.springs[lr][i+1].solid; ++i){
				Vec3f p[2];
				p[0]= human.springs[lr][i].GetPos();
				p[1] = human.springs[lr][i+1].GetPos();
				render->DrawDirect(GRRender::LINES, p, 2);
			}
		}
	}
}
void TDiffFile::TRecords::Draw(GRRender* render, float time){
	TDiffRecord pos, vel;
	GetRecord(pos, vel, time);
	if (pos.data.size() < 2) return;
	render->SetLineWidth(5);
	for(unsigned i=0; i<pos.data.size(); ++i){
		if (pos.data[i].norm() < 9000){
			render->DrawDirect(GRRender::POINTS, &*pos.data.begin()+i, 1);
		}
		render->SetLineWidth(3);
	}
	render->SetLineWidth(1);
	for(int i=0; i<pos.data.size(); ++i){
		if (pos.data[i].norm() < 9000 && pos.data[i+1].norm() < 9000){
			render->DrawDirect(GRRender::LINES, &*pos.data.begin()+i, 2);
		}
	}
}
void TDiffFile::GetRecPos(TRecords*& rec, int &pos, int id, bool bLeft){
	int lr = bLeft ? 0 : 1;
	int rl = bLeft ? 1 : 0;
	if (id == 0){
		rec = &g1[lr];
		pos = 0;
	}else if (id == 1){
		rec = &g2[lr];
		pos = 1;
	}else if (id == 2){
		rec = &g1[lr];
		pos = 1;
	}else if (id == 3){
		rec = &g1[rl];
		pos = 8;
	}else if (4 <= id && id <= 9){
		rec = &g1[lr];
		pos = id-2;
	}else if (10 <= id && id <= 12){
		rec = &g2[lr];
		pos = id-6;
	}else if (id == 13){
		rec = &g2[lr];
		pos = 3;
	}
}

float TDiffFile::GetLinkLength(TDiffFile::TRecords& r1, int i1, TDiffFile::TRecords& r2, int i2){
	int i=0;
	while(1){
		for(; i<r1.size() && !IsValid(r1[i].data[i1]); ++i);
		assert(i < r1.size());
		TDiffRecord pos, vel;
		r2.GetRecord(pos, vel, r1[i].time);
		Vec3f pos1 = r1[i].data[i1];
		Vec3f pos2 = pos.data[i2];
		if (IsValid(pos2)){
			return (pos2 - pos1).norm();
		}
		++i;
	}
}
///	通し番号で計測点の位置を取得する．全14点
bool TDiffFile::GetRecord(Spr::Vec3f* pos, Spr::Vec3f* vel, bool bLeft, float time){
	bool rv = true;
	int lr = bLeft ? 0 : 1;
	int rl = bLeft ? 1 : 0;
	static TDiffRecord p1, v1;
	static TDiffRecord p2, v2;
	static TDiffRecord p3, v3;
	rv &= g1[lr].GetRecord(p1, v1, time);
	rv &= g2[lr].GetRecord(p2, v2, time);
	rv &= g1[rl].GetRecord(p3, v3, time);
	pos[0] = p1.data[0];
	pos[1] = p2.data[1];
	pos[2] = p1.data[1];
	pos[3] = p3.data[8];
	for(int i=4; i<=9; ++i) pos[i] = p1.data[i-2];
	for(int i=10; i<=12; ++i) pos[i] = p2.data[i-6];
	pos[13] = p2.data[3];

	vel[0] = v1.data[0];
	vel[1] = v2.data[1];
	vel[2] = v1.data[1];
	vel[3] = v3.data[8];
	for(int i=4; i<=9; ++i) vel[i] = v1.data[i-2];
	for(int i=10; i<=12; ++i) vel[i] = v2.data[i-6];
	vel[13] = v2.data[3];
	return rv;
}
float TDiffFile::GetLinkLength(int id1, int id2){
	return (GetLinkLength(id1, false, id2, false) 
		+ GetLinkLength(id1, true, id2, true)) / 2;
}
float TDiffFile::GetLinkLength(int id1, bool bLeft1, int id2, bool bLeft2){
	int ids[] = {id1, id2};
	bool bLefts[] = {bLeft1, bLeft2};
	TRecords* rec[2];
	int pos[2];
	for(int i=0; i<2; ++i){
		GetRecPos(rec[i], pos[i], ids[i], bLefts[i]);
	}
	return GetLinkLength(*rec[0], pos[0], *rec[1], pos[1]);
}
bool TDiffFile::AddSpringForce(THuman& human, float time, float dt){
	if (!IsLoaded() || !human.IsLoaded()) return false;
	static Vec3f pos[2][14], vel[2][14];
	bool rv=true;
	rv &= GetRecord(pos[0], vel[0], true,  time);
	rv &= GetRecord(pos[1], vel[1], false, time);
	const float SAFETYRATE = 0.03f;
	const float SPRING = 0.4f * SAFETYRATE;		//	バネ
	const float DAMPER = 0.9f * SAFETYRATE;		//	ダンパ
	for(int lr=0; lr<2; ++lr){
		for(int i=0+lr; i<human.springs[lr].size() && human.springs[lr][i].solid; ++i){
			float mass = human.springs[lr][i].solid->GetMass();
			float spring = SPRING * mass / (2*dt*dt);
			float damper = DAMPER * mass / (dt);

			Vec3f pos2 = human.springs[lr][i].GetPos();
			Vec3f vel2 = human.springs[lr][i].GetVel();
			if (IsValid(pos[lr][i])){
				Vec3f dPos = pos[lr][i] - pos2;
				Vec3f dVel = vel[lr][i] - vel2;
				if (dVel.norm() > 1e5f) dVel = dVel.unit()*1e5f;
				if (dPos.norm() > 1e2f) dPos = dPos.unit()*1e2f;
				Vec3f force = spring*dPos + damper*dVel;
				if (force.norm()/mass > 1e6f) force = force.unit() * mass*1e6f;
				human.springs[lr][i].AddForce(force);
			}
		}
	}
	return rv;
}
void TDiffFile::SetHumanScale(THuman& human){
	if (!IsLoaded()) return;
	float len01 = GetLinkLength(0, 1);
	Vec2f head, shoulder, body, hip;
	head.X() = GetLinkLength(1, false, 1, true);
	head.Y() = sqrt(Square(len01)-Square(head.X()/2));
	float len12 = GetLinkLength(1, 2);
	shoulder.X() = GetLinkLength(2, false, 2, true);
	shoulder.Y() = sqrt(Square(len12)-Square((shoulder.X()-head.X())/2));
	body.X() = GetLinkLength(3, false, 3, true);
	float len23 = GetLinkLength(2, 3);
	body.Y() = sqrt( Square(len23) - Square((shoulder.X()-body.X())/2) );
	hip.X() = GetLinkLength(4, false, 4, true);
	float len34 = GetLinkLength(3, 4);
	hip.Y() = sqrt( Square(len34) - Square((body.X()-hip.X())/2) );
	float thighLen = GetLinkLength(4,5);
	float shinLen = GetLinkLength(5,6);
	float footLen = GetLinkLength(8,9);
	float armLen = GetLinkLength(2, 10);
	float forearmLen = GetLinkLength(10, 11);
	float handLen = GetLinkLength(11, 12);
	float len78 = GetLinkLength(7,8);
	float len79 = GetLinkLength(7,9);
	Vec3f pos7;
	pos7.Z() = (Square(len79)-Square(len78)) / (2*footLen);
	pos7.X() = sqrt( Square(len79) - Square(footLen/2+pos7.Z()) );
	float la = GetLinkLength(6,8);
	float lb = GetLinkLength(6,9);
	float lp = GetLinkLength(6,7);
	Vec3f pos6;
	pos6.X() = -Square(footLen)/(8*pos7.X())
		+ Square(la)/(4*pos7.X()) + Square(lb)/(4*pos7.X())
		- Square(lp)/(2*pos7.X()) +  pos7.X()/2
		+ Square(la)*pos7.Z()/(2*footLen*pos7.X())
		- Square(lb)*pos7.Z()/(2*footLen*pos7.X())
		+ Square(pos7.Z())/(2*pos7.X());
	pos6.Z() = -(Square(la) - Square(lb)) / (2*footLen);
	pos6.Y() = sqrt(Square(lb) - Square(pos6.X()) - Square(footLen/2+pos6.Z()));
	Vec3f foot(pos7.X(), pos6.Y(), footLen);	//	足の大きさ
	human.SetScale(head, shoulder, body, hip, armLen, forearmLen, handLen, thighLen, shinLen, foot, pos6, pos7);
}
