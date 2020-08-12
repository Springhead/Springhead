using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
#pragma warning disable 0108
namespace SprCs {
    public partial class PHConstraintEngineDesc : CsObject {
        public PHConstraintEngineDesc() { _this = SprExport.Spr_new_PHConstraintEngineDesc(); _flag = true; }
        public PHConstraintEngineDesc(IntPtr ptr) : base(ptr) { }
        public PHConstraintEngineDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHConstraintEngineDesc() { if (_flag) { SprExport.Spr_delete_PHConstraintEngineDesc(_this); _flag = false; } }
        public int method {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_method(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_method(_this, value); }
        }
        public int numIter {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_numIter(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_numIter(_this, value); }
        }
        public int numIterCorrection {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_numIterCorrection(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_numIterCorrection(_this, value); }
        }
        public int numIterContactCorrection {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_numIterContactCorrection(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_numIterContactCorrection(_this, value); }
        }
        public double velCorrectionRate {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_velCorrectionRate(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_velCorrectionRate(_this, value); }
        }
        public double posCorrectionRate {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_posCorrectionRate(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_posCorrectionRate(_this, value); }
        }
        public double contactCorrectionRate {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_contactCorrectionRate(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_contactCorrectionRate(_this, value); }
        }
        public double shrinkRate {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_shrinkRate(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_shrinkRate(_this, value); }
        }
        public double shrinkRateCorrection {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_shrinkRateCorrection(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_shrinkRateCorrection(_this, value); }
        }
        public double freezeThreshold {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_freezeThreshold(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_freezeThreshold(_this, value); }
        }
        public double accelSOR {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_accelSOR(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_accelSOR(_this, value); }
        }
        public double dfEps {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_dfEps(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_dfEps(_this, value); }
        }
        public double regularization {
            get { return SprExport.Spr_PHConstraintEngineDesc_get_regularization(_this); }
            set { SprExport.Spr_PHConstraintEngineDesc_set_regularization(_this, value); }
        }
        public bool bSaveConstraints {
            get {
                byte ret = (byte)SprExport.Spr_PHConstraintEngineDesc_get_bSaveConstraints(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHConstraintEngineDesc_set_bSaveConstraints(_this, (char)val);
            }
        }
        public bool bUpdateAllState {
            get {
                byte ret = (byte)SprExport.Spr_PHConstraintEngineDesc_get_bUpdateAllState(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHConstraintEngineDesc_set_bUpdateAllState(_this, (char)val);
            }
        }
        public bool bUseContactSurface {
            get {
                byte ret = (byte)SprExport.Spr_PHConstraintEngineDesc_get_bUseContactSurface(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHConstraintEngineDesc_set_bUseContactSurface(_this, (char)val);
            }
        }
        public bool bReport {
            get {
                byte ret = (byte)SprExport.Spr_PHConstraintEngineDesc_get_bReport(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHConstraintEngineDesc_set_bReport(_this, (char)val);
            }
        }
    }
    [System.Serializable]
    public class PHConstraintEngineDescStruct : CsObject {
        public PHConstraintEngineDescStruct() {
            PHConstraintEngineDesc desc = new PHConstraintEngineDesc();
            ApplyFrom(desc, false);
        }
        public int method;
        public int numIter;
        public int numIterCorrection;
        public int numIterContactCorrection;
        public double velCorrectionRate;
        public double posCorrectionRate;
        public double contactCorrectionRate;
        public double shrinkRate;
        public double shrinkRateCorrection;
        public double freezeThreshold;
        public double accelSOR;
        public double dfEps;
        public double regularization;
        public bool bSaveConstraints;
        public bool bUpdateAllState;
        public bool bUseContactSurface;
        public bool bReport;
        public void ApplyTo(PHConstraintEngineDesc r, bool apply_to_base_class = false) {
            r.method = method;
            r.numIter = numIter;
            r.numIterCorrection = numIterCorrection;
            r.numIterContactCorrection = numIterContactCorrection;
            r.velCorrectionRate = velCorrectionRate;
            r.posCorrectionRate = posCorrectionRate;
            r.contactCorrectionRate = contactCorrectionRate;
            r.shrinkRate = shrinkRate;
            r.shrinkRateCorrection = shrinkRateCorrection;
            r.freezeThreshold = freezeThreshold;
            r.accelSOR = accelSOR;
            r.dfEps = dfEps;
            r.regularization = regularization;
            r.bSaveConstraints = bSaveConstraints;
            r.bUpdateAllState = bUpdateAllState;
            r.bUseContactSurface = bUseContactSurface;
            r.bReport = bReport;
        }
        public void ApplyFrom(PHConstraintEngineDesc r, bool apply_to_base_class = false) {
            method = r.method;
            numIter = r.numIter;
            numIterCorrection = r.numIterCorrection;
            numIterContactCorrection = r.numIterContactCorrection;
            velCorrectionRate = r.velCorrectionRate;
            posCorrectionRate = r.posCorrectionRate;
            contactCorrectionRate = r.contactCorrectionRate;
            shrinkRate = r.shrinkRate;
            shrinkRateCorrection = r.shrinkRateCorrection;
            freezeThreshold = r.freezeThreshold;
            accelSOR = r.accelSOR;
            dfEps = r.dfEps;
            regularization = r.regularization;
            bSaveConstraints = r.bSaveConstraints;
            bUpdateAllState = r.bUpdateAllState;
            bUseContactSurface = r.bUseContactSurface;
            bReport = r.bReport;
        }
        public static implicit operator PHConstraintEngineDesc(PHConstraintEngineDescStruct m) {
            PHConstraintEngineDesc r = new PHConstraintEngineDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHConstraintEngineDescStruct(PHConstraintEngineDesc r) {
            PHConstraintEngineDescStruct m = new PHConstraintEngineDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKEngineDesc : CsObject {
        public PHIKEngineDesc() { _this = SprExport.Spr_new_PHIKEngineDesc(); _flag = true; }
        public PHIKEngineDesc(IntPtr ptr) : base(ptr) { }
        public PHIKEngineDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKEngineDesc() { if (_flag) { SprExport.Spr_delete_PHIKEngineDesc(_this); _flag = false; } }
        public ulong numIter {
            get { return SprExport.Spr_PHIKEngineDesc_get_numIter(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_numIter(_this, value); }
        }
        public double maxVel {
            get { return SprExport.Spr_PHIKEngineDesc_get_maxVel(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_maxVel(_this, value); }
        }
        public double maxAngVel {
            get { return SprExport.Spr_PHIKEngineDesc_get_maxAngVel(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_maxAngVel(_this, value); }
        }
        public double maxActVel {
            get { return SprExport.Spr_PHIKEngineDesc_get_maxActVel(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_maxActVel(_this, value); }
        }
        public double regularizeParam {
            get { return SprExport.Spr_PHIKEngineDesc_get_regularizeParam(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_regularizeParam(_this, value); }
        }
        public double regularizeParam2 {
            get { return SprExport.Spr_PHIKEngineDesc_get_regularizeParam2(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_regularizeParam2(_this, value); }
        }
        public int regularizeMode {
            get { return SprExport.Spr_PHIKEngineDesc_get_regularizeMode(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_regularizeMode(_this, value); }
        }
        public int iterGaussSeidel {
            get { return SprExport.Spr_PHIKEngineDesc_get_iterGaussSeidel(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_iterGaussSeidel(_this, value); }
        }
        public Mode solverMode {
            get { return (Mode)SprExport.Spr_PHIKEngineDesc_get_solverMode(_this); }
            set { SprExport.Spr_PHIKEngineDesc_set_solverMode(_this, (int)value); }
        }
        public enum Mode : int {
            SVD = 0, QR = SVD + 1, LM = QR + 1
        }
    }
    [System.Serializable]
    public class PHIKEngineDescStruct : CsObject {
        public PHIKEngineDescStruct() {
            PHIKEngineDesc desc = new PHIKEngineDesc();
            ApplyFrom(desc, false);
        }
        public ulong numIter;
        public double maxVel;
        public double maxAngVel;
        public double maxActVel;
        public double regularizeParam;
        public double regularizeParam2;
        public int regularizeMode;
        public int iterGaussSeidel;
        public PHIKEngineDesc.Mode solverMode;
        public void ApplyTo(PHIKEngineDesc r, bool apply_to_base_class = false) {
            r.numIter = numIter;
            r.maxVel = maxVel;
            r.maxAngVel = maxAngVel;
            r.maxActVel = maxActVel;
            r.regularizeParam = regularizeParam;
            r.regularizeParam2 = regularizeParam2;
            r.regularizeMode = regularizeMode;
            r.iterGaussSeidel = iterGaussSeidel;
            r.solverMode = solverMode;
        }
        public void ApplyFrom(PHIKEngineDesc r, bool apply_to_base_class = false) {
            numIter = r.numIter;
            maxVel = r.maxVel;
            maxAngVel = r.maxAngVel;
            maxActVel = r.maxActVel;
            regularizeParam = r.regularizeParam;
            regularizeParam2 = r.regularizeParam2;
            regularizeMode = r.regularizeMode;
            iterGaussSeidel = r.iterGaussSeidel;
            solverMode = r.solverMode;
        }
        public static implicit operator PHIKEngineDesc(PHIKEngineDescStruct m) {
            PHIKEngineDesc r = new PHIKEngineDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKEngineDescStruct(PHIKEngineDesc r) {
            PHIKEngineDescStruct m = new PHIKEngineDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemMeshState : CsObject {
        public PHFemMeshState() { _this = SprExport.Spr_new_PHFemMeshState(); _flag = true; }
        public PHFemMeshState(IntPtr ptr) : base(ptr) { }
        public PHFemMeshState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemMeshState() { if (_flag) { SprExport.Spr_delete_PHFemMeshState(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHFemMeshStateStruct : CsObject {
        public PHFemMeshStateStruct() {
            PHFemMeshState desc = new PHFemMeshState();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHFemMeshState r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHFemMeshState r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHFemMeshState(PHFemMeshStateStruct m) {
            PHFemMeshState r = new PHFemMeshState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemMeshStateStruct(PHFemMeshState r) {
            PHFemMeshStateStruct m = new PHFemMeshStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemMeshDesc : PHFemMeshState {
        public PHFemMeshDesc() { _this = SprExport.Spr_new_PHFemMeshDesc(); _flag = true; }
        public PHFemMeshDesc(IntPtr ptr) : base(ptr) { }
        public PHFemMeshDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemMeshDesc() { if (_flag) { SprExport.Spr_delete_PHFemMeshDesc(_this); _flag = false; } }
        public vectorwrapper_Vec3d vertices {
            get { return new vectorwrapper_Vec3d(SprExport.Spr_PHFemMeshDesc_addr_vertices(_this)); }
            set { SprExport.Spr_PHFemMeshDesc_set_vertices(_this, value); }
        }
        public vectorwrapper_int tets {
            get { return new vectorwrapper_int(SprExport.Spr_PHFemMeshDesc_addr_tets(_this)); }
            set { SprExport.Spr_PHFemMeshDesc_set_tets(_this, value); }
        }
        public vectorwrapper_int faces {
            get { return new vectorwrapper_int(SprExport.Spr_PHFemMeshDesc_addr_faces(_this)); }
            set { SprExport.Spr_PHFemMeshDesc_set_faces(_this, value); }
        }
        public void Init() {
            SprExport.Spr_PHFemMeshDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHFemMeshDescStruct : PHFemMeshStateStruct {
        public PHFemMeshDescStruct() {
            PHFemMeshDesc desc = new PHFemMeshDesc();
            ApplyFrom(desc, false);
        }
        public vectorwrapper_Vec3d vertices;
        public vectorwrapper_int tets;
        public vectorwrapper_int faces;
        public void ApplyTo(PHFemMeshDesc r, bool apply_to_base_class = false) {
            r.vertices = vertices;
            r.tets = tets;
            r.faces = faces;
        }
        public void ApplyFrom(PHFemMeshDesc r, bool apply_to_base_class = false) {
            vertices = r.vertices;
            tets = r.tets;
            faces = r.faces;
        }
        public static implicit operator PHFemMeshDesc(PHFemMeshDescStruct m) {
            PHFemMeshDesc r = new PHFemMeshDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemMeshDescStruct(PHFemMeshDesc r) {
            PHFemMeshDescStruct m = new PHFemMeshDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemMeshThermoDesc : PHFemMeshDesc {
        public PHFemMeshThermoDesc() { _this = SprExport.Spr_new_PHFemMeshThermoDesc(); _flag = true; }
        public PHFemMeshThermoDesc(IntPtr ptr) : base(ptr) { }
        public PHFemMeshThermoDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemMeshThermoDesc() { if (_flag) { SprExport.Spr_delete_PHFemMeshThermoDesc(_this); _flag = false; } }
        public double rho {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_rho(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_rho(_this, value); }
        }
        public double thConduct {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_thConduct(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_thConduct(_this, value); }
        }
        public double thConduct_x {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_thConduct_x(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_thConduct_x(_this, value); }
        }
        public double thConduct_y {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_thConduct_y(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_thConduct_y(_this, value); }
        }
        public double thConduct_z {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_thConduct_z(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_thConduct_z(_this, value); }
        }
        public double heatTrans {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_heatTrans(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_heatTrans(_this, value); }
        }
        public double specificHeat {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_specificHeat(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_specificHeat(_this, value); }
        }
        public double radiantHeat {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_radiantHeat(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_radiantHeat(_this, value); }
        }
        public double initial_temp {
            get { return SprExport.Spr_PHFemMeshThermoDesc_get_initial_temp(_this); }
            set { SprExport.Spr_PHFemMeshThermoDesc_set_initial_temp(_this, value); }
        }
        public void Init() {
            SprExport.Spr_PHFemMeshThermoDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHFemMeshThermoDescStruct : PHFemMeshDescStruct {
        public PHFemMeshThermoDescStruct() {
            PHFemMeshThermoDesc desc = new PHFemMeshThermoDesc();
            ApplyFrom(desc, true);
        }
        public double rho;
        public double thConduct;
        public double thConduct_x;
        public double thConduct_y;
        public double thConduct_z;
        public double heatTrans;
        public double specificHeat;
        public double radiantHeat;
        public double initial_temp;
        public void ApplyTo(PHFemMeshThermoDesc r, bool apply_to_base_class = false) {
            r.rho = rho;
            r.thConduct = thConduct;
            r.thConduct_x = thConduct_x;
            r.thConduct_y = thConduct_y;
            r.thConduct_z = thConduct_z;
            r.heatTrans = heatTrans;
            r.specificHeat = specificHeat;
            r.radiantHeat = radiantHeat;
            r.initial_temp = initial_temp;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFemMeshThermoDesc r, bool apply_to_base_class = false) {
            rho = r.rho;
            thConduct = r.thConduct;
            thConduct_x = r.thConduct_x;
            thConduct_y = r.thConduct_y;
            thConduct_z = r.thConduct_z;
            heatTrans = r.heatTrans;
            specificHeat = r.specificHeat;
            radiantHeat = r.radiantHeat;
            initial_temp = r.initial_temp;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFemMeshThermoDesc(PHFemMeshThermoDescStruct m) {
            PHFemMeshThermoDesc r = new PHFemMeshThermoDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemMeshThermoDescStruct(PHFemMeshThermoDesc r) {
            PHFemMeshThermoDescStruct m = new PHFemMeshThermoDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemMeshNewState : CsObject {
        public PHFemMeshNewState() { _this = SprExport.Spr_new_PHFemMeshNewState(); _flag = true; }
        public PHFemMeshNewState(IntPtr ptr) : base(ptr) { }
        public PHFemMeshNewState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemMeshNewState() { if (_flag) { SprExport.Spr_delete_PHFemMeshNewState(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHFemMeshNewStateStruct : CsObject {
        public PHFemMeshNewStateStruct() {
            PHFemMeshNewState desc = new PHFemMeshNewState();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHFemMeshNewState r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHFemMeshNewState r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHFemMeshNewState(PHFemMeshNewStateStruct m) {
            PHFemMeshNewState r = new PHFemMeshNewState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemMeshNewStateStruct(PHFemMeshNewState r) {
            PHFemMeshNewStateStruct m = new PHFemMeshNewStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemMeshNewDesc : PHFemMeshNewState {
        public PHFemMeshNewDesc() { _this = SprExport.Spr_new_PHFemMeshNewDesc(); _flag = true; }
        public PHFemMeshNewDesc(IntPtr ptr) : base(ptr) { }
        public PHFemMeshNewDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemMeshNewDesc() { if (_flag) { SprExport.Spr_delete_PHFemMeshNewDesc(_this); _flag = false; } }
        public vectorwrapper_Vec3d vertices {
            get { return new vectorwrapper_Vec3d(SprExport.Spr_PHFemMeshNewDesc_addr_vertices(_this)); }
            set { SprExport.Spr_PHFemMeshNewDesc_set_vertices(_this, value); }
        }
        public vectorwrapper_int tets {
            get { return new vectorwrapper_int(SprExport.Spr_PHFemMeshNewDesc_addr_tets(_this)); }
            set { SprExport.Spr_PHFemMeshNewDesc_set_tets(_this, value); }
        }
        public vectorwrapper_int faces {
            get { return new vectorwrapper_int(SprExport.Spr_PHFemMeshNewDesc_addr_faces(_this)); }
            set { SprExport.Spr_PHFemMeshNewDesc_set_faces(_this, value); }
        }
        public bool spheric {
            get {
                byte ret = (byte)SprExport.Spr_PHFemMeshNewDesc_get_spheric(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHFemMeshNewDesc_set_spheric(_this, (char)val);
            }
        }
    }
    [System.Serializable]
    public class PHFemMeshNewDescStruct : PHFemMeshNewStateStruct {
        public PHFemMeshNewDescStruct() {
            PHFemMeshNewDesc desc = new PHFemMeshNewDesc();
            ApplyFrom(desc, false);
        }
        public vectorwrapper_Vec3d vertices;
        public vectorwrapper_int tets;
        public vectorwrapper_int faces;
        public bool spheric;
        public void ApplyTo(PHFemMeshNewDesc r, bool apply_to_base_class = false) {
            r.vertices = vertices;
            r.tets = tets;
            r.faces = faces;
            r.spheric = spheric;
        }
        public void ApplyFrom(PHFemMeshNewDesc r, bool apply_to_base_class = false) {
            vertices = r.vertices;
            tets = r.tets;
            faces = r.faces;
            spheric = r.spheric;
        }
        public static implicit operator PHFemMeshNewDesc(PHFemMeshNewDescStruct m) {
            PHFemMeshNewDesc r = new PHFemMeshNewDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemMeshNewDescStruct(PHFemMeshNewDesc r) {
            PHFemMeshNewDescStruct m = new PHFemMeshNewDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemBaseDesc : CsObject {
        public PHFemBaseDesc() { _this = SprExport.Spr_new_PHFemBaseDesc(); _flag = true; }
        public PHFemBaseDesc(IntPtr ptr) : base(ptr) { }
        public PHFemBaseDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemBaseDesc() { if (_flag) { SprExport.Spr_delete_PHFemBaseDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHFemBaseDescStruct : CsObject {
        public PHFemBaseDescStruct() {
            PHFemBaseDesc desc = new PHFemBaseDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHFemBaseDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHFemBaseDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHFemBaseDesc(PHFemBaseDescStruct m) {
            PHFemBaseDesc r = new PHFemBaseDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemBaseDescStruct(PHFemBaseDesc r) {
            PHFemBaseDescStruct m = new PHFemBaseDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemVibrationDesc : PHFemBaseDesc {
        public PHFemVibrationDesc() { _this = SprExport.Spr_new_PHFemVibrationDesc(); _flag = true; }
        public PHFemVibrationDesc(IntPtr ptr) : base(ptr) { }
        public PHFemVibrationDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemVibrationDesc() { if (_flag) { SprExport.Spr_delete_PHFemVibrationDesc(_this); _flag = false; } }
        public double young {
            get { return SprExport.Spr_PHFemVibrationDesc_get_young(_this); }
            set { SprExport.Spr_PHFemVibrationDesc_set_young(_this, value); }
        }
        public double poisson {
            get { return SprExport.Spr_PHFemVibrationDesc_get_poisson(_this); }
            set { SprExport.Spr_PHFemVibrationDesc_set_poisson(_this, value); }
        }
        public double density {
            get { return SprExport.Spr_PHFemVibrationDesc_get_density(_this); }
            set { SprExport.Spr_PHFemVibrationDesc_set_density(_this, value); }
        }
        public double alpha {
            get { return SprExport.Spr_PHFemVibrationDesc_get_alpha(_this); }
            set { SprExport.Spr_PHFemVibrationDesc_set_alpha(_this, value); }
        }
        public double beta {
            get { return SprExport.Spr_PHFemVibrationDesc_get_beta(_this); }
            set { SprExport.Spr_PHFemVibrationDesc_set_beta(_this, value); }
        }
        public vectorwrapper_int fixedVertices {
            get { return new vectorwrapper_int(SprExport.Spr_PHFemVibrationDesc_addr_fixedVertices(_this)); }
            set { SprExport.Spr_PHFemVibrationDesc_set_fixedVertices(_this, value); }
        }
        public enum ANALYSIS_MODE : int {
            ANALYSIS_DIRECT = 0, ANALYSIS_MODAL = ANALYSIS_DIRECT + 1
        }
        public enum INTEGRATION_MODE : int {
            INT_EXPLICIT_EULER = 0, INT_IMPLICIT_EULER = INT_EXPLICIT_EULER + 1, INT_SIMPLECTIC = INT_IMPLICIT_EULER + 1, INT_NEWMARK_BETA = INT_SIMPLECTIC + 1
        }
    }
    [System.Serializable]
    public class PHFemVibrationDescStruct : PHFemBaseDescStruct {
        public PHFemVibrationDescStruct() {
            PHFemVibrationDesc desc = new PHFemVibrationDesc();
            ApplyFrom(desc, true);
        }
        public double young;
        public double poisson;
        public double density;
        public double alpha;
        public double beta;
        public vectorwrapper_int fixedVertices;
        public void ApplyTo(PHFemVibrationDesc r, bool apply_to_base_class = false) {
            r.young = young;
            r.poisson = poisson;
            r.density = density;
            r.alpha = alpha;
            r.beta = beta;
            r.fixedVertices = fixedVertices;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFemVibrationDesc r, bool apply_to_base_class = false) {
            young = r.young;
            poisson = r.poisson;
            density = r.density;
            alpha = r.alpha;
            beta = r.beta;
            fixedVertices = r.fixedVertices;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFemVibrationDesc(PHFemVibrationDescStruct m) {
            PHFemVibrationDesc r = new PHFemVibrationDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemVibrationDescStruct(PHFemVibrationDesc r) {
            PHFemVibrationDescStruct m = new PHFemVibrationDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemThermoDesc : PHFemBaseDesc {
        public PHFemThermoDesc() { _this = SprExport.Spr_new_PHFemThermoDesc(); _flag = true; }
        public PHFemThermoDesc(IntPtr ptr) : base(ptr) { }
        public PHFemThermoDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemThermoDesc() { if (_flag) { SprExport.Spr_delete_PHFemThermoDesc(_this); _flag = false; } }
        public double rho {
            get { return SprExport.Spr_PHFemThermoDesc_get_rho(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_rho(_this, value); }
        }
        public double heatTrans {
            get { return SprExport.Spr_PHFemThermoDesc_get_heatTrans(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_heatTrans(_this, value); }
        }
        public double specificHeat {
            get { return SprExport.Spr_PHFemThermoDesc_get_specificHeat(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_specificHeat(_this, value); }
        }
        public double radiantHeat {
            get { return SprExport.Spr_PHFemThermoDesc_get_radiantHeat(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_radiantHeat(_this, value); }
        }
        public double initial_temp {
            get { return SprExport.Spr_PHFemThermoDesc_get_initial_temp(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_initial_temp(_this, value); }
        }
        public double weekPow_full {
            get { return SprExport.Spr_PHFemThermoDesc_get_weekPow_full(_this); }
            set { SprExport.Spr_PHFemThermoDesc_set_weekPow_full(_this, value); }
        }
        public void Init() {
            SprExport.Spr_PHFemThermoDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHFemThermoDescStruct : PHFemBaseDescStruct {
        public PHFemThermoDescStruct() {
            PHFemThermoDesc desc = new PHFemThermoDesc();
            ApplyFrom(desc, true);
        }
        public double rho;
        public double heatTrans;
        public double specificHeat;
        public double radiantHeat;
        public double initial_temp;
        public double weekPow_full;
        public void ApplyTo(PHFemThermoDesc r, bool apply_to_base_class = false) {
            r.rho = rho;
            r.heatTrans = heatTrans;
            r.specificHeat = specificHeat;
            r.radiantHeat = radiantHeat;
            r.initial_temp = initial_temp;
            r.weekPow_full = weekPow_full;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFemThermoDesc r, bool apply_to_base_class = false) {
            rho = r.rho;
            heatTrans = r.heatTrans;
            specificHeat = r.specificHeat;
            radiantHeat = r.radiantHeat;
            initial_temp = r.initial_temp;
            weekPow_full = r.weekPow_full;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFemThermoDesc(PHFemThermoDescStruct m) {
            PHFemThermoDesc r = new PHFemThermoDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemThermoDescStruct(PHFemThermoDesc r) {
            PHFemThermoDescStruct m = new PHFemThermoDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFemPorousWOMoveDesc : PHFemBaseDesc {
        public PHFemPorousWOMoveDesc() { _this = SprExport.Spr_new_PHFemPorousWOMoveDesc(); _flag = true; }
        public PHFemPorousWOMoveDesc(IntPtr ptr) : base(ptr) { }
        public PHFemPorousWOMoveDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFemPorousWOMoveDesc() { if (_flag) { SprExport.Spr_delete_PHFemPorousWOMoveDesc(_this); _flag = false; } }
        public double wDiffAir {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_wDiffAir(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_wDiffAir(_this, value); }
        }
        public double K {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_K(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_K(_this, value); }
        }
        public double kc {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_kc(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_kc(_this, value); }
        }
        public double kp {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_kp(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_kp(_this, value); }
        }
        public double alpha {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_alpha(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_alpha(_this, value); }
        }
        public double gamma {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_gamma(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_gamma(_this, value); }
        }
        public double rhoWater {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_rhoWater(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_rhoWater(_this, value); }
        }
        public double rhoOil {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_rhoOil(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_rhoOil(_this, value); }
        }
        public double rhowInit {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_rhowInit(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_rhowInit(_this, value); }
        }
        public double rhooInit {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_rhooInit(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_rhooInit(_this, value); }
        }
        public double evapoRate {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_evapoRate(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_evapoRate(_this, value); }
        }
        public double denatTemp {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_denatTemp(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_denatTemp(_this, value); }
        }
        public double boundWaterRatio {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_boundWaterRatio(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_boundWaterRatio(_this, value); }
        }
        public double equilWaterCont {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_equilWaterCont(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_equilWaterCont(_this, value); }
        }
        public double limitWaterCont {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_limitWaterCont(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_limitWaterCont(_this, value); }
        }
        public double boundaryThick {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_boundaryThick(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_boundaryThick(_this, value); }
        }
        public double initMassAll {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_initMassAll(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_initMassAll(_this, value); }
        }
        public double initWaterRatio {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_initWaterRatio(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_initWaterRatio(_this, value); }
        }
        public double initOilRatio {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_initOilRatio(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_initOilRatio(_this, value); }
        }
        public double shrinkageRatio {
            get { return SprExport.Spr_PHFemPorousWOMoveDesc_get_shrinkageRatio(_this); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_shrinkageRatio(_this, value); }
        }
        public Vec3d top {
            get { return new Vec3d(SprExport.Spr_PHFemPorousWOMoveDesc_addr_top(_this)); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_top(_this, value); }
        }
        public Vec3d center {
            get { return new Vec3d(SprExport.Spr_PHFemPorousWOMoveDesc_addr_center(_this)); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_center(_this, value); }
        }
        public Vec3d bottom {
            get { return new Vec3d(SprExport.Spr_PHFemPorousWOMoveDesc_addr_bottom(_this)); }
            set { SprExport.Spr_PHFemPorousWOMoveDesc_set_bottom(_this, value); }
        }
        public void Init() {
            SprExport.Spr_PHFemPorousWOMoveDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHFemPorousWOMoveDescStruct : PHFemBaseDescStruct {
        public PHFemPorousWOMoveDescStruct() {
            PHFemPorousWOMoveDesc desc = new PHFemPorousWOMoveDesc();
            ApplyFrom(desc, true);
        }
        public double wDiffAir;
        public double K;
        public double kc;
        public double kp;
        public double alpha;
        public double gamma;
        public double rhoWater;
        public double rhoOil;
        public double rhowInit;
        public double rhooInit;
        public double evapoRate;
        public double denatTemp;
        public double boundWaterRatio;
        public double equilWaterCont;
        public double limitWaterCont;
        public double boundaryThick;
        public double initMassAll;
        public double initWaterRatio;
        public double initOilRatio;
        public double shrinkageRatio;
        public Vec3dStruct top;
        public Vec3dStruct center;
        public Vec3dStruct bottom;
        public void ApplyTo(PHFemPorousWOMoveDesc r, bool apply_to_base_class = false) {
            r.wDiffAir = wDiffAir;
            r.K = K;
            r.kc = kc;
            r.kp = kp;
            r.alpha = alpha;
            r.gamma = gamma;
            r.rhoWater = rhoWater;
            r.rhoOil = rhoOil;
            r.rhowInit = rhowInit;
            r.rhooInit = rhooInit;
            r.evapoRate = evapoRate;
            r.denatTemp = denatTemp;
            r.boundWaterRatio = boundWaterRatio;
            r.equilWaterCont = equilWaterCont;
            r.limitWaterCont = limitWaterCont;
            r.boundaryThick = boundaryThick;
            r.initMassAll = initMassAll;
            r.initWaterRatio = initWaterRatio;
            r.initOilRatio = initOilRatio;
            r.shrinkageRatio = shrinkageRatio;
            r.top = top;
            r.center = center;
            r.bottom = bottom;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFemPorousWOMoveDesc r, bool apply_to_base_class = false) {
            wDiffAir = r.wDiffAir;
            K = r.K;
            kc = r.kc;
            kp = r.kp;
            alpha = r.alpha;
            gamma = r.gamma;
            rhoWater = r.rhoWater;
            rhoOil = r.rhoOil;
            rhowInit = r.rhowInit;
            rhooInit = r.rhooInit;
            evapoRate = r.evapoRate;
            denatTemp = r.denatTemp;
            boundWaterRatio = r.boundWaterRatio;
            equilWaterCont = r.equilWaterCont;
            limitWaterCont = r.limitWaterCont;
            boundaryThick = r.boundaryThick;
            initMassAll = r.initMassAll;
            initWaterRatio = r.initWaterRatio;
            initOilRatio = r.initOilRatio;
            shrinkageRatio = r.shrinkageRatio;
            top = r.top;
            center = r.center;
            bottom = r.bottom;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFemPorousWOMoveDesc(PHFemPorousWOMoveDescStruct m) {
            PHFemPorousWOMoveDesc r = new PHFemPorousWOMoveDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFemPorousWOMoveDescStruct(PHFemPorousWOMoveDesc r) {
            PHFemPorousWOMoveDescStruct m = new PHFemPorousWOMoveDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFrameDesc : CsObject {
        public PHFrameDesc() { _this = SprExport.Spr_new_PHFrameDesc(); _flag = true; }
        public PHFrameDesc(IntPtr ptr) : base(ptr) { }
        public PHFrameDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFrameDesc() { if (_flag) { SprExport.Spr_delete_PHFrameDesc(_this); _flag = false; } }
        public Posed pose {
            get { return new Posed(SprExport.Spr_PHFrameDesc_addr_pose(_this)); }
            set { SprExport.Spr_PHFrameDesc_set_pose(_this, value); }
        }
    }
    [System.Serializable]
    public class PHFrameDescStruct : CsObject {
        public PHFrameDescStruct() {
            PHFrameDesc desc = new PHFrameDesc();
            ApplyFrom(desc, false);
        }
        public PosedStruct pose;
        public void ApplyTo(PHFrameDesc r, bool apply_to_base_class = false) {
            r.pose = pose;
        }
        public void ApplyFrom(PHFrameDesc r, bool apply_to_base_class = false) {
            pose = r.pose;
        }
        public static implicit operator PHFrameDesc(PHFrameDescStruct m) {
            PHFrameDesc r = new PHFrameDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFrameDescStruct(PHFrameDesc r) {
            PHFrameDescStruct m = new PHFrameDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSolidState : CsObject {
        public PHSolidState() { _this = SprExport.Spr_new_PHSolidState(); _flag = true; }
        public PHSolidState(IntPtr ptr) : base(ptr) { }
        public PHSolidState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSolidState() { if (_flag) { SprExport.Spr_delete_PHSolidState(_this); _flag = false; } }
        public Vec3d velocity {
            get { return new Vec3d(SprExport.Spr_PHSolidState_addr_velocity(_this)); }
            set { SprExport.Spr_PHSolidState_set_velocity(_this, value); }
        }
        public Vec3d angVelocity {
            get { return new Vec3d(SprExport.Spr_PHSolidState_addr_angVelocity(_this)); }
            set { SprExport.Spr_PHSolidState_set_angVelocity(_this, value); }
        }
        public Posed pose {
            get { return new Posed(SprExport.Spr_PHSolidState_addr_pose(_this)); }
            set { SprExport.Spr_PHSolidState_set_pose(_this, value); }
        }
    }
    [System.Serializable]
    public class PHSolidStateStruct : CsObject {
        public PHSolidStateStruct() {
            PHSolidState desc = new PHSolidState();
            ApplyFrom(desc, false);
        }
        public Vec3dStruct velocity;
        public Vec3dStruct angVelocity;
        public PosedStruct pose;
        public void ApplyTo(PHSolidState r, bool apply_to_base_class = false) {
            r.velocity = velocity;
            r.angVelocity = angVelocity;
            r.pose = pose;
        }
        public void ApplyFrom(PHSolidState r, bool apply_to_base_class = false) {
            velocity = r.velocity;
            angVelocity = r.angVelocity;
            pose = r.pose;
        }
        public static implicit operator PHSolidState(PHSolidStateStruct m) {
            PHSolidState r = new PHSolidState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSolidStateStruct(PHSolidState r) {
            PHSolidStateStruct m = new PHSolidStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSolidDesc : PHSolidState {
        public PHSolidDesc() { _this = SprExport.Spr_new_PHSolidDesc(); _flag = true; }
        public PHSolidDesc(IntPtr ptr) : base(ptr) { }
        public PHSolidDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSolidDesc() { if (_flag) { SprExport.Spr_delete_PHSolidDesc(_this); _flag = false; } }
        public double mass {
            get { return SprExport.Spr_PHSolidDesc_get_mass(_this); }
            set { SprExport.Spr_PHSolidDesc_set_mass(_this, value); }
        }
        public Matrix3d inertia {
            get { return new Matrix3d(SprExport.Spr_PHSolidDesc_addr_inertia(_this)); }
            set { SprExport.Spr_PHSolidDesc_set_inertia(_this, value); }
        }
        public Vec3d center {
            get { return new Vec3d(SprExport.Spr_PHSolidDesc_addr_center(_this)); }
            set { SprExport.Spr_PHSolidDesc_set_center(_this, value); }
        }
        public bool dynamical {
            get {
                byte ret = (byte)SprExport.Spr_PHSolidDesc_get_dynamical(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSolidDesc_set_dynamical(_this, (char)val);
            }
        }
        public bool stationary {
            get {
                byte ret = (byte)SprExport.Spr_PHSolidDesc_get_stationary(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSolidDesc_set_stationary(_this, (char)val);
            }
        }
        public void Init() {
            SprExport.Spr_PHSolidDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHSolidDescStruct : PHSolidStateStruct {
        public PHSolidDescStruct() {
            PHSolidDesc desc = new PHSolidDesc();
            ApplyFrom(desc, false);
        }
        public double mass;
        public Matrix3dStruct inertia;
        public Vec3dStruct center;
        public bool dynamical;
        public bool stationary;
        public void ApplyTo(PHSolidDesc r, bool apply_to_base_class = false) {
            r.mass = mass;
            r.inertia = inertia;
            r.center = center;
            r.dynamical = dynamical;
            r.stationary = stationary;
        }
        public void ApplyFrom(PHSolidDesc r, bool apply_to_base_class = false) {
            mass = r.mass;
            inertia = r.inertia;
            center = r.center;
            dynamical = r.dynamical;
            stationary = r.stationary;
        }
        public static implicit operator PHSolidDesc(PHSolidDescStruct m) {
            PHSolidDesc r = new PHSolidDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSolidDescStruct(PHSolidDesc r) {
            PHSolidDescStruct m = new PHSolidDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHCollisionListener : CsObject {
        public PHCollisionListener() { _this = SprExport.Spr_new_PHCollisionListener(); _flag = true; }
        public PHCollisionListener(IntPtr ptr) : base(ptr) { }
        public PHCollisionListener(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHCollisionListener() { if (_flag) { SprExport.Spr_delete_PHCollisionListener(_this); _flag = false; } }
        public void OnDetect(PHSolidPairIf sop, CDShapePairIf shp, uint ct, double dt) {
            SprExport.Spr_PHCollisionListener_OnDetect((IntPtr)_this, (IntPtr)sop, (IntPtr)shp, (uint)ct, (double)dt);
        }
        public void OnContDetect(PHSolidPairIf sop, CDShapePairIf shp, uint ct, double dt) {
            SprExport.Spr_PHCollisionListener_OnContDetect((IntPtr)_this, (IntPtr)sop, (IntPtr)shp, (uint)ct, (double)dt);
        }
    }
    public partial class PHHapticPointerDesc : CsObject {
        public PHHapticPointerDesc() { _this = SprExport.Spr_new_PHHapticPointerDesc(); _flag = true; }
        public PHHapticPointerDesc(IntPtr ptr) : base(ptr) { }
        public PHHapticPointerDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHapticPointerDesc() { if (_flag) { SprExport.Spr_delete_PHHapticPointerDesc(_this); _flag = false; } }
        public HapticRenderMode renderMode {
            get { return (HapticRenderMode)SprExport.Spr_PHHapticPointerDesc_get_renderMode(_this); }
            set { SprExport.Spr_PHHapticPointerDesc_set_renderMode(_this, (int)value); }
        }
        public enum HapticRenderMode : int {
            PENALTY = 0, CONSTRAINT = PENALTY + 1, DYNAMIC_PROXY = CONSTRAINT + 1
        }
    }
    [System.Serializable]
    public class PHHapticPointerDescStruct : CsObject {
        public PHHapticPointerDescStruct() {
            PHHapticPointerDesc desc = new PHHapticPointerDesc();
            ApplyFrom(desc, false);
        }
        public PHHapticPointerDesc.HapticRenderMode renderMode;
        public void ApplyTo(PHHapticPointerDesc r, bool apply_to_base_class = false) {
            r.renderMode = renderMode;
        }
        public void ApplyFrom(PHHapticPointerDesc r, bool apply_to_base_class = false) {
            renderMode = r.renderMode;
        }
        public static implicit operator PHHapticPointerDesc(PHHapticPointerDescStruct m) {
            PHHapticPointerDesc r = new PHHapticPointerDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHapticPointerDescStruct(PHHapticPointerDesc r) {
            PHHapticPointerDescStruct m = new PHHapticPointerDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHHapticEngineDesc : CsObject {
        public PHHapticEngineDesc() { _this = SprExport.Spr_new_PHHapticEngineDesc(); _flag = true; }
        public PHHapticEngineDesc(IntPtr ptr) : base(ptr) { }
        public PHHapticEngineDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHapticEngineDesc() { if (_flag) { SprExport.Spr_delete_PHHapticEngineDesc(_this); _flag = false; } }
        public enum HapticStepMode : int {
            SINGLE_THREAD, MULTI_THREAD = SINGLE_THREAD + 1, LOCAL_DYNAMICS = MULTI_THREAD + 1
        }
    }
    [System.Serializable]
    public class PHHapticEngineDescStruct : CsObject {
        public PHHapticEngineDescStruct() {
            PHHapticEngineDesc desc = new PHHapticEngineDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHHapticEngineDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHHapticEngineDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHHapticEngineDesc(PHHapticEngineDescStruct m) {
            PHHapticEngineDesc r = new PHHapticEngineDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHapticEngineDescStruct(PHHapticEngineDesc r) {
            PHHapticEngineDescStruct m = new PHHapticEngineDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKEndEffectorState : CsObject {
        public PHIKEndEffectorState() { _this = SprExport.Spr_new_PHIKEndEffectorState(); _flag = true; }
        public PHIKEndEffectorState(IntPtr ptr) : base(ptr) { }
        public PHIKEndEffectorState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKEndEffectorState() { if (_flag) { SprExport.Spr_delete_PHIKEndEffectorState(_this); _flag = false; } }
        public Posed solidTempPose {
            get { return new Posed(SprExport.Spr_PHIKEndEffectorState_addr_solidTempPose(_this)); }
            set { SprExport.Spr_PHIKEndEffectorState_set_solidTempPose(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKEndEffectorStateStruct : CsObject {
        public PHIKEndEffectorStateStruct() {
            PHIKEndEffectorState desc = new PHIKEndEffectorState();
            ApplyFrom(desc, false);
        }
        public PosedStruct solidTempPose;
        public void ApplyTo(PHIKEndEffectorState r, bool apply_to_base_class = false) {
            r.solidTempPose = solidTempPose;
        }
        public void ApplyFrom(PHIKEndEffectorState r, bool apply_to_base_class = false) {
            solidTempPose = r.solidTempPose;
        }
        public static implicit operator PHIKEndEffectorState(PHIKEndEffectorStateStruct m) {
            PHIKEndEffectorState r = new PHIKEndEffectorState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKEndEffectorStateStruct(PHIKEndEffectorState r) {
            PHIKEndEffectorStateStruct m = new PHIKEndEffectorStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKEndEffectorDesc : PHIKEndEffectorState {
        public PHIKEndEffectorDesc() { _this = SprExport.Spr_new_PHIKEndEffectorDesc(); _flag = true; }
        public PHIKEndEffectorDesc(IntPtr ptr) : base(ptr) { }
        public PHIKEndEffectorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKEndEffectorDesc() { if (_flag) { SprExport.Spr_delete_PHIKEndEffectorDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHIKEndEffectorDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKEndEffectorDesc_set_bEnabled(_this, (char)val);
            }
        }
        public bool bPosition {
            get {
                byte ret = (byte)SprExport.Spr_PHIKEndEffectorDesc_get_bPosition(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKEndEffectorDesc_set_bPosition(_this, (char)val);
            }
        }
        public bool bOrientation {
            get {
                byte ret = (byte)SprExport.Spr_PHIKEndEffectorDesc_get_bOrientation(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKEndEffectorDesc_set_bOrientation(_this, (char)val);
            }
        }
        public int oriCtlMode {
            get { return SprExport.Spr_PHIKEndEffectorDesc_get_oriCtlMode(_this); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_oriCtlMode(_this, value); }
        }
        public bool bForce {
            get {
                byte ret = (byte)SprExport.Spr_PHIKEndEffectorDesc_get_bForce(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKEndEffectorDesc_set_bForce(_this, (char)val);
            }
        }
        public bool bTorque {
            get {
                byte ret = (byte)SprExport.Spr_PHIKEndEffectorDesc_get_bTorque(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKEndEffectorDesc_set_bTorque(_this, (char)val);
            }
        }
        public double positionPriority {
            get { return SprExport.Spr_PHIKEndEffectorDesc_get_positionPriority(_this); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_positionPriority(_this, value); }
        }
        public double orientationPriority {
            get { return SprExport.Spr_PHIKEndEffectorDesc_get_orientationPriority(_this); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_orientationPriority(_this, value); }
        }
        public double forcePriority {
            get { return SprExport.Spr_PHIKEndEffectorDesc_get_forcePriority(_this); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_forcePriority(_this, value); }
        }
        public double torquePriority {
            get { return SprExport.Spr_PHIKEndEffectorDesc_get_torquePriority(_this); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_torquePriority(_this, value); }
        }
        public Vec3d targetPosition {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetPosition(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetPosition(_this, value); }
        }
        public Vec3d targetVelocity {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetVelocity(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetVelocity(_this, value); }
        }
        public Vec3d targetLocalPosition {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetLocalPosition(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetLocalPosition(_this, value); }
        }
        public Vec3d targetLocalDirection {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetLocalDirection(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetLocalDirection(_this, value); }
        }
        public Quaterniond targetOrientation {
            get { return new Quaterniond(SprExport.Spr_PHIKEndEffectorDesc_addr_targetOrientation(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetOrientation(_this, value); }
        }
        public Vec3d targetDirection {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetDirection(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetDirection(_this, value); }
        }
        public Vec3d targetLookat {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetLookat(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetLookat(_this, value); }
        }
        public Vec3d targetAngVel {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetAngVel(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetAngVel(_this, value); }
        }
        public Vec3d targetForce {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetForce(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetForce(_this, value); }
        }
        public Vec3d targetForceWorkingPoint {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetForceWorkingPoint(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetForceWorkingPoint(_this, value); }
        }
        public Vec3d targetTorque {
            get { return new Vec3d(SprExport.Spr_PHIKEndEffectorDesc_addr_targetTorque(_this)); }
            set { SprExport.Spr_PHIKEndEffectorDesc_set_targetTorque(_this, value); }
        }
        public enum OriCtlMode : int {
            MODE_QUATERNION = 0, MODE_DIRECTION = MODE_QUATERNION + 1, MODE_LOOKAT = MODE_DIRECTION + 1
        }
    }
    [System.Serializable]
    public class PHIKEndEffectorDescStruct : PHIKEndEffectorStateStruct {
        public PHIKEndEffectorDescStruct() {
            PHIKEndEffectorDesc desc = new PHIKEndEffectorDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public bool bPosition;
        public bool bOrientation;
        public int oriCtlMode;
        public bool bForce;
        public bool bTorque;
        public double positionPriority;
        public double orientationPriority;
        public double forcePriority;
        public double torquePriority;
        public Vec3dStruct targetPosition;
        public Vec3dStruct targetVelocity;
        public Vec3dStruct targetLocalPosition;
        public Vec3dStruct targetLocalDirection;
        public QuaterniondStruct targetOrientation;
        public Vec3dStruct targetDirection;
        public Vec3dStruct targetLookat;
        public Vec3dStruct targetAngVel;
        public Vec3dStruct targetForce;
        public Vec3dStruct targetForceWorkingPoint;
        public Vec3dStruct targetTorque;
        public void ApplyTo(PHIKEndEffectorDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.bPosition = bPosition;
            r.bOrientation = bOrientation;
            r.oriCtlMode = oriCtlMode;
            r.bForce = bForce;
            r.bTorque = bTorque;
            r.positionPriority = positionPriority;
            r.orientationPriority = orientationPriority;
            r.forcePriority = forcePriority;
            r.torquePriority = torquePriority;
            r.targetPosition = targetPosition;
            r.targetVelocity = targetVelocity;
            r.targetLocalPosition = targetLocalPosition;
            r.targetLocalDirection = targetLocalDirection;
            r.targetOrientation = targetOrientation;
            r.targetDirection = targetDirection;
            r.targetLookat = targetLookat;
            r.targetAngVel = targetAngVel;
            r.targetForce = targetForce;
            r.targetForceWorkingPoint = targetForceWorkingPoint;
            r.targetTorque = targetTorque;
        }
        public void ApplyFrom(PHIKEndEffectorDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            bPosition = r.bPosition;
            bOrientation = r.bOrientation;
            oriCtlMode = r.oriCtlMode;
            bForce = r.bForce;
            bTorque = r.bTorque;
            positionPriority = r.positionPriority;
            orientationPriority = r.orientationPriority;
            forcePriority = r.forcePriority;
            torquePriority = r.torquePriority;
            targetPosition = r.targetPosition;
            targetVelocity = r.targetVelocity;
            targetLocalPosition = r.targetLocalPosition;
            targetLocalDirection = r.targetLocalDirection;
            targetOrientation = r.targetOrientation;
            targetDirection = r.targetDirection;
            targetLookat = r.targetLookat;
            targetAngVel = r.targetAngVel;
            targetForce = r.targetForce;
            targetForceWorkingPoint = r.targetForceWorkingPoint;
            targetTorque = r.targetTorque;
        }
        public static implicit operator PHIKEndEffectorDesc(PHIKEndEffectorDescStruct m) {
            PHIKEndEffectorDesc r = new PHIKEndEffectorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKEndEffectorDescStruct(PHIKEndEffectorDesc r) {
            PHIKEndEffectorDescStruct m = new PHIKEndEffectorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKActuatorState : CsObject {
        public PHIKActuatorState() { _this = SprExport.Spr_new_PHIKActuatorState(); _flag = true; }
        public PHIKActuatorState(IntPtr ptr) : base(ptr) { }
        public PHIKActuatorState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKActuatorState() { if (_flag) { SprExport.Spr_delete_PHIKActuatorState(_this); _flag = false; } }
        public Posed solidTempPose {
            get { return new Posed(SprExport.Spr_PHIKActuatorState_addr_solidTempPose(_this)); }
            set { SprExport.Spr_PHIKActuatorState_set_solidTempPose(_this, value); }
        }
        public Quaterniond jointTempOri {
            get { return new Quaterniond(SprExport.Spr_PHIKActuatorState_addr_jointTempOri(_this)); }
            set { SprExport.Spr_PHIKActuatorState_set_jointTempOri(_this, value); }
        }
        public Vec3d jointTempOriIntp {
            get { return new Vec3d(SprExport.Spr_PHIKActuatorState_addr_jointTempOriIntp(_this)); }
            set { SprExport.Spr_PHIKActuatorState_set_jointTempOriIntp(_this, value); }
        }
        public double jointTempAngle {
            get { return SprExport.Spr_PHIKActuatorState_get_jointTempAngle(_this); }
            set { SprExport.Spr_PHIKActuatorState_set_jointTempAngle(_this, value); }
        }
        public double jointTempAngleIntp {
            get { return SprExport.Spr_PHIKActuatorState_get_jointTempAngleIntp(_this); }
            set { SprExport.Spr_PHIKActuatorState_set_jointTempAngleIntp(_this, value); }
        }
        public Posed solidPullbackPose {
            get { return new Posed(SprExport.Spr_PHIKActuatorState_addr_solidPullbackPose(_this)); }
            set { SprExport.Spr_PHIKActuatorState_set_solidPullbackPose(_this, value); }
        }
        public Posed jointTempPose {
            get { return new Posed(SprExport.Spr_PHIKActuatorState_addr_jointTempPose(_this)); }
            set { SprExport.Spr_PHIKActuatorState_set_jointTempPose(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKActuatorStateStruct : CsObject {
        public PHIKActuatorStateStruct() {
            PHIKActuatorState desc = new PHIKActuatorState();
            ApplyFrom(desc, false);
        }
        public PosedStruct solidTempPose;
        public QuaterniondStruct jointTempOri;
        public Vec3dStruct jointTempOriIntp;
        public double jointTempAngle;
        public double jointTempAngleIntp;
        public PosedStruct solidPullbackPose;
        public PosedStruct jointTempPose;
        public void ApplyTo(PHIKActuatorState r, bool apply_to_base_class = false) {
            r.solidTempPose = solidTempPose;
            r.jointTempOri = jointTempOri;
            r.jointTempOriIntp = jointTempOriIntp;
            r.jointTempAngle = jointTempAngle;
            r.jointTempAngleIntp = jointTempAngleIntp;
            r.solidPullbackPose = solidPullbackPose;
            r.jointTempPose = jointTempPose;
        }
        public void ApplyFrom(PHIKActuatorState r, bool apply_to_base_class = false) {
            solidTempPose = r.solidTempPose;
            jointTempOri = r.jointTempOri;
            jointTempOriIntp = r.jointTempOriIntp;
            jointTempAngle = r.jointTempAngle;
            jointTempAngleIntp = r.jointTempAngleIntp;
            solidPullbackPose = r.solidPullbackPose;
            jointTempPose = r.jointTempPose;
        }
        public static implicit operator PHIKActuatorState(PHIKActuatorStateStruct m) {
            PHIKActuatorState r = new PHIKActuatorState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKActuatorStateStruct(PHIKActuatorState r) {
            PHIKActuatorStateStruct m = new PHIKActuatorStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKActuatorDesc : PHIKActuatorState {
        public PHIKActuatorDesc() { _this = SprExport.Spr_new_PHIKActuatorDesc(); _flag = true; }
        public PHIKActuatorDesc(IntPtr ptr) : base(ptr) { }
        public PHIKActuatorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKActuatorDesc() { if (_flag) { SprExport.Spr_delete_PHIKActuatorDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHIKActuatorDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHIKActuatorDesc_set_bEnabled(_this, (char)val);
            }
        }
        public float bias {
            get { return SprExport.Spr_PHIKActuatorDesc_get_bias(_this); }
            set { SprExport.Spr_PHIKActuatorDesc_set_bias(_this, value); }
        }
        public double pullbackRate {
            get { return SprExport.Spr_PHIKActuatorDesc_get_pullbackRate(_this); }
            set { SprExport.Spr_PHIKActuatorDesc_set_pullbackRate(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKActuatorDescStruct : PHIKActuatorStateStruct {
        public PHIKActuatorDescStruct() {
            PHIKActuatorDesc desc = new PHIKActuatorDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public float bias;
        public double pullbackRate;
        public void ApplyTo(PHIKActuatorDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.bias = bias;
            r.pullbackRate = pullbackRate;
        }
        public void ApplyFrom(PHIKActuatorDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            bias = r.bias;
            pullbackRate = r.pullbackRate;
        }
        public static implicit operator PHIKActuatorDesc(PHIKActuatorDescStruct m) {
            PHIKActuatorDesc r = new PHIKActuatorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKActuatorDescStruct(PHIKActuatorDesc r) {
            PHIKActuatorDescStruct m = new PHIKActuatorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKBallActuatorDesc : PHIKActuatorDesc {
        public PHIKBallActuatorDesc() { _this = SprExport.Spr_new_PHIKBallActuatorDesc(); _flag = true; }
        public PHIKBallActuatorDesc(IntPtr ptr) : base(ptr) { }
        public PHIKBallActuatorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKBallActuatorDesc() { if (_flag) { SprExport.Spr_delete_PHIKBallActuatorDesc(_this); _flag = false; } }
        public Quaterniond pullbackTarget {
            get { return new Quaterniond(SprExport.Spr_PHIKBallActuatorDesc_addr_pullbackTarget(_this)); }
            set { SprExport.Spr_PHIKBallActuatorDesc_set_pullbackTarget(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKBallActuatorDescStruct : PHIKActuatorDescStruct {
        public PHIKBallActuatorDescStruct() {
            PHIKBallActuatorDesc desc = new PHIKBallActuatorDesc();
            ApplyFrom(desc, true);
        }
        public QuaterniondStruct pullbackTarget;
        public void ApplyTo(PHIKBallActuatorDesc r, bool apply_to_base_class = false) {
            r.pullbackTarget = pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHIKBallActuatorDesc r, bool apply_to_base_class = false) {
            pullbackTarget = r.pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHIKBallActuatorDesc(PHIKBallActuatorDescStruct m) {
            PHIKBallActuatorDesc r = new PHIKBallActuatorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKBallActuatorDescStruct(PHIKBallActuatorDesc r) {
            PHIKBallActuatorDescStruct m = new PHIKBallActuatorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKHingeActuatorDesc : PHIKActuatorDesc {
        public PHIKHingeActuatorDesc() { _this = SprExport.Spr_new_PHIKHingeActuatorDesc(); _flag = true; }
        public PHIKHingeActuatorDesc(IntPtr ptr) : base(ptr) { }
        public PHIKHingeActuatorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKHingeActuatorDesc() { if (_flag) { SprExport.Spr_delete_PHIKHingeActuatorDesc(_this); _flag = false; } }
        public double pullbackTarget {
            get { return SprExport.Spr_PHIKHingeActuatorDesc_get_pullbackTarget(_this); }
            set { SprExport.Spr_PHIKHingeActuatorDesc_set_pullbackTarget(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKHingeActuatorDescStruct : PHIKActuatorDescStruct {
        public PHIKHingeActuatorDescStruct() {
            PHIKHingeActuatorDesc desc = new PHIKHingeActuatorDesc();
            ApplyFrom(desc, true);
        }
        public double pullbackTarget;
        public void ApplyTo(PHIKHingeActuatorDesc r, bool apply_to_base_class = false) {
            r.pullbackTarget = pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHIKHingeActuatorDesc r, bool apply_to_base_class = false) {
            pullbackTarget = r.pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHIKHingeActuatorDesc(PHIKHingeActuatorDescStruct m) {
            PHIKHingeActuatorDesc r = new PHIKHingeActuatorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKHingeActuatorDescStruct(PHIKHingeActuatorDesc r) {
            PHIKHingeActuatorDescStruct m = new PHIKHingeActuatorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHIKSpringActuatorDesc : PHIKActuatorDesc {
        public PHIKSpringActuatorDesc() { _this = SprExport.Spr_new_PHIKSpringActuatorDesc(); _flag = true; }
        public PHIKSpringActuatorDesc(IntPtr ptr) : base(ptr) { }
        public PHIKSpringActuatorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHIKSpringActuatorDesc() { if (_flag) { SprExport.Spr_delete_PHIKSpringActuatorDesc(_this); _flag = false; } }
        public Posed pullbackTarget {
            get { return new Posed(SprExport.Spr_PHIKSpringActuatorDesc_addr_pullbackTarget(_this)); }
            set { SprExport.Spr_PHIKSpringActuatorDesc_set_pullbackTarget(_this, value); }
        }
    }
    [System.Serializable]
    public class PHIKSpringActuatorDescStruct : PHIKActuatorDescStruct {
        public PHIKSpringActuatorDescStruct() {
            PHIKSpringActuatorDesc desc = new PHIKSpringActuatorDesc();
            ApplyFrom(desc, true);
        }
        public PosedStruct pullbackTarget;
        public void ApplyTo(PHIKSpringActuatorDesc r, bool apply_to_base_class = false) {
            r.pullbackTarget = pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHIKSpringActuatorDesc r, bool apply_to_base_class = false) {
            pullbackTarget = r.pullbackTarget;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHIKSpringActuatorDesc(PHIKSpringActuatorDescStruct m) {
            PHIKSpringActuatorDesc r = new PHIKSpringActuatorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHIKSpringActuatorDescStruct(PHIKSpringActuatorDesc r) {
            PHIKSpringActuatorDescStruct m = new PHIKSpringActuatorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHConstraintDesc : CsObject {
        public PHConstraintDesc() { _this = SprExport.Spr_new_PHConstraintDesc(); _flag = true; }
        public PHConstraintDesc(IntPtr ptr) : base(ptr) { }
        public PHConstraintDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHConstraintDesc() { if (_flag) { SprExport.Spr_delete_PHConstraintDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHConstraintDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHConstraintDesc_set_bEnabled(_this, (char)val);
            }
        }
        public Posed poseSocket {
            get { return new Posed(SprExport.Spr_PHConstraintDesc_addr_poseSocket(_this)); }
            set { SprExport.Spr_PHConstraintDesc_set_poseSocket(_this, value); }
        }
        public Posed posePlug {
            get { return new Posed(SprExport.Spr_PHConstraintDesc_addr_posePlug(_this)); }
            set { SprExport.Spr_PHConstraintDesc_set_posePlug(_this, value); }
        }
    }
    [System.Serializable]
    public class PHConstraintDescStruct : CsObject {
        public PHConstraintDescStruct() {
            PHConstraintDesc desc = new PHConstraintDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public PosedStruct poseSocket;
        public PosedStruct posePlug;
        public void ApplyTo(PHConstraintDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.poseSocket = poseSocket;
            r.posePlug = posePlug;
        }
        public void ApplyFrom(PHConstraintDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            poseSocket = r.poseSocket;
            posePlug = r.posePlug;
        }
        public static implicit operator PHConstraintDesc(PHConstraintDescStruct m) {
            PHConstraintDesc r = new PHConstraintDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHConstraintDescStruct(PHConstraintDesc r) {
            PHConstraintDescStruct m = new PHConstraintDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHJointDesc : PHConstraintDesc {
        public PHJointDesc() { _this = SprExport.Spr_new_PHJointDesc(); _flag = true; }
        public PHJointDesc(IntPtr ptr) : base(ptr) { }
        public PHJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHJointDesc() { if (_flag) { SprExport.Spr_delete_PHJointDesc(_this); _flag = false; } }
        public double fMax {
            get { return SprExport.Spr_PHJointDesc_get_fMax(_this); }
            set { SprExport.Spr_PHJointDesc_set_fMax(_this, value); }
        }
    }
    [System.Serializable]
    public class PHJointDescStruct : PHConstraintDescStruct {
        public PHJointDescStruct() {
            PHJointDesc desc = new PHJointDesc();
            ApplyFrom(desc, true);
        }
        public double fMax;
        public void ApplyTo(PHJointDesc r, bool apply_to_base_class = false) {
            r.fMax = fMax;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHJointDesc r, bool apply_to_base_class = false) {
            fMax = r.fMax;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHJointDesc(PHJointDescStruct m) {
            PHJointDesc r = new PHJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHJointDescStruct(PHJointDesc r) {
            PHJointDescStruct m = new PHJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PH1DJointDesc : PHJointDesc {
        public PH1DJointDesc() { _this = SprExport.Spr_new_PH1DJointDesc(); _flag = true; }
        public PH1DJointDesc(IntPtr ptr) : base(ptr) { }
        public PH1DJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PH1DJointDesc() { if (_flag) { SprExport.Spr_delete_PH1DJointDesc(_this); _flag = false; } }
        public bool cyclic {
            get {
                byte ret = (byte)SprExport.Spr_PH1DJointDesc_get_cyclic(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PH1DJointDesc_set_cyclic(_this, (char)val);
            }
        }
        public double spring {
            get { return SprExport.Spr_PH1DJointDesc_get_spring(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_spring(_this, value); }
        }
        public double damper {
            get { return SprExport.Spr_PH1DJointDesc_get_damper(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_damper(_this, value); }
        }
        public double secondDamper {
            get { return SprExport.Spr_PH1DJointDesc_get_secondDamper(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_secondDamper(_this, value); }
        }
        public double targetPosition {
            get { return SprExport.Spr_PH1DJointDesc_get_targetPosition(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_targetPosition(_this, value); }
        }
        public double targetVelocity {
            get { return SprExport.Spr_PH1DJointDesc_get_targetVelocity(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_targetVelocity(_this, value); }
        }
        public double offsetForce {
            get { return SprExport.Spr_PH1DJointDesc_get_offsetForce(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_offsetForce(_this, value); }
        }
        public double yieldStress {
            get { return SprExport.Spr_PH1DJointDesc_get_yieldStress(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_yieldStress(_this, value); }
        }
        public double hardnessRate {
            get { return SprExport.Spr_PH1DJointDesc_get_hardnessRate(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_hardnessRate(_this, value); }
        }
        public double secondMoment {
            get { return SprExport.Spr_PH1DJointDesc_get_secondMoment(_this); }
            set { SprExport.Spr_PH1DJointDesc_set_secondMoment(_this, value); }
        }
    }
    [System.Serializable]
    public class PH1DJointDescStruct : PHJointDescStruct {
        public PH1DJointDescStruct() {
            PH1DJointDesc desc = new PH1DJointDesc();
            ApplyFrom(desc, true);
        }
        public bool cyclic;
        public double spring;
        public double damper;
        public double secondDamper;
        public double targetPosition;
        public double targetVelocity;
        public double offsetForce;
        public double yieldStress;
        public double hardnessRate;
        public double secondMoment;
        public void ApplyTo(PH1DJointDesc r, bool apply_to_base_class = false) {
            r.cyclic = cyclic;
            r.spring = spring;
            r.damper = damper;
            r.secondDamper = secondDamper;
            r.targetPosition = targetPosition;
            r.targetVelocity = targetVelocity;
            r.offsetForce = offsetForce;
            r.yieldStress = yieldStress;
            r.hardnessRate = hardnessRate;
            r.secondMoment = secondMoment;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PH1DJointDesc r, bool apply_to_base_class = false) {
            cyclic = r.cyclic;
            spring = r.spring;
            damper = r.damper;
            secondDamper = r.secondDamper;
            targetPosition = r.targetPosition;
            targetVelocity = r.targetVelocity;
            offsetForce = r.offsetForce;
            yieldStress = r.yieldStress;
            hardnessRate = r.hardnessRate;
            secondMoment = r.secondMoment;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PH1DJointDesc(PH1DJointDescStruct m) {
            PH1DJointDesc r = new PH1DJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PH1DJointDescStruct(PH1DJointDesc r) {
            PH1DJointDescStruct m = new PH1DJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHHingeJointDesc : PH1DJointDesc {
        public PHHingeJointDesc() { _this = SprExport.Spr_new_PHHingeJointDesc(); _flag = true; }
        public PHHingeJointDesc(IntPtr ptr) : base(ptr) { }
        public PHHingeJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHingeJointDesc() { if (_flag) { SprExport.Spr_delete_PHHingeJointDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHHingeJointDescStruct : PH1DJointDescStruct {
        public PHHingeJointDescStruct() {
            PHHingeJointDesc desc = new PHHingeJointDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHHingeJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHHingeJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHHingeJointDesc(PHHingeJointDescStruct m) {
            PHHingeJointDesc r = new PHHingeJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHingeJointDescStruct(PHHingeJointDesc r) {
            PHHingeJointDescStruct m = new PHHingeJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSliderJointDesc : PH1DJointDesc {
        public PHSliderJointDesc() { _this = SprExport.Spr_new_PHSliderJointDesc(); _flag = true; }
        public PHSliderJointDesc(IntPtr ptr) : base(ptr) { }
        public PHSliderJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSliderJointDesc() { if (_flag) { SprExport.Spr_delete_PHSliderJointDesc(_this); _flag = false; } }
        public bool bConstraintY {
            get {
                byte ret = (byte)SprExport.Spr_PHSliderJointDesc_get_bConstraintY(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSliderJointDesc_set_bConstraintY(_this, (char)val);
            }
        }
        public bool bConstraintRollX {
            get {
                byte ret = (byte)SprExport.Spr_PHSliderJointDesc_get_bConstraintRollX(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSliderJointDesc_set_bConstraintRollX(_this, (char)val);
            }
        }
        public bool bConstraintRollZ {
            get {
                byte ret = (byte)SprExport.Spr_PHSliderJointDesc_get_bConstraintRollZ(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSliderJointDesc_set_bConstraintRollZ(_this, (char)val);
            }
        }
    }
    [System.Serializable]
    public class PHSliderJointDescStruct : PH1DJointDescStruct {
        public PHSliderJointDescStruct() {
            PHSliderJointDesc desc = new PHSliderJointDesc();
            ApplyFrom(desc, true);
        }
        public bool bConstraintY;
        public bool bConstraintRollX;
        public bool bConstraintRollZ;
        public void ApplyTo(PHSliderJointDesc r, bool apply_to_base_class = false) {
            r.bConstraintY = bConstraintY;
            r.bConstraintRollX = bConstraintRollX;
            r.bConstraintRollZ = bConstraintRollZ;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHSliderJointDesc r, bool apply_to_base_class = false) {
            bConstraintY = r.bConstraintY;
            bConstraintRollX = r.bConstraintRollX;
            bConstraintRollZ = r.bConstraintRollZ;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHSliderJointDesc(PHSliderJointDescStruct m) {
            PHSliderJointDesc r = new PHSliderJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSliderJointDescStruct(PHSliderJointDesc r) {
            PHSliderJointDescStruct m = new PHSliderJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPathPoint : CsObject {
        public PHPathPoint() { _this = SprExport.Spr_new_PHPathPoint(); _flag = true; }
        public PHPathPoint(IntPtr ptr) : base(ptr) { }
        public PHPathPoint(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public PHPathPoint(double _s, Posed _pose) { _this = SprExport.Spr_new_PHPathPoint_4(_s, _pose); _flag = true; }
        ~PHPathPoint() { if (_flag) { SprExport.Spr_delete_PHPathPoint(_this); _flag = false; } }
        public double s {
            get { return SprExport.Spr_PHPathPoint_get_s(_this); }
            set { SprExport.Spr_PHPathPoint_set_s(_this, value); }
        }
        public Posed pose {
            get { return new Posed(SprExport.Spr_PHPathPoint_addr_pose(_this)); }
            set { SprExport.Spr_PHPathPoint_set_pose(_this, value); }
        }
    }
    public partial class PHPathDesc : CsObject {
        public PHPathDesc() { _this = SprExport.Spr_new_PHPathDesc(); _flag = true; }
        public PHPathDesc(IntPtr ptr) : base(ptr) { }
        public PHPathDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPathDesc() { if (_flag) { SprExport.Spr_delete_PHPathDesc(_this); _flag = false; } }
        public vectorwrapper_PHPathPoint points {
            get { return new vectorwrapper_PHPathPoint(SprExport.Spr_PHPathDesc_addr_points(_this)); }
            set { SprExport.Spr_PHPathDesc_set_points(_this, value); }
        }
        public bool bLoop {
            get {
                byte ret = (byte)SprExport.Spr_PHPathDesc_get_bLoop(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHPathDesc_set_bLoop(_this, (char)val);
            }
        }
    }
    [System.Serializable]
    public class PHPathDescStruct : CsObject {
        public PHPathDescStruct() {
            PHPathDesc desc = new PHPathDesc();
            ApplyFrom(desc, false);
        }
        public vectorwrapper_PHPathPoint points;
        public bool bLoop;
        public void ApplyTo(PHPathDesc r, bool apply_to_base_class = false) {
            r.points = points;
            r.bLoop = bLoop;
        }
        public void ApplyFrom(PHPathDesc r, bool apply_to_base_class = false) {
            points = r.points;
            bLoop = r.bLoop;
        }
        public static implicit operator PHPathDesc(PHPathDescStruct m) {
            PHPathDesc r = new PHPathDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPathDescStruct(PHPathDesc r) {
            PHPathDescStruct m = new PHPathDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPathJointDesc : PH1DJointDesc {
        public PHPathJointDesc() { _this = SprExport.Spr_new_PHPathJointDesc(); _flag = true; }
        public PHPathJointDesc(IntPtr ptr) : base(ptr) { }
        public PHPathJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPathJointDesc() { if (_flag) { SprExport.Spr_delete_PHPathJointDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHPathJointDescStruct : PH1DJointDescStruct {
        public PHPathJointDescStruct() {
            PHPathJointDesc desc = new PHPathJointDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHPathJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPathJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPathJointDesc(PHPathJointDescStruct m) {
            PHPathJointDesc r = new PHPathJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPathJointDescStruct(PHPathJointDesc r) {
            PHPathJointDescStruct m = new PHPathJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHGenericJointCallback : CsObject {
        public PHGenericJointCallback() { _this = SprExport.Spr_new_PHGenericJointCallback(); _flag = true; }
        public PHGenericJointCallback(IntPtr ptr) : base(ptr) { }
        public PHGenericJointCallback(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHGenericJointCallback() { if (_flag) { SprExport.Spr_delete_PHGenericJointCallback(_this); _flag = false; } }
        public void SetParam(PHGenericJointIf jnt, string name, double value) {
            IntPtr remote2 = Marshal.StringToBSTR(name);
            SprExport.Spr_PHGenericJointCallback_SetParam((IntPtr)_this, (IntPtr)jnt, (IntPtr)remote2, (double)value);
            Marshal.FreeBSTR(remote2);
        }
        public bool IsCyclic(PHGenericJointIf jnt) {
            char ret = SprExport.Spr_PHGenericJointCallback_IsCyclic((IntPtr)_this, (IntPtr)jnt);
            return (ret == 0) ? false : true;
        }
        public void GetMovableAxes(PHGenericJointIf jnt, int n, int indices) {
            SprExport.Spr_PHGenericJointCallback_GetMovableAxes((IntPtr)_this, (IntPtr)jnt, (int)n, (int)indices);
        }
        public void CompBias(PHGenericJointIf jnt, Vec3d dbv, Vec3d dbw, Vec3d prel, Quaterniond qrel, Vec3d vrel, Vec3d wrel) {
            SprExport.Spr_PHGenericJointCallback_CompBias((IntPtr)_this, (IntPtr)jnt, (IntPtr)dbv, (IntPtr)dbw, (IntPtr)prel, (IntPtr)qrel, (IntPtr)vrel, (IntPtr)wrel);
        }
        public void CompError(PHGenericJointIf jnt, Vec3d Bv, Vec3d Bw, Vec3d prel, Quaterniond qrel) {
            SprExport.Spr_PHGenericJointCallback_CompError((IntPtr)_this, (IntPtr)jnt, (IntPtr)Bv, (IntPtr)Bw, (IntPtr)prel, (IntPtr)qrel);
        }
        public void UpdateJointState(PHGenericJointIf jnt, double pos, double vel, Vec3d prel, Quaterniond qrel, Vec3d vrel, Vec3d wrel) {
            SprExport.Spr_PHGenericJointCallback_UpdateJointState((IntPtr)_this, (IntPtr)jnt, (double)pos, (double)vel, (IntPtr)prel, (IntPtr)qrel, (IntPtr)vrel, (IntPtr)wrel);
        }
        public void CompJointJacobian(PHGenericJointIf jnt, Vec3d Jv, Vec3d Jw, double pos) {
            SprExport.Spr_PHGenericJointCallback_CompJointJacobian((IntPtr)_this, (IntPtr)jnt, (IntPtr)Jv, (IntPtr)Jw, (double)pos);
        }
        public void CompJointCoriolisAccel(PHGenericJointIf jnt, Vec3d cv, Vec3d cw, double pos, double vel) {
            SprExport.Spr_PHGenericJointCallback_CompJointCoriolisAccel((IntPtr)_this, (IntPtr)jnt, (IntPtr)cv, (IntPtr)cw, (double)pos, (double)vel);
        }
        public void CompRelativePosition(PHGenericJointIf jnt, Vec3d prel, Quaterniond qrel, double pos) {
            SprExport.Spr_PHGenericJointCallback_CompRelativePosition((IntPtr)_this, (IntPtr)jnt, (IntPtr)prel, (IntPtr)qrel, (double)pos);
        }
        public void CompRelativeVelocity(PHGenericJointIf jnt, Vec3d vrel, Vec3d wrel, double pos, double vel) {
            SprExport.Spr_PHGenericJointCallback_CompRelativeVelocity((IntPtr)_this, (IntPtr)jnt, (IntPtr)vrel, (IntPtr)wrel, (double)pos, (double)vel);
        }
    }
    public partial class PHGenericJointDesc : PH1DJointDesc {
        public PHGenericJointDesc() { _this = SprExport.Spr_new_PHGenericJointDesc(); _flag = true; }
        public PHGenericJointDesc(IntPtr ptr) : base(ptr) { }
        public PHGenericJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHGenericJointDesc() { if (_flag) { SprExport.Spr_delete_PHGenericJointDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHGenericJointDescStruct : PH1DJointDescStruct {
        public PHGenericJointDescStruct() {
            PHGenericJointDesc desc = new PHGenericJointDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHGenericJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHGenericJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHGenericJointDesc(PHGenericJointDescStruct m) {
            PHGenericJointDesc r = new PHGenericJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHGenericJointDescStruct(PHGenericJointDesc r) {
            PHGenericJointDescStruct m = new PHGenericJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointDesc : PHJointDesc {
        public PHBallJointDesc() { _this = SprExport.Spr_new_PHBallJointDesc(); _flag = true; }
        public PHBallJointDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointDesc(_this); _flag = false; } }
        public double spring {
            get { return SprExport.Spr_PHBallJointDesc_get_spring(_this); }
            set { SprExport.Spr_PHBallJointDesc_set_spring(_this, value); }
        }
        public double damper {
            get { return SprExport.Spr_PHBallJointDesc_get_damper(_this); }
            set { SprExport.Spr_PHBallJointDesc_set_damper(_this, value); }
        }
        public Vec3d secondDamper {
            get { return new Vec3d(SprExport.Spr_PHBallJointDesc_addr_secondDamper(_this)); }
            set { SprExport.Spr_PHBallJointDesc_set_secondDamper(_this, value); }
        }
        public Quaterniond targetPosition {
            get { return new Quaterniond(SprExport.Spr_PHBallJointDesc_addr_targetPosition(_this)); }
            set { SprExport.Spr_PHBallJointDesc_set_targetPosition(_this, value); }
        }
        public Vec3d targetVelocity {
            get { return new Vec3d(SprExport.Spr_PHBallJointDesc_addr_targetVelocity(_this)); }
            set { SprExport.Spr_PHBallJointDesc_set_targetVelocity(_this, value); }
        }
        public Vec3d offsetForce {
            get { return new Vec3d(SprExport.Spr_PHBallJointDesc_addr_offsetForce(_this)); }
            set { SprExport.Spr_PHBallJointDesc_set_offsetForce(_this, value); }
        }
        public double yieldStress {
            get { return SprExport.Spr_PHBallJointDesc_get_yieldStress(_this); }
            set { SprExport.Spr_PHBallJointDesc_set_yieldStress(_this, value); }
        }
        public double hardnessRate {
            get { return SprExport.Spr_PHBallJointDesc_get_hardnessRate(_this); }
            set { SprExport.Spr_PHBallJointDesc_set_hardnessRate(_this, value); }
        }
        public Vec3d secondMoment {
            get { return new Vec3d(SprExport.Spr_PHBallJointDesc_addr_secondMoment(_this)); }
            set { SprExport.Spr_PHBallJointDesc_set_secondMoment(_this, value); }
        }
    }
    [System.Serializable]
    public class PHBallJointDescStruct : PHJointDescStruct {
        public PHBallJointDescStruct() {
            PHBallJointDesc desc = new PHBallJointDesc();
            ApplyFrom(desc, true);
        }
        public double spring;
        public double damper;
        public Vec3dStruct secondDamper;
        public QuaterniondStruct targetPosition;
        public Vec3dStruct targetVelocity;
        public Vec3dStruct offsetForce;
        public double yieldStress;
        public double hardnessRate;
        public Vec3dStruct secondMoment;
        public void ApplyTo(PHBallJointDesc r, bool apply_to_base_class = false) {
            r.spring = spring;
            r.damper = damper;
            r.secondDamper = secondDamper;
            r.targetPosition = targetPosition;
            r.targetVelocity = targetVelocity;
            r.offsetForce = offsetForce;
            r.yieldStress = yieldStress;
            r.hardnessRate = hardnessRate;
            r.secondMoment = secondMoment;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointDesc r, bool apply_to_base_class = false) {
            spring = r.spring;
            damper = r.damper;
            secondDamper = r.secondDamper;
            targetPosition = r.targetPosition;
            targetVelocity = r.targetVelocity;
            offsetForce = r.offsetForce;
            yieldStress = r.yieldStress;
            hardnessRate = r.hardnessRate;
            secondMoment = r.secondMoment;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointDesc(PHBallJointDescStruct m) {
            PHBallJointDesc r = new PHBallJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointDescStruct(PHBallJointDesc r) {
            PHBallJointDescStruct m = new PHBallJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFixJointDesc : PHJointDesc {
        public PHFixJointDesc() { _this = SprExport.Spr_new_PHFixJointDesc(); _flag = true; }
        public PHFixJointDesc(IntPtr ptr) : base(ptr) { }
        public PHFixJointDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFixJointDesc() { if (_flag) { SprExport.Spr_delete_PHFixJointDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHFixJointDescStruct : PHJointDescStruct {
        public PHFixJointDescStruct() {
            PHFixJointDesc desc = new PHFixJointDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHFixJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFixJointDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFixJointDesc(PHFixJointDescStruct m) {
            PHFixJointDesc r = new PHFixJointDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFixJointDescStruct(PHFixJointDesc r) {
            PHFixJointDescStruct m = new PHFixJointDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSpringDesc : PHJointDesc {
        public PHSpringDesc() { _this = SprExport.Spr_new_PHSpringDesc(); _flag = true; }
        public PHSpringDesc(IntPtr ptr) : base(ptr) { }
        public PHSpringDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSpringDesc() { if (_flag) { SprExport.Spr_delete_PHSpringDesc(_this); _flag = false; } }
        public Vec3d targetPosition {
            get { return new Vec3d(SprExport.Spr_PHSpringDesc_addr_targetPosition(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_targetPosition(_this, value); }
        }
        public Quaterniond targetOrientation {
            get { return new Quaterniond(SprExport.Spr_PHSpringDesc_addr_targetOrientation(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_targetOrientation(_this, value); }
        }
        public Vec3d spring {
            get { return new Vec3d(SprExport.Spr_PHSpringDesc_addr_spring(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_spring(_this, value); }
        }
        public Vec3d damper {
            get { return new Vec3d(SprExport.Spr_PHSpringDesc_addr_damper(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_damper(_this, value); }
        }
        public Vec3d secondDamper {
            get { return new Vec3d(SprExport.Spr_PHSpringDesc_addr_secondDamper(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_secondDamper(_this, value); }
        }
        public double springOri {
            get { return SprExport.Spr_PHSpringDesc_get_springOri(_this); }
            set { SprExport.Spr_PHSpringDesc_set_springOri(_this, value); }
        }
        public double damperOri {
            get { return SprExport.Spr_PHSpringDesc_get_damperOri(_this); }
            set { SprExport.Spr_PHSpringDesc_set_damperOri(_this, value); }
        }
        public double secondDamperOri {
            get { return SprExport.Spr_PHSpringDesc_get_secondDamperOri(_this); }
            set { SprExport.Spr_PHSpringDesc_set_secondDamperOri(_this, value); }
        }
        public double yieldStress {
            get { return SprExport.Spr_PHSpringDesc_get_yieldStress(_this); }
            set { SprExport.Spr_PHSpringDesc_set_yieldStress(_this, value); }
        }
        public double hardnessRate {
            get { return SprExport.Spr_PHSpringDesc_get_hardnessRate(_this); }
            set { SprExport.Spr_PHSpringDesc_set_hardnessRate(_this, value); }
        }
        public Vec3d secondMoment {
            get { return new Vec3d(SprExport.Spr_PHSpringDesc_addr_secondMoment(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_secondMoment(_this, value); }
        }
        public Vec6d targetVelocity {
            get { return new Vec6d(SprExport.Spr_PHSpringDesc_addr_targetVelocity(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_targetVelocity(_this, value); }
        }
        public Vec6d offsetForce {
            get { return new Vec6d(SprExport.Spr_PHSpringDesc_addr_offsetForce(_this)); }
            set { SprExport.Spr_PHSpringDesc_set_offsetForce(_this, value); }
        }
    }
    [System.Serializable]
    public class PHSpringDescStruct : PHJointDescStruct {
        public PHSpringDescStruct() {
            PHSpringDesc desc = new PHSpringDesc();
            ApplyFrom(desc, true);
        }
        public Vec3dStruct targetPosition;
        public QuaterniondStruct targetOrientation;
        public Vec3dStruct spring;
        public Vec3dStruct damper;
        public Vec3dStruct secondDamper;
        public double springOri;
        public double damperOri;
        public double secondDamperOri;
        public double yieldStress;
        public double hardnessRate;
        public Vec3dStruct secondMoment;
        public Vec6dStruct targetVelocity;
        public Vec6dStruct offsetForce;
        public void ApplyTo(PHSpringDesc r, bool apply_to_base_class = false) {
            r.targetPosition = targetPosition;
            r.targetOrientation = targetOrientation;
            r.spring = spring;
            r.damper = damper;
            r.secondDamper = secondDamper;
            r.springOri = springOri;
            r.damperOri = damperOri;
            r.secondDamperOri = secondDamperOri;
            r.yieldStress = yieldStress;
            r.hardnessRate = hardnessRate;
            r.secondMoment = secondMoment;
            r.targetVelocity = targetVelocity;
            r.offsetForce = offsetForce;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHSpringDesc r, bool apply_to_base_class = false) {
            targetPosition = r.targetPosition;
            targetOrientation = r.targetOrientation;
            spring = r.spring;
            damper = r.damper;
            secondDamper = r.secondDamper;
            springOri = r.springOri;
            damperOri = r.damperOri;
            secondDamperOri = r.secondDamperOri;
            yieldStress = r.yieldStress;
            hardnessRate = r.hardnessRate;
            secondMoment = r.secondMoment;
            targetVelocity = r.targetVelocity;
            offsetForce = r.offsetForce;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHSpringDesc(PHSpringDescStruct m) {
            PHSpringDesc r = new PHSpringDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSpringDescStruct(PHSpringDesc r) {
            PHSpringDescStruct m = new PHSpringDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHMateDesc : PHJointDesc {
        public PHMateDesc() { _this = SprExport.Spr_new_PHMateDesc(); _flag = true; }
        public PHMateDesc(IntPtr ptr) : base(ptr) { }
        public PHMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHMateDesc() { if (_flag) { SprExport.Spr_delete_PHMateDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHMateDescStruct : PHJointDescStruct {
        public PHMateDescStruct() {
            PHMateDesc desc = new PHMateDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHMateDesc(PHMateDescStruct m) {
            PHMateDesc r = new PHMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHMateDescStruct(PHMateDesc r) {
            PHMateDescStruct m = new PHMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPointToPointMateDesc : PHMateDesc {
        public PHPointToPointMateDesc() { _this = SprExport.Spr_new_PHPointToPointMateDesc(); _flag = true; }
        public PHPointToPointMateDesc(IntPtr ptr) : base(ptr) { }
        public PHPointToPointMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPointToPointMateDesc() { if (_flag) { SprExport.Spr_delete_PHPointToPointMateDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHPointToPointMateDescStruct : PHMateDescStruct {
        public PHPointToPointMateDescStruct() {
            PHPointToPointMateDesc desc = new PHPointToPointMateDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHPointToPointMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPointToPointMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPointToPointMateDesc(PHPointToPointMateDescStruct m) {
            PHPointToPointMateDesc r = new PHPointToPointMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPointToPointMateDescStruct(PHPointToPointMateDesc r) {
            PHPointToPointMateDescStruct m = new PHPointToPointMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPointToLineMateDesc : PHMateDesc {
        public PHPointToLineMateDesc() { _this = SprExport.Spr_new_PHPointToLineMateDesc(); _flag = true; }
        public PHPointToLineMateDesc(IntPtr ptr) : base(ptr) { }
        public PHPointToLineMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPointToLineMateDesc() { if (_flag) { SprExport.Spr_delete_PHPointToLineMateDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHPointToLineMateDescStruct : PHMateDescStruct {
        public PHPointToLineMateDescStruct() {
            PHPointToLineMateDesc desc = new PHPointToLineMateDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHPointToLineMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPointToLineMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPointToLineMateDesc(PHPointToLineMateDescStruct m) {
            PHPointToLineMateDesc r = new PHPointToLineMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPointToLineMateDescStruct(PHPointToLineMateDesc r) {
            PHPointToLineMateDescStruct m = new PHPointToLineMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPointToPlaneMateDesc : PHMateDesc {
        public PHPointToPlaneMateDesc() { _this = SprExport.Spr_new_PHPointToPlaneMateDesc(); _flag = true; }
        public PHPointToPlaneMateDesc(IntPtr ptr) : base(ptr) { }
        public PHPointToPlaneMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPointToPlaneMateDesc() { if (_flag) { SprExport.Spr_delete_PHPointToPlaneMateDesc(_this); _flag = false; } }
        public Vec2d range {
            get { return new Vec2d(SprExport.Spr_PHPointToPlaneMateDesc_addr_range(_this)); }
            set { SprExport.Spr_PHPointToPlaneMateDesc_set_range(_this, value); }
        }
    }
    [System.Serializable]
    public class PHPointToPlaneMateDescStruct : PHMateDescStruct {
        public PHPointToPlaneMateDescStruct() {
            PHPointToPlaneMateDesc desc = new PHPointToPlaneMateDesc();
            ApplyFrom(desc, true);
        }
        public Vec2dStruct range;
        public void ApplyTo(PHPointToPlaneMateDesc r, bool apply_to_base_class = false) {
            r.range = range;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPointToPlaneMateDesc r, bool apply_to_base_class = false) {
            range = r.range;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPointToPlaneMateDesc(PHPointToPlaneMateDescStruct m) {
            PHPointToPlaneMateDesc r = new PHPointToPlaneMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPointToPlaneMateDescStruct(PHPointToPlaneMateDesc r) {
            PHPointToPlaneMateDescStruct m = new PHPointToPlaneMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHLineToLineMateDesc : PHMateDesc {
        public PHLineToLineMateDesc() { _this = SprExport.Spr_new_PHLineToLineMateDesc(); _flag = true; }
        public PHLineToLineMateDesc(IntPtr ptr) : base(ptr) { }
        public PHLineToLineMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHLineToLineMateDesc() { if (_flag) { SprExport.Spr_delete_PHLineToLineMateDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHLineToLineMateDescStruct : PHMateDescStruct {
        public PHLineToLineMateDescStruct() {
            PHLineToLineMateDesc desc = new PHLineToLineMateDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHLineToLineMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHLineToLineMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHLineToLineMateDesc(PHLineToLineMateDescStruct m) {
            PHLineToLineMateDesc r = new PHLineToLineMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHLineToLineMateDescStruct(PHLineToLineMateDesc r) {
            PHLineToLineMateDescStruct m = new PHLineToLineMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPlaneToPlaneMateDesc : PHMateDesc {
        public PHPlaneToPlaneMateDesc() { _this = SprExport.Spr_new_PHPlaneToPlaneMateDesc(); _flag = true; }
        public PHPlaneToPlaneMateDesc(IntPtr ptr) : base(ptr) { }
        public PHPlaneToPlaneMateDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPlaneToPlaneMateDesc() { if (_flag) { SprExport.Spr_delete_PHPlaneToPlaneMateDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHPlaneToPlaneMateDescStruct : PHMateDescStruct {
        public PHPlaneToPlaneMateDescStruct() {
            PHPlaneToPlaneMateDesc desc = new PHPlaneToPlaneMateDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHPlaneToPlaneMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPlaneToPlaneMateDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPlaneToPlaneMateDesc(PHPlaneToPlaneMateDescStruct m) {
            PHPlaneToPlaneMateDesc r = new PHPlaneToPlaneMateDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPlaneToPlaneMateDescStruct(PHPlaneToPlaneMateDesc r) {
            PHPlaneToPlaneMateDescStruct m = new PHPlaneToPlaneMateDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHTreeNodeDesc : CsObject {
        public PHTreeNodeDesc() { _this = SprExport.Spr_new_PHTreeNodeDesc(); _flag = true; }
        public PHTreeNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHTreeNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHTreeNodeDesc() { if (_flag) { SprExport.Spr_delete_PHTreeNodeDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHTreeNodeDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHTreeNodeDesc_set_bEnabled(_this, (char)val);
            }
        }
    }
    [System.Serializable]
    public class PHTreeNodeDescStruct : CsObject {
        public PHTreeNodeDescStruct() {
            PHTreeNodeDesc desc = new PHTreeNodeDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public void ApplyTo(PHTreeNodeDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
        }
        public void ApplyFrom(PHTreeNodeDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
        }
        public static implicit operator PHTreeNodeDesc(PHTreeNodeDescStruct m) {
            PHTreeNodeDesc r = new PHTreeNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHTreeNodeDescStruct(PHTreeNodeDesc r) {
            PHTreeNodeDescStruct m = new PHTreeNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHRootNodeDesc : PHTreeNodeDesc {
        public PHRootNodeDesc() { _this = SprExport.Spr_new_PHRootNodeDesc(); _flag = true; }
        public PHRootNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHRootNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHRootNodeDesc() { if (_flag) { SprExport.Spr_delete_PHRootNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHRootNodeDescStruct : PHTreeNodeDescStruct {
        public PHRootNodeDescStruct() {
            PHRootNodeDesc desc = new PHRootNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHRootNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHRootNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHRootNodeDesc(PHRootNodeDescStruct m) {
            PHRootNodeDesc r = new PHRootNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHRootNodeDescStruct(PHRootNodeDesc r) {
            PHRootNodeDescStruct m = new PHRootNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHTreeNode1DDesc : PHTreeNodeDesc {
        public PHTreeNode1DDesc() { _this = SprExport.Spr_new_PHTreeNode1DDesc(); _flag = true; }
        public PHTreeNode1DDesc(IntPtr ptr) : base(ptr) { }
        public PHTreeNode1DDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHTreeNode1DDesc() { if (_flag) { SprExport.Spr_delete_PHTreeNode1DDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHTreeNode1DDescStruct : PHTreeNodeDescStruct {
        public PHTreeNode1DDescStruct() {
            PHTreeNode1DDesc desc = new PHTreeNode1DDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHTreeNode1DDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHTreeNode1DDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHTreeNode1DDesc(PHTreeNode1DDescStruct m) {
            PHTreeNode1DDesc r = new PHTreeNode1DDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHTreeNode1DDescStruct(PHTreeNode1DDesc r) {
            PHTreeNode1DDescStruct m = new PHTreeNode1DDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHHingeJointNodeDesc : PHTreeNode1DDesc {
        public PHHingeJointNodeDesc() { _this = SprExport.Spr_new_PHHingeJointNodeDesc(); _flag = true; }
        public PHHingeJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHHingeJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHingeJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHHingeJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHHingeJointNodeDescStruct : PHTreeNode1DDescStruct {
        public PHHingeJointNodeDescStruct() {
            PHHingeJointNodeDesc desc = new PHHingeJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHHingeJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHHingeJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHHingeJointNodeDesc(PHHingeJointNodeDescStruct m) {
            PHHingeJointNodeDesc r = new PHHingeJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHingeJointNodeDescStruct(PHHingeJointNodeDesc r) {
            PHHingeJointNodeDescStruct m = new PHHingeJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSliderJointNodeDesc : PHTreeNode1DDesc {
        public PHSliderJointNodeDesc() { _this = SprExport.Spr_new_PHSliderJointNodeDesc(); _flag = true; }
        public PHSliderJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHSliderJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSliderJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHSliderJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHSliderJointNodeDescStruct : PHTreeNode1DDescStruct {
        public PHSliderJointNodeDescStruct() {
            PHSliderJointNodeDesc desc = new PHSliderJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHSliderJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHSliderJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHSliderJointNodeDesc(PHSliderJointNodeDescStruct m) {
            PHSliderJointNodeDesc r = new PHSliderJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSliderJointNodeDescStruct(PHSliderJointNodeDesc r) {
            PHSliderJointNodeDescStruct m = new PHSliderJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHPathJointNodeDesc : PHTreeNode1DDesc {
        public PHPathJointNodeDesc() { _this = SprExport.Spr_new_PHPathJointNodeDesc(); _flag = true; }
        public PHPathJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHPathJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHPathJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHPathJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHPathJointNodeDescStruct : PHTreeNode1DDescStruct {
        public PHPathJointNodeDescStruct() {
            PHPathJointNodeDesc desc = new PHPathJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHPathJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHPathJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHPathJointNodeDesc(PHPathJointNodeDescStruct m) {
            PHPathJointNodeDesc r = new PHPathJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHPathJointNodeDescStruct(PHPathJointNodeDesc r) {
            PHPathJointNodeDescStruct m = new PHPathJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHGenericJointNodeDesc : PHTreeNode1DDesc {
        public PHGenericJointNodeDesc() { _this = SprExport.Spr_new_PHGenericJointNodeDesc(); _flag = true; }
        public PHGenericJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHGenericJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHGenericJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHGenericJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHGenericJointNodeDescStruct : PHTreeNode1DDescStruct {
        public PHGenericJointNodeDescStruct() {
            PHGenericJointNodeDesc desc = new PHGenericJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHGenericJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHGenericJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHGenericJointNodeDesc(PHGenericJointNodeDescStruct m) {
            PHGenericJointNodeDesc r = new PHGenericJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHGenericJointNodeDescStruct(PHGenericJointNodeDesc r) {
            PHGenericJointNodeDescStruct m = new PHGenericJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointNodeDesc : PHTreeNodeDesc {
        public PHBallJointNodeDesc() { _this = SprExport.Spr_new_PHBallJointNodeDesc(); _flag = true; }
        public PHBallJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHBallJointNodeDescStruct : PHTreeNodeDescStruct {
        public PHBallJointNodeDescStruct() {
            PHBallJointNodeDesc desc = new PHBallJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHBallJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointNodeDesc(PHBallJointNodeDescStruct m) {
            PHBallJointNodeDesc r = new PHBallJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointNodeDescStruct(PHBallJointNodeDesc r) {
            PHBallJointNodeDescStruct m = new PHBallJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHFixJointNodeDesc : PHTreeNodeDesc {
        public PHFixJointNodeDesc() { _this = SprExport.Spr_new_PHFixJointNodeDesc(); _flag = true; }
        public PHFixJointNodeDesc(IntPtr ptr) : base(ptr) { }
        public PHFixJointNodeDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHFixJointNodeDesc() { if (_flag) { SprExport.Spr_delete_PHFixJointNodeDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHFixJointNodeDescStruct : PHTreeNodeDescStruct {
        public PHFixJointNodeDescStruct() {
            PHFixJointNodeDesc desc = new PHFixJointNodeDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHFixJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHFixJointNodeDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHFixJointNodeDesc(PHFixJointNodeDescStruct m) {
            PHFixJointNodeDesc r = new PHFixJointNodeDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHFixJointNodeDescStruct(PHFixJointNodeDesc r) {
            PHFixJointNodeDescStruct m = new PHFixJointNodeDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHGearDesc : CsObject {
        public PHGearDesc() { _this = SprExport.Spr_new_PHGearDesc(); _flag = true; }
        public PHGearDesc(IntPtr ptr) : base(ptr) { }
        public PHGearDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHGearDesc() { if (_flag) { SprExport.Spr_delete_PHGearDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHGearDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHGearDesc_set_bEnabled(_this, (char)val);
            }
        }
        public double ratio {
            get { return SprExport.Spr_PHGearDesc_get_ratio(_this); }
            set { SprExport.Spr_PHGearDesc_set_ratio(_this, value); }
        }
        public double offset {
            get { return SprExport.Spr_PHGearDesc_get_offset(_this); }
            set { SprExport.Spr_PHGearDesc_set_offset(_this, value); }
        }
        public int mode {
            get { return SprExport.Spr_PHGearDesc_get_mode(_this); }
            set { SprExport.Spr_PHGearDesc_set_mode(_this, value); }
        }
    }
    [System.Serializable]
    public class PHGearDescStruct : CsObject {
        public PHGearDescStruct() {
            PHGearDesc desc = new PHGearDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public double ratio;
        public double offset;
        public int mode;
        public void ApplyTo(PHGearDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.ratio = ratio;
            r.offset = offset;
            r.mode = mode;
        }
        public void ApplyFrom(PHGearDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            ratio = r.ratio;
            offset = r.offset;
            mode = r.mode;
        }
        public static implicit operator PHGearDesc(PHGearDescStruct m) {
            PHGearDesc r = new PHGearDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHGearDescStruct(PHGearDesc r) {
            PHGearDescStruct m = new PHGearDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PH1DJointLimitDesc : CsObject {
        public PH1DJointLimitDesc() { _this = SprExport.Spr_new_PH1DJointLimitDesc(); _flag = true; }
        public PH1DJointLimitDesc(IntPtr ptr) : base(ptr) { }
        public PH1DJointLimitDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PH1DJointLimitDesc() { if (_flag) { SprExport.Spr_delete_PH1DJointLimitDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PH1DJointLimitDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PH1DJointLimitDesc_set_bEnabled(_this, (char)val);
            }
        }
        public double spring {
            get { return SprExport.Spr_PH1DJointLimitDesc_get_spring(_this); }
            set { SprExport.Spr_PH1DJointLimitDesc_set_spring(_this, value); }
        }
        public double damper {
            get { return SprExport.Spr_PH1DJointLimitDesc_get_damper(_this); }
            set { SprExport.Spr_PH1DJointLimitDesc_set_damper(_this, value); }
        }
        public Vec2d range {
            get { return new Vec2d(SprExport.Spr_PH1DJointLimitDesc_addr_range(_this)); }
            set { SprExport.Spr_PH1DJointLimitDesc_set_range(_this, value); }
        }
    }
    [System.Serializable]
    public class PH1DJointLimitDescStruct : CsObject {
        public PH1DJointLimitDescStruct() {
            PH1DJointLimitDesc desc = new PH1DJointLimitDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public double spring;
        public double damper;
        public Vec2dStruct range;
        public void ApplyTo(PH1DJointLimitDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.spring = spring;
            r.damper = damper;
            r.range = range;
        }
        public void ApplyFrom(PH1DJointLimitDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            spring = r.spring;
            damper = r.damper;
            range = r.range;
        }
        public static implicit operator PH1DJointLimitDesc(PH1DJointLimitDescStruct m) {
            PH1DJointLimitDesc r = new PH1DJointLimitDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PH1DJointLimitDescStruct(PH1DJointLimitDesc r) {
            PH1DJointLimitDescStruct m = new PH1DJointLimitDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointLimitDesc : CsObject {
        public PHBallJointLimitDesc() { _this = SprExport.Spr_new_PHBallJointLimitDesc(); _flag = true; }
        public PHBallJointLimitDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointLimitDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointLimitDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointLimitDesc(_this); _flag = false; } }
        public bool bEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHBallJointLimitDesc_get_bEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHBallJointLimitDesc_set_bEnabled(_this, (char)val);
            }
        }
        public double spring {
            get { return SprExport.Spr_PHBallJointLimitDesc_get_spring(_this); }
            set { SprExport.Spr_PHBallJointLimitDesc_set_spring(_this, value); }
        }
        public double damper {
            get { return SprExport.Spr_PHBallJointLimitDesc_get_damper(_this); }
            set { SprExport.Spr_PHBallJointLimitDesc_set_damper(_this, value); }
        }
        public Vec3d limitDir {
            get { return new Vec3d(SprExport.Spr_PHBallJointLimitDesc_addr_limitDir(_this)); }
            set { SprExport.Spr_PHBallJointLimitDesc_set_limitDir(_this, value); }
        }
    }
    [System.Serializable]
    public class PHBallJointLimitDescStruct : CsObject {
        public PHBallJointLimitDescStruct() {
            PHBallJointLimitDesc desc = new PHBallJointLimitDesc();
            ApplyFrom(desc, false);
        }
        public bool bEnabled;
        public double spring;
        public double damper;
        public Vec3dStruct limitDir;
        public void ApplyTo(PHBallJointLimitDesc r, bool apply_to_base_class = false) {
            r.bEnabled = bEnabled;
            r.spring = spring;
            r.damper = damper;
            r.limitDir = limitDir;
        }
        public void ApplyFrom(PHBallJointLimitDesc r, bool apply_to_base_class = false) {
            bEnabled = r.bEnabled;
            spring = r.spring;
            damper = r.damper;
            limitDir = r.limitDir;
        }
        public static implicit operator PHBallJointLimitDesc(PHBallJointLimitDescStruct m) {
            PHBallJointLimitDesc r = new PHBallJointLimitDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointLimitDescStruct(PHBallJointLimitDesc r) {
            PHBallJointLimitDescStruct m = new PHBallJointLimitDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointConeLimitDesc : PHBallJointLimitDesc {
        public PHBallJointConeLimitDesc() { _this = SprExport.Spr_new_PHBallJointConeLimitDesc(); _flag = true; }
        public PHBallJointConeLimitDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointConeLimitDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointConeLimitDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointConeLimitDesc(_this); _flag = false; } }
        public Vec2d limitSwing {
            get { return new Vec2d(SprExport.Spr_PHBallJointConeLimitDesc_addr_limitSwing(_this)); }
            set { SprExport.Spr_PHBallJointConeLimitDesc_set_limitSwing(_this, value); }
        }
        public Vec2d limitSwingDir {
            get { return new Vec2d(SprExport.Spr_PHBallJointConeLimitDesc_addr_limitSwingDir(_this)); }
            set { SprExport.Spr_PHBallJointConeLimitDesc_set_limitSwingDir(_this, value); }
        }
        public Vec2d limitTwist {
            get { return new Vec2d(SprExport.Spr_PHBallJointConeLimitDesc_addr_limitTwist(_this)); }
            set { SprExport.Spr_PHBallJointConeLimitDesc_set_limitTwist(_this, value); }
        }
    }
    [System.Serializable]
    public class PHBallJointConeLimitDescStruct : PHBallJointLimitDescStruct {
        public PHBallJointConeLimitDescStruct() {
            PHBallJointConeLimitDesc desc = new PHBallJointConeLimitDesc();
            ApplyFrom(desc, true);
        }
        public Vec2dStruct limitSwing;
        public Vec2dStruct limitSwingDir;
        public Vec2dStruct limitTwist;
        public void ApplyTo(PHBallJointConeLimitDesc r, bool apply_to_base_class = false) {
            r.limitSwing = limitSwing;
            r.limitSwingDir = limitSwingDir;
            r.limitTwist = limitTwist;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointConeLimitDesc r, bool apply_to_base_class = false) {
            limitSwing = r.limitSwing;
            limitSwingDir = r.limitSwingDir;
            limitTwist = r.limitTwist;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointConeLimitDesc(PHBallJointConeLimitDescStruct m) {
            PHBallJointConeLimitDesc r = new PHBallJointConeLimitDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointConeLimitDescStruct(PHBallJointConeLimitDesc r) {
            PHBallJointConeLimitDescStruct m = new PHBallJointConeLimitDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSplineLimitNode : CsObject {
        public PHSplineLimitNode() { _this = SprExport.Spr_new_PHSplineLimitNode(); _flag = true; }
        public PHSplineLimitNode(IntPtr ptr) : base(ptr) { }
        public PHSplineLimitNode(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public PHSplineLimitNode(double s, double sd, double ds, double dsd, double tmn, double tmx) { _this = SprExport.Spr_new_PHSplineLimitNode_8(s, sd, ds, dsd, tmn, tmx); _flag = true; }
        ~PHSplineLimitNode() { if (_flag) { SprExport.Spr_delete_PHSplineLimitNode(_this); _flag = false; } }
        public double swing {
            get { return SprExport.Spr_PHSplineLimitNode_get_swing(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_swing(_this, value); }
        }
        public double swingDir {
            get { return SprExport.Spr_PHSplineLimitNode_get_swingDir(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_swingDir(_this, value); }
        }
        public double dSwing {
            get { return SprExport.Spr_PHSplineLimitNode_get_dSwing(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_dSwing(_this, value); }
        }
        public double dSwingDir {
            get { return SprExport.Spr_PHSplineLimitNode_get_dSwingDir(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_dSwingDir(_this, value); }
        }
        public double twistMin {
            get { return SprExport.Spr_PHSplineLimitNode_get_twistMin(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_twistMin(_this, value); }
        }
        public double twistMax {
            get { return SprExport.Spr_PHSplineLimitNode_get_twistMax(_this); }
            set { SprExport.Spr_PHSplineLimitNode_set_twistMax(_this, value); }
        }
    }
    public partial class PHBallJointSplineLimitDesc : PHBallJointLimitDesc {
        public PHBallJointSplineLimitDesc() { _this = SprExport.Spr_new_PHBallJointSplineLimitDesc(); _flag = true; }
        public PHBallJointSplineLimitDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointSplineLimitDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointSplineLimitDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointSplineLimitDesc(_this); _flag = false; } }
        public vectorwrapper_PHSplineLimitNode nodes {
            get { return new vectorwrapper_PHSplineLimitNode(SprExport.Spr_PHBallJointSplineLimitDesc_addr_nodes(_this)); }
            set { SprExport.Spr_PHBallJointSplineLimitDesc_set_nodes(_this, value); }
        }
        public Vec2d poleTwist {
            get { return new Vec2d(SprExport.Spr_PHBallJointSplineLimitDesc_addr_poleTwist(_this)); }
            set { SprExport.Spr_PHBallJointSplineLimitDesc_set_poleTwist(_this, value); }
        }
    }
    [System.Serializable]
    public class PHBallJointSplineLimitDescStruct : PHBallJointLimitDescStruct {
        public PHBallJointSplineLimitDescStruct() {
            PHBallJointSplineLimitDesc desc = new PHBallJointSplineLimitDesc();
            ApplyFrom(desc, true);
        }
        public vectorwrapper_PHSplineLimitNode nodes;
        public Vec2dStruct poleTwist;
        public void ApplyTo(PHBallJointSplineLimitDesc r, bool apply_to_base_class = false) {
            r.nodes = nodes;
            r.poleTwist = poleTwist;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointSplineLimitDesc r, bool apply_to_base_class = false) {
            nodes = r.nodes;
            poleTwist = r.poleTwist;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointSplineLimitDesc(PHBallJointSplineLimitDescStruct m) {
            PHBallJointSplineLimitDesc r = new PHBallJointSplineLimitDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointSplineLimitDescStruct(PHBallJointSplineLimitDesc r) {
            PHBallJointSplineLimitDescStruct m = new PHBallJointSplineLimitDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointIndependentLimitDesc : PHBallJointLimitDesc {
        public PHBallJointIndependentLimitDesc() { _this = SprExport.Spr_new_PHBallJointIndependentLimitDesc(); _flag = true; }
        public PHBallJointIndependentLimitDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointIndependentLimitDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointIndependentLimitDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointIndependentLimitDesc(_this); _flag = false; } }
        public Vec2d limitX {
            get { return new Vec2d(SprExport.Spr_PHBallJointIndependentLimitDesc_addr_limitX(_this)); }
            set { SprExport.Spr_PHBallJointIndependentLimitDesc_set_limitX(_this, value); }
        }
        public Vec2d limitY {
            get { return new Vec2d(SprExport.Spr_PHBallJointIndependentLimitDesc_addr_limitY(_this)); }
            set { SprExport.Spr_PHBallJointIndependentLimitDesc_set_limitY(_this, value); }
        }
        public Vec2d limitZ {
            get { return new Vec2d(SprExport.Spr_PHBallJointIndependentLimitDesc_addr_limitZ(_this)); }
            set { SprExport.Spr_PHBallJointIndependentLimitDesc_set_limitZ(_this, value); }
        }
    }
    [System.Serializable]
    public class PHBallJointIndependentLimitDescStruct : PHBallJointLimitDescStruct {
        public PHBallJointIndependentLimitDescStruct() {
            PHBallJointIndependentLimitDesc desc = new PHBallJointIndependentLimitDesc();
            ApplyFrom(desc, true);
        }
        public Vec2dStruct limitX;
        public Vec2dStruct limitY;
        public Vec2dStruct limitZ;
        public void ApplyTo(PHBallJointIndependentLimitDesc r, bool apply_to_base_class = false) {
            r.limitX = limitX;
            r.limitY = limitY;
            r.limitZ = limitZ;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointIndependentLimitDesc r, bool apply_to_base_class = false) {
            limitX = r.limitX;
            limitY = r.limitY;
            limitZ = r.limitZ;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointIndependentLimitDesc(PHBallJointIndependentLimitDescStruct m) {
            PHBallJointIndependentLimitDesc r = new PHBallJointIndependentLimitDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointIndependentLimitDescStruct(PHBallJointIndependentLimitDesc r) {
            PHBallJointIndependentLimitDescStruct m = new PHBallJointIndependentLimitDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PH1DJointMotorDesc : CsObject {
        public PH1DJointMotorDesc() { _this = SprExport.Spr_new_PH1DJointMotorDesc(); _flag = true; }
        public PH1DJointMotorDesc(IntPtr ptr) : base(ptr) { }
        public PH1DJointMotorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PH1DJointMotorDesc() { if (_flag) { SprExport.Spr_delete_PH1DJointMotorDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PH1DJointMotorDescStruct : CsObject {
        public PH1DJointMotorDescStruct() {
            PH1DJointMotorDesc desc = new PH1DJointMotorDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PH1DJointMotorDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PH1DJointMotorDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PH1DJointMotorDesc(PH1DJointMotorDescStruct m) {
            PH1DJointMotorDesc r = new PH1DJointMotorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PH1DJointMotorDescStruct(PH1DJointMotorDesc r) {
            PH1DJointMotorDescStruct m = new PH1DJointMotorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PH1DJointNonLinearMotorDesc : PH1DJointMotorDesc {
        public PH1DJointNonLinearMotorDesc() { _this = SprExport.Spr_new_PH1DJointNonLinearMotorDesc(); _flag = true; }
        public PH1DJointNonLinearMotorDesc(IntPtr ptr) : base(ptr) { }
        public PH1DJointNonLinearMotorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PH1DJointNonLinearMotorDesc() { if (_flag) { SprExport.Spr_delete_PH1DJointNonLinearMotorDesc(_this); _flag = false; } }
        public enum FunctionMode : int {
            LINEAR = 0, HUMANJOINTRESITANCE = LINEAR + 1
        }
    }
    [System.Serializable]
    public class PH1DJointNonLinearMotorDescStruct : PH1DJointMotorDescStruct {
        public PH1DJointNonLinearMotorDescStruct() {
            PH1DJointNonLinearMotorDesc desc = new PH1DJointNonLinearMotorDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PH1DJointNonLinearMotorDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PH1DJointNonLinearMotorDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PH1DJointNonLinearMotorDesc(PH1DJointNonLinearMotorDescStruct m) {
            PH1DJointNonLinearMotorDesc r = new PH1DJointNonLinearMotorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PH1DJointNonLinearMotorDescStruct(PH1DJointNonLinearMotorDesc r) {
            PH1DJointNonLinearMotorDescStruct m = new PH1DJointNonLinearMotorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHHuman1DJointResistanceDesc : PH1DJointNonLinearMotorDesc {
        public PHHuman1DJointResistanceDesc() { _this = SprExport.Spr_new_PHHuman1DJointResistanceDesc(); _flag = true; }
        public PHHuman1DJointResistanceDesc(IntPtr ptr) : base(ptr) { }
        public PHHuman1DJointResistanceDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHuman1DJointResistanceDesc() { if (_flag) { SprExport.Spr_delete_PHHuman1DJointResistanceDesc(_this); _flag = false; } }
        public Vec4d coefficient {
            get { return new Vec4d(SprExport.Spr_PHHuman1DJointResistanceDesc_addr_coefficient(_this)); }
            set { SprExport.Spr_PHHuman1DJointResistanceDesc_set_coefficient(_this, value); }
        }
    }
    [System.Serializable]
    public class PHHuman1DJointResistanceDescStruct : PH1DJointNonLinearMotorDescStruct {
        public PHHuman1DJointResistanceDescStruct() {
            PHHuman1DJointResistanceDesc desc = new PHHuman1DJointResistanceDesc();
            ApplyFrom(desc, true);
        }
        public Vec4dStruct coefficient;
        public void ApplyTo(PHHuman1DJointResistanceDesc r, bool apply_to_base_class = false) {
            r.coefficient = coefficient;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHHuman1DJointResistanceDesc r, bool apply_to_base_class = false) {
            coefficient = r.coefficient;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHHuman1DJointResistanceDesc(PHHuman1DJointResistanceDescStruct m) {
            PHHuman1DJointResistanceDesc r = new PHHuman1DJointResistanceDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHuman1DJointResistanceDescStruct(PHHuman1DJointResistanceDesc r) {
            PHHuman1DJointResistanceDescStruct m = new PHHuman1DJointResistanceDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointMotorDesc : CsObject {
        public PHBallJointMotorDesc() { _this = SprExport.Spr_new_PHBallJointMotorDesc(); _flag = true; }
        public PHBallJointMotorDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointMotorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointMotorDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointMotorDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHBallJointMotorDescStruct : CsObject {
        public PHBallJointMotorDescStruct() {
            PHBallJointMotorDesc desc = new PHBallJointMotorDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHBallJointMotorDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHBallJointMotorDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHBallJointMotorDesc(PHBallJointMotorDescStruct m) {
            PHBallJointMotorDesc r = new PHBallJointMotorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointMotorDescStruct(PHBallJointMotorDesc r) {
            PHBallJointMotorDescStruct m = new PHBallJointMotorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBallJointNonLinearMotorDesc : PHBallJointMotorDesc {
        public PHBallJointNonLinearMotorDesc() { _this = SprExport.Spr_new_PHBallJointNonLinearMotorDesc(); _flag = true; }
        public PHBallJointNonLinearMotorDesc(IntPtr ptr) : base(ptr) { }
        public PHBallJointNonLinearMotorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBallJointNonLinearMotorDesc() { if (_flag) { SprExport.Spr_delete_PHBallJointNonLinearMotorDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHBallJointNonLinearMotorDescStruct : PHBallJointMotorDescStruct {
        public PHBallJointNonLinearMotorDescStruct() {
            PHBallJointNonLinearMotorDesc desc = new PHBallJointNonLinearMotorDesc();
            ApplyFrom(desc, true);
        }
        public void ApplyTo(PHBallJointNonLinearMotorDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHBallJointNonLinearMotorDesc r, bool apply_to_base_class = false) {
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHBallJointNonLinearMotorDesc(PHBallJointNonLinearMotorDescStruct m) {
            PHBallJointNonLinearMotorDesc r = new PHBallJointNonLinearMotorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBallJointNonLinearMotorDescStruct(PHBallJointNonLinearMotorDesc r) {
            PHBallJointNonLinearMotorDescStruct m = new PHBallJointNonLinearMotorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHHumanBallJointResistanceDesc : PHBallJointNonLinearMotorDesc {
        public PHHumanBallJointResistanceDesc() { _this = SprExport.Spr_new_PHHumanBallJointResistanceDesc(); _flag = true; }
        public PHHumanBallJointResistanceDesc(IntPtr ptr) : base(ptr) { }
        public PHHumanBallJointResistanceDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHHumanBallJointResistanceDesc() { if (_flag) { SprExport.Spr_delete_PHHumanBallJointResistanceDesc(_this); _flag = false; } }
        public Vec4d xCoefficient {
            get { return new Vec4d(SprExport.Spr_PHHumanBallJointResistanceDesc_addr_xCoefficient(_this)); }
            set { SprExport.Spr_PHHumanBallJointResistanceDesc_set_xCoefficient(_this, value); }
        }
        public Vec4d yCoefficient {
            get { return new Vec4d(SprExport.Spr_PHHumanBallJointResistanceDesc_addr_yCoefficient(_this)); }
            set { SprExport.Spr_PHHumanBallJointResistanceDesc_set_yCoefficient(_this, value); }
        }
        public Vec4d zCoefficient {
            get { return new Vec4d(SprExport.Spr_PHHumanBallJointResistanceDesc_addr_zCoefficient(_this)); }
            set { SprExport.Spr_PHHumanBallJointResistanceDesc_set_zCoefficient(_this, value); }
        }
    }
    [System.Serializable]
    public class PHHumanBallJointResistanceDescStruct : PHBallJointNonLinearMotorDescStruct {
        public PHHumanBallJointResistanceDescStruct() {
            PHHumanBallJointResistanceDesc desc = new PHHumanBallJointResistanceDesc();
            ApplyFrom(desc, true);
        }
        public Vec4dStruct xCoefficient;
        public Vec4dStruct yCoefficient;
        public Vec4dStruct zCoefficient;
        public void ApplyTo(PHHumanBallJointResistanceDesc r, bool apply_to_base_class = false) {
            r.xCoefficient = xCoefficient;
            r.yCoefficient = yCoefficient;
            r.zCoefficient = zCoefficient;
            if (apply_to_base_class) {
                base.ApplyTo(r, apply_to_base_class);
            }
        }
        public void ApplyFrom(PHHumanBallJointResistanceDesc r, bool apply_to_base_class = false) {
            xCoefficient = r.xCoefficient;
            yCoefficient = r.yCoefficient;
            zCoefficient = r.zCoefficient;
            if (apply_to_base_class) {
                base.ApplyFrom(r, apply_to_base_class);
            }
        }
        public static implicit operator PHHumanBallJointResistanceDesc(PHHumanBallJointResistanceDescStruct m) {
            PHHumanBallJointResistanceDesc r = new PHHumanBallJointResistanceDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHHumanBallJointResistanceDescStruct(PHHumanBallJointResistanceDesc r) {
            PHHumanBallJointResistanceDescStruct m = new PHHumanBallJointResistanceDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSpringMotorDesc : CsObject {
        public PHSpringMotorDesc() { _this = SprExport.Spr_new_PHSpringMotorDesc(); _flag = true; }
        public PHSpringMotorDesc(IntPtr ptr) : base(ptr) { }
        public PHSpringMotorDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSpringMotorDesc() { if (_flag) { SprExport.Spr_delete_PHSpringMotorDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHSpringMotorDescStruct : CsObject {
        public PHSpringMotorDescStruct() {
            PHSpringMotorDesc desc = new PHSpringMotorDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHSpringMotorDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHSpringMotorDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHSpringMotorDesc(PHSpringMotorDescStruct m) {
            PHSpringMotorDesc r = new PHSpringMotorDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSpringMotorDescStruct(PHSpringMotorDesc r) {
            PHSpringMotorDescStruct m = new PHSpringMotorDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHInactiveSolids : CsObject {
        public PHInactiveSolids() { _this = SprExport.Spr_new_PHInactiveSolids(); _flag = true; }
        public PHInactiveSolids(IntPtr ptr) : base(ptr) { }
        public PHInactiveSolids(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHInactiveSolids() { if (_flag) { SprExport.Spr_delete_PHInactiveSolids(_this); _flag = false; } }
        public vectorwrapper_string solids {
            get { return new vectorwrapper_string(SprExport.Spr_PHInactiveSolids_addr_solids(_this)); }
            set { SprExport.Spr_PHInactiveSolids_set_solids(_this, value); }
        }
    }
    public partial class PHOpObjState : CsObject {
        public PHOpObjState() { _this = SprExport.Spr_new_PHOpObjState(); _flag = true; }
        public PHOpObjState(IntPtr ptr) : base(ptr) { }
        public PHOpObjState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpObjState() { if (_flag) { SprExport.Spr_delete_PHOpObjState(_this); _flag = false; } }
        public Vec3d velocity {
            get { return new Vec3d(SprExport.Spr_PHOpObjState_addr_velocity(_this)); }
            set { SprExport.Spr_PHOpObjState_set_velocity(_this, value); }
        }
        public Vec3d angVelocity {
            get { return new Vec3d(SprExport.Spr_PHOpObjState_addr_angVelocity(_this)); }
            set { SprExport.Spr_PHOpObjState_set_angVelocity(_this, value); }
        }
        public Posed pose {
            get { return new Posed(SprExport.Spr_PHOpObjState_addr_pose(_this)); }
            set { SprExport.Spr_PHOpObjState_set_pose(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpObjStateStruct : CsObject {
        public PHOpObjStateStruct() {
            PHOpObjState desc = new PHOpObjState();
            ApplyFrom(desc, false);
        }
        public Vec3dStruct velocity;
        public Vec3dStruct angVelocity;
        public PosedStruct pose;
        public void ApplyTo(PHOpObjState r, bool apply_to_base_class = false) {
            r.velocity = velocity;
            r.angVelocity = angVelocity;
            r.pose = pose;
        }
        public void ApplyFrom(PHOpObjState r, bool apply_to_base_class = false) {
            velocity = r.velocity;
            angVelocity = r.angVelocity;
            pose = r.pose;
        }
        public static implicit operator PHOpObjState(PHOpObjStateStruct m) {
            PHOpObjState r = new PHOpObjState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpObjStateStruct(PHOpObjState r) {
            PHOpObjStateStruct m = new PHOpObjStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpHapticRendererDesc : CsObject {
        public PHOpHapticRendererDesc() { _this = SprExport.Spr_new_PHOpHapticRendererDesc(); _flag = true; }
        public PHOpHapticRendererDesc(IntPtr ptr) : base(ptr) { }
        public PHOpHapticRendererDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpHapticRendererDesc() { if (_flag) { SprExport.Spr_delete_PHOpHapticRendererDesc(_this); _flag = false; } }
        public float extForceSpring {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_extForceSpring(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_extForceSpring(_this, value); }
        }
        public float outForceSpring {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_outForceSpring(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_outForceSpring(_this, value); }
        }
        public float constraintSpring {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_constraintSpring(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_constraintSpring(_this, value); }
        }
        public float timeStep {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_timeStep(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_timeStep(_this, value); }
        }
        public float forceOnRadius {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_forceOnRadius(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_forceOnRadius(_this, value); }
        }
        public bool useConstrainForce {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_useConstrainForce(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_useConstrainForce(_this, (char)val);
            }
        }
        public bool useProxyfixSubitr {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_useProxyfixSubitr(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_useProxyfixSubitr(_this, (char)val);
            }
        }
        public bool useIncreaseMeshMove {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_useIncreaseMeshMove(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_useIncreaseMeshMove(_this, (char)val);
            }
        }
        public bool sqrtAlphaForce {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_sqrtAlphaForce(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_sqrtAlphaForce(_this, (char)val);
            }
        }
        public bool hitWall {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_hitWall(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_hitWall(_this, (char)val);
            }
        }
        public bool useDualTranFix {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_useDualTranFix(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_useDualTranFix(_this, (char)val);
            }
        }
        public bool rigid {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticRendererDesc_get_rigid(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticRendererDesc_set_rigid(_this, (char)val);
            }
        }
        public int proxyItrtNum {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_proxyItrtNum(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_proxyItrtNum(_this, value); }
        }
        public int proxyItrtMaxNum {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_proxyItrtMaxNum(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_proxyItrtMaxNum(_this, value); }
        }
        public int currDtType {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_currDtType(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_currDtType(_this, value); }
        }
        public int objNum {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_objNum(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_objNum(_this, value); }
        }
        public int noCtcItrNum {
            get { return SprExport.Spr_PHOpHapticRendererDesc_get_noCtcItrNum(_this); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_noCtcItrNum(_this, value); }
        }
        public Vec3f proxyFixPos {
            get { return new Vec3f(SprExport.Spr_PHOpHapticRendererDesc_addr_proxyFixPos(_this)); }
            set { SprExport.Spr_PHOpHapticRendererDesc_set_proxyFixPos(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpHapticRendererDescStruct : CsObject {
        public PHOpHapticRendererDescStruct() {
            PHOpHapticRendererDesc desc = new PHOpHapticRendererDesc();
            ApplyFrom(desc, false);
        }
        public float extForceSpring;
        public float outForceSpring;
        public float constraintSpring;
        public float timeStep;
        public float forceOnRadius;
        public bool useConstrainForce;
        public bool useProxyfixSubitr;
        public bool useIncreaseMeshMove;
        public bool sqrtAlphaForce;
        public bool hitWall;
        public bool useDualTranFix;
        public bool rigid;
        public int proxyItrtNum;
        public int proxyItrtMaxNum;
        public int currDtType;
        public int objNum;
        public int noCtcItrNum;
        public Vec3fStruct proxyFixPos;
        public void ApplyTo(PHOpHapticRendererDesc r, bool apply_to_base_class = false) {
            r.extForceSpring = extForceSpring;
            r.outForceSpring = outForceSpring;
            r.constraintSpring = constraintSpring;
            r.timeStep = timeStep;
            r.forceOnRadius = forceOnRadius;
            r.useConstrainForce = useConstrainForce;
            r.useProxyfixSubitr = useProxyfixSubitr;
            r.useIncreaseMeshMove = useIncreaseMeshMove;
            r.sqrtAlphaForce = sqrtAlphaForce;
            r.hitWall = hitWall;
            r.useDualTranFix = useDualTranFix;
            r.rigid = rigid;
            r.proxyItrtNum = proxyItrtNum;
            r.proxyItrtMaxNum = proxyItrtMaxNum;
            r.currDtType = currDtType;
            r.objNum = objNum;
            r.noCtcItrNum = noCtcItrNum;
            r.proxyFixPos = proxyFixPos;
        }
        public void ApplyFrom(PHOpHapticRendererDesc r, bool apply_to_base_class = false) {
            extForceSpring = r.extForceSpring;
            outForceSpring = r.outForceSpring;
            constraintSpring = r.constraintSpring;
            timeStep = r.timeStep;
            forceOnRadius = r.forceOnRadius;
            useConstrainForce = r.useConstrainForce;
            useProxyfixSubitr = r.useProxyfixSubitr;
            useIncreaseMeshMove = r.useIncreaseMeshMove;
            sqrtAlphaForce = r.sqrtAlphaForce;
            hitWall = r.hitWall;
            useDualTranFix = r.useDualTranFix;
            rigid = r.rigid;
            proxyItrtNum = r.proxyItrtNum;
            proxyItrtMaxNum = r.proxyItrtMaxNum;
            currDtType = r.currDtType;
            objNum = r.objNum;
            noCtcItrNum = r.noCtcItrNum;
            proxyFixPos = r.proxyFixPos;
        }
        public static implicit operator PHOpHapticRendererDesc(PHOpHapticRendererDescStruct m) {
            PHOpHapticRendererDesc r = new PHOpHapticRendererDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpHapticRendererDescStruct(PHOpHapticRendererDesc r) {
            PHOpHapticRendererDescStruct m = new PHOpHapticRendererDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpObjDesc : PHOpObjState {
        public PHOpObjDesc() { _this = SprExport.Spr_new_PHOpObjDesc(); _flag = true; }
        public PHOpObjDesc(IntPtr ptr) : base(ptr) { }
        public PHOpObjDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpObjDesc() { if (_flag) { SprExport.Spr_delete_PHOpObjDesc(_this); _flag = false; } }
        public double mass {
            get { return SprExport.Spr_PHOpObjDesc_get_mass(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_mass(_this, value); }
        }
        public Matrix3d inertia {
            get { return new Matrix3d(SprExport.Spr_PHOpObjDesc_addr_inertia(_this)); }
            set { SprExport.Spr_PHOpObjDesc_set_inertia(_this, value); }
        }
        public Vec3d center {
            get { return new Vec3d(SprExport.Spr_PHOpObjDesc_addr_center(_this)); }
            set { SprExport.Spr_PHOpObjDesc_set_center(_this, value); }
        }
        public bool dynamical {
            get {
                byte ret = (byte)SprExport.Spr_PHOpObjDesc_get_dynamical(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpObjDesc_set_dynamical(_this, (char)val);
            }
        }
        public Vec3f objTargetVts {
            get { return new Vec3f(SprExport.Spr_PHOpObjDesc_get_objTargetVts(_this)); }
            set { SprExport.Spr_PHOpObjDesc_set_objTargetVts(_this, value); }
        }
        public arraywrapper_float objTargetVtsArr {
            get { return new arraywrapper_float(SprExport.Spr_PHOpObjDesc_addr_objTargetVtsArr(_this)); }
            set { SprExport.Spr_PHOpObjDesc_set_objTargetVtsArr(_this, value); }
        }
        public int objTargetVtsNum {
            get { return SprExport.Spr_PHOpObjDesc_get_objTargetVtsNum(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objTargetVtsNum(_this, value); }
        }
        public int assPsNum {
            get { return SprExport.Spr_PHOpObjDesc_get_assPsNum(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_assPsNum(_this, value); }
        }
        public int assGrpNum {
            get { return SprExport.Spr_PHOpObjDesc_get_assGrpNum(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_assGrpNum(_this, value); }
        }
        public int objGrouplinkCount {
            get { return SprExport.Spr_PHOpObjDesc_get_objGrouplinkCount(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objGrouplinkCount(_this, value); }
        }
        public int objId {
            get { return SprExport.Spr_PHOpObjDesc_get_objId(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objId(_this, value); }
        }
        public bool objUseDistCstr {
            get {
                byte ret = (byte)SprExport.Spr_PHOpObjDesc_get_objUseDistCstr(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpObjDesc_set_objUseDistCstr(_this, (char)val);
            }
        }
        public bool objNoMeshObj {
            get {
                byte ret = (byte)SprExport.Spr_PHOpObjDesc_get_objNoMeshObj(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpObjDesc_set_objNoMeshObj(_this, (char)val);
            }
        }
        public bool isRigid {
            get {
                byte ret = (byte)SprExport.Spr_PHOpObjDesc_get_isRigid(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpObjDesc_set_isRigid(_this, (char)val);
            }
        }
        public int objType {
            get { return SprExport.Spr_PHOpObjDesc_get_objType(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objType(_this, value); }
        }
        public float objAverRadius {
            get { return SprExport.Spr_PHOpObjDesc_get_objAverRadius(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objAverRadius(_this, value); }
        }
        public float objMaxRadius {
            get { return SprExport.Spr_PHOpObjDesc_get_objMaxRadius(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objMaxRadius(_this, value); }
        }
        public int objitrTime {
            get { return SprExport.Spr_PHOpObjDesc_get_objitrTime(_this); }
            set { SprExport.Spr_PHOpObjDesc_set_objitrTime(_this, value); }
        }
        public bool updateNormals {
            get {
                byte ret = (byte)SprExport.Spr_PHOpObjDesc_get_updateNormals(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpObjDesc_set_updateNormals(_this, (char)val);
            }
        }
        public void Init() {
            SprExport.Spr_PHOpObjDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHOpObjDescStruct : PHOpObjStateStruct {
        public PHOpObjDescStruct() {
            PHOpObjDesc desc = new PHOpObjDesc();
            ApplyFrom(desc, false);
        }
        public double mass;
        public Matrix3dStruct inertia;
        public Vec3dStruct center;
        public bool dynamical;
        public Vec3fStruct objTargetVts;
        public arraywrapper_float objTargetVtsArr;
        public int objTargetVtsNum;
        public int assPsNum;
        public int assGrpNum;
        public int objGrouplinkCount;
        public int objId;
        public bool objUseDistCstr;
        public bool objNoMeshObj;
        public bool isRigid;
        public int objType;
        public float objAverRadius;
        public float objMaxRadius;
        public int objitrTime;
        public bool updateNormals;
        public void ApplyTo(PHOpObjDesc r, bool apply_to_base_class = false) {
            r.mass = mass;
            r.inertia = inertia;
            r.center = center;
            r.dynamical = dynamical;
            r.objTargetVts = objTargetVts;
            r.objTargetVtsArr = objTargetVtsArr;
            r.objTargetVtsNum = objTargetVtsNum;
            r.assPsNum = assPsNum;
            r.assGrpNum = assGrpNum;
            r.objGrouplinkCount = objGrouplinkCount;
            r.objId = objId;
            r.objUseDistCstr = objUseDistCstr;
            r.objNoMeshObj = objNoMeshObj;
            r.isRigid = isRigid;
            r.objType = objType;
            r.objAverRadius = objAverRadius;
            r.objMaxRadius = objMaxRadius;
            r.objitrTime = objitrTime;
            r.updateNormals = updateNormals;
        }
        public void ApplyFrom(PHOpObjDesc r, bool apply_to_base_class = false) {
            mass = r.mass;
            inertia = r.inertia;
            center = r.center;
            dynamical = r.dynamical;
            objTargetVts = r.objTargetVts;
            objTargetVtsArr = r.objTargetVtsArr;
            objTargetVtsNum = r.objTargetVtsNum;
            assPsNum = r.assPsNum;
            assGrpNum = r.assGrpNum;
            objGrouplinkCount = r.objGrouplinkCount;
            objId = r.objId;
            objUseDistCstr = r.objUseDistCstr;
            objNoMeshObj = r.objNoMeshObj;
            isRigid = r.isRigid;
            objType = r.objType;
            objAverRadius = r.objAverRadius;
            objMaxRadius = r.objMaxRadius;
            objitrTime = r.objitrTime;
            updateNormals = r.updateNormals;
        }
        public static implicit operator PHOpObjDesc(PHOpObjDescStruct m) {
            PHOpObjDesc r = new PHOpObjDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpObjDescStruct(PHOpObjDesc r) {
            PHOpObjDescStruct m = new PHOpObjDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpParticleDesc : CsObject {
        public PHOpParticleDesc() { _this = SprExport.Spr_new_PHOpParticleDesc(); _flag = true; }
        public PHOpParticleDesc(IntPtr ptr) : base(ptr) { }
        public PHOpParticleDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpParticleDesc() { if (_flag) { SprExport.Spr_delete_PHOpParticleDesc(_this); _flag = false; } }
        public int pNvertex {
            get { return SprExport.Spr_PHOpParticleDesc_get_pNvertex(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pNvertex(_this, value); }
        }
        public int pMyGroupInd {
            get { return SprExport.Spr_PHOpParticleDesc_get_pMyGroupInd(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pMyGroupInd(_this, value); }
        }
        public int pPId {
            get { return SprExport.Spr_PHOpParticleDesc_get_pPId(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pPId(_this, value); }
        }
        public int pObjId {
            get { return SprExport.Spr_PHOpParticleDesc_get_pObjId(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pObjId(_this, value); }
        }
        public float pParaAlpha {
            get { return SprExport.Spr_PHOpParticleDesc_get_pParaAlpha(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pParaAlpha(_this, value); }
        }
        public bool hitedByMouse {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_hitedByMouse(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_hitedByMouse(_this, (char)val);
            }
        }
        public float pSingleVMass {
            get { return SprExport.Spr_PHOpParticleDesc_get_pSingleVMass(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pSingleVMass(_this, value); }
        }
        public float pTotalMass {
            get { return SprExport.Spr_PHOpParticleDesc_get_pTotalMass(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pTotalMass(_this, value); }
        }
        public float pRadii {
            get { return SprExport.Spr_PHOpParticleDesc_get_pRadii(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pRadii(_this, value); }
        }
        public float pDynamicRadius {
            get { return SprExport.Spr_PHOpParticleDesc_get_pDynamicRadius(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pDynamicRadius(_this, value); }
        }
        public Vec3f pMainRadiusVec {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pMainRadiusVec(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pMainRadiusVec(_this, value); }
        }
        public Vec3f pSecRadiusVec {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pSecRadiusVec(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pSecRadiusVec(_this, value); }
        }
        public Vec3f pThrRadiusVec {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pThrRadiusVec(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pThrRadiusVec(_this, value); }
        }
        public float pMainRadius {
            get { return SprExport.Spr_PHOpParticleDesc_get_pMainRadius(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pMainRadius(_this, value); }
        }
        public float pSecRadius {
            get { return SprExport.Spr_PHOpParticleDesc_get_pSecRadius(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pSecRadius(_this, value); }
        }
        public float pThrRadius {
            get { return SprExport.Spr_PHOpParticleDesc_get_pThrRadius(_this); }
            set { SprExport.Spr_PHOpParticleDesc_set_pThrRadius(_this, value); }
        }
        public Vec3f pCurrCtr {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pCurrCtr(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pCurrCtr(_this, value); }
        }
        public Vec3f pOrigCtr {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pOrigCtr(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pOrigCtr(_this, value); }
        }
        public Vec3f pNewCtr {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pNewCtr(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pNewCtr(_this, value); }
        }
        public Vec3f pVelocity {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pVelocity(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pVelocity(_this, value); }
        }
        public bool isColliedbySphash {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isColliedbySphash(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isColliedbySphash(_this, (char)val);
            }
        }
        public bool isColliedbyColliCube {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isColliedbyColliCube(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isColliedbyColliCube(_this, (char)val);
            }
        }
        public bool isFixedbyMouse {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isFixedbyMouse(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isFixedbyMouse(_this, (char)val);
            }
        }
        public bool isFixed {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isFixed(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isFixed(_this, (char)val);
            }
        }
        public bool isColliedSphashSolved {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isColliedSphashSolved(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isColliedSphashSolved(_this, (char)val);
            }
        }
        public bool isColliedSphashSolvedReady {
            get {
                byte ret = (byte)SprExport.Spr_PHOpParticleDesc_get_isColliedSphashSolvedReady(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpParticleDesc_set_isColliedSphashSolvedReady(_this, (char)val);
            }
        }
        public Vec3f pExternalForce {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pExternalForce(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pExternalForce(_this, value); }
        }
        public Vec3f pExternalTorque {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pExternalTorque(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pExternalTorque(_this, value); }
        }
        public Vec3f pExternalDisplacement {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pExternalDisplacement(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pExternalDisplacement(_this, value); }
        }
        public vectorwrapper_int pFaceInd {
            get { return new vectorwrapper_int(SprExport.Spr_PHOpParticleDesc_addr_pFaceInd(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pFaceInd(_this, value); }
        }
        public Matrix3f pMomentR {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_pMomentR(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pMomentR(_this, value); }
        }
        public Matrix3f pSmR {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_pSmR(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pSmR(_this, value); }
        }
        public Matrix3f pMomentInertia {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_pMomentInertia(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pMomentInertia(_this, value); }
        }
        public Matrix3f pInverseOfMomentInertia {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_pInverseOfMomentInertia(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pInverseOfMomentInertia(_this, value); }
        }
        public Matrix3f ellipRotMatrix {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_ellipRotMatrix(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_ellipRotMatrix(_this, value); }
        }
        public Quaternionf ellipOrigOrint {
            get { return new Quaternionf(SprExport.Spr_PHOpParticleDesc_addr_ellipOrigOrint(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_ellipOrigOrint(_this, value); }
        }
        public Vec3f pWvel {
            get { return new Vec3f(SprExport.Spr_PHOpParticleDesc_addr_pWvel(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pWvel(_this, value); }
        }
        public Matrix3f pPreEllipA {
            get { return new Matrix3f(SprExport.Spr_PHOpParticleDesc_addr_pPreEllipA(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pPreEllipA(_this, value); }
        }
        public Quaternionf pCurrOrint {
            get { return new Quaternionf(SprExport.Spr_PHOpParticleDesc_addr_pCurrOrint(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pCurrOrint(_this, value); }
        }
        public Quaternionf pOrigOrint {
            get { return new Quaternionf(SprExport.Spr_PHOpParticleDesc_addr_pOrigOrint(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pOrigOrint(_this, value); }
        }
        public Quaternionf pNewOrint {
            get { return new Quaternionf(SprExport.Spr_PHOpParticleDesc_addr_pNewOrint(_this)); }
            set { SprExport.Spr_PHOpParticleDesc_set_pNewOrint(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpParticleDescStruct : CsObject {
        public PHOpParticleDescStruct() {
            PHOpParticleDesc desc = new PHOpParticleDesc();
            ApplyFrom(desc, false);
        }
        public int pNvertex;
        public int pMyGroupInd;
        public int pPId;
        public int pObjId;
        public float pParaAlpha;
        public bool hitedByMouse;
        public float pSingleVMass;
        public float pTotalMass;
        public float pRadii;
        public float pDynamicRadius;
        public Vec3fStruct pMainRadiusVec;
        public Vec3fStruct pSecRadiusVec;
        public Vec3fStruct pThrRadiusVec;
        public float pMainRadius;
        public float pSecRadius;
        public float pThrRadius;
        public Vec3fStruct pCurrCtr;
        public Vec3fStruct pOrigCtr;
        public Vec3fStruct pNewCtr;
        public Vec3fStruct pVelocity;
        public bool isColliedbySphash;
        public bool isColliedbyColliCube;
        public bool isFixedbyMouse;
        public bool isFixed;
        public bool isColliedSphashSolved;
        public bool isColliedSphashSolvedReady;
        public Vec3fStruct pExternalForce;
        public Vec3fStruct pExternalTorque;
        public Vec3fStruct pExternalDisplacement;
        public vectorwrapper_int pFaceInd;
        public Matrix3fStruct pMomentR;
        public Matrix3fStruct pSmR;
        public Matrix3fStruct pMomentInertia;
        public Matrix3fStruct pInverseOfMomentInertia;
        public Matrix3fStruct ellipRotMatrix;
        public QuaternionfStruct ellipOrigOrint;
        public Vec3fStruct pWvel;
        public Matrix3fStruct pPreEllipA;
        public QuaternionfStruct pCurrOrint;
        public QuaternionfStruct pOrigOrint;
        public QuaternionfStruct pNewOrint;
        public void ApplyTo(PHOpParticleDesc r, bool apply_to_base_class = false) {
            r.pNvertex = pNvertex;
            r.pMyGroupInd = pMyGroupInd;
            r.pPId = pPId;
            r.pObjId = pObjId;
            r.pParaAlpha = pParaAlpha;
            r.hitedByMouse = hitedByMouse;
            r.pSingleVMass = pSingleVMass;
            r.pTotalMass = pTotalMass;
            r.pRadii = pRadii;
            r.pDynamicRadius = pDynamicRadius;
            r.pMainRadiusVec = pMainRadiusVec;
            r.pSecRadiusVec = pSecRadiusVec;
            r.pThrRadiusVec = pThrRadiusVec;
            r.pMainRadius = pMainRadius;
            r.pSecRadius = pSecRadius;
            r.pThrRadius = pThrRadius;
            r.pCurrCtr = pCurrCtr;
            r.pOrigCtr = pOrigCtr;
            r.pNewCtr = pNewCtr;
            r.pVelocity = pVelocity;
            r.isColliedbySphash = isColliedbySphash;
            r.isColliedbyColliCube = isColliedbyColliCube;
            r.isFixedbyMouse = isFixedbyMouse;
            r.isFixed = isFixed;
            r.isColliedSphashSolved = isColliedSphashSolved;
            r.isColliedSphashSolvedReady = isColliedSphashSolvedReady;
            r.pExternalForce = pExternalForce;
            r.pExternalTorque = pExternalTorque;
            r.pExternalDisplacement = pExternalDisplacement;
            r.pFaceInd = pFaceInd;
            r.pMomentR = pMomentR;
            r.pSmR = pSmR;
            r.pMomentInertia = pMomentInertia;
            r.pInverseOfMomentInertia = pInverseOfMomentInertia;
            r.ellipRotMatrix = ellipRotMatrix;
            r.ellipOrigOrint = ellipOrigOrint;
            r.pWvel = pWvel;
            r.pPreEllipA = pPreEllipA;
            r.pCurrOrint = pCurrOrint;
            r.pOrigOrint = pOrigOrint;
            r.pNewOrint = pNewOrint;
        }
        public void ApplyFrom(PHOpParticleDesc r, bool apply_to_base_class = false) {
            pNvertex = r.pNvertex;
            pMyGroupInd = r.pMyGroupInd;
            pPId = r.pPId;
            pObjId = r.pObjId;
            pParaAlpha = r.pParaAlpha;
            hitedByMouse = r.hitedByMouse;
            pSingleVMass = r.pSingleVMass;
            pTotalMass = r.pTotalMass;
            pRadii = r.pRadii;
            pDynamicRadius = r.pDynamicRadius;
            pMainRadiusVec = r.pMainRadiusVec;
            pSecRadiusVec = r.pSecRadiusVec;
            pThrRadiusVec = r.pThrRadiusVec;
            pMainRadius = r.pMainRadius;
            pSecRadius = r.pSecRadius;
            pThrRadius = r.pThrRadius;
            pCurrCtr = r.pCurrCtr;
            pOrigCtr = r.pOrigCtr;
            pNewCtr = r.pNewCtr;
            pVelocity = r.pVelocity;
            isColliedbySphash = r.isColliedbySphash;
            isColliedbyColliCube = r.isColliedbyColliCube;
            isFixedbyMouse = r.isFixedbyMouse;
            isFixed = r.isFixed;
            isColliedSphashSolved = r.isColliedSphashSolved;
            isColliedSphashSolvedReady = r.isColliedSphashSolvedReady;
            pExternalForce = r.pExternalForce;
            pExternalTorque = r.pExternalTorque;
            pExternalDisplacement = r.pExternalDisplacement;
            pFaceInd = r.pFaceInd;
            pMomentR = r.pMomentR;
            pSmR = r.pSmR;
            pMomentInertia = r.pMomentInertia;
            pInverseOfMomentInertia = r.pInverseOfMomentInertia;
            ellipRotMatrix = r.ellipRotMatrix;
            ellipOrigOrint = r.ellipOrigOrint;
            pWvel = r.pWvel;
            pPreEllipA = r.pPreEllipA;
            pCurrOrint = r.pCurrOrint;
            pOrigOrint = r.pOrigOrint;
            pNewOrint = r.pNewOrint;
        }
        public static implicit operator PHOpParticleDesc(PHOpParticleDescStruct m) {
            PHOpParticleDesc r = new PHOpParticleDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpParticleDescStruct(PHOpParticleDesc r) {
            PHOpParticleDescStruct m = new PHOpParticleDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpGroupDesc : CsObject {
        public PHOpGroupDesc() { _this = SprExport.Spr_new_PHOpGroupDesc(); _flag = true; }
        public PHOpGroupDesc(IntPtr ptr) : base(ptr) { }
        public PHOpGroupDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpGroupDesc() { if (_flag) { SprExport.Spr_delete_PHOpGroupDesc(_this); _flag = false; } }
        public int gMyIndex {
            get { return SprExport.Spr_PHOpGroupDesc_get_gMyIndex(_this); }
            set { SprExport.Spr_PHOpGroupDesc_set_gMyIndex(_this, value); }
        }
        public Matrix3f gAgroup {
            get { return new Matrix3f(SprExport.Spr_PHOpGroupDesc_addr_gAgroup(_this)); }
            set { SprExport.Spr_PHOpGroupDesc_set_gAgroup(_this, value); }
        }
        public Matrix3f gEigen {
            get { return new Matrix3f(SprExport.Spr_PHOpGroupDesc_addr_gEigen(_this)); }
            set { SprExport.Spr_PHOpGroupDesc_set_gEigen(_this, value); }
        }
        public float gtotalMass {
            get { return SprExport.Spr_PHOpGroupDesc_get_gtotalMass(_this); }
            set { SprExport.Spr_PHOpGroupDesc_set_gtotalMass(_this, value); }
        }
        public Vec3f gOrigCenter {
            get { return new Vec3f(SprExport.Spr_PHOpGroupDesc_addr_gOrigCenter(_this)); }
            set { SprExport.Spr_PHOpGroupDesc_set_gOrigCenter(_this, value); }
        }
        public Vec3f gCurrCenter {
            get { return new Vec3f(SprExport.Spr_PHOpGroupDesc_addr_gCurrCenter(_this)); }
            set { SprExport.Spr_PHOpGroupDesc_set_gCurrCenter(_this, value); }
        }
        public int gNptcl {
            get { return SprExport.Spr_PHOpGroupDesc_get_gNptcl(_this); }
            set { SprExport.Spr_PHOpGroupDesc_set_gNptcl(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpGroupDescStruct : CsObject {
        public PHOpGroupDescStruct() {
            PHOpGroupDesc desc = new PHOpGroupDesc();
            ApplyFrom(desc, false);
        }
        public int gMyIndex;
        public Matrix3fStruct gAgroup;
        public Matrix3fStruct gEigen;
        public float gtotalMass;
        public Vec3fStruct gOrigCenter;
        public Vec3fStruct gCurrCenter;
        public int gNptcl;
        public void ApplyTo(PHOpGroupDesc r, bool apply_to_base_class = false) {
            r.gMyIndex = gMyIndex;
            r.gAgroup = gAgroup;
            r.gEigen = gEigen;
            r.gtotalMass = gtotalMass;
            r.gOrigCenter = gOrigCenter;
            r.gCurrCenter = gCurrCenter;
            r.gNptcl = gNptcl;
        }
        public void ApplyFrom(PHOpGroupDesc r, bool apply_to_base_class = false) {
            gMyIndex = r.gMyIndex;
            gAgroup = r.gAgroup;
            gEigen = r.gEigen;
            gtotalMass = r.gtotalMass;
            gOrigCenter = r.gOrigCenter;
            gCurrCenter = r.gCurrCenter;
            gNptcl = r.gNptcl;
        }
        public static implicit operator PHOpGroupDesc(PHOpGroupDescStruct m) {
            PHOpGroupDesc r = new PHOpGroupDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpGroupDescStruct(PHOpGroupDesc r) {
            PHOpGroupDescStruct m = new PHOpGroupDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpHapticControllerDesc : CsObject {
        public PHOpHapticControllerDesc() { _this = SprExport.Spr_new_PHOpHapticControllerDesc(); _flag = true; }
        public PHOpHapticControllerDesc(IntPtr ptr) : base(ptr) { }
        public PHOpHapticControllerDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpHapticControllerDesc() { if (_flag) { SprExport.Spr_delete_PHOpHapticControllerDesc(_this); _flag = false; } }
        public float posScale {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_posScale(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_posScale(_this, value); }
        }
        public float rotScale {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_rotScale(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_rotScale(_this, value); }
        }
        public float forceScale {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_forceScale(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_forceScale(_this, value); }
        }
        public Vec3f userPos {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_userPos(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_userPos(_this, value); }
        }
        public Posef userPose {
            get { return new Posef(SprExport.Spr_PHOpHapticControllerDesc_addr_userPose(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_userPose(_this, value); }
        }
        public Vec3f hcCurrPPos {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_hcCurrPPos(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcCurrPPos(_this, value); }
        }
        public Vec3f hcCurrUPos {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_hcCurrUPos(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcCurrUPos(_this, value); }
        }
        public Vec3f hcLastUPos {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_hcLastUPos(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcLastUPos(_this, value); }
        }
        public Vec3f hcFixsubGoal {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_hcFixsubGoal(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcFixsubGoal(_this, value); }
        }
        public float c_obstRadius {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_c_obstRadius(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_c_obstRadius(_this, value); }
        }
        public int hpObjIndex {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_hpObjIndex(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hpObjIndex(_this, value); }
        }
        public HapticDOFType hcType {
            get { return (HapticDOFType)SprExport.Spr_PHOpHapticControllerDesc_get_hcType(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcType(_this, (int)value); }
        }
        public bool logForce {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticControllerDesc_get_logForce(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticControllerDesc_set_logForce(_this, (char)val);
            }
        }
        public bool hcReady {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticControllerDesc_get_hcReady(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticControllerDesc_set_hcReady(_this, (char)val);
            }
        }
        public bool IsSetForceReady {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticControllerDesc_get_IsSetForceReady(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticControllerDesc_set_IsSetForceReady(_this, (char)val);
            }
        }
        public int fileindex {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_fileindex(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_fileindex(_this, value); }
        }
        public Vec3f lastHpRateF {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_lastHpRateF(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_lastHpRateF(_this, value); }
        }
        public bool hcCollied {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticControllerDesc_get_hcCollied(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticControllerDesc_set_hcCollied(_this, (char)val);
            }
        }
        public int constrainCount {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_constrainCount(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_constrainCount(_this, value); }
        }
        public int constrainCountHc {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_constrainCountHc(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_constrainCountHc(_this, value); }
        }
        public Vec3f couterPlaneN {
            get { return new Vec3f(SprExport.Spr_PHOpHapticControllerDesc_addr_couterPlaneN(_this)); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_couterPlaneN(_this, value); }
        }
        public float hcElmDtcRadius {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_hcElmDtcRadius(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_hcElmDtcRadius(_this, value); }
        }
        public int collectCount {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_collectCount(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_collectCount(_this, value); }
        }
        public int collectItrtor {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_collectItrtor(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_collectItrtor(_this, value); }
        }
        public bool hcProxyOn {
            get {
                byte ret = (byte)SprExport.Spr_PHOpHapticControllerDesc_get_hcProxyOn(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpHapticControllerDesc_set_hcProxyOn(_this, (char)val);
            }
        }
        public int suspObjid {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_suspObjid(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_suspObjid(_this, value); }
        }
        public float proxyRadius {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_proxyRadius(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_proxyRadius(_this, value); }
        }
        public int surrCnstrs {
            get { return SprExport.Spr_PHOpHapticControllerDesc_get_surrCnstrs(_this); }
            set { SprExport.Spr_PHOpHapticControllerDesc_set_surrCnstrs(_this, value); }
        }
        public enum HapticDOFType : int {
            _3DOF = 0, _6DOF = _3DOF + 1
        }
        public enum cstrainType : int {
            cstPoint, cstEdge = cstPoint + 1, cstFace = cstEdge + 1
        }
    }
    [System.Serializable]
    public class PHOpHapticControllerDescStruct : CsObject {
        public PHOpHapticControllerDescStruct() {
            PHOpHapticControllerDesc desc = new PHOpHapticControllerDesc();
            ApplyFrom(desc, false);
        }
        public float posScale;
        public float rotScale;
        public float forceScale;
        public Vec3fStruct userPos;
        public PosefStruct userPose;
        public Vec3fStruct hcCurrPPos;
        public Vec3fStruct hcCurrUPos;
        public Vec3fStruct hcLastUPos;
        public Vec3fStruct hcFixsubGoal;
        public float c_obstRadius;
        public int hpObjIndex;
        public PHOpHapticControllerDesc.HapticDOFType hcType;
        public bool logForce;
        public bool hcReady;
        public bool IsSetForceReady;
        public int fileindex;
        public Vec3fStruct lastHpRateF;
        public bool hcCollied;
        public int constrainCount;
        public int constrainCountHc;
        public Vec3fStruct couterPlaneN;
        public float hcElmDtcRadius;
        public int collectCount;
        public int collectItrtor;
        public bool hcProxyOn;
        public int suspObjid;
        public float proxyRadius;
        public int surrCnstrs;
        public void ApplyTo(PHOpHapticControllerDesc r, bool apply_to_base_class = false) {
            r.posScale = posScale;
            r.rotScale = rotScale;
            r.forceScale = forceScale;
            r.userPos = userPos;
            r.userPose = userPose;
            r.hcCurrPPos = hcCurrPPos;
            r.hcCurrUPos = hcCurrUPos;
            r.hcLastUPos = hcLastUPos;
            r.hcFixsubGoal = hcFixsubGoal;
            r.c_obstRadius = c_obstRadius;
            r.hpObjIndex = hpObjIndex;
            r.hcType = hcType;
            r.logForce = logForce;
            r.hcReady = hcReady;
            r.IsSetForceReady = IsSetForceReady;
            r.fileindex = fileindex;
            r.lastHpRateF = lastHpRateF;
            r.hcCollied = hcCollied;
            r.constrainCount = constrainCount;
            r.constrainCountHc = constrainCountHc;
            r.couterPlaneN = couterPlaneN;
            r.hcElmDtcRadius = hcElmDtcRadius;
            r.collectCount = collectCount;
            r.collectItrtor = collectItrtor;
            r.hcProxyOn = hcProxyOn;
            r.suspObjid = suspObjid;
            r.proxyRadius = proxyRadius;
            r.surrCnstrs = surrCnstrs;
        }
        public void ApplyFrom(PHOpHapticControllerDesc r, bool apply_to_base_class = false) {
            posScale = r.posScale;
            rotScale = r.rotScale;
            forceScale = r.forceScale;
            userPos = r.userPos;
            userPose = r.userPose;
            hcCurrPPos = r.hcCurrPPos;
            hcCurrUPos = r.hcCurrUPos;
            hcLastUPos = r.hcLastUPos;
            hcFixsubGoal = r.hcFixsubGoal;
            c_obstRadius = r.c_obstRadius;
            hpObjIndex = r.hpObjIndex;
            hcType = r.hcType;
            logForce = r.logForce;
            hcReady = r.hcReady;
            IsSetForceReady = r.IsSetForceReady;
            fileindex = r.fileindex;
            lastHpRateF = r.lastHpRateF;
            hcCollied = r.hcCollied;
            constrainCount = r.constrainCount;
            constrainCountHc = r.constrainCountHc;
            couterPlaneN = r.couterPlaneN;
            hcElmDtcRadius = r.hcElmDtcRadius;
            collectCount = r.collectCount;
            collectItrtor = r.collectItrtor;
            hcProxyOn = r.hcProxyOn;
            suspObjid = r.suspObjid;
            proxyRadius = r.proxyRadius;
            surrCnstrs = r.surrCnstrs;
        }
        public static implicit operator PHOpHapticControllerDesc(PHOpHapticControllerDescStruct m) {
            PHOpHapticControllerDesc r = new PHOpHapticControllerDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpHapticControllerDescStruct(PHOpHapticControllerDesc r) {
            PHOpHapticControllerDescStruct m = new PHOpHapticControllerDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpAnimationDesc : CsObject {
        public PHOpAnimationDesc() { _this = SprExport.Spr_new_PHOpAnimationDesc(); _flag = true; }
        public PHOpAnimationDesc(IntPtr ptr) : base(ptr) { }
        public PHOpAnimationDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpAnimationDesc() { if (_flag) { SprExport.Spr_delete_PHOpAnimationDesc(_this); _flag = false; } }
        public float timestep {
            get { return SprExport.Spr_PHOpAnimationDesc_get_timestep(_this); }
            set { SprExport.Spr_PHOpAnimationDesc_set_timestep(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpAnimationDescStruct : CsObject {
        public PHOpAnimationDescStruct() {
            PHOpAnimationDesc desc = new PHOpAnimationDesc();
            ApplyFrom(desc, false);
        }
        public float timestep;
        public void ApplyTo(PHOpAnimationDesc r, bool apply_to_base_class = false) {
            r.timestep = timestep;
        }
        public void ApplyFrom(PHOpAnimationDesc r, bool apply_to_base_class = false) {
            timestep = r.timestep;
        }
        public static implicit operator PHOpAnimationDesc(PHOpAnimationDescStruct m) {
            PHOpAnimationDesc r = new PHOpAnimationDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpAnimationDescStruct(PHOpAnimationDesc r) {
            PHOpAnimationDescStruct m = new PHOpAnimationDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHOpSpHashColliAgentDesc : CsObject {
        public PHOpSpHashColliAgentDesc() { _this = SprExport.Spr_new_PHOpSpHashColliAgentDesc(); _flag = true; }
        public PHOpSpHashColliAgentDesc(IntPtr ptr) : base(ptr) { }
        public PHOpSpHashColliAgentDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHOpSpHashColliAgentDesc() { if (_flag) { SprExport.Spr_delete_PHOpSpHashColliAgentDesc(_this); _flag = false; } }
        public bool useDirColli {
            get {
                byte ret = (byte)SprExport.Spr_PHOpSpHashColliAgentDesc_get_useDirColli(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHOpSpHashColliAgentDesc_set_useDirColli(_this, (char)val);
            }
        }
        public float collisionCstrStiffness {
            get { return SprExport.Spr_PHOpSpHashColliAgentDesc_get_collisionCstrStiffness(_this); }
            set { SprExport.Spr_PHOpSpHashColliAgentDesc_set_collisionCstrStiffness(_this, value); }
        }
    }
    [System.Serializable]
    public class PHOpSpHashColliAgentDescStruct : CsObject {
        public PHOpSpHashColliAgentDescStruct() {
            PHOpSpHashColliAgentDesc desc = new PHOpSpHashColliAgentDesc();
            ApplyFrom(desc, false);
        }
        public bool useDirColli;
        public float collisionCstrStiffness;
        public void ApplyTo(PHOpSpHashColliAgentDesc r, bool apply_to_base_class = false) {
            r.useDirColli = useDirColli;
            r.collisionCstrStiffness = collisionCstrStiffness;
        }
        public void ApplyFrom(PHOpSpHashColliAgentDesc r, bool apply_to_base_class = false) {
            useDirColli = r.useDirColli;
            collisionCstrStiffness = r.collisionCstrStiffness;
        }
        public static implicit operator PHOpSpHashColliAgentDesc(PHOpSpHashColliAgentDescStruct m) {
            PHOpSpHashColliAgentDesc r = new PHOpSpHashColliAgentDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHOpSpHashColliAgentDescStruct(PHOpSpHashColliAgentDesc r) {
            PHOpSpHashColliAgentDescStruct m = new PHOpSpHashColliAgentDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHRaycastHit : CsObject {
        public PHRaycastHit() { _this = SprExport.Spr_new_PHRaycastHit(); _flag = true; }
        public PHRaycastHit(IntPtr ptr) : base(ptr) { }
        public PHRaycastHit(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHRaycastHit() { if (_flag) { SprExport.Spr_delete_PHRaycastHit(_this); _flag = false; } }
        public PHSolidIf solid {
            get { return new PHSolidIf(SprExport.Spr_PHRaycastHit_get_solid(_this)); }
            set { SprExport.Spr_PHRaycastHit_set_solid(_this, value); }
        }
        public CDShapeIf shape {
            get { return new CDShapeIf(SprExport.Spr_PHRaycastHit_get_shape(_this)); }
            set { SprExport.Spr_PHRaycastHit_set_shape(_this, value); }
        }
        public Vec3f point {
            get { return new Vec3f(SprExport.Spr_PHRaycastHit_addr_point(_this)); }
            set { SprExport.Spr_PHRaycastHit_set_point(_this, value); }
        }
        public float distance {
            get { return SprExport.Spr_PHRaycastHit_get_distance(_this); }
            set { SprExport.Spr_PHRaycastHit_set_distance(_this, value); }
        }
    }
    public partial class PHRayDesc : CsObject {
        public PHRayDesc() { _this = SprExport.Spr_new_PHRayDesc(); _flag = true; }
        public PHRayDesc(IntPtr ptr) : base(ptr) { }
        public PHRayDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHRayDesc() { if (_flag) { SprExport.Spr_delete_PHRayDesc(_this); _flag = false; } }
        public Vec3d origin {
            get { return new Vec3d(SprExport.Spr_PHRayDesc_addr_origin(_this)); }
            set { SprExport.Spr_PHRayDesc_set_origin(_this, value); }
        }
        public Vec3d direction {
            get { return new Vec3d(SprExport.Spr_PHRayDesc_addr_direction(_this)); }
            set { SprExport.Spr_PHRayDesc_set_direction(_this, value); }
        }
    }
    [System.Serializable]
    public class PHRayDescStruct : CsObject {
        public PHRayDescStruct() {
            PHRayDesc desc = new PHRayDesc();
            ApplyFrom(desc, false);
        }
        public Vec3dStruct origin;
        public Vec3dStruct direction;
        public void ApplyTo(PHRayDesc r, bool apply_to_base_class = false) {
            r.origin = origin;
            r.direction = direction;
        }
        public void ApplyFrom(PHRayDesc r, bool apply_to_base_class = false) {
            origin = r.origin;
            direction = r.direction;
        }
        public static implicit operator PHRayDesc(PHRayDescStruct m) {
            PHRayDesc r = new PHRayDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHRayDescStruct(PHRayDesc r) {
            PHRayDescStruct m = new PHRayDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSceneState : CsObject {
        public PHSceneState() { _this = SprExport.Spr_new_PHSceneState(); _flag = true; }
        public PHSceneState(IntPtr ptr) : base(ptr) { }
        public PHSceneState(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSceneState() { if (_flag) { SprExport.Spr_delete_PHSceneState(_this); _flag = false; } }
        public double timeStep {
            get { return SprExport.Spr_PHSceneState_get_timeStep(_this); }
            set { SprExport.Spr_PHSceneState_set_timeStep(_this, value); }
        }
        public double haptictimeStep {
            get { return SprExport.Spr_PHSceneState_get_haptictimeStep(_this); }
            set { SprExport.Spr_PHSceneState_set_haptictimeStep(_this, value); }
        }
        public uint count {
            get { return SprExport.Spr_PHSceneState_get_count(_this); }
            set { SprExport.Spr_PHSceneState_set_count(_this, value); }
        }
        public void Init() {
            SprExport.Spr_PHSceneState_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHSceneStateStruct : CsObject {
        public PHSceneStateStruct() {
            PHSceneState desc = new PHSceneState();
            ApplyFrom(desc, false);
        }
        public double timeStep;
        public double haptictimeStep;
        public uint count;
        public void ApplyTo(PHSceneState r, bool apply_to_base_class = false) {
            r.timeStep = timeStep;
            r.haptictimeStep = haptictimeStep;
            r.count = count;
        }
        public void ApplyFrom(PHSceneState r, bool apply_to_base_class = false) {
            timeStep = r.timeStep;
            haptictimeStep = r.haptictimeStep;
            count = r.count;
        }
        public static implicit operator PHSceneState(PHSceneStateStruct m) {
            PHSceneState r = new PHSceneState();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSceneStateStruct(PHSceneState r) {
            PHSceneStateStruct m = new PHSceneStateStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSceneDesc : PHSceneState {
        public PHSceneDesc() { _this = SprExport.Spr_new_PHSceneDesc(); _flag = true; }
        public PHSceneDesc(IntPtr ptr) : base(ptr) { }
        public PHSceneDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSceneDesc() { if (_flag) { SprExport.Spr_delete_PHSceneDesc(_this); _flag = false; } }
        public Vec3d gravity {
            get { return new Vec3d(SprExport.Spr_PHSceneDesc_addr_gravity(_this)); }
            set { SprExport.Spr_PHSceneDesc_set_gravity(_this, value); }
        }
        public double airResistanceRateForVelocity {
            get { return SprExport.Spr_PHSceneDesc_get_airResistanceRateForVelocity(_this); }
            set { SprExport.Spr_PHSceneDesc_set_airResistanceRateForVelocity(_this, value); }
        }
        public double airResistanceRateForAngularVelocity {
            get { return SprExport.Spr_PHSceneDesc_get_airResistanceRateForAngularVelocity(_this); }
            set { SprExport.Spr_PHSceneDesc_set_airResistanceRateForAngularVelocity(_this, value); }
        }
        public double contactTolerance {
            get { return SprExport.Spr_PHSceneDesc_get_contactTolerance(_this); }
            set { SprExport.Spr_PHSceneDesc_set_contactTolerance(_this, value); }
        }
        public double impactThreshold {
            get { return SprExport.Spr_PHSceneDesc_get_impactThreshold(_this); }
            set { SprExport.Spr_PHSceneDesc_set_impactThreshold(_this, value); }
        }
        public double frictionThreshold {
            get { return SprExport.Spr_PHSceneDesc_get_frictionThreshold(_this); }
            set { SprExport.Spr_PHSceneDesc_set_frictionThreshold(_this, value); }
        }
        public double maxDeltaPosition {
            get { return SprExport.Spr_PHSceneDesc_get_maxDeltaPosition(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxDeltaPosition(_this, value); }
        }
        public double maxDeltaOrientation {
            get { return SprExport.Spr_PHSceneDesc_get_maxDeltaOrientation(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxDeltaOrientation(_this, value); }
        }
        public double maxVelocity {
            get { return SprExport.Spr_PHSceneDesc_get_maxVelocity(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxVelocity(_this, value); }
        }
        public double maxAngularVelocity {
            get { return SprExport.Spr_PHSceneDesc_get_maxAngularVelocity(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxAngularVelocity(_this, value); }
        }
        public double maxForce {
            get { return SprExport.Spr_PHSceneDesc_get_maxForce(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxForce(_this, value); }
        }
        public double maxMoment {
            get { return SprExport.Spr_PHSceneDesc_get_maxMoment(_this); }
            set { SprExport.Spr_PHSceneDesc_set_maxMoment(_this, value); }
        }
        public int numIteration {
            get { return SprExport.Spr_PHSceneDesc_get_numIteration(_this); }
            set { SprExport.Spr_PHSceneDesc_set_numIteration(_this, value); }
        }
        public int method {
            get { return SprExport.Spr_PHSceneDesc_get_method(_this); }
            set { SprExport.Spr_PHSceneDesc_set_method(_this, value); }
        }
        public bool bContactDetectionEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHSceneDesc_get_bContactDetectionEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSceneDesc_set_bContactDetectionEnabled(_this, (char)val);
            }
        }
        public bool bCCDEnabled {
            get {
                byte ret = (byte)SprExport.Spr_PHSceneDesc_get_bCCDEnabled(_this);
                return (ret == 0) ? false : true;
            }
            set {
                byte val = (byte)(value ? 1 : 0);
                SprExport.Spr_PHSceneDesc_set_bCCDEnabled(_this, (char)val);
            }
        }
        public int broadPhaseMode {
            get { return SprExport.Spr_PHSceneDesc_get_broadPhaseMode(_this); }
            set { SprExport.Spr_PHSceneDesc_set_broadPhaseMode(_this, value); }
        }
        public int blendMode {
            get { return SprExport.Spr_PHSceneDesc_get_blendMode(_this); }
            set { SprExport.Spr_PHSceneDesc_set_blendMode(_this, value); }
        }
        public enum BroadPhaseMode : int {
            MODE_SORT_AND_SWEEP_X = 0, MODE_SORT_AND_SWEEP_Y = MODE_SORT_AND_SWEEP_X + 1, MODE_SORT_AND_SWEEP_Z = MODE_SORT_AND_SWEEP_Y + 1, MODE_PARTITION = MODE_SORT_AND_SWEEP_Z + 1
        }
        public enum ContactMode : int {
            MODE_NONE = 0, MODE_PENALTY = MODE_NONE + 1, MODE_LCP = MODE_PENALTY + 1
        }
        public enum MatrialBlending : int {
            BLEND_MIN = 0, BLEND_MAX = BLEND_MIN + 1, BLEND_AVE_ADD = BLEND_MAX + 1, BLEND_AVE_MUL = BLEND_AVE_ADD + 1
        }
        public enum LCPSolver : int {
            SOLVER_GS = 0, SOLVER_CG = SOLVER_GS + 1
        }
        public void Init() {
            SprExport.Spr_PHSceneDesc_Init((IntPtr)_this);
        }
    }
    [System.Serializable]
    public class PHSceneDescStruct : PHSceneStateStruct {
        public PHSceneDescStruct() {
            PHSceneDesc desc = new PHSceneDesc();
            ApplyFrom(desc, false);
        }
        public Vec3dStruct gravity;
        public double airResistanceRateForVelocity;
        public double airResistanceRateForAngularVelocity;
        public double contactTolerance;
        public double impactThreshold;
        public double frictionThreshold;
        public double maxDeltaPosition;
        public double maxDeltaOrientation;
        public double maxVelocity;
        public double maxAngularVelocity;
        public double maxForce;
        public double maxMoment;
        public int numIteration;
        public int method;
        public bool bContactDetectionEnabled;
        public bool bCCDEnabled;
        public int broadPhaseMode;
        public int blendMode;
        public void ApplyTo(PHSceneDesc r, bool apply_to_base_class = false) {
            r.gravity = gravity;
            r.airResistanceRateForVelocity = airResistanceRateForVelocity;
            r.airResistanceRateForAngularVelocity = airResistanceRateForAngularVelocity;
            r.contactTolerance = contactTolerance;
            r.impactThreshold = impactThreshold;
            r.frictionThreshold = frictionThreshold;
            r.maxDeltaPosition = maxDeltaPosition;
            r.maxDeltaOrientation = maxDeltaOrientation;
            r.maxVelocity = maxVelocity;
            r.maxAngularVelocity = maxAngularVelocity;
            r.maxForce = maxForce;
            r.maxMoment = maxMoment;
            r.numIteration = numIteration;
            r.method = method;
            r.bContactDetectionEnabled = bContactDetectionEnabled;
            r.bCCDEnabled = bCCDEnabled;
            r.broadPhaseMode = broadPhaseMode;
            r.blendMode = blendMode;
        }
        public void ApplyFrom(PHSceneDesc r, bool apply_to_base_class = false) {
            gravity = r.gravity;
            airResistanceRateForVelocity = r.airResistanceRateForVelocity;
            airResistanceRateForAngularVelocity = r.airResistanceRateForAngularVelocity;
            contactTolerance = r.contactTolerance;
            impactThreshold = r.impactThreshold;
            frictionThreshold = r.frictionThreshold;
            maxDeltaPosition = r.maxDeltaPosition;
            maxDeltaOrientation = r.maxDeltaOrientation;
            maxVelocity = r.maxVelocity;
            maxAngularVelocity = r.maxAngularVelocity;
            maxForce = r.maxForce;
            maxMoment = r.maxMoment;
            numIteration = r.numIteration;
            method = r.method;
            bContactDetectionEnabled = r.bContactDetectionEnabled;
            bCCDEnabled = r.bCCDEnabled;
            broadPhaseMode = r.broadPhaseMode;
            blendMode = r.blendMode;
        }
        public static implicit operator PHSceneDesc(PHSceneDescStruct m) {
            PHSceneDesc r = new PHSceneDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSceneDescStruct(PHSceneDesc r) {
            PHSceneDescStruct m = new PHSceneDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSdkDesc : CsObject {
        public PHSdkDesc() { _this = SprExport.Spr_new_PHSdkDesc(); _flag = true; }
        public PHSdkDesc(IntPtr ptr) : base(ptr) { }
        public PHSdkDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSdkDesc() { if (_flag) { SprExport.Spr_delete_PHSdkDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHSdkDescStruct : CsObject {
        public PHSdkDescStruct() {
            PHSdkDesc desc = new PHSdkDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHSdkDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHSdkDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHSdkDesc(PHSdkDescStruct m) {
            PHSdkDesc r = new PHSdkDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSdkDescStruct(PHSdkDesc r) {
            PHSdkDescStruct m = new PHSdkDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHBoneDesc : CsObject {
        public PHBoneDesc() { _this = SprExport.Spr_new_PHBoneDesc(); _flag = true; }
        public PHBoneDesc(IntPtr ptr) : base(ptr) { }
        public PHBoneDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHBoneDesc() { if (_flag) { SprExport.Spr_delete_PHBoneDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHBoneDescStruct : CsObject {
        public PHBoneDescStruct() {
            PHBoneDesc desc = new PHBoneDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHBoneDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHBoneDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHBoneDesc(PHBoneDescStruct m) {
            PHBoneDesc r = new PHBoneDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHBoneDescStruct(PHBoneDesc r) {
            PHBoneDescStruct m = new PHBoneDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHSkeletonDesc : CsObject {
        public PHSkeletonDesc() { _this = SprExport.Spr_new_PHSkeletonDesc(); _flag = true; }
        public PHSkeletonDesc(IntPtr ptr) : base(ptr) { }
        public PHSkeletonDesc(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~PHSkeletonDesc() { if (_flag) { SprExport.Spr_delete_PHSkeletonDesc(_this); _flag = false; } }
    }
    [System.Serializable]
    public class PHSkeletonDescStruct : CsObject {
        public PHSkeletonDescStruct() {
            PHSkeletonDesc desc = new PHSkeletonDesc();
            ApplyFrom(desc, false);
        }
        public void ApplyTo(PHSkeletonDesc r, bool apply_to_base_class = false) {
        }
        public void ApplyFrom(PHSkeletonDesc r, bool apply_to_base_class = false) {
        }
        public static implicit operator PHSkeletonDesc(PHSkeletonDescStruct m) {
            PHSkeletonDesc r = new PHSkeletonDesc();
            m.ApplyTo(r, true);
            return r;
        }
        public static implicit operator PHSkeletonDescStruct(PHSkeletonDesc r) {
            PHSkeletonDescStruct m = new PHSkeletonDescStruct();
            m.ApplyFrom(r, true);
            return m;
        }
    }
    public partial class PHEngineIf : SceneObjectIf {
        public PHEngineIf() { _this = SprExport.Spr_new_PHEngineIf(); _flag = true; }
        public PHEngineIf(IntPtr ptr) : base(ptr) { }
        public PHEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHEngineIf.GetIfInfoStatic()) ? new PHEngineIf(target._this, target._flag) : null);
        }
        ~PHEngineIf() { if (_flag) { SprExport.Spr_delete_PHEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int GetPriority() {
            int result = (int)SprExport.Spr_PHEngineIf_GetPriority((IntPtr)_this);
            return result;
        }
        public void Step() {
            SprExport.Spr_PHEngineIf_Step((IntPtr)_this);
        }
        public void Enable(bool on) {
            SprExport.Spr_PHEngineIf_Enable((IntPtr)_this, (bool)on);
        }
        public void Enable() {
            SprExport.Spr_PHEngineIf_Enable_1((IntPtr)_this);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHEngineIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PHSceneIf GetScene() {
            IntPtr ptr = SprExport.Spr_PHEngineIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSceneIf obj = new PHSceneIf(ptr);
            return obj;
        }
    }
    public partial class PHConstraintEngineIf : PHEngineIf {
        public PHConstraintEngineIf() { _this = SprExport.Spr_new_PHConstraintEngineIf(); _flag = true; }
        public PHConstraintEngineIf(IntPtr ptr) : base(ptr) { }
        public PHConstraintEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHConstraintEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHConstraintEngineIf.GetIfInfoStatic()) ? new PHConstraintEngineIf(target._this, target._flag) : null);
        }
        ~PHConstraintEngineIf() { if (_flag) { SprExport.Spr_delete_PHConstraintEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHConstraintEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHConstraintEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHConstraintEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHConstraintEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHConstraintsIf GetContactPoints() {
            IntPtr ptr = SprExport.Spr_PHConstraintEngineIf_GetContactPoints((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHConstraintsIf obj = new PHConstraintsIf(ptr);
            return obj;
        }
        public void SetVelCorrectionRate(double value) {
            SprExport.Spr_PHConstraintEngineIf_SetVelCorrectionRate((IntPtr)_this, (double)value);
        }
        public double GetVelCorrectionRate() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetVelCorrectionRate((IntPtr)_this);
            return result;
        }
        public void SetPosCorrectionRate(double value) {
            SprExport.Spr_PHConstraintEngineIf_SetPosCorrectionRate((IntPtr)_this, (double)value);
        }
        public double GetPosCorrectionRate() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetPosCorrectionRate((IntPtr)_this);
            return result;
        }
        public void SetContactCorrectionRate(double value) {
            SprExport.Spr_PHConstraintEngineIf_SetContactCorrectionRate((IntPtr)_this, (double)value);
        }
        public double GetContactCorrectionRate() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetContactCorrectionRate((IntPtr)_this);
            return result;
        }
        public void SetBSaveConstraints(bool value) {
            SprExport.Spr_PHConstraintEngineIf_SetBSaveConstraints((IntPtr)_this, (bool)value);
        }
        public void SetUpdateAllSolidState(bool flag) {
            SprExport.Spr_PHConstraintEngineIf_SetUpdateAllSolidState((IntPtr)_this, (bool)flag);
        }
        public void SetUseContactSurface(bool flag) {
            SprExport.Spr_PHConstraintEngineIf_SetUseContactSurface((IntPtr)_this, (bool)flag);
        }
        public void SetShrinkRate(double data) {
            SprExport.Spr_PHConstraintEngineIf_SetShrinkRate((IntPtr)_this, (double)data);
        }
        public double GetShrinkRate() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetShrinkRate((IntPtr)_this);
            return result;
        }
        public void SetShrinkRateCorrection(double data) {
            SprExport.Spr_PHConstraintEngineIf_SetShrinkRateCorrection((IntPtr)_this, (double)data);
        }
        public double GetShrinkRateCorrection() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetShrinkRateCorrection((IntPtr)_this);
            return result;
        }
        public void SetRegularization(double reg) {
            SprExport.Spr_PHConstraintEngineIf_SetRegularization((IntPtr)_this, (double)reg);
        }
        public double GetRegularization() {
            double result = (double)SprExport.Spr_PHConstraintEngineIf_GetRegularization((IntPtr)_this);
            return result;
        }
        public void EnableRenderContact(bool enable) {
            SprExport.Spr_PHConstraintEngineIf_EnableRenderContact((IntPtr)_this, (bool)enable);
        }
    }
    public partial class PHGravityEngineIf : PHEngineIf {
        public PHGravityEngineIf() { _this = SprExport.Spr_new_PHGravityEngineIf(); _flag = true; }
        public PHGravityEngineIf(IntPtr ptr) : base(ptr) { }
        public PHGravityEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHGravityEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHGravityEngineIf.GetIfInfoStatic()) ? new PHGravityEngineIf(target._this, target._flag) : null);
        }
        ~PHGravityEngineIf() { if (_flag) { SprExport.Spr_delete_PHGravityEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHGravityEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHGravityEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHGravityEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHGravityEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPenaltyEngineIf : PHEngineIf {
        public PHPenaltyEngineIf() { _this = SprExport.Spr_new_PHPenaltyEngineIf(); _flag = true; }
        public PHPenaltyEngineIf(IntPtr ptr) : base(ptr) { }
        public PHPenaltyEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPenaltyEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHPenaltyEngineIf.GetIfInfoStatic()) ? new PHPenaltyEngineIf(target._this, target._flag) : null);
        }
        ~PHPenaltyEngineIf() { if (_flag) { SprExport.Spr_delete_PHPenaltyEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPenaltyEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPenaltyEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPenaltyEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPenaltyEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHIKEngineIf : PHEngineIf {
        public PHIKEngineIf() { _this = SprExport.Spr_new_PHIKEngineIf(); _flag = true; }
        public PHIKEngineIf(IntPtr ptr) : base(ptr) { }
        public PHIKEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHIKEngineIf.GetIfInfoStatic()) ? new PHIKEngineIf(target._this, target._flag) : null);
        }
        ~PHIKEngineIf() { if (_flag) { SprExport.Spr_delete_PHIKEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public void SetMaxVelocity(double maxVel) {
            SprExport.Spr_PHIKEngineIf_SetMaxVelocity((IntPtr)_this, (double)maxVel);
        }
        public double GetMaxVelocity() {
            double result = (double)SprExport.Spr_PHIKEngineIf_GetMaxVelocity((IntPtr)_this);
            return result;
        }
        public void SetMaxAngularVelocity(double maxAV) {
            SprExport.Spr_PHIKEngineIf_SetMaxAngularVelocity((IntPtr)_this, (double)maxAV);
        }
        public double GetMaxAngularVelocity() {
            double result = (double)SprExport.Spr_PHIKEngineIf_GetMaxAngularVelocity((IntPtr)_this);
            return result;
        }
        public void SetMaxActuatorVelocity(double maxAV) {
            SprExport.Spr_PHIKEngineIf_SetMaxActuatorVelocity((IntPtr)_this, (double)maxAV);
        }
        public double GetMaxActuatorVelocity() {
            double result = (double)SprExport.Spr_PHIKEngineIf_GetMaxActuatorVelocity((IntPtr)_this);
            return result;
        }
        public void SetRegularizeParam(double epsilon) {
            SprExport.Spr_PHIKEngineIf_SetRegularizeParam((IntPtr)_this, (double)epsilon);
        }
        public double GetRegularizeParam() {
            double result = (double)SprExport.Spr_PHIKEngineIf_GetRegularizeParam((IntPtr)_this);
            return result;
        }
        public void SetIterCutOffAngVel(double epsilon) {
            SprExport.Spr_PHIKEngineIf_SetIterCutOffAngVel((IntPtr)_this, (double)epsilon);
        }
        public double GetIterCutOffAngVel() {
            double result = (double)SprExport.Spr_PHIKEngineIf_GetIterCutOffAngVel((IntPtr)_this);
            return result;
        }
        public void SetIntpRate() {
            SprExport.Spr_PHIKEngineIf_SetIntpRate((IntPtr)_this);
        }
        public int GetIntpRate() {
            int result = (int)SprExport.Spr_PHIKEngineIf_GetIntpRate((IntPtr)_this);
            return result;
        }
        public void ApplyExactState(bool reverse) {
            SprExport.Spr_PHIKEngineIf_ApplyExactState((IntPtr)_this, (bool)reverse);
        }
        public void ApplyExactState() {
            SprExport.Spr_PHIKEngineIf_ApplyExactState_1((IntPtr)_this);
        }
        public void SetNumIter(int numIter) {
            SprExport.Spr_PHIKEngineIf_SetNumIter((IntPtr)_this, (int)numIter);
        }
        public int GetNumIter() {
            int result = (int)SprExport.Spr_PHIKEngineIf_GetNumIter((IntPtr)_this);
            return result;
        }
        public void FK() {
            SprExport.Spr_PHIKEngineIf_FK((IntPtr)_this);
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHFemEngineIf : PHEngineIf {
        public PHFemEngineIf() { _this = SprExport.Spr_new_PHFemEngineIf(); _flag = true; }
        public PHFemEngineIf(IntPtr ptr) : base(ptr) { }
        public PHFemEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHFemEngineIf.GetIfInfoStatic()) ? new PHFemEngineIf(target._this, target._flag) : null);
        }
        ~PHFemEngineIf() { if (_flag) { SprExport.Spr_delete_PHFemEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetTimeStep(double dt) {
            SprExport.Spr_PHFemEngineIf_SetTimeStep((IntPtr)_this, (double)dt);
        }
        public double GetTimeStep() {
            double result = (double)SprExport.Spr_PHFemEngineIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public void SetVibrationTransfer(bool bEnable) {
            SprExport.Spr_PHFemEngineIf_SetVibrationTransfer((IntPtr)_this, (bool)bEnable);
        }
        public void SetThermalTransfer(bool bEnable) {
            SprExport.Spr_PHFemEngineIf_SetThermalTransfer((IntPtr)_this, (bool)bEnable);
        }
        public int NMeshNew() {
            int result = (int)SprExport.Spr_PHFemEngineIf_NMeshNew((IntPtr)_this);
            return result;
        }
        public PHFemMeshNewIf GetMeshNew(int i) {
            IntPtr ptr = SprExport.Spr_PHFemEngineIf_GetMeshNew((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemMeshNewIf obj = new PHFemMeshNewIf(ptr);
            return obj;
        }
        public bool AddMeshPair(PHFemMeshNewIf m0, PHFemMeshNewIf m1) {
            char ret = SprExport.Spr_PHFemEngineIf_AddMeshPair((IntPtr)_this, (IntPtr)m0, (IntPtr)m1);
            return (ret == 0) ? false : true;
        }
        public bool RemoveMeshPair(PHFemMeshNewIf m0, PHFemMeshNewIf m1) {
            char ret = SprExport.Spr_PHFemEngineIf_RemoveMeshPair((IntPtr)_this, (IntPtr)m0, (IntPtr)m1);
            return (ret == 0) ? false : true;
        }
        public void ThermalTransfer() {
            SprExport.Spr_PHFemEngineIf_ThermalTransfer((IntPtr)_this);
        }
        public void setheatTransferRatio(double setheatTransferRatio) {
            SprExport.Spr_PHFemEngineIf_setheatTransferRatio((IntPtr)_this, (double)setheatTransferRatio);
        }
        public void FEMSolidMatchRefresh() {
            SprExport.Spr_PHFemEngineIf_FEMSolidMatchRefresh((IntPtr)_this);
        }
        public void InitContacts() {
            SprExport.Spr_PHFemEngineIf_InitContacts((IntPtr)_this);
        }
        public void ClearContactVectors() {
            SprExport.Spr_PHFemEngineIf_ClearContactVectors((IntPtr)_this);
        }
        public int NMesh() {
            int result = (int)SprExport.Spr_PHFemEngineIf_NMesh((IntPtr)_this);
            return result;
        }
        public PHFemMeshIf GetMesh(int i) {
            IntPtr ptr = SprExport.Spr_PHFemEngineIf_GetMesh((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemMeshIf obj = new PHFemMeshIf(ptr);
            if (obj.GetIfInfo() == PHFemMeshThermoIf.GetIfInfoStatic()) { return new PHFemMeshThermoIf(ptr); }
            return obj;
        }
    }
    public partial class PHOpEngineIf : PHEngineIf {
        public PHOpEngineIf() { _this = SprExport.Spr_new_PHOpEngineIf(); _flag = true; }
        public PHOpEngineIf(IntPtr ptr) : base(ptr) { }
        public PHOpEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHOpEngineIf.GetIfInfoStatic()) ? new PHOpEngineIf(target._this, target._flag) : null);
        }
        ~PHOpEngineIf() { if (_flag) { SprExport.Spr_delete_PHOpEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetTimeStep(double dt) {
            SprExport.Spr_PHOpEngineIf_SetTimeStep((IntPtr)_this, (double)dt);
        }
        public double GetTimeStep() {
            double result = (double)SprExport.Spr_PHOpEngineIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public void SetGravity(bool gflag) {
            SprExport.Spr_PHOpEngineIf_SetGravity((IntPtr)_this, (bool)gflag);
        }
        public void InitialHapticRenderer(int objId) {
            SprExport.Spr_PHOpEngineIf_InitialHapticRenderer((IntPtr)_this, (int)objId);
        }
        public arraywrapper_PHOpObjDesc GetOpObj(int i) {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetOpObj((IntPtr)_this, (int)i);
            return new arraywrapper_PHOpObjDesc(ptr);
        }
        public void StepWithBlend() {
            SprExport.Spr_PHOpEngineIf_StepWithBlend((IntPtr)_this);
        }
        public int AddOpObj() {
            int result = (int)SprExport.Spr_PHOpEngineIf_AddOpObj((IntPtr)_this);
            return result;
        }
        public void SetHapticSolveEnable(bool enable) {
            SprExport.Spr_PHOpEngineIf_SetHapticSolveEnable((IntPtr)_this, (bool)enable);
        }
        public bool IsHapticSolve() {
            char ret = SprExport.Spr_PHOpEngineIf_IsHapticSolve((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetProxyCorrectionEnable(bool enable) {
            SprExport.Spr_PHOpEngineIf_SetProxyCorrectionEnable((IntPtr)_this, (bool)enable);
        }
        public bool IsProxyCorrection() {
            char ret = SprExport.Spr_PHOpEngineIf_IsProxyCorrection((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetUpdateNormal(bool flag) {
            SprExport.Spr_PHOpEngineIf_SetUpdateNormal((IntPtr)_this, (bool)flag);
        }
        public bool IsUpdateNormal(int obji) {
            char ret = SprExport.Spr_PHOpEngineIf_IsUpdateNormal((IntPtr)_this, (int)obji);
            return (ret == 0) ? false : true;
        }
        public void SetUseHaptic(bool hapticUsage) {
            SprExport.Spr_PHOpEngineIf_SetUseHaptic((IntPtr)_this, (bool)hapticUsage);
        }
        public bool GetUseHaptic() {
            char ret = SprExport.Spr_PHOpEngineIf_GetUseHaptic((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PHOpObjIf GetOpObjIf(int obji) {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetOpObjIf((IntPtr)_this, (int)obji);
            if (ptr == IntPtr.Zero) { return null; }
            PHOpObjIf obj = new PHOpObjIf(ptr);
            return obj;
        }
        public int GetOpObjNum() {
            int result = (int)SprExport.Spr_PHOpEngineIf_GetOpObjNum((IntPtr)_this);
            return result;
        }
        public ObjectIf GetOpAnimator() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetOpAnimator((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == PHSolidPairIf.GetIfInfoStatic()) { return new PHSolidPairIf(ptr); }
            if (obj.GetIfInfo() == PHOpParticleIf.GetIfInfoStatic()) { return new PHOpParticleIf(ptr); }
            if (obj.GetIfInfo() == PHOpGroupIf.GetIfInfoStatic()) { return new PHOpGroupIf(ptr); }
            if (obj.GetIfInfo() == PHOpSpHashColliAgentIf.GetIfInfoStatic()) { return new PHOpSpHashColliAgentIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForLCPIf.GetIfInfoStatic()) { return new PHSolidPairForLCPIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForHapticIf.GetIfInfoStatic()) { return new PHSolidPairForHapticIf(ptr); }
            return obj;
        }
        public ObjectIf GetOpHapticController() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetOpHapticController((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == PHSolidPairIf.GetIfInfoStatic()) { return new PHSolidPairIf(ptr); }
            if (obj.GetIfInfo() == PHOpParticleIf.GetIfInfoStatic()) { return new PHOpParticleIf(ptr); }
            if (obj.GetIfInfo() == PHOpGroupIf.GetIfInfoStatic()) { return new PHOpGroupIf(ptr); }
            if (obj.GetIfInfo() == PHOpSpHashColliAgentIf.GetIfInfoStatic()) { return new PHOpSpHashColliAgentIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForLCPIf.GetIfInfoStatic()) { return new PHSolidPairForLCPIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForHapticIf.GetIfInfoStatic()) { return new PHSolidPairForHapticIf(ptr); }
            return obj;
        }
        public ObjectIf GetOpHapticRenderer() {
            IntPtr ptr = SprExport.Spr_PHOpEngineIf_GetOpHapticRenderer((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == PHSolidPairIf.GetIfInfoStatic()) { return new PHSolidPairIf(ptr); }
            if (obj.GetIfInfo() == PHOpParticleIf.GetIfInfoStatic()) { return new PHOpParticleIf(ptr); }
            if (obj.GetIfInfo() == PHOpGroupIf.GetIfInfoStatic()) { return new PHOpGroupIf(ptr); }
            if (obj.GetIfInfo() == PHOpSpHashColliAgentIf.GetIfInfoStatic()) { return new PHOpSpHashColliAgentIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForLCPIf.GetIfInfoStatic()) { return new PHSolidPairForLCPIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForHapticIf.GetIfInfoStatic()) { return new PHSolidPairForHapticIf(ptr); }
            return obj;
        }
        public void SetIterationCount(int count) {
            SprExport.Spr_PHOpEngineIf_SetIterationCount((IntPtr)_this, (int)count);
        }
        public int GetIterationCount() {
            int result = (int)SprExport.Spr_PHOpEngineIf_GetIterationCount((IntPtr)_this);
            return result;
        }
        public void SetAnimationFlag(bool flag) {
            SprExport.Spr_PHOpEngineIf_SetAnimationFlag((IntPtr)_this, (bool)flag);
        }
        public bool GetAnimationFlag() {
            char ret = SprExport.Spr_PHOpEngineIf_GetAnimationFlag((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetDrawPtclR(float r) {
            SprExport.Spr_PHOpEngineIf_SetDrawPtclR((IntPtr)_this, (float)r);
        }
        public float GetDrawPtclR() {
            float result = (float)SprExport.Spr_PHOpEngineIf_GetDrawPtclR((IntPtr)_this);
            return result;
        }
        public void InitialNoMeshHapticRenderer() {
            SprExport.Spr_PHOpEngineIf_InitialNoMeshHapticRenderer((IntPtr)_this);
        }
    }
    public partial class PHFemMeshIf : SceneObjectIf {
        public PHFemMeshIf() { _this = SprExport.Spr_new_PHFemMeshIf(); _flag = true; }
        public PHFemMeshIf(IntPtr ptr) : base(ptr) { }
        public PHFemMeshIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemMeshIf(CsCastObject target) {
            return (target._info.Inherit(PHFemMeshIf.GetIfInfoStatic()) ? new PHFemMeshIf(target._this, target._flag) : null);
        }
        ~PHFemMeshIf() { if (_flag) { SprExport.Spr_delete_PHFemMeshIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemMeshIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemMeshIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemMeshIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemMeshIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHFemMeshThermoIf : PHFemMeshIf {
        public PHFemMeshThermoIf() { _this = SprExport.Spr_new_PHFemMeshThermoIf(); _flag = true; }
        public PHFemMeshThermoIf(IntPtr ptr) : base(ptr) { }
        public PHFemMeshThermoIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemMeshThermoIf(CsCastObject target) {
            return (target._info.Inherit(PHFemMeshThermoIf.GetIfInfoStatic()) ? new PHFemMeshThermoIf(target._this, target._flag) : null);
        }
        ~PHFemMeshThermoIf() { if (_flag) { SprExport.Spr_delete_PHFemMeshThermoIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemMeshThermoIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int GetSurfaceVertex(int id) {
            int result = (int)SprExport.Spr_PHFemMeshThermoIf_GetSurfaceVertex((IntPtr)_this, (int)id);
            return result;
        }
        public int NSurfaceVertices() {
            int result = (int)SprExport.Spr_PHFemMeshThermoIf_NSurfaceVertices((IntPtr)_this);
            return result;
        }
        public void SetVertexTc(int id, double temp) {
            SprExport.Spr_PHFemMeshThermoIf_SetVertexTc((IntPtr)_this, (int)id, (double)temp);
        }
        public void SetVertexTc(int id, double temp, double heatTrans) {
            SprExport.Spr_PHFemMeshThermoIf_SetVertexTc_1((IntPtr)_this, (int)id, (double)temp, (double)heatTrans);
        }
        public Vec3d GetPose(int id) {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetPose((IntPtr)_this, (int)id);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetSufVtxPose(uint id) {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetSufVtxPose((IntPtr)_this, (uint)id);
            return new Vec3d(ptr, true);
        }
        public uint GetStepCount() {
            uint result = (uint)SprExport.Spr_PHFemMeshThermoIf_GetStepCount((IntPtr)_this);
            return result;
        }
        public uint GetStepCountCyc() {
            uint result = (uint)SprExport.Spr_PHFemMeshThermoIf_GetStepCountCyc((IntPtr)_this);
            return result;
        }
        public double GetVertexTemp(uint id) {
            double result = (double)SprExport.Spr_PHFemMeshThermoIf_GetVertexTemp((IntPtr)_this, (uint)id);
            return result;
        }
        public double GetSufVertexTemp(uint id) {
            double result = (double)SprExport.Spr_PHFemMeshThermoIf_GetSufVertexTemp((IntPtr)_this, (uint)id);
            return result;
        }
        public void SetVertexTemp(uint id, double temp) {
            SprExport.Spr_PHFemMeshThermoIf_SetVertexTemp((IntPtr)_this, (uint)id, (double)temp);
        }
        public void SetVerticesTempAll(double temp) {
            SprExport.Spr_PHFemMeshThermoIf_SetVerticesTempAll((IntPtr)_this, (double)temp);
        }
        public void AddvecFAll(uint id, double dqdt) {
            SprExport.Spr_PHFemMeshThermoIf_AddvecFAll((IntPtr)_this, (uint)id, (double)dqdt);
        }
        public void SetvecFAll(uint id, double dqdt) {
            SprExport.Spr_PHFemMeshThermoIf_SetvecFAll((IntPtr)_this, (uint)id, (double)dqdt);
        }
        public void SetRhoSpheat(double rho, double Cp) {
            SprExport.Spr_PHFemMeshThermoIf_SetRhoSpheat((IntPtr)_this, (double)rho, (double)Cp);
        }
        public uint GetNFace() {
            uint result = (uint)SprExport.Spr_PHFemMeshThermoIf_GetNFace((IntPtr)_this);
            return result;
        }
        public vectorwrapper_Vec3d GetFaceEdgeVtx(uint id) {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetFaceEdgeVtx((IntPtr)_this, (uint)id);
            return new vectorwrapper_Vec3d(ptr);
        }
        public Vec3d GetFaceEdgeVtx(uint id, uint vtx) {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetFaceEdgeVtx_1((IntPtr)_this, (uint)id, (uint)vtx);
            return new Vec3d(ptr, true);
        }
        public Vec2d GetIHbandDrawVtx() {
            IntPtr ptr = SprExport.Spr_PHFemMeshThermoIf_GetIHbandDrawVtx((IntPtr)_this);
            return new Vec2d(ptr, true);
        }
        public void CalcIHdqdt_atleast(double r, double R, double dqdtAll, uint num) {
            SprExport.Spr_PHFemMeshThermoIf_CalcIHdqdt_atleast((IntPtr)_this, (double)r, (double)R, (double)dqdtAll, (uint)num);
        }
        public void UpdateIHheatband(double xS, double xE, uint heatingMODE) {
            SprExport.Spr_PHFemMeshThermoIf_UpdateIHheatband((IntPtr)_this, (double)xS, (double)xE, (uint)heatingMODE);
        }
        public void UpdateIHheat(uint heating) {
            SprExport.Spr_PHFemMeshThermoIf_UpdateIHheat((IntPtr)_this, (uint)heating);
        }
        public void UpdateVecF() {
            SprExport.Spr_PHFemMeshThermoIf_UpdateVecF((IntPtr)_this);
        }
        public void UpdateVecF_frypan() {
            SprExport.Spr_PHFemMeshThermoIf_UpdateVecF_frypan((IntPtr)_this);
        }
        public void DecrMoist() {
            SprExport.Spr_PHFemMeshThermoIf_DecrMoist((IntPtr)_this);
        }
        public void DecrMoist_velo(double vel) {
            SprExport.Spr_PHFemMeshThermoIf_DecrMoist_velo((IntPtr)_this, (double)vel);
        }
        public void DecrMoist_vel(double dt) {
            SprExport.Spr_PHFemMeshThermoIf_DecrMoist_vel((IntPtr)_this, (double)dt);
        }
        public void InitAllVertexTemp() {
            SprExport.Spr_PHFemMeshThermoIf_InitAllVertexTemp((IntPtr)_this);
        }
        public void SetInitThermoConductionParam(double thConduct, double rho, double specificHeat, double heatTrans) {
            SprExport.Spr_PHFemMeshThermoIf_SetInitThermoConductionParam((IntPtr)_this, (double)thConduct, (double)rho, (double)specificHeat, (double)heatTrans);
        }
        public void SetParamAndReCreateMatrix(double thConduct0, double roh0, double specificHeat0) {
            SprExport.Spr_PHFemMeshThermoIf_SetParamAndReCreateMatrix((IntPtr)_this, (double)thConduct0, (double)roh0, (double)specificHeat0);
        }
        public double GetArbitraryPointTemp(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemMeshThermoIf_GetArbitraryPointTemp((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public double GetVtxTempInTets(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemMeshThermoIf_GetVtxTempInTets((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public void InitVecFAlls() {
            SprExport.Spr_PHFemMeshThermoIf_InitVecFAlls((IntPtr)_this);
        }
        public double Get_thConduct() {
            double result = (double)SprExport.Spr_PHFemMeshThermoIf_Get_thConduct((IntPtr)_this);
            return result;
        }
        public bool SetConcentricHeatMap(vectorwrapper_double r, vectorwrapper_double temp, Vec2d origin) {
            char ret = SprExport.Spr_PHFemMeshThermoIf_SetConcentricHeatMap((IntPtr)_this, (IntPtr)r, (IntPtr)temp, (IntPtr)origin);
            return (ret == 0) ? false : true;
        }
        public void SetThermalEmissivityToVerticesAll(double thermalEmissivity, double thermalEmissivity_const) {
            SprExport.Spr_PHFemMeshThermoIf_SetThermalEmissivityToVerticesAll((IntPtr)_this, (double)thermalEmissivity, (double)thermalEmissivity_const);
        }
        public void SetOuterTemp(double temp) {
            SprExport.Spr_PHFemMeshThermoIf_SetOuterTemp((IntPtr)_this, (double)temp);
        }
        public void SetThermalRadiation(double ems, double ems_const) {
            SprExport.Spr_PHFemMeshThermoIf_SetThermalRadiation((IntPtr)_this, (double)ems, (double)ems_const);
        }
        public void SetGaussCalcParam(uint cyc, double epsilon) {
            SprExport.Spr_PHFemMeshThermoIf_SetGaussCalcParam((IntPtr)_this, (uint)cyc, (double)epsilon);
        }
        public void InitTcAll(double temp) {
            SprExport.Spr_PHFemMeshThermoIf_InitTcAll((IntPtr)_this, (double)temp);
        }
        public void InitToutAll(double temp) {
            SprExport.Spr_PHFemMeshThermoIf_InitToutAll((IntPtr)_this, (double)temp);
        }
        public void SetWeekPow(double weekPow_) {
            SprExport.Spr_PHFemMeshThermoIf_SetWeekPow((IntPtr)_this, (double)weekPow_);
        }
        public void SetIHParamWEEK(double inr_, double outR_, double weekPow_) {
            SprExport.Spr_PHFemMeshThermoIf_SetIHParamWEEK((IntPtr)_this, (double)inr_, (double)outR_, (double)weekPow_);
        }
        public void SetHeatTransRatioToAllVertex(double heatTransR_) {
            SprExport.Spr_PHFemMeshThermoIf_SetHeatTransRatioToAllVertex((IntPtr)_this, (double)heatTransR_);
        }
        public void AfterSetDesc() {
            SprExport.Spr_PHFemMeshThermoIf_AfterSetDesc((IntPtr)_this);
        }
        public void SetStopTimespan(double timespan) {
            SprExport.Spr_PHFemMeshThermoIf_SetStopTimespan((IntPtr)_this, (double)timespan);
        }
        public void UpdateMatk_RadiantHeatToAir() {
            SprExport.Spr_PHFemMeshThermoIf_UpdateMatk_RadiantHeatToAir((IntPtr)_this);
        }
        public void ActivateVtxbeRadiantHeat() {
            SprExport.Spr_PHFemMeshThermoIf_ActivateVtxbeRadiantHeat((IntPtr)_this);
        }
        public void OutputMatKall() {
            SprExport.Spr_PHFemMeshThermoIf_OutputMatKall((IntPtr)_this);
        }
        public void IfRadiantHeatTrans() {
            SprExport.Spr_PHFemMeshThermoIf_IfRadiantHeatTrans((IntPtr)_this);
        }
    }
    public partial class PHFemMeshNewIf : SceneObjectIf {
        public PHFemMeshNewIf() { _this = SprExport.Spr_new_PHFemMeshNewIf(); _flag = true; }
        public PHFemMeshNewIf(IntPtr ptr) : base(ptr) { }
        public PHFemMeshNewIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemMeshNewIf(CsCastObject target) {
            return (target._info.Inherit(PHFemMeshNewIf.GetIfInfoStatic()) ? new PHFemMeshNewIf(target._this, target._flag) : null);
        }
        ~PHFemMeshNewIf() { if (_flag) { SprExport.Spr_delete_PHFemMeshNewIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemMeshNewIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetPHSolid(PHSolidIf s) {
            SprExport.Spr_PHFemMeshNewIf_SetPHSolid((IntPtr)_this, (IntPtr)s);
        }
        public PHSolidIf GetPHSolid() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetPHSolid((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public PHFemVibrationIf GetPHFemVibration() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetPHFemVibration((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemVibrationIf obj = new PHFemVibrationIf(ptr);
            return obj;
        }
        public PHFemThermoIf GetPHFemThermo() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetPHFemThermo((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemThermoIf obj = new PHFemThermoIf(ptr);
            return obj;
        }
        public PHFemPorousWOMoveIf GetPHFemPorousWOMove() {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetPHFemPorousWOMove((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemPorousWOMoveIf obj = new PHFemPorousWOMoveIf(ptr);
            return obj;
        }
        public int NVertices() {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_NVertices((IntPtr)_this);
            return result;
        }
        public int NFaces() {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_NFaces((IntPtr)_this);
            return result;
        }
        public int NTets() {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_NTets((IntPtr)_this);
            return result;
        }
        public void SetVertexUpdateFlags(bool flg) {
            SprExport.Spr_PHFemMeshNewIf_SetVertexUpdateFlags((IntPtr)_this, (bool)flg);
        }
        public void SetVertexUpateFlag(int vid, bool flg) {
            SprExport.Spr_PHFemMeshNewIf_SetVertexUpateFlag((IntPtr)_this, (int)vid, (bool)flg);
        }
        public double CompTetVolume(int tetID, bool bDeform) {
            double result = (double)SprExport.Spr_PHFemMeshNewIf_CompTetVolume((IntPtr)_this, (int)tetID, (bool)bDeform);
            return result;
        }
        public bool AddVertexDisplacementW(int vtxId, Vec3d disW) {
            char ret = SprExport.Spr_PHFemMeshNewIf_AddVertexDisplacementW((IntPtr)_this, (int)vtxId, (IntPtr)disW);
            return (ret == 0) ? false : true;
        }
        public bool AddVertexDisplacementL(int vtxId, Vec3d disL) {
            char ret = SprExport.Spr_PHFemMeshNewIf_AddVertexDisplacementL((IntPtr)_this, (int)vtxId, (IntPtr)disL);
            return (ret == 0) ? false : true;
        }
        public bool SetVertexPositionW(int vtxId, Vec3d posW) {
            char ret = SprExport.Spr_PHFemMeshNewIf_SetVertexPositionW((IntPtr)_this, (int)vtxId, (IntPtr)posW);
            return (ret == 0) ? false : true;
        }
        public bool SetVertexPositionL(int vtxId, Vec3d posL) {
            char ret = SprExport.Spr_PHFemMeshNewIf_SetVertexPositionL((IntPtr)_this, (int)vtxId, (IntPtr)posL);
            return (ret == 0) ? false : true;
        }
        public bool SetVertexVelocityL(int vtxId, Vec3d posL) {
            char ret = SprExport.Spr_PHFemMeshNewIf_SetVertexVelocityL((IntPtr)_this, (int)vtxId, (IntPtr)posL);
            return (ret == 0) ? false : true;
        }
        public Vec3d GetVertexVelocityL(int vtxId) {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetVertexVelocityL((IntPtr)_this, (int)vtxId);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetVertexPositionL(int vtxId) {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetVertexPositionL((IntPtr)_this, (int)vtxId);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetVertexDisplacementL(int vtxId) {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetVertexDisplacementL((IntPtr)_this, (int)vtxId);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetVertexInitalPositionL(int vtxId) {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetVertexInitalPositionL((IntPtr)_this, (int)vtxId);
            return new Vec3d(ptr, true);
        }
        public void SetVelocity(Vec3d v) {
            SprExport.Spr_PHFemMeshNewIf_SetVelocity((IntPtr)_this, (IntPtr)v);
        }
        public int GetTetVertexIds(int t) {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_GetTetVertexIds((IntPtr)_this, (int)t);
            return result;
        }
        public int GetFaceVertexIds(int f) {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_GetFaceVertexIds((IntPtr)_this, (int)f);
            return result;
        }
        public Vec3d GetFaceNormal(int f) {
            IntPtr ptr = SprExport.Spr_PHFemMeshNewIf_GetFaceNormal((IntPtr)_this, (int)f);
            return new Vec3d(ptr, true);
        }
        public int GetSurfaceVertex(int i) {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_GetSurfaceVertex((IntPtr)_this, (int)i);
            return result;
        }
        public int NSurfaceVertices() {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_NSurfaceVertices((IntPtr)_this);
            return result;
        }
        public int NSurfaceFace() {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_NSurfaceFace((IntPtr)_this);
            return result;
        }
        public bool CompTetShapeFunctionValue(int tetId, Vec3d posL, Vec4d value, bool bDeform) {
            char ret = SprExport.Spr_PHFemMeshNewIf_CompTetShapeFunctionValue((IntPtr)_this, (int)tetId, (IntPtr)posL, (IntPtr)value, (bool)bDeform);
            return (ret == 0) ? false : true;
        }
        public int FindTetFromFace(int faceId) {
            int result = (int)SprExport.Spr_PHFemMeshNewIf_FindTetFromFace((IntPtr)_this, (int)faceId);
            return result;
        }
    }
    public partial class PHFemBaseIf : SceneObjectIf {
        public PHFemBaseIf() { _this = SprExport.Spr_new_PHFemBaseIf(); _flag = true; }
        public PHFemBaseIf(IntPtr ptr) : base(ptr) { }
        public PHFemBaseIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemBaseIf(CsCastObject target) {
            return (target._info.Inherit(PHFemBaseIf.GetIfInfoStatic()) ? new PHFemBaseIf(target._this, target._flag) : null);
        }
        ~PHFemBaseIf() { if (_flag) { SprExport.Spr_delete_PHFemBaseIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemBaseIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemBaseIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemBaseIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemBaseIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHFemMeshNewIf GetPHFemMesh() {
            IntPtr ptr = SprExport.Spr_PHFemBaseIf_GetPHFemMesh((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemMeshNewIf obj = new PHFemMeshNewIf(ptr);
            return obj;
        }
    }
    public partial class PHFemVibrationIf : PHFemBaseIf {
        public PHFemVibrationIf() { _this = SprExport.Spr_new_PHFemVibrationIf(); _flag = true; }
        public PHFemVibrationIf(IntPtr ptr) : base(ptr) { }
        public PHFemVibrationIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemVibrationIf(CsCastObject target) {
            return (target._info.Inherit(PHFemVibrationIf.GetIfInfoStatic()) ? new PHFemVibrationIf(target._this, target._flag) : null);
        }
        ~PHFemVibrationIf() { if (_flag) { SprExport.Spr_delete_PHFemVibrationIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemVibrationIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemVibrationIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemVibrationIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemVibrationIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetTimeStep(double dt) {
            SprExport.Spr_PHFemVibrationIf_SetTimeStep((IntPtr)_this, (double)dt);
        }
        public double GetTimeStep() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public void SetYoungModulus(double value) {
            SprExport.Spr_PHFemVibrationIf_SetYoungModulus((IntPtr)_this, (double)value);
        }
        public double GetYoungModulus() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetYoungModulus((IntPtr)_this);
            return result;
        }
        public void SetPoissonsRatio(double value) {
            SprExport.Spr_PHFemVibrationIf_SetPoissonsRatio((IntPtr)_this, (double)value);
        }
        public double GetPoissonsRatio() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetPoissonsRatio((IntPtr)_this);
            return result;
        }
        public void SetDensity(double value) {
            SprExport.Spr_PHFemVibrationIf_SetDensity((IntPtr)_this, (double)value);
        }
        public double GetDensity() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetDensity((IntPtr)_this);
            return result;
        }
        public void SetAlpha(double value) {
            SprExport.Spr_PHFemVibrationIf_SetAlpha((IntPtr)_this, (double)value);
        }
        public double GetAlpha() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetAlpha((IntPtr)_this);
            return result;
        }
        public void SetBeta(double value) {
            SprExport.Spr_PHFemVibrationIf_SetBeta((IntPtr)_this, (double)value);
        }
        public double GetBeta() {
            double result = (double)SprExport.Spr_PHFemVibrationIf_GetBeta((IntPtr)_this);
            return result;
        }
        public void SetBoundary(int vtxIds) {
            SprExport.Spr_PHFemVibrationIf_SetBoundary((IntPtr)_this, (int)vtxIds);
        }
        public void ClearBoundary() {
            SprExport.Spr_PHFemVibrationIf_ClearBoundary((IntPtr)_this);
        }
        public vectorwrapper_int GetBoundary() {
            IntPtr ptr = SprExport.Spr_PHFemVibrationIf_GetBoundary((IntPtr)_this);
            return new vectorwrapper_int(ptr);
        }
        public void SetAnalysisMode(PHFemVibrationDesc.ANALYSIS_MODE mode) {
            SprExport.Spr_PHFemVibrationIf_SetAnalysisMode((IntPtr)_this, (int)mode);
        }
        public void SetIntegrationMode(PHFemVibrationDesc.INTEGRATION_MODE mode) {
            SprExport.Spr_PHFemVibrationIf_SetIntegrationMode((IntPtr)_this, (int)mode);
        }
        public bool AddBoundaryCondition(int vtxId, Vec3i dof) {
            char ret = SprExport.Spr_PHFemVibrationIf_AddBoundaryCondition((IntPtr)_this, (int)vtxId, (IntPtr)dof);
            return (ret == 0) ? false : true;
        }
        public void DeleteBoundaryCondition() {
            SprExport.Spr_PHFemVibrationIf_DeleteBoundaryCondition((IntPtr)_this);
        }
        public bool FindNeighborTetrahedron(Vec3d posW, int tetId, Vec3d cpW, bool bDeform) {
            char ret = SprExport.Spr_PHFemVibrationIf_FindNeighborTetrahedron((IntPtr)_this, (IntPtr)posW, (int)tetId, (IntPtr)cpW, (bool)bDeform);
            return (ret == 0) ? false : true;
        }
        public bool SetDamping(int tetId, Vec3d posW, double damp_ratio) {
            char ret = SprExport.Spr_PHFemVibrationIf_SetDamping((IntPtr)_this, (int)tetId, (IntPtr)posW, (double)damp_ratio);
            return (ret == 0) ? false : true;
        }
        public bool AddForce(int tetId, Vec3d posW, Vec3d fW) {
            char ret = SprExport.Spr_PHFemVibrationIf_AddForce((IntPtr)_this, (int)tetId, (IntPtr)posW, (IntPtr)fW);
            return (ret == 0) ? false : true;
        }
        public bool GetDisplacement(int tetId, Vec3d posW, Vec3d disp, bool bDeform) {
            char ret = SprExport.Spr_PHFemVibrationIf_GetDisplacement((IntPtr)_this, (int)tetId, (IntPtr)posW, (IntPtr)disp, (bool)bDeform);
            return (ret == 0) ? false : true;
        }
        public bool GetVelocity(int tetId, Vec3d posW, Vec3d vel, bool bDeform) {
            char ret = SprExport.Spr_PHFemVibrationIf_GetVelocity((IntPtr)_this, (int)tetId, (IntPtr)posW, (IntPtr)vel, (bool)bDeform);
            return (ret == 0) ? false : true;
        }
        public bool GetPosition(int tetId, Vec3d posW, Vec3d pos, bool bDeform) {
            char ret = SprExport.Spr_PHFemVibrationIf_GetPosition((IntPtr)_this, (int)tetId, (IntPtr)posW, (IntPtr)pos, (bool)bDeform);
            return (ret == 0) ? false : true;
        }
        public void SetbRecomp() {
            SprExport.Spr_PHFemVibrationIf_SetbRecomp((IntPtr)_this);
        }
        public void Init() {
            SprExport.Spr_PHFemVibrationIf_Init((IntPtr)_this);
        }
    }
    public partial class PHFemThermoIf : PHFemBaseIf {
        public PHFemThermoIf() { _this = SprExport.Spr_new_PHFemThermoIf(); _flag = true; }
        public PHFemThermoIf(IntPtr ptr) : base(ptr) { }
        public PHFemThermoIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemThermoIf(CsCastObject target) {
            return (target._info.Inherit(PHFemThermoIf.GetIfInfoStatic()) ? new PHFemThermoIf(target._this, target._flag) : null);
        }
        ~PHFemThermoIf() { if (_flag) { SprExport.Spr_delete_PHFemThermoIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemThermoIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int GetSurfaceVertex(int id) {
            int result = (int)SprExport.Spr_PHFemThermoIf_GetSurfaceVertex((IntPtr)_this, (int)id);
            return result;
        }
        public int NSurfaceVertices() {
            int result = (int)SprExport.Spr_PHFemThermoIf_NSurfaceVertices((IntPtr)_this);
            return result;
        }
        public void SetVertexTc(int id, double temp) {
            SprExport.Spr_PHFemThermoIf_SetVertexTc((IntPtr)_this, (int)id, (double)temp);
        }
        public void SetVertexTc(int id, double temp, double heatTrans) {
            SprExport.Spr_PHFemThermoIf_SetVertexTc_1((IntPtr)_this, (int)id, (double)temp, (double)heatTrans);
        }
        public Vec3d GetPose(int id) {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetPose((IntPtr)_this, (int)id);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetSufVtxPose(uint id) {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetSufVtxPose((IntPtr)_this, (uint)id);
            return new Vec3d(ptr, true);
        }
        public uint GetStepCount() {
            uint result = (uint)SprExport.Spr_PHFemThermoIf_GetStepCount((IntPtr)_this);
            return result;
        }
        public uint GetStepCountCyc() {
            uint result = (uint)SprExport.Spr_PHFemThermoIf_GetStepCountCyc((IntPtr)_this);
            return result;
        }
        public double GetVertexTemp(uint id) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetVertexTemp((IntPtr)_this, (uint)id);
            return result;
        }
        public double GetSufVertexTemp(uint id) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetSufVertexTemp((IntPtr)_this, (uint)id);
            return result;
        }
        public void SetVertexTemp(uint id, double temp) {
            SprExport.Spr_PHFemThermoIf_SetVertexTemp((IntPtr)_this, (uint)id, (double)temp);
        }
        public void SetVerticesTempAll(double temp) {
            SprExport.Spr_PHFemThermoIf_SetVerticesTempAll((IntPtr)_this, (double)temp);
        }
        public void AddvecFAll(uint id, double dqdt) {
            SprExport.Spr_PHFemThermoIf_AddvecFAll((IntPtr)_this, (uint)id, (double)dqdt);
        }
        public void SetvecFAll(uint id, double dqdt) {
            SprExport.Spr_PHFemThermoIf_SetvecFAll((IntPtr)_this, (uint)id, (double)dqdt);
        }
        public void SetRhoSpheat(double rho, double Cp) {
            SprExport.Spr_PHFemThermoIf_SetRhoSpheat((IntPtr)_this, (double)rho, (double)Cp);
        }
        public Vec2d GetIHbandDrawVtx() {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetIHbandDrawVtx((IntPtr)_this);
            return new Vec2d(ptr, true);
        }
        public void CalcIHdqdt_atleast(double r, double R, double dqdtAll, uint num) {
            SprExport.Spr_PHFemThermoIf_CalcIHdqdt_atleast((IntPtr)_this, (double)r, (double)R, (double)dqdtAll, (uint)num);
        }
        public void UpdateIHheatband(double xS, double xE, uint heatingMODE) {
            SprExport.Spr_PHFemThermoIf_UpdateIHheatband((IntPtr)_this, (double)xS, (double)xE, (uint)heatingMODE);
        }
        public void UpdateIHheat(uint heating) {
            SprExport.Spr_PHFemThermoIf_UpdateIHheat((IntPtr)_this, (uint)heating);
        }
        public void UpdateVecF() {
            SprExport.Spr_PHFemThermoIf_UpdateVecF((IntPtr)_this);
        }
        public void UpdateVecF_frypan() {
            SprExport.Spr_PHFemThermoIf_UpdateVecF_frypan((IntPtr)_this);
        }
        public void DecrMoist() {
            SprExport.Spr_PHFemThermoIf_DecrMoist((IntPtr)_this);
        }
        public void DecrMoist_velo(double vel) {
            SprExport.Spr_PHFemThermoIf_DecrMoist_velo((IntPtr)_this, (double)vel);
        }
        public void DecrMoist_vel(double dt) {
            SprExport.Spr_PHFemThermoIf_DecrMoist_vel((IntPtr)_this, (double)dt);
        }
        public void InitAllVertexTemp() {
            SprExport.Spr_PHFemThermoIf_InitAllVertexTemp((IntPtr)_this);
        }
        public void SetInitThermoConductionParam(double thConduct, double rho, double specificHeat, double heatTrans) {
            SprExport.Spr_PHFemThermoIf_SetInitThermoConductionParam((IntPtr)_this, (double)thConduct, (double)rho, (double)specificHeat, (double)heatTrans);
        }
        public void SetParamAndReCreateMatrix(double thConduct0, double roh0, double specificHeat0) {
            SprExport.Spr_PHFemThermoIf_SetParamAndReCreateMatrix((IntPtr)_this, (double)thConduct0, (double)roh0, (double)specificHeat0);
        }
        public double GetArbitraryPointTemp(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetArbitraryPointTemp((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public double GetVtxTempInTets(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetVtxTempInTets((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public void InitVecFAlls() {
            SprExport.Spr_PHFemThermoIf_InitVecFAlls((IntPtr)_this);
        }
        public double Get_thConduct() {
            double result = (double)SprExport.Spr_PHFemThermoIf_Get_thConduct((IntPtr)_this);
            return result;
        }
        public bool SetConcentricHeatMap(vectorwrapper_double r, vectorwrapper_double temp, Vec2d origin) {
            char ret = SprExport.Spr_PHFemThermoIf_SetConcentricHeatMap((IntPtr)_this, (IntPtr)r, (IntPtr)temp, (IntPtr)origin);
            return (ret == 0) ? false : true;
        }
        public void SetThermalEmissivityToVerticesAll(double thermalEmissivity, double thermalEmissivity_const) {
            SprExport.Spr_PHFemThermoIf_SetThermalEmissivityToVerticesAll((IntPtr)_this, (double)thermalEmissivity, (double)thermalEmissivity_const);
        }
        public void SetOuterTemp(double temp) {
            SprExport.Spr_PHFemThermoIf_SetOuterTemp((IntPtr)_this, (double)temp);
        }
        public void SetThermalRadiation(double ems, double ems_const) {
            SprExport.Spr_PHFemThermoIf_SetThermalRadiation((IntPtr)_this, (double)ems, (double)ems_const);
        }
        public void SetGaussCalcParam(uint cyc, double epsilon) {
            SprExport.Spr_PHFemThermoIf_SetGaussCalcParam((IntPtr)_this, (uint)cyc, (double)epsilon);
        }
        public void InitTcAll(double temp) {
            SprExport.Spr_PHFemThermoIf_InitTcAll((IntPtr)_this, (double)temp);
        }
        public void InitToutAll(double temp) {
            SprExport.Spr_PHFemThermoIf_InitToutAll((IntPtr)_this, (double)temp);
        }
        public void SetWeekPow(double weekPow_) {
            SprExport.Spr_PHFemThermoIf_SetWeekPow((IntPtr)_this, (double)weekPow_);
        }
        public void SetIHParamWEEK(double inr_, double outR_, double weekPow_) {
            SprExport.Spr_PHFemThermoIf_SetIHParamWEEK((IntPtr)_this, (double)inr_, (double)outR_, (double)weekPow_);
        }
        public void SetHeatTransRatioToAllVertex(double heatTransR_) {
            SprExport.Spr_PHFemThermoIf_SetHeatTransRatioToAllVertex((IntPtr)_this, (double)heatTransR_);
        }
        public void AfterSetDesc() {
            SprExport.Spr_PHFemThermoIf_AfterSetDesc((IntPtr)_this);
        }
        public void SetStopTimespan(double timespan) {
            SprExport.Spr_PHFemThermoIf_SetStopTimespan((IntPtr)_this, (double)timespan);
        }
        public void UpdateMatk_RadiantHeatToAir() {
            SprExport.Spr_PHFemThermoIf_UpdateMatk_RadiantHeatToAir((IntPtr)_this);
        }
        public void ReCreateMatrix(double thConduct0) {
            SprExport.Spr_PHFemThermoIf_ReCreateMatrix((IntPtr)_this, (double)thConduct0);
        }
        public void ActivateVtxbeRadiantHeat() {
            SprExport.Spr_PHFemThermoIf_ActivateVtxbeRadiantHeat((IntPtr)_this);
        }
        public void OutputMatKall() {
            SprExport.Spr_PHFemThermoIf_OutputMatKall((IntPtr)_this);
        }
        public void IfRadiantHeatTrans() {
            SprExport.Spr_PHFemThermoIf_IfRadiantHeatTrans((IntPtr)_this);
        }
        public void IfRadiantHeatTransSteak() {
            SprExport.Spr_PHFemThermoIf_IfRadiantHeatTransSteak((IntPtr)_this);
        }
        public float calcGvtx(string fwfood, int pv, uint texture_mode) {
            IntPtr remote1 = Marshal.StringToBSTR(fwfood);
            float result = (float)SprExport.Spr_PHFemThermoIf_calcGvtx((IntPtr)_this, (IntPtr)remote1, (int)pv, (uint)texture_mode);
            Marshal.FreeBSTR(remote1);
            return result;
        }
        public void SetTimeStep(double dt) {
            SprExport.Spr_PHFemThermoIf_SetTimeStep((IntPtr)_this, (double)dt);
        }
        public Vec3d GetVertexNormal(uint vtxid) {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetVertexNormal((IntPtr)_this, (uint)vtxid);
            return new Vec3d(ptr, true);
        }
        public void SetVertexHeatTransRatio(uint vtxid, double heattransRatio) {
            SprExport.Spr_PHFemThermoIf_SetVertexHeatTransRatio((IntPtr)_this, (uint)vtxid, (double)heattransRatio);
        }
        public void SetVertexBeRadiantHeat(uint vtxid, bool flag) {
            SprExport.Spr_PHFemThermoIf_SetVertexBeRadiantHeat((IntPtr)_this, (uint)vtxid, (bool)flag);
        }
        public double GetVertexArea(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetVertexArea((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public void SetVertexToofar(uint vtxid, bool tooFar) {
            SprExport.Spr_PHFemThermoIf_SetVertexToofar((IntPtr)_this, (uint)vtxid, (bool)tooFar);
        }
        public bool GetVertexToofar(uint vtxid) {
            char ret = SprExport.Spr_PHFemThermoIf_GetVertexToofar((IntPtr)_this, (uint)vtxid);
            return (ret == 0) ? false : true;
        }
        public void SetVertexBeCondVtxs(uint vtxid, bool becondVtxs) {
            SprExport.Spr_PHFemThermoIf_SetVertexBeCondVtxs((IntPtr)_this, (uint)vtxid, (bool)becondVtxs);
        }
        public void CreateVecFAll() {
            SprExport.Spr_PHFemThermoIf_CreateVecFAll((IntPtr)_this);
        }
        public void CalcFaceNormalAll() {
            SprExport.Spr_PHFemThermoIf_CalcFaceNormalAll((IntPtr)_this);
        }
        public void CalcVertexNormalAll() {
            SprExport.Spr_PHFemThermoIf_CalcVertexNormalAll((IntPtr)_this);
        }
        public void InitFaceNormalAll() {
            SprExport.Spr_PHFemThermoIf_InitFaceNormalAll((IntPtr)_this);
        }
        public void InitVertexNormalAll() {
            SprExport.Spr_PHFemThermoIf_InitVertexNormalAll((IntPtr)_this);
        }
        public void RevVertexNormalAll() {
            SprExport.Spr_PHFemThermoIf_RevVertexNormalAll((IntPtr)_this);
        }
        public void SetWeekPowFULL(double weekPow_full) {
            SprExport.Spr_PHFemThermoIf_SetWeekPowFULL((IntPtr)_this, (double)weekPow_full);
        }
        public void SetweekPow_FULL(double setweekPow_FULL) {
            SprExport.Spr_PHFemThermoIf_SetweekPow_FULL((IntPtr)_this, (double)setweekPow_FULL);
        }
        public void Setems(double setems) {
            SprExport.Spr_PHFemThermoIf_Setems((IntPtr)_this, (double)setems);
        }
        public void Setems_steak(double setems_steak) {
            SprExport.Spr_PHFemThermoIf_Setems_steak((IntPtr)_this, (double)setems_steak);
        }
        public void SetthConduct(double thConduct) {
            SprExport.Spr_PHFemThermoIf_SetthConduct((IntPtr)_this, (double)thConduct);
        }
        public double GetWeekPowFULL() {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetWeekPowFULL((IntPtr)_this);
            return result;
        }
        public Vec3d GetVertexPose(uint vtxid) {
            IntPtr ptr = SprExport.Spr_PHFemThermoIf_GetVertexPose((IntPtr)_this, (uint)vtxid);
            return new Vec3d(ptr, true);
        }
        public void OutTetVolumeAll() {
            SprExport.Spr_PHFemThermoIf_OutTetVolumeAll((IntPtr)_this);
        }
        public int GetTetsV(uint tetid, uint vtxid) {
            int result = (int)SprExport.Spr_PHFemThermoIf_GetTetsV((IntPtr)_this, (uint)tetid, (uint)vtxid);
            return result;
        }
        public void VecFNegativeCheck() {
            SprExport.Spr_PHFemThermoIf_VecFNegativeCheck((IntPtr)_this);
        }
        public double GetVecFElem(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetVecFElem((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public int GetTetVNums(uint id, uint num) {
            int result = (int)SprExport.Spr_PHFemThermoIf_GetTetVNums((IntPtr)_this, (uint)id, (uint)num);
            return result;
        }
        public double GetInitialTemp() {
            double result = (double)SprExport.Spr_PHFemThermoIf_GetInitialTemp((IntPtr)_this);
            return result;
        }
        public void UpdateVertexTempAll() {
            SprExport.Spr_PHFemThermoIf_UpdateVertexTempAll((IntPtr)_this);
        }
        public void SetThermoCameraScale(double minTemp, double maxTemp) {
            SprExport.Spr_PHFemThermoIf_SetThermoCameraScale((IntPtr)_this, (double)minTemp, (double)maxTemp);
        }
    }
    public partial class PHFemPorousWOMoveIf : PHFemBaseIf {
        public PHFemPorousWOMoveIf() { _this = SprExport.Spr_new_PHFemPorousWOMoveIf(); _flag = true; }
        public PHFemPorousWOMoveIf(IntPtr ptr) : base(ptr) { }
        public PHFemPorousWOMoveIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFemPorousWOMoveIf(CsCastObject target) {
            return (target._info.Inherit(PHFemPorousWOMoveIf.GetIfInfoStatic()) ? new PHFemPorousWOMoveIf(target._this, target._flag) : null);
        }
        ~PHFemPorousWOMoveIf() { if (_flag) { SprExport.Spr_delete_PHFemPorousWOMoveIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFemPorousWOMoveIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFemPorousWOMoveIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFemPorousWOMoveIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFemPorousWOMoveIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetTimeStep(double dt) {
            SprExport.Spr_PHFemPorousWOMoveIf_SetTimeStep((IntPtr)_this, (double)dt);
        }
        public double GetTimeStep() {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public void UpdateVertexRhoWAll() {
            SprExport.Spr_PHFemPorousWOMoveIf_UpdateVertexRhoWAll((IntPtr)_this);
        }
        public void UpdateVertexRhoOAll() {
            SprExport.Spr_PHFemPorousWOMoveIf_UpdateVertexRhoOAll((IntPtr)_this);
        }
        public void SetVertexMw(uint vtxid, double mw) {
            SprExport.Spr_PHFemPorousWOMoveIf_SetVertexMw((IntPtr)_this, (uint)vtxid, (double)mw);
        }
        public void SetVertexMo(uint vtxid, double mo) {
            SprExport.Spr_PHFemPorousWOMoveIf_SetVertexMo((IntPtr)_this, (uint)vtxid, (double)mo);
        }
        public double GetVertexMw(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVertexMw((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public double GetVertexMo(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVertexMo((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public double GetVtxWaterInTets(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVtxWaterInTets((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public double GetVtxOilInTets(Vec3d temppos) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVtxOilInTets((IntPtr)_this, (IntPtr)temppos);
            return result;
        }
        public double GetVertexRhoW(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVertexRhoW((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public double GetVertexRhoO(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVertexRhoO((IntPtr)_this, (uint)vtxid);
            return result;
        }
        public float calcGvtx(string fwfood, int pv, uint texture_mode) {
            IntPtr remote1 = Marshal.StringToBSTR(fwfood);
            float result = (float)SprExport.Spr_PHFemPorousWOMoveIf_calcGvtx((IntPtr)_this, (IntPtr)remote1, (int)pv, (uint)texture_mode);
            Marshal.FreeBSTR(remote1);
            return result;
        }
        public void setGravity(Vec3d g) {
            SprExport.Spr_PHFemPorousWOMoveIf_setGravity((IntPtr)_this, (IntPtr)g);
        }
        public double decideWetValue(uint faceid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_decideWetValue((IntPtr)_this, (uint)faceid);
            return result;
        }
        public double GetVtxSaturation(uint vtxid) {
            double result = (double)SprExport.Spr_PHFemPorousWOMoveIf_GetVtxSaturation((IntPtr)_this, (uint)vtxid);
            return result;
        }
    }
    public partial class PHFrameIf : SceneObjectIf {
        public PHFrameIf() { _this = SprExport.Spr_new_PHFrameIf(); _flag = true; }
        public PHFrameIf(IntPtr ptr) : base(ptr) { }
        public PHFrameIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFrameIf(CsCastObject target) {
            return (target._info.Inherit(PHFrameIf.GetIfInfoStatic()) ? new PHFrameIf(target._this, target._flag) : null);
        }
        ~PHFrameIf() { if (_flag) { SprExport.Spr_delete_PHFrameIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFrameIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFrameIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFrameIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFrameIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Posed GetPose() {
            IntPtr ptr = SprExport.Spr_PHFrameIf_GetPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
        public void SetPose(Posed p) {
            SprExport.Spr_PHFrameIf_SetPose((IntPtr)_this, (IntPtr)p);
        }
        public CDShapeIf GetShape() {
            IntPtr ptr = SprExport.Spr_PHFrameIf_GetShape((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            return obj;
        }
    }
    public partial class PHBodyIf : SceneObjectIf {
        public PHBodyIf() { _this = SprExport.Spr_new_PHBodyIf(); _flag = true; }
        public PHBodyIf(IntPtr ptr) : base(ptr) { }
        public PHBodyIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBodyIf(CsCastObject target) {
            return (target._info.Inherit(PHBodyIf.GetIfInfoStatic()) ? new PHBodyIf(target._this, target._flag) : null);
        }
        ~PHBodyIf() { if (_flag) { SprExport.Spr_delete_PHBodyIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBodyIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Posed GetPose() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHBodyIf_GetPose(
                    (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return new Posed(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHBodyIf_GetPose((IntPtr)_thisArray[0]);
                return new Posed(ptr, true);
            }
        }
        public Vec3d GetVelocity() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetVelocity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetAngularVelocity() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetAngularVelocity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetCenterOfMass() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetCenterOfMass((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetFramePosition() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetFramePosition((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetCenterPosition() {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetCenterPosition((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public bool IsDynamical() {
            char ret = SprExport.Spr_PHBodyIf_IsDynamical((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public bool IsFrozen() {
            char ret = SprExport.Spr_PHBodyIf_IsFrozen((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void AddShape(CDShapeIf shape) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBodyIf_AddShape((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)shape);
                            SprExport.Spr_PHBodyIf_AddShape((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)shape);
                        });
                        SprExport.Spr_PHBodyIf_AddShape((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)shape);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBodyIf_AddShape((IntPtr)_this, (IntPtr)shape);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBodyIf_AddShape((IntPtr)_thisArray[0], (IntPtr)shape);
            }
        }
        public void AddShapes(CDShapeIf shBegin, CDShapeIf shEnd) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBodyIf_AddShapes((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)shBegin, (IntPtr)shEnd);
                            SprExport.Spr_PHBodyIf_AddShapes((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)shBegin, (IntPtr)shEnd);
                        });
                        SprExport.Spr_PHBodyIf_AddShapes((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)shBegin, (IntPtr)shEnd);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBodyIf_AddShapes((IntPtr)_this, (IntPtr)shBegin, (IntPtr)shEnd);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBodyIf_AddShapes((IntPtr)_thisArray[0], (IntPtr)shBegin, (IntPtr)shEnd);
            }
        }
        public void RemoveShape(int index) {
            SprExport.Spr_PHBodyIf_RemoveShape((IntPtr)_this, (int)index);
        }
        public void RemoveShapes(int idxBegin, int idxEnd) {
            SprExport.Spr_PHBodyIf_RemoveShapes((IntPtr)_this, (int)idxBegin, (int)idxEnd);
        }
        public void RemoveShape(CDShapeIf shape) {
            SprExport.Spr_PHBodyIf_RemoveShape_1((IntPtr)_this, (IntPtr)shape);
        }
        public int NShape() {
            int result = (int)SprExport.Spr_PHBodyIf_NShape((IntPtr)_this);
            return result;
        }
        public CDShapeIf GetShape(int index) {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetShape((IntPtr)_this, (int)index);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            return obj;
        }
        public Posed GetShapePose(int index) {
            IntPtr ptr = SprExport.Spr_PHBodyIf_GetShapePose((IntPtr)_this, (int)index);
            return new Posed(ptr, true);
        }
        public void SetShapePose(int index, Posed pose) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Posed(pose);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForStep], (int)index, (IntPtr)newP);
                            SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (int)index, (IntPtr)newP);
                        });
                        SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForGet], (int)index, (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this, (int)index, (IntPtr)pose);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[0], (int)index, (IntPtr)pose);
            }
        }
        public void ClearShape() {
            SprExport.Spr_PHBodyIf_ClearShape((IntPtr)_this);
        }
        public void GetBBox(Vec3d bbmin, Vec3d bbmax, bool world) {
            SprExport.Spr_PHBodyIf_GetBBox((IntPtr)_this, (IntPtr)bbmin, (IntPtr)bbmax, (bool)world);
        }
    }
    public partial class PHSolidIf : PHBodyIf {
        public PHSolidIf() { _this = SprExport.Spr_new_PHSolidIf(); _flag = true; }
        public PHSolidIf(IntPtr ptr) : base(ptr) { }
        public PHSolidIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSolidIf(CsCastObject target) {
            return (target._info.Inherit(PHSolidIf.GetIfInfoStatic()) ? new PHSolidIf(target._this, target._flag) : null);
        }
        ~PHSolidIf() { if (_flag) { SprExport.Spr_delete_PHSolidIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSolidIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void AddForce(Vec3d f) {
            SprExport.Spr_PHSolidIf_AddForce((IntPtr)_this, (IntPtr)f);
        }
        public void AddTorque(Vec3d t) {
            SprExport.Spr_PHSolidIf_AddTorque((IntPtr)_this, (IntPtr)t);
        }
        public void AddForce(Vec3d f, Vec3d r) {
            SprExport.Spr_PHSolidIf_AddForce_1((IntPtr)_this, (IntPtr)f, (IntPtr)r);
        }
        public double GetMass() {
            double result = (double)SprExport.Spr_PHSolidIf_GetMass((IntPtr)_this);
            return result;
        }
        public double GetMassInv() {
            double result = (double)SprExport.Spr_PHSolidIf_GetMassInv((IntPtr)_this);
            return result;
        }
        public void SetMass(double m) {
            SprExport.Spr_PHSolidIf_SetMass((IntPtr)_this, (double)m);
        }
        public void SetCenterOfMass(Vec3d center) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newCenter = new Vec3d(center);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSolidIf_SetCenterOfMass((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newCenter);
                            SprExport.Spr_PHSolidIf_SetCenterOfMass((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newCenter);
                        });
                        SprExport.Spr_PHSolidIf_SetCenterOfMass((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newCenter);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSolidIf_SetCenterOfMass((IntPtr)_this, (IntPtr)center);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSolidIf_SetCenterOfMass((IntPtr)_thisArray[0], (IntPtr)center);
            }
        }
        public Matrix3d GetInertia() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetInertia((IntPtr)_this);
            return new Matrix3d(ptr, true);
        }
        public Matrix3d GetInertiaInv() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetInertiaInv((IntPtr)_this);
            return new Matrix3d(ptr, true);
        }
        public void SetInertia(Matrix3d I) {
            SprExport.Spr_PHSolidIf_SetInertia((IntPtr)_this, (IntPtr)I);
        }
        public void CompInertia() {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSolidIf_CompInertia((IntPtr)_thisArray[phSceneIf.sceneForStep]);
                            SprExport.Spr_PHSolidIf_CompInertia((IntPtr)_thisArray[phSceneIf.sceneForBuffer]);
                        });
                        SprExport.Spr_PHSolidIf_CompInertia((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSolidIf_CompInertia((IntPtr)_this);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSolidIf_CompInertia((IntPtr)_thisArray[0]);
            }
        }
        public void SetFramePosition(Vec3d p) {
            SprExport.Spr_PHSolidIf_SetFramePosition((IntPtr)_this, (IntPtr)p);
        }
        public void SetCenterPosition(Vec3d p) {
            SprExport.Spr_PHSolidIf_SetCenterPosition((IntPtr)_this, (IntPtr)p);
        }
        public Vec3d GetDeltaPosition() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetDeltaPosition((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetDeltaPosition(Vec3d p) {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetDeltaPosition_1((IntPtr)_this, (IntPtr)p);
            return new Vec3d(ptr, true);
        }
        public Quaterniond GetOrientation() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetOrientation((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
        public void SetOrientation(Quaterniond q) {
            SprExport.Spr_PHSolidIf_SetOrientation((IntPtr)_this, (IntPtr)q);
        }
        public void SetPose(Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Posed(p);
                        //Console.WriteLine("PHSolid.SetPose in isStepping");
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newP);
                            SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newP);
                        });
                        SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)p);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[0], (IntPtr)p);
            }
        }
        public void SetVelocity(Vec3d v) {
            SprExport.Spr_PHSolidIf_SetVelocity((IntPtr)_this, (IntPtr)v);
        }
        public void SetAngularVelocity(Vec3d av) {
            SprExport.Spr_PHSolidIf_SetAngularVelocity((IntPtr)_this, (IntPtr)av);
        }
        public Vec3d GetForce() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetForce((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetTorque() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetTorque((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetGravity(bool bOn) {
            SprExport.Spr_PHSolidIf_SetGravity((IntPtr)_this, (bool)bOn);
        }
        public void SetDynamical(bool bOn) {
            SprExport.Spr_PHSolidIf_SetDynamical((IntPtr)_this, (bool)bOn);
        }
        public bool IsDynamical() {
            char ret = SprExport.Spr_PHSolidIf_IsDynamical((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetStationary(bool bOn) {
            SprExport.Spr_PHSolidIf_SetStationary((IntPtr)_this, (bool)bOn);
        }
        public bool IsStationary() {
            char ret = SprExport.Spr_PHSolidIf_IsStationary((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PHTreeNodeIf GetTreeNode() {
            IntPtr ptr = SprExport.Spr_PHSolidIf_GetTreeNode((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHTreeNodeIf obj = new PHTreeNodeIf(ptr);
            if (obj.GetIfInfo() == PHRootNodeIf.GetIfInfoStatic()) { return new PHRootNodeIf(ptr); }
            if (obj.GetIfInfo() == PHTreeNode1DIf.GetIfInfoStatic()) { return new PHTreeNode1DIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointNodeIf.GetIfInfoStatic()) { return new PHBallJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointNodeIf.GetIfInfoStatic()) { return new PHFixJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointNodeIf.GetIfInfoStatic()) { return new PHHingeJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointNodeIf.GetIfInfoStatic()) { return new PHSliderJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointNodeIf.GetIfInfoStatic()) { return new PHPathJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointNodeIf.GetIfInfoStatic()) { return new PHGenericJointNodeIf(ptr); }
            return obj;
        }
    }
    public partial class PHShapePairForLCPIf : CDShapePairIf {
        public PHShapePairForLCPIf() { _this = SprExport.Spr_new_PHShapePairForLCPIf(); _flag = true; }
        public PHShapePairForLCPIf(IntPtr ptr) : base(ptr) { }
        public PHShapePairForLCPIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHShapePairForLCPIf(CsCastObject target) {
            return (target._info.Inherit(PHShapePairForLCPIf.GetIfInfoStatic()) ? new PHShapePairForLCPIf(target._this, target._flag) : null);
        }
        ~PHShapePairForLCPIf() { if (_flag) { SprExport.Spr_delete_PHShapePairForLCPIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHShapePairForLCPIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int NSectionVertexes() {
            int result = (int)SprExport.Spr_PHShapePairForLCPIf_NSectionVertexes((IntPtr)_this);
            return result;
        }
        public Vec3d GetSectionVertex(int i) {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_GetSectionVertex((IntPtr)_this, (int)i);
            return new Vec3d(ptr, true);
        }
        public double GetContactDimension() {
            double result = (double)SprExport.Spr_PHShapePairForLCPIf_GetContactDimension((IntPtr)_this);
            return result;
        }
        public Vec3d GetNormalVector() {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_GetNormalVector((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void GetClosestPoints(Vec3d pa, Vec3d pb) {
            SprExport.Spr_PHShapePairForLCPIf_GetClosestPoints((IntPtr)_this, (IntPtr)pa, (IntPtr)pb);
        }
        public CDShapeIf GetShape(int i) {
            IntPtr ptr = SprExport.Spr_PHShapePairForLCPIf_GetShape((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            return obj;
        }
    }
    public partial class PHSolidPairIf : ObjectIf {
        public PHSolidPairIf() { _this = SprExport.Spr_new_PHSolidPairIf(); _flag = true; }
        public PHSolidPairIf(IntPtr ptr) : base(ptr) { }
        public PHSolidPairIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSolidPairIf(CsCastObject target) {
            return (target._info.Inherit(PHSolidPairIf.GetIfInfoStatic()) ? new PHSolidPairIf(target._this, target._flag) : null);
        }
        ~PHSolidPairIf() { if (_flag) { SprExport.Spr_delete_PHSolidPairIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSolidPairIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSolidPairIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSolidPairIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSolidPairIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHSolidIf GetSolid(int i) {
            IntPtr ptr = SprExport.Spr_PHSolidPairIf_GetSolid((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public int NListener() {
            int result = (int)SprExport.Spr_PHSolidPairIf_NListener((IntPtr)_this);
            return result;
        }
        public PHCollisionListener GetListener(int i) {
            IntPtr ptr = SprExport.Spr_PHSolidPairIf_GetListener((IntPtr)_this, (int)i);
            return new PHCollisionListener(ptr);
        }
        public void RemoveListener(int i) {
            SprExport.Spr_PHSolidPairIf_RemoveListener((IntPtr)_this, (int)i);
        }
        public void AddListener(PHCollisionListener l, int pos) {
            SprExport.Spr_PHSolidPairIf_AddListener((IntPtr)_this, (IntPtr)l, (int)pos);
        }
        public void AddListener(PHCollisionListener l) {
            SprExport.Spr_PHSolidPairIf_AddListener_1((IntPtr)_this, (IntPtr)l);
        }
    }
    public partial class PHSolidPairForLCPIf : PHSolidPairIf {
        public PHSolidPairForLCPIf() { _this = SprExport.Spr_new_PHSolidPairForLCPIf(); _flag = true; }
        public PHSolidPairForLCPIf(IntPtr ptr) : base(ptr) { }
        public PHSolidPairForLCPIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSolidPairForLCPIf(CsCastObject target) {
            return (target._info.Inherit(PHSolidPairForLCPIf.GetIfInfoStatic()) ? new PHSolidPairForLCPIf(target._this, target._flag) : null);
        }
        ~PHSolidPairForLCPIf() { if (_flag) { SprExport.Spr_delete_PHSolidPairForLCPIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForLCPIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSolidPairForLCPIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForLCPIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForLCPIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public bool IsContactEnabled() {
            char ret = SprExport.Spr_PHSolidPairForLCPIf_IsContactEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableContact(bool enable) {
            SprExport.Spr_PHSolidPairForLCPIf_EnableContact((IntPtr)_this, (bool)enable);
        }
        public int GetContactState(int i, int j) {
            int result = (int)SprExport.Spr_PHSolidPairForLCPIf_GetContactState((IntPtr)_this, (int)i, (int)j);
            return result;
        }
        public Vec3d GetCommonPoint(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHSolidPairForLCPIf_GetCommonPoint((IntPtr)_this, (int)i, (int)j);
            return new Vec3d(ptr, true);
        }
        public uint GetLastContactCount(int i, int j) {
            uint result = (uint)SprExport.Spr_PHSolidPairForLCPIf_GetLastContactCount((IntPtr)_this, (int)i, (int)j);
            return result;
        }
        public double GetContactDepth(int i, int j) {
            double result = (double)SprExport.Spr_PHSolidPairForLCPIf_GetContactDepth((IntPtr)_this, (int)i, (int)j);
            return result;
        }
        public PHShapePairForLCPIf GetShapePair(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHSolidPairForLCPIf_GetShapePair((IntPtr)_this, (int)i, (int)j);
            if (ptr == IntPtr.Zero) { return null; }
            PHShapePairForLCPIf obj = new PHShapePairForLCPIf(ptr);
            return obj;
        }
    }
    public partial class PHHapticPointerIf : PHSolidIf {
        public PHHapticPointerIf() { _this = SprExport.Spr_new_PHHapticPointerIf(); _flag = true; }
        public PHHapticPointerIf(IntPtr ptr) : base(ptr) { }
        public PHHapticPointerIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHapticPointerIf(CsCastObject target) {
            return (target._info.Inherit(PHHapticPointerIf.GetIfInfoStatic()) ? new PHHapticPointerIf(target._this, target._flag) : null);
        }
        ~PHHapticPointerIf() { if (_flag) { SprExport.Spr_delete_PHHapticPointerIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHapticPointerIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetHapticRenderMode(PHHapticPointerDesc.HapticRenderMode m) {
            SprExport.Spr_PHHapticPointerIf_SetHapticRenderMode((IntPtr)_this, (int)m);
        }
        public PHHapticPointerDesc.HapticRenderMode GetHapticRenderMode() {
            int result = SprExport.Spr_PHHapticPointerIf_GetHapticRenderMode((IntPtr)_this);
            return (PHHapticPointerDesc.HapticRenderMode)result;
        }
        public void EnableRotation(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableRotation((IntPtr)_this, (bool)b);
        }
        public bool IsRotation() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsRotation((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableForce(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableForce((IntPtr)_this, (bool)b);
        }
        public bool IsForce() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsForce((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableFriction(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableFriction((IntPtr)_this, (bool)b);
        }
        public bool IsFriction() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsFriction((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableTimeVaryFriction(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableTimeVaryFriction((IntPtr)_this, (bool)b);
        }
        public bool IsTimeVaryFriction() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsTimeVaryFriction((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableVibration(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableVibration((IntPtr)_this, (bool)b);
        }
        public bool IsVibration() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsVibration((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableMultiPoints(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableMultiPoints((IntPtr)_this, (bool)b);
        }
        public bool IsMultiPoints() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsMultiPoints((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableMultiProxy(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableMultiProxy((IntPtr)_this, (bool)b);
        }
        public bool IsMultiProxy() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsMultiProxy((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableSimulation(bool b) {
            SprExport.Spr_PHHapticPointerIf_EnableSimulation((IntPtr)_this, (bool)b);
        }
        public bool IsSimulation() {
            char ret = SprExport.Spr_PHHapticPointerIf_IsSimulation((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetFrictionSpring(float s) {
            SprExport.Spr_PHHapticPointerIf_SetFrictionSpring((IntPtr)_this, (float)s);
        }
        public float GetFrictionSpring() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetFrictionSpring((IntPtr)_this);
            return result;
        }
        public void SetFrictionDamper(float s) {
            SprExport.Spr_PHHapticPointerIf_SetFrictionDamper((IntPtr)_this, (float)s);
        }
        public float GetFrictionDamper() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetFrictionDamper((IntPtr)_this);
            return result;
        }
        public void SetReflexSpring(float s) {
            SprExport.Spr_PHHapticPointerIf_SetReflexSpring((IntPtr)_this, (float)s);
        }
        public float GetReflexSpring() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetReflexSpring((IntPtr)_this);
            return result;
        }
        public void SetReflexDamper(float d) {
            SprExport.Spr_PHHapticPointerIf_SetReflexDamper((IntPtr)_this, (float)d);
        }
        public float GetReflexDamper() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetReflexDamper((IntPtr)_this);
            return result;
        }
        public void SetRotationReflexSpring(float s) {
            SprExport.Spr_PHHapticPointerIf_SetRotationReflexSpring((IntPtr)_this, (float)s);
        }
        public float GetRotationReflexSpring() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetRotationReflexSpring((IntPtr)_this);
            return result;
        }
        public void SetRotationReflexDamper(float d) {
            SprExport.Spr_PHHapticPointerIf_SetRotationReflexDamper((IntPtr)_this, (float)d);
        }
        public float GetRotationReflexDamper() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetRotationReflexDamper((IntPtr)_this);
            return result;
        }
        public void SetLocalRange(float r) {
            SprExport.Spr_PHHapticPointerIf_SetLocalRange((IntPtr)_this, (float)r);
        }
        public float GetLocalRange() {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetLocalRange((IntPtr)_this);
            return result;
        }
        public void SetPosScale(double scale) {
            SprExport.Spr_PHHapticPointerIf_SetPosScale((IntPtr)_this, (double)scale);
        }
        public double GetPosScale() {
            double result = (double)SprExport.Spr_PHHapticPointerIf_GetPosScale((IntPtr)_this);
            return result;
        }
        public void SetRotationalWeight(double w) {
            SprExport.Spr_PHHapticPointerIf_SetRotationalWeight((IntPtr)_this, (double)w);
        }
        public double GetRotationalWeight() {
            double result = (double)SprExport.Spr_PHHapticPointerIf_GetRotationalWeight((IntPtr)_this);
            return result;
        }
        public void SetDefaultPose(Posed p) {
            SprExport.Spr_PHHapticPointerIf_SetDefaultPose((IntPtr)_this, (IntPtr)p);
        }
        public Posed GetDefaultPose() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetDefaultPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
        public int NNeighborSolids() {
            int result = (int)SprExport.Spr_PHHapticPointerIf_NNeighborSolids((IntPtr)_this);
            return result;
        }
        public int GetNeighborSolidId(int i) {
            int result = (int)SprExport.Spr_PHHapticPointerIf_GetNeighborSolidId((IntPtr)_this, (int)i);
            return result;
        }
        public PHSolidIf GetNeighborSolid(int i) {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetNeighborSolid((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public float GetContactForce(int i) {
            float result = (float)SprExport.Spr_PHHapticPointerIf_GetContactForce((IntPtr)_this, (int)i);
            return result;
        }
        public SpatialVector GetHapticForce() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetHapticForce((IntPtr)_this);
            return new SpatialVector(ptr, true);
        }
        public SpatialVector GetProxyVelocity() {
            IntPtr ptr = SprExport.Spr_PHHapticPointerIf_GetProxyVelocity((IntPtr)_this);
            return new SpatialVector(ptr, true);
        }
        public void SetProxyN(int n) {
            SprExport.Spr_PHHapticPointerIf_SetProxyN((IntPtr)_this, (int)n);
        }
        public int GetProxyN() {
            int result = (int)SprExport.Spr_PHHapticPointerIf_GetProxyN((IntPtr)_this);
            return result;
        }
        public int GetTotalSlipState() {
            int result = (int)SprExport.Spr_PHHapticPointerIf_GetTotalSlipState((IntPtr)_this);
            return result;
        }
        public int GetSlipState(int i) {
            int result = (int)SprExport.Spr_PHHapticPointerIf_GetSlipState((IntPtr)_this, (int)i);
            return result;
        }
        public void SetProxyVelocity(SpatialVector spv) {
            SprExport.Spr_PHHapticPointerIf_SetProxyVelocity((IntPtr)_this, (IntPtr)spv);
        }
        public void AddHapticForce(SpatialVector f) {
            SprExport.Spr_PHHapticPointerIf_AddHapticForce((IntPtr)_this, (IntPtr)f);
        }
        public void ClearHapticForce() {
            SprExport.Spr_PHHapticPointerIf_ClearHapticForce((IntPtr)_this);
        }
        public void UpdateHumanInterface(Posed pose, SpatialVector vel) {
            SprExport.Spr_PHHapticPointerIf_UpdateHumanInterface((IntPtr)_this, (IntPtr)pose, (IntPtr)vel);
        }
    }
    public partial class PHShapePairForHapticIf : CDShapePairIf {
        public PHShapePairForHapticIf() { _this = SprExport.Spr_new_PHShapePairForHapticIf(); _flag = true; }
        public PHShapePairForHapticIf(IntPtr ptr) : base(ptr) { }
        public PHShapePairForHapticIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHShapePairForHapticIf(CsCastObject target) {
            return (target._info.Inherit(PHShapePairForHapticIf.GetIfInfoStatic()) ? new PHShapePairForHapticIf(target._this, target._flag) : null);
        }
        ~PHShapePairForHapticIf() { if (_flag) { SprExport.Spr_delete_PHShapePairForHapticIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHShapePairForHapticIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHShapePairForHapticIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHShapePairForHapticIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHShapePairForHapticIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int NIrs() {
            int result = (int)SprExport.Spr_PHShapePairForHapticIf_NIrs((IntPtr)_this);
            return result;
        }
        public int NIrsNormal() {
            int result = (int)SprExport.Spr_PHShapePairForHapticIf_NIrsNormal((IntPtr)_this);
            return result;
        }
        public Vec3d GetIrForce(int i) {
            IntPtr ptr = SprExport.Spr_PHShapePairForHapticIf_GetIrForce((IntPtr)_this, (int)i);
            return new Vec3d(ptr, true);
        }
        public double GetMu() {
            double result = (double)SprExport.Spr_PHShapePairForHapticIf_GetMu((IntPtr)_this);
            return result;
        }
        public PHFrameIf GetFrame(int i) {
            IntPtr ptr = SprExport.Spr_PHShapePairForHapticIf_GetFrame((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHFrameIf obj = new PHFrameIf(ptr);
            return obj;
        }
        public void UpdateCache() {
            SprExport.Spr_PHShapePairForHapticIf_UpdateCache((IntPtr)_this);
        }
        public double GetMus(int id) {
            double result = (double)SprExport.Spr_PHShapePairForHapticIf_GetMus((IntPtr)_this, (int)id);
            return result;
        }
    }
    public partial class PHSolidPairForHapticIf : PHSolidPairIf {
        public PHSolidPairForHapticIf() { _this = SprExport.Spr_new_PHSolidPairForHapticIf(); _flag = true; }
        public PHSolidPairForHapticIf(IntPtr ptr) : base(ptr) { }
        public PHSolidPairForHapticIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSolidPairForHapticIf(CsCastObject target) {
            return (target._info.Inherit(PHSolidPairForHapticIf.GetIfInfoStatic()) ? new PHSolidPairForHapticIf(target._this, target._flag) : null);
        }
        ~PHSolidPairForHapticIf() { if (_flag) { SprExport.Spr_delete_PHSolidPairForHapticIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSolidPairForHapticIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHShapePairForHapticIf GetShapePair(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_GetShapePair((IntPtr)_this, (int)i, (int)j);
            if (ptr == IntPtr.Zero) { return null; }
            PHShapePairForHapticIf obj = new PHShapePairForHapticIf(ptr);
            return obj;
        }
        public FrictionState GetFrictionState() {
            int result = SprExport.Spr_PHSolidPairForHapticIf_GetFrictionState((IntPtr)_this);
            return (PHSolidPairForHapticIf.FrictionState)result;
        }
        public uint GetContactCount() {
            uint result = (uint)SprExport.Spr_PHSolidPairForHapticIf_GetContactCount((IntPtr)_this);
            return result;
        }
        public uint GetFrictionCount() {
            uint result = (uint)SprExport.Spr_PHSolidPairForHapticIf_GetFrictionCount((IntPtr)_this);
            return result;
        }
        public void InitFrictionState(int n) {
            SprExport.Spr_PHSolidPairForHapticIf_InitFrictionState((IntPtr)_this, (int)n);
        }
        public void InitFrictionCount(int n) {
            SprExport.Spr_PHSolidPairForHapticIf_InitFrictionCount((IntPtr)_this, (int)n);
        }
        public void InitContactCount(int n) {
            SprExport.Spr_PHSolidPairForHapticIf_InitContactCount((IntPtr)_this, (int)n);
        }
        public void InitSlipState(int n) {
            SprExport.Spr_PHSolidPairForHapticIf_InitSlipState((IntPtr)_this, (int)n);
        }
        public int GetSlipState(int i) {
            int result = (int)SprExport.Spr_PHSolidPairForHapticIf_GetSlipState((IntPtr)_this, (int)i);
            return result;
        }
        public Vec3d GetForce() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_GetForce((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetTorque() {
            IntPtr ptr = SprExport.Spr_PHSolidPairForHapticIf_GetTorque((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public FrictionState GetFrictionStates(int i) {
            int result = SprExport.Spr_PHSolidPairForHapticIf_GetFrictionStates((IntPtr)_this, (int)i);
            return (PHSolidPairForHapticIf.FrictionState)result;
        }
        public enum FrictionState : int {
            FREE = 0, STATIC = FREE + 1, DYNAMIC = STATIC + 1
        }
    }
    public partial class PHHapticEngineIf : PHEngineIf {
        public PHHapticEngineIf() { _this = SprExport.Spr_new_PHHapticEngineIf(); _flag = true; }
        public PHHapticEngineIf(IntPtr ptr) : base(ptr) { }
        public PHHapticEngineIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHapticEngineIf(CsCastObject target) {
            return (target._info.Inherit(PHHapticEngineIf.GetIfInfoStatic()) ? new PHHapticEngineIf(target._this, target._flag) : null);
        }
        ~PHHapticEngineIf() { if (_flag) { SprExport.Spr_delete_PHHapticEngineIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHapticEngineIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetHapticStepMode(PHHapticEngineDesc.HapticStepMode mode) {
            SprExport.Spr_PHHapticEngineIf_SetHapticStepMode((IntPtr)_this, (int)mode);
        }
        public PHHapticEngineDesc.HapticStepMode GetHapticStepMode() {
            int result = SprExport.Spr_PHHapticEngineIf_GetHapticStepMode((IntPtr)_this);
            return (PHHapticEngineDesc.HapticStepMode)result;
        }
        public int NSolids() {
            int result = (int)SprExport.Spr_PHHapticEngineIf_NSolids((IntPtr)_this);
            return result;
        }
        public int NPointers() {
            int result = (int)SprExport.Spr_PHHapticEngineIf_NPointers((IntPtr)_this);
            return result;
        }
        public PHHapticPointerIf GetPointer(int i) {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetPointer((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHHapticPointerIf obj = new PHHapticPointerIf(ptr);
            return obj;
        }
        public PHSolidPairForHapticIf GetSolidPair(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetSolidPair((IntPtr)_this, (int)i, (int)j);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidPairForHapticIf obj = new PHSolidPairForHapticIf(ptr);
            return obj;
        }
        public int NSolidsInHaptic() {
            int result = (int)SprExport.Spr_PHHapticEngineIf_NSolidsInHaptic((IntPtr)_this);
            return result;
        }
        public int NPointersInHaptic() {
            int result = (int)SprExport.Spr_PHHapticEngineIf_NPointersInHaptic((IntPtr)_this);
            return result;
        }
        public PHHapticPointerIf GetPointerInHaptic(int i) {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetPointerInHaptic((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHHapticPointerIf obj = new PHHapticPointerIf(ptr);
            return obj;
        }
        public PHSolidPairForHapticIf GetSolidPairInHaptic(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHHapticEngineIf_GetSolidPairInHaptic((IntPtr)_this, (int)i, (int)j);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidPairForHapticIf obj = new PHSolidPairForHapticIf(ptr);
            return obj;
        }
        public void StepPhysicsSimulation() {
            SprExport.Spr_PHHapticEngineIf_StepPhysicsSimulation((IntPtr)_this);
        }
        public void ReleaseState() {
            SprExport.Spr_PHHapticEngineIf_ReleaseState((IntPtr)_this);
        }
        public IntPtr Callback(CsObject arg) {
            IntPtr result = (IntPtr)SprExport.Spr_PHHapticEngineIf_Callback((IntPtr)_this, (IntPtr)arg);
            return result;
        }
    }
    public partial class PHIKEndEffectorIf : SceneObjectIf {
        public PHIKEndEffectorIf() { _this = SprExport.Spr_new_PHIKEndEffectorIf(); _flag = true; }
        public PHIKEndEffectorIf(IntPtr ptr) : base(ptr) { }
        public PHIKEndEffectorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKEndEffectorIf(CsCastObject target) {
            return (target._info.Inherit(PHIKEndEffectorIf.GetIfInfoStatic()) ? new PHIKEndEffectorIf(target._this, target._flag) : null);
        }
        ~PHIKEndEffectorIf() { if (_flag) { SprExport.Spr_delete_PHIKEndEffectorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKEndEffectorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetSolid(PHSolidIf solid) {
            SprExport.Spr_PHIKEndEffectorIf_SetSolid((IntPtr)_this, (IntPtr)solid);
        }
        public PHSolidIf GetSolid() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetSolid((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public void SetParentActuator(PHIKActuatorIf ika) {
            SprExport.Spr_PHIKEndEffectorIf_SetParentActuator((IntPtr)_this, (IntPtr)ika);
        }
        public PHIKActuatorIf GetParentActuator() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetParentActuator((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public void Enable(bool enable) {
            SprExport.Spr_PHIKEndEffectorIf_Enable((IntPtr)_this, (bool)enable);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHIKEndEffectorIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetTargetLocalPosition(Vec3d localPosition) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetLocalPosition((IntPtr)_this, (IntPtr)localPosition);
        }
        public Vec3d GetTargetLocalPosition() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetLocalPosition((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetTargetLocalDirection(Vec3d localDirection) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetLocalDirection((IntPtr)_this, (IntPtr)localDirection);
        }
        public Vec3d GetTargetLocalDirection() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetLocalDirection((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void EnablePositionControl(bool enable) {
            SprExport.Spr_PHIKEndEffectorIf_EnablePositionControl((IntPtr)_this, (bool)enable);
        }
        public bool IsPositionControlEnabled() {
            char ret = SprExport.Spr_PHIKEndEffectorIf_IsPositionControlEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetPositionPriority(double priority) {
            SprExport.Spr_PHIKEndEffectorIf_SetPositionPriority((IntPtr)_this, (double)priority);
        }
        public double GetPositionPriority() {
            double result = (double)SprExport.Spr_PHIKEndEffectorIf_GetPositionPriority((IntPtr)_this);
            return result;
        }
        public void SetTargetPosition(Vec3d position) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetPosition((IntPtr)_this, (IntPtr)position);
        }
        public Vec3d GetTargetPosition() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetPosition((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void EnableOrientationControl(bool enable) {
            SprExport.Spr_PHIKEndEffectorIf_EnableOrientationControl((IntPtr)_this, (bool)enable);
        }
        public bool IsOrientationControlEnabled() {
            char ret = SprExport.Spr_PHIKEndEffectorIf_IsOrientationControlEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetOrientationPriority(double priority) {
            SprExport.Spr_PHIKEndEffectorIf_SetOrientationPriority((IntPtr)_this, (double)priority);
        }
        public double GetOrientationPriority() {
            double result = (double)SprExport.Spr_PHIKEndEffectorIf_GetOrientationPriority((IntPtr)_this);
            return result;
        }
        public void SetOriCtlMode(PHIKEndEffectorDesc.OriCtlMode mode) {
            SprExport.Spr_PHIKEndEffectorIf_SetOriCtlMode((IntPtr)_this, (int)mode);
        }
        public PHIKEndEffectorDesc.OriCtlMode GetOriCtlMode() {
            int result = SprExport.Spr_PHIKEndEffectorIf_GetOriCtlMode((IntPtr)_this);
            return (PHIKEndEffectorDesc.OriCtlMode)result;
        }
        public void SetTargetOrientation(Quaterniond orientation) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetOrientation((IntPtr)_this, (IntPtr)orientation);
        }
        public Quaterniond GetTargetOrientation() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetOrientation((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
        public void SetTargetDirection(Vec3d direction) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetDirection((IntPtr)_this, (IntPtr)direction);
        }
        public Vec3d GetTargetDirection() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetDirection((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetTargetLookat(Vec3d lookat) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetLookat((IntPtr)_this, (IntPtr)lookat);
        }
        public Vec3d GetTargetLookat() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetLookat((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetTargetVelocity(Vec3d velocity) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetVelocity((IntPtr)_this, (IntPtr)velocity);
        }
        public Vec3d GetTargetVelocity() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetVelocity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetTargetAngularVelocity(Vec3d angVel) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetAngularVelocity((IntPtr)_this, (IntPtr)angVel);
        }
        public Vec3d GetTargetAngularVelocity() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetAngularVelocity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void EnableForceControl(bool enable) {
            SprExport.Spr_PHIKEndEffectorIf_EnableForceControl((IntPtr)_this, (bool)enable);
        }
        public void SetTargetForce(Vec3d force, Vec3d workingPoint) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetForce((IntPtr)_this, (IntPtr)force, (IntPtr)workingPoint);
        }
        public void SetTargetForce(Vec3d force) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetForce_1((IntPtr)_this, (IntPtr)force);
        }
        public Vec3d GetTargetForce() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetForce((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetTargetForceWorkingPoint() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetForceWorkingPoint((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void EnableTorqueControl(bool enable) {
            SprExport.Spr_PHIKEndEffectorIf_EnableTorqueControl((IntPtr)_this, (bool)enable);
        }
        public void SetTargetTorque(Vec3d torque) {
            SprExport.Spr_PHIKEndEffectorIf_SetTargetTorque((IntPtr)_this, (IntPtr)torque);
        }
        public Vec3d GetTargetTorque() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetTargetTorque((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Posed GetSolidTempPose() {
            IntPtr ptr = SprExport.Spr_PHIKEndEffectorIf_GetSolidTempPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
        public void ApplyExactState() {
            SprExport.Spr_PHIKEndEffectorIf_ApplyExactState((IntPtr)_this);
        }
    }
    public partial class PHIKActuatorIf : SceneObjectIf {
        public PHIKActuatorIf() { _this = SprExport.Spr_new_PHIKActuatorIf(); _flag = true; }
        public PHIKActuatorIf(IntPtr ptr) : base(ptr) { }
        public PHIKActuatorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKActuatorIf(CsCastObject target) {
            return (target._info.Inherit(PHIKActuatorIf.GetIfInfoStatic()) ? new PHIKActuatorIf(target._this, target._flag) : null);
        }
        ~PHIKActuatorIf() { if (_flag) { SprExport.Spr_delete_PHIKActuatorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKActuatorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void PrepareSolve() {
            SprExport.Spr_PHIKActuatorIf_PrepareSolve((IntPtr)_this);
        }
        public void ProceedSolve() {
            SprExport.Spr_PHIKActuatorIf_ProceedSolve((IntPtr)_this);
        }
        public void Move() {
            SprExport.Spr_PHIKActuatorIf_Move((IntPtr)_this);
        }
        public void ApplyExactState(bool reverse) {
            SprExport.Spr_PHIKActuatorIf_ApplyExactState((IntPtr)_this, (bool)reverse);
        }
        public void ApplyExactState() {
            SprExport.Spr_PHIKActuatorIf_ApplyExactState_1((IntPtr)_this);
        }
        public void SetBias(float bias) {
            SprExport.Spr_PHIKActuatorIf_SetBias((IntPtr)_this, (float)bias);
        }
        public float GetBias() {
            float result = (float)SprExport.Spr_PHIKActuatorIf_GetBias((IntPtr)_this);
            return result;
        }
        public void SetPullbackRate(double pullbackRate) {
            SprExport.Spr_PHIKActuatorIf_SetPullbackRate((IntPtr)_this, (double)pullbackRate);
        }
        public double GetPullbackRate() {
            double result = (double)SprExport.Spr_PHIKActuatorIf_GetPullbackRate((IntPtr)_this);
            return result;
        }
        public void Enable(bool enable) {
            SprExport.Spr_PHIKActuatorIf_Enable((IntPtr)_this, (bool)enable);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHIKActuatorIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public int NAncestors() {
            int result = (int)SprExport.Spr_PHIKActuatorIf_NAncestors((IntPtr)_this);
            return result;
        }
        public PHIKActuatorIf GetAncestor(int i) {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetAncestor((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public PHIKActuatorIf GetParent() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetParent((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public int NChildActuators() {
            int result = (int)SprExport.Spr_PHIKActuatorIf_NChildActuators((IntPtr)_this);
            return result;
        }
        public PHIKActuatorIf GetChildActuator(int i) {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetChildActuator((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public PHIKEndEffectorIf GetChildEndEffector() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetChildEndEffector((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKEndEffectorIf obj = new PHIKEndEffectorIf(ptr);
            return obj;
        }
        public Posed GetSolidTempPose() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetSolidTempPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
        public Posed GetSolidPullbackPose() {
            IntPtr ptr = SprExport.Spr_PHIKActuatorIf_GetSolidPullbackPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
    }
    public partial class PHIKBallActuatorIf : PHIKActuatorIf {
        public PHIKBallActuatorIf() { _this = SprExport.Spr_new_PHIKBallActuatorIf(); _flag = true; }
        public PHIKBallActuatorIf(IntPtr ptr) : base(ptr) { }
        public PHIKBallActuatorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKBallActuatorIf(CsCastObject target) {
            return (target._info.Inherit(PHIKBallActuatorIf.GetIfInfoStatic()) ? new PHIKBallActuatorIf(target._this, target._flag) : null);
        }
        ~PHIKBallActuatorIf() { if (_flag) { SprExport.Spr_delete_PHIKBallActuatorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKBallActuatorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetJoint(PHBallJointIf joint) {
            SprExport.Spr_PHIKBallActuatorIf_SetJoint((IntPtr)_this, (IntPtr)joint);
        }
        public PHBallJointIf GetJoint() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_GetJoint((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointIf obj = new PHBallJointIf(ptr);
            return obj;
        }
        public void SetJointTempOri(Quaterniond ori) {
            SprExport.Spr_PHIKBallActuatorIf_SetJointTempOri((IntPtr)_this, (IntPtr)ori);
        }
        public void SetPullbackTarget(Quaterniond ori) {
            SprExport.Spr_PHIKBallActuatorIf_SetPullbackTarget((IntPtr)_this, (IntPtr)ori);
        }
        public Quaterniond GetJointTempOri() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_GetJointTempOri((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
        public Quaterniond GetPullbackTarget() {
            IntPtr ptr = SprExport.Spr_PHIKBallActuatorIf_GetPullbackTarget((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
    }
    public partial class PHIKHingeActuatorIf : PHIKActuatorIf {
        public PHIKHingeActuatorIf() { _this = SprExport.Spr_new_PHIKHingeActuatorIf(); _flag = true; }
        public PHIKHingeActuatorIf(IntPtr ptr) : base(ptr) { }
        public PHIKHingeActuatorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKHingeActuatorIf(CsCastObject target) {
            return (target._info.Inherit(PHIKHingeActuatorIf.GetIfInfoStatic()) ? new PHIKHingeActuatorIf(target._this, target._flag) : null);
        }
        ~PHIKHingeActuatorIf() { if (_flag) { SprExport.Spr_delete_PHIKHingeActuatorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKHingeActuatorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKHingeActuatorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKHingeActuatorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKHingeActuatorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetJoint(PHHingeJointIf joint) {
            SprExport.Spr_PHIKHingeActuatorIf_SetJoint((IntPtr)_this, (IntPtr)joint);
        }
        public PHHingeJointIf GetJoint() {
            IntPtr ptr = SprExport.Spr_PHIKHingeActuatorIf_GetJoint((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHHingeJointIf obj = new PHHingeJointIf(ptr);
            return obj;
        }
        public void SetJointTempAngle(double angle) {
            SprExport.Spr_PHIKHingeActuatorIf_SetJointTempAngle((IntPtr)_this, (double)angle);
        }
        public void SetPullbackTarget(double angle) {
            SprExport.Spr_PHIKHingeActuatorIf_SetPullbackTarget((IntPtr)_this, (double)angle);
        }
        public double GetJointTempAngle() {
            double result = (double)SprExport.Spr_PHIKHingeActuatorIf_GetJointTempAngle((IntPtr)_this);
            return result;
        }
        public double GetPullbackTarget() {
            double result = (double)SprExport.Spr_PHIKHingeActuatorIf_GetPullbackTarget((IntPtr)_this);
            return result;
        }
    }
    public partial class PHIKSpringActuatorIf : PHIKActuatorIf {
        public PHIKSpringActuatorIf() { _this = SprExport.Spr_new_PHIKSpringActuatorIf(); _flag = true; }
        public PHIKSpringActuatorIf(IntPtr ptr) : base(ptr) { }
        public PHIKSpringActuatorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHIKSpringActuatorIf(CsCastObject target) {
            return (target._info.Inherit(PHIKSpringActuatorIf.GetIfInfoStatic()) ? new PHIKSpringActuatorIf(target._this, target._flag) : null);
        }
        ~PHIKSpringActuatorIf() { if (_flag) { SprExport.Spr_delete_PHIKSpringActuatorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHIKSpringActuatorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetJoint(PHSpringIf joint) {
            SprExport.Spr_PHIKSpringActuatorIf_SetJoint((IntPtr)_this, (IntPtr)joint);
        }
        public PHSpringIf GetJoint() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_GetJoint((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSpringIf obj = new PHSpringIf(ptr);
            return obj;
        }
        public void SetJointTempPose(Posed pose) {
            SprExport.Spr_PHIKSpringActuatorIf_SetJointTempPose((IntPtr)_this, (IntPtr)pose);
        }
        public void SetPullbackTarget(Posed pose) {
            SprExport.Spr_PHIKSpringActuatorIf_SetPullbackTarget((IntPtr)_this, (IntPtr)pose);
        }
        public Posed GetJointTempPose() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_GetJointTempPose((IntPtr)_this);
            return new Posed(ptr, true);
        }
        public Posed GetPullbackTarget() {
            IntPtr ptr = SprExport.Spr_PHIKSpringActuatorIf_GetPullbackTarget((IntPtr)_this);
            return new Posed(ptr, true);
        }
    }
    public partial class PHConstraintIf : SceneObjectIf {
        public PHConstraintIf() { _this = SprExport.Spr_new_PHConstraintIf(); _flag = true; }
        public PHConstraintIf(IntPtr ptr) : base(ptr) { }
        public PHConstraintIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHConstraintIf(CsCastObject target) {
            return (target._info.Inherit(PHConstraintIf.GetIfInfoStatic()) ? new PHConstraintIf(target._this, target._flag) : null);
        }
        ~PHConstraintIf() { if (_flag) { SprExport.Spr_delete_PHConstraintIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHConstraintIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHSolidIf GetSocketSolid() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHConstraintIf_GetSocketSolid((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    if (ptr == IntPtr.Zero) { return null; }
                    PHSolidIf obj = new PHSolidIf(ptr);
                    obj._thisArray[phSceneIf.sceneForGet] = ptr;
                    if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
                    return obj;
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHConstraintIf_GetSocketSolid((IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHSolidIf obj = new PHSolidIf(ptr);
                if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
                return obj;
            }
        }
        public PHSolidIf GetPlugSolid() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHConstraintIf_GetPlugSolid(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    if (ptr == IntPtr.Zero) { return null; }
                    PHSolidIf obj = new PHSolidIf(ptr);
                    obj._thisArray[phSceneIf.sceneForGet] = ptr;
                    if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
                    return obj;
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHConstraintIf_GetPlugSolid(
                    (IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHSolidIf obj = new PHSolidIf(ptr);
                if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
                return obj;
            }
        }
        public PHSceneIf GetScene() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSceneIf obj = new PHSceneIf(ptr);
            return obj;
        }
        public void Enable(bool bEnable) {
            SprExport.Spr_PHConstraintIf_Enable((IntPtr)_this, (bool)bEnable);
        }
        public void Enable() {
            SprExport.Spr_PHConstraintIf_Enable_1((IntPtr)_this);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHConstraintIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public bool IsArticulated() {
            char ret = SprExport.Spr_PHConstraintIf_IsArticulated((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void GetSocketPose(Posed pose) {
            SprExport.Spr_PHConstraintIf_GetSocketPose((IntPtr)_this, (IntPtr)pose);
        }
        public void SetSocketPose(Posed pose) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newPose = new Posed(pose);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            Console.WriteLine("SetSocketPose in Callback");
                            SprExport.Spr_PHConstraintIf_SetSocketPose((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newPose);
                            SprExport.Spr_PHConstraintIf_SetSocketPose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newPose);
                        });
                        SprExport.Spr_PHConstraintIf_SetSocketPose((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newPose);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHConstraintIf_SetSocketPose((IntPtr)_this, (IntPtr)pose);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHConstraintIf_SetSocketPose((IntPtr)_thisArray[0], (IntPtr)pose);
            }
        }
        public void GetPlugPose(Posed pose) {
            SprExport.Spr_PHConstraintIf_GetPlugPose((IntPtr)_this, (IntPtr)pose);
        }
        public void SetPlugPose(Posed pose) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newPose = new Posed(pose);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHConstraintIf_SetPlugPose((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newPose);
                            SprExport.Spr_PHConstraintIf_SetPlugPose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newPose);
                        });
                        SprExport.Spr_PHConstraintIf_SetPlugPose((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newPose);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHConstraintIf_SetPlugPose((IntPtr)_this, (IntPtr)pose);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHConstraintIf_SetPlugPose((IntPtr)_thisArray[0], (IntPtr)pose);
            }
        }
        public void GetRelativePose(Posed p) {
            SprExport.Spr_PHConstraintIf_GetRelativePose((IntPtr)_this, (IntPtr)p);
        }
        public Vec3d GetRelativePoseR() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetRelativePoseR((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Quaterniond GetRelativePoseQ() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetRelativePoseQ((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
        public Quaternionf GetAbsolutePoseQ() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetAbsolutePoseQ((IntPtr)_this);
            return new Quaternionf(ptr, true);
        }
        public void GetRelativeVelocity(Vec3d v, Vec3d w) {
            SprExport.Spr_PHConstraintIf_GetRelativeVelocity((IntPtr)_this, (IntPtr)v, (IntPtr)w);
        }
        public void GetConstraintForce(Vec3d f, Vec3d t) {
            SprExport.Spr_PHConstraintIf_GetConstraintForce((IntPtr)_this, (IntPtr)f, (IntPtr)t);
        }
        public bool IsYielded() {
            char ret = SprExport.Spr_PHConstraintIf_IsYielded((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHConstraintsIf : SceneObjectIf {
        public PHConstraintsIf() { _this = SprExport.Spr_new_PHConstraintsIf(); _flag = true; }
        public PHConstraintsIf(IntPtr ptr) : base(ptr) { }
        public PHConstraintsIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHConstraintsIf(CsCastObject target) {
            return (target._info.Inherit(PHConstraintsIf.GetIfInfoStatic()) ? new PHConstraintsIf(target._this, target._flag) : null);
        }
        ~PHConstraintsIf() { if (_flag) { SprExport.Spr_delete_PHConstraintsIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHConstraintsIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHConstraintsIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHConstraintsIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHConstraintsIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHConstraintIf FindBySolidPair(PHSolidIf lhs, PHSolidIf rhs) {
            IntPtr ptr = SprExport.Spr_PHConstraintsIf_FindBySolidPair((IntPtr)_this, (IntPtr)lhs, (IntPtr)rhs);
            if (ptr == IntPtr.Zero) { return null; }
            PHConstraintIf obj = new PHConstraintIf(ptr);
            if (obj.GetIfInfo() == PHContactPointIf.GetIfInfoStatic()) { return new PHContactPointIf(ptr); }
            if (obj.GetIfInfo() == PHJointIf.GetIfInfoStatic()) { return new PHJointIf(ptr); }
            if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) { return new PHBallJointIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(ptr); }
            if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) { return new PHSpringIf(ptr); }
            if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) { return new PHMateIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) { return new PHHingeJointIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) { return new PHSliderJointIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) { return new PHPathJointIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) { return new PHGenericJointIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) { return new PHPointToPointMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) { return new PHPointToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) { return new PHPointToPlaneMateIf(ptr); }
            if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) { return new PHLineToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) { return new PHPlaneToPlaneMateIf(ptr); }
            return obj;
        }
        public Vec3d GetTotalForce(PHSolidIf lhs, PHSolidIf rhs) {
            IntPtr ptr = SprExport.Spr_PHConstraintsIf_GetTotalForce((IntPtr)_this, (IntPtr)lhs, (IntPtr)rhs);
            return new Vec3d(ptr, true);
        }
    }
    public partial class PHContactPointIf : PHConstraintIf {
        public PHContactPointIf() { _this = SprExport.Spr_new_PHContactPointIf(); _flag = true; }
        public PHContactPointIf(IntPtr ptr) : base(ptr) { }
        public PHContactPointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHContactPointIf(CsCastObject target) {
            return (target._info.Inherit(PHContactPointIf.GetIfInfoStatic()) ? new PHContactPointIf(target._this, target._flag) : null);
        }
        ~PHContactPointIf() { if (_flag) { SprExport.Spr_delete_PHContactPointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHContactPointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHContactPointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHContactPointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHContactPointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHJointIf : PHConstraintIf {
        public PHJointIf() { _this = SprExport.Spr_new_PHJointIf(); _flag = true; }
        public PHJointIf(IntPtr ptr) : base(ptr) { }
        public PHJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHJointIf(CsCastObject target) {
            return (target._info.Inherit(PHJointIf.GetIfInfoStatic()) ? new PHJointIf(target._this, target._flag) : null);
        }
        ~PHJointIf() { if (_flag) { SprExport.Spr_delete_PHJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetMaxForce(double max) {
            SprExport.Spr_PHJointIf_SetMaxForce((IntPtr)_this, (double)max);
        }
        public double GetMaxForce() {
            double result = (double)SprExport.Spr_PHJointIf_GetMaxForce((IntPtr)_this);
            return result;
        }
    }
    public partial class PH1DJointIf : PHJointIf {
        public PH1DJointIf() { _this = SprExport.Spr_new_PH1DJointIf(); _flag = true; }
        public PH1DJointIf(IntPtr ptr) : base(ptr) { }
        public PH1DJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PH1DJointIf(CsCastObject target) {
            return (target._info.Inherit(PH1DJointIf.GetIfInfoStatic()) ? new PH1DJointIf(target._this, target._flag) : null);
        }
        ~PH1DJointIf() { if (_flag) { SprExport.Spr_delete_PH1DJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PH1DJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PH1DJointLimitIf CreateLimit(PH1DJointLimitDesc desc) {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_CreateLimit((IntPtr)_this, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointLimitIf obj = new PH1DJointLimitIf(ptr);
            return obj;
        }
        public PH1DJointLimitIf CreateLimit() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_CreateLimit_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointLimitIf obj = new PH1DJointLimitIf(ptr);
            return obj;
        }
        public PH1DJointMotorIf CreateMotor(IfInfo ii, PH1DJointMotorDesc desc) {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_CreateMotor((IntPtr)_this, (IntPtr)ii, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointMotorIf obj = new PH1DJointMotorIf(ptr);
            if (obj.GetIfInfo() == PH1DJointNonLinearMotorIf.GetIfInfoStatic()) { return new PH1DJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHuman1DJointResistanceIf.GetIfInfoStatic()) { return new PHHuman1DJointResistanceIf(ptr); }
            return obj;
        }
        public PH1DJointMotorIf CreateMotor(IfInfo ii) {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_CreateMotor_1((IntPtr)_this, (IntPtr)ii);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointMotorIf obj = new PH1DJointMotorIf(ptr);
            if (obj.GetIfInfo() == PH1DJointNonLinearMotorIf.GetIfInfoStatic()) { return new PH1DJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHuman1DJointResistanceIf.GetIfInfoStatic()) { return new PHHuman1DJointResistanceIf(ptr); }
            return obj;
        }
        public bool AddMotor(PH1DJointMotorIf m) {
            char ret = SprExport.Spr_PH1DJointIf_AddMotor((IntPtr)_this, (IntPtr)m);
            return (ret == 0) ? false : true;
        }
        public bool RemoveMotor(int n) {
            char ret = SprExport.Spr_PH1DJointIf_RemoveMotor((IntPtr)_this, (int)n);
            return (ret == 0) ? false : true;
        }
        public bool IsCyclic() {
            char ret = SprExport.Spr_PH1DJointIf_IsCyclic((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetCyclic(bool on) {
            SprExport.Spr_PH1DJointIf_SetCyclic((IntPtr)_this, (bool)on);
        }
        public double GetPosition() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetPosition((IntPtr)_this);
            return result;
        }
        public double GetVelocity() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetVelocity((IntPtr)_this);
            return result;
        }
        public bool HasLimit() {
            char ret = SprExport.Spr_PH1DJointIf_HasLimit((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PH1DJointLimitIf GetLimit() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_GetLimit((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointLimitIf obj = new PH1DJointLimitIf(ptr);
            return obj;
        }
        public void SetSpring(double spring) {
            SprExport.Spr_PH1DJointIf_SetSpring((IntPtr)_this, (double)spring);
        }
        public double GetSpring() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetSpring((IntPtr)_this);
            return result;
        }
        public void SetDamper(double damper) {
            SprExport.Spr_PH1DJointIf_SetDamper((IntPtr)_this, (double)damper);
        }
        public double GetDamper() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetDamper((IntPtr)_this);
            return result;
        }
        public double GetSecondDamper() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetSecondDamper((IntPtr)_this);
            return result;
        }
        public void SetSecondDamper(double input) {
            SprExport.Spr_PH1DJointIf_SetSecondDamper((IntPtr)_this, (double)input);
        }
        public void SetTargetPosition(double targetPosition) {
            SprExport.Spr_PH1DJointIf_SetTargetPosition((IntPtr)_this, (double)targetPosition);
        }
        public double GetTargetPosition() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetTargetPosition((IntPtr)_this);
            return result;
        }
        public void SetTargetVelocity(double v) {
            SprExport.Spr_PH1DJointIf_SetTargetVelocity((IntPtr)_this, (double)v);
        }
        public double GetTargetVelocity() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetTargetVelocity((IntPtr)_this);
            return result;
        }
        public void SetOffsetForce(double dat) {
            SprExport.Spr_PH1DJointIf_SetOffsetForce((IntPtr)_this, (double)dat);
        }
        public double GetOffsetForce() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetOffsetForce((IntPtr)_this);
            return result;
        }
        public void SetOffsetForceN(int n, double dat) {
            SprExport.Spr_PH1DJointIf_SetOffsetForceN((IntPtr)_this, (int)n, (double)dat);
        }
        public double GetOffsetForceN(int n) {
            double result = (double)SprExport.Spr_PH1DJointIf_GetOffsetForceN((IntPtr)_this, (int)n);
            return result;
        }
        public double GetYieldStress() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetYieldStress((IntPtr)_this);
            return result;
        }
        public void SetYieldStress(double yS) {
            SprExport.Spr_PH1DJointIf_SetYieldStress((IntPtr)_this, (double)yS);
        }
        public double GetHardnessRate() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetHardnessRate((IntPtr)_this);
            return result;
        }
        public void SetHardnessRate(double hR) {
            SprExport.Spr_PH1DJointIf_SetHardnessRate((IntPtr)_this, (double)hR);
        }
        public void SetSecondMoment(double sM) {
            SprExport.Spr_PH1DJointIf_SetSecondMoment((IntPtr)_this, (double)sM);
        }
        public double GetSecondMoment() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetSecondMoment((IntPtr)_this);
            return result;
        }
        public int NMotors() {
            int result = (int)SprExport.Spr_PH1DJointIf_NMotors((IntPtr)_this);
            return result;
        }
        public PH1DJointMotorIf GetMotors() {
            IntPtr ptr = SprExport.Spr_PH1DJointIf_GetMotors((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PH1DJointMotorIf obj = new PH1DJointMotorIf(ptr);
            if (obj.GetIfInfo() == PH1DJointNonLinearMotorIf.GetIfInfoStatic()) { return new PH1DJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHuman1DJointResistanceIf.GetIfInfoStatic()) { return new PHHuman1DJointResistanceIf(ptr); }
            return obj;
        }
        public double GetMotorForce() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetMotorForce((IntPtr)_this);
            return result;
        }
        public double GetMotorForceN(int n) {
            double result = (double)SprExport.Spr_PH1DJointIf_GetMotorForceN((IntPtr)_this, (int)n);
            return result;
        }
        public double GetLimitForce() {
            double result = (double)SprExport.Spr_PH1DJointIf_GetLimitForce((IntPtr)_this);
            return result;
        }
    }
    public partial class PHHingeJointIf : PH1DJointIf {
        public PHHingeJointIf() { _this = SprExport.Spr_new_PHHingeJointIf(); _flag = true; }
        public PHHingeJointIf(IntPtr ptr) : base(ptr) { }
        public PHHingeJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHingeJointIf(CsCastObject target) {
            return (target._info.Inherit(PHHingeJointIf.GetIfInfoStatic()) ? new PHHingeJointIf(target._this, target._flag) : null);
        }
        ~PHHingeJointIf() { if (_flag) { SprExport.Spr_delete_PHHingeJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHingeJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHingeJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHingeJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHingeJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHSliderJointIf : PH1DJointIf {
        public PHSliderJointIf() { _this = SprExport.Spr_new_PHSliderJointIf(); _flag = true; }
        public PHSliderJointIf(IntPtr ptr) : base(ptr) { }
        public PHSliderJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSliderJointIf(CsCastObject target) {
            return (target._info.Inherit(PHSliderJointIf.GetIfInfoStatic()) ? new PHSliderJointIf(target._this, target._flag) : null);
        }
        ~PHSliderJointIf() { if (_flag) { SprExport.Spr_delete_PHSliderJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSliderJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSliderJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSliderJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSliderJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPathIf : SceneObjectIf {
        public PHPathIf() { _this = SprExport.Spr_new_PHPathIf(); _flag = true; }
        public PHPathIf(IntPtr ptr) : base(ptr) { }
        public PHPathIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPathIf(CsCastObject target) {
            return (target._info.Inherit(PHPathIf.GetIfInfoStatic()) ? new PHPathIf(target._this, target._flag) : null);
        }
        ~PHPathIf() { if (_flag) { SprExport.Spr_delete_PHPathIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPathIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPathIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPathIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPathIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void AddPoint(double s, Posed pose) {
            SprExport.Spr_PHPathIf_AddPoint((IntPtr)_this, (double)s, (IntPtr)pose);
        }
        public void SetLoop(bool bOnOff) {
            SprExport.Spr_PHPathIf_SetLoop((IntPtr)_this, (bool)bOnOff);
        }
        public void SetLoop() {
            SprExport.Spr_PHPathIf_SetLoop_1((IntPtr)_this);
        }
        public bool IsLoop() {
            char ret = SprExport.Spr_PHPathIf_IsLoop((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHPathJointIf : PH1DJointIf {
        public PHPathJointIf() { _this = SprExport.Spr_new_PHPathJointIf(); _flag = true; }
        public PHPathJointIf(IntPtr ptr) : base(ptr) { }
        public PHPathJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPathJointIf(CsCastObject target) {
            return (target._info.Inherit(PHPathJointIf.GetIfInfoStatic()) ? new PHPathJointIf(target._this, target._flag) : null);
        }
        ~PHPathJointIf() { if (_flag) { SprExport.Spr_delete_PHPathJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPathJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPathJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPathJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPathJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetPosition(double q) {
            SprExport.Spr_PHPathJointIf_SetPosition((IntPtr)_this, (double)q);
        }
    }
    public partial class PHGenericJointIf : PH1DJointIf {
        public PHGenericJointIf() { _this = SprExport.Spr_new_PHGenericJointIf(); _flag = true; }
        public PHGenericJointIf(IntPtr ptr) : base(ptr) { }
        public PHGenericJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHGenericJointIf(CsCastObject target) {
            return (target._info.Inherit(PHGenericJointIf.GetIfInfoStatic()) ? new PHGenericJointIf(target._this, target._flag) : null);
        }
        ~PHGenericJointIf() { if (_flag) { SprExport.Spr_delete_PHGenericJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHGenericJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHGenericJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHGenericJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHGenericJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetCallback(PHGenericJointCallback cb) {
            SprExport.Spr_PHGenericJointIf_SetCallback((IntPtr)_this, (IntPtr)cb);
        }
        public void SetParam(string name, double value) {
            IntPtr remote1 = Marshal.StringToBSTR(name);
            SprExport.Spr_PHGenericJointIf_SetParam((IntPtr)_this, (IntPtr)remote1, (double)value);
            Marshal.FreeBSTR(remote1);
        }
    }
    public partial class PHBallJointIf : PHJointIf {
        public PHBallJointIf() { _this = SprExport.Spr_new_PHBallJointIf(); _flag = true; }
        public PHBallJointIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointIf.GetIfInfoStatic()) ? new PHBallJointIf(target._this, target._flag) : null);
        }
        ~PHBallJointIf() { if (_flag) { SprExport.Spr_delete_PHBallJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHBallJointLimitIf CreateLimit(IfInfo ii, PHBallJointLimitDesc desc) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_CreateLimit((IntPtr)_this, (IntPtr)ii, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointLimitIf obj = new PHBallJointLimitIf(ptr);
            if (obj.GetIfInfo() == PHBallJointConeLimitIf.GetIfInfoStatic()) { return new PHBallJointConeLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointSplineLimitIf.GetIfInfoStatic()) { return new PHBallJointSplineLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIndependentLimitIf.GetIfInfoStatic()) { return new PHBallJointIndependentLimitIf(ptr); }
            return obj;
        }
        public PHBallJointLimitIf CreateLimit(IfInfo ii) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_CreateLimit_1((IntPtr)_this, (IntPtr)ii);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointLimitIf obj = new PHBallJointLimitIf(ptr);
            if (obj.GetIfInfo() == PHBallJointConeLimitIf.GetIfInfoStatic()) { return new PHBallJointConeLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointSplineLimitIf.GetIfInfoStatic()) { return new PHBallJointSplineLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIndependentLimitIf.GetIfInfoStatic()) { return new PHBallJointIndependentLimitIf(ptr); }
            return obj;
        }
        public PHBallJointMotorIf CreateMotor(IfInfo ii, PHBallJointMotorDesc desc) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_CreateMotor((IntPtr)_this, (IntPtr)ii, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointMotorIf obj = new PHBallJointMotorIf(ptr);
            if (obj.GetIfInfo() == PHBallJointNonLinearMotorIf.GetIfInfoStatic()) { return new PHBallJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHumanBallJointResistanceIf.GetIfInfoStatic()) { return new PHHumanBallJointResistanceIf(ptr); }
            return obj;
        }
        public PHBallJointMotorIf CreateMotor(IfInfo ii) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_CreateMotor_1((IntPtr)_this, (IntPtr)ii);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointMotorIf obj = new PHBallJointMotorIf(ptr);
            if (obj.GetIfInfo() == PHBallJointNonLinearMotorIf.GetIfInfoStatic()) { return new PHBallJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHumanBallJointResistanceIf.GetIfInfoStatic()) { return new PHHumanBallJointResistanceIf(ptr); }
            return obj;
        }
        public bool AddMotor(PHBallJointMotorIf m) {
            char ret = SprExport.Spr_PHBallJointIf_AddMotor((IntPtr)_this, (IntPtr)m);
            return (ret == 0) ? false : true;
        }
        public bool RemoveMotor(int n) {
            char ret = SprExport.Spr_PHBallJointIf_RemoveMotor((IntPtr)_this, (int)n);
            return (ret == 0) ? false : true;
        }
        public Vec3d GetAngle() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetAngle((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Quaterniond GetPosition() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetPosition((IntPtr)_this);
            return new Quaterniond(ptr, true);
        }
        public Vec3d GetVelocity() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetVelocity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public bool HasLimit() {
            char ret = SprExport.Spr_PHBallJointIf_HasLimit((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public PHBallJointLimitIf GetLimit() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetLimit((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointLimitIf obj = new PHBallJointLimitIf(ptr);
            if (obj.GetIfInfo() == PHBallJointConeLimitIf.GetIfInfoStatic()) { return new PHBallJointConeLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointSplineLimitIf.GetIfInfoStatic()) { return new PHBallJointSplineLimitIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIndependentLimitIf.GetIfInfoStatic()) { return new PHBallJointIndependentLimitIf(ptr); }
            return obj;
        }
        public void SetSpring(double spring) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        //Console.WriteLine("PHBallJoint SetSpring in isStepping");
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            //Console.WriteLine("SetSpring in CallBack");
                            SprExport.Spr_PHBallJointIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForStep], (double)spring);
                            SprExport.Spr_PHBallJointIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (double)spring);
                        });
                        //Console.WriteLine("SetSpring in isStepping");
                        SprExport.Spr_PHBallJointIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForGet], (double)spring);
                    } else {
                        //Console.WriteLine("SetSpring not in isStepping");
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetSpring((IntPtr)_this, (double)spring);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetSpring((IntPtr)_thisArray[0], (double)spring);
            }
        }
        public double GetSpring() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    double result = SprExport.Spr_PHBallJointIf_GetSpring(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return result;
                }
            } else {
                double result = SprExport.Spr_PHBallJointIf_GetSpring(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return result;
            }
        }
        public void SetDamper(double damper) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBallJointIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForStep], (double)damper);
                            SprExport.Spr_PHBallJointIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (double)damper);
                        });
                        SprExport.Spr_PHBallJointIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForGet], (double)damper);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetDamper((IntPtr)_this, (double)damper);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetDamper((IntPtr)_thisArray[0], (double)damper);
            }
        }
        public double GetDamper() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    double result = SprExport.Spr_PHBallJointIf_GetDamper(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return result;
                }
            } else {
                double result = SprExport.Spr_PHBallJointIf_GetDamper(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return result;
            }
        }
        public Vec3d GetSecondDamper() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetSecondDamper((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetSecondDamper(Vec3d damper2) {
            SprExport.Spr_PHBallJointIf_SetSecondDamper((IntPtr)_this, (IntPtr)damper2);
        }
        public void SetTargetPosition(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        //Console.WriteLine("SetTargetPosition isStepping");
                        var newQ = new Quaterniond(q);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newQ);
                            SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newQ);
                        });
                        SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newQ);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)q);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[0], (IntPtr)q);
            }
        }
        public Quaterniond GetTargetPosition() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHBallJointIf_GetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    return new Quaterniond(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHBallJointIf_GetTargetPosition((IntPtr)_thisArray[0]);
                return new Quaterniond(ptr, true);
            }
        }
        public void SetTargetVelocity(Vec3d v) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec3d(v);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV);
                            SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        });
                        SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[0], (IntPtr)v);
            }
        }
        public Vec3d GetTargetVelocity() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        IntPtr ptr = SprExport.Spr_PHBallJointIf_GetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep]);
                        return new Vec3d(ptr, true);
                    }
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        phSceneIf.isFixedUpdating = true;
                        IntPtr ptr = SprExport.Spr_PHBallJointIf_GetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                        return new Vec3d(ptr, true);
                    }
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHBallJointIf_GetTargetVelocity((IntPtr)_thisArray[0]);
                return new Vec3d(ptr, true);
            }
            return null;
        }
        public void SetOffsetForce(Vec3d ofst) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec3d(ofst);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHBallJointIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV);
                            SprExport.Spr_PHBallJointIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        });
                        SprExport.Spr_PHBallJointIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetOffsetForce((IntPtr)_this, (IntPtr)ofst);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetOffsetForce((IntPtr)_thisArray[0], (IntPtr)ofst);
            }
        }
        public Vec3d GetOffsetForce() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetOffsetForce((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetOffsetForceN(int n, Vec3d ofst) {
            SprExport.Spr_PHBallJointIf_SetOffsetForceN((IntPtr)_this, (int)n, (IntPtr)ofst);
        }
        public Vec3d GetOffsetForceN(int n) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetOffsetForceN((IntPtr)_this, (int)n);
            return new Vec3d(ptr, true);
        }
        public void SetYieldStress(double yS) {
            SprExport.Spr_PHBallJointIf_SetYieldStress((IntPtr)_this, (double)yS);
        }
        public double GetYieldStress() {
            double result = (double)SprExport.Spr_PHBallJointIf_GetYieldStress((IntPtr)_this);
            return result;
        }
        public void SetHardnessRate(double hR) {
            SprExport.Spr_PHBallJointIf_SetHardnessRate((IntPtr)_this, (double)hR);
        }
        public double GetHardnessRate() {
            double result = (double)SprExport.Spr_PHBallJointIf_GetHardnessRate((IntPtr)_this);
            return result;
        }
        public void SetSecondMoment(Vec3d m) {
            SprExport.Spr_PHBallJointIf_SetSecondMoment((IntPtr)_this, (IntPtr)m);
        }
        public Vec3d GetSecondMoment() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetSecondMoment((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public int NMotors() {
            int result = (int)SprExport.Spr_PHBallJointIf_NMotors((IntPtr)_this);
            return result;
        }
        public PHBallJointMotorIf GetMotors() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetMotors((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHBallJointMotorIf obj = new PHBallJointMotorIf(ptr);
            if (obj.GetIfInfo() == PHBallJointNonLinearMotorIf.GetIfInfoStatic()) { return new PHBallJointNonLinearMotorIf(ptr); }
            if (obj.GetIfInfo() == PHHumanBallJointResistanceIf.GetIfInfoStatic()) { return new PHHumanBallJointResistanceIf(ptr); }
            return obj;
        }
        public Vec3d GetMotorForce() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHBallJointIf_GetMotorForce((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    return new Vec3d(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHBallJointIf_GetMotorForce((IntPtr)_thisArray[0]);
                return new Vec3d(ptr, true);
            }
        }
        public Vec3d GetMotorForceN(int n) {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetMotorForceN((IntPtr)_this, (int)n);
            return new Vec3d(ptr, true);
        }
        public Vec3d GetLimitForce() {
            IntPtr ptr = SprExport.Spr_PHBallJointIf_GetLimitForce((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
    }
    public partial class PHFixJointIf : PHJointIf {
        public PHFixJointIf() { _this = SprExport.Spr_new_PHFixJointIf(); _flag = true; }
        public PHFixJointIf(IntPtr ptr) : base(ptr) { }
        public PHFixJointIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFixJointIf(CsCastObject target) {
            return (target._info.Inherit(PHFixJointIf.GetIfInfoStatic()) ? new PHFixJointIf(target._this, target._flag) : null);
        }
        ~PHFixJointIf() { if (_flag) { SprExport.Spr_delete_PHFixJointIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFixJointIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFixJointIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFixJointIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFixJointIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHSpringIf : PHJointIf {
        public PHSpringIf() { _this = SprExport.Spr_new_PHSpringIf(); _flag = true; }
        public PHSpringIf(IntPtr ptr) : base(ptr) { }
        public PHSpringIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSpringIf(CsCastObject target) {
            return (target._info.Inherit(PHSpringIf.GetIfInfoStatic()) ? new PHSpringIf(target._this, target._flag) : null);
        }
        ~PHSpringIf() { if (_flag) { SprExport.Spr_delete_PHSpringIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSpringIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSpringIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSpringIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSpringIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetTargetPosition(Vec3d targetPosition) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Vec3d(targetPosition);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newP);
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newP);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)targetPosition);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[0], (IntPtr)targetPosition);
            }
        }
        public Vec3d GetTargetPosition() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep]);
                        return new Vec3d(ptr, true);
                    }
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        phSceneIf.isFixedUpdating = true;
                        IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                        return new Vec3d(ptr, true);
                    }
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetPosition((IntPtr)_thisArray[0]);
                return new Vec3d(ptr, true);
            }
            return null;
        }
        public void SetTargetOrientation(Quaterniond targetOrientation) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newQ = new Quaterniond(targetOrientation);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newQ);
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newQ);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newQ);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)targetOrientation);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[0], (IntPtr)targetOrientation);
            }
        }
        public Quaterniond GetTargetOrientation() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    return new Quaterniond(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetOrientation((IntPtr)_thisArray[0]);
                return new Quaterniond(ptr, true);
            }
        }
        public void SetSpring(Vec3d spring) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newSpring = new Vec3d(spring);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForStep], newSpring);
                            SprExport.Spr_PHSpringIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForBuffer], newSpring);
                        });
                        SprExport.Spr_PHSpringIf_SetSpring((IntPtr)_thisArray[phSceneIf.sceneForGet], newSpring);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetSpring((IntPtr)_this, spring);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetSpring((IntPtr)_thisArray[0], spring);
            }
        }
        public Vec3d GetSpring() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        IntPtr ptr = SprExport.Spr_PHSpringIf_GetSpring((IntPtr)_thisArray[phSceneIf.sceneForStep]);
                        return new Vec3d(ptr, true);
                    }
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        phSceneIf.isFixedUpdating = true;
                        IntPtr ptr = SprExport.Spr_PHSpringIf_GetSpring(
                            (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                        return new Vec3d(ptr, true);
                    }
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetSpring(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return new Vec3d(ptr, true);
            }
            return null;
        }
        public void SetDamper(Vec3d damper) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newDamper = new Vec3d(damper);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForStep], newDamper);
                            SprExport.Spr_PHSpringIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForBuffer], newDamper);
                        });
                        SprExport.Spr_PHSpringIf_SetDamper((IntPtr)_thisArray[phSceneIf.sceneForGet], newDamper);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetDamper((IntPtr)_this, damper);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetDamper((IntPtr)_thisArray[0], damper);
            }
        }
        public Vec3d GetDamper() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHSpringIf_GetDamper(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return new Vec3d(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetDamper(
                    (IntPtr)_thisArray[0]);
                return new Vec3d(ptr, true);
            }
        }
        public void SetSecondDamper(Vec3d secondDamper) {
            SprExport.Spr_PHSpringIf_SetSecondDamper((IntPtr)_this, (IntPtr)secondDamper);
        }
        public Vec3d GetSecondDamper() {
            IntPtr ptr = SprExport.Spr_PHSpringIf_GetSecondDamper((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetSpringOri(double spring) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetSpringOri((IntPtr)_thisArray[phSceneIf.sceneForStep], (double)spring);
                            SprExport.Spr_PHSpringIf_SetSpringOri((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (double)spring);
                        });
                        SprExport.Spr_PHSpringIf_SetSpringOri((IntPtr)_thisArray[phSceneIf.sceneForGet], (double)spring);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetSpringOri((IntPtr)_this, (double)spring);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetSpringOri((IntPtr)_thisArray[0], (double)spring);
            }
        }
        public double GetSpringOri() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    double result = SprExport.Spr_PHSpringIf_GetSpringOri(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return result;
                }
            } else {
                double result = SprExport.Spr_PHSpringIf_GetSpringOri(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return result;
            }
        }
        public void SetDamperOri(double damper) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetDamperOri((IntPtr)_thisArray[phSceneIf.sceneForStep], (double)damper);
                            SprExport.Spr_PHSpringIf_SetDamperOri((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (double)damper);
                        });
                        SprExport.Spr_PHSpringIf_SetDamperOri((IntPtr)_thisArray[phSceneIf.sceneForGet], (double)damper);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetDamperOri((IntPtr)_this, (double)damper);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetDamperOri((IntPtr)_thisArray[0], (double)damper);
            }
        }
        public double GetDamperOri() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    double result = SprExport.Spr_PHSpringIf_GetDamperOri(
                        (IntPtr)_thisArray[phSceneIf.sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return result;
                }
            } else {
                double result = SprExport.Spr_PHSpringIf_GetDamperOri(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return result;
            }
        }
        public void SetSecondDamperOri(double secondDamperOri) {
            SprExport.Spr_PHSpringIf_SetSecondDamperOri((IntPtr)_this, (double)secondDamperOri);
        }
        public double GetSecondDamperOri() {
            double result = (double)SprExport.Spr_PHSpringIf_GetSecondDamperOri((IntPtr)_this);
            return result;
        }
        public void SetYieldStress(double yieldStress) {
            SprExport.Spr_PHSpringIf_SetYieldStress((IntPtr)_this, (double)yieldStress);
        }
        public double GetYieldStress() {
            double result = (double)SprExport.Spr_PHSpringIf_GetYieldStress((IntPtr)_this);
            return result;
        }
        public void SetHardnessRate(double hardnessRate) {
            SprExport.Spr_PHSpringIf_SetHardnessRate((IntPtr)_this, (double)hardnessRate);
        }
        public double GetHardnessRate() {
            double result = (double)SprExport.Spr_PHSpringIf_GetHardnessRate((IntPtr)_this);
            return result;
        }
        public void SetSecondMoment(Vec3d secondMoment) {
            SprExport.Spr_PHSpringIf_SetSecondMoment((IntPtr)_this, (IntPtr)secondMoment);
        }
        public Vec3d GetSecondMoment() {
            IntPtr ptr = SprExport.Spr_PHSpringIf_GetSecondMoment((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public Vec6d GetMotorForce() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHSpringIf_GetMotorForce((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    return new Vec6d(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetMotorForce((IntPtr)_thisArray[0]);
                return new Vec6d(ptr, true);
            }
        }
        public void SetOffsetForce(Vec6d offsetForce) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec6d(offsetForce);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV);
                            SprExport.Spr_PHSpringIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        });
                        SprExport.Spr_PHSpringIf_SetOffsetForce((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetOffsetForce((IntPtr)_this, (IntPtr)offsetForce);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetOffsetForce((IntPtr)_thisArray[0], (IntPtr)offsetForce);
            }
        }
        public void SetTargetVelocity(Vec6d targetVelocity) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec6d(targetVelocity);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV);
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)targetVelocity);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[0], (IntPtr)targetVelocity);
            }
        }
        public Vec6d GetTargetVelocity() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet]);
                    return new Vec6d(ptr, true);
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSpringIf_GetTargetVelocity((IntPtr)_thisArray[0]);
                return new Vec6d(ptr, true);
            }
        }
    }
    public partial class PHMateIf : PHJointIf {
        public PHMateIf() { _this = SprExport.Spr_new_PHMateIf(); _flag = true; }
        public PHMateIf(IntPtr ptr) : base(ptr) { }
        public PHMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHMateIf(CsCastObject target) {
            return (target._info.Inherit(PHMateIf.GetIfInfoStatic()) ? new PHMateIf(target._this, target._flag) : null);
        }
        ~PHMateIf() { if (_flag) { SprExport.Spr_delete_PHMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPointToPointMateIf : PHMateIf {
        public PHPointToPointMateIf() { _this = SprExport.Spr_new_PHPointToPointMateIf(); _flag = true; }
        public PHPointToPointMateIf(IntPtr ptr) : base(ptr) { }
        public PHPointToPointMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPointToPointMateIf(CsCastObject target) {
            return (target._info.Inherit(PHPointToPointMateIf.GetIfInfoStatic()) ? new PHPointToPointMateIf(target._this, target._flag) : null);
        }
        ~PHPointToPointMateIf() { if (_flag) { SprExport.Spr_delete_PHPointToPointMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPointToPointMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPointToPointMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPointToPointMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPointToPointMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPointToLineMateIf : PHMateIf {
        public PHPointToLineMateIf() { _this = SprExport.Spr_new_PHPointToLineMateIf(); _flag = true; }
        public PHPointToLineMateIf(IntPtr ptr) : base(ptr) { }
        public PHPointToLineMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPointToLineMateIf(CsCastObject target) {
            return (target._info.Inherit(PHPointToLineMateIf.GetIfInfoStatic()) ? new PHPointToLineMateIf(target._this, target._flag) : null);
        }
        ~PHPointToLineMateIf() { if (_flag) { SprExport.Spr_delete_PHPointToLineMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPointToLineMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPointToLineMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPointToLineMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPointToLineMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPointToPlaneMateIf : PHMateIf {
        public PHPointToPlaneMateIf() { _this = SprExport.Spr_new_PHPointToPlaneMateIf(); _flag = true; }
        public PHPointToPlaneMateIf(IntPtr ptr) : base(ptr) { }
        public PHPointToPlaneMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPointToPlaneMateIf(CsCastObject target) {
            return (target._info.Inherit(PHPointToPlaneMateIf.GetIfInfoStatic()) ? new PHPointToPlaneMateIf(target._this, target._flag) : null);
        }
        ~PHPointToPlaneMateIf() { if (_flag) { SprExport.Spr_delete_PHPointToPlaneMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPointToPlaneMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPointToPlaneMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPointToPlaneMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPointToPlaneMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetRange(Vec2d range) {
            SprExport.Spr_PHPointToPlaneMateIf_SetRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetRange(Vec2d range) {
            SprExport.Spr_PHPointToPlaneMateIf_GetRange((IntPtr)_this, (IntPtr)range);
        }
    }
    public partial class PHLineToLineMateIf : PHMateIf {
        public PHLineToLineMateIf() { _this = SprExport.Spr_new_PHLineToLineMateIf(); _flag = true; }
        public PHLineToLineMateIf(IntPtr ptr) : base(ptr) { }
        public PHLineToLineMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHLineToLineMateIf(CsCastObject target) {
            return (target._info.Inherit(PHLineToLineMateIf.GetIfInfoStatic()) ? new PHLineToLineMateIf(target._this, target._flag) : null);
        }
        ~PHLineToLineMateIf() { if (_flag) { SprExport.Spr_delete_PHLineToLineMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHLineToLineMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHLineToLineMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHLineToLineMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHLineToLineMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPlaneToPlaneMateIf : PHMateIf {
        public PHPlaneToPlaneMateIf() { _this = SprExport.Spr_new_PHPlaneToPlaneMateIf(); _flag = true; }
        public PHPlaneToPlaneMateIf(IntPtr ptr) : base(ptr) { }
        public PHPlaneToPlaneMateIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPlaneToPlaneMateIf(CsCastObject target) {
            return (target._info.Inherit(PHPlaneToPlaneMateIf.GetIfInfoStatic()) ? new PHPlaneToPlaneMateIf(target._this, target._flag) : null);
        }
        ~PHPlaneToPlaneMateIf() { if (_flag) { SprExport.Spr_delete_PHPlaneToPlaneMateIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPlaneToPlaneMateIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPlaneToPlaneMateIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPlaneToPlaneMateIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPlaneToPlaneMateIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHTreeNodeIf : SceneObjectIf {
        public PHTreeNodeIf() { _this = SprExport.Spr_new_PHTreeNodeIf(); _flag = true; }
        public PHTreeNodeIf(IntPtr ptr) : base(ptr) { }
        public PHTreeNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHTreeNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHTreeNodeIf.GetIfInfoStatic()) ? new PHTreeNodeIf(target._this, target._flag) : null);
        }
        ~PHTreeNodeIf() { if (_flag) { SprExport.Spr_delete_PHTreeNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHTreeNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void Enable(bool bEnable) {
            SprExport.Spr_PHTreeNodeIf_Enable((IntPtr)_this, (bool)bEnable);
        }
        public void Enable() {
            SprExport.Spr_PHTreeNodeIf_Enable_1((IntPtr)_this);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHTreeNodeIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public int NChildren() {
            int result = (int)SprExport.Spr_PHTreeNodeIf_NChildren((IntPtr)_this);
            return result;
        }
        public PHTreeNodeIf GetParentNode() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetParentNode((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHTreeNodeIf obj = new PHTreeNodeIf(ptr);
            if (obj.GetIfInfo() == PHRootNodeIf.GetIfInfoStatic()) { return new PHRootNodeIf(ptr); }
            if (obj.GetIfInfo() == PHTreeNode1DIf.GetIfInfoStatic()) { return new PHTreeNode1DIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointNodeIf.GetIfInfoStatic()) { return new PHBallJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointNodeIf.GetIfInfoStatic()) { return new PHFixJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointNodeIf.GetIfInfoStatic()) { return new PHHingeJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointNodeIf.GetIfInfoStatic()) { return new PHSliderJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointNodeIf.GetIfInfoStatic()) { return new PHPathJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointNodeIf.GetIfInfoStatic()) { return new PHGenericJointNodeIf(ptr); }
            return obj;
        }
        public PHTreeNodeIf GetChildNode(int i) {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetChildNode((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHTreeNodeIf obj = new PHTreeNodeIf(ptr);
            if (obj.GetIfInfo() == PHRootNodeIf.GetIfInfoStatic()) { return new PHRootNodeIf(ptr); }
            if (obj.GetIfInfo() == PHTreeNode1DIf.GetIfInfoStatic()) { return new PHTreeNode1DIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointNodeIf.GetIfInfoStatic()) { return new PHBallJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointNodeIf.GetIfInfoStatic()) { return new PHFixJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointNodeIf.GetIfInfoStatic()) { return new PHHingeJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointNodeIf.GetIfInfoStatic()) { return new PHSliderJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointNodeIf.GetIfInfoStatic()) { return new PHPathJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointNodeIf.GetIfInfoStatic()) { return new PHGenericJointNodeIf(ptr); }
            return obj;
        }
        public PHRootNodeIf GetRootNode() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetRootNode((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHRootNodeIf obj = new PHRootNodeIf(ptr);
            return obj;
        }
        public PHSolidIf GetSolid() {
            IntPtr ptr = SprExport.Spr_PHTreeNodeIf_GetSolid((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
    }
    public partial class PHRootNodeIf : PHTreeNodeIf {
        public PHRootNodeIf() { _this = SprExport.Spr_new_PHRootNodeIf(); _flag = true; }
        public PHRootNodeIf(IntPtr ptr) : base(ptr) { }
        public PHRootNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHRootNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHRootNodeIf.GetIfInfoStatic()) ? new PHRootNodeIf(target._this, target._flag) : null);
        }
        ~PHRootNodeIf() { if (_flag) { SprExport.Spr_delete_PHRootNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHRootNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHRootNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHRootNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHRootNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHTreeNode1DIf : PHTreeNodeIf {
        public PHTreeNode1DIf() { _this = SprExport.Spr_new_PHTreeNode1DIf(); _flag = true; }
        public PHTreeNode1DIf(IntPtr ptr) : base(ptr) { }
        public PHTreeNode1DIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHTreeNode1DIf(CsCastObject target) {
            return (target._info.Inherit(PHTreeNode1DIf.GetIfInfoStatic()) ? new PHTreeNode1DIf(target._this, target._flag) : null);
        }
        ~PHTreeNode1DIf() { if (_flag) { SprExport.Spr_delete_PHTreeNode1DIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHTreeNode1DIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHTreeNode1DIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHTreeNode1DIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHTreeNode1DIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHHingeJointNodeIf : PHTreeNode1DIf {
        public PHHingeJointNodeIf() { _this = SprExport.Spr_new_PHHingeJointNodeIf(); _flag = true; }
        public PHHingeJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHHingeJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHingeJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHHingeJointNodeIf.GetIfInfoStatic()) ? new PHHingeJointNodeIf(target._this, target._flag) : null);
        }
        ~PHHingeJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHHingeJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHingeJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHingeJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHingeJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHingeJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHSliderJointNodeIf : PHTreeNode1DIf {
        public PHSliderJointNodeIf() { _this = SprExport.Spr_new_PHSliderJointNodeIf(); _flag = true; }
        public PHSliderJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHSliderJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSliderJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHSliderJointNodeIf.GetIfInfoStatic()) ? new PHSliderJointNodeIf(target._this, target._flag) : null);
        }
        ~PHSliderJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHSliderJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSliderJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSliderJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSliderJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSliderJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHPathJointNodeIf : PHTreeNode1DIf {
        public PHPathJointNodeIf() { _this = SprExport.Spr_new_PHPathJointNodeIf(); _flag = true; }
        public PHPathJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHPathJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHPathJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHPathJointNodeIf.GetIfInfoStatic()) ? new PHPathJointNodeIf(target._this, target._flag) : null);
        }
        ~PHPathJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHPathJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHPathJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHPathJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHPathJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHPathJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHGenericJointNodeIf : PHTreeNode1DIf {
        public PHGenericJointNodeIf() { _this = SprExport.Spr_new_PHGenericJointNodeIf(); _flag = true; }
        public PHGenericJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHGenericJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHGenericJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHGenericJointNodeIf.GetIfInfoStatic()) ? new PHGenericJointNodeIf(target._this, target._flag) : null);
        }
        ~PHGenericJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHGenericJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHGenericJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHGenericJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHGenericJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHGenericJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHBallJointNodeIf : PHTreeNodeIf {
        public PHBallJointNodeIf() { _this = SprExport.Spr_new_PHBallJointNodeIf(); _flag = true; }
        public PHBallJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointNodeIf.GetIfInfoStatic()) ? new PHBallJointNodeIf(target._this, target._flag) : null);
        }
        ~PHBallJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHBallJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHFixJointNodeIf : PHTreeNodeIf {
        public PHFixJointNodeIf() { _this = SprExport.Spr_new_PHFixJointNodeIf(); _flag = true; }
        public PHFixJointNodeIf(IntPtr ptr) : base(ptr) { }
        public PHFixJointNodeIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHFixJointNodeIf(CsCastObject target) {
            return (target._info.Inherit(PHFixJointNodeIf.GetIfInfoStatic()) ? new PHFixJointNodeIf(target._this, target._flag) : null);
        }
        ~PHFixJointNodeIf() { if (_flag) { SprExport.Spr_delete_PHFixJointNodeIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHFixJointNodeIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHFixJointNodeIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHFixJointNodeIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHFixJointNodeIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHGearIf : SceneObjectIf {
        public PHGearIf() { _this = SprExport.Spr_new_PHGearIf(); _flag = true; }
        public PHGearIf(IntPtr ptr) : base(ptr) { }
        public PHGearIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHGearIf(CsCastObject target) {
            return (target._info.Inherit(PHGearIf.GetIfInfoStatic()) ? new PHGearIf(target._this, target._flag) : null);
        }
        ~PHGearIf() { if (_flag) { SprExport.Spr_delete_PHGearIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHGearIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHGearIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHGearIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHGearIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void Enable(bool bEnable) {
            SprExport.Spr_PHGearIf_Enable((IntPtr)_this, (bool)bEnable);
        }
        public void Enable() {
            SprExport.Spr_PHGearIf_Enable_1((IntPtr)_this);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHGearIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetRatio(double ratio) {
            SprExport.Spr_PHGearIf_SetRatio((IntPtr)_this, (double)ratio);
        }
        public double GetRatio() {
            double result = (double)SprExport.Spr_PHGearIf_GetRatio((IntPtr)_this);
            return result;
        }
        public void SetOffset(double offset) {
            SprExport.Spr_PHGearIf_SetOffset((IntPtr)_this, (double)offset);
        }
        public double GetOffset() {
            double result = (double)SprExport.Spr_PHGearIf_GetOffset((IntPtr)_this);
            return result;
        }
        public void SetMode(int mode) {
            SprExport.Spr_PHGearIf_SetMode((IntPtr)_this, (int)mode);
        }
        public int GetMode() {
            int result = (int)SprExport.Spr_PHGearIf_GetMode((IntPtr)_this);
            return result;
        }
    }
    public partial class PH1DJointLimitIf : SceneObjectIf {
        public PH1DJointLimitIf() { _this = SprExport.Spr_new_PH1DJointLimitIf(); _flag = true; }
        public PH1DJointLimitIf(IntPtr ptr) : base(ptr) { }
        public PH1DJointLimitIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PH1DJointLimitIf(CsCastObject target) {
            return (target._info.Inherit(PH1DJointLimitIf.GetIfInfoStatic()) ? new PH1DJointLimitIf(target._this, target._flag) : null);
        }
        ~PH1DJointLimitIf() { if (_flag) { SprExport.Spr_delete_PH1DJointLimitIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PH1DJointLimitIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PH1DJointLimitIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PH1DJointLimitIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PH1DJointLimitIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetRange(Vec2d range) {
            SprExport.Spr_PH1DJointLimitIf_SetRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetRange(Vec2d range) {
            SprExport.Spr_PH1DJointLimitIf_GetRange((IntPtr)_this, (IntPtr)range);
        }
        public void SetSpring(double spring) {
            SprExport.Spr_PH1DJointLimitIf_SetSpring((IntPtr)_this, (double)spring);
        }
        public double GetSpring() {
            double result = (double)SprExport.Spr_PH1DJointLimitIf_GetSpring((IntPtr)_this);
            return result;
        }
        public void SetDamper(double damper) {
            SprExport.Spr_PH1DJointLimitIf_SetDamper((IntPtr)_this, (double)damper);
        }
        public double GetDamper() {
            double result = (double)SprExport.Spr_PH1DJointLimitIf_GetDamper((IntPtr)_this);
            return result;
        }
        public bool IsOnLimit() {
            char ret = SprExport.Spr_PH1DJointLimitIf_IsOnLimit((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void Enable(bool b) {
            SprExport.Spr_PH1DJointLimitIf_Enable((IntPtr)_this, (bool)b);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PH1DJointLimitIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHBallJointLimitIf : SceneObjectIf {
        public PHBallJointLimitIf() { _this = SprExport.Spr_new_PHBallJointLimitIf(); _flag = true; }
        public PHBallJointLimitIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointLimitIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointLimitIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointLimitIf.GetIfInfoStatic()) ? new PHBallJointLimitIf(target._this, target._flag) : null);
        }
        ~PHBallJointLimitIf() { if (_flag) { SprExport.Spr_delete_PHBallJointLimitIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointLimitIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointLimitIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointLimitIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointLimitIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetSpring(double rSpring) {
            SprExport.Spr_PHBallJointLimitIf_SetSpring((IntPtr)_this, (double)rSpring);
        }
        public double GetSpring() {
            double result = (double)SprExport.Spr_PHBallJointLimitIf_GetSpring((IntPtr)_this);
            return result;
        }
        public void SetDamper(double rDamper) {
            SprExport.Spr_PHBallJointLimitIf_SetDamper((IntPtr)_this, (double)rDamper);
        }
        public double GetDamper() {
            double result = (double)SprExport.Spr_PHBallJointLimitIf_GetDamper((IntPtr)_this);
            return result;
        }
        public void SetLimitDir(Vec3d limDir) {
            SprExport.Spr_PHBallJointLimitIf_SetLimitDir((IntPtr)_this, (IntPtr)limDir);
        }
        public Vec3d GetLimitDir() {
            IntPtr ptr = SprExport.Spr_PHBallJointLimitIf_GetLimitDir((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public bool IsOnLimit() {
            char ret = SprExport.Spr_PHBallJointLimitIf_IsOnLimit((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void Enable(bool b) {
            SprExport.Spr_PHBallJointLimitIf_Enable((IntPtr)_this, (bool)b);
        }
        public bool IsEnabled() {
            char ret = SprExport.Spr_PHBallJointLimitIf_IsEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHBallJointConeLimitIf : PHBallJointLimitIf {
        public PHBallJointConeLimitIf() { _this = SprExport.Spr_new_PHBallJointConeLimitIf(); _flag = true; }
        public PHBallJointConeLimitIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointConeLimitIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointConeLimitIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointConeLimitIf.GetIfInfoStatic()) ? new PHBallJointConeLimitIf(target._this, target._flag) : null);
        }
        ~PHBallJointConeLimitIf() { if (_flag) { SprExport.Spr_delete_PHBallJointConeLimitIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointConeLimitIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointConeLimitIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointConeLimitIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointConeLimitIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetSwingRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_SetSwingRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetSwingRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_GetSwingRange((IntPtr)_this, (IntPtr)range);
        }
        public void SetSwingDirRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_SetSwingDirRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetSwingDirRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_GetSwingDirRange((IntPtr)_this, (IntPtr)range);
        }
        public void SetTwistRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_SetTwistRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetTwistRange(Vec2d range) {
            SprExport.Spr_PHBallJointConeLimitIf_GetTwistRange((IntPtr)_this, (IntPtr)range);
        }
    }
    public partial class PHBallJointSplineLimitIf : PHBallJointLimitIf {
        public PHBallJointSplineLimitIf() { _this = SprExport.Spr_new_PHBallJointSplineLimitIf(); _flag = true; }
        public PHBallJointSplineLimitIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointSplineLimitIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointSplineLimitIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointSplineLimitIf.GetIfInfoStatic()) ? new PHBallJointSplineLimitIf(target._this, target._flag) : null);
        }
        ~PHBallJointSplineLimitIf() { if (_flag) { SprExport.Spr_delete_PHBallJointSplineLimitIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointSplineLimitIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointSplineLimitIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointSplineLimitIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointSplineLimitIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void AddNode(PHSplineLimitNode node) {
            SprExport.Spr_PHBallJointSplineLimitIf_AddNode((IntPtr)_this, (IntPtr)node);
        }
        public void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax) {
            SprExport.Spr_PHBallJointSplineLimitIf_AddNode_1((IntPtr)_this, (double)S, (double)SD, (double)dS, (double)dSD, (double)tMin, (double)tMax);
        }
        public void AddNode(PHSplineLimitNode node, int pos) {
            SprExport.Spr_PHBallJointSplineLimitIf_AddNode_2((IntPtr)_this, (IntPtr)node, (int)pos);
        }
        public void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax, int pos) {
            SprExport.Spr_PHBallJointSplineLimitIf_AddNode_3((IntPtr)_this, (double)S, (double)SD, (double)dS, (double)dSD, (double)tMin, (double)tMax, (int)pos);
        }
        public void SetPoleTwistRange(Vec2d range) {
            SprExport.Spr_PHBallJointSplineLimitIf_SetPoleTwistRange((IntPtr)_this, (IntPtr)range);
        }
        public void GetPoleTwistRange(Vec2d range) {
            SprExport.Spr_PHBallJointSplineLimitIf_GetPoleTwistRange((IntPtr)_this, (IntPtr)range);
        }
    }
    public partial class PHBallJointIndependentLimitIf : PHBallJointLimitIf {
        public PHBallJointIndependentLimitIf() { _this = SprExport.Spr_new_PHBallJointIndependentLimitIf(); _flag = true; }
        public PHBallJointIndependentLimitIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointIndependentLimitIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointIndependentLimitIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointIndependentLimitIf.GetIfInfoStatic()) ? new PHBallJointIndependentLimitIf(target._this, target._flag) : null);
        }
        ~PHBallJointIndependentLimitIf() { if (_flag) { SprExport.Spr_delete_PHBallJointIndependentLimitIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointIndependentLimitIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointIndependentLimitIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointIndependentLimitIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointIndependentLimitIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public bool IsOnLimit() {
            char ret = SprExport.Spr_PHBallJointIndependentLimitIf_IsOnLimit((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public int AxesEnabled() {
            int result = (int)SprExport.Spr_PHBallJointIndependentLimitIf_AxesEnabled((IntPtr)_this);
            return result;
        }
        public void SetLimitRangeN(int n, Vec2d range) {
            SprExport.Spr_PHBallJointIndependentLimitIf_SetLimitRangeN((IntPtr)_this, (int)n, (IntPtr)range);
        }
        public void GetLimitRangeN(int n, Vec2d range) {
            SprExport.Spr_PHBallJointIndependentLimitIf_GetLimitRangeN((IntPtr)_this, (int)n, (IntPtr)range);
        }
    }
    public partial class PH1DJointMotorIf : SceneObjectIf {
        public PH1DJointMotorIf() { _this = SprExport.Spr_new_PH1DJointMotorIf(); _flag = true; }
        public PH1DJointMotorIf(IntPtr ptr) : base(ptr) { }
        public PH1DJointMotorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PH1DJointMotorIf(CsCastObject target) {
            return (target._info.Inherit(PH1DJointMotorIf.GetIfInfoStatic()) ? new PH1DJointMotorIf(target._this, target._flag) : null);
        }
        ~PH1DJointMotorIf() { if (_flag) { SprExport.Spr_delete_PH1DJointMotorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PH1DJointMotorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PH1DJointMotorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PH1DJointMotorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PH1DJointMotorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PH1DJointNonLinearMotorIf : PH1DJointMotorIf {
        public PH1DJointNonLinearMotorIf() { _this = SprExport.Spr_new_PH1DJointNonLinearMotorIf(); _flag = true; }
        public PH1DJointNonLinearMotorIf(IntPtr ptr) : base(ptr) { }
        public PH1DJointNonLinearMotorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PH1DJointNonLinearMotorIf(CsCastObject target) {
            return (target._info.Inherit(PH1DJointNonLinearMotorIf.GetIfInfoStatic()) ? new PH1DJointNonLinearMotorIf(target._this, target._flag) : null);
        }
        ~PH1DJointNonLinearMotorIf() { if (_flag) { SprExport.Spr_delete_PH1DJointNonLinearMotorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PH1DJointNonLinearMotorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PH1DJointNonLinearMotorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PH1DJointNonLinearMotorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PH1DJointNonLinearMotorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetSpring(PH1DJointNonLinearMotorDesc.FunctionMode m, CsObject param) {
            SprExport.Spr_PH1DJointNonLinearMotorIf_SetSpring((IntPtr)_this, (int)m, (IntPtr)param);
        }
        public void SetDamper(PH1DJointNonLinearMotorDesc.FunctionMode m, CsObject param) {
            SprExport.Spr_PH1DJointNonLinearMotorIf_SetDamper((IntPtr)_this, (int)m, (IntPtr)param);
        }
        public void SetSpringDamper(PH1DJointNonLinearMotorDesc.FunctionMode smode, PH1DJointNonLinearMotorDesc.FunctionMode dmode, CsObject sparam, CsObject dparam) {
            SprExport.Spr_PH1DJointNonLinearMotorIf_SetSpringDamper((IntPtr)_this, (int)smode, (int)dmode, (IntPtr)sparam, (IntPtr)dparam);
        }
    }
    public partial class PHHuman1DJointResistanceIf : PH1DJointNonLinearMotorIf {
        public PHHuman1DJointResistanceIf() { _this = SprExport.Spr_new_PHHuman1DJointResistanceIf(); _flag = true; }
        public PHHuman1DJointResistanceIf(IntPtr ptr) : base(ptr) { }
        public PHHuman1DJointResistanceIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHuman1DJointResistanceIf(CsCastObject target) {
            return (target._info.Inherit(PHHuman1DJointResistanceIf.GetIfInfoStatic()) ? new PHHuman1DJointResistanceIf(target._this, target._flag) : null);
        }
        ~PHHuman1DJointResistanceIf() { if (_flag) { SprExport.Spr_delete_PHHuman1DJointResistanceIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHuman1DJointResistanceIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHuman1DJointResistanceIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHuman1DJointResistanceIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHuman1DJointResistanceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public double GetCurrentResistance() {
            double result = (double)SprExport.Spr_PHHuman1DJointResistanceIf_GetCurrentResistance((IntPtr)_this);
            return result;
        }
    }
    public partial class PHBallJointMotorIf : SceneObjectIf {
        public PHBallJointMotorIf() { _this = SprExport.Spr_new_PHBallJointMotorIf(); _flag = true; }
        public PHBallJointMotorIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointMotorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointMotorIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointMotorIf.GetIfInfoStatic()) ? new PHBallJointMotorIf(target._this, target._flag) : null);
        }
        ~PHBallJointMotorIf() { if (_flag) { SprExport.Spr_delete_PHBallJointMotorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointMotorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointMotorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointMotorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointMotorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHBallJointNonLinearMotorIf : PHBallJointMotorIf {
        public PHBallJointNonLinearMotorIf() { _this = SprExport.Spr_new_PHBallJointNonLinearMotorIf(); _flag = true; }
        public PHBallJointNonLinearMotorIf(IntPtr ptr) : base(ptr) { }
        public PHBallJointNonLinearMotorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBallJointNonLinearMotorIf(CsCastObject target) {
            return (target._info.Inherit(PHBallJointNonLinearMotorIf.GetIfInfoStatic()) ? new PHBallJointNonLinearMotorIf(target._this, target._flag) : null);
        }
        ~PHBallJointNonLinearMotorIf() { if (_flag) { SprExport.Spr_delete_PHBallJointNonLinearMotorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBallJointNonLinearMotorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBallJointNonLinearMotorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBallJointNonLinearMotorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBallJointNonLinearMotorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetFuncFromDatabaseN(int n, int i, int j, CsObject sparam, CsObject dparam) {
            SprExport.Spr_PHBallJointNonLinearMotorIf_SetFuncFromDatabaseN((IntPtr)_this, (int)n, (int)i, (int)j, (IntPtr)sparam, (IntPtr)dparam);
        }
    }
    public partial class PHHumanBallJointResistanceIf : PHBallJointNonLinearMotorIf {
        public PHHumanBallJointResistanceIf() { _this = SprExport.Spr_new_PHHumanBallJointResistanceIf(); _flag = true; }
        public PHHumanBallJointResistanceIf(IntPtr ptr) : base(ptr) { }
        public PHHumanBallJointResistanceIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHHumanBallJointResistanceIf(CsCastObject target) {
            return (target._info.Inherit(PHHumanBallJointResistanceIf.GetIfInfoStatic()) ? new PHHumanBallJointResistanceIf(target._this, target._flag) : null);
        }
        ~PHHumanBallJointResistanceIf() { if (_flag) { SprExport.Spr_delete_PHHumanBallJointResistanceIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHHumanBallJointResistanceIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHHumanBallJointResistanceIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHHumanBallJointResistanceIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHHumanBallJointResistanceIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Vec3d GetCurrentResistance() {
            IntPtr ptr = SprExport.Spr_PHHumanBallJointResistanceIf_GetCurrentResistance((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
    }
    public partial class PHSpringMotorIf : SceneObjectIf {
        public PHSpringMotorIf() { _this = SprExport.Spr_new_PHSpringMotorIf(); _flag = true; }
        public PHSpringMotorIf(IntPtr ptr) : base(ptr) { }
        public PHSpringMotorIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSpringMotorIf(CsCastObject target) {
            return (target._info.Inherit(PHSpringMotorIf.GetIfInfoStatic()) ? new PHSpringMotorIf(target._this, target._flag) : null);
        }
        ~PHSpringMotorIf() { if (_flag) { SprExport.Spr_delete_PHSpringMotorIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSpringMotorIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSpringMotorIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSpringMotorIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSpringMotorIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
    }
    public partial class PHOpObjIf : SceneObjectIf {
        public PHOpObjIf() { _this = SprExport.Spr_new_PHOpObjIf(); _flag = true; }
        public PHOpObjIf(IntPtr ptr) : base(ptr) { }
        public PHOpObjIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpObjIf(CsCastObject target) {
            return (target._info.Inherit(PHOpObjIf.GetIfInfoStatic()) ? new PHOpObjIf(target._this, target._flag) : null);
        }
        ~PHOpObjIf() { if (_flag) { SprExport.Spr_delete_PHOpObjIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpObjIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public bool initialPHOpObj(Vec3f vts, int vtsNum, float pSize) {
            char ret = SprExport.Spr_PHOpObjIf_initialPHOpObj((IntPtr)_this, (IntPtr)vts, (int)vtsNum, (float)pSize);
            return (ret == 0) ? false : true;
        }
        public void SetGravity(bool bOn) {
            SprExport.Spr_PHOpObjIf_SetGravity((IntPtr)_this, (bool)bOn);
        }
        public void SimpleSimulationStep() {
            SprExport.Spr_PHOpObjIf_SimpleSimulationStep((IntPtr)_this);
        }
        public void AddVertextoLocalBuffer(Vec3f v) {
            SprExport.Spr_PHOpObjIf_AddVertextoLocalBuffer((IntPtr)_this, (IntPtr)v);
        }
        public bool InitialObjUsingLocalBuffer(float pSize) {
            char ret = SprExport.Spr_PHOpObjIf_InitialObjUsingLocalBuffer((IntPtr)_this, (float)pSize);
            return (ret == 0) ? false : true;
        }
        public void positionPredict() {
            SprExport.Spr_PHOpObjIf_positionPredict((IntPtr)_this);
        }
        public void groupStep() {
            SprExport.Spr_PHOpObjIf_groupStep((IntPtr)_this);
        }
        public void integrationStep() {
            SprExport.Spr_PHOpObjIf_integrationStep((IntPtr)_this);
        }
        public void ReducedPositionProject() {
            SprExport.Spr_PHOpObjIf_ReducedPositionProject((IntPtr)_this);
        }
        public void positionProject() {
            SprExport.Spr_PHOpObjIf_positionProject((IntPtr)_this);
        }
        public void SetDefaultLinkNum(int linkNum) {
            SprExport.Spr_PHOpObjIf_SetDefaultLinkNum((IntPtr)_this, (int)linkNum);
        }
        public void BuildBlendWeight() {
            SprExport.Spr_PHOpObjIf_BuildBlendWeight((IntPtr)_this);
        }
        public void buildGroupCenter() {
            SprExport.Spr_PHOpObjIf_buildGroupCenter((IntPtr)_this);
        }
        public int GetVertexNum() {
            int result = (int)SprExport.Spr_PHOpObjIf_GetVertexNum((IntPtr)_this);
            return result;
        }
        public Vec3f GetVertex(int vi) {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_GetVertex((IntPtr)_this, (int)vi);
            return new Vec3f(ptr, true);
        }
        public ObjectIf GetOpParticle(int pi) {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_GetOpParticle((IntPtr)_this, (int)pi);
            if (ptr == IntPtr.Zero) { return null; }
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == PHSolidPairIf.GetIfInfoStatic()) { return new PHSolidPairIf(ptr); }
            if (obj.GetIfInfo() == PHOpParticleIf.GetIfInfoStatic()) { return new PHOpParticleIf(ptr); }
            if (obj.GetIfInfo() == PHOpGroupIf.GetIfInfoStatic()) { return new PHOpGroupIf(ptr); }
            if (obj.GetIfInfo() == PHOpSpHashColliAgentIf.GetIfInfoStatic()) { return new PHOpSpHashColliAgentIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForLCPIf.GetIfInfoStatic()) { return new PHSolidPairForLCPIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForHapticIf.GetIfInfoStatic()) { return new PHSolidPairForHapticIf(ptr); }
            return obj;
        }
        public ObjectIf GetOpGroup(int gi) {
            IntPtr ptr = SprExport.Spr_PHOpObjIf_GetOpGroup((IntPtr)_this, (int)gi);
            if (ptr == IntPtr.Zero) { return null; }
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == PHSolidPairIf.GetIfInfoStatic()) { return new PHSolidPairIf(ptr); }
            if (obj.GetIfInfo() == PHOpParticleIf.GetIfInfoStatic()) { return new PHOpParticleIf(ptr); }
            if (obj.GetIfInfo() == PHOpGroupIf.GetIfInfoStatic()) { return new PHOpGroupIf(ptr); }
            if (obj.GetIfInfo() == PHOpSpHashColliAgentIf.GetIfInfoStatic()) { return new PHOpSpHashColliAgentIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForLCPIf.GetIfInfoStatic()) { return new PHSolidPairForLCPIf(ptr); }
            if (obj.GetIfInfo() == PHSolidPairForHapticIf.GetIfInfoStatic()) { return new PHSolidPairForHapticIf(ptr); }
            return obj;
        }
        public int GetobjVtoPmap(int vi) {
            int result = (int)SprExport.Spr_PHOpObjIf_GetobjVtoPmap((IntPtr)_this, (int)vi);
            return result;
        }
        public float GetVtxBlendWeight(int Vtxi, int Grpi) {
            float result = (float)SprExport.Spr_PHOpObjIf_GetVtxBlendWeight((IntPtr)_this, (int)Vtxi, (int)Grpi);
            return result;
        }
        public void SetVelocityDamping(float vd) {
            SprExport.Spr_PHOpObjIf_SetVelocityDamping((IntPtr)_this, (float)vd);
        }
        public float GetVelocityDamping() {
            float result = (float)SprExport.Spr_PHOpObjIf_GetVelocityDamping((IntPtr)_this);
            return result;
        }
        public float GetBoundLength() {
            float result = (float)SprExport.Spr_PHOpObjIf_GetBoundLength((IntPtr)_this);
            return result;
        }
        public void SetBound(float b) {
            SprExport.Spr_PHOpObjIf_SetBound((IntPtr)_this, (float)b);
        }
        public void SetTimeStep(float t) {
            SprExport.Spr_PHOpObjIf_SetTimeStep((IntPtr)_this, (float)t);
        }
        public float GetTimeStep() {
            float result = (float)SprExport.Spr_PHOpObjIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public float GetObjBeta() {
            float result = (float)SprExport.Spr_PHOpObjIf_GetObjBeta((IntPtr)_this);
            return result;
        }
        public void SetObjBeta(float beta) {
            SprExport.Spr_PHOpObjIf_SetObjBeta((IntPtr)_this, (float)beta);
        }
        public void SetObjAlpha(float alpha) {
            SprExport.Spr_PHOpObjIf_SetObjAlpha((IntPtr)_this, (float)alpha);
        }
        public float GetObjAlpha() {
            float result = (float)SprExport.Spr_PHOpObjIf_GetObjAlpha((IntPtr)_this);
            return result;
        }
        public bool GetObjDstConstraint() {
            char ret = SprExport.Spr_PHOpObjIf_GetObjDstConstraint((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetObjDstConstraint(bool d) {
            SprExport.Spr_PHOpObjIf_SetObjDstConstraint((IntPtr)_this, (bool)d);
        }
        public void SetObjItrTime(int itrT) {
            SprExport.Spr_PHOpObjIf_SetObjItrTime((IntPtr)_this, (int)itrT);
        }
        public int GetObjItrTime() {
            int result = (int)SprExport.Spr_PHOpObjIf_GetObjItrTime((IntPtr)_this);
            return result;
        }
        public void StoreOrigPose() {
            SprExport.Spr_PHOpObjIf_StoreOrigPose((IntPtr)_this);
        }
    }
    public partial class PHOpParticleIf : ObjectIf {
        public PHOpParticleIf() { _this = SprExport.Spr_new_PHOpParticleIf(); _flag = true; }
        public PHOpParticleIf(IntPtr ptr) : base(ptr) { }
        public PHOpParticleIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpParticleIf(CsCastObject target) {
            return (target._info.Inherit(PHOpParticleIf.GetIfInfoStatic()) ? new PHOpParticleIf(target._this, target._flag) : null);
        }
        ~PHOpParticleIf() { if (_flag) { SprExport.Spr_delete_PHOpParticleIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpParticleIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpParticleIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpParticleIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpParticleIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int GetOpPtclVtxId(int vi) {
            int result = (int)SprExport.Spr_PHOpParticleIf_GetOpPtclVtxId((IntPtr)_this, (int)vi);
            return result;
        }
        public int GetinGrpListNum() {
            int result = (int)SprExport.Spr_PHOpParticleIf_GetinGrpListNum((IntPtr)_this);
            return result;
        }
        public int GetinGrpList(int gi) {
            int result = (int)SprExport.Spr_PHOpParticleIf_GetinGrpList((IntPtr)_this, (int)gi);
            return result;
        }
        public float GetVtxDisWeight(int vi) {
            float result = (float)SprExport.Spr_PHOpParticleIf_GetVtxDisWeight((IntPtr)_this, (int)vi);
            return result;
        }
        public arraywrapper_PHOpParticleDesc GetParticleDesc() {
            IntPtr ptr = SprExport.Spr_PHOpParticleIf_GetParticleDesc((IntPtr)_this);
            return new arraywrapper_PHOpParticleDesc(ptr);
        }
    }
    public partial class PHOpGroupIf : ObjectIf {
        public PHOpGroupIf() { _this = SprExport.Spr_new_PHOpGroupIf(); _flag = true; }
        public PHOpGroupIf(IntPtr ptr) : base(ptr) { }
        public PHOpGroupIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpGroupIf(CsCastObject target) {
            return (target._info.Inherit(PHOpGroupIf.GetIfInfoStatic()) ? new PHOpGroupIf(target._this, target._flag) : null);
        }
        ~PHOpGroupIf() { if (_flag) { SprExport.Spr_delete_PHOpGroupIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpGroupIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpGroupIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpGroupIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpGroupIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int GetGrpInPtclNum() {
            int result = (int)SprExport.Spr_PHOpGroupIf_GetGrpInPtclNum((IntPtr)_this);
            return result;
        }
        public int GetGrpInPtcl(int gi) {
            int result = (int)SprExport.Spr_PHOpGroupIf_GetGrpInPtcl((IntPtr)_this, (int)gi);
            return result;
        }
        public arraywrapper_PHOpGroupDesc GetGroupDesc() {
            IntPtr ptr = SprExport.Spr_PHOpGroupIf_GetGroupDesc((IntPtr)_this);
            return new arraywrapper_PHOpGroupDesc(ptr);
        }
    }
    public partial class PHOpHapticControllerIf : SceneObjectIf {
        public PHOpHapticControllerIf() { _this = SprExport.Spr_new_PHOpHapticControllerIf(); _flag = true; }
        public PHOpHapticControllerIf(IntPtr ptr) : base(ptr) { }
        public PHOpHapticControllerIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpHapticControllerIf(CsCastObject target) {
            return (target._info.Inherit(PHOpHapticControllerIf.GetIfInfoStatic()) ? new PHOpHapticControllerIf(target._this, target._flag) : null);
        }
        ~PHOpHapticControllerIf() { if (_flag) { SprExport.Spr_delete_PHOpHapticControllerIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpHapticControllerIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetHCForceReady(bool flag) {
            SprExport.Spr_PHOpHapticControllerIf_SetHCForceReady((IntPtr)_this, (bool)flag);
        }
        public bool GetHCForceReady() {
            char ret = SprExport.Spr_PHOpHapticControllerIf_GetHCForceReady((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public bool CheckProxyState() {
            char ret = SprExport.Spr_PHOpHapticControllerIf_CheckProxyState((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void AddColliedPtcl(int pIndex, int objindex, Vec3f ctcPos) {
            SprExport.Spr_PHOpHapticControllerIf_AddColliedPtcl((IntPtr)_this, (int)pIndex, (int)objindex, (IntPtr)ctcPos);
        }
        public bool BeginLogForce() {
            char ret = SprExport.Spr_PHOpHapticControllerIf_BeginLogForce((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EndLogForce() {
            SprExport.Spr_PHOpHapticControllerIf_EndLogForce((IntPtr)_this);
        }
        public void setC_ObstacleRadius(float r) {
            SprExport.Spr_PHOpHapticControllerIf_setC_ObstacleRadius((IntPtr)_this, (float)r);
        }
        public Vec3f GetUserPos() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetUserPos((IntPtr)_this);
            return new Vec3f(ptr, true);
        }
        public Vec3f GetHCPosition() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetHCPosition((IntPtr)_this);
            return new Vec3f(ptr, true);
        }
        public void SetHCPosition(Vec3f pos) {
            SprExport.Spr_PHOpHapticControllerIf_SetHCPosition((IntPtr)_this, (IntPtr)pos);
        }
        public void SetHCPose(Posef pose) {
            SprExport.Spr_PHOpHapticControllerIf_SetHCPose((IntPtr)_this, (IntPtr)pose);
        }
        public Posef GetHCPose() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetHCPose((IntPtr)_this);
            return new Posef(ptr, true);
        }
        public float GetC_ObstacleRadius() {
            float result = (float)SprExport.Spr_PHOpHapticControllerIf_GetC_ObstacleRadius((IntPtr)_this);
            return result;
        }
        public Vec3f GetCurrentOutputForce() {
            IntPtr ptr = SprExport.Spr_PHOpHapticControllerIf_GetCurrentOutputForce((IntPtr)_this);
            return new Vec3f(ptr, true);
        }
    }
    public partial class PHOpHapticRendererIf : SceneObjectIf {
        public PHOpHapticRendererIf() { _this = SprExport.Spr_new_PHOpHapticRendererIf(); _flag = true; }
        public PHOpHapticRendererIf(IntPtr ptr) : base(ptr) { }
        public PHOpHapticRendererIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpHapticRendererIf(CsCastObject target) {
            return (target._info.Inherit(PHOpHapticRendererIf.GetIfInfoStatic()) ? new PHOpHapticRendererIf(target._this, target._flag) : null);
        }
        ~PHOpHapticRendererIf() { if (_flag) { SprExport.Spr_delete_PHOpHapticRendererIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpHapticRendererIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpHapticRendererIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpHapticRendererIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpHapticRendererIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetRigid(bool set) {
            SprExport.Spr_PHOpHapticRendererIf_SetRigid((IntPtr)_this, (bool)set);
        }
        public bool IsRigid() {
            char ret = SprExport.Spr_PHOpHapticRendererIf_IsRigid((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHOpAnimationIf : SceneObjectIf {
        public PHOpAnimationIf() { _this = SprExport.Spr_new_PHOpAnimationIf(); _flag = true; }
        public PHOpAnimationIf(IntPtr ptr) : base(ptr) { }
        public PHOpAnimationIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpAnimationIf(CsCastObject target) {
            return (target._info.Inherit(PHOpAnimationIf.GetIfInfoStatic()) ? new PHOpAnimationIf(target._this, target._flag) : null);
        }
        ~PHOpAnimationIf() { if (_flag) { SprExport.Spr_delete_PHOpAnimationIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpAnimationIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpAnimationIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpAnimationIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpAnimationIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void AddAnimationP(int objIndex, int pIndex, Vec3f start, Vec3f end, float totalTime) {
            SprExport.Spr_PHOpAnimationIf_AddAnimationP((IntPtr)_this, (int)objIndex, (int)pIndex, (IntPtr)start, (IntPtr)end, (float)totalTime);
        }
        public void AddAnimationP(int objIndex, int pIndex, Vec3f force, float totalTime) {
            SprExport.Spr_PHOpAnimationIf_AddAnimationP_1((IntPtr)_this, (int)objIndex, (int)pIndex, (IntPtr)force, (float)totalTime);
        }
        public void AnimationStep(CsObject opEngine) {
            SprExport.Spr_PHOpAnimationIf_AnimationStep((IntPtr)_this, (IntPtr)opEngine);
        }
        public void AnimationIntergration(CsObject opEngine) {
            SprExport.Spr_PHOpAnimationIf_AnimationIntergration((IntPtr)_this, (IntPtr)opEngine);
        }
    }
    public partial class PHOpSpHashColliAgentIf : ObjectIf {
        public PHOpSpHashColliAgentIf() { _this = SprExport.Spr_new_PHOpSpHashColliAgentIf(); _flag = true; }
        public PHOpSpHashColliAgentIf(IntPtr ptr) : base(ptr) { }
        public PHOpSpHashColliAgentIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHOpSpHashColliAgentIf(CsCastObject target) {
            return (target._info.Inherit(PHOpSpHashColliAgentIf.GetIfInfoStatic()) ? new PHOpSpHashColliAgentIf(target._this, target._flag) : null);
        }
        ~PHOpSpHashColliAgentIf() { if (_flag) { SprExport.Spr_delete_PHOpSpHashColliAgentIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHOpSpHashColliAgentIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHOpSpHashColliAgentIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHOpSpHashColliAgentIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHOpSpHashColliAgentIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetCollisionCstrStiffness(float alpha) {
            SprExport.Spr_PHOpSpHashColliAgentIf_SetCollisionCstrStiffness((IntPtr)_this, (float)alpha);
        }
        public float GetCollisionCstrStiffness() {
            float result = (float)SprExport.Spr_PHOpSpHashColliAgentIf_GetCollisionCstrStiffness((IntPtr)_this);
            return result;
        }
        public void EnableCollisionDetection(bool able) {
            SprExport.Spr_PHOpSpHashColliAgentIf_EnableCollisionDetection((IntPtr)_this, (bool)able);
        }
        public void EnableCollisionDetection() {
            SprExport.Spr_PHOpSpHashColliAgentIf_EnableCollisionDetection_1((IntPtr)_this);
        }
        public void Initial(float cellSize, CDBounds bounds) {
            SprExport.Spr_PHOpSpHashColliAgentIf_Initial((IntPtr)_this, (float)cellSize, (IntPtr)bounds);
        }
        public void OpCollisionProcedure(int myTimeStamp) {
            SprExport.Spr_PHOpSpHashColliAgentIf_OpCollisionProcedure((IntPtr)_this, (int)myTimeStamp);
        }
        public void OpCollisionProcedure() {
            SprExport.Spr_PHOpSpHashColliAgentIf_OpCollisionProcedure_1((IntPtr)_this);
        }
        public void OpDirCollision() {
            SprExport.Spr_PHOpSpHashColliAgentIf_OpDirCollision((IntPtr)_this);
        }
        public void AddContactPlane(Vec3f planeP, Vec3f planeN) {
            SprExport.Spr_PHOpSpHashColliAgentIf_AddContactPlane((IntPtr)_this, (IntPtr)planeP, (IntPtr)planeN);
        }
        public bool IsCollisionEnabled() {
            char ret = SprExport.Spr_PHOpSpHashColliAgentIf_IsCollisionEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetIsDirCollision(bool flag) {
            SprExport.Spr_PHOpSpHashColliAgentIf_SetIsDirCollision((IntPtr)_this, (bool)flag);
        }
        public bool GetIsDirCollision() {
            char ret = SprExport.Spr_PHOpSpHashColliAgentIf_GetIsDirCollision((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetDebugMode(bool flag) {
            SprExport.Spr_PHOpSpHashColliAgentIf_SetDebugMode((IntPtr)_this, (bool)flag);
        }
        public bool GetDebugMode() {
            char ret = SprExport.Spr_PHOpSpHashColliAgentIf_GetDebugMode((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
    }
    public partial class PHRayIf : SceneObjectIf {
        public PHRayIf() { _this = SprExport.Spr_new_PHRayIf(); _flag = true; }
        public PHRayIf(IntPtr ptr) : base(ptr) { }
        public PHRayIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHRayIf(CsCastObject target) {
            return (target._info.Inherit(PHRayIf.GetIfInfoStatic()) ? new PHRayIf(target._this, target._flag) : null);
        }
        ~PHRayIf() { if (_flag) { SprExport.Spr_delete_PHRayIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHRayIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHRayIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public Vec3d GetOrigin() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetOrigin((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetOrigin(Vec3d ori) {
            SprExport.Spr_PHRayIf_SetOrigin((IntPtr)_this, (IntPtr)ori);
        }
        public Vec3d GetDirection() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetDirection((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetDirection(Vec3d dir) {
            SprExport.Spr_PHRayIf_SetDirection((IntPtr)_this, (IntPtr)dir);
        }
        public void Apply() {
            SprExport.Spr_PHRayIf_Apply((IntPtr)_this);
        }
        public int NHits() {
            int result = (int)SprExport.Spr_PHRayIf_NHits((IntPtr)_this);
            return result;
        }
        public arraywrapper_PHRaycastHit GetHits() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetHits((IntPtr)_this);
            return new arraywrapper_PHRaycastHit(ptr);
        }
        public arraywrapper_PHRaycastHit GetNearest() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetNearest((IntPtr)_this);
            return new arraywrapper_PHRaycastHit(ptr);
        }
        public arraywrapper_PHRaycastHit GetDynamicalNearest() {
            IntPtr ptr = SprExport.Spr_PHRayIf_GetDynamicalNearest((IntPtr)_this);
            return new arraywrapper_PHRaycastHit(ptr);
        }
    }
    public partial class PHSceneIf : SceneIf {
        public PHSceneIf() { _this = SprExport.Spr_new_PHSceneIf(); _flag = true; }
        public PHSceneIf(IntPtr ptr) : base(ptr) { }
        public PHSceneIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSceneIf(CsCastObject target) {
            return (target._info.Inherit(PHSceneIf.GetIfInfoStatic()) ? new PHSceneIf(target._this, target._flag) : null);
        }
        ~PHSceneIf() { if (_flag) { SprExport.Spr_delete_PHSceneIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSceneIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHSdkIf GetSdk() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSdk((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSdkIf obj = new PHSdkIf(ptr);
            return obj;
        }
        public PHSolidIf CreateSolid(PHSolidDesc desc) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    stateForSwap = ObjectStatesIf.Create();
                    if (isStepping) {
                        Console.WriteLine("Create Solid In isStepping");
                        IntPtr get = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
                        if (get == IntPtr.Zero) {
                            Console.Write("Create Solid null");
                            return null;
                        }
                        PHSolidIf obj = new PHSolidIf(get); // _thsisÇ…GetÇì¸ÇÍÇÈ
                        obj.phSceneIf = this;
                        obj._thisArray[sceneForGet] = get;
                        Console.WriteLine(sceneForGet);
                        AddWaitUntilNextStepCallback(() => {
                            IntPtr step = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
                            IntPtr buffer = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[sceneForBuffer], (IntPtr)desc);
                            obj._thisArray[sceneForStep] = step;
                            obj._thisArray[sceneForBuffer] = buffer;
                        });
                        if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(get); }
                        Console.WriteLine("Create Solid In isStepping End");
                        return obj;
                    } else {
                        Console.WriteLine("Create Solid not In isStepping");
                        IntPtr ptr0 = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[0], (IntPtr)desc);
                        IntPtr ptr1 = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[1], (IntPtr)desc);
                        IntPtr ptr2 = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[2], (IntPtr)desc);
                        if (ptr0 == IntPtr.Zero || ptr1 == IntPtr.Zero || ptr2 == IntPtr.Zero) {
                            Console.Write("Create Solid null");
                            return null;
                        }
                        PHSolidIf obj = new PHSolidIf(ptr0); // _thsisÇ…GetÇì¸ÇÍÇÈ
                        obj.phSceneIf = this;
                        obj._thisArray[0] = ptr0;
                        obj._thisArray[1] = ptr1;
                        obj._thisArray[2] = ptr2;
                        if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr0); }
                        return obj;
                    }
                }
            } else {
                IntPtr ptr0 = SprExport.Spr_PHSceneIf_CreateSolid((IntPtr)_thisArray[0], (IntPtr)desc);
                if (ptr0 == IntPtr.Zero) {
                    Console.Write("Create Solid null not Thread");
                    return null;
                }
                PHSolidIf obj = new PHSolidIf(ptr0); // _thsisÇ…GetÇì¸ÇÍÇÈ
                obj.phSceneIf = this;
                obj._thisArray[0] = ptr0;
                if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr0); }
                return obj;
            }
        }
        public PHSolidIf CreateSolid() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateSolid_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public int NSolids() {
            int result = (int)SprExport.Spr_PHSceneIf_NSolids((IntPtr)_this);
            return result;
        }
        public PHSolidIf GetSolid(int idx) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSolid((IntPtr)_this, (int)idx);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public int GetSolidIndex(PHSolidIf s) {
            int result = (int)SprExport.Spr_PHSceneIf_GetSolidIndex((IntPtr)_this, (IntPtr)s);
            return result;
        }
        public PHSolidIf GetSolids() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSolids((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public int NSolidPairs() {
            int result = (int)SprExport.Spr_PHSceneIf_NSolidPairs((IntPtr)_this);
            return result;
        }
        public PHSolidPairForLCPIf GetSolidPair(int i, int j) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSolidPair((IntPtr)_this, (int)i, (int)j);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidPairForLCPIf obj = new PHSolidPairForLCPIf(ptr);
            return obj;
        }
        public PHSolidPairForLCPIf GetSolidPair(PHSolidIf lhs, PHSolidIf rhs, bool bSwap) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSolidPair_1((IntPtr)_this, (IntPtr)lhs, (IntPtr)rhs, (bool)bSwap);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidPairForLCPIf obj = new PHSolidPairForLCPIf(ptr);
            return obj;
        }
        public void SetContactMode(PHSolidIf lhs, PHSolidIf rhs, PHSceneDesc.ContactMode mode) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep], (int)mode);
                            SprExport.Spr_PHSceneIf_SetContactMode((IntPtr)_thisArray[sceneForBuffer], (IntPtr)lhs._thisArray[sceneForBuffer], (IntPtr)rhs._thisArray[sceneForBuffer], (int)mode);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode((IntPtr)_thisArray[sceneForGet], (IntPtr)lhs._thisArray[sceneForGet], (IntPtr)rhs._thisArray[sceneForGet], (int)mode);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode((IntPtr)_thisArray[i], (IntPtr)lhs._thisArray[i], (IntPtr)rhs._thisArray[i], (int)mode);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode((IntPtr)_thisArray[0], (IntPtr)lhs._thisArray[0], (IntPtr)rhs._thisArray[0], (int)mode);
            }
        }
        public void SetContactMode(PHSolidIf lhs, PHSolidIf rhs) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_1((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep]);
                            SprExport.Spr_PHSceneIf_SetContactMode_1((IntPtr)_thisArray[sceneForBuffer], (IntPtr)lhs._thisArray[sceneForBuffer], (IntPtr)rhs._thisArray[sceneForBuffer]);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_1((IntPtr)_thisArray[sceneForGet], (IntPtr)lhs._thisArray[sceneForGet], (IntPtr)rhs._thisArray[sceneForGet]);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_1((IntPtr)_thisArray[i], (IntPtr)lhs._thisArray[i], (IntPtr)rhs._thisArray[i]);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_1((IntPtr)_thisArray[0], (IntPtr)lhs._thisArray[0], (IntPtr)rhs._thisArray[0]);
            }
        }
        public void SetContactMode(PHSolidIf group, ulong length, PHSceneDesc.ContactMode mode) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_2((IntPtr)_thisArray[sceneForStep], (IntPtr)group._thisArray[sceneForStep], (ulong)length, (int)mode);
                            SprExport.Spr_PHSceneIf_SetContactMode_2((IntPtr)_thisArray[sceneForBuffer], (IntPtr)group._thisArray[sceneForBuffer], (ulong)length, (int)mode);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_2((IntPtr)_thisArray[sceneForGet], (IntPtr)group._thisArray[sceneForGet], (ulong)length, (int)mode);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_2((IntPtr)_thisArray[i], (IntPtr)group._thisArray[i], (ulong)length, (int)mode);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_2((IntPtr)_thisArray[0], (IntPtr)group._thisArray[0], (ulong)length, (int)mode);
            }
        }
        public void SetContactMode(PHSolidIf group, ulong length) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_3((IntPtr)_thisArray[sceneForStep], (IntPtr)group._thisArray[sceneForStep], (ulong)length);
                            SprExport.Spr_PHSceneIf_SetContactMode_3((IntPtr)_thisArray[sceneForBuffer], (IntPtr)group._thisArray[sceneForBuffer], (ulong)length);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_3((IntPtr)_thisArray[sceneForGet], (IntPtr)group._thisArray[sceneForGet], (ulong)length);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_3((IntPtr)_thisArray[i], (IntPtr)group._thisArray[i], (ulong)length);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_3((IntPtr)_thisArray[0], (IntPtr)group._thisArray[0], (ulong)length);
            }
        }
        public void SetContactMode(PHSolidIf solid, PHSceneDesc.ContactMode mode) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_4((IntPtr)_thisArray[sceneForStep], (IntPtr)solid._thisArray[sceneForStep], (int)mode);
                            SprExport.Spr_PHSceneIf_SetContactMode_4((IntPtr)_thisArray[sceneForBuffer], (IntPtr)solid._thisArray[sceneForBuffer], (int)mode);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_4((IntPtr)_thisArray[sceneForGet], (IntPtr)solid._thisArray[sceneForGet], (int)mode);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_4((IntPtr)_thisArray[i], (IntPtr)solid._thisArray[i], (int)mode);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_4((IntPtr)_thisArray[0], (IntPtr)solid._thisArray[0], (int)mode);
            }
        }
        public void SetContactMode(PHSolidIf solid) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_5((IntPtr)_thisArray[sceneForStep], (IntPtr)solid._thisArray[sceneForStep]);
                            SprExport.Spr_PHSceneIf_SetContactMode_5((IntPtr)_thisArray[sceneForBuffer], (IntPtr)solid._thisArray[sceneForBuffer]);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_5((IntPtr)_thisArray[sceneForGet], (IntPtr)solid._thisArray[sceneForGet]);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_5((IntPtr)_thisArray[i], (IntPtr)solid._thisArray[i]);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_5((IntPtr)_thisArray[0], (IntPtr)solid._thisArray[0]);
            }
        }
        public void SetContactMode(PHSceneDesc.ContactMode mode) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_6((IntPtr)_thisArray[sceneForStep], (int)mode);
                            SprExport.Spr_PHSceneIf_SetContactMode_6((IntPtr)_thisArray[sceneForBuffer], (int)mode);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_6((IntPtr)_thisArray[sceneForGet], (int)mode);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_6((IntPtr)_thisArray[i], (int)mode);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_6((IntPtr)_thisArray[0], (int)mode);
            }
        }
        public void SetContactMode() {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetContactMode_7((IntPtr)_thisArray[sceneForStep]);
                            SprExport.Spr_PHSceneIf_SetContactMode_7((IntPtr)_thisArray[sceneForBuffer]);
                        });
                        SprExport.Spr_PHSceneIf_SetContactMode_7((IntPtr)_thisArray[sceneForGet]);
                    } else {
                        for (int i = 0; i < _thisNumber; i++) {
                            SprExport.Spr_PHSceneIf_SetContactMode_7((IntPtr)_thisArray[i]);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetContactMode_7((IntPtr)_thisArray[0]);
            }
        }
        public PHJointIf CreateJoint(PHSolidIf lhs, PHSolidIf rhs, IfInfo ii, PHJointDesc desc) {
            if (multiThreadMode) {
                PHJointIf obj = new PHJointIf(IntPtr.Zero);
                lock (phSceneForGetSetLock) {
                    stateForSwap = ObjectStatesIf.Create();
                    if (isStepping) {
                        Console.WriteLine("Create Joint In isStepping");
                        IntPtr get = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForGet], (IntPtr)lhs._thisArray[sceneForGet], (IntPtr)rhs._thisArray[sceneForGet], (IntPtr)ii, (IntPtr)desc);
                        if (get == IntPtr.Zero) {
                            Console.Write("Create Joint null");
                            return null;
                        }
                        obj._thisArray[sceneForGet] = get;
                        Console.WriteLine(sceneForGet);
                        obj._this = obj._thisArray[sceneForGet];
                    } else {
                        Console.WriteLine("Create Joint not In isStepping");
                        IntPtr ptr0 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[0], (IntPtr)lhs._thisArray[0], (IntPtr)rhs._thisArray[0], (IntPtr)ii, (IntPtr)desc);
                        IntPtr ptr1 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[1], (IntPtr)lhs._thisArray[1], (IntPtr)rhs._thisArray[1], (IntPtr)ii, (IntPtr)desc);
                        IntPtr ptr2 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[2], (IntPtr)lhs._thisArray[2], (IntPtr)rhs._thisArray[2], (IntPtr)ii, (IntPtr)desc);
                        if (ptr0 == IntPtr.Zero || ptr1 == IntPtr.Zero || ptr2 == IntPtr.Zero) {
                            Console.Write("Create Joint null");
                            return null;
                        }
                        obj._thisArray[0] = ptr0;
                        obj._thisArray[1] = ptr1;
                        obj._thisArray[2] = ptr2;
                        obj._this = obj._thisArray[0];
                    }

                    if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                        var newJoint = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        newJoint.phSceneIf = this;
                        if (isStepping) {
                            AddWaitUntilNextStepCallback(() => {
                                IntPtr step = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep], (IntPtr)ii, (IntPtr)desc);
                                IntPtr buffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForBuffer], (IntPtr)lhs._thisArray[sceneForBuffer], (IntPtr)rhs._thisArray[sceneForBuffer], (IntPtr)ii, (IntPtr)desc);
                                newJoint._thisArray[sceneForStep] = step;
                                newJoint._thisArray[sceneForBuffer] = buffer;
                            });
                        }
                        return newJoint;
                    }
                    if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                        var newJoint = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        newJoint.phSceneIf = this;
                        if (isStepping) {
                            AddWaitUntilNextStepCallback(() => {
                                IntPtr step = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep], (IntPtr)ii, (IntPtr)desc);
                                IntPtr buffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForBuffer], (IntPtr)lhs._thisArray[sceneForBuffer], (IntPtr)rhs._thisArray[sceneForBuffer], (IntPtr)ii, (IntPtr)desc);
                                newJoint._thisArray[sceneForStep] = step;
                                newJoint._thisArray[sceneForBuffer] = buffer;
                            });
                        }
                        return newJoint;
                    }
                    if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) { return new PHMateIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) { return new PHHingeJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) { return new PHSliderJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) { return new PHPathJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) { return new PHGenericJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) { return new PHPointToPointMateIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) { return new PHPointToLineMateIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) { return new PHPointToPlaneMateIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) { return new PHLineToLineMateIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) { return new PHPlaneToPlaneMateIf(IntPtr.Zero); }
                    return obj;
                }
            } else { // ÉRÉìÉXÉgÉâÉNÉ^ÇÃé¿ëïÇ…ÇÊÇ¡ÇƒèCê≥Ç™ïKóv
                IntPtr ptr = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[0], (IntPtr)lhs._thisArray[0], (IntPtr)rhs._thisArray[0], (IntPtr)ii, (IntPtr)desc);
                if (ptr == IntPtr.Zero) { return null; }
                PHJointIf obj = new PHJointIf(ptr);
                if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(ptr); }
                if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) { return new PHBallJointIf(ptr); }
                if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(ptr); }
                if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) { return new PHSpringIf(ptr); }
                if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) { return new PHMateIf(ptr); }
                if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) { return new PHHingeJointIf(ptr); }
                if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) { return new PHSliderJointIf(ptr); }
                if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) { return new PHPathJointIf(ptr); }
                if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) { return new PHGenericJointIf(ptr); }
                if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) { return new PHPointToPointMateIf(ptr); }
                if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) { return new PHPointToLineMateIf(ptr); }
                if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) { return new PHPointToPlaneMateIf(ptr); }
                if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) { return new PHLineToLineMateIf(ptr); }
                if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) { return new PHPlaneToPlaneMateIf(ptr); }
                return obj;
            }
        }
        public int NJoints() {
            int result = (int)SprExport.Spr_PHSceneIf_NJoints((IntPtr)_this);
            return result;
        }
        public PHJointIf GetJoint(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetJoint((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHJointIf obj = new PHJointIf(ptr);
            if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) { return new PHBallJointIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(ptr); }
            if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) { return new PHSpringIf(ptr); }
            if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) { return new PHMateIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) { return new PHHingeJointIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) { return new PHSliderJointIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) { return new PHPathJointIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) { return new PHGenericJointIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) { return new PHPointToPointMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) { return new PHPointToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) { return new PHPointToPlaneMateIf(ptr); }
            if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) { return new PHLineToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) { return new PHPlaneToPlaneMateIf(ptr); }
            return obj;
        }
        public int NContacts() {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    isFixedUpdating = true;
                    int result = SprExport.Spr_PHSceneIf_NContacts(
                        (IntPtr)_thisArray[sceneForGet]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    return result;
                }
            } else {
                int result = SprExport.Spr_PHSceneIf_NContacts(
                    (IntPtr)_thisArray[0]); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                return result;
            }
        }
        public PHContactPointIf GetContact(int i) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    isFixedUpdating = true;
                    IntPtr ptr = SprExport.Spr_PHSceneIf_GetContact(
                        (IntPtr)_thisArray[sceneForGet], (int)i); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                    if (ptr == IntPtr.Zero) { return null; }
                    PHContactPointIf obj = new PHContactPointIf(ptr);
                    obj._thisArray[sceneForGet] = ptr;
                    return obj;
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHSceneIf_GetContact(
                    (IntPtr)_thisArray[0], (int)i); // Ç±Ç±Ç≈éÊìæÇ≥ÇÍÇÈPosedÇÕï°êª
                if (ptr == IntPtr.Zero) { return null; }
                PHContactPointIf obj = new PHContactPointIf(ptr);
                return obj;
            }
        }
        public int NFemMeshes() {
            int result = (int)SprExport.Spr_PHSceneIf_NFemMeshes((IntPtr)_this);
            return result;
        }
        public PHFemMeshIf GetFemMesh(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetFemMesh((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemMeshIf obj = new PHFemMeshIf(ptr);
            if (obj.GetIfInfo() == PHFemMeshThermoIf.GetIfInfoStatic()) { return new PHFemMeshThermoIf(ptr); }
            return obj;
        }
        public int NFemMeshesNew() {
            int result = (int)SprExport.Spr_PHSceneIf_NFemMeshesNew((IntPtr)_this);
            return result;
        }
        public PHFemMeshNewIf GetFemMeshNew(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetFemMeshNew((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemMeshNewIf obj = new PHFemMeshNewIf(ptr);
            return obj;
        }
        public PHRootNodeIf CreateRootNode(PHSolidIf root, PHRootNodeDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateRootNode((IntPtr)_this, (IntPtr)root, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHRootNodeIf obj = new PHRootNodeIf(ptr);
            return obj;
        }
        public PHRootNodeIf CreateRootNode(PHSolidIf root) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateRootNode_1((IntPtr)_this, (IntPtr)root);
            if (ptr == IntPtr.Zero) { return null; }
            PHRootNodeIf obj = new PHRootNodeIf(ptr);
            return obj;
        }
        public int NRootNodes() {
            int result = (int)SprExport.Spr_PHSceneIf_NRootNodes((IntPtr)_this);
            return result;
        }
        public PHRootNodeIf GetRootNode(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetRootNode((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHRootNodeIf obj = new PHRootNodeIf(ptr);
            return obj;
        }
        public PHTreeNodeIf CreateTreeNode(PHTreeNodeIf parent, PHSolidIf child, PHTreeNodeDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateTreeNode((IntPtr)_this, (IntPtr)parent, (IntPtr)child, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHTreeNodeIf obj = new PHTreeNodeIf(ptr);
            if (obj.GetIfInfo() == PHRootNodeIf.GetIfInfoStatic()) { return new PHRootNodeIf(ptr); }
            if (obj.GetIfInfo() == PHTreeNode1DIf.GetIfInfoStatic()) { return new PHTreeNode1DIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointNodeIf.GetIfInfoStatic()) { return new PHBallJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointNodeIf.GetIfInfoStatic()) { return new PHFixJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointNodeIf.GetIfInfoStatic()) { return new PHHingeJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointNodeIf.GetIfInfoStatic()) { return new PHSliderJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointNodeIf.GetIfInfoStatic()) { return new PHPathJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointNodeIf.GetIfInfoStatic()) { return new PHGenericJointNodeIf(ptr); }
            return obj;
        }
        public PHTreeNodeIf CreateTreeNode(PHTreeNodeIf parent, PHSolidIf child) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateTreeNode_1((IntPtr)_this, (IntPtr)parent, (IntPtr)child);
            if (ptr == IntPtr.Zero) { return null; }
            PHTreeNodeIf obj = new PHTreeNodeIf(ptr);
            if (obj.GetIfInfo() == PHRootNodeIf.GetIfInfoStatic()) { return new PHRootNodeIf(ptr); }
            if (obj.GetIfInfo() == PHTreeNode1DIf.GetIfInfoStatic()) { return new PHTreeNode1DIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointNodeIf.GetIfInfoStatic()) { return new PHBallJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointNodeIf.GetIfInfoStatic()) { return new PHFixJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointNodeIf.GetIfInfoStatic()) { return new PHHingeJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointNodeIf.GetIfInfoStatic()) { return new PHSliderJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointNodeIf.GetIfInfoStatic()) { return new PHPathJointNodeIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointNodeIf.GetIfInfoStatic()) { return new PHGenericJointNodeIf(ptr); }
            return obj;
        }
        public PHRootNodeIf CreateTreeNodes(PHSolidIf solid) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateTreeNodes((IntPtr)_this, (IntPtr)solid);
            if (ptr == IntPtr.Zero) { return null; }
            PHRootNodeIf obj = new PHRootNodeIf(ptr);
            return obj;
        }
        public PHGearIf CreateGear(PH1DJointIf lhs, PH1DJointIf rhs, PHGearDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateGear((IntPtr)_this, (IntPtr)lhs, (IntPtr)rhs, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHGearIf obj = new PHGearIf(ptr);
            return obj;
        }
        public PHGearIf CreateGear(PH1DJointIf lhs, PH1DJointIf rhs) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateGear_1((IntPtr)_this, (IntPtr)lhs, (IntPtr)rhs);
            if (ptr == IntPtr.Zero) { return null; }
            PHGearIf obj = new PHGearIf(ptr);
            return obj;
        }
        public int NGears() {
            int result = (int)SprExport.Spr_PHSceneIf_NGears((IntPtr)_this);
            return result;
        }
        public PHGearIf GetGear(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetGear((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHGearIf obj = new PHGearIf(ptr);
            return obj;
        }
        public PHPathIf CreatePath(PHPathDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreatePath((IntPtr)_this, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHPathIf obj = new PHPathIf(ptr);
            return obj;
        }
        public PHPathIf CreatePath() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreatePath_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHPathIf obj = new PHPathIf(ptr);
            return obj;
        }
        public int NPaths() {
            int result = (int)SprExport.Spr_PHSceneIf_NPaths((IntPtr)_this);
            return result;
        }
        public PHPathIf GetPath(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetPath((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHPathIf obj = new PHPathIf(ptr);
            return obj;
        }
        public PHRayIf CreateRay(PHRayDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateRay((IntPtr)_this, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHRayIf obj = new PHRayIf(ptr);
            return obj;
        }
        public PHRayIf CreateRay() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateRay_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHRayIf obj = new PHRayIf(ptr);
            return obj;
        }
        public int NRays() {
            int result = (int)SprExport.Spr_PHSceneIf_NRays((IntPtr)_this);
            return result;
        }
        public PHRayIf GetRay(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetRay((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHRayIf obj = new PHRayIf(ptr);
            return obj;
        }
        public bool SetPosesOfJointedSolids(PHSolidIf base_) {
            char ret = SprExport.Spr_PHSceneIf_SetPosesOfJointedSolids((IntPtr)_this, (IntPtr)base_);
            return (ret == 0) ? false : true;
        }
        public PHIKActuatorIf CreateIKActuator(IfInfo ii, PHIKActuatorDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateIKActuator((IntPtr)_this, (IntPtr)ii, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public PHIKActuatorIf CreateIKActuator(IfInfo ii) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateIKActuator_1((IntPtr)_this, (IntPtr)ii);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public int NIKActuators() {
            int result = (int)SprExport.Spr_PHSceneIf_NIKActuators((IntPtr)_this);
            return result;
        }
        public PHIKActuatorIf GetIKActuator(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetIKActuator((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKActuatorIf obj = new PHIKActuatorIf(ptr);
            if (obj.GetIfInfo() == PHIKBallActuatorIf.GetIfInfoStatic()) { return new PHIKBallActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKHingeActuatorIf.GetIfInfoStatic()) { return new PHIKHingeActuatorIf(ptr); }
            if (obj.GetIfInfo() == PHIKSpringActuatorIf.GetIfInfoStatic()) { return new PHIKSpringActuatorIf(ptr); }
            return obj;
        }
        public PHIKEndEffectorIf CreateIKEndEffector(PHIKEndEffectorDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateIKEndEffector((IntPtr)_this, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKEndEffectorIf obj = new PHIKEndEffectorIf(ptr);
            return obj;
        }
        public PHIKEndEffectorIf CreateIKEndEffector() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateIKEndEffector_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKEndEffectorIf obj = new PHIKEndEffectorIf(ptr);
            return obj;
        }
        public int NIKEndEffectors() {
            int result = (int)SprExport.Spr_PHSceneIf_NIKEndEffectors((IntPtr)_this);
            return result;
        }
        public PHIKEndEffectorIf GetIKEndEffector(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetIKEndEffector((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKEndEffectorIf obj = new PHIKEndEffectorIf(ptr);
            return obj;
        }
        public int NSkeletons() {
            int result = (int)SprExport.Spr_PHSceneIf_NSkeletons((IntPtr)_this);
            return result;
        }
        public PHSkeletonIf GetSkeleton(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetSkeleton((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHSkeletonIf obj = new PHSkeletonIf(ptr);
            return obj;
        }
        public PHSkeletonIf CreateSkeleton(PHSkeletonDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateSkeleton((IntPtr)_this, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHSkeletonIf obj = new PHSkeletonIf(ptr);
            return obj;
        }
        public PHSkeletonIf CreateSkeleton() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateSkeleton_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSkeletonIf obj = new PHSkeletonIf(ptr);
            return obj;
        }
        public double GetTimeStep() {
            double result = (double)SprExport.Spr_PHSceneIf_GetTimeStep((IntPtr)_this);
            return result;
        }
        public double GetTimeStepInv() {
            double result = (double)SprExport.Spr_PHSceneIf_GetTimeStepInv((IntPtr)_this);
            return result;
        }
        public void SetTimeStep(double dt) {
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr)_thisArray[sceneForStep], (double)dt);
                            SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr)_thisArray[sceneForBuffer], (double)dt);
                        });
                        SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr)_thisArray[sceneForGet], (double)dt);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr)_this, (double)dt);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr)_thisArray[0], (double)dt);
            }
            //SprExport.Spr_PHSceneIf_SetTimeStep((IntPtr) _this, (double) dt);
        }
        public double GetHapticTimeStep() {
            double result = (double)SprExport.Spr_PHSceneIf_GetHapticTimeStep((IntPtr)_this);
            return result;
        }
        public void SetHapticTimeStep(double dt) {
            SprExport.Spr_PHSceneIf_SetHapticTimeStep((IntPtr)_this, (double)dt);
        }
        public uint GetCount() {
            uint result = (uint)SprExport.Spr_PHSceneIf_GetCount((IntPtr)_this);
            return result;
        }
        public void SetCount(uint count) {
            SprExport.Spr_PHSceneIf_SetCount((IntPtr)_this, (uint)count);
        }
        public void SetGravity(Vec3d accel) {
            SprExport.Spr_PHSceneIf_SetGravity((IntPtr)_this, (IntPtr)accel);
        }
        public Vec3d GetGravity() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetGravity((IntPtr)_this);
            return new Vec3d(ptr, true);
        }
        public void SetAirResistanceRateForVelocity(double rate) {
            SprExport.Spr_PHSceneIf_SetAirResistanceRateForVelocity((IntPtr)_this, (double)rate);
        }
        public void SetAirResistanceRateForAngularVelocity(double rate) {
            SprExport.Spr_PHSceneIf_SetAirResistanceRateForAngularVelocity((IntPtr)_this, (double)rate);
        }
        public double GetAirResistanceRateForVelocity() {
            double result = (double)SprExport.Spr_PHSceneIf_GetAirResistanceRateForVelocity((IntPtr)_this);
            return result;
        }
        public double GetAirResistanceRateForAngularVelocity() {
            double result = (double)SprExport.Spr_PHSceneIf_GetAirResistanceRateForAngularVelocity((IntPtr)_this);
            return result;
        }
        public void SetContactTolerance(double tol) {
            SprExport.Spr_PHSceneIf_SetContactTolerance((IntPtr)_this, (double)tol);
        }
        public double GetContactTolerance() {
            double result = (double)SprExport.Spr_PHSceneIf_GetContactTolerance((IntPtr)_this);
            return result;
        }
        public void SetImpactThreshold(double vth) {
            SprExport.Spr_PHSceneIf_SetImpactThreshold((IntPtr)_this, (double)vth);
        }
        public double GetImpactThreshold() {
            double result = (double)SprExport.Spr_PHSceneIf_GetImpactThreshold((IntPtr)_this);
            return result;
        }
        public void SetFrictionThreshold(double vth) {
            SprExport.Spr_PHSceneIf_SetFrictionThreshold((IntPtr)_this, (double)vth);
        }
        public double GetFrictionThreshold() {
            double result = (double)SprExport.Spr_PHSceneIf_GetFrictionThreshold((IntPtr)_this);
            return result;
        }
        public void SetMaxVelocity(double vmax) {
            SprExport.Spr_PHSceneIf_SetMaxVelocity((IntPtr)_this, (double)vmax);
        }
        public double GetMaxVelocity() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxVelocity((IntPtr)_this);
            return result;
        }
        public void SetMaxAngularVelocity(double wmax) {
            SprExport.Spr_PHSceneIf_SetMaxAngularVelocity((IntPtr)_this, (double)wmax);
        }
        public double GetMaxAngularVelocity() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxAngularVelocity((IntPtr)_this);
            return result;
        }
        public void SetMaxForce(double fmax) {
            SprExport.Spr_PHSceneIf_SetMaxForce((IntPtr)_this, (double)fmax);
        }
        public double GetMaxForce() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxForce((IntPtr)_this);
            return result;
        }
        public void SetMaxMoment(double tmax) {
            SprExport.Spr_PHSceneIf_SetMaxMoment((IntPtr)_this, (double)tmax);
        }
        public double GetMaxMoment() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxMoment((IntPtr)_this);
            return result;
        }
        public void SetMaxDeltaPosition(double dpmax) {
            SprExport.Spr_PHSceneIf_SetMaxDeltaPosition((IntPtr)_this, (double)dpmax);
        }
        public double GetMaxDeltaPosition() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxDeltaPosition((IntPtr)_this);
            return result;
        }
        public void SetMaxDeltaOrientation(double dqmax) {
            SprExport.Spr_PHSceneIf_SetMaxDeltaOrientation((IntPtr)_this, (double)dqmax);
        }
        public double GetMaxDeltaOrientation() {
            double result = (double)SprExport.Spr_PHSceneIf_GetMaxDeltaOrientation((IntPtr)_this);
            return result;
        }
        public int GetLCPSolver() {
            int result = (int)SprExport.Spr_PHSceneIf_GetLCPSolver((IntPtr)_this);
            return result;
        }
        public void SetLCPSolver(int method) {
            SprExport.Spr_PHSceneIf_SetLCPSolver((IntPtr)_this, (int)method);
        }
        public int GetNumIteration() {
            int result = (int)SprExport.Spr_PHSceneIf_GetNumIteration((IntPtr)_this);
            return result;
        }
        public void SetNumIteration(int n) {
            SprExport.Spr_PHSceneIf_SetNumIteration((IntPtr)_this, (int)n);
        }
        public void SetStateMode(bool bConstraints) {
            SprExport.Spr_PHSceneIf_SetStateMode((IntPtr)_this, (bool)bConstraints);
        }
        public void EnableContactDetection(bool enable) {
            SprExport.Spr_PHSceneIf_EnableContactDetection((IntPtr)_this, (bool)enable);
        }
        public bool IsContactDetectionEnabled() {
            char ret = SprExport.Spr_PHSceneIf_IsContactDetectionEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void EnableCCD(bool enable) {
            SprExport.Spr_PHSceneIf_EnableCCD((IntPtr)_this, (bool)enable);
        }
        public bool IsCCDEnabled() {
            char ret = SprExport.Spr_PHSceneIf_IsCCDEnabled((IntPtr)_this);
            return (ret == 0) ? false : true;
        }
        public void SetBroadPhaseMode(int mode) {
            SprExport.Spr_PHSceneIf_SetBroadPhaseMode((IntPtr)_this, (int)mode);
        }
        public int GetBroadPhaseMode() {
            int result = (int)SprExport.Spr_PHSceneIf_GetBroadPhaseMode((IntPtr)_this);
            return result;
        }
        public void SetContactDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz) {
            SprExport.Spr_PHSceneIf_SetContactDetectionRange((IntPtr)_this, (IntPtr)center, (IntPtr)extent, (int)nx, (int)ny, (int)nz);
        }
        public void SetMaterialBlending(int mode) {
            SprExport.Spr_PHSceneIf_SetMaterialBlending((IntPtr)_this, (int)mode);
        }
        public void ClearForce() {
            SprExport.Spr_PHSceneIf_ClearForce((IntPtr)_this);
        }
        public void GenerateForce() {
            SprExport.Spr_PHSceneIf_GenerateForce((IntPtr)_this);
        }
        public void Integrate() {
            SprExport.Spr_PHSceneIf_Integrate((IntPtr)_this);
        }
        public void IntegratePart1() {
            SprExport.Spr_PHSceneIf_IntegratePart1((IntPtr)_this);
        }
        public void IntegratePart2() {
            SprExport.Spr_PHSceneIf_IntegratePart2((IntPtr)_this);
        }
        public void Clear() {
            SprExport.Spr_PHSceneIf_Clear((IntPtr)_this);
        }
        public int NEngines() {
            int result = (int)SprExport.Spr_PHSceneIf_NEngines((IntPtr)_this);
            return result;
        }
        public PHEngineIf GetEngine(int i) {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetEngine((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHEngineIf obj = new PHEngineIf(ptr);
            if (obj.GetIfInfo() == PHConstraintEngineIf.GetIfInfoStatic()) { return new PHConstraintEngineIf(ptr); }
            if (obj.GetIfInfo() == PHGravityEngineIf.GetIfInfoStatic()) { return new PHGravityEngineIf(ptr); }
            if (obj.GetIfInfo() == PHPenaltyEngineIf.GetIfInfoStatic()) { return new PHPenaltyEngineIf(ptr); }
            if (obj.GetIfInfo() == PHIKEngineIf.GetIfInfoStatic()) { return new PHIKEngineIf(ptr); }
            if (obj.GetIfInfo() == PHFemEngineIf.GetIfInfoStatic()) { return new PHFemEngineIf(ptr); }
            if (obj.GetIfInfo() == PHOpEngineIf.GetIfInfoStatic()) { return new PHOpEngineIf(ptr); }
            if (obj.GetIfInfo() == PHHapticEngineIf.GetIfInfoStatic()) { return new PHHapticEngineIf(ptr); }
            return obj;
        }
        public PHConstraintEngineIf GetConstraintEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetConstraintEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHConstraintEngineIf obj = new PHConstraintEngineIf(ptr);
            return obj;
        }
        public PHGravityEngineIf GetGravityEngine() {
            if (multiThreadMode) {
                IntPtr ptr = SprExport.Spr_PHSceneIf_GetGravityEngine((IntPtr)_thisArray[0]);
                IntPtr ptr1 = SprExport.Spr_PHSceneIf_GetGravityEngine((IntPtr)_thisArray[1]);
                IntPtr ptr2 = SprExport.Spr_PHSceneIf_GetGravityEngine((IntPtr)_thisArray[2]);
                if (ptr == IntPtr.Zero) { return null; }
                PHGravityEngineIf obj = new PHGravityEngineIf(ptr);
                obj._thisArray[0] = ptr;
                obj._thisArray[1] = ptr1;
                obj._thisArray[2] = ptr2;
                return obj;
            } else {
                IntPtr ptr = SprExport.Spr_PHSceneIf_GetGravityEngine((IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHGravityEngineIf obj = new PHGravityEngineIf(ptr);
                obj._thisArray[0] = ptr;
                return obj;
            }
        }
        public PHPenaltyEngineIf GetPenaltyEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetPenaltyEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHPenaltyEngineIf obj = new PHPenaltyEngineIf(ptr);
            return obj;
        }
        public PHIKEngineIf GetIKEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetIKEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHIKEngineIf obj = new PHIKEngineIf(ptr);
            return obj;
        }
        public PHFemEngineIf GetFemEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetFemEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHFemEngineIf obj = new PHFemEngineIf(ptr);
            return obj;
        }
        public PHHapticEngineIf GetHapticEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetHapticEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHHapticEngineIf obj = new PHHapticEngineIf(ptr);
            return obj;
        }
        public PHOpEngineIf GetOpEngine() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetOpEngine((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHOpEngineIf obj = new PHOpEngineIf(ptr);
            return obj;
        }
        public PHOpSpHashColliAgentIf GetOpColliAgent() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_GetOpColliAgent((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHOpSpHashColliAgentIf obj = new PHOpSpHashColliAgentIf(ptr);
            return obj;
        }
        public PHHapticPointerIf CreateHapticPointer() {
            IntPtr ptr = SprExport.Spr_PHSceneIf_CreateHapticPointer((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHHapticPointerIf obj = new PHHapticPointerIf(ptr);
            return obj;
        }
        public void StepHapticLoop() {
            SprExport.Spr_PHSceneIf_StepHapticLoop((IntPtr)_this);
        }
        public void StepHapticSync() {
            SprExport.Spr_PHSceneIf_StepHapticSync((IntPtr)_this);
        }
    }
    public partial class PHSdkIf : SdkIf {
        public PHSdkIf() { _this = SprExport.Spr_new_PHSdkIf(); _flag = true; }
        public PHSdkIf(IntPtr ptr) : base(ptr) { }
        public PHSdkIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSdkIf(CsCastObject target) {
            return (target._info.Inherit(PHSdkIf.GetIfInfoStatic()) ? new PHSdkIf(target._this, target._flag) : null);
        }
        ~PHSdkIf() { if (_flag) { SprExport.Spr_delete_PHSdkIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSdkIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public PHSceneIf CreateScene(PHSceneDesc desc, bool multiThreadMode = false) {
            if (multiThreadMode) {
                IntPtr ptr1 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_this, (IntPtr)desc);
                IntPtr ptr2 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_this, (IntPtr)desc);
                IntPtr ptr3 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_this, (IntPtr)desc);
                if (ptr1 == IntPtr.Zero || ptr2 == IntPtr.Zero || ptr3 == IntPtr.Zero) {
                    Console.WriteLine("Create Scene null");
                    return null;
                }
                PHSceneIf obj = PHSceneIf.CreateCSInstance(ptr1, ptr2, ptr3);
                return obj;
            } else {
                IntPtr ptr1 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_this, (IntPtr)desc);
                if (ptr1 == IntPtr.Zero) {
                    Console.WriteLine("Create Scene null");
                    return null;
                }
                PHSceneIf obj = PHSceneIf.CreateCSInstance(ptr1);
                return obj;
            }
        }
        public PHSceneIf CreateScene() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateScene_1((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSceneIf obj = new PHSceneIf(ptr);
            return obj;
        }
        public int NScene() {
            int result = (int)SprExport.Spr_PHSdkIf_NScene((IntPtr)_this);
            return result;
        }
        public PHSceneIf GetScene(int i) {
            IntPtr ptr = SprExport.Spr_PHSdkIf_GetScene((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHSceneIf obj = new PHSceneIf(ptr);
            return obj;
        }
        public void MergeScene(PHSceneIf scene0, PHSceneIf scene1) {
            SprExport.Spr_PHSdkIf_MergeScene((IntPtr)_this, (IntPtr)scene0, (IntPtr)scene1);
        }
        public CDShapeIf CreateShape(IfInfo ii, CDShapeDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateShape((IntPtr)_this, (IntPtr)ii, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            return obj;
        }
        public int NShape() {
            int result = (int)SprExport.Spr_PHSdkIf_NShape((IntPtr)_this);
            return result;
        }
        public CDShapeIf GetShape(int i) {
            IntPtr ptr = SprExport.Spr_PHSdkIf_GetShape((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            CDShapeIf obj = new CDShapeIf(ptr);
            return obj;
        }
        public static PHSdkIf CreateSdk(PHSdkDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateSdk((IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHSdkIf obj = new PHSdkIf(ptr);
            return obj;
        }
        public static PHSdkIf CreateSdk() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateSdk_1();
            if (ptr == IntPtr.Zero) { return null; }
            PHSdkIf obj = new PHSdkIf(ptr);
            return obj;
        }
        public static void RegisterSdk() {
            SprExport.Spr_PHSdkIf_RegisterSdk();
        }
    }
    public partial class PHBoneIf : SceneObjectIf {
        public PHBoneIf() { _this = SprExport.Spr_new_PHBoneIf(); _flag = true; }
        public PHBoneIf(IntPtr ptr) : base(ptr) { }
        public PHBoneIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHBoneIf(CsCastObject target) {
            return (target._info.Inherit(PHBoneIf.GetIfInfoStatic()) ? new PHBoneIf(target._this, target._flag) : null);
        }
        ~PHBoneIf() { if (_flag) { SprExport.Spr_delete_PHBoneIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHBoneIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public void SetSolid(PHSolidIf solid) {
            SprExport.Spr_PHBoneIf_SetSolid((IntPtr)_this, (IntPtr)solid);
        }
        public PHSolidIf GetSolid() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetSolid((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public void SetProxySolid(PHSolidIf solid) {
            SprExport.Spr_PHBoneIf_SetProxySolid((IntPtr)_this, (IntPtr)solid);
        }
        public PHSolidIf GetProxySolid() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetProxySolid((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHSolidIf obj = new PHSolidIf(ptr);
            if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) { return new PHHapticPointerIf(ptr); }
            return obj;
        }
        public void SetJoint(PHJointIf joint) {
            SprExport.Spr_PHBoneIf_SetJoint((IntPtr)_this, (IntPtr)joint);
        }
        public PHJointIf GetJoint() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetJoint((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHJointIf obj = new PHJointIf(ptr);
            if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(ptr); }
            if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) { return new PHBallJointIf(ptr); }
            if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(ptr); }
            if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) { return new PHSpringIf(ptr); }
            if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) { return new PHMateIf(ptr); }
            if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) { return new PHHingeJointIf(ptr); }
            if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) { return new PHSliderJointIf(ptr); }
            if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) { return new PHPathJointIf(ptr); }
            if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) { return new PHGenericJointIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) { return new PHPointToPointMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) { return new PHPointToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) { return new PHPointToPlaneMateIf(ptr); }
            if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) { return new PHLineToLineMateIf(ptr); }
            if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) { return new PHPlaneToPlaneMateIf(ptr); }
            return obj;
        }
        public void SetParent(PHBoneIf parent) {
            SprExport.Spr_PHBoneIf_SetParent((IntPtr)_this, (IntPtr)parent);
        }
        public PHBoneIf GetParent() {
            IntPtr ptr = SprExport.Spr_PHBoneIf_GetParent((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            PHBoneIf obj = new PHBoneIf(ptr);
            return obj;
        }
        public void SetLength(double length) {
            SprExport.Spr_PHBoneIf_SetLength((IntPtr)_this, (double)length);
        }
        public void SetDirection(Vec3d dir) {
            SprExport.Spr_PHBoneIf_SetDirection((IntPtr)_this, (IntPtr)dir);
        }
        public void SetPosition(Vec3d pos) {
            SprExport.Spr_PHBoneIf_SetPosition((IntPtr)_this, (IntPtr)pos);
        }
    }
    public partial class PHSkeletonIf : SceneObjectIf {
        public PHSkeletonIf() { _this = SprExport.Spr_new_PHSkeletonIf(); _flag = true; }
        public PHSkeletonIf(IntPtr ptr) : base(ptr) { }
        public PHSkeletonIf(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public static implicit operator PHSkeletonIf(CsCastObject target) {
            return (target._info.Inherit(PHSkeletonIf.GetIfInfoStatic()) ? new PHSkeletonIf(target._this, target._flag) : null);
        }
        ~PHSkeletonIf() { if (_flag) { SprExport.Spr_delete_PHSkeletonIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_ToString((IntPtr)_this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_PHSkeletonIf_FreeString((IntPtr)ptr);
            return bstr;
        }
        public IfInfo GetIfInfo() {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_GetIfInfo((IntPtr)_this);
            return new IfInfo(ptr);
        }
        public static IfInfo GetIfInfoStatic() {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_GetIfInfoStatic();
            return new IfInfo(ptr);
        }
        public int NBones() {
            int result = (int)SprExport.Spr_PHSkeletonIf_NBones((IntPtr)_this);
            return result;
        }
        public PHBoneIf GetBone(int i) {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_GetBone((IntPtr)_this, (int)i);
            if (ptr == IntPtr.Zero) { return null; }
            PHBoneIf obj = new PHBoneIf(ptr);
            return obj;
        }
        public PHBoneIf CreateBone(PHBoneIf parent, PHBoneDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_CreateBone((IntPtr)_this, (IntPtr)parent, (IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHBoneIf obj = new PHBoneIf(ptr);
            return obj;
        }
        public PHBoneIf CreateBone(PHBoneIf parent) {
            IntPtr ptr = SprExport.Spr_PHSkeletonIf_CreateBone_1((IntPtr)_this, (IntPtr)parent);
            if (ptr == IntPtr.Zero) { return null; }
            PHBoneIf obj = new PHBoneIf(ptr);
            return obj;
        }
        public float GetGrabStrength() {
            float result = (float)SprExport.Spr_PHSkeletonIf_GetGrabStrength((IntPtr)_this);
            return result;
        }
    }
    public partial class IfInfoToCsType {
        public static Dictionary<IntPtr, Type> mapPhysics = new Dictionary<IntPtr, Type>() {
            {PHEngineIf.GetIfInfoStatic(), typeof(PHEngineIf)},
            {PHConstraintEngineIf.GetIfInfoStatic(), typeof(PHConstraintEngineIf)},
            {PHGravityEngineIf.GetIfInfoStatic(), typeof(PHGravityEngineIf)},
            {PHPenaltyEngineIf.GetIfInfoStatic(), typeof(PHPenaltyEngineIf)},
            {PHIKEngineIf.GetIfInfoStatic(), typeof(PHIKEngineIf)},
            {PHFemEngineIf.GetIfInfoStatic(), typeof(PHFemEngineIf)},
            {PHOpEngineIf.GetIfInfoStatic(), typeof(PHOpEngineIf)},
            {PHFemMeshIf.GetIfInfoStatic(), typeof(PHFemMeshIf)},
            {PHFemMeshThermoIf.GetIfInfoStatic(), typeof(PHFemMeshThermoIf)},
            {PHFemMeshNewIf.GetIfInfoStatic(), typeof(PHFemMeshNewIf)},
            {PHFemBaseIf.GetIfInfoStatic(), typeof(PHFemBaseIf)},
            {PHFemVibrationIf.GetIfInfoStatic(), typeof(PHFemVibrationIf)},
            {PHFemThermoIf.GetIfInfoStatic(), typeof(PHFemThermoIf)},
            {PHFemPorousWOMoveIf.GetIfInfoStatic(), typeof(PHFemPorousWOMoveIf)},
            {PHFrameIf.GetIfInfoStatic(), typeof(PHFrameIf)},
            {PHBodyIf.GetIfInfoStatic(), typeof(PHBodyIf)},
            {PHSolidIf.GetIfInfoStatic(), typeof(PHSolidIf)},
            {PHShapePairForLCPIf.GetIfInfoStatic(), typeof(PHShapePairForLCPIf)},
            {PHSolidPairIf.GetIfInfoStatic(), typeof(PHSolidPairIf)},
            {PHSolidPairForLCPIf.GetIfInfoStatic(), typeof(PHSolidPairForLCPIf)},
            {PHHapticPointerIf.GetIfInfoStatic(), typeof(PHHapticPointerIf)},
            {PHShapePairForHapticIf.GetIfInfoStatic(), typeof(PHShapePairForHapticIf)},
            {PHSolidPairForHapticIf.GetIfInfoStatic(), typeof(PHSolidPairForHapticIf)},
            {PHHapticEngineIf.GetIfInfoStatic(), typeof(PHHapticEngineIf)},
            {PHIKEndEffectorIf.GetIfInfoStatic(), typeof(PHIKEndEffectorIf)},
            {PHIKActuatorIf.GetIfInfoStatic(), typeof(PHIKActuatorIf)},
            {PHIKBallActuatorIf.GetIfInfoStatic(), typeof(PHIKBallActuatorIf)},
            {PHIKHingeActuatorIf.GetIfInfoStatic(), typeof(PHIKHingeActuatorIf)},
            {PHIKSpringActuatorIf.GetIfInfoStatic(), typeof(PHIKSpringActuatorIf)},
            {PHConstraintIf.GetIfInfoStatic(), typeof(PHConstraintIf)},
            {PHConstraintsIf.GetIfInfoStatic(), typeof(PHConstraintsIf)},
            {PHContactPointIf.GetIfInfoStatic(), typeof(PHContactPointIf)},
            {PHJointIf.GetIfInfoStatic(), typeof(PHJointIf)},
            {PH1DJointIf.GetIfInfoStatic(), typeof(PH1DJointIf)},
            {PHHingeJointIf.GetIfInfoStatic(), typeof(PHHingeJointIf)},
            {PHSliderJointIf.GetIfInfoStatic(), typeof(PHSliderJointIf)},
            {PHPathIf.GetIfInfoStatic(), typeof(PHPathIf)},
            {PHPathJointIf.GetIfInfoStatic(), typeof(PHPathJointIf)},
            {PHGenericJointIf.GetIfInfoStatic(), typeof(PHGenericJointIf)},
            {PHBallJointIf.GetIfInfoStatic(), typeof(PHBallJointIf)},
            {PHFixJointIf.GetIfInfoStatic(), typeof(PHFixJointIf)},
            {PHSpringIf.GetIfInfoStatic(), typeof(PHSpringIf)},
            {PHMateIf.GetIfInfoStatic(), typeof(PHMateIf)},
            {PHPointToPointMateIf.GetIfInfoStatic(), typeof(PHPointToPointMateIf)},
            {PHPointToLineMateIf.GetIfInfoStatic(), typeof(PHPointToLineMateIf)},
            {PHPointToPlaneMateIf.GetIfInfoStatic(), typeof(PHPointToPlaneMateIf)},
            {PHLineToLineMateIf.GetIfInfoStatic(), typeof(PHLineToLineMateIf)},
            {PHPlaneToPlaneMateIf.GetIfInfoStatic(), typeof(PHPlaneToPlaneMateIf)},
            {PHTreeNodeIf.GetIfInfoStatic(), typeof(PHTreeNodeIf)},
            {PHRootNodeIf.GetIfInfoStatic(), typeof(PHRootNodeIf)},
            {PHTreeNode1DIf.GetIfInfoStatic(), typeof(PHTreeNode1DIf)},
            {PHHingeJointNodeIf.GetIfInfoStatic(), typeof(PHHingeJointNodeIf)},
            {PHSliderJointNodeIf.GetIfInfoStatic(), typeof(PHSliderJointNodeIf)},
            {PHPathJointNodeIf.GetIfInfoStatic(), typeof(PHPathJointNodeIf)},
            {PHGenericJointNodeIf.GetIfInfoStatic(), typeof(PHGenericJointNodeIf)},
            {PHBallJointNodeIf.GetIfInfoStatic(), typeof(PHBallJointNodeIf)},
            {PHFixJointNodeIf.GetIfInfoStatic(), typeof(PHFixJointNodeIf)},
            {PHGearIf.GetIfInfoStatic(), typeof(PHGearIf)},
            {PH1DJointLimitIf.GetIfInfoStatic(), typeof(PH1DJointLimitIf)},
            {PHBallJointLimitIf.GetIfInfoStatic(), typeof(PHBallJointLimitIf)},
            {PHBallJointConeLimitIf.GetIfInfoStatic(), typeof(PHBallJointConeLimitIf)},
            {PHBallJointSplineLimitIf.GetIfInfoStatic(), typeof(PHBallJointSplineLimitIf)},
            {PHBallJointIndependentLimitIf.GetIfInfoStatic(), typeof(PHBallJointIndependentLimitIf)},
            {PH1DJointMotorIf.GetIfInfoStatic(), typeof(PH1DJointMotorIf)},
            {PH1DJointNonLinearMotorIf.GetIfInfoStatic(), typeof(PH1DJointNonLinearMotorIf)},
            {PHHuman1DJointResistanceIf.GetIfInfoStatic(), typeof(PHHuman1DJointResistanceIf)},
            {PHBallJointMotorIf.GetIfInfoStatic(), typeof(PHBallJointMotorIf)},
            {PHBallJointNonLinearMotorIf.GetIfInfoStatic(), typeof(PHBallJointNonLinearMotorIf)},
            {PHHumanBallJointResistanceIf.GetIfInfoStatic(), typeof(PHHumanBallJointResistanceIf)},
            {PHSpringMotorIf.GetIfInfoStatic(), typeof(PHSpringMotorIf)},
            {PHOpObjIf.GetIfInfoStatic(), typeof(PHOpObjIf)},
            {PHOpParticleIf.GetIfInfoStatic(), typeof(PHOpParticleIf)},
            {PHOpGroupIf.GetIfInfoStatic(), typeof(PHOpGroupIf)},
            {PHOpHapticControllerIf.GetIfInfoStatic(), typeof(PHOpHapticControllerIf)},
            {PHOpHapticRendererIf.GetIfInfoStatic(), typeof(PHOpHapticRendererIf)},
            {PHOpAnimationIf.GetIfInfoStatic(), typeof(PHOpAnimationIf)},
            {PHOpSpHashColliAgentIf.GetIfInfoStatic(), typeof(PHOpSpHashColliAgentIf)},
            {PHRayIf.GetIfInfoStatic(), typeof(PHRayIf)},
            {PHSceneIf.GetIfInfoStatic(), typeof(PHSceneIf)},
            {PHSdkIf.GetIfInfoStatic(), typeof(PHSdkIf)},
            {PHBoneIf.GetIfInfoStatic(), typeof(PHBoneIf)},
            {PHSkeletonIf.GetIfInfoStatic(), typeof(PHSkeletonIf)},
        };
    }
}
