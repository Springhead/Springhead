using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SprCs {
    public partial class PHSceneIf : SceneIf {
        // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
        // Thread処理用
        public delegate void ThreadCallback();
        //public List<ThreadCallback> waitUntilNextStepCallbackList = new List<ThreadCallback>();
        public void ExecWaitUntilNextStepCallbackList(IntPtr intPtr) { // Step側でLockする
            foreach (var callback in waitUntilNextStepCallbackDictionary[intPtr]) {
                callback();
            }
            waitUntilNextStepCallbackDictionary[intPtr].Clear();
        }
        public void AddWaitUntilNextStepCallback(IntPtr intPtr, ThreadCallback callback) {
            lock (waitUntilNextStepCallbackDictionaryLock) { //StepのExecWaitUntilNextStepCallbackListに合わせてwaitUntilNextStepCallbackDictionaryでロック
                waitUntilNextStepCallbackDictionary[intPtr].Add(callback);
            }
        }
        public static PHSceneIf CreateCSInstance(IntPtr defaultIntPtr, IntPtr threadIntPtr) { // 次にStepされるPHSceneIfを返す
            if (!instances.ContainsKey(defaultIntPtr)) { // defaultIntPtrをinstances[defaultIntPtr]._thisに代入
                instances[defaultIntPtr] = new PHSceneIf(defaultIntPtr);
            }
            instances[defaultIntPtr]._this2 = threadIntPtr;
            instances[defaultIntPtr].waitUntilNextStepCallbackDictionary[defaultIntPtr] = new List<ThreadCallback>(); // ここで初期化するしかないのか？
            instances[defaultIntPtr].waitUntilNextStepCallbackDictionary[threadIntPtr] = new List<ThreadCallback>();
            return instances[defaultIntPtr];
        }
        public static PHSceneIf GetCSInstance(IntPtr intptr1) { // 引数にはGetSceneしてきた_thisを入れる
            PHSceneIf defaultPHSceneIf = null;
            if (instances.ContainsKey(intptr1)) {
                defaultPHSceneIf = instances[intptr1];
                return defaultPHSceneIf;
            }
            return null;
        }
        public void GetNextStepAndNotNextStepPHScene(ref IntPtr nextStepPHScene, ref IntPtr notNextStepPHScene) {
            lock (nextStepLock) {
                if (nextStep) {
                    nextStepPHScene = _this;
                    notNextStepPHScene = _this2;
                } else {
                    nextStepPHScene = _this2;
                    notNextStepPHScene = _this;
                }
            }
        }
        public IntPtr GetNotNextStepPHScene() { // GetやSetが使用
            lock (nextStepLock) {
                if (nextStep) {
                    return _this;
                } else {
                    return _this2;
                }
            }
        }
        public void ChangeNextStep() {
            lock (nextStepLock) {
                nextStep = !nextStep;
            }
            //instances[_this2].nextStep = !nextStep;
        }
        public bool threadMode = false;
        public bool nextStep = true; // true:次_thisがStep false:次_this2がStep 
        public bool show_this2 = false;
        public readonly object waitUntilNextStepCallbackDictionaryLock = new object();
        private static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        private Dictionary<IntPtr, List<ThreadCallback>> waitUntilNextStepCallbackDictionary = new Dictionary<IntPtr, List<ThreadCallback>>();
        private ObjectStatesIf state = ObjectStatesIf.Create();
        private readonly object nextStepLock = new object(); // nextStep用Lock変数
    }

    public partial class PHSpringIf : PHJointIf {
        public PHSpringIf(IntPtr ptr, IntPtr ptr2, bool flag = false) {
            _this = ptr;
            _this2 = ptr2;
            _flag = flag;
        }
        public void SetTargetVelocityAsync(Vec6d v) {
            var phSceneIf = GetCSPHSceneIf();
            var newV = new Vec6d(v);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)newV));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this2, (IntPtr)newV));
        }
        public void SetTargetOrientationAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            var newQ = new Quaterniond(q);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)newQ));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this2, (IntPtr)newQ));
        }
        public void SetTargetPositionAsync(Vec3d p) {
            var phSceneIf = GetCSPHSceneIf();
            var newP = new Vec3d(p);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)newP));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this2, (IntPtr)newP));
        }
        public PHSceneIf GetCSPHSceneIf() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            return PHSceneIf.GetCSInstance(ptr);
        }
    }
    public partial class PHBallJointIf : PHJointIf {
        public PHBallJointIf(IntPtr ptr, IntPtr ptr2, bool flag = false) {
            _this = ptr;
            _this2 = ptr2;
            _flag = flag;
        }
        // Thread処理のためのメソッド
        public void SetTargetVelocityAsync(Vec3d v) {
            var phSceneIf = GetCSPHSceneIf();
            var newV = new Vec3d(v);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)newV));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this2, (IntPtr)newV));
        }
        public void SetTargetPositionAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            var newQ = new Quaterniond(q);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)newQ));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this2, (IntPtr)newQ));
        }
        public PHSceneIf GetCSPHSceneIf() {
            IntPtr ptr = SprExport.Spr_PHConstraintIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            return PHSceneIf.GetCSInstance(ptr);
        }
    }

    public partial class PHSolidIf : PHBodyIf {
        // Thread処理のためのメソッド
        public void SetPoseAsync(Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            var newP = new Posed(p);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)newP));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this2, (IntPtr)newP));
        }
        public void SetShapePoseAsync(int index, Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            var newP = new Posed(p);
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this,
                () => SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this, (int)index, (IntPtr)newP));
            phSceneIf.AddWaitUntilNextStepCallback(phSceneIf._this2,
                () => SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this2, (int)index, (IntPtr)newP));
        }
        public PHSceneIf GetCSPHSceneIf() {
            IntPtr ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            return PHSceneIf.GetCSInstance(ptr);
        }
    }
    //public partial class PHBodyIf : SceneObjectIf {
    //    public IntPtr _this2;
    //}
}
