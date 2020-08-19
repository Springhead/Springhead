using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SprCs {
    public partial class PHSdkIf : SdkIf {
        public readonly object phSdkLock = new object();
        public override bool DelChildObject(ObjectIf o) { // PHSDKから引数CDShapeBehaviourで呼ばれることを想定
            lock (phSdkLock) {
                Console.WriteLine("PHSdkIf.DelChildObject");
                char ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
                if (ret0 == 0) {
                    Console.WriteLine("failed PHSdkIf.DelChildObject");
                    return false;
                } else {
                    return true;
                }
            }
        }
    }
    public partial class PHSceneIf : SceneIf {
        public bool isStepping = false;
        public bool isFixedUpdating = false;
        public bool isSwapping = false;
        public bool multiThreadMode = false;
        public readonly object phSceneForGetSetLock = new object();
        public int sceneForStep = 0;
        public int sceneForGet = 1;
        public int sceneForBuffer = 2;

        public static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        public readonly AutoResetEvent fixedUpdateAutoWait = new AutoResetEvent(false);
        //Set系メソッド用(fixedUpdateAutoWaitの実装にも関与)，速度などの更新にはこれが必須，位置の更新などの更新に関してはこの機能を一度も使用しないコードでは位置更新途中でStepが実行されかねない
        public Dictionary<System.Object, bool> executeSetFunctionFlagDictionary = new Dictionary<System.Object, bool>(); // インスタンスごとにステップ済みかのフラグを用意，trueにするのはTimer内，falseにするのは各インスタンス
        public bool changeAllExecuteSetFunctionFlagsTrue = false;
        public bool changedAllExecuteSetFunctionFlagsTrue = true;
        public bool firstGetExecuteSetFunctionInOneFixedUpdate = true; // FixedUpdateのDefaultExecutionOrderを使わないための苦肉の策

        //Get系メソッド用，そのまま実行してもFixedUpdate中に値が変わらないため安全だが，より効率を求める場合に使用(デフォルトの機能用PHSceneの更新など)
        public Dictionary<System.Object, bool> executeGetFunctionFlagDictionary = new Dictionary<System.Object, bool>(); // インスタンスごとにステップ済みかのフラグを用意，trueにするのはTimer内，falseにするのは各インスタンス
        public bool changeAllExecuteGetFunctionFlagsTrue = false;
        public bool firstGetExecuteGetFunctionInOneFixedUpdate = true;

        public Thread stepThread = null; // Stepを実行するスレッド
        public Thread subThread = null; // 描画用のスレッド(Unity想定)
        // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
        // Thread処理用
        private List<ThreadCallback> waitDuringStepCallbackList = new List<ThreadCallback>();
        private List<ThreadCallback> callbackForStepThread = new List<ThreadCallback>();
        private List<ThreadCallback> callbackForStepThreadOnSwapAfterFixedUpdate = new List<ThreadCallback>();
        private ObjectStatesIf stateForSwap = ObjectStatesIf.Create();
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
        public void ExecCallbackForStepThread() {
            if (callbackForStepThread.Count != 0) {
                Console.WriteLine("callbackForStepThread " + callbackForStepThread.Count);
            }
            foreach (var callback in callbackForStepThread) {
                callback();
            }
            callbackForStepThread.Clear();
        }
        public void AddCallbackForStepThread(ThreadCallback callback) {
            callbackForStepThread.Add(callback);
        }
        public void ExecCallbackForStepThreadOnSwapAfterFixedUpdate() {
            if (callbackForStepThreadOnSwapAfterFixedUpdate.Count != 0) {
                Console.WriteLine("callbackForStepThreadOnSwapAfterFixedUpdate " + callbackForStepThreadOnSwapAfterFixedUpdate.Count);
            }
            foreach (var callback in callbackForStepThreadOnSwapAfterFixedUpdate) {
                callback();
            }
            callbackForStepThreadOnSwapAfterFixedUpdate.Clear();
        }
        public static PHSceneIf CreateCSInstance(IntPtr stepPHScene) {
            if (!instances.ContainsKey(stepPHScene)) { // defaultIntPtrをinstances[defaultIntPtr]._thisに代入
                var newPHSceneIf = new PHSceneIf(stepPHScene);
                instances[stepPHScene] = newPHSceneIf;
                instances[stepPHScene]._thisArray[0] = stepPHScene;
            }
            return instances[stepPHScene];
        }
        public static PHSceneIf CreateCSInstance(IntPtr stepPHScene, IntPtr bufferPHScene, IntPtr getPHScene) { // 次にStepされるPHSceneIfを返す
            if (!instances.ContainsKey(stepPHScene)) { // defaultIntPtrをinstances[defaultIntPtr]._thisに代入
                var newPHSceneIf = new PHSceneIf(stepPHScene);
                instances[stepPHScene] = newPHSceneIf;
                instances[bufferPHScene] = newPHSceneIf;
                instances[getPHScene] = newPHSceneIf;
                instances[stepPHScene]._thisArray[0] = stepPHScene;
            }
            instances[stepPHScene]._thisArray[1] = bufferPHScene;
            if (instances[stepPHScene]._thisArray[1] == IntPtr.Zero) {
                Console.WriteLine("instances[stepPHScene]._thisArray[1] null");
            } else {
                Console.WriteLine("instances[stepPHScene]._thisArray[1] not null");
            }
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
            if (multiThreadMode) {
                lock (GetSdk().phSdkLock) { // phSdkのDelChildObjectはStep中に呼ばれないように
                    SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForStep]);
                }
            } else {
                SprExport.Spr_PHSceneIf_Step(_thisArray[0]);
            }
        }
        public void Swap() {
            lock (phSceneForGetSetLock) {
                if (isSwapping) {
                    Console.WriteLine("isSwapping True");
                }
                ExecWaitUntilNextStepCallbackList(); // NotnextStepPHSceneを呼ぶとSave/Load後に呼ばれるべきCallbackが先に実行されてしまう
                SprExport.Spr_ObjectStatesIf_SaveState(stateForSwap._this, _thisArray[sceneForStep]); // phScene→state
                if (!isFixedUpdating) { // Step↔Get
                    ExecCallbackForStepThread();
                    SprExport.Spr_ObjectStatesIf_LoadState(stateForSwap._this, _thisArray[sceneForGet]); // state→phScene
                    var temp = sceneForStep;
                    sceneForStep = sceneForGet;
                    sceneForGet = temp;
                    Console.WriteLine("Swap Step↔Get changeAllExecuteGetFunctionFlagsTrue = true");
                    changeAllExecuteGetFunctionFlagsTrue = true;
                } else { // Step↔Buffer
                    Console.WriteLine("Swap Step↔Buffer");
                    isSwapping = true;
                    callbackForStepThreadOnSwapAfterFixedUpdate = new List<ThreadCallback>(callbackForStepThread); // ディープコピー
                    callbackForStepThread.Clear();
                    SprExport.Spr_ObjectStatesIf_LoadState(stateForSwap._this, _thisArray[sceneForBuffer]); // state→phScene
                    var temp = sceneForStep;
                    sceneForStep = sceneForBuffer;
                    sceneForBuffer = temp;
                }
                isStepping = false;

            }
        }
        public void SwapAfterFixedUpdate() {
            lock (phSceneForGetSetLock) {
                if (isSwapping) { // Buffer↔Get
                    ExecCallbackForStepThreadOnSwapAfterFixedUpdate(); // ここで単にExecCallbackForStepThreadしてしまうと実行中のstepThreadのeveryTimeCallbackListで実行されたSet系メソッドのCallbackを実行してしまう
                    var temp = sceneForBuffer;
                    sceneForBuffer = sceneForGet;
                    sceneForGet = temp;
                    isSwapping = false;
                    Console.WriteLine("SwapAfterFixedUpdate changeAllExecuteGetFunctionFlagsTrue = true");
                    changeAllExecuteGetFunctionFlagsTrue = true;
                }
                isFixedUpdating = false;
            }
        }
        public bool GetExecuteSetFunctionFlag(System.Object o) {
            lock (phSceneForGetSetLock) {
                if (!executeSetFunctionFlagDictionary.ContainsKey(o)) {
                    return executeSetFunctionFlagDictionary[o] = true; // 恐らく初期値trueで問題ない，制御されてないのにStepされても困るし
                } else {
                    if (changeAllExecuteSetFunctionFlagsTrue && firstGetExecuteSetFunctionInOneFixedUpdate) { // FixedUpdate中のGetの一番最初でFlagをtrueにするのはchangeAllExecuteSetFunctionFlagsTrueがSpringheadスレッドで呼ばれるため，ここで呼ぶのが最適
                        List<System.Object> Keys = new List<object>(executeSetFunctionFlagDictionary.Keys);
                        foreach (var key in Keys) {
                            executeSetFunctionFlagDictionary[key] = true;
                        }
                        changeAllExecuteSetFunctionFlagsTrue = false;
                        changedAllExecuteSetFunctionFlagsTrue = true;
                    }
                    firstGetExecuteSetFunctionInOneFixedUpdate = false; // こいつはlock要らないがここしか書けなそう
                    return executeSetFunctionFlagDictionary[o];
                }
            }
        }
        public bool SetExecuteSetFunctionFlagFalse(System.Object o) { // Flagが全てFalseになってからfixedUpdateAutoWaitをSetしないと更新されてないのにStepが実行される
            lock (phSceneForGetSetLock) {
                if (!executeSetFunctionFlagDictionary.ContainsKey(o)) {
                    return false;
                }
                executeSetFunctionFlagDictionary[o] = false;
                return true;
            }
        }

        public bool GetExecuteGetFunctionFlag(System.Object o) {
            lock (phSceneForGetSetLock) {
                if (!executeGetFunctionFlagDictionary.ContainsKey(o)) {
                    return executeGetFunctionFlagDictionary[o] = true; // 恐らく初期値trueで問題ない，制御されてないのにStepされても困るし
                } else {
                    if (changeAllExecuteGetFunctionFlagsTrue && firstGetExecuteGetFunctionInOneFixedUpdate) {
                        List<System.Object> Keys = new List<object>(executeGetFunctionFlagDictionary.Keys);
                        foreach (var key in Keys) {
                            executeGetFunctionFlagDictionary[key] = true;
                        }
                        changeAllExecuteGetFunctionFlagsTrue = false;
                    }
                    firstGetExecuteGetFunctionInOneFixedUpdate = false; // こいつはlock要らないがここしか書けなそう
                    return executeGetFunctionFlagDictionary[o];
                }
            }
        }
        public bool SetExecuteGetFunctionFlagFalse(System.Object o) {
            lock (phSceneForGetSetLock) {
                if (!executeGetFunctionFlagDictionary.ContainsKey(o)) {
                    return false;
                }
                executeGetFunctionFlagDictionary[o] = false;
                return true;
            }
        }
        public override bool DelChildObject(ObjectIf o) {
            ObjectIf objectIf = this as ObjectIf;
            Console.WriteLine("DelChildObject(overrided) "/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            if (multiThreadMode) {
                char[] ret0 = new char[_thisNumber] { (char)1, (char)1, (char)1 };
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        //Console.WriteLine("DelChildObject(overrided) isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
                        AddWaitUntilNextStepCallback(() => {
                            Console.WriteLine("DelChildObject(overrided) In callback"/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
                            var ret1 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForStep], (IntPtr)o._thisArray[sceneForStep]);
                            if (ret1 == 0) {
                                Console.WriteLine("failed DelChildObject ret1");
                            }
                            SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForBuffer], (IntPtr)o._thisArray[sceneForBuffer]); // 全てCallbackで実行しないとstepThreadで実行されるものが参照できなくなる
                            SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForGet], (IntPtr)o._thisArray[sceneForGet]);
                        });
                    } else {
                        //Console.WriteLine("DelChildObject(overrided) not isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
                        for (int num = 0; num < _thisNumber; num++) {
                            ret0[num] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[num], (IntPtr)o._thisArray[num]);
                        }
                    }
                }
                if (ret0[0] == 0 || ret0[1] == 0 || ret0[2] == 0) {
                    Console.WriteLine("failed DelChildObject");
                    return false;
                } else {
                    return true;
                }
            } else {
                var ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
                if (ret0 == 0) {
                    return false;
                } else {
                    return true;
                }
            }
            //return (ret0 == 0||ret1 == 0 || ret2 == 0) ? false : true;
        }
        public override bool GetDesc(CsObject desc) {
            char ret = (char)0; // <!!> これいいのか？
            ObjectIf objectIf = this as ObjectIf;
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (_thisArray[sceneForGet] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり
                                                                  //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
                    } else {
                        //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_this, (IntPtr)desc);
                    }
                }
            } else {
                ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
            }
            return (ret == 0) ? false : true;
        }
        public override void SetDesc(CsObject desc) {
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            ObjectIf objectIf = this as ObjectIf;
            if (multiThreadMode) {
                lock (phSceneForGetSetLock) {
                    if (isStepping) {
                        //Console.WriteLine("SetDesc(override)" + " isStepping " + objectIf.GetIfInfo().ClassName());
                        AddWaitUntilNextStepCallback(() => {
                            //Console.WriteLine("SetDesc(override)" + " isStepping in Callback " + objectIf.GetIfInfo().ClassName());
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
                        });
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForBuffer], (IntPtr)desc);
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
                    } else {
                        //Console.WriteLine("SetDesc(override)" + " not isStepping " + objectIf.GetIfInfo().ClassName());
                        foreach (var _this in _thisArray) {
                            if (_this != IntPtr.Zero) {
                                SprExport.Spr_ObjectIf_SetDesc((IntPtr)_this, (IntPtr)desc);
                            }
                        }
                    }
                }
            } else {
                SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
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
        /*
        public void SetTargetVelocityAsync(Vec6d v) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        var newV = new Vec6d(v);
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newV);
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newV);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newV);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
                        }
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
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newQ);
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newQ);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newQ);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetOrientation((IntPtr)_this, (IntPtr)q);
                        }
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
                        phSceneIf.AddWaitUntilNextStepCallback(() => {
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)newP);
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)newP);
                        });
                        SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)newP);
                    } else {
                        foreach (var _this in _thisArray) {
                            SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)p);
                        }
                    }
                }
            } else {
                SprExport.Spr_PHSpringIf_SetTargetPosition((IntPtr)_this, (IntPtr)p);
            }
        }
        */
    }
    public partial class PHBallJointIf : PHJointIf {
        public PHBallJointIf(IntPtr ptr, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _thisArray[0] = ptr;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
        // Thread処理のためのメソッド
        /*
        public void SetTargetVelocityAsync(Vec3d v) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
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
                SprExport.Spr_PHBallJointIf_SetTargetVelocity((IntPtr)_this, (IntPtr)v);
            }
        }
        public void SetTargetPositionAsync(Quaterniond q) {
            var phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.threadMode) {
                lock (phSceneIf.phSceneForGetSetLock) {
                    if (phSceneIf.isStepping) {
                        //Console.WriteLine("SetTargetPositionAsync isStepping");
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
                SprExport.Spr_PHBallJointIf_SetTargetPosition((IntPtr)_this, (IntPtr)q);
            }
        }
        */
    }
    /*
        public partial class PHSolidIf : PHBodyIf {
            // Thread処理のためのメソッド
            public void SetPoseAsync(Posed p) {
                var phSceneIf = GetCSPHSceneIf();
                if (phSceneIf.threadMode) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        if (phSceneIf.isStepping) {
                            var newP = new Posed(p);
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
                    SprExport.Spr_PHSolidIf_SetPose((IntPtr)_this, (IntPtr)p);
                }
            }
            public void SetShapePoseAsync(int index, Posed p) {
                var phSceneIf = GetCSPHSceneIf();
                if (phSceneIf.threadMode) {
                    lock (phSceneIf.phSceneForGetSetLock) {
                        if (phSceneIf.isStepping) {
                            var newP = new Posed(p);
                            phSceneIf.AddWaitUntilNextStepCallback(() => {
                                SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForStep], (int)index, (IntPtr)newP);
                                SprExport.Spr_PHBodyIf_SetShapePose((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (int)index, (IntPtr)newP);
                            });
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
    */
}
