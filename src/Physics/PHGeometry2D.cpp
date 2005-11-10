#include "PHGeometry2D.h"
#include <algorithm>

namespace Spr{;

using namespace std;
using namespace PHG2;

double PHG2_Epsilon = 1.0e-3f;
const double PHG2_PI = 3.14159265358979f;

SGOBJECTIMP(PHGeometry2DEngine, SGBehaviorEngine);
SGOBJECTIMPABST(PHConstraint2D, SGObject);
SGOBJECTIMP(PHPointToPoint2D, PHConstraint2D);
SGOBJECTIMP(PHPointToLine2D, PHConstraint2D);
SGOBJECTIMP(PHPointToArc2D, PHConstraint2D);
SGOBJECTIMP(PHLineToLine2D, PHConstraint2D);
SGOBJECTIMP(PHLineToArc2D, PHConstraint2D);
SGOBJECTIMP(PHArcToArc2D, PHConstraint2D);
SGOBJECTIMP(PHParallel2D, PHConstraint2D);
SGOBJECTIMP(PHAngle2D, PHConstraint2D);
SGOBJECTIMP(PHFix2D, PHConstraint2D);

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PHGeometry2DEngine

bool PHGeometry2DEngine::AddChildObject(SGObject* o, SGScene* s){
	PHConstraint2D* con = DCAST(PHConstraint2D, o);
	if(con){
		Add(con);
		return true;
	}
	SGFrame* fr = DCAST(SGFrame, o);
	if(fr && !plane){
		plane = new PHG2Frame(fr);	//ìÆçÏïΩñ Ç∆Ç»ÇÈÉtÉåÅ[ÉÄ
		//frames.push_back(new PHG2Frame(plane));
	}
	return false;
}

bool PHGeometry2DEngine::DelChildObject(SGObject* o, SGScene* s){
	PHConstraint2D* con = DCAST(PHConstraint2D, o);
	if(con){
		Remove(con);
		return true;
	}
	if(plane->frame == DCAST(SGFrame, o))
		plane = 0;
	return false;
}

void PHGeometry2DEngine::Loaded(SGScene* scene){
	//äeconstraintÇ™éùÇ¡ÇƒÇ¢ÇÈÉtÉåÅ[ÉÄÇ÷ÇÃéQè∆ÇÉGÉìÉWÉìÇÃÉtÉåÅ[ÉÄÉäÉXÉgÇ…Ç‹Ç∆ÇﬂÇÈ
	PHConstraintList2D::iterator icon;
	for(icon = cons.begin(); icon != cons.end(); icon++){
		for(int i = 0; i < 2; i++){
			UTRef<PHG2Frame>& fr = (i == 0 ? (*icon)->lhs : (*icon)->rhs);

			PHG2FrameList::iterator ifr = frames.find(fr->frame);
			//Ç‹ÇæÉGÉìÉWÉìÇÃÉtÉåÅ[ÉÄÉäÉXÉgÇ…ìoò^Ç≥ÇÍÇƒÇ¢Ç»Ç¢èÍçá
			//í«â¡Ç∑ÇÈ
			if(ifr == frames.end())
				frames.push_back(fr);
			//ä˘Ç…ÉGÉìÉWÉìÇÃÉtÉåÅ[ÉÄÉäÉXÉgÇ…Ç†Ç¡ÇΩèÍçá
			//constraintÇ™ï€éùÇµÇƒÇ¢ÇΩï˚Çîjä¸ÇµÇƒÇ¬Ç»Ç¨Ç©Ç¶ÇÈ
			else fr = *ifr;
		}
	}
}

//ëSÉNÉäÉA
void PHGeometry2DEngine::Clear(SGScene* s){
	frames.clear();
	cons.clear();
}

//çSë©Çìoò^
PHConstraint2D* PHGeometry2DEngine::Add(PHConstraint2D* con){
	cons.push_back(con);
	return con;
}

//çSë©ÇçÌèú
void PHGeometry2DEngine::Remove(PHConstraint2D* con){
	cons.erase(find(cons.begin(), cons.end(), con));
}

//SolveÇÃëOèàóù
void PHGeometry2DEngine::_Preprocess(){
	cons_tmp = cons;
	plane->dof = PHG2_T0R0;
}

//SolveÇÃå„èàóù
void PHGeometry2DEngine::_Postprocess(){
	//à íuÅCåXÇ´ÇSGFrameÇ…îΩâfÇµÇƒÅCé©óRìxÇÉäÉZÉbÉg
	//ìÆçÏïΩñ ÉtÉåÅ[ÉÄÇÃposture
	Affinef afPlane = plane->frame->GetWorldPosture();

	PHG2FrameList::iterator it;
	PHG2Frame* fr;
	for(it = frames.begin(); it != frames.end(); it++){
		fr = *it;
		fr->frame->SetWorldPosture(
			afPlane * Affinef::Trn(fr->position.x, fr->position.y, 0.0f) * Affinef::Rot(fr->angle, 'z'));
		fr->dof = PHG2_T2R1;
	}
}

//çSë©Çâåà
void PHGeometry2DEngine::Step(SGScene* s){
 	_Preprocess();

	//åvéZñ{ëÃÅF
	bool bChanged;
	PHConstraintList2D	newcon;
	PHG2Result result;
	PHConstraintList2D::iterator itcon, itcon_tmp;

	//ïœâªÇ™ê∂Ç∂Ç»Ç≠Ç»ÇÈÇ‹Ç≈åJÇËï‘Ç∑
	do{
		bChanged = false;
		for(itcon = cons_tmp.begin(); itcon != cons_tmp.end();){
			//çSë©Çï]âø
			result = (*itcon)->Solve(newcon);

			//ïœâªÇµÇΩÇ©
			if(result != PHG2_REDUNDANT)
				bChanged = true;
			
			//êVÇµÇ¢çSë©
			switch(result){
			case PHG2_NEW:
				itcon_tmp = itcon;
				cons_tmp.insert(++itcon_tmp, newcon.begin(), newcon.end());
				newcon.clear();
				break;
			}

			//ï]âøÇµÇΩçSë©ÇÇ«Ç§Ç∑ÇÈÇ©
			switch(result){
			//Ç‡Ç§àÍìxï]âø
			case PHG2_AGAIN: break;
			//å„âÒÇµ
			case PHG2_REDUNDANT: itcon++; break;
			//è¡ãé
			default: itcon = cons_tmp.erase(itcon);
			}
		}
	}while(bChanged);
	
	_Postprocess();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// PHConstraint2D

bool PHConstraint2D::AddChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if(fr){
		if(!lhs){
			lhs = new PHG2Frame(fr);
			return true;
		}
		if(!rhs){
			rhs = new PHG2Frame(fr);
			return true;
		}
	}
	return false;
}

bool PHConstraint2D::DelChildObject(SGObject* o, SGScene* s){
	SGFrame* fr = DCAST(SGFrame, o);
	if(fr){
		if(fr == lhs->frame){
			lhs = 0;
			return true;
		}
		if(fr == rhs->frame){
			rhs = 0;
			return true;
		}
	}
	return false;
}

PHG2Result PHPointToPoint2D::Solve(PHG2Frame* plane, PHConstraintList2D& newcon){
	Vec2d pw = lhs ? lhs->toWorld(point_l) : point_l;
	Vec2d qw = rhs ? rhs->toWorld(point_r) : point_r;
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	switch(dof_lhs){
	case PHG2_T0R0:
		switch(dof_rhs){
		case PHG2_T0R0:
			return (pw - qw).norm() < PHG2_Epsilon ? PHG2_SOLVED : PHG2_OVER;
		case PHG2_T0R1:{
			//rhsÇçSë©íÜêSÇ‹ÇÌÇËÇ…âÒì]ÇµÇƒçSë©ÇñûÇΩÇ∑
			Vec2d c = rhs->center;
			Vec2d r0 = pw - c, r1 = qw - c;
			//çSë©íÜêSÇ∆çSë©ì_ÇÃÇ¢Ç∏ÇÍÇ©Ç™àÍívÇµÇƒÇ¢ÇƒÇÕÇ»ÇÁÇ»Ç¢
			if( r0.square() > 0 && r1.square() > 0){
				double phi = Line_Line_Angle(r1, r0);
				RotationAlong(&(rhs->position), &(rhs->angle), c, phi);
				rhs->dof = PHG2_T0R0;
				return PHG2_SOLVED;
			}
			else return PHG2_ILLPOSED;
		}
		case PHG2_T1R0:{
			//çSë©åÎç∑Ç™ç≈è¨Ç…Ç»ÇÈà íuÇ‹Ç≈rhsÇçSë©íºê¸Ç…âàÇ¡ÇƒïΩçsà⁄ìÆÇ≥ÇπÇÈ
			Vec2d y, pos;
			NearestPoint(&y, pw, qw, rhs->range1 - rhs->range0);
			//êVÇµÇ¢à íuÇ™çSë©ê¸ï™è„Ç…Ç†ÇÈèÍçáÇÕÇªÇ±Ç…à⁄ìÆ
			pos = rhs->position + (y - qw);
			if(Point_On_LineSegment(pos, rhs->range0, rhs->range1)){
				rhs->position = pos;
				rhs->dof = PHG2_T0R0;
				return PHG2_SOLVED;
			}
			return PHG2_ILLPOSED;
		}
		case PHG2_T2R0:
		case PHG2_T2R1:
			//rhsÇçSë©ÇñûÇΩÇ∑à íuÇ…ïΩçsà⁄ìÆ
			rhs->position += pw - qw;
			if(rhs->dof == PHG2_T2R1){
				rhs->center = pw;
				rhs->dof = PHG2_T0R1;
			}
			else rhs->dof = PHG2_T0R0;
			return PHG2_SOLVED;
		}
	case PHG2_T0R1:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(point_l, point_r); return PHG2_AGAIN;
		case PHG2_T0R1:{
			//ÇQâ~ÇÃåì_ÇãÅÇﬂÇÈ:
			Vec2d y0, y1;
			if(!Circle_Circle_Intersect(
				&y0, &y1,
				lhs->center, (lhs->center - pw).norm(),
				rhs->center, (rhs->center - qw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHPointToPoint2D(0, y0, lhs->frame, point_l));
			newcon.push_back(new PHPointToPoint2D(0, y0, rhs->frame, point_r));
			return PHG2_NEW;
		}
		case PHG2_T1R0:{
			//â~Ç∆íºê¸ÇÃåì_ÇãÅÇﬂÇÈÅF
			Vec2d y0, y1;
			if(!Line_Circle_Intersect(
				&y0, &y1,
				qw, rhs->range1 - rhs->range0,
				lhs->center, (lhs->center - pw).norm()))
				return PHG2_ILLPOSED;
			double d0 = (y0 - pw).square() + (y0 - qw).square();
			double d1 = (y1 - pw).square() + (y1 - qw).square();
			if(d0 > d1)y0 = y1;
			newcon.push_back(new PHPointToPoint2D(0, y0, lhs->frame, p));
			newcon.push_back(new PHPointToPoint2D(0, y0, rhs->frame, q));
			return PHG2_NEW;
		}
		case PHG2_T2R0:
		case PHG2_T2R1:
			return PHG2_REDUNDANT;
		}
	case PHG2_T1R0:
		switch(dof_rhs){
		case PHG2_T0R0:
		case PHG2_T0R1: 
			swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		case PHG2_T1R0:{
			//ìÒíºê¸ÇÃåì_ÇãÅÇﬂÇÈÅF
			Vec2d y;
			if(!Line_Line_Intersect(
				&y,
				pw, lhs->range1 - lhs->range0,
				qw, rhs->range1 - rhs->range0))
				return PHG2_ILLPOSED;
			//åì_Ç™óºé“ÇÃâ¬ìÆîÕàÕì‡Ç…Ç»ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢
			newcon.push_back(new PHPointToPoint2D(0, y, lhs->frame, p));
			newcon.push_back(new PHPointToPoint2D(0, y, rhs->frame, q));
			return PHG2_NEW;
		}
		default: return PHG2_REDUNDANT;
		}
	case PHG2_T2R0:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		default: return PHG2_REDUNDANT;
		}
	case PHG2_T2R1:
		switch(dof_rhs){
		case PHG2_T0R0: swap(lhs, rhs); swap(p, q); return PHG2_AGAIN;
		default: return PHG2_REDUNDANT;
		}
	}
	return PHG2_ERROR;
}

PHG2Result PHPointToLine2D::Solve(PHConstraintList2D& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw  = lhs ? lhs->toWorld(point) : point;
	Vec2d qw0 = rhs ? rhs->toWorld(line0) : line0;
	Vec2d qw1 = rhs ? rhs->toWorld(line1) : line1;
	//óºï˚Ç∆Ç‡T0R0ÇÃèÍçá
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)
		return Point_On_LineSegment(pw, qw0, qw1) ? PHG2_SOLVED : PHG2_OVER;
	//ï–ï˚Ç™T0R0Ç≈Ç‡Ç§àÍï˚Ç™T0R1ÇÃèÍçá
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//â~Ç∆íºê¸ÇÃåì_:
		Vec2d y0, y1, c = (dof_lhs == PHG2_T0R1) ? lhs->center : rhs->center;
		if(!Line_Circle_Intersect(
			&y0, &y1,
			qw0, qw1 - qw0,
			c, (pw - c).norm()))
			return PHG2_ILLPOSED;
		bool y0_online = Point_On_LineSegment(y0, qw0, qw1);
		bool y1_online = Point_On_LineSegment(y1, qw0, qw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_online){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_online){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	//ï–ï˚Ç™T0R0Ç≈Ç‡Ç§àÍï˚Ç™T1R0ÇÃèÍçá
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) ||
		(dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//ìÒíºê¸ÇÃåì_ÇãÅÇﬂÇÈÅF
		Vec2d y, dir = (dof_lhs == PHG2_T1R0) ? (lhs->range1 - lhs->range0) : (rhs->range1 - rhs->range0);
		if(!Line_Line_Intersect(&y, pw, dir, qw0, qw1 - qw0))
			return PHG2_ILLPOSED;
		//yÇ™ê¸ï™è„Ç…Ç»ÇØÇÍÇŒÇ»ÇÁÇ»Ç¢
		if(!Point_On_LineSegment(y, qw0, qw1))
			return PHG2_ILLPOSED;
		if(dof_lhs == PHG2_T1R0)
			 newcon.push_back(new PHPointToPoint2D(0, y, lhs->frame, p));
		else newcon.push_back(new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y)));
		return PHG2_NEW;
	}
	//ï–ï˚Ç™T0R0Ç≈Ç‡Ç§àÍï˚Ç™T2R0ÇÃèÍçá
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R0) ||
		(dof_lhs == PHG2_T2R0 && dof_rhs == PHG2_T0R0)){
		//ì_Ç™íºê¸Ç…èÊÇÈÇÊÇ§Ç…ïΩçsà⁄ìÆ
		PHG2Frame* obj = (dof_lhs == PHG2_T2R0) ? lhs : rhs;
		obj->position += (dof_lhs == PHG2_T2R0) ? (qw0 - pw) : (pw - qw0);
		obj->dof = PHG2_T1R0;
		obj->range0 = obj->position;
		obj->range1 = obj->position + (qw1 - qw0);
		return PHG2_SOLVED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result Spr::PHPointToArc2D::Solve(PHConstraintList2D& newcon){
	//ä»íPÇÃÇΩÇﬂÇ¢Ç∏ÇÍÇ©Ç™T0R0Ç≈ÅAÇ©Ç¬Ç‡Ç§àÍï˚Ç™T0R1Ç©T1R0ÇÃèÍçáÇÃÇ›âÇ≠
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R0)return PHG2_OVER;
	
	Vec2d pw = lhs ? lhs->toWorld(point) : point;
	Vec2d cw = rhs ? rhs->toWorld(center) : center;
	double sw0 = rhs ? (limit0 + rhs->angle) : limit0;
	double sw1 = rhs ? (limit1 + rhs->angle) : limit1;
	Vec2d y0, y1, y;

	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T0R1) || (dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R0)){
		//ÇQâ~ÇÃåì_
		Vec2d c = (dof_lhs == PHG2_T0R1 ? lhs->center : rhs->center);
		if(!Circle_Circle_Intersect(
			&y0, &y1,
			c, (pw - c).norm(),
			cw, radius))
			return PHG2_ILLPOSED;
		bool y0_onarc = Point_On_Arc(y0, cw, radius, sw0, sw1);
		bool y1_onarc = Point_On_Arc(y1, cw, radius, sw0, sw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_onarc){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T0R1 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	if((dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T1R0) || (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R0)){
		//â~Ç∆íºê¸Ç∆ÇÃåì_
		Vec2d dir = (dof_lhs == PHG2_T1R0 ? lhs->range1 - lhs->range0 : rhs->range1 - rhs->range0);
		Vec2d y0, y1;
		if(!Line_Circle_Intersect(
			&y0, &y1,
			pw, dir,
			cw, radius))
			return PHG2_ILLPOSED;
		bool y0_onarc = Point_On_Arc(y0, cw, radius, sw0, sw1);
		bool y1_onarc = Point_On_Arc(y1, cw, radius, sw0, sw1);
		if((y0 - pw).square() > (y1 - pw).square())
			y0 = y1;
		if(y0_onarc){
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHPointToPoint2D(0, y0, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y0)));
			return PHG2_NEW;
		}
		if(y1_onarc){
			newcon.push_back(dof_lhs == PHG2_T1R0 ? new PHPointToPoint2D(0, y1, lhs->frame, p) : new PHPointToPoint2D(0, pw, rhs->frame, rhs->toLocal(y1)));
			return PHG2_NEW;
		}
		return PHG2_ILLPOSED;
	}
	return PHG2_REDUNDANT;
}

