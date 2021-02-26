using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
#pragma warning disable 0108
namespace SprCs {
    public partial class PHMaterial : CsObject {
	public PHMaterial() { _thisArray[0] = SprExport.Spr_new_PHMaterial(); _flag = true; }
	public PHMaterial(IntPtr ptr) : base(ptr, 0, false) {}
	public PHMaterial(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public PHMaterial(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public PHMaterial(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public PHMaterial(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~PHMaterial() { if (_flag) { SprExport.Spr_delete_PHMaterial(_thisArray[0]); _flag = false; } }
	public float density {
	    get { return SprExport.Spr_PHMaterial_get_density(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_density(_thisArray[0], value); }
	}
	public float mu {
	    get { return SprExport.Spr_PHMaterial_get_mu(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_mu(_thisArray[0], value); }
	}
	public float mu0 {
	    get { return SprExport.Spr_PHMaterial_get_mu0(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_mu0(_thisArray[0], value); }
	}
	public float e {
	    get { return SprExport.Spr_PHMaterial_get_e(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_e(_thisArray[0], value); }
	}
	public float spring {
	    get { return SprExport.Spr_PHMaterial_get_spring(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_spring(_thisArray[0], value); }
	}
	public float damper {
	    get { return SprExport.Spr_PHMaterial_get_damper(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_damper(_thisArray[0], value); }
	}
	public float reflexSpring {
	    get { return SprExport.Spr_PHMaterial_get_reflexSpring(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_reflexSpring(_thisArray[0], value); }
	}
	public float reflexDamper {
	    get { return SprExport.Spr_PHMaterial_get_reflexDamper(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_reflexDamper(_thisArray[0], value); }
	}
	public float frictionSpring {
	    get { return SprExport.Spr_PHMaterial_get_frictionSpring(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_frictionSpring(_thisArray[0], value); }
	}
	public float frictionDamper {
	    get { return SprExport.Spr_PHMaterial_get_frictionDamper(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_frictionDamper(_thisArray[0], value); }
	}
	public float vibA {
	    get { return SprExport.Spr_PHMaterial_get_vibA(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_vibA(_thisArray[0], value); }
	}
	public float vibB {
	    get { return SprExport.Spr_PHMaterial_get_vibB(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_vibB(_thisArray[0], value); }
	}
	public float vibW {
	    get { return SprExport.Spr_PHMaterial_get_vibW(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_vibW(_thisArray[0], value); }
	}
	public float vibT {
	    get { return SprExport.Spr_PHMaterial_get_vibT(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_vibT(_thisArray[0], value); }
	}
	public bool vibContact {
	    get {
	        byte ret = (byte) SprExport.Spr_PHMaterial_get_vibContact(_thisArray[0]);
	        return (ret == 0) ? false : true;
	    }
	    set {
	        byte val = (byte) (value ? 1 : 0);
	        SprExport.Spr_PHMaterial_set_vibContact(_thisArray[0], (char) val);
	    }
	}
	public bool vibFric {
	    get {
	        byte ret = (byte) SprExport.Spr_PHMaterial_get_vibFric(_thisArray[0]);
	        return (ret == 0) ? false : true;
	    }
	    set {
	        byte val = (byte) (value ? 1 : 0);
	        SprExport.Spr_PHMaterial_set_vibFric(_thisArray[0], (char) val);
	    }
	}
	public float timeVaryFrictionA {
	    get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionA(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_timeVaryFrictionA(_thisArray[0], value); }
	}
	public float timeVaryFrictionB {
	    get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionB(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_timeVaryFrictionB(_thisArray[0], value); }
	}
	public float timeVaryFrictionC {
	    get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionC(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_timeVaryFrictionC(_thisArray[0], value); }
	}
	public float frictionViscosity {
	    get { return SprExport.Spr_PHMaterial_get_frictionViscosity(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_frictionViscosity(_thisArray[0], value); }
	}
	public float stribeckVelocity {
	    get { return SprExport.Spr_PHMaterial_get_stribeckVelocity(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_stribeckVelocity(_thisArray[0], value); }
	}
	public float stribeckmu {
	    get { return SprExport.Spr_PHMaterial_get_stribeckmu(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_stribeckmu(_thisArray[0], value); }
	}
        public vectorwrapper_float mus {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_mus(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_mus(_thisArray[0], value); }
        }
        public vectorwrapper_float mu0s {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_mu0s(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_mu0s(_thisArray[0], value); }
        }
        public vectorwrapper_float timeVaryFrictionAs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionAs(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionAs(_thisArray[0], value); }
        }
        public vectorwrapper_float timeVaryFrictionBs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionBs(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionBs(_thisArray[0], value); }
        }
        public vectorwrapper_float timeVaryFrictionDs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionDs(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionDs(_thisArray[0], value); }
        }
        public vectorwrapper_float timeVaryFrictionCs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionCs(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionCs(_thisArray[0], value); }
        }
        public vectorwrapper_float stribeckVelocitys {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_stribeckVelocitys(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_stribeckVelocitys(_thisArray[0], value); }
        }
        public vectorwrapper_float stribeckmus {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_stribeckmus(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_stribeckmus(_thisArray[0], value); }
        }
        public vectorwrapper_double c {
            get { return new vectorwrapper_double(SprExport.Spr_PHMaterial_addr_c(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_c(_thisArray[0], value); }
        }
        public vectorwrapper_float bristleK {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_bristleK(_thisArray[0])); }
            set { SprExport.Spr_PHMaterial_set_bristleK(_thisArray[0], value); }
        }
	public int velocityFieldMode {
	    get { return SprExport.Spr_PHMaterial_get_velocityFieldMode(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_velocityFieldMode(_thisArray[0], value); }
	}
	public Vec3d velocityFieldAxis {
	    get { return new Vec3d(SprExport.Spr_PHMaterial_addr_velocityFieldAxis(_thisArray[0])); }
	    set { SprExport.Spr_PHMaterial_set_velocityFieldAxis(_thisArray[0], value); }
	}
	public double velocityFieldMagnitude {
	    get { return SprExport.Spr_PHMaterial_get_velocityFieldMagnitude(_thisArray[0]); }
	    set { SprExport.Spr_PHMaterial_set_velocityFieldMagnitude(_thisArray[0], value); }
	}
	public Vec3d CalcVelocity(Vec3d pos, Vec3d normal) {
	    IntPtr ptr = SprExport.Spr_PHMaterial_CalcVelocity((IntPtr) _thisArray[0], (IntPtr) pos, (IntPtr) normal);
            return new Vec3d(ptr, true);
	}
    }
    [System.Serializable]
    public class PHMaterialStruct : CsObject {
        public PHMaterialStruct() {
            PHMaterial desc = new PHMaterial();
            ApplyFrom(desc, false);
        }
	public float density;
	public float mu;
	public float mu0;
	public float e;
	public float spring;
	public float damper;
	public float reflexSpring;
	public float reflexDamper;
	public float frictionSpring;
	public float frictionDamper;
	public float vibA;
	public float vibB;
	public float vibW;
	public float vibT;
	public bool vibContact;
	public bool vibFric;
	public float timeVaryFrictionA;
	public float timeVaryFrictionB;
	public float timeVaryFrictionC;
	public float frictionViscosity;
	public float stribeckVelocity;
	public float stribeckmu;
	public vectorwrapper_float mus;
	public vectorwrapper_float mu0s;
	public vectorwrapper_float timeVaryFrictionAs;
	public vectorwrapper_float timeVaryFrictionBs;
	public vectorwrapper_float timeVaryFrictionDs;
	public vectorwrapper_float timeVaryFrictionCs;
	public vectorwrapper_float stribeckVelocitys;
	public vectorwrapper_float stribeckmus;
	public vectorwrapper_double c;
	public vectorwrapper_float bristleK;
	public int velocityFieldMode;
	public Vec3dStruct velocityFieldAxis;
	public double velocityFieldMagnitude;
	public void ApplyTo(PHMaterial r, bool apply_to_base_class = false) {
		r.density = density;
		r.mu = mu;
		r.mu0 = mu0;
		r.e = e;
		r.spring = spring;
		r.damper = damper;
		r.reflexSpring = reflexSpring;
		r.reflexDamper = reflexDamper;
		r.frictionSpring = frictionSpring;
		r.frictionDamper = frictionDamper;
		r.vibA = vibA;
		r.vibB = vibB;
		r.vibW = vibW;
		r.vibT = vibT;
		r.vibContact = vibContact;
		r.vibFric = vibFric;
		r.timeVaryFrictionA = timeVaryFrictionA;
		r.timeVaryFrictionB = timeVaryFrictionB;
		r.timeVaryFrictionC = timeVaryFrictionC;
		r.frictionViscosity = frictionViscosity;
		r.stribeckVelocity = stribeckVelocity;
		r.stribeckmu = stribeckmu;
		r.velocityFieldMode = velocityFieldMode;
		r.velocityFieldAxis = velocityFieldAxis;
		r.velocityFieldMagnitude = velocityFieldMagnitude;
	}
	public void ApplyFrom(PHMaterial r, bool apply_to_base_class = false) {
		density = r.density;
		mu = r.mu;
		mu0 = r.mu0;
		e = r.e;
		spring = r.spring;
		damper = r.damper;
		reflexSpring = r.reflexSpring;
		reflexDamper = r.reflexDamper;
		frictionSpring = r.frictionSpring;
		frictionDamper = r.frictionDamper;
		vibA = r.vibA;
		vibB = r.vibB;
		vibW = r.vibW;
		vibT = r.vibT;
		vibContact = r.vibContact;
		vibFric = r.vibFric;
		timeVaryFrictionA = r.timeVaryFrictionA;
		timeVaryFrictionB = r.timeVaryFrictionB;
		timeVaryFrictionC = r.timeVaryFrictionC;
		frictionViscosity = r.frictionViscosity;
		stribeckVelocity = r.stribeckVelocity;
		stribeckmu = r.stribeckmu;
		velocityFieldMode = r.velocityFieldMode;
		velocityFieldAxis = r.velocityFieldAxis;
		velocityFieldMagnitude = r.velocityFieldMagnitude;
	}
	public static implicit operator PHMaterial(PHMaterialStruct m) {
	    PHMaterial r = new PHMaterial();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator PHMaterialStruct(PHMaterial r) {
	    PHMaterialStruct m = new PHMaterialStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDShapeDesc : CsObject {
	public CDShapeDesc() { _thisArray[0] = SprExport.Spr_new_CDShapeDesc(); _flag = true; }
	public CDShapeDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDShapeDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDShapeDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDShapeDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDShapeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDShapeDesc() { if (_flag) { SprExport.Spr_delete_CDShapeDesc(_thisArray[0]); _flag = false; } }
	public PHMaterial material {
	    get { return new PHMaterial(SprExport.Spr_CDShapeDesc_addr_material(_thisArray[0])); }
	    set { SprExport.Spr_CDShapeDesc_set_material(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDShapeDescStruct : CsObject {
        public CDShapeDescStruct() {
            CDShapeDesc desc = new CDShapeDesc();
            ApplyFrom(desc, false);
        }
	public PHMaterialStruct material;
	public void ApplyTo(CDShapeDesc r, bool apply_to_base_class = false) {
		r.material = material;
	}
	public void ApplyFrom(CDShapeDesc r, bool apply_to_base_class = false) {
		material = r.material;
	}
	public static implicit operator CDShapeDesc(CDShapeDescStruct m) {
	    CDShapeDesc r = new CDShapeDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDShapeDescStruct(CDShapeDesc r) {
	    CDShapeDescStruct m = new CDShapeDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDConvexMeshDesc : CDShapeDesc {
	public CDConvexMeshDesc() { _thisArray[0] = SprExport.Spr_new_CDConvexMeshDesc(); _flag = true; }
	public CDConvexMeshDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDConvexMeshDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDConvexMeshDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDConvexMeshDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDConvexMeshDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDConvexMeshDesc() { if (_flag) { SprExport.Spr_delete_CDConvexMeshDesc(_thisArray[0]); _flag = false; } }
        public vectorwrapper_Vec3f vertices {
            get { return new vectorwrapper_Vec3f(SprExport.Spr_CDConvexMeshDesc_addr_vertices(_thisArray[0])); }
            set { SprExport.Spr_CDConvexMeshDesc_set_vertices(_thisArray[0], value); }
        }
    }
    [System.Serializable]
    public class CDConvexMeshDescStruct : CDShapeDescStruct {
        public CDConvexMeshDescStruct() {
            CDConvexMeshDesc desc = new CDConvexMeshDesc();
            ApplyFrom(desc, true);
        }
	public vectorwrapper_Vec3f vertices;
	public void ApplyTo(CDConvexMeshDesc r, bool apply_to_base_class = false) {
		r.vertices = vertices;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDConvexMeshDesc r, bool apply_to_base_class = false) {
		vertices = r.vertices;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDConvexMeshDesc(CDConvexMeshDescStruct m) {
	    CDConvexMeshDesc r = new CDConvexMeshDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDConvexMeshDescStruct(CDConvexMeshDesc r) {
	    CDConvexMeshDescStruct m = new CDConvexMeshDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDConvexMeshInterpolateDesc : CDConvexMeshDesc {
	public CDConvexMeshInterpolateDesc() { _thisArray[0] = SprExport.Spr_new_CDConvexMeshInterpolateDesc(); _flag = true; }
	public CDConvexMeshInterpolateDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDConvexMeshInterpolateDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDConvexMeshInterpolateDesc() { if (_flag) { SprExport.Spr_delete_CDConvexMeshInterpolateDesc(_thisArray[0]); _flag = false; } }
    }
    [System.Serializable]
    public class CDConvexMeshInterpolateDescStruct : CDConvexMeshDescStruct {
        public CDConvexMeshInterpolateDescStruct() {
            CDConvexMeshInterpolateDesc desc = new CDConvexMeshInterpolateDesc();
            ApplyFrom(desc, true);
        }
	public void ApplyTo(CDConvexMeshInterpolateDesc r, bool apply_to_base_class = false) {
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDConvexMeshInterpolateDesc r, bool apply_to_base_class = false) {
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDConvexMeshInterpolateDesc(CDConvexMeshInterpolateDescStruct m) {
	    CDConvexMeshInterpolateDesc r = new CDConvexMeshInterpolateDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDConvexMeshInterpolateDescStruct(CDConvexMeshInterpolateDesc r) {
	    CDConvexMeshInterpolateDescStruct m = new CDConvexMeshInterpolateDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDSphereDesc : CDShapeDesc {
	public CDSphereDesc() { _thisArray[0] = SprExport.Spr_new_CDSphereDesc(); _flag = true; }
	public CDSphereDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDSphereDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDSphereDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDSphereDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDSphereDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDSphereDesc() { if (_flag) { SprExport.Spr_delete_CDSphereDesc(_thisArray[0]); _flag = false; } }
	public float radius {
	    get { return SprExport.Spr_CDSphereDesc_get_radius(_thisArray[0]); }
	    set { SprExport.Spr_CDSphereDesc_set_radius(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDSphereDescStruct : CDShapeDescStruct {
        public CDSphereDescStruct() {
            CDSphereDesc desc = new CDSphereDesc();
            ApplyFrom(desc, true);
        }
	public float radius;
	public void ApplyTo(CDSphereDesc r, bool apply_to_base_class = false) {
		r.radius = radius;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDSphereDesc r, bool apply_to_base_class = false) {
		radius = r.radius;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDSphereDesc(CDSphereDescStruct m) {
	    CDSphereDesc r = new CDSphereDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDSphereDescStruct(CDSphereDesc r) {
	    CDSphereDescStruct m = new CDSphereDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDEllipsoidDesc : CDShapeDesc {
	public CDEllipsoidDesc() { _thisArray[0] = SprExport.Spr_new_CDEllipsoidDesc(); _flag = true; }
	public CDEllipsoidDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDEllipsoidDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDEllipsoidDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDEllipsoidDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDEllipsoidDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDEllipsoidDesc() { if (_flag) { SprExport.Spr_delete_CDEllipsoidDesc(_thisArray[0]); _flag = false; } }
	public Vec3d radius {
	    get { return new Vec3d(SprExport.Spr_CDEllipsoidDesc_addr_radius(_thisArray[0])); }
	    set { SprExport.Spr_CDEllipsoidDesc_set_radius(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDEllipsoidDescStruct : CDShapeDescStruct {
        public CDEllipsoidDescStruct() {
            CDEllipsoidDesc desc = new CDEllipsoidDesc();
            ApplyFrom(desc, true);
        }
	public Vec3dStruct radius;
	public void ApplyTo(CDEllipsoidDesc r, bool apply_to_base_class = false) {
		r.radius = radius;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDEllipsoidDesc r, bool apply_to_base_class = false) {
		radius = r.radius;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDEllipsoidDesc(CDEllipsoidDescStruct m) {
	    CDEllipsoidDesc r = new CDEllipsoidDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDEllipsoidDescStruct(CDEllipsoidDesc r) {
	    CDEllipsoidDescStruct m = new CDEllipsoidDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDCapsuleDesc : CDShapeDesc {
	public CDCapsuleDesc() { _thisArray[0] = SprExport.Spr_new_CDCapsuleDesc(); _flag = true; }
	public CDCapsuleDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDCapsuleDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDCapsuleDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDCapsuleDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDCapsuleDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDCapsuleDesc() { if (_flag) { SprExport.Spr_delete_CDCapsuleDesc(_thisArray[0]); _flag = false; } }
	public float radius {
	    get { return SprExport.Spr_CDCapsuleDesc_get_radius(_thisArray[0]); }
	    set { SprExport.Spr_CDCapsuleDesc_set_radius(_thisArray[0], value); }
	}
	public float length {
	    get { return SprExport.Spr_CDCapsuleDesc_get_length(_thisArray[0]); }
	    set { SprExport.Spr_CDCapsuleDesc_set_length(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDCapsuleDescStruct : CDShapeDescStruct {
        public CDCapsuleDescStruct() {
            CDCapsuleDesc desc = new CDCapsuleDesc();
            ApplyFrom(desc, true);
        }
	public float radius;
	public float length;
	public void ApplyTo(CDCapsuleDesc r, bool apply_to_base_class = false) {
		r.radius = radius;
		r.length = length;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDCapsuleDesc r, bool apply_to_base_class = false) {
		radius = r.radius;
		length = r.length;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDCapsuleDesc(CDCapsuleDescStruct m) {
	    CDCapsuleDesc r = new CDCapsuleDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDCapsuleDescStruct(CDCapsuleDesc r) {
	    CDCapsuleDescStruct m = new CDCapsuleDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDRoundConeDesc : CDShapeDesc {
	public CDRoundConeDesc() { _thisArray[0] = SprExport.Spr_new_CDRoundConeDesc(); _flag = true; }
	public CDRoundConeDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDRoundConeDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDRoundConeDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDRoundConeDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDRoundConeDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDRoundConeDesc() { if (_flag) { SprExport.Spr_delete_CDRoundConeDesc(_thisArray[0]); _flag = false; } }
	public Vec2f radius {
	    get { return new Vec2f(SprExport.Spr_CDRoundConeDesc_addr_radius(_thisArray[0])); }
	    set { SprExport.Spr_CDRoundConeDesc_set_radius(_thisArray[0], value); }
	}
	public float length {
	    get { return SprExport.Spr_CDRoundConeDesc_get_length(_thisArray[0]); }
	    set { SprExport.Spr_CDRoundConeDesc_set_length(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDRoundConeDescStruct : CDShapeDescStruct {
        public CDRoundConeDescStruct() {
            CDRoundConeDesc desc = new CDRoundConeDesc();
            ApplyFrom(desc, true);
        }
	public Vec2fStruct radius;
	public float length;
	public void ApplyTo(CDRoundConeDesc r, bool apply_to_base_class = false) {
		r.radius = radius;
		r.length = length;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDRoundConeDesc r, bool apply_to_base_class = false) {
		radius = r.radius;
		length = r.length;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDRoundConeDesc(CDRoundConeDescStruct m) {
	    CDRoundConeDesc r = new CDRoundConeDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDRoundConeDescStruct(CDRoundConeDesc r) {
	    CDRoundConeDescStruct m = new CDRoundConeDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDBoxDesc : CDShapeDesc {
	public CDBoxDesc() { _thisArray[0] = SprExport.Spr_new_CDBoxDesc(); _flag = true; }
	public CDBoxDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDBoxDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDBoxDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDBoxDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDBoxDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public CDBoxDesc(Vec3d bs) { _thisArray[0] = SprExport.Spr_new_CDBoxDesc_2(bs); _flag = true; }
	~CDBoxDesc() { if (_flag) { SprExport.Spr_delete_CDBoxDesc(_thisArray[0]); _flag = false; } }
	public Vec3f boxsize {
	    get { return new Vec3f(SprExport.Spr_CDBoxDesc_addr_boxsize(_thisArray[0])); }
	    set { SprExport.Spr_CDBoxDesc_set_boxsize(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDBoxDescStruct : CDShapeDescStruct {
        public CDBoxDescStruct() {
            CDBoxDesc desc = new CDBoxDesc();
            ApplyFrom(desc, true);
        }
	public Vec3fStruct boxsize;
	public void ApplyTo(CDBoxDesc r, bool apply_to_base_class = false) {
		r.boxsize = boxsize;
		if (apply_to_base_class) {
		    base.ApplyTo(r, apply_to_base_class);
		}
	}
	public void ApplyFrom(CDBoxDesc r, bool apply_to_base_class = false) {
		boxsize = r.boxsize;
		if (apply_to_base_class) {
		    base.ApplyFrom(r, apply_to_base_class);
		}
	}
	public static implicit operator CDBoxDesc(CDBoxDescStruct m) {
	    CDBoxDesc r = new CDBoxDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDBoxDescStruct(CDBoxDesc r) {
	    CDBoxDescStruct m = new CDBoxDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDShapePairState : CsObject {
	public CDShapePairState() { _thisArray[0] = SprExport.Spr_new_CDShapePairState(); _flag = true; }
	public CDShapePairState(IntPtr ptr) : base(ptr, 0, false) {}
	public CDShapePairState(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDShapePairState(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDShapePairState(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDShapePairState(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDShapePairState() { if (_flag) { SprExport.Spr_delete_CDShapePairState(_thisArray[0]); _flag = false; } }
	public Vec3d normal {
	    get { return new Vec3d(SprExport.Spr_CDShapePairState_addr_normal(_thisArray[0])); }
	    set { SprExport.Spr_CDShapePairState_set_normal(_thisArray[0], value); }
	}
	public double depth {
	    get { return SprExport.Spr_CDShapePairState_get_depth(_thisArray[0]); }
	    set { SprExport.Spr_CDShapePairState_set_depth(_thisArray[0], value); }
	}
	public uint lastContactCount {
	    get { return SprExport.Spr_CDShapePairState_get_lastContactCount(_thisArray[0]); }
	    set { SprExport.Spr_CDShapePairState_set_lastContactCount(_thisArray[0], value); }
	}
    }
    [System.Serializable]
    public class CDShapePairStateStruct : CsObject {
        public CDShapePairStateStruct() {
            CDShapePairState desc = new CDShapePairState();
            ApplyFrom(desc, false);
        }
	public Vec3dStruct normal;
	public double depth;
	public uint lastContactCount;
	public void ApplyTo(CDShapePairState r, bool apply_to_base_class = false) {
		r.normal = normal;
		r.depth = depth;
		r.lastContactCount = lastContactCount;
	}
	public void ApplyFrom(CDShapePairState r, bool apply_to_base_class = false) {
		normal = r.normal;
		depth = r.depth;
		lastContactCount = r.lastContactCount;
	}
	public static implicit operator CDShapePairState(CDShapePairStateStruct m) {
	    CDShapePairState r = new CDShapePairState();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDShapePairStateStruct(CDShapePairState r) {
	    CDShapePairStateStruct m = new CDShapePairStateStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDBounds : CsObject {
	public CDBounds() { _thisArray[0] = SprExport.Spr_new_CDBounds(); _flag = true; }
	public CDBounds(IntPtr ptr) : base(ptr, 0, false) {}
	public CDBounds(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDBounds(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDBounds(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDBounds(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public CDBounds(Vec3d min0, Vec3d max0) { _thisArray[0] = SprExport.Spr_new_CDBounds_3(min0, max0); _flag = true; }
	~CDBounds() { if (_flag) { SprExport.Spr_delete_CDBounds(_thisArray[0]); _flag = false; } }
	public Vec3f min {
	    get { return new Vec3f(SprExport.Spr_CDBounds_addr_min(_thisArray[0])); }
	    set { SprExport.Spr_CDBounds_set_min(_thisArray[0], value); }
	}
	public Vec3f max {
	    get { return new Vec3f(SprExport.Spr_CDBounds_addr_max(_thisArray[0])); }
	    set { SprExport.Spr_CDBounds_set_max(_thisArray[0], value); }
	}
	public void set(Vec3d min0, Vec3d max0) {
	    SprExport.Spr_CDBounds_set((IntPtr) _thisArray[0], (IntPtr) min0, (IntPtr) max0);
	}
	public void clamp(Vec3f pos) {
	    SprExport.Spr_CDBounds_clamp((IntPtr) _thisArray[0], (IntPtr) pos);
	}
	public bool isEmpty() {
	    char ret = SprExport.Spr_CDBounds_isEmpty((IntPtr) _thisArray[0]);
	    return (ret == 0) ? false : true;
	}
	public void minimum(Vec3f self, Vec3f other) {
	    SprExport.Spr_CDBounds_minimum((IntPtr) _thisArray[0], (IntPtr) self, (IntPtr) other);
	}
	public void maximum(Vec3f self, Vec3f other) {
	    SprExport.Spr_CDBounds_maximum((IntPtr) _thisArray[0], (IntPtr) self, (IntPtr) other);
	}
    }
    public partial class CDSpatialHashTableDesc : CsObject {
	public CDSpatialHashTableDesc() { _thisArray[0] = SprExport.Spr_new_CDSpatialHashTableDesc(); _flag = true; }
	public CDSpatialHashTableDesc(IntPtr ptr) : base(ptr, 0, false) {}
	public CDSpatialHashTableDesc(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDSpatialHashTableDesc(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDSpatialHashTableDesc(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDSpatialHashTableDesc(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	~CDSpatialHashTableDesc() { if (_flag) { SprExport.Spr_delete_CDSpatialHashTableDesc(_thisArray[0]); _flag = false; } }
    }
    [System.Serializable]
    public class CDSpatialHashTableDescStruct : CsObject {
        public CDSpatialHashTableDescStruct() {
            CDSpatialHashTableDesc desc = new CDSpatialHashTableDesc();
            ApplyFrom(desc, false);
        }
	public void ApplyTo(CDSpatialHashTableDesc r, bool apply_to_base_class = false) {
	}
	public void ApplyFrom(CDSpatialHashTableDesc r, bool apply_to_base_class = false) {
	}
	public static implicit operator CDSpatialHashTableDesc(CDSpatialHashTableDescStruct m) {
	    CDSpatialHashTableDesc r = new CDSpatialHashTableDesc();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator CDSpatialHashTableDescStruct(CDSpatialHashTableDesc r) {
	    CDSpatialHashTableDescStruct m = new CDSpatialHashTableDescStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class CDShapeIf : NamedObjectIf {
	public CDShapeIf() { _thisArray[0] = SprExport.Spr_new_CDShapeIf(); _flag = true; }
	public CDShapeIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDShapeIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDShapeIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDShapeIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDShapeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDShapeIf(CsCastObject target) {
		return (target._info.Inherit(CDShapeIf.GetIfInfoStatic()) ? new CDShapeIf(target._thisArray[0], target._flag) : null);
	}
	~CDShapeIf() { if (_flag) { SprExport.Spr_delete_CDShapeIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDShapeIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDShapeIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDShapeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public void SetStaticFriction(float mu0) {
	    SprExport.Spr_CDShapeIf_SetStaticFriction((IntPtr) _thisArray[0], (float) mu0);
	}
	public float GetStaticFriction() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetStaticFriction((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetDynamicFriction(float mu) {
	    SprExport.Spr_CDShapeIf_SetDynamicFriction((IntPtr) _thisArray[0], (float) mu);
	}
	public float GetDynamicFriction() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetDynamicFriction((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetElasticity(float e) {
	    SprExport.Spr_CDShapeIf_SetElasticity((IntPtr) _thisArray[0], (float) e);
	}
	public float GetElasticity() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetElasticity((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetDensity(float d) {
	    SprExport.Spr_CDShapeIf_SetDensity((IntPtr) _thisArray[0], (float) d);
	}
	public float GetDensity() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetDensity((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetContactSpring(float K) {
	    SprExport.Spr_CDShapeIf_SetContactSpring((IntPtr) _thisArray[0], (float) K);
	}
	public float GetContactSpring() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetContactSpring((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetContactDamper(float D) {
	    SprExport.Spr_CDShapeIf_SetContactDamper((IntPtr) _thisArray[0], (float) D);
	}
	public float GetContactDamper() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetContactDamper((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetReflexSpring(float K) {
	    SprExport.Spr_CDShapeIf_SetReflexSpring((IntPtr) _thisArray[0], (float) K);
	}
	public float GetReflexSpring() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetReflexSpring((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetReflexDamper(float D) {
	    SprExport.Spr_CDShapeIf_SetReflexDamper((IntPtr) _thisArray[0], (float) D);
	}
	public float GetReflexDamper() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetReflexDamper((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetFrictionSpring(float K) {
	    SprExport.Spr_CDShapeIf_SetFrictionSpring((IntPtr) _thisArray[0], (float) K);
	}
	public float GetFrictionSpring() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetFrictionSpring((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetFrictionDamper(float D) {
	    SprExport.Spr_CDShapeIf_SetFrictionDamper((IntPtr) _thisArray[0], (float) D);
	}
	public float GetFrictionDamper() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetFrictionDamper((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetVibration(float vibA, float vibB, float vibW) {
	    SprExport.Spr_CDShapeIf_SetVibration((IntPtr) _thisArray[0], (float) vibA, (float) vibB, (float) vibW);
	}
	public void SetVibA(float vibA) {
	    SprExport.Spr_CDShapeIf_SetVibA((IntPtr) _thisArray[0], (float) vibA);
	}
	public float GetVibA() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetVibA((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetVibB(float vibB) {
	    SprExport.Spr_CDShapeIf_SetVibB((IntPtr) _thisArray[0], (float) vibB);
	}
	public float GetVibB() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetVibB((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetVibW(float vibW) {
	    SprExport.Spr_CDShapeIf_SetVibW((IntPtr) _thisArray[0], (float) vibW);
	}
	public float GetVibW() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetVibW((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetVibT(float vibT) {
	    SprExport.Spr_CDShapeIf_SetVibT((IntPtr) _thisArray[0], (float) vibT);
	}
	public float GetVibT() {
	    float result = (float) SprExport.Spr_CDShapeIf_GetVibT((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetVibContact(bool vibContact) {
	    SprExport.Spr_CDShapeIf_SetVibContact((IntPtr) _thisArray[0], (bool) vibContact);
	}
	public bool GetVibContact() {
	    char ret = SprExport.Spr_CDShapeIf_GetVibContact((IntPtr) _thisArray[0]);
	    return (ret == 0) ? false : true;
	}
	public PHMaterial GetMaterial() {
	    IntPtr ptr = SprExport.Spr_CDShapeIf_GetMaterial((IntPtr) _thisArray[0]);
            return new PHMaterial(ptr, true);
	}
	public void SetMaterial(PHMaterial mat) {
	    SprExport.Spr_CDShapeIf_SetMaterial((IntPtr) _thisArray[0], (IntPtr) mat);
	}
	public float CalcVolume() {
	    float result = (float) SprExport.Spr_CDShapeIf_CalcVolume((IntPtr) _thisArray[0]);
	    return result;
	}
	public Vec3f CalcCenterOfMass() {
	    IntPtr ptr = SprExport.Spr_CDShapeIf_CalcCenterOfMass((IntPtr) _thisArray[0]);
            return new Vec3f(ptr, true);
	}
	public Matrix3f CalcMomentOfInertia() {
	    IntPtr ptr = SprExport.Spr_CDShapeIf_CalcMomentOfInertia((IntPtr) _thisArray[0]);
            return new Matrix3f(ptr, true);
	}
	public bool IsInside(Vec3f p) {
	    char ret = SprExport.Spr_CDShapeIf_IsInside((IntPtr) _thisArray[0], (IntPtr) p);
	    return (ret == 0) ? false : true;
	}
	public void CalcBBox(Vec3f bbmin, Vec3f bbmax, Posed pose) {
	    SprExport.Spr_CDShapeIf_CalcBBox((IntPtr) _thisArray[0], (IntPtr) bbmin, (IntPtr) bbmax, (IntPtr) pose);
	}
	public void CalcBBox(Vec3f bbmin, Vec3f bbmax) {
	    SprExport.Spr_CDShapeIf_CalcBBox_1((IntPtr) _thisArray[0], (IntPtr) bbmin, (IntPtr) bbmax);
	}
    }
    public partial class CDConvexIf : CDShapeIf {
	public CDConvexIf() { _thisArray[0] = SprExport.Spr_new_CDConvexIf(); _flag = true; }
	public CDConvexIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDConvexIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDConvexIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDConvexIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDConvexIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDConvexIf(CsCastObject target) {
		return (target._info.Inherit(CDConvexIf.GetIfInfoStatic()) ? new CDConvexIf(target._thisArray[0], target._flag) : null);
	}
	~CDConvexIf() { if (_flag) { SprExport.Spr_delete_CDConvexIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDConvexIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDConvexIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public double CurvatureRadius(Vec3d p) {
	    double result = (double) SprExport.Spr_CDConvexIf_CurvatureRadius((IntPtr) _thisArray[0], (IntPtr) p);
	    return result;
	}
	public Vec3d Normal(Vec3d p) {
	    IntPtr ptr = SprExport.Spr_CDConvexIf_Normal((IntPtr) _thisArray[0], (IntPtr) p);
            return new Vec3d(ptr, true);
	}
    }
    public partial class CDFaceIf : ObjectIf {
	public CDFaceIf() { _thisArray[0] = SprExport.Spr_new_CDFaceIf(); _flag = true; }
	public CDFaceIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDFaceIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDFaceIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDFaceIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDFaceIf(CsCastObject target) {
		return (target._info.Inherit(CDFaceIf.GetIfInfoStatic()) ? new CDFaceIf(target._thisArray[0], target._flag) : null);
	}
	~CDFaceIf() { if (_flag) { SprExport.Spr_delete_CDFaceIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDFaceIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDFaceIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDFaceIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDFaceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public int NIndex() {
	    int result = (int) SprExport.Spr_CDFaceIf_NIndex((IntPtr) _thisArray[0]);
	    return result;
	}
	public int GetIndices() {
	    int result = (int) SprExport.Spr_CDFaceIf_GetIndices((IntPtr) _thisArray[0]);
	    return result;
	}
    }
    public partial class CDQuadFaceIf : ObjectIf {
	public CDQuadFaceIf() { _thisArray[0] = SprExport.Spr_new_CDQuadFaceIf(); _flag = true; }
	public CDQuadFaceIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDQuadFaceIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDQuadFaceIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDQuadFaceIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDQuadFaceIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDQuadFaceIf(CsCastObject target) {
		return (target._info.Inherit(CDQuadFaceIf.GetIfInfoStatic()) ? new CDQuadFaceIf(target._thisArray[0], target._flag) : null);
	}
	~CDQuadFaceIf() { if (_flag) { SprExport.Spr_delete_CDQuadFaceIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDQuadFaceIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDQuadFaceIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDQuadFaceIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDQuadFaceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public int NIndex() {
	    int result = (int) SprExport.Spr_CDQuadFaceIf_NIndex((IntPtr) _thisArray[0]);
	    return result;
	}
	public int GetIndices() {
	    int result = (int) SprExport.Spr_CDQuadFaceIf_GetIndices((IntPtr) _thisArray[0]);
	    return result;
	}
    }
    public partial class CDConvexMeshIf : CDConvexIf {
	public CDConvexMeshIf() { _thisArray[0] = SprExport.Spr_new_CDConvexMeshIf(); _flag = true; }
	public CDConvexMeshIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDConvexMeshIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDConvexMeshIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDConvexMeshIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDConvexMeshIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDConvexMeshIf(CsCastObject target) {
		return (target._info.Inherit(CDConvexMeshIf.GetIfInfoStatic()) ? new CDConvexMeshIf(target._thisArray[0], target._flag) : null);
	}
	~CDConvexMeshIf() { if (_flag) { SprExport.Spr_delete_CDConvexMeshIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexMeshIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public CDFaceIf GetFace(int i) {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetFace((IntPtr) _thisArray[0], (int) i);
            if (ptr == IntPtr.Zero) { return null; } 
            CDFaceIf obj = new CDFaceIf(ptr, 0);
            return obj;
	}
	public int NFace() {
	    int result = (int) SprExport.Spr_CDConvexMeshIf_NFace((IntPtr) _thisArray[0]);
	    return result;
	}
	public arraywrapper_Vec3f GetVertices() {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetVertices((IntPtr) _thisArray[0]);
            return new arraywrapper_Vec3f(ptr);
	}
	public int NVertex() {
	    int result = (int) SprExport.Spr_CDConvexMeshIf_NVertex((IntPtr) _thisArray[0]);
	    return result;
	}
    }
    public partial class CDConvexMeshInterpolateIf : CDConvexMeshIf {
	public CDConvexMeshInterpolateIf() { _thisArray[0] = SprExport.Spr_new_CDConvexMeshInterpolateIf(); _flag = true; }
	public CDConvexMeshInterpolateIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDConvexMeshInterpolateIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDConvexMeshInterpolateIf(CsCastObject target) {
		return (target._info.Inherit(CDConvexMeshInterpolateIf.GetIfInfoStatic()) ? new CDConvexMeshInterpolateIf(target._thisArray[0], target._flag) : null);
	}
	~CDConvexMeshInterpolateIf() { if (_flag) { SprExport.Spr_delete_CDConvexMeshInterpolateIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexMeshInterpolateIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
    }
    public partial class CDSphereIf : CDConvexIf {
	public CDSphereIf() { _thisArray[0] = SprExport.Spr_new_CDSphereIf(); _flag = true; }
	public CDSphereIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDSphereIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDSphereIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDSphereIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDSphereIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDSphereIf(CsCastObject target) {
		return (target._info.Inherit(CDSphereIf.GetIfInfoStatic()) ? new CDSphereIf(target._thisArray[0], target._flag) : null);
	}
	~CDSphereIf() { if (_flag) { SprExport.Spr_delete_CDSphereIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDSphereIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDSphereIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDSphereIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDSphereIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public float GetRadius() {
	    float result = (float) SprExport.Spr_CDSphereIf_GetRadius((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetRadius(float r) {
	    SprExport.Spr_CDSphereIf_SetRadius((IntPtr) _thisArray[0], (float) r);
	}
    }
    public partial class CDEllipsoidIf : CDConvexIf {
	public CDEllipsoidIf() { _thisArray[0] = SprExport.Spr_new_CDEllipsoidIf(); _flag = true; }
	public CDEllipsoidIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDEllipsoidIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDEllipsoidIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDEllipsoidIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDEllipsoidIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDEllipsoidIf(CsCastObject target) {
		return (target._info.Inherit(CDEllipsoidIf.GetIfInfoStatic()) ? new CDEllipsoidIf(target._thisArray[0], target._flag) : null);
	}
	~CDEllipsoidIf() { if (_flag) { SprExport.Spr_delete_CDEllipsoidIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDEllipsoidIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDEllipsoidIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public Vec3d GetRadius() {
	    IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetRadius((IntPtr) _thisArray[0]);
            return new Vec3d(ptr, true);
	}
	public void SetRadius(Vec3d r) {
	    SprExport.Spr_CDEllipsoidIf_SetRadius((IntPtr) _thisArray[0], (IntPtr) r);
	}
    }
    public partial class CDCapsuleIf : CDConvexIf {
	public CDCapsuleIf() { _thisArray[0] = SprExport.Spr_new_CDCapsuleIf(); _flag = true; }
	public CDCapsuleIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDCapsuleIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDCapsuleIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDCapsuleIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDCapsuleIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDCapsuleIf(CsCastObject target) {
		return (target._info.Inherit(CDCapsuleIf.GetIfInfoStatic()) ? new CDCapsuleIf(target._thisArray[0], target._flag) : null);
	}
	~CDCapsuleIf() { if (_flag) { SprExport.Spr_delete_CDCapsuleIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDCapsuleIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDCapsuleIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDCapsuleIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDCapsuleIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public float GetRadius() {
	    float result = (float) SprExport.Spr_CDCapsuleIf_GetRadius((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetRadius(float r) {
	    SprExport.Spr_CDCapsuleIf_SetRadius((IntPtr) _thisArray[0], (float) r);
	}
	public float GetLength() {
	    float result = (float) SprExport.Spr_CDCapsuleIf_GetLength((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetLength(float l) {
	    SprExport.Spr_CDCapsuleIf_SetLength((IntPtr) _thisArray[0], (float) l);
	}
    }
    public partial class CDRoundConeIf : CDConvexIf {
	public CDRoundConeIf() { _thisArray[0] = SprExport.Spr_new_CDRoundConeIf(); _flag = true; }
	public CDRoundConeIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDRoundConeIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDRoundConeIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDRoundConeIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDRoundConeIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDRoundConeIf(CsCastObject target) {
		return (target._info.Inherit(CDRoundConeIf.GetIfInfoStatic()) ? new CDRoundConeIf(target._thisArray[0], target._flag) : null);
	}
	~CDRoundConeIf() { if (_flag) { SprExport.Spr_delete_CDRoundConeIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDRoundConeIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDRoundConeIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public Vec2f GetRadius() {
	    IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetRadius((IntPtr) _thisArray[0]);
            return new Vec2f(ptr, true);
	}
	public float GetLength() {
	    float result = (float) SprExport.Spr_CDRoundConeIf_GetLength((IntPtr) _thisArray[0]);
	    return result;
	}
	public void SetRadius(Vec2f r) {
	    SprExport.Spr_CDRoundConeIf_SetRadius((IntPtr) _thisArray[0], (IntPtr) r);
	}
	public void SetLength(float l) {
	    SprExport.Spr_CDRoundConeIf_SetLength((IntPtr) _thisArray[0], (float) l);
	}
	public void SetWidth(Vec2f r) {
	    SprExport.Spr_CDRoundConeIf_SetWidth((IntPtr) _thisArray[0], (IntPtr) r);
	}
    }
    public partial class CDBoxIf : CDConvexIf {
	public CDBoxIf() { _thisArray[0] = SprExport.Spr_new_CDBoxIf(); _flag = true; }
	public CDBoxIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDBoxIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDBoxIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDBoxIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDBoxIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDBoxIf(CsCastObject target) {
		return (target._info.Inherit(CDBoxIf.GetIfInfoStatic()) ? new CDBoxIf(target._thisArray[0], target._flag) : null);
	}
	~CDBoxIf() { if (_flag) { SprExport.Spr_delete_CDBoxIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDBoxIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public Vec3f GetBoxSize() {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_GetBoxSize((IntPtr) _thisArray[0]);
            return new Vec3f(ptr, true);
	}
	public arraywrapper_Vec3f GetVertices() {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_GetVertices((IntPtr) _thisArray[0]);
            return new arraywrapper_Vec3f(ptr);
	}
	public CDFaceIf GetFace(int i) {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_GetFace((IntPtr) _thisArray[0], (int) i);
            if (ptr == IntPtr.Zero) { return null; } 
            CDFaceIf obj = new CDFaceIf(ptr, 0);
            return obj;
	}
	public Vec3f SetBoxSize(Vec3f boxsize) {
	    IntPtr ptr = SprExport.Spr_CDBoxIf_SetBoxSize((IntPtr) _thisArray[0], (IntPtr) boxsize);
            return new Vec3f(ptr, true);
	}
    }
    public partial class CDShapePairIf : ObjectIf {
	public CDShapePairIf() { _thisArray[0] = SprExport.Spr_new_CDShapePairIf(); _flag = true; }
	public CDShapePairIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDShapePairIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDShapePairIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDShapePairIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDShapePairIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDShapePairIf(CsCastObject target) {
		return (target._info.Inherit(CDShapePairIf.GetIfInfoStatic()) ? new CDShapePairIf(target._thisArray[0], target._flag) : null);
	}
	~CDShapePairIf() { if (_flag) { SprExport.Spr_delete_CDShapePairIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDShapePairIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public CDShapeIf GetShape(int i) {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetShape((IntPtr) _thisArray[0], (int) i);
            if (ptr == IntPtr.Zero) { return null; } 
            CDShapeIf obj = new CDShapeIf(ptr, 0);
            if (obj.GetIfInfo() == CDConvexIf.GetIfInfoStatic()) {
				CDConvexIf appropriate_type = new CDConvexIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDConvexMeshIf.GetIfInfoStatic()) {
				CDConvexMeshIf appropriate_type = new CDConvexMeshIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDSphereIf.GetIfInfoStatic()) {
				CDSphereIf appropriate_type = new CDSphereIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDEllipsoidIf.GetIfInfoStatic()) {
				CDEllipsoidIf appropriate_type = new CDEllipsoidIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDCapsuleIf.GetIfInfoStatic()) {
				CDCapsuleIf appropriate_type = new CDCapsuleIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDRoundConeIf.GetIfInfoStatic()) {
				CDRoundConeIf appropriate_type = new CDRoundConeIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDBoxIf.GetIfInfoStatic()) {
				CDBoxIf appropriate_type = new CDBoxIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }else if (obj.GetIfInfo() == CDConvexMeshInterpolateIf.GetIfInfoStatic()) {
				CDConvexMeshInterpolateIf appropriate_type = new CDConvexMeshInterpolateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
				return appropriate_type;
            }
            return obj;
	}
	public Posed GetShapePose(int i) {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetShapePose((IntPtr) _thisArray[0], (int) i);
            return new Posed(ptr, true);
	}
	public Vec3d GetClosestPointOnWorld(int i) {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetClosestPointOnWorld((IntPtr) _thisArray[0], (int) i);
            return new Vec3d(ptr, true);
	}
	public Vec3d GetNormal() {
	    IntPtr ptr = SprExport.Spr_CDShapePairIf_GetNormal((IntPtr) _thisArray[0]);
            return new Vec3d(ptr, true);
	}
    }
    public partial class CDSpatialHashTableIf : ObjectIf {
	public CDSpatialHashTableIf() { _thisArray[0] = SprExport.Spr_new_CDSpatialHashTableIf(); _flag = true; }
	public CDSpatialHashTableIf(IntPtr ptr) : base(ptr, 0, false) {}
	public CDSpatialHashTableIf(IntPtr ptr, bool flag) : base(ptr, 0, flag) {}
	public CDSpatialHashTableIf(IntPtr ptr, int sceneIndex) : base(ptr, sceneIndex, false) {}
	public CDSpatialHashTableIf(IntPtr ptr, int sceneIndex, bool flag) : base(ptr, sceneIndex, flag) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2) : base(ptr0, ptr1, ptr2, false) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag) : base(ptr0, ptr1, ptr2, flag) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index) : base(ptr0, ptr1, ptr0Index, ptr1Index, false) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, int ptr0Index, int ptr1Index, bool flag) : base(ptr0, ptr1, ptr0Index, ptr1Index, flag) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, false) {}
	public CDSpatialHashTableIf(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag) : base(ptr0, ptr1, ptr2, ptr0Index, ptr1Index, ptr2Index, flag) {}
	public static implicit operator CDSpatialHashTableIf(CsCastObject target) {
		return (target._info.Inherit(CDSpatialHashTableIf.GetIfInfoStatic()) ? new CDSpatialHashTableIf(target._thisArray[0], target._flag) : null);
	}
	~CDSpatialHashTableIf() { if (_flag) { SprExport.Spr_delete_CDSpatialHashTableIf(_thisArray[0]); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_ToString((IntPtr) _thisArray[0]);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDSpatialHashTableIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_GetIfInfo((IntPtr) _thisArray[0]);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public void Init(float cellSize, CDBounds bounds) {
	    SprExport.Spr_CDSpatialHashTableIf_Init((IntPtr) _thisArray[0], (float) cellSize, (IntPtr) bounds);
	}
	public bool ReducedSelectPointsToAdd(Vec3f pCtr, float radius, int objInd, int pInd, int timestamp) {
	    char ret = SprExport.Spr_CDSpatialHashTableIf_ReducedSelectPointsToAdd((IntPtr) _thisArray[0], (IntPtr) pCtr, (float) radius, (int) objInd, (int) pInd, (int) timestamp);
	    return (ret == 0) ? false : true;
	}
    }
	public partial class IfInfoToCsType {
		public static Dictionary<IntPtr, Type> mapCollision = new Dictionary<IntPtr, Type>() {
			{CDShapeIf.GetIfInfoStatic(), typeof(CDShapeIf)},
			{CDConvexIf.GetIfInfoStatic(), typeof(CDConvexIf)},
			{CDFaceIf.GetIfInfoStatic(), typeof(CDFaceIf)},
			{CDQuadFaceIf.GetIfInfoStatic(), typeof(CDQuadFaceIf)},
			{CDConvexMeshIf.GetIfInfoStatic(), typeof(CDConvexMeshIf)},
			{CDConvexMeshInterpolateIf.GetIfInfoStatic(), typeof(CDConvexMeshInterpolateIf)},
			{CDSphereIf.GetIfInfoStatic(), typeof(CDSphereIf)},
			{CDEllipsoidIf.GetIfInfoStatic(), typeof(CDEllipsoidIf)},
			{CDCapsuleIf.GetIfInfoStatic(), typeof(CDCapsuleIf)},
			{CDRoundConeIf.GetIfInfoStatic(), typeof(CDRoundConeIf)},
			{CDBoxIf.GetIfInfoStatic(), typeof(CDBoxIf)},
			{CDShapePairIf.GetIfInfoStatic(), typeof(CDShapePairIf)},
			{CDSpatialHashTableIf.GetIfInfoStatic(), typeof(CDSpatialHashTableIf)},
		};
	}
}
