using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SprCs {
    public partial class PHSdkIf : SdkIf {
        public static readonly object phSdkLock = new object();
        //public override bool DelChildObject(ObjectIf o) { 
        //    // PHSDKから引数CDShapeBehaviourで呼ばれることを想定，PHSdkIfからPHSceneIfを参照し処理を分けることが難しいためisGetFunctionCalledInSubThreadなどのフラグは使っていない
        //    lock (phSdkLock) {
        //        Console.WriteLine("PHSdkIf.DelChildObject");
        //        char ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
        //        if (ret0 == 0) {
        //            Console.WriteLine("failed PHSdkIf.DelChildObject");
        //            return false;
        //        } else {
        //            return true;
        //        }
        //    }
        //}
    }
    public partial class PHSceneIf : SceneIf {
        public bool isStepThreadExecuting = false;
        public bool isGetFunctionCalledInSubThread = false; // subThreadの一実行でGetFunctionが呼ばれた場合，一実行中にSwapが呼ばれないようにする
        public bool isSetFunctionCalledInSubThread = false; // subThreadの一実行でSetFunctionが呼ばれた場合，一実行中に物理エンジンStepが実行されないようにする
        public bool isSwapping = false;
        public bool multiThreadMode = false;
        public readonly object phSceneLock = new object();
        public int sceneForStep = 0;
        public int sceneForGet = 1;
        public int sceneForBuffer = 2;
        public int stepCount = 0; // デバッグ用、subThread側で取得する値が何回目のStepの結果か
        public static Dictionary<IntPtr, PHSceneIf> instances = new Dictionary<IntPtr, PHSceneIf>();
        public readonly AutoResetEvent subThreadWait = new AutoResetEvent(false);
        public readonly ManualResetEvent stepWaitForSetFunction = new ManualResetEvent(true); // stepがSetFunction呼び終わりまで待つ
        //Set系メソッド用(fixedUpdateAutoWaitの実装にも関与)，速度などの更新にはこれが必須，位置の更新などの更新に関してはこの機能を一度も使用しないコードでは位置更新途中でStepが実行されかねない
        public Dictionary<System.Object, bool> executeSetFunctionFlagDictionary = new Dictionary<System.Object, bool>(); // インスタンスごとにステップ済みかのフラグを用意，trueにするのはTimer内，falseにするのは各インスタンス
        public bool changeAllExecuteSetFunctionFlagsTrue = false;
        public bool changedAllExecuteSetFunctionFlagsTrue = true;
        public bool firstGetExecuteSetFunctionInSubThreadOneExecution = true; // FixedUpdateのDefaultExecutionOrderを使わないための苦肉の策
        public bool stopStepThreadExecutionStartForSetFunction = false; // スタートするのを止めるフラグ

        //Get系メソッド用，そのまま実行してもFixedUpdate中に値が変わらないため安全だが，より効率を求める場合に使用(デフォルトの機能用PHSceneの更新など)
        public Dictionary<System.Object, bool> executeGetFunctionFlagDictionary = new Dictionary<System.Object, bool>(); // インスタンスごとにステップ済みかのフラグを用意，trueにするのはTimer内，falseにするのは各インスタンス
        public bool changeAllExecuteGetFunctionFlagsTrue = false;
        public bool firstGetExecuteGetFunctionInSubThreadOneExecution = true;

        public Thread stepThread = null; // Stepを実行するスレッド
        public Thread subThread = null; // 描画用のスレッド(Unity想定)
        // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
        // Thread処理用
        private List<ThreadCallback> callbackForSubThreadList = new List<ThreadCallback>();
        private List<ThreadCallback> callbackForSubThreadForDeleteList = new List<ThreadCallback>(); // subTheadのdelete時にstepThreadで参照するとsceneForGet,sceneForBufferに対して実行できないため，参照してからDelete
        public bool DeletingOnSubThread { // delete中にStepが呼ばれることを防ぐ,callbackForSubThreadForDeleteの個数により判定可能
            get {
                lock (phSceneLock) {
                    if (callbackForSubThreadForDeleteList.Count == 0) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }

        private List<ThreadCallback> callbackForStepThreadToSceneForBufferList = new List<ThreadCallback>();
        private List<ThreadCallback> callbackForStepThreadToSceneForGetList = new List<ThreadCallback>();
        private List<ThreadCallback> callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList = new List<ThreadCallback>();
        public ObjectStatesIf stateForSwap;
        private bool callObjectStatesIf_Create = true;
        public delegate void ThreadCallback();
        // Test用
        public bool debugSceneForGet = false;
        public bool debugSceneForBuffer = false;
        private bool isFirstForDebugSceneForBuffer = true;

        public void ExecCallbackForSubThreadList() {
            if (callbackForSubThreadList.Count != 0) {
                Console.WriteLine("CallbackForSubThread " + callbackForSubThreadList.Count);
            }
            foreach (var callback in callbackForSubThreadList) {
                callback();
            }
            callbackForSubThreadList.Clear();
        }
        public void AddCallbackForSubThread(ThreadCallback callback) {
            callbackForSubThreadList.Add(callback);
        }

        public void ExecCallbackForSubThreadForDeleteList() {
            if (callbackForSubThreadForDeleteList.Count != 0) {
                Console.WriteLine("callbackForSubThreadForDelete " + callbackForSubThreadForDeleteList.Count);
            }
            foreach (var callback in callbackForSubThreadForDeleteList) {
                callback();
            }
            callbackForSubThreadForDeleteList.Clear();
        }
        public void AddCallbackForSubThreadForDelete(ThreadCallback callback) {
            callbackForSubThreadForDeleteList.Add(callback);
        }

        public void ExecCallbackForStepThreadToSceneForBufferList() {
            if (callbackForStepThreadToSceneForBufferList.Count != 0) {
                Console.WriteLine("callbackForStepThreadToSceneForBuffer " + callbackForStepThreadToSceneForBufferList.Count);
            }
            foreach (var callback in callbackForStepThreadToSceneForBufferList) {
                callback();
            }
            callbackForStepThreadToSceneForBufferList.Clear();
        }
        public void ExecCallbackForStepThreadToSceneForGetList() {
            if (callbackForStepThreadToSceneForGetList.Count != 0) {
                Console.WriteLine("callbackForStepThreadToSceneForGet " + callbackForStepThreadToSceneForGetList.Count);
            }
            foreach (var callback in callbackForStepThreadToSceneForGetList) {
                callback();
            }
            callbackForStepThreadToSceneForGetList.Clear();
        }
        public void AddCallbackForStepThread(ThreadCallback callbackToSceneForBuffer, ThreadCallback callbackToSceneForGet) {
            callbackForStepThreadToSceneForBufferList.Add(callbackToSceneForBuffer);
            callbackForStepThreadToSceneForGetList.Add(callbackToSceneForGet);
        }

        public void ExecCallbackForStepThreadOnSwapAfterSubThreadOneExecution() {
            if (callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList.Count != 0) {
                Console.WriteLine("callbackForStepThreadOnSwapAfterSubThreadOneExecution " + callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList.Count);
            }
            foreach (var callback in callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList) {
                callback();
            }
            callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList.Clear();
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
        //public void Step() {
        //    if (multiThreadMode) {
        //        if (isSwapping) {
        //            Console.WriteLine("isSwapping True before Step");
        //        }
        //        lock (PHSdkIf.phSdkLock) { // phSdkのDelChildObjectはStep中に呼ばれないように
        //            if (!debugSceneForGet && !debugSceneForBuffer) {
        //                SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForStep]);
        //                Console.WriteLine("not debug");
        //            } else if (debugSceneForGet) {
        //                SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForGet]);
        //                changeAllExecuteGetFunctionFlagsTrue = true;
        //                stepCount++;
        //                Console.WriteLine("debugSceneForGet");
        //            } else if (debugSceneForBuffer) {
        //                if (isFirstForDebugSceneForBuffer) {
        //                    var temp = sceneForBuffer;
        //                    sceneForBuffer = sceneForGet;
        //                    sceneForGet = temp;
        //                    isFirstForDebugSceneForBuffer = false;
        //                }
        //                SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForGet]);
        //                changeAllExecuteGetFunctionFlagsTrue = true;
        //                stepCount++;
        //                Console.WriteLine("debugSceneForBuffer");
        //            }
        //        }
        //        if (isSwapping) {
        //            Console.WriteLine("isSwapping True after Step");
        //        }
        //    } else {
        //        SprExport.Spr_PHSceneIf_Step(_thisArray[0]);
        //    }
        //}
        public void Swap() {
            while (isSwapping) { // falseになるまで待つ，Step用Callback(ExecWaitUntilNextStepCallbackList)の直後が良いがlock周りを考えるとここでなくてはならない
                Console.WriteLine("Wait isSwapping");
                Thread.Sleep(1); // 通常FixedUpdateが20millisecondsで呼ばれていることを考えるとSleepは短くて良い(調べところちょうど良さそう)
            }
            lock (phSceneLock) {
                if (isSwapping) {
                    Console.WriteLine("isSwapping True in Swap");
                }
                ExecCallbackForSubThreadList();
                if (callObjectStatesIf_Create) {
                    stateForSwap = ObjectStatesIf.Create();
                    callObjectStatesIf_Create = false;
                }
                if (!isGetFunctionCalledInSubThread) { // Step↔Get
                    ExecCallbackForStepThreadToSceneForBufferList(); // SetState系メソッドはStateを変更するためSave/LoadStateより前で実行(後や中間で実行するとSceneごとに値が変化する)
                    ExecCallbackForStepThreadToSceneForGetList();
                    SprExport.Spr_ObjectStatesIf_SaveState(stateForSwap._thisArray[0], _thisArray[sceneForStep]); // phScene→state
                    SprExport.Spr_ObjectStatesIf_LoadState(stateForSwap._thisArray[0], _thisArray[sceneForGet]); // state→phScene
                    Console.WriteLine("ExecCallbackForSubThreadForDeleteList In Swap");
                    ExecCallbackForSubThreadForDeleteList(); // LoadStateの前で実行するとstateForSwapとphSceneの状態が変わってしまう
                    var temp = sceneForStep;
                    sceneForStep = sceneForGet;
                    sceneForGet = temp;
                    stepCount++;
                    Console.WriteLine("Swap Step↔Get changeAllExecuteGetFunctionFlagsTrue = true");
                    changeAllExecuteGetFunctionFlagsTrue = true;
                } else { // Step↔Buffer
                    Console.WriteLine("Swap Step↔Buffer");
                    isSwapping = true;
                    ExecCallbackForStepThreadToSceneForBufferList(); // 現在のBufferはStepとなるため，ここで実行しなければならない
                    callbackForStepThreadToSceneForGetOnSwapAfterSubThreadOneExecutionList = new List<ThreadCallback>(callbackForStepThreadToSceneForGetList); // ディープコピー
                    callbackForStepThreadToSceneForGetList.Clear();
                    SprExport.Spr_ObjectStatesIf_SaveState(stateForSwap._thisArray[0], _thisArray[sceneForStep]); // phScene→state
                    SprExport.Spr_ObjectStatesIf_LoadState(stateForSwap._thisArray[0], _thisArray[sceneForBuffer]); // state→phScene
                    var temp = sceneForStep;
                    sceneForStep = sceneForBuffer;
                    sceneForBuffer = temp;
                }
                isStepThreadExecuting = false;

            }
        }
        public void SwapAfterSubThreadOneExecution() {
            lock (PHSdkIf.phSdkLock) { // <!!> stepThreadでphSceneLockをしているとphSdkLock→phSceneLockの順番でロックを掛けるとデッドロックになる可能性あり
                lock (phSceneLock) {
                    if (isSwapping) { // Buffer↔Get
                        Console.WriteLine("SwapAfterSubThreadOneExecution called");
                        if (isStepThreadExecuting) {
                            Console.WriteLine("isStepping True");
                        }
                        ExecCallbackForStepThreadOnSwapAfterSubThreadOneExecution(); // ここで単にExecCallbackForStepThreadしてしまうと実行中のstepThreadのeveryTimeCallbackListで実行されたSet系メソッドのCallbackを実行してしまう
                        Console.WriteLine("ExecCallbackForSubThreadForDeleteList In SwapAfterSubThreadOneExecution");
                        if (!isStepThreadExecuting) { // たらい回し作戦
                            ExecCallbackForSubThreadForDeleteList();// subThreadで実行されるためStep中にsceneForStepに参照してはならないがSetのようにcallbackForSubThreadに登録してもsceneForGetにアクセスすることになるため、ここで実行
                        }
                        Console.WriteLine("End ExecCallbackForSubThreadForDeleteList In SwapAfterSubThreadOneExecution");
                        var temp = sceneForBuffer;
                        sceneForBuffer = sceneForGet;
                        sceneForGet = temp;
                        stepCount++;
                        changeAllExecuteGetFunctionFlagsTrue = true;
                        isSwapping = false;
                    }
                    isGetFunctionCalledInSubThread = false;
                }
            }
        }
        public bool GetExecuteSetFunctionFlag(System.Object o) {
            if (!multiThreadMode) {
                return true;
            }
            lock (phSceneLock) {
                if (!executeSetFunctionFlagDictionary.ContainsKey(o)) {
                    return executeSetFunctionFlagDictionary[o] = true; // 恐らく初期値trueで問題ない，制御されてないのにStepされても困るし
                } else {
                    if (changeAllExecuteSetFunctionFlagsTrue && firstGetExecuteSetFunctionInSubThreadOneExecution) { // FixedUpdate中のGetの一番最初でFlagをtrueにするのはchangeAllExecuteSetFunctionFlagsTrueがSpringheadスレッドで呼ばれるため，ここで呼ぶのが最適
                        List<System.Object> Keys = new List<object>(executeSetFunctionFlagDictionary.Keys);
                        foreach (var key in Keys) {
                            executeSetFunctionFlagDictionary[key] = true;
                        }
                        changeAllExecuteSetFunctionFlagsTrue = false;
                        changedAllExecuteSetFunctionFlagsTrue = true;
                    }
                    firstGetExecuteSetFunctionInSubThreadOneExecution = false; // こいつはlock要らないがここしか書けなそう
                    return executeSetFunctionFlagDictionary[o];
                }
            }
        }
        public bool SetExecuteSetFunctionFlagFalse(System.Object o) { // Flagが全てFalseになってからfixedUpdateAutoWaitをSetしないと更新されてないのにStepが実行される
            lock (phSceneLock) {
                if (!executeSetFunctionFlagDictionary.ContainsKey(o)) {
                    return false;
                }
                executeSetFunctionFlagDictionary[o] = false;
                return true;
            }
        }

        public bool GetExecuteGetFunctionFlag(System.Object o) {
            if (!multiThreadMode) {
                return true;
            }
            lock (phSceneLock) {
                if (!executeGetFunctionFlagDictionary.ContainsKey(o)) {
                    return executeGetFunctionFlagDictionary[o] = true; // 恐らく初期値trueで問題ない，制御されてないのにStepされても困るし
                } else {
                    if (changeAllExecuteGetFunctionFlagsTrue && firstGetExecuteGetFunctionInSubThreadOneExecution) {
                        List<System.Object> Keys = new List<object>(executeGetFunctionFlagDictionary.Keys);
                        foreach (var key in Keys) {
                            executeGetFunctionFlagDictionary[key] = true;
                        }
                        changeAllExecuteGetFunctionFlagsTrue = false;
                    }
                    firstGetExecuteGetFunctionInSubThreadOneExecution = false; // こいつはlock要らないがここしか書けなそう
                    return executeGetFunctionFlagDictionary[o];
                }
            }
        }
        public bool SetExecuteGetFunctionFlagFalse(System.Object o) {
            lock (phSceneLock) {
                if (!executeGetFunctionFlagDictionary.ContainsKey(o)) {
                    return false;
                }
                executeGetFunctionFlagDictionary[o] = false;
                return true;
            }
        }
        //public override bool DelChildObject(ObjectIf o) {
        //    ObjectIf objectIf = this as ObjectIf;
        //    Console.WriteLine("PHSceneIf.DelChildObject "/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
        //    // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
        //    if (multiThreadMode) {
        //        char[] ret0 = new char[_thisNumber] { (char)1, (char)1, (char)1 };
        //        lock (phSceneLock) {
        //            isSetFunctionCalledInSubThread = true;
        //            if (isStepThreadExecuting || isSwapping) {
        //                //Console.WriteLine("DelChildObject(overrided) isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
        //                AddCallbackForSubThreadForDelete(() => {
        //                    Console.WriteLine("PHSceneIf.DelChildObject In callback"/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
        //                    var ret1 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForStep], (IntPtr)o._thisArray[sceneForStep]);
        //                    if (ret1 == 0) {
        //                        Console.WriteLine("failed PHSceneIf.DelChildObject ret1");
        //                    }
        //                    SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForBuffer], (IntPtr)o._thisArray[sceneForBuffer]); // 全てCallbackで実行しないとstepThreadで実行されるものが参照できなくなる
        //                    SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForGet], (IntPtr)o._thisArray[sceneForGet]);
        //                    callObjectStatesIf_Create = true;
        //                });
        //            } else {
        //                Console.WriteLine("PHSceneIf.DelChildObject not isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
        //                for (int num = 0; num < _thisNumber; num++) {
        //                    ret0[num] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[num], (IntPtr)o._thisArray[num]);
        //                }
        //                callObjectStatesIf_Create = true;
        //            }
        //        }
        //        if (ret0[0] == 0 || ret0[1] == 0 || ret0[2] == 0) {
        //            Console.WriteLine("failed PHSceneIf.DelChildObject");
        //            return false;
        //        } else {
        //            return true;
        //        }
        //    } else {
        //        Console.WriteLine("PHSceneIf.DelChildObject not multi");
        //        var ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
        //        if (ret0 == 0) {
        //            return false;
        //        } else {
        //            return true;
        //        }
        //    }
        //    //return (ret0 == 0||ret1 == 0 || ret2 == 0) ? false : true;
        //}
        //public override bool GetDesc(CsObject desc) {
        //    char ret = (char)0; // <!!> これいいのか？
        //    ObjectIf objectIf = this as ObjectIf;
        //    if (multiThreadMode) {
        //        var currentThread = Thread.CurrentThread;
        //        if (currentThread == stepThread) {
        //            if (_thisArray[sceneForStep] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり,PHIKEnginが呼んでるな
        //                                                                     //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
        //                ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
        //            } else {
        //                //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
        //                ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
        //            }
        //        } else if (currentThread == subThread) {
        //            lock (phSceneLock) {
        //                if (_thisArray[sceneForGet] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり
        //                                                                        //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
        //                    ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
        //                } else {
        //                    //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
        //                    ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
        //                }
        //            }
        //        }
        //    } else {
        //        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
        //    }
        //    return (ret == 0) ? false : true;
        //}
        //public override void SetDesc(CsObject desc) {
        //    // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
        //    ObjectIf objectIf = this as ObjectIf;
        //    if (multiThreadMode) {
        //        lock (phSceneLock) {
        //            isSetFunctionCalledInSubThread = true;
        //            if (isStepThreadExecuting) {
        //                Console.WriteLine("SetDesc(override)" + " isStepping " + objectIf.GetIfInfo().ClassName());
        //                AddCallbackForSubThread(() => {
        //                    //Console.WriteLine("SetDesc(override)" + " isStepping in Callback " + objectIf.GetIfInfo().ClassName());
        //                    SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
        //                });
        //                SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForBuffer], (IntPtr)desc);
        //                SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
        //            } else {
        //                Console.WriteLine("SetDesc(override)" + " not isStepping " + objectIf.GetIfInfo().ClassName());
        //                foreach (var _this in _thisArray) {
        //                    if (_this != IntPtr.Zero) {
        //                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_this, (IntPtr)desc);
        //                    }
        //                }
        //            }
        //        }
        //    } else {
        //        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
        //    }
        //}
    }

    public partial class PHSpringIf : PHJointIf {
        public PHSpringIf(IntPtr ptr, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _thisArray[0] = ptr;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
    }
    public partial class PHBallJointIf : PHJointIf {
        public PHBallJointIf(IntPtr ptr, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _thisArray[0] = ptr;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
    }
}
