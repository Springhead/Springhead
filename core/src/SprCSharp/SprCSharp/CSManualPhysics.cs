using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SprCs {
    public partial class PHSceneIf : SceneIf {
        public bool isStepping = false;
        public bool isFixedUpdating = false;
        public bool isSwapping = false;
        public bool threadMode = false;
        public bool show_this2 = false;
        public bool fixedUpdateFinished = true;
        public readonly object phSceneForGetSetLock = new object();
        public int sceneForStep = 0;
        public int sceneForGet = 1;
        public int sceneForBuffer = 2;

        private static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        private List<ThreadCallback> waitDuringStepCallbackList = new List<ThreadCallback>();
        private ObjectStatesIf state = ObjectStatesIf.Create();
        // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
        // Thread処理用
        public delegate void ThreadCallback();
        //public List<ThreadCallback> waitUntilNextStepCallbackList = new List<ThreadCallback>();
        public void ExecWaitUntilNextStepCallbackList() { // Step側でLockする
            if (waitDuringStepCallbackList.Count != 0) {
                Console.WriteLine("waitduringstepcallbacklist " + waitDuringStepCallbackList.Count);
            }
            foreach (var callback in waitDuringStepCallbackList) {
                callback();
            }
            waitDuringStepCallbackList.Clear();
        }
        public void AddWaitUntilNextStepCallback(ThreadCallback callback) {
            waitDuringStepCallbackList.Add(callback);
        }
        public static PHSceneIf CreateCSInstance(IntPtr stepPHScene, IntPtr bufferPHScene, IntPtr getPHScene) { // 次にStepされるPHSceneIfを返す
            if (!instances.ContainsKey(stepPHScene)) { // defaultIntPtrをinstances[defaultIntPtr]._thisに代入
                instances[stepPHScene] = new PHSceneIf(stepPHScene);
                instances[stepPHScene]._thisArray[0] = stepPHScene;
            }
            instances[stepPHScene]._thisArray[1] = bufferPHScene;
            instances[stepPHScene]._thisArray[2] = getPHScene;
            return instances[stepPHScene];
        }
        public static PHSceneIf GetCSInstance(IntPtr intptr1) { // 引数にはGetSceneしてきた_thisを入れる
            PHSceneIf defaultPHSceneIf = null;
            if (instances.ContainsKey(intptr1)) {
                defaultPHSceneIf = instances[intptr1];
                return defaultPHSceneIf;
            }
            return null;
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
        public void Step() {
            var phSceneIf = PHSceneIf.GetCSInstance(_this);
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    phSceneIf.isStepping = true;
                }
                SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForStep]);
            } else {
                if (phSceneIf.show_this2) {
                    SprExport.Spr_PHSceneIf_Step(_this2);
                    SprExport.Spr_ObjectStatesIf_SaveState(phSceneIf.state._this, _this2); // _this2→state
                    SprExport.Spr_ObjectStatesIf_LoadState(phSceneIf.state._this, _this); // state→_this
                } else {
                    SprExport.Spr_PHSceneIf_Step(_this);
                }
            }
        }
        public void Swap() {
            var phSceneIf = PHSceneIf.GetCSInstance(_thisArray[0]);
            lock (phSceneIf.phSceneForGetSetLock) {
                phSceneIf.ExecWaitUntilNextStepCallbackList(); // NotnextStepPHSceneを呼ぶとSave/Load後に呼ばれるべきCallbackが先に実行されてしまう
                SprExport.Spr_ObjectStatesIf_SaveState(phSceneIf.state._this, _thisArray[sceneForStep]); // phScene→state
                if (!isFixedUpdating) { // Step↔Get
                    SprExport.Spr_ObjectStatesIf_LoadState(phSceneIf.state._this, _thisArray[sceneForGet]); // state→phScene
                    var temp = sceneForStep;
                    sceneForStep = sceneForGet;
                    sceneForGet = temp;
                } else { // Step↔Buffer
                    isSwapping = true;
                    SprExport.Spr_ObjectStatesIf_LoadState(phSceneIf.state._this, _thisArray[sceneForBuffer]); // state→phScene
                    var temp = sceneForStep;
                    sceneForStep = sceneForBuffer;
                    sceneForBuffer = temp;
                }
                phSceneIf.isStepping = false;
            }
        }
        public void SwapAfterFixedUpdate() {
            var phSceneIf = PHSceneIf.GetCSInstance(_thisArray[0]);
            lock (phSceneIf.phSceneForGetSetLock) {
                if (isSwapping) { // Buffer↔Get
                    Console.WriteLine("Use Buffer");
                    var temp = sceneForBuffer;
                    sceneForBuffer = sceneForGet;
                    sceneForGet = temp;
                    isSwapping = false;
                }
                isFixedUpdating = false;
            }
        }
    }

    public partial class PHSpringIf : PHJointIf {
        public PHSpringIf(IntPtr ptr, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _this = ptr; // <!!> SetNameのために
            _thisArray[0] = ptr;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
        public void SetTargetVelocityAsync(Vec6d v) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec6d(v);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV)
                        );
                        SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v); }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
            }
        }
        public void SetTargetOrientationAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newQ = new Quaterniond(q);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newQ)
                        );
                        SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newQ);
                        SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newQ);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)q); }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)q);
            }
        }
        public void SetTargetPositionAsync(Vec3d p) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Vec3d(p);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newP)
                        );
                        SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newP);
                        SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)p); }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)p);
            }
        }
    }
    public partial class PHBallJointIf : PHJointIf {
        public PHBallJointIf(IntPtr ptr, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _this = ptr; // <!!> SetNameのために
            _thisArray[0] = ptr;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
        // Thread処理のためのメソッド
        public void SetTargetVelocityAsync(Vec3d v) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec3d(v);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV)
                        );
                        SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
            }
        }
        public void SetTargetPositionAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newQ = new Quaterniond(q);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newQ)
                        );
                        SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newQ);
                        SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newQ);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)q);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)q);
            }
        }
    }

    public partial class PHSolidIf : PHBodyIf {
        // Thread処理のためのメソッド
        public void SetPoseAsync(Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Posed(p);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newP)
                        );
                        SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newP);
                        SprExport.Spr_PHSolidIf_SetPose((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)p);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)p);
            }
        }
        public void SetShapePoseAsync(int index, Posed p) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newP = new Posed(p);
                        phSceneIf.AddWaitUntilNextStepCallback(() =>
                        SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForStep], (int)index, (IntPtr)newP)
                        );
                        SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (int)index, (IntPtr)newP);
                        SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForGet], (int)index, (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this, (int)index, (IntPtr)p);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_this, (int)index, (IntPtr)p);
            }
        }
    }

    public partial class SceneObjectIf : NamedObjectIf {
        public PHSceneIf GetCSPHSceneIf() {
            IntPtr ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_thisArray[0]);
            if (ptr == IntPtr.Zero) { return null; }
            return PHSceneIf.GetCSInstance(ptr);
        }
    }

    public partial class PHBodyIf : SceneObjectIf {
        public Posed GetPoseAsync() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            lock (phSceneIf.phSceneForGetSetLock) {
                phSceneIf.isFixedUpdating = true;
                IntPtr ptr = SprExport.Spr_PHBodyIf_GetPose(
                    (IntPtr)_thisArray[phSceneIf.sceneForGet]); // ここで取得されるPosedは複製
                return new Posed(ptr, true);
            }
        }
    }

}