PHG2Result PHLineToLine2D::Solve(PHConstraintList2D& newcon){
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	Vec2d pw0 = lhs ? lhs->toWorld(line0_l) : line0_l;
	Vec2d pw1 = lhs ? lhs->toWorld(line1_l) : line1_l;
	Vec2d qw0 = rhs ? rhs->toWorld(line0_r) : line0_r;
	Vec2d qw1 = rhs ? rhs->toWorld(line1_r) : line1_r;
	if( (dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R1) ||
		(dof_lhs == PHG2_T2R1 && dof_rhs == PHG2_T0R0)){
		//ñ¢é¿ëï
		return PHG2_REDUNDANT;
	}
	if( (dof_lhs == PHG2_T1R0 && dof_rhs == PHG2_T0R1) ||
		(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T1R0)){
		//ñ¢é¿ëï
		return PHG2_REDUNDANT;
	}
	if(dof_lhs == PHG2_T0R1 && dof_rhs == PHG2_T0R1){
		//ÇQâ~ÇÃê⁄ê¸ÇãÅÇﬂÇÈÅF
		Vec2d u, v;		//ê¸ï™è„ÇÃçSë©ì_Ç…ëŒÇ∑ÇÈç≈ãﬂì_
		Vec2d a0, a1, a2, a3, b0, b1, b2, b3;	//ÇQâ~ÇÃê⁄ì_
		NearestPoint(&u, lhs->center, pw0, pw1 - pw0);
		NearestPoint(&v, rhs->center, qw0, qw1 - qw0);
		int nline = Circle_Circle_Tangent(
			lhs->center, (u - lhs->center).norm(),
			rhs->center, (v - rhs->center).norm(),
			&a0, &a1, &a2, &a3,
			&b0, &b1, &b2, &b3);
		if(nline == 0)
			return PHG2_ILLPOSED;
		//ê⁄ê¸ÇÃì‡ç≈Ç‡ãﬂÇ¢Ç‡ÇÃÇëIÇÒÇ≈ê⁄ì_Ç÷ÇÃPTPçSë©Ç…ïœä∑
		double d0, d1;
		Vec2d a, b;
		a = a0, b = b0;
		d0 = (u - a).square() + (v - b).square();
		if(nline >= 2){
			d1 = (u - a1).square() + (v - b1).square();
			if(d0 > d1)a = a1, b = b1, d0 = d1;
		}
		if(nline >= 3){
			d1 = (u - a2).square() + (v - b2).square();
			if(d0 > d1)a = a2, b = b2, d0 = d1;
		}
		if(nline == 4){
			d1 = (u - a3).square() + (v - b3).square();
			if(d0 > d1)a = a3, b = b3;
		}
		newcon.push_back(new PHPointToPoint2D(0, a, lhs->frame, lhs->toLocal(u)));
		newcon.push_back(new PHPointToPoint2D(0, b, rhs->frame, rhs->toLocal(v)));
		return PHG2_NEW;
	}
	
	return PHG2_REDUNDANT;
}

