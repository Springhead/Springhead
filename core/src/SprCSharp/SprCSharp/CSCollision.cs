using UnityEngine;
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
#pragma warning disable 0108
namespace SprCs {
    public partial class PHMaterial : CsObject {
        public PHMaterial() { _this = SprExport.Spr_new_PHMaterial(); _flag = true; }
        public PHMaterial(IntPtr ptr) : base(ptr) { }
        public PHMaterial(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHMaterial() { if (_flag) { SprExport.Spr_delete_PHMaterial(_this); _flag = false; } }
        public float density {
            get { return SprExport.Spr_PHMaterial_get_density(_this); }
            set { SprExport.Spr_PHMaterial_set_density(_this, value); }
        }
        public float mu {
            get { return SprExport.Spr_PHMaterial_get_mu(_this); }
            set { SprExport.Spr_PHMaterial_set_mu(_this, value); }
        }
        public float mu0 {
            get { return SprExport.Spr_PHMaterial_get_mu0(_this); }
            set { SprExport.Spr_PHMaterial_set_mu0(_this, value); }
        }
        public float e {
            get { return SprExport.Spr_PHMaterial_get_e(_this); }
            set { SprExport.Spr_PHMaterial_set_e(_this, value); }
        }
        public float spring {
            get { return SprExport.Spr_PHMaterial_get_spring(_this); }
            set { SprExport.Spr_PHMaterial_set_spring(_this, value); }
        }
        public float damper {
            get { return SprExport.Spr_PHMaterial_get_damper(_this); }
            set { SprExport.Spr_PHMaterial_set_damper(_this, value); }
        }
        public float reflexSpring {
            get { return SprExport.Spr_PHMaterial_get_reflexSpring(_this); }
            set { SprExport.Spr_PHMaterial_set_reflexSpring(_this, value); }
        }
        public float reflexDamper {
            get { return SprExport.Spr_PHMaterial_get_reflexDamper(_this); }
            set { SprExport.Spr_PHMaterial_set_reflexDamper(_this, value); }
        }
        public float frictionSpring {
            get { return SprExport.Spr_PHMaterial_get_frictionSpring(_this); }
            set { SprExport.Spr_PHMaterial_set_frictionSpring(_this, value); }
        }
        public float frictionDamper {
            get { return SprExport.Spr_PHMaterial_get_frictionDamper(_this); }
            set { SprExport.Spr_PHMaterial_set_frictionDamper(_this, value); }
        }
        public float vibA {
            get { return SprExport.Spr_PHMaterial_get_vibA(_this); }
            set { SprExport.Spr_PHMaterial_set_vibA(_this, value); }
        }
        public float vibB {
            get { return SprExport.Spr_PHMaterial_get_vibB(_this); }
            set { SprExport.Spr_PHMaterial_set_vibB(_this, value); }
        }
        public float vibW {
            get { return SprExport.Spr_PHMaterial_get_vibW(_this); }
            set { SprExport.Spr_PHMaterial_set_vibW(_this, value); }
        }
        public float vibT {
            get { return SprExport.Spr_PHMaterial_get_vibT(_this); }
            set { SprExport.Spr_PHMaterial_set_vibT(_this, value); }
        }
        public bool vibContact {
            get {
                byte ret = (byte)SprExport.Spr_PHMaterial_get_vibContact(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHMaterial_set_vibContact(_this, (char)val);
            }
        }
        public bool vibFric {
            get {
                byte ret = (byte)SprExport.Spr_PHMaterial_get_vibFric(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHMaterial_set_vibFric(_this, (char)val);
            }
        }
        public float timeVaryFrictionA {
            get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionA(_this); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionA(_this, value); }
        }
        public float timeVaryFrictionB {
            get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionB(_this); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionB(_this, value); }
        }
        public float timeVaryFrictionC {
            get { return SprExport.Spr_PHMaterial_get_timeVaryFrictionC(_this); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionC(_this, value); }
        }
        public float frictionViscosity {
            get { return SprExport.Spr_PHMaterial_get_frictionViscosity(_this); }
            set { SprExport.Spr_PHMaterial_set_frictionViscosity(_this, value); }
        }
        public float stribeckVelocity {
            get { return SprExport.Spr_PHMaterial_get_stribeckVelocity(_this); }
            set { SprExport.Spr_PHMaterial_set_stribeckVelocity(_this, value); }
        }
        public float stribeckmu {
            get { return SprExport.Spr_PHMaterial_get_stribeckmu(_this); }
            set { SprExport.Spr_PHMaterial_set_stribeckmu(_this, value); }
        }
        public vectorwrapper_float mus {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_mus(_this)); }
            set { SprExport.Spr_PHMaterial_set_mus(_this, value); }
        }
        public vectorwrapper_float mu0s {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_mu0s(_this)); }
            set { SprExport.Spr_PHMaterial_set_mu0s(_this, value); }
        }
        public vectorwrapper_float timeVaryFrictionAs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionAs(_this)); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionAs(_this, value); }
        }
        public vectorwrapper_float timeVaryFrictionBs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionBs(_this)); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionBs(_this, value); }
        }
        public vectorwrapper_float timeVaryFrictionDs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionDs(_this)); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionDs(_this, value); }
        }
        public vectorwrapper_float timeVaryFrictionCs {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_timeVaryFrictionCs(_this)); }
            set { SprExport.Spr_PHMaterial_set_timeVaryFrictionCs(_this, value); }
        }
        public vectorwrapper_float stribeckVelocitys {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_stribeckVelocitys(_this)); }
            set { SprExport.Spr_PHMaterial_set_stribeckVelocitys(_this, value); }
        }
        public vectorwrapper_float stribeckmus {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_stribeckmus(_this)); }
            set { SprExport.Spr_PHMaterial_set_stribeckmus(_this, value); }
        }
        public vectorwrapper_double c {
            get { return new vectorwrapper_double(SprExport.Spr_PHMaterial_addr_c(_this)); }
            set { SprExport.Spr_PHMaterial_set_c(_this, value); }
        }
        public vectorwrapper_float bristleK {
            get { return new vectorwrapper_float(SprExport.Spr_PHMaterial_addr_bristleK(_this)); }
            set { SprExport.Spr_PHMaterial_set_bristleK(_this, value); }
        }
        public int velocityFieldMode {
            get { return SprExport.Spr_PHMaterial_get_velocityFieldMode(_this); }
            set { SprExport.Spr_PHMaterial_set_velocityFieldMode(_this, value); }
        }
        public Vec3d velocityFieldAxis {
            get { return new Vec3d(SprExport.Spr_PHMaterial_addr_velocityFieldAxis(_this)); }
            set { SprExport.Spr_PHMaterial_set_velocityFieldAxis(_this, value); }
        }
        public double velocityFieldMagnitude {
            get { return SprExport.Spr_PHMaterial_get_velocityFieldMagnitude(_this); }
            set { SprExport.Spr_PHMaterial_set_velocityFieldMagnitude(_this, value); }
        }
        public Vec3d CalcVelocity(Vec3d pos, Vec3d normal) {
            IntPtr ptr = SprExport.Spr_PHMaterial_CalcVelocity((IntPtr)_this, (IntPtr)pos, (IntPtr)normal);
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
            //r.mus = mus;
            //r.mu0s = mu0s;
            //r.timeVaryFrictionAs = timeVaryFrictionAs;
            //r.timeVaryFrictionBs = timeVaryFrictionBs;
            //r.timeVaryFrictionDs = timeVaryFrictionDs;
            //r.timeVaryFrictionCs = timeVaryFrictionCs;
            //r.stribeckVelocitys = stribeckVelocitys;
            //r.stribeckmus = stribeckmus;
            //r.c = c;
            //r.bristleK = bristleK;
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
            //mus = r.mus;
            //mu0s = r.mu0s;
            //timeVaryFrictionAs = r.timeVaryFrictionAs;
            //timeVaryFrictionBs = r.timeVaryFrictionBs;
            //timeVaryFrictionDs = r.timeVaryFrictionDs;
            //timeVaryFrictionCs = r.timeVaryFrictionCs;
            //stribeckVelocitys = r.stribeckVelocitys;
            //stribeckmus = r.stribeckmus;
            //c = r.c;
            //bristleK = r.bristleK;
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
        private IntPtr _this_CDShapeDesc;
        private bool _flag_CDShapeDesc;

        public CDShapeDesc() {
            Console.WriteLine("CDShapeDesc Constructor");
            Debug.Log("CDShapeDesc Constructor");
            _this_CDShapeDesc = SprExport.Spr_new_CDShapeDesc();
            _flag_CDShapeDesc = true;

            _this = _this_CDShapeDesc;
            _flag = true;
        }
        public CDShapeDesc(IntPtr ptr) : base(ptr) { }
        public CDShapeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDShapeDesc() {
            Console.WriteLine("CDShapeDesc Destructor");
            if (_flag_CDShapeDesc) {
                Console.WriteLine("CDShapeDesc Destructor flag true");
                SprExport.Spr_delete_CDShapeDesc(_this_CDShapeDesc);
            }
        }
        public PHMaterial material {
            get { return new PHMaterial(SprExport.Spr_CDShapeDesc_addr_material(_this)); }
            set { SprExport.Spr_CDShapeDesc_set_material(_this, value); }
        }
    }
    [System.Serializable]
    public class CDShapeDescStruct : CsObject {
        public CDShapeDescStruct() {
            Console.WriteLine("CDShapeDescStruct Constructor");
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
            Console.WriteLine("CDShapeDesc(CDShapeDescStruct m)");
            CDShapeDesc r = new CDShapeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator CDShapeDescStruct(CDShapeDesc r) {
            Console.WriteLine("CDShapeDescStruct(CDShapeDesc r)");
            CDShapeDescStruct m = new CDShapeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class CDConvexMeshDesc : CDShapeDesc {
        public CDConvexMeshDesc() { _this = SprExport.Spr_new_CDConvexMeshDesc(); _flag = true; }
        public CDConvexMeshDesc(IntPtr ptr) : base(ptr) { }
        public CDConvexMeshDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDConvexMeshDesc() { if (_flag) { SprExport.Spr_delete_CDConvexMeshDesc(_this); _flag = false; } }
        public vectorwrapper_Vec3f vertices {
            get { return new vectorwrapper_Vec3f(SprExport.Spr_CDConvexMeshDesc_addr_vertices(_this)); }
            set { SprExport.Spr_CDConvexMeshDesc_set_vertices(_this, value); }
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
        public CDConvexMeshInterpolateDesc() { _this = SprExport.Spr_new_CDConvexMeshInterpolateDesc(); _flag = true; }
        public CDConvexMeshInterpolateDesc(IntPtr ptr) : base(ptr) { }
        public CDConvexMeshInterpolateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDConvexMeshInterpolateDesc() { if (_flag) { SprExport.Spr_delete_CDConvexMeshInterpolateDesc(_this); _flag = false; } }
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
        public CDSphereDesc() { _this = SprExport.Spr_new_CDSphereDesc(); _flag = true; }
        public CDSphereDesc(IntPtr ptr) : base(ptr) { }
        public CDSphereDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDSphereDesc() { if (_flag) { SprExport.Spr_delete_CDSphereDesc(_this); _flag = false; } }
        public float radius {
            get { return SprExport.Spr_CDSphereDesc_get_radius(_this); }
            set { SprExport.Spr_CDSphereDesc_set_radius(_this, value); }
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
        public CDEllipsoidDesc() { _this = SprExport.Spr_new_CDEllipsoidDesc(); _flag = true; }
        public CDEllipsoidDesc(IntPtr ptr) : base(ptr) { }
        public CDEllipsoidDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDEllipsoidDesc() { if (_flag) { SprExport.Spr_delete_CDEllipsoidDesc(_this); _flag = false; } }
        public Vec3d radius {
            get { return new Vec3d(SprExport.Spr_CDEllipsoidDesc_addr_radius(_this)); }
            set { SprExport.Spr_CDEllipsoidDesc_set_radius(_this, value); }
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
        public CDCapsuleDesc() { _this = SprExport.Spr_new_CDCapsuleDesc(); _flag = true; }
        public CDCapsuleDesc(IntPtr ptr) : base(ptr) { }
        public CDCapsuleDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDCapsuleDesc() { if (_flag) { SprExport.Spr_delete_CDCapsuleDesc(_this); _flag = false; } }
        public float radius {
            get { return SprExport.Spr_CDCapsuleDesc_get_radius(_this); }
            set { SprExport.Spr_CDCapsuleDesc_set_radius(_this, value); }
        }
        public float length {
            get { return SprExport.Spr_CDCapsuleDesc_get_length(_this); }
            set { SprExport.Spr_CDCapsuleDesc_set_length(_this, value); }
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
        public CDRoundConeDesc() { _this = SprExport.Spr_new_CDRoundConeDesc(); _flag = true; }
        public CDRoundConeDesc(IntPtr ptr) : base(ptr) { }
        public CDRoundConeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDRoundConeDesc() { if (_flag) { SprExport.Spr_delete_CDRoundConeDesc(_this); _flag = false; } }
        public Vec2f radius {
            get { return new Vec2f(SprExport.Spr_CDRoundConeDesc_addr_radius(_this)); }
            set { SprExport.Spr_CDRoundConeDesc_set_radius(_this, value); }
        }
        public float length {
            get { return SprExport.Spr_CDRoundConeDesc_get_length(_this); }
            set { SprExport.Spr_CDRoundConeDesc_set_length(_this, value); }
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
        public CDBoxDesc() {
            Console.WriteLine("CDBoxDesc Constructor");
            Debug.Log("CDBoxDesc Constructor");
            _this = SprExport.Spr_new_CDBoxDesc(); _flag = true;
        }
        public CDBoxDesc(IntPtr ptr) : base(ptr) { }
        public CDBoxDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public CDBoxDesc(Vec3d bs) { _this = SprExport.Spr_new_CDBoxDesc_2(bs); _flag = true; }
        ~CDBoxDesc() { if (_flag) { SprExport.Spr_delete_CDBoxDesc(_this); _flag = false; } }
        public Vec3f boxsize {
            get { return new Vec3f(SprExport.Spr_CDBoxDesc_addr_boxsize(_this)); }
            set { SprExport.Spr_CDBoxDesc_set_boxsize(_this, value); }
        }
    }
    [System.Serializable]
    public class CDBoxDescStruct : CDShapeDescStruct {
        public CDBoxDescStruct() {
            Console.WriteLine("CDBoxDescStruct Constructor");
            CDBoxDesc desc = new CDBoxDesc();
            ApplyFrom(desc, true);
        }
        public Vec3fStruct boxsize;
        public void ApplyTo(CDBoxDesc r, bool apply_to_base_class = false) {
            Console.WriteLine("CDBoxDescStruct.ApplyTo");
            r.boxsize = boxsize;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(CDBoxDesc r, bool apply_to_base_class = false) {
            boxsize = r.boxsize;
            if (apply_to_base_class) {
                Console.WriteLine("CDBoxDescStruct.ApplyFrom");
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator CDBoxDesc(CDBoxDescStruct m) {
            Console.WriteLine("CDBoxDesc(CDBoxDescStruct m)");
            CDBoxDesc r = new CDBoxDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator CDBoxDescStruct(CDBoxDesc r) {
            Console.WriteLine("CDBoxDescStruct(CDBoxDesc r)");
            CDBoxDescStruct m = new CDBoxDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class CDShapePairState : CsObject {
        public CDShapePairState() { _this = SprExport.Spr_new_CDShapePairState(); _flag = true; }
        public CDShapePairState(IntPtr ptr) : base(ptr) { }
        public CDShapePairState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDShapePairState() { if (_flag) { SprExport.Spr_delete_CDShapePairState(_this); _flag = false; } }
        public Vec3d normal {
            get { return new Vec3d(SprExport.Spr_CDShapePairState_addr_normal(_this)); }
            set { SprExport.Spr_CDShapePairState_set_normal(_this, value); }
        }
        public double depth {
            get { return SprExport.Spr_CDShapePairState_get_depth(_this); }
            set { SprExport.Spr_CDShapePairState_set_depth(_this, value); }
        }
        public uint lastContactCount {
            get { return SprExport.Spr_CDShapePairState_get_lastContactCount(_this); }
            set { SprExport.Spr_CDShapePairState_set_lastContactCount(_this, value); }
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
        public CDBounds() { _this = SprExport.Spr_new_CDBounds(); _flag = true; }
        public CDBounds(IntPtr ptr) : base(ptr) { }
        public CDBounds(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public CDBounds(Vec3d min0, Vec3d max0) { _this = SprExport.Spr_new_CDBounds_3(min0, max0); _flag = true; }
        ~CDBounds() { if (_flag) { SprExport.Spr_delete_CDBounds(_this); _flag = false; } }
        public Vec3f min {
            get { return new Vec3f(SprExport.Spr_CDBounds_addr_min(_this)); }
            set { SprExport.Spr_CDBounds_set_min(_this, value); }
        }
        public Vec3f max {
            get { return new Vec3f(SprExport.Spr_CDBounds_addr_max(_this)); }
            set { SprExport.Spr_CDBounds_set_max(_this, value); }
        }
        public void set(Vec3d min0, Vec3d max0) {
            SprExport.Spr_CDBounds_set((IntPtr)_this, (IntPtr)min0, (IntPtr)max0);
        }
        public void clamp(Vec3f pos) {
            SprExport.Spr_CDBounds_clamp((IntPtr)_this, (IntPtr)pos);
        }
        public bool isEmpty() {
            char ret = SprExport.Spr_CDBounds_isEmpty((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void minimum(Vec3f self, Vec3f other) {
            SprExport.Spr_CDBounds_minimum((IntPtr)_this, (IntPtr)self, (IntPtr)other);
        }
        public void maximum(Vec3f self, Vec3f other) {
            SprExport.Spr_CDBounds_maximum((IntPtr)_this, (IntPtr)self, (IntPtr)other);
        }
    }
    public partial class CDSpatialHashTableDesc : CsObject {
        public CDSpatialHashTableDesc() { _this = SprExport.Spr_new_CDSpatialHashTableDesc(); _flag = true; }
        public CDSpatialHashTableDesc(IntPtr ptr) : base(ptr) { }
        public CDSpatialHashTableDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~CDSpatialHashTableDesc() { if (_flag) { SprExport.Spr_delete_CDSpatialHashTableDesc(_this); _flag = false; } }
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
        public CDShapeIf() { _this = SprExport.Spr_new_CDShapeIf(); _flag = true; }
        public CDShapeIf(IntPtr ptr) : base(ptr) { }
        public CDShapeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDShapeIf(CsCastObject target) {
            return (target._info.Inherit(CDShapeIf.GetIfInfoStatic()) ? new CDShapeIf(target._this, target._flag) : null);
        }
        ~CDShapeIf() { if (_flag) { SprExport.Spr_delete_CDShapeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDShapeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetStaticFriction(float mu0) {
            SprExport.Spr_CDShapeIf_SetStaticFriction((IntPtr)_this, (float)mu0);
        }
        public float GetStaticFriction() {
            float result = (float)SprExport.Spr_CDShapeIf_GetStaticFriction((IntPtr)_this);
            return result;
        }
        public void SetDynamicFriction(float mu) {
            SprExport.Spr_CDShapeIf_SetDynamicFriction((IntPtr)_this, (float)mu);
        }
        public float GetDynamicFriction() {
            float result = (float)SprExport.Spr_CDShapeIf_GetDynamicFriction((IntPtr)_this);
            return result;
        }
        public void SetElasticity(float e) {
            SprExport.Spr_CDShapeIf_SetElasticity((IntPtr)_this, (float)e);
        }
        public float GetElasticity() {
            float result = (float)SprExport.Spr_CDShapeIf_GetElasticity((IntPtr)_this);
            return result;
        }
        public void SetDensity(float d) {
            SprExport.Spr_CDShapeIf_SetDensity((IntPtr)_this, (float)d);
        }
        public float GetDensity() {
            float result = (float)SprExport.Spr_CDShapeIf_GetDensity((IntPtr)_this);
            return result;
        }
        public void SetContactSpring(float K) {
            SprExport.Spr_CDShapeIf_SetContactSpring((IntPtr)_this, (float)K);
        }
        public float GetContactSpring() {
            float result = (float)SprExport.Spr_CDShapeIf_GetContactSpring((IntPtr)_this);
            return result;
        }
        public void SetContactDamper(float D) {
            SprExport.Spr_CDShapeIf_SetContactDamper((IntPtr)_this, (float)D);
        }
        public float GetContactDamper() {
            float result = (float)SprExport.Spr_CDShapeIf_GetContactDamper((IntPtr)_this);
            return result;
        }
        public void SetReflexSpring(float K) {
            SprExport.Spr_CDShapeIf_SetReflexSpring((IntPtr)_this, (float)K);
        }
        public float GetReflexSpring() {
            float result = (float)SprExport.Spr_CDShapeIf_GetReflexSpring((IntPtr)_this);
            return result;
        }
        public void SetReflexDamper(float D) {
            SprExport.Spr_CDShapeIf_SetReflexDamper((IntPtr)_this, (float)D);
        }
        public float GetReflexDamper() {
            float result = (float)SprExport.Spr_CDShapeIf_GetReflexDamper((IntPtr)_this);
            return result;
        }
        public void SetFrictionSpring(float K) {
            SprExport.Spr_CDShapeIf_SetFrictionSpring((IntPtr)_this, (float)K);
        }
        public float GetFrictionSpring() {
            float result = (float)SprExport.Spr_CDShapeIf_GetFrictionSpring((IntPtr)_this);
            return result;
        }
        public void SetFrictionDamper(float D) {
            SprExport.Spr_CDShapeIf_SetFrictionDamper((IntPtr)_this, (float)D);
        }
        public float GetFrictionDamper() {
            float result = (float)SprExport.Spr_CDShapeIf_GetFrictionDamper((IntPtr)_this);
            return result;
        }
        public void SetVibration(float vibA, float vibB, float vibW) {
            SprExport.Spr_CDShapeIf_SetVibration((IntPtr)_this, (float)vibA, (float)vibB, (float)vibW);
        }
        public void SetVibA(float vibA) {
            SprExport.Spr_CDShapeIf_SetVibA((IntPtr)_this, (float)vibA);
        }
        public float GetVibA() {
            float result = (float)SprExport.Spr_CDShapeIf_GetVibA((IntPtr)_this);
            return result;
        }
        public void SetVibB(float vibB) {
            SprExport.Spr_CDShapeIf_SetVibB((IntPtr)_this, (float)vibB);
        }
        public float GetVibB() {
            float result = (float)SprExport.Spr_CDShapeIf_GetVibB((IntPtr)_this);
            return result;
        }
        public void SetVibW(float vibW) {
            SprExport.Spr_CDShapeIf_SetVibW((IntPtr)_this, (float)vibW);
        }
        public float GetVibW() {
            float result = (float)SprExport.Spr_CDShapeIf_GetVibW((IntPtr)_this);
            return result;
        }
        public void SetVibT(float vibT) {
            SprExport.Spr_CDShapeIf_SetVibT((IntPtr)_this, (float)vibT);
        }
        public float GetVibT() {
            float result = (float)SprExport.Spr_CDShapeIf_GetVibT((IntPtr)_this);
            return result;
        }
        public void SetVibContact(bool vibContact) {
            SprExport.Spr_CDShapeIf_SetVibContact((IntPtr)_this, (bool)vibContact);
        }
        public bool GetVibContact() {
            char ret = SprExport.Spr_CDShapeIf_GetVibContact((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PHMaterial GetMaterial() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_GetMaterial((IntPtr)_this);
            return new PHMaterial(ptr, true);
        }
        public void SetMaterial(PHMaterial mat) {
            SprExport.Spr_CDShapeIf_SetMaterial((IntPtr)_this, (IntPtr)mat);
        }
        public float CalcVolume() {
            float result = (float)SprExport.Spr_CDShapeIf_CalcVolume((IntPtr)_this);
            return result;
        }
        public Vec3f CalcCenterOfMass() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_CalcCenterOfMass((IntPtr)_this);
            return new Vec3f(ptr, true);
        }
        public Matrix3f CalcMomentOfInertia() {
            IntPtr ptr = SprExport.Spr_CDShapeIf_CalcMomentOfInertia((IntPtr)_this);
            return new Matrix3f(ptr, true);
        }
        public bool IsInside(Vec3f p) {
            char ret = SprExport.Spr_CDShapeIf_IsInside((IntPtr)_this, (IntPtr)p);
            return (ret == 0) ? false : true;
        }
        public void CalcBBox(Vec3f bbmin, Vec3f bbmax, Posed pose) {
            SprExport.Spr_CDShapeIf_CalcBBox((IntPtr)_this, (IntPtr)bbmin, (IntPtr)bbmax, (IntPtr)pose);
        }
        public void CalcBBox(Vec3f bbmin, Vec3f bbmax) {
            SprExport.Spr_CDShapeIf_CalcBBox_1((IntPtr)_this, (IntPtr)bbmin, (IntPtr)bbmax);
        }
    }
    public partial class CDConvexIf : CDShapeIf {
        public CDConvexIf() { _this = SprExport.Spr_new_CDConvexIf(); _flag = true; }
        public CDConvexIf(IntPtr ptr) : base(ptr) { }
        public CDConvexIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDConvexIf(CsCastObject target) {
            return (target._info.Inherit(CDConvexIf.GetIfInfoStatic()) ? new CDConvexIf(target._this, target._flag) : null);
        }
        ~CDConvexIf() { if (_flag) { SprExport.Spr_delete_CDConvexIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDConvexIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDConvexIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public double CurvatureRadius(Vec3d p) {
            double result = (double)SprExport.Spr_CDConvexIf_CurvatureRadius((IntPtr)_this, (IntPtr)p);
            return result;
        }
        public Vec3d Normal(Vec3d p) {
            IntPtr ptr = SprExport.Spr_CDConvexIf_Normal((IntPtr)_this, (IntPtr)p);
            return new Vec3d(ptr, true);
        }
    }
    public partial class CDFaceIf : ObjectIf {
        public CDFaceIf() { _this = SprExport.Spr_new_CDFaceIf(); _flag = true; }
        public CDFaceIf(IntPtr ptr) : base(ptr) { }
        public CDFaceIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDFaceIf(CsCastObject target) {
            return (target._info.Inherit(CDFaceIf.GetIfInfoStatic()) ? new CDFaceIf(target._this, target._flag) : null);
        }
        ~CDFaceIf() { if (_flag) { SprExport.Spr_delete_CDFaceIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDFaceIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDFaceIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDFaceIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDFaceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int NIndex() {
            int result = (int)SprExport.Spr_CDFaceIf_NIndex((IntPtr)_this);
            return result;
        }
        public int GetIndices() {
            int result = (int)SprExport.Spr_CDFaceIf_GetIndices((IntPtr)_this);
            return result;
        }
    }
    public partial class CDQuadFaceIf : ObjectIf {
        public CDQuadFaceIf() { _this = SprExport.Spr_new_CDQuadFaceIf(); _flag = true; }
        public CDQuadFaceIf(IntPtr ptr) : base(ptr) { }
        public CDQuadFaceIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDQuadFaceIf(CsCastObject target) {
            return (target._info.Inherit(CDQuadFaceIf.GetIfInfoStatic()) ? new CDQuadFaceIf(target._this, target._flag) : null);
        }
        ~CDQuadFaceIf() { if (_flag) { SprExport.Spr_delete_CDQuadFaceIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDQuadFaceIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDQuadFaceIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDQuadFaceIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDQuadFaceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int NIndex() {
            int result = (int)SprExport.Spr_CDQuadFaceIf_NIndex((IntPtr)_this);
            return result;
        }
        public int GetIndices() {
            int result = (int)SprExport.Spr_CDQuadFaceIf_GetIndices((IntPtr)_this);
            return result;
        }
    }
    public partial class CDConvexMeshIf : CDConvexIf {
        public CDConvexMeshIf() { _this = SprExport.Spr_new_CDConvexMeshIf(); _flag = true; }
        public CDConvexMeshIf(IntPtr ptr) : base(ptr) { }
        public CDConvexMeshIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDConvexMeshIf(CsCastObject target) {
            return (target._info.Inherit(CDConvexMeshIf.GetIfInfoStatic()) ? new CDConvexMeshIf(target._this, target._flag) : null);
        }
        ~CDConvexMeshIf() { if (_flag) { SprExport.Spr_delete_CDConvexMeshIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexMeshIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public CDFaceIf GetFace(int i) {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetFace((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            CDFaceIf obj = new CDFaceIf(ptr);
            return obj;
        }
        public int NFace() {
            int result = (int)SprExport.Spr_CDConvexMeshIf_NFace((IntPtr)_this);
            return result;
        }
        public arraywrapper_Vec3f GetVertices() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshIf_GetVertices((IntPtr)_this);
            return new arraywrapper_Vec3f(ptr);
        }
        public int NVertex() {
            int result = (int)SprExport.Spr_CDConvexMeshIf_NVertex((IntPtr)_this);
            return result;
        }
    }
    public partial class CDConvexMeshInterpolateIf : CDConvexMeshIf {
        public CDConvexMeshInterpolateIf() { _this = SprExport.Spr_new_CDConvexMeshInterpolateIf(); _flag = true; }
        public CDConvexMeshInterpolateIf(IntPtr ptr) : base(ptr) { }
        public CDConvexMeshInterpolateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDConvexMeshInterpolateIf(CsCastObject target) {
            return (target._info.Inherit(CDConvexMeshInterpolateIf.GetIfInfoStatic()) ? new CDConvexMeshInterpolateIf(target._this, target._flag) : null);
        }
        ~CDConvexMeshInterpolateIf() { if (_flag) { SprExport.Spr_delete_CDConvexMeshInterpolateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDConvexMeshInterpolateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDConvexMeshInterpolateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class CDSphereIf : CDConvexIf {
        public CDSphereIf() { _this = SprExport.Spr_new_CDSphereIf(); _flag = true; }
        public CDSphereIf(IntPtr ptr) : base(ptr) { }
        public CDSphereIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDSphereIf(CsCastObject target) {
            return (target._info.Inherit(CDSphereIf.GetIfInfoStatic()) ? new CDSphereIf(target._this, target._flag) : null);
        }
        ~CDSphereIf() { if (_flag) { SprExport.Spr_delete_CDSphereIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDSphereIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDSphereIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDSphereIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDSphereIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public float GetRadius() {
            float result = (float)SprExport.Spr_CDSphereIf_GetRadius((IntPtr)_this);
            return result;
        }
        public void SetRadius(float r) {
            SprExport.Spr_CDSphereIf_SetRadius((IntPtr)_this, (float)r);
        }
    }
    public partial class CDEllipsoidIf : CDConvexIf {
        public CDEllipsoidIf() { _this = SprExport.Spr_new_CDEllipsoidIf(); _flag = true; }
        public CDEllipsoidIf(IntPtr ptr) : base(ptr) { }
        public CDEllipsoidIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDEllipsoidIf(CsCastObject target) {
            return (target._info.Inherit(CDEllipsoidIf.GetIfInfoStatic()) ? new CDEllipsoidIf(target._this, target._flag) : null);
        }
        ~CDEllipsoidIf() { if (_flag) { SprExport.Spr_delete_CDEllipsoidIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDEllipsoidIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDEllipsoidIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Vec3d GetRadius() {
            IntPtr ptr = SprExport.Spr_CDEllipsoidIf_GetRadius((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetRadius(Vec3d r) {
            SprExport.Spr_CDEllipsoidIf_SetRadius((IntPtr)_this, (IntPtr)r);
        }
    }
    public partial class CDCapsuleIf : CDConvexIf {
        public CDCapsuleIf() { _this = SprExport.Spr_new_CDCapsuleIf(); _flag = true; }
        public CDCapsuleIf(IntPtr ptr) : base(ptr) { }
        public CDCapsuleIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDCapsuleIf(CsCastObject target) {
            return (target._info.Inherit(CDCapsuleIf.GetIfInfoStatic()) ? new CDCapsuleIf(target._this, target._flag) : null);
        }
        ~CDCapsuleIf() { if (_flag) { SprExport.Spr_delete_CDCapsuleIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDCapsuleIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDCapsuleIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDCapsuleIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDCapsuleIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public float GetRadius() {
            float result = (float)SprExport.Spr_CDCapsuleIf_GetRadius((IntPtr)_this);
            return result;
        }
        public void SetRadius(float r) {
            SprExport.Spr_CDCapsuleIf_SetRadius((IntPtr)_this, (float)r);
        }
        public float GetLength() {
            float result = (float)SprExport.Spr_CDCapsuleIf_GetLength((IntPtr)_this);
            return result;
        }
        public void SetLength(float l) {
            SprExport.Spr_CDCapsuleIf_SetLength((IntPtr)_this, (float)l);
        }
    }
    public partial class CDRoundConeIf : CDConvexIf {
        public CDRoundConeIf() { _this = SprExport.Spr_new_CDRoundConeIf(); _flag = true; }
        public CDRoundConeIf(IntPtr ptr) : base(ptr) { }
        public CDRoundConeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDRoundConeIf(CsCastObject target) {
            return (target._info.Inherit(CDRoundConeIf.GetIfInfoStatic()) ? new CDRoundConeIf(target._this, target._flag) : null);
        }
        ~CDRoundConeIf() { if (_flag) { SprExport.Spr_delete_CDRoundConeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDRoundConeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDRoundConeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Vec2f GetRadius() {
            IntPtr ptr = SprExport.Spr_CDRoundConeIf_GetRadius((IntPtr)_this);
            return new Vec2f(ptr, true);
        }
        public float GetLength() {
            float result = (float)SprExport.Spr_CDRoundConeIf_GetLength((IntPtr)_this);
            return result;
        }
        public void SetRadius(Vec2f r) {
            SprExport.Spr_CDRoundConeIf_SetRadius((IntPtr)_this, (IntPtr)r);
        }
        public void SetLength(float l) {
            SprExport.Spr_CDRoundConeIf_SetLength((IntPtr)_this, (float)l);
        }
        public void SetWidth(Vec2f r) {
            SprExport.Spr_CDRoundConeIf_SetWidth((IntPtr)_this, (IntPtr)r);
        }
    }
    public partial class CDBoxIf : CDConvexIf {
        public CDBoxIf() { _this = SprExport.Spr_new_CDBoxIf(); _flag = true; }
        public CDBoxIf(IntPtr ptr) : base(ptr) { }
        public CDBoxIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDBoxIf(CsCastObject target) {
            return (target._info.Inherit(CDBoxIf.GetIfInfoStatic()) ? new CDBoxIf(target._this, target._flag) : null);
        }
        ~CDBoxIf() { if (_flag) { SprExport.Spr_delete_CDBoxIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDBoxIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Vec3f GetBoxSize() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_GetBoxSize((IntPtr)_this);
            return new Vec3f(ptr, true);
        }
        public arraywrapper_Vec3f GetVertices() {
            IntPtr ptr = SprExport.Spr_CDBoxIf_GetVertices((IntPtr)_this);
            return new arraywrapper_Vec3f(ptr);
        }
        public CDFaceIf GetFace(int i) {
            IntPtr ptr = SprExport.Spr_CDBoxIf_GetFace((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            CDFaceIf obj = new CDFaceIf(ptr);
            return obj;
        }
        public Vec3f SetBoxSize(Vec3f boxsize) {
            IntPtr ptr = SprExport.Spr_CDBoxIf_SetBoxSize((IntPtr)_this, (IntPtr)boxsize);
            return new Vec3f(ptr, true);
        }
    }
    public partial class CDShapePairIf : ObjectIf {
        public CDShapePairIf() { _this = SprExport.Spr_new_CDShapePairIf(); _flag = true; }
        public CDShapePairIf(IntPtr ptr) : base(ptr) { }
        public CDShapePairIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDShapePairIf(CsCastObject target) {
            return (target._info.Inherit(CDShapePairIf.GetIfInfoStatic()) ? new CDShapePairIf(target._this, target._flag) : null);
        }
        ~CDShapePairIf() { if (_flag) { SprExport.Spr_delete_CDShapePairIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDShapePairIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public CDShapeIf GetShape(int i) {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetShape((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            if (obj.GetIfInfo() == CDConvexIf.GetIfInfoStatic()) { return new CDConvexIf(ptr); }
            if (obj.GetIfInfo() == CDConvexMeshIf.GetIfInfoStatic()) { return new CDConvexMeshIf(ptr); }
            if (obj.GetIfInfo() == CDSphereIf.GetIfInfoStatic()) { return new CDSphereIf(ptr); }
            if (obj.GetIfInfo() == CDEllipsoidIf.GetIfInfoStatic()) { return new CDEllipsoidIf(ptr); }
            if (obj.GetIfInfo() == CDCapsuleIf.GetIfInfoStatic()) { return new CDCapsuleIf(ptr); }
            if (obj.GetIfInfo() == CDRoundConeIf.GetIfInfoStatic()) { return new CDRoundConeIf(ptr); }
            if (obj.GetIfInfo() == CDBoxIf.GetIfInfoStatic()) { return new CDBoxIf(ptr); }
            if (obj.GetIfInfo() == CDConvexMeshInterpolateIf.GetIfInfoStatic()) { return new CDConvexMeshInterpolateIf(ptr); }
            return obj;
        }
        public Posed GetShapePose(int i) {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetShapePose((IntPtr)_this, (int)i);
            return new Posed(ptr, true);
        }
        public Vec3d GetClosestPointOnWorld(int i) {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetClosestPointOnWorld((IntPtr)_this, (int)i);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetNormal() {
            IntPtr ptr = SprExport.Spr_CDShapePairIf_GetNormal((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
    }
    public partial class CDSpatialHashTableIf : ObjectIf {
        public CDSpatialHashTableIf() { _this = SprExport.Spr_new_CDSpatialHashTableIf(); _flag = true; }
        public CDSpatialHashTableIf(IntPtr ptr) : base(ptr) { }
        public CDSpatialHashTableIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator CDSpatialHashTableIf(CsCastObject target) {
            return (target._info.Inherit(CDSpatialHashTableIf.GetIfInfoStatic()) ? new CDSpatialHashTableIf(target._this, target._flag) : null);
        }
        ~CDSpatialHashTableIf() { if (_flag) { SprExport.Spr_delete_CDSpatialHashTableIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_CDSpatialHashTableIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_CDSpatialHashTableIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void Init(float cellSize, CDBounds bounds) {
            SprExport.Spr_CDSpatialHashTableIf_Init((IntPtr)_this, (float)cellSize, (IntPtr)bounds);
        }
        public bool ReducedSelectPointsToAdd(Vec3f pCtr, float radius, int objInd, int pInd, int timestamp) {
            char ret = SprExport.Spr_CDSpatialHashTableIf_ReducedSelectPointsToAdd((IntPtr)_this, (IntPtr)pCtr, (float)radius, (int)objInd, (int)pInd, (int)timestamp);
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
