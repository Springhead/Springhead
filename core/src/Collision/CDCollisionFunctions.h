#ifndef CDCOLLISIONFUNCTION_H
#define CDCOLLISIONFUNCTION_H
/*

	codes are refer from "Realtime collsion detection" written by Christer Ericson
*/


#include <Foundation/Object.h>

namespace Spr{
	;

	class CollisionFuntions{
	public:
		static int  IntersectSegmentSphere(Vec3f a, Vec3f unitb_aNome, float dista_b, Vec3f sphereCtr, float sphereR, float &t, Vec3f &q)
	{
		Vec3f m = a - sphereCtr;
		float b = m.dot(unitb_aNome);
		float c = m.dot(m) - sphereR * sphereR;
		// Exit if r痴 origin outside s (c > 0)and r pointing away from s (b > 0)
		if (c > 0.0f && b > 0.0f) return 0;
		float discr = b*b - c;
		// A negative discriminant corresponds to ray missing sphere
		if (discr < 0.0f) return 0;
		// Ray now found to intersect sphere, compute smallest t value of intersection
		t = -b - sqrt(discr);
		// If t is negative, ray started inside sphere so clamp t to zero
		if (t < 0.0f) t = 0.0f;

		q = a + t * unitb_aNome;
		if (t < dista_b)return 1;
		return 0;
	}

		static 	Vec3f  SaveTriLocalCoordinates(Vec3f p, Vec3f q, Vec3f a, Vec3f b, Vec3f c, float &u, float &v, float &w, float &t)
	{
		//int regionFlag, insideFlag;
		//Check if P in vertex region outside A
		Vec3f ab = b - a;
		Vec3f ac = c - a;
		Vec3f ap = p - a;
		float d1 = ab * ap;
		float d2 = ac * ap;
		//if (d1 <= 0.0f && d2 <= 0.0f) { regionFlag = 1; return a; }

		//Check if P in vertex region outside B
		Vec3f bp = p - b;
		float d3 = ab * bp;
		float d4 = ac * bp;
		//if(d3 >= 0.0f && d4 <= d3 ) {regionFlag = 2;return b;}

		//Vec3f bc = c - b;
		//float d4_ = bc * bp;
		//if (d3 >= 0.0f && d4_ <= 0.0f)  { regionFlag = 2; return b; }

		//Check if P in edge region of AB, then return the projection of P
		float vc = d1 * d4 - d3 * d2;
		if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
		{
			//float v = d1 / (d1 - d3);
			//{regionFlag = 11; }
			//return a + v * ab;
		}

		//Check if P in vertex region of outside C
		Vec3f cp = p - c;
		float d5 = ab * cp;
		float d6 = ac * cp;
		//if(d6 <=0.0f && d5 <= d6) {regionFlag = 3;return c;}

		//float d5_ = bc * cp;
		//if (d6 >= 0.0f && d5_ >= 0.0f)  { regionFlag = 3; return c; }

		// Check if P in edge region of AC, if so return projection of P onto AC
		float vb = d5*d2 - d1*d6;
		if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
			//float w = d2 / (d2 - d6);
			//{regionFlag = 12; }
			//return a + w * ac; // barycentric coordinates (1-w,0,w)
		}

		// Check if P in edge region of BC, if so return projection of P onto BC
		float va = d3*d6 - d5*d4;
		if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
			//float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			//{regionFlag = 13; }
			//return b + w * (c - b); // barycentric coordinates (0,1-w,w)
		}

		// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
		float denom = 1.0f / (va + vb + vc);
		v = vb * denom;
		w = vc * denom;
		//insideFlag = true;
		//{regionFlag = 0; }
		u = 1 - v - w;
		return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
	}

	// Given segment pq and triangle abc, returns whether segment intersects
	// triangle and if so, also returns the barycentric coordinates (u,v,w)
	// of the intersection point
		static  int  IntersectSegmentTriangle(Vec3f p, Vec3f q, Vec3f a, Vec3f b, Vec3f c, float &u, float &v, float &w, float &t)
	{


		Vec3f ab = b - a;
		Vec3f ac = c - a;
		Vec3f qp = p - q;

		// Compute triangle normal. Can be precalculated or cached if
		// intersecting multiple segments against the same triangle
		Vec3f n = cross(ab, ac);

		// Compute denominator d. If d <= 0, segment is parallel to or points
		// away from triangle, so exit early
		float d = qp.dot(n);
		if (d <= 0.0f) return 0;

		// Compute intersection t value of pq with plane of triangle. A ray
		// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
		// dividing by d until intersection has been found to pierce triangle
		Vec3f ap = p - a;
		t = ap.dot(n);
		if (t < 0.0f) return 0;
		if (t > d) return 0; // For segment; exclude this code line for a ray test

		// Compute barycentric coordinate components and test if within bounds
		Vec3f e = cross(qp, ap);
		v = ac.dot(e);
		if (v < 0.0f || v > d) return 0;
		w = -ab.dot(e);
		if (w < 0.0f || v + w > d) return 0;

		// Segment/ray intersects triangle. Perform delayed division and
		// compute the last barycentric coordinate component
		float ood = 1.0f / d;
		t *= ood;
		v *= ood;
		w *= ood;
		u = 1.0f - v - w;
		return 1;
	}

	// Clamp n to lie within the range [min, max]
		static float  Clamp(float n, float min, float max) {
		if (n < min) return min;
		if (n > max) return max;
		return n;
	}

	// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
	// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
	// distance between between S1(s) and S2(t)
		static float  ClosestPtSegmentSegment(Vec3f p1, Vec3f q1, Vec3f p2, Vec3f q2, float &s, float &t, Vec3f &c1, Vec3f &c2)
	{
		Vec3f d1 = q1 - p1; // Direction vector of segment S1
		Vec3f d2 = q2 - p2; // Direction vector of segment S2
		Vec3f r = p1 - p2;
		float a = d1.dot(d1); // Squared length of segment S1, always nonnegative
		float e = d2.dot(d2); // Squared length of segment S2, always nonnegative
		float f = d2.dot(r);
		float EPSILON = 50 * std::numeric_limits<float>::epsilon();

		// Check if either or both segments degenerate into points
		if (a <= EPSILON && e <= EPSILON) {
			// Both segments degenerate into points
			s = t = 0.0f;
			c1 = p1;
			c2 = p2;
			return (c1 - c2).dot(c1 - c2);
		}
		if (a <= EPSILON) {
			// First segment degenerates into a point
			s = 0.0f;
			t = f / e; // s = 0 => t = (b*s + f) / e = f / e
			t = Clamp(t, 0.0f, 1.0f);
		}
		else {
			float c = d1.dot(r);
			if (e <= EPSILON) {
				// Second segment degenerates into a point
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
			}
			else {
				//// The general nondegenerate case starts here
				float b = d1.dot(d2);
				float denom = a*e - b*b; // Always nonnegative

				// If segments not parallel, compute closest point on L1 to L2, and
				// clamp to segment S1. Else pick arbitrary s (here 0)
				if (denom != 0.0f) {
					s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
				}
				else s = 0.0f;

				//// Compute point on L2 closest to S1(s) using
				//// t = (P1+D1*s)-P2,D2) / D2,D2) = (b*s + f) / e
				//t = (b*s + f) / e;

				//// If t in [0,1] done. Else clamp t, recompute s for the new value
				//// of t using s = (P2+D2*t)-P1,D1) / D1,D1)= (t*b - c) / a
				//// and clamp s to [0, 1]
				//if (t < 0.0f) {
				//    t = 0.0f;
				//    s = Clamp(-c / a, 0.0f, 1.0f);
				//} else if (t > 1.0f) {
				//    t = 1.0f;
				//    s = Clamp((b - c) / a, 0.0f, 1.0f);
				//}

				float tnom = b*s + f;
				if (tnom < 0.0f) {
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (tnom > e) {
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
				else {
					t = tnom / e;
				}
			}
		}

		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		return (c1 - c2).dot(c1 - c2);
	}
	// Given segment ab and point c, computes closest point d on ab.
	// Also returns t for the position of d, d(t) = a + t*(b - a)
		static void  ClosestPtPointSegment(Vec3f c, Vec3f a, Vec3f b, float &t, Vec3f &d)
	{
		Vec3f ab = b - a;
		// Project c onto ab, computing parameterized position d(t) = a + t*(b  - a)
		t = (c - a).dot(ab) / ab.dot(ab);
		// If outside segment, clamp t (and therefore d) to the closest endpoint
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		// Compute projected position from the clamped t
		d = a + t * ab;
	}
		static void  SaveCylLocalCoordinates(Vec3f c, Vec3f a, Vec3f b, float &t, Vec3f &d)
	{
		Vec3f ab = b - a;
		// Project c onto ab, computing parameterized position d(t) = a + t*(b  - a)
		t = (c - a).dot(ab) / ab.dot(ab);
		// If outside segment, clamp t (and therefore d) to the closest endpoint
		//if (t < 0.0f) t = 0.0f;
		//if (t > 1.0f) t = 1.0f;
		// Compute projected position from the clamped t
		d = a + t * ab;
	}



	// Intersect segment S(t)=sa+t(sb-sa), 0<=t<=1 against cylinder specified by p, q and r
	static int  IntersectSegmentCylinder(Vec3f sa, Vec3f sb, Vec3f p, Vec3f q, float r, float &t)
	{
		Vec3f d = q - p, m = sa - p, n = sb - sa;
		float md = m.dot(d);
		float nd = n.dot(d);
		float dd = d.dot(d);
		// Test if segment fully outside either endcap of cylinder
		if (md < 0.0f && md + nd < 0.0f) return 0; // Segment outside 叢 - side of cylinder
		if (md > dd && md + nd > dd) return 0;     // Segment outside 倉 - side of cylinder
		float nn = n.dot(n);
		float mn = m.dot(n);
		float a = dd * nn - nd * nd;
		float k = m.dot(m) - r * r;
		float c = dd * k - md * md;
		float EPSILON = 50 * std::numeric_limits<float>::epsilon();
		if (fabs(a) < EPSILON) {
			// Segment runs parallel to cylinder axis
			if (c > 0.0f) return 0; // 疎 - and thus the segment lie outside cylinder
			// Now known that segment intersects cylinder; figure out how it intersects
			if (md < 0.0f) t = -mn / nn; // Intersect segment against 叢 - endcap
			else if (md > dd) t = (nd - mn) / nn; // Intersect segment against 倉 - endcap
			else t = 0.0f; // 疎 - lies inside cylinder
			return 1;
		}
		float b = dd * mn - nd * md;
		float discr = b * b - a * c;
		if (discr < 0.0f) return 0; // No real roots; no intersection
		t = (-b - sqrt(discr)) / a;
		if (t < 0.0f || t > 1.0f) return 0; // Intersection lies outside segment
		if (md + t * nd < 0.0f) {
			// Intersection outside cylinder on 叢 - side
			if (nd <= 0.0f) return 0; // Segment pointing away from endcap
			t = -md / nd;
			// Keep intersection ifS(t) - p, S(t) - p) <= r^2
			return k + 2 * t * (mn + t * nn) <= 0.0f;
		}
		else if (md + t * nd > dd) {
			// Intersection outside cylinder on 倉 - side
			if (nd >= 0.0f) return 0; // Segment pointing away from endcap
			t = (dd - md) / nd;
			// Keep intersection ifS(t) - q, S(t) - q) <= r^2
			return k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0f;
		}
		// Segment intersects cylinder between the end-caps; t is correct
		return 1;
	}

	static Vec3f  ClosestPtoTriangle(Vec3f p, Vec3f a, Vec3f b, Vec3f c, bool &insideFlag, int &regionFlag)
	{
		//Check if P in vertex region outside A
		Vec3f ab = b - a;
		Vec3f ac = c - a;
		Vec3f ap = p - a;
		float d1 = ab * ap;
		float d2 = ac * ap;
		if (d1 <= 0.0f && d2 <= 0.0f) { regionFlag = 1; return a; }

		//Check if P in vertex region outside B
		Vec3f bp = p - b;
		float d3 = ab * bp;
		float d4 = ac * bp;
		//if(d3 >= 0.0f && d4 <= d3 ) {regionFlag = 2;return b;}

		Vec3f bc = c - b;
		float d4_ = bc * bp;
		if (d3 >= 0.0f && d4_ <= 0.0f)  { regionFlag = 2; return b; }

		//Check if P in edge region of AB, then return the projection of P
		float vc = d1 * d4 - d3 * d2;
		if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
		{
			float v = d1 / (d1 - d3);
			{regionFlag = 11; }
			return a + v * ab;
		}

		//Check if P in vertex region of outside C
		Vec3f cp = p - c;
		float d5 = ab * cp;
		float d6 = ac * cp;
		//if(d6 <=0.0f && d5 <= d6) {regionFlag = 3;return c;}

		float d5_ = bc * cp;
		if (d6 >= 0.0f && d5_ >= 0.0f)  { regionFlag = 3; return c; }

		// Check if P in edge region of AC, if so return projection of P onto AC
		float vb = d5*d2 - d1*d6;
		if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
			float w = d2 / (d2 - d6);
			{regionFlag = 12; }
			return a + w * ac; // barycentric coordinates (1-w,0,w)
		}

		// Check if P in edge region of BC, if so return projection of P onto BC
		float va = d3*d6 - d5*d4;
		if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
			float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			{regionFlag = 13; }
			return b + w * (c - b); // barycentric coordinates (0,1-w,w)
		}

		// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		insideFlag = true;
		{regionFlag = 0; }
		return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w

	}

	};
}//namespace


#endif