PHG2Result PHLineToArc2D::Solve(PHConstraintList2D& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHArcToArc2D::Solve(PHConstraintList2D& newcon){
	return PHG2_REDUNDANT;
}

PHG2Result PHParallel2D::Solve(PHConstraintList2D& newcon){
	//lhs, rhsã§Ç…âÒì]é©óRìxÇéùÇ¬èÍçáÇÕèÁí∑
	//lhs, rhsã§Ç…âÒì]é©óRìxÇéùÇΩÇ»Ç¢èÍçáÇÕâﬂèË
	//Ç¢Ç∏ÇÍÇ©ÇÃÇ›âÒì]é©óRìxÇéùÇ¬èÍçáÇÕÇªÇøÇÁÇâÒì]ÇµÇƒçSë©Çâåà
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	bool revolutive_lhs = (dof_lhs == PHG2_T0R1 || dof_lhs == PHG2_T2R1);
	bool revolutive_rhs = (dof_rhs == PHG2_T0R1 || dof_rhs == PHG2_T2R1);
	if( revolutive_lhs &&  revolutive_rhs)return PHG2_REDUNDANT;
	if(!revolutive_lhs && !revolutive_rhs)return PHG2_OVER;
	if( revolutive_lhs && !revolutive_rhs){
		swap(lhs, rhs);
		swap(theta0, theta1);
		return PHG2_AGAIN;
	}
	double thetaw0 = lhs ? dir_l + lhs->angle : dir_l;
	double thetaw1 = rhs ? dir_r + rhs->angle : dir_r;
	
	//äpìxÇãÅÇﬂÇÈ
	double diff = thetaw1 - thetaw0;
	if(diff >  PHG2_PI / 2.0)diff -= PHG2_PI;
	if(diff < -PHG2_PI / 2.0)diff += PHG2_PI;
	switch(dof_rhs){
	case PHG2_T0R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->center, -diff);
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	case PHG2_T2R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->position, -diff);
		rhs->dof = PHG2_T2R0;
		return PHG2_SOLVED;
	}
	
	return PHG2_ERROR;
}

