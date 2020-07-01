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
        public static PHSceneIf GetCSInstance(IntPtr intptr) {
            if (!instances.ContainsKey(intptr)) { instances[intptr] = new PHSceneIf(intptr); }
            return instances[intptr];
        }
        private static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
    }

    public partial class PHSpringIf : PHJointIf {
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
	    IntPtr ptr = SprExport.Spr_PHConstraintIf_GetScene((IntPtr) _this);
            if (ptr == IntPtr.Zero) { return null; } 
            return PHSceneIf.GetCSInstance(ptr);
        }
    }
    public partial class PHBallJointIf : PHJointIf {
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
	        IntPtr ptr = SprExport.Spr_PHConstraintIf_GetScene((IntPtr) _this);
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
}
