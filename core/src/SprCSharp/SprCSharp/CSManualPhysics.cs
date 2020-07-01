using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SprCs {
    public partial class PHSceneIf : SceneIf {
        // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
        // Thread処理用
        public delegate void ThreadCallback();
        public List<ThreadCallback> waitUntilNextStepCallbackList = new List<ThreadCallback>();
        public void ExecWaitUntilNextStepCallbackList() {
            lock (waitUntilNextStepCallbackList) {
                foreach (var callback in waitUntilNextStepCallbackList) {
                    callback();
                }
                waitUntilNextStepCallbackList.Clear();
            }
        }
        public static PHSceneIf CreateCSInstance(IntPtr defaultIntPtr, IntPtr threadIntPtr) { // 次にStepされるPHSceneIfを返す
            if (!instances.ContainsKey(defaultIntPtr)) { // defaultIntPtrをinstances[defaultIntPtr]._thisに代入
                instances[defaultIntPtr] = new PHSceneIf(defaultIntPtr);
            }
            instances[defaultIntPtr]._this2 = threadIntPtr;
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
        public void GetNextStepPHScene(ref IntPtr nextStepPHScene, ref IntPtr notNextStepPHScene) {
            if (nextStep) {
                nextStepPHScene = _this;
                notNextStepPHScene = _this2;
            } else {
                nextStepPHScene = _this2;
                notNextStepPHScene = _this;
            }
        }
        public void ChangeNextStep() {
            nextStep = !nextStep;
            //instances[_this2].nextStep = !nextStep;
        }
        public bool threadMode = false;
        public bool nextStep = true; // 次Stepするか 下手すると二つとも
        public bool show_this2 = false;
        private static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        private ObjectStatesIf state = ObjectStatesIf.Create();
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
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetTargetVelocity(newV));
            }
        }
        public void SetTargetOrientationAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            var newQ = new Quaterniond(q);
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetTargetOrientation(newQ));
            }
        }
        public void SetTargetPositionAsync(Vec3d p) {
            var phSceneIf = GetCSPHSceneIf();
            var newP = new Vec3d(p);
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetTargetPosition(newP));
            }
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
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetTargetVelocity(newV));
            }
        }
        public void SetTargetPositionAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            var newQ = new Quaterniond(q);
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetTargetPosition(newQ));
            }
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
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetPose(newP));
            }
        }
        public void SetShapePoseAsync(int num, Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            var newP = new Posed(p);
            lock (phSceneIf.waitUntilNextStepCallbackList) {
                phSceneIf.waitUntilNextStepCallbackList.Add(() => SetShapePose(num, newP));
            }
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