PHG2Result PHAngle2D::Solve(PHConstraintList2D& newcon){
	//lhs, rhsã§Ç…âÒì]é©óRìxÇéùÇ¬èÍçáÇÕèÁí∑
	//lhs, rhsã§Ç…âÒì]é©óRìxÇéùÇΩÇ»Ç¢èÍçáÇÕâﬂèË
	//Ç¢Ç∏ÇÍÇ©ÇÃÇ›âÒì]é©óRìxÇéùÇ¬èÍçáÇÕÇªÇøÇÁÇâÒì]ÇµÇƒçSë©Çâåà
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	bool revolutive_lhs = (dof_lhs == PHG2_T0R1 || dof_lhs == PHG2_T2R1);
	bool revolutive_rhs = (dof_rhs == PHG2_T0R1 || dof_rhs == PHG2_T2R1);
	if( revolutive_lhs &&  revolutive_rhs)return PHG2_REDUNDANT;
	if(!revolutive_lhs && !revolutive_rhs)return PHG2_OVER;
	if( revolutive_lhs && !revolutive_rhs){
		swap(lhs, rhs), swap(theta0, theta1), angle = -angle;
		return PHG2_AGAIN;
	}
	double thetaw0 = lhs ? offset_l + lhs->angle : offset_l;
	double thetaw1 = rhs ? offset_r + rhs->angle : offset_r;
	
	//äpìxÇãÅÇﬂÇÈ
	double diff = phi - (thetaw1 - thetaw0);
	switch(dof_rhs){
	case PHG2_T0R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->center, diff);
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	case PHG2_T2R1:
		RotationAlong(&(rhs->position), &(rhs->angle), rhs->position, diff);
		rhs->dof = PHG2_T2R0;
		return PHG2_SOLVED;
	}
	
	return PHG2_ERROR;
}

