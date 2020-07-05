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
        public void ExecWaitUntilNextStepCallbackList() { // Step側でLockする
            //Console.Write()
            foreach (var callback in waitDuringStepCallbackList) {
                callback();
            }
            waitDuringStepCallbackList.Clear();
        }
        public void AddWaitUntilNextStepCallback(ThreadCallback callback) {
            waitDuringStepCallbackList.Add(callback);
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
        public void GetNextStepAndNotNextStepPHScene(ref IntPtr nextStepPHScene, ref IntPtr notNextStepPHScene) { // isStepping==trueの場合nextStepPHSceneがStep中
            if (step_thisOnNext) {
                nextStepPHScene = _this;
                notNextStepPHScene = _this2;
            } else {
                nextStepPHScene = _this2;
                notNextStepPHScene = _this;
            }
        }
        //public IntPtr GetNotNextStepPHScene() { // GetやSetが使用
        //    lock (phSceneForGetSetLock) {
        //        if (step_thisOnNext) {
        //            return _this;
        //        } else {
        //            return _this2;
        //        }
        //    }
        //}
        public void ChangeNextStep() {
            step_thisOnNext = !step_thisOnNext;
        }
        public bool isStepping = false;
        public bool threadMode = false;
        public bool step_thisOnNext = true; // true:次_thisがStep false:次_this2がStep 
        public bool show_this2 = false;
        public readonly object phSceneForGetSetLock = new object();
        private static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        private List<ThreadCallback> waitDuringStepCallbackList = new List<ThreadCallback>();
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
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newV = new Vec6d(v);
                    SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)getsetObject, (IntPtr)newV);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)steppingObject, (IntPtr)newV));
                } else {
                    SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
                    SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this2, (IntPtr)v);
                }
            }
        }
        public void SetTargetOrientationAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newQ = new Quaterniond(q);
                    SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)getsetObject, (IntPtr)newQ);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)steppingObject, (IntPtr)newQ));
                } else {
                    SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)q);
                    SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this2, (IntPtr)q);
                }
            }
        }
        public void SetTargetPositionAsync(Vec3d p) {
            var phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newP = new Vec3d(p);
                    SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)getsetObject, (IntPtr)newP);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)steppingObject, (IntPtr)newP));
                } else {
                    SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)p);
                    SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this2, (IntPtr)p);
                }
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
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newV = new Vec3d(v);
                    SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)getsetObject, (IntPtr)newV);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)steppingObject, (IntPtr)newV));
                } else {
                    SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
                    SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this2, (IntPtr)v);
                }
            }
        }
        public void SetTargetPositionAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newQ = new Quaterniond(q);
                    SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)getsetObject, (IntPtr)newQ);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)steppingObject, (IntPtr)newQ));
                } else {
                    SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)q);
                    SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this2, (IntPtr)q);
                }
            }
        }
    }

    public partial class PHSolidIf : PHBodyIf {
        // Thread処理のためのメソッド
        public void SetPoseAsync(Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newP = new Posed(p);
                    SprExport.Spr_PHSolidIf_SetPose((IntPtr)getsetObject, (IntPtr)newP);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHSolidIf_SetPose((IntPtr)steppingObject, (IntPtr)newP));
                } else {
                    SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)p);
                    SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this2, (IntPtr)p);
                }
            }
        }
        public void SetShapePoseAsync(int index, Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                    IntPtr steppingObject = IntPtr.Zero;
                    IntPtr getsetObject = IntPtr.Zero;
                    GetSteppingObjectAndGetSetObject(ref steppingObject, ref getsetObject);
                    var newP = new Posed(p);
                    SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)getsetObject, (int)index, (IntPtr)newP);
                    phSceneIf.AddWaitUntilNextStepCallback(
                        () => SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)steppingObject, (int)index, (IntPtr)newP));
                } else {
                    SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this, (int)index, (IntPtr)p);
                    SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this2, (int)index, (IntPtr)p);
                }
            }
        }
    }

    public partial class SceneObjectIf : NamedObjectIf {
        public PHSceneIf GetCSPHSceneIf() {
            IntPtr ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_this);
            if (ptr == IntPtr.Zero) { return null; }
            return PHSceneIf.GetCSInstance(ptr);
        }
        public IntPtr GetNotNextStepObject(bool nextStep) { // 呼ぶ側でphSceneIf.nextStepLockをlockする必要がある
            if (nextStep) {
                return _this2;
            } else {
                return _this;
            }
        }
        public void GetSteppingObjectAndGetSetObject(ref IntPtr steppingObject, ref IntPtr getsetObject) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.step_thisOnNext) {
                steppingObject = _this;
                getsetObject = _this2;
            } else {
                steppingObject = _this2;
                getsetObject = _this;
            }
        }
    }

    public partial class PHBodyIf : SceneObjectIf {
        public Posed GetPoseAsync() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                IntPtr ptr = SprExport.Spr_PHBodyIf_GetPose(
                    (IntPtr)GetNotNextStepObject(phSceneIf.step_thisOnNext)); // ここで取得されるPosedは直参照か複製か
                return new Posed(ptr, true);
            }
        }
    }

}