PHG2Result PHFix2D::Solve(PHConstraintList2D& newcon){
	//Ç¢Ç∏ÇÍÇ©Ç™T2R1Ç≈ëºï˚Ç™T0R0ÇÃèÍçáÇÃÇ›àµÇ§
	PHG2_DOF dof_lhs = lhs ? lhs->dof : PHG2_T0R0;
	PHG2_DOF dof_rhs = rhs ? rhs->dof : PHG2_T0R0;
	if(dof_lhs == PHG2_T0R0 && dof_rhs == PHG2_T2R1){
		rhs->position	= lhs ? lhs->toWorld(offset) : offset;
		rhs->angle		= lhs ? lhs->angle + angle : angle;
		rhs->dof = PHG2_T0R0;
		return PHG2_SOLVED;
	}
	else if(dof_lhs == PHG2_T2R1 && dof_rhs == PHG2_T0R0){
		swap(lhs, rhs);
		p = -Matrix2d::Rot(-theta) * p;
		theta = -theta;
		return PHG2_AGAIN;
	}
	else return PHG2_REDUNDANT;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Loader & Saver

//SGOBJECTIMP(Vector2, SGObject);

/*class Vector2Loader:public FIObjectLoader<Vector2>{
public:
	virtual bool LoadData(FILoadScene* ctx, Vector2* v){
		FIDocNodeBase* doc = ctx->docs.Top();
		doc->GetWholeData(*(XVector2*)v);
		return true;
	}
	Vector2Loader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Float);
		db->REG_RECORD_PROTO(XVector2);
	}
};

class Vector2Saver : public FIObjectSaver<Vector2>{
	virtual UTString GetType() const{ return "Vector2"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, Vector2 *v){
		doc->SetWholeData(*(XVector2*)v);
	}
};*/

class PHGeometry2DEngineLoader:public FIObjectLoader<PHGeometry2DEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHGeometry2DEngine* engine){
		return true;
	}
	PHGeometry2DEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(Float);
		db->REG_FIELD(Vector2);
		db->REG_RECORD_PROTO(XGeometry2DEngine);
	}
};

class PHGeometry2DEngineSaver : public FIBaseSaver{
	virtual UTString GetType() const{return "PHGeometry2DEngine";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHGeometry2DEngine* e = (PHGeometry2DEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHGeometry2DEngine", e);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		PHConstraintList2D::const_iterator it;
		for(it = e->Constraints().begin(); it != e->Constraints().end(); ++it){
			ctx->SaveRecursive((SGObject*)&**it);
		}
		ctx->docs.pop_back();
	}
};

#define DEF_LOADER(NAME)										\
class PH##NAME##Loader : public FIObjectLoader<PH##NAME>{		\
public:															\
	typedef X##NAME template_type;								\
	virtual bool LoadData(FILoadScene* ctx, PH##NAME *con){		\
		FIDocNodeBase* doc = ctx->docs.Top();					\
		doc->GetWholeData(*(template_type*)con);				\
		return true;											\
	}															\
	PH##NAME##Loader(){											\
		UTRef<FITypeDescDb> db = new FITypeDescDb;				\
		db->SetPrefix("X");										\
		db->REG_FIELD(Float);									\
		db->REG_FIELD(Vector2);									\
		db->REG_RECORD_PROTO(X##NAME);							\
	}															\
};

#define DEF_SAVER(NAME)															\
class PH##NAME##Saver : public FIObjectSaver<PH##NAME>{							\
	virtual UTString GetType() const{ return "PH"#NAME; }						\
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, PH##NAME *con){	\
		X##NAME dat;															\
		dat = *(X##NAME *)con;													\
		doc->SetWholeData(dat);													\
		doc->AddChild(ctx->CreateDocNode("REF", &*(con->lhs->frame)));			\
		doc->AddChild(ctx->CreateDocNode("REF", &*(con->rhs->frame)));			\
	}																			\
};

DEF_LOADER(PointToPoint2D)
DEF_LOADER(PointToLine2D)
DEF_LOADER(PointToArc2D)
DEF_LOADER(LineToLine2D)
DEF_LOADER(LineToArc2D)
DEF_LOADER(ArcToArc2D)
DEF_LOADER(Parallel2D)
DEF_LOADER(Angle2D)
DEF_LOADER(Fix2D)

DEF_SAVER(PointToPoint2D)
DEF_SAVER(PointToLine2D)
DEF_SAVER(PointToArc2D)
DEF_SAVER(LineToLine2D)
DEF_SAVER(LineToArc2D)
DEF_SAVER(ArcToArc2D)
DEF_SAVER(Parallel2D)
DEF_SAVER(Angle2D)
DEF_SAVER(Fix2D)

//DEF_REGISTER_BOTH(Vector2);
DEF_REGISTER_BOTH(PHGeometry2DEngine);
DEF_REGISTER_BOTH(PHPointToPoint2D);
DEF_REGISTER_BOTH(PHPointToLine2D);
DEF_REGISTER_BOTH(PHPointToArc2D);
DEF_REGISTER_BOTH(PHLineToLine2D);
DEF_REGISTER_BOTH(PHLineToArc2D);
DEF_REGISTER_BOTH(PHArcToArc2D);
DEF_REGISTER_BOTH(PHParallel2D);
DEF_REGISTER_BOTH(PHAngle2D);
DEF_REGISTER_BOTH(PHFix2D);

#undef DEF_LOADER
#undef DEF_SAVER

}