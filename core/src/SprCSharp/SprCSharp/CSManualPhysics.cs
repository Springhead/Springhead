using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SprCs {
    public partial class PHSdkIf : SdkIf {
        public static readonly object phSdkLock = new object();
        public static PHSdkIf phSdkIf = null;
        public static PHSdkIf CreateSdk(PHSdkDesc desc) {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateSdk((IntPtr)desc);
            if (ptr == IntPtr.Zero) { return null; }
            PHSdkIf obj = new PHSdkIf(ptr, 0);
            if (phSdkIf != null) {
                throw new NotSupportedException();
            }
            phSdkIf = obj;
            return obj;
        }
        public static PHSdkIf CreateSdk() {
            IntPtr ptr = SprExport.Spr_PHSdkIf_CreateSdk_1();
            if (ptr == IntPtr.Zero) { return null; }
            PHSdkIf obj = new PHSdkIf(ptr);
            if (phSdkIf != null) {
                throw new NotSupportedException();
            }
            phSdkIf = obj;
            return obj;
        }

        public PHSceneIf CreateScene(PHSceneDesc desc, bool multiThreadMode = false) {
            if (multiThreadMode) {
                IntPtr ptr1 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_thisArray[0], (IntPtr)desc);
                IntPtr ptr2 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_thisArray[0], (IntPtr)desc);
                IntPtr ptr3 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_thisArray[0], (IntPtr)desc);
                if (ptr1 == IntPtr.Zero || ptr2 == IntPtr.Zero || ptr3 == IntPtr.Zero) {
                    Console.WriteLine("Create Scene null");
                    return null;
                }
                PHSceneIf obj = PHSceneIf.CreateCSInstance(ptr1, ptr2, ptr3);
                return obj;
            } else {
                IntPtr ptr1 = SprExport.Spr_PHSdkIf_CreateScene((IntPtr)_thisArray[0], (IntPtr)desc);
                if (ptr1 == IntPtr.Zero) {
                    Console.WriteLine("Create Scene null");
                    return null;
                }
                PHSceneIf obj = PHSceneIf.CreateCSInstance(ptr1);
                return obj;
            }
        }

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

    public partial class PHContactPointIf : PHConstraintIf {
        public override PHSolidIf GetSocketSolid() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    IntPtr ptrStep = SprExport.Spr_PHConstraintIf_GetSocketSolid((IntPtr)_thisArray[0]);
                    if (ptrStep == IntPtr.Zero) { return null; }
                    PHSolidIf obj = new PHSolidIf(ptrStep, phSceneIf.sceneForStep);
                    if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                        PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    return obj;
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneLock) {
                        phSceneIf.isGetFunctionCalledInSubThread = true;
                        IntPtr ptrGet = SprExport.Spr_PHConstraintIf_GetSocketSolid((IntPtr)_thisArray[0]);
                        if (ptrGet == IntPtr.Zero) { return null; }
                        PHSolidIf obj = new PHSolidIf(ptrGet, phSceneIf.sceneForGet);
                        if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                            PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                            return appropriate_type;
                        }
                        return obj;
                    }
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHConstraintIf_GetSocketSolid((IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHSolidIf obj = new PHSolidIf(ptr, 0);
                if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                    PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                return obj;
            }
            throw new InvalidOperationException();
        }
        public virtual PHSolidIf GetPlugSolid() {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    IntPtr ptrStep = SprExport.Spr_PHConstraintIf_GetPlugSolid((IntPtr)_thisArray[0]);
                    if (ptrStep == IntPtr.Zero) { return null; }
                    PHSolidIf obj = new PHSolidIf(ptrStep, phSceneIf.sceneForStep);
                    if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                        PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    return obj;
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneLock) {
                        phSceneIf.isGetFunctionCalledInSubThread = true;
                        IntPtr ptrGet = SprExport.Spr_PHConstraintIf_GetPlugSolid((IntPtr)_thisArray[0]);
                        if (ptrGet == IntPtr.Zero) { return null; }
                        PHSolidIf obj = new PHSolidIf(ptrGet, phSceneIf.sceneForGet);
                        if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                            PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                            return appropriate_type;
                        }
                        return obj;
                    }
                }
            } else {
                IntPtr ptr = SprExport.Spr_PHConstraintIf_GetPlugSolid((IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHSolidIf obj = new PHSolidIf(ptr, 0);
                if (obj.GetIfInfo() == PHHapticPointerIf.GetIfInfoStatic()) {
                    PHHapticPointerIf appropriate_type = new PHHapticPointerIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                return obj;
            }
            throw new InvalidOperationException();
        }
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
        public bool callObjectStatesIf_Create = true;
        public delegate void ThreadCallback();
        // Test用
        public bool debugSceneForGet = false;
        public bool debugSceneForBuffer = false;
        private bool isFirstForDebugSceneForBuffer = true;

        public PHSdkIf GetSdk() {
            return PHSdkIf.phSdkIf;
        }

        public PHSceneIf GetCSPHSceneIf() { // 自動生成コードをSceneObjectIfと同じにするため
            return this;
        }
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
        public void Step() {
            if (multiThreadMode) {
                if (isSwapping) {
                    Console.WriteLine("isSwapping True before Step");
                }
                lock (PHSdkIf.phSdkLock) { // phSdkのDelChildObjectはStep中に呼ばれないように
                    if (!debugSceneForGet && !debugSceneForBuffer) {
                        SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForStep]);
                        Console.WriteLine("not debug");
                    } else if (debugSceneForGet) {
                        SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForGet]);
                        changeAllExecuteGetFunctionFlagsTrue = true;
                        stepCount++;
                        Console.WriteLine("debugSceneForGet");
                    } else if (debugSceneForBuffer) {
                        if (isFirstForDebugSceneForBuffer) {
                            var temp = sceneForBuffer;
                            sceneForBuffer = sceneForGet;
                            sceneForGet = temp;
                            isFirstForDebugSceneForBuffer = false;
                        }
                        SprExport.Spr_PHSceneIf_Step(_thisArray[sceneForGet]);
                        changeAllExecuteGetFunctionFlagsTrue = true;
                        stepCount++;
                        Console.WriteLine("debugSceneForBuffer");
                    }
                }
                if (isSwapping) {
                    Console.WriteLine("isSwapping True after Step");
                }
            } else {
                SprExport.Spr_PHSceneIf_Step(_thisArray[0]);
            }
        }
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
        public override bool DelChildObject(ObjectIf o) {
            ObjectIf objectIf = this as ObjectIf;
            Console.WriteLine("PHSceneIf.DelChildObject "/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            if (multiThreadMode) {
                char[] ret0 = new char[_thisNumber] { (char)1, (char)1, (char)1 };
                lock (phSceneLock) {
                    isSetFunctionCalledInSubThread = true;
                    if (isStepThreadExecuting || isSwapping) {
                        //Console.WriteLine("DelChildObject(overrided) isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
                        AddCallbackForSubThreadForDelete(() => {
                            Console.WriteLine("PHSceneIf.DelChildObject In callback"/* + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()*/); // <!!> GravityEngineはC++内部で実装されてる？
                            var ret1 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForStep], (IntPtr)o._thisArray[sceneForStep]);
                            if (ret1 == 0) {
                                Console.WriteLine("failed PHSceneIf.DelChildObject ret1");
                            }
                            SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForBuffer], (IntPtr)o._thisArray[sceneForBuffer]); // 全てCallbackで実行しないとstepThreadで実行されるものが参照できなくなる
                            SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[sceneForGet], (IntPtr)o._thisArray[sceneForGet]);
                            callObjectStatesIf_Create = true;
                        });
                    } else {
                        Console.WriteLine("PHSceneIf.DelChildObject not isStepping " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
                        for (int num = 0; num < _thisNumber; num++) {
                            ret0[num] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[num], (IntPtr)o._thisArray[num]);
                        }
                        callObjectStatesIf_Create = true;
                    }
                }
                if (ret0[0] == 0 || ret0[1] == 0 || ret0[2] == 0) {
                    Console.WriteLine("failed PHSceneIf.DelChildObject");
                    return false;
                } else {
                    return true;
                }
            } else {
                Console.WriteLine("PHSceneIf.DelChildObject not multi");
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
                var currentThread = Thread.CurrentThread;
                if (currentThread == stepThread) {
                    if (_thisArray[sceneForStep] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり,PHIKEnginが呼んでるな
                                                                   //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
                    } else {
                        //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
                    }
                } else if (currentThread == subThread) {
                    lock (phSceneLock) {
                        if (_thisArray[sceneForGet] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり
                                                                      //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                            ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
                        } else {
                            //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                            ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
                        }
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
                lock (phSceneLock) {
                    isSetFunctionCalledInSubThread = true;
                    if (isStepThreadExecuting) {
                        Console.WriteLine("SetDesc(override)" + " isStepping " + objectIf.GetIfInfo().ClassName());
                        AddCallbackForSubThread(() => {
                            //Console.WriteLine("SetDesc(override)" + " isStepping in Callback " + objectIf.GetIfInfo().ClassName());
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForStep], (IntPtr)desc);
                        });
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForBuffer], (IntPtr)desc);
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[sceneForGet], (IntPtr)desc);
                    } else {
                        Console.WriteLine("SetDesc(override)" + " not isStepping " + objectIf.GetIfInfo().ClassName());
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
        public override void SetName(string n) {
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

        public PHIKEngineIf GetIKEngine() {
            if (multiThreadMode) {
                IntPtr ptr = SprExport.Spr_PHSceneIf_GetIKEngine((IntPtr)_thisArray[0]);
                IntPtr ptr1 = SprExport.Spr_PHSceneIf_GetIKEngine((IntPtr)_thisArray[1]);
                IntPtr ptr2 = SprExport.Spr_PHSceneIf_GetIKEngine((IntPtr)_thisArray[2]);
                if (ptr == IntPtr.Zero) { return null; }
                PHIKEngineIf obj = new PHIKEngineIf(ptr);
                obj._thisArray[0] = ptr;
                obj._thisArray[1] = ptr1;
                obj._thisArray[2] = ptr2;
                return obj;
            } else {
                IntPtr ptr = SprExport.Spr_PHSceneIf_GetIKEngine((IntPtr)_thisArray[0]);
                if (ptr == IntPtr.Zero) { return null; }
                PHIKEngineIf obj = new PHIKEngineIf(ptr);
                obj._thisArray[0] = ptr;
                return obj;
            }
        }
                public PHJointIf CreateJoint(PHSolidIf lhs, PHSolidIf rhs, IfInfo ii, PHJointDesc desc) {
            if (multiThreadMode) {
                PHJointIf obj = new PHJointIf(IntPtr.Zero);
                lock (phSceneLock) {
                    isSetFunctionCalledInSubThread = true;
                    if (stateForSwap != null) { // Createをメモリリークさせないために，一番最初のCreateをしないため，一番最初だけnullになる
                        SprExport.Spr_ObjectStatesIf_ReleaseState(stateForSwap._thisArray[0], _thisArray[sceneForGet]); // 恐らくstateForSwap._thisと_thisArray[sceneForGet]が同じ構造でないといけないのでここで
                    }
                    callObjectStatesIf_Create = true;
                    if (isStepThreadExecuting) {
                        IntPtr get = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForGet], (IntPtr)lhs._thisArray[sceneForGet], (IntPtr)rhs._thisArray[sceneForGet], (IntPtr)ii, (IntPtr)desc);
                        IntPtr buffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForBuffer], (IntPtr)lhs._thisArray[sceneForBuffer], (IntPtr)rhs._thisArray[sceneForBuffer], (IntPtr)ii, (IntPtr)desc);
                        if (get == IntPtr.Zero || buffer == IntPtr.Zero) {
                            return null;
                        }
                        obj._thisArray[sceneForGet] = get;
                        obj._thisArray[sceneForBuffer] = buffer;
                        Console.WriteLine(sceneForGet);
                        obj._thisArray[0] = obj._thisArray[sceneForGet];
                    } else {
                        IntPtr ptr0 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[0], (IntPtr)lhs._thisArray[0], (IntPtr)rhs._thisArray[0], (IntPtr)ii, (IntPtr)desc);
                        IntPtr ptr1 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[1], (IntPtr)lhs._thisArray[1], (IntPtr)rhs._thisArray[1], (IntPtr)ii, (IntPtr)desc);
                        IntPtr ptr2 = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[2], (IntPtr)lhs._thisArray[2], (IntPtr)rhs._thisArray[2], (IntPtr)ii, (IntPtr)desc);
                        if (ptr0 == IntPtr.Zero || ptr1 == IntPtr.Zero || ptr2 == IntPtr.Zero) {
                            return null;
                        }
                        obj._thisArray[0] = ptr0;
                        obj._thisArray[1] = ptr1;
                        obj._thisArray[2] = ptr2;
                        obj._thisArray[0] = obj._thisArray[0];
                    }

                    if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) { return new PH1DJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                        var newJoint = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        newJoint.phSceneIf = this;
                        if (isStepThreadExecuting) {
                            AddCallbackForSubThread(() => { // newJointがここにしかないため
                                IntPtr step = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep], (IntPtr)ii, (IntPtr)desc);
                                newJoint._thisArray[sceneForStep] = step;
                            });
                        }
                        return newJoint;
                    }
                    if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) { return new PHFixJointIf(IntPtr.Zero); }
                    if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                        var newJoint = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        newJoint.phSceneIf = this;
                        if (isStepThreadExecuting) {
                            AddCallbackForSubThread(() => {
                                IntPtr step = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[sceneForStep], (IntPtr)lhs._thisArray[sceneForStep], (IntPtr)rhs._thisArray[sceneForStep], (IntPtr)ii, (IntPtr)desc);
                                newJoint._thisArray[sceneForStep] = step;
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
            } else { // コンストラクタの実装によって修正が必要
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

/*
        public PHJointIf CreateJoint(PHSolidIf lhs, PHSolidIf rhs, IfInfo ii, PHJointDesc desc) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            if (phSceneIf.multiThreadMode) {
                ;
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    PHSolidIf new_lhs = new PHSolidIf(lhs);
                    // intrinsic
                    PHSolidIf new_rhs = new PHSolidIf(rhs);
                    // intrinsic
                    IfInfo new_ii = new IfInfo(ii);
                    // intrinsic
                    PHJointDesc new_desc = new PHJointDesc(desc);
                    // intrinsic
                    //_[function_prep: 1586] 
                    //_[function_body: 1617] 
                    IntPtr ptrStep = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)new_lhs, (IntPtr)new_rhs, (IntPtr)new_ii, (IntPtr)new_desc);
                    phSceneIf.AddCallbackForStepThread(
                        () => {
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrBuffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)new_lhs, (IntPtr)new_rhs, (IntPtr)new_ii, (IntPtr)new_desc);
                        },
                        () => {
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrGet = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)new_lhs, (IntPtr)new_rhs, (IntPtr)new_ii, (IntPtr)new_desc);
                        });
                    if (ptrStep == IntPtr.Zero) { return null; }
                    PHJointIf obj = new PHJointIf(ptrStep, phSceneIf.sceneForStep);
                    if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) {
                        PH1DJointIf appropriate_type = new PH1DJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                        PHBallJointIf appropriate_type = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) {
                        PHFixJointIf appropriate_type = new PHFixJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                        PHSpringIf appropriate_type = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) {
                        PHMateIf appropriate_type = new PHMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) {
                        PHHingeJointIf appropriate_type = new PHHingeJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) {
                        PHSliderJointIf appropriate_type = new PHSliderJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) {
                        PHPathJointIf appropriate_type = new PHPathJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) {
                        PHGenericJointIf appropriate_type = new PHGenericJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) {
                        PHPointToPointMateIf appropriate_type = new PHPointToPointMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) {
                        PHPointToLineMateIf appropriate_type = new PHPointToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) {
                        PHPointToPlaneMateIf appropriate_type = new PHPointToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) {
                        PHLineToLineMateIf appropriate_type = new PHLineToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) {
                        PHPlaneToPlaneMateIf appropriate_type = new PHPlaneToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                        return appropriate_type;
                    }
                    return obj;
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneLock) {
                        phSceneIf.isSetFunctionCalledInSubThread = true;
                        if (phSceneIf.stateForSwap != null) {
                            SprExport.Spr_ObjectStatesIf_ReleaseState(phSceneIf.stateForSwap._thisArray[0], _thisArray[phSceneIf.sceneForGet]);
                        }
                        phSceneIf.callObjectStatesIf_Create = true;
                        if (phSceneIf.isStepThreadExecuting) {
                            PHSolidIf new_lhs = lhs;
                            // intrinsic
                            PHSolidIf new_rhs = rhs;
                            // intrinsic
                            IfInfo new_ii = new IfInfo(ii);
                            // intrinsic
                            PHJointDesc new_desc = new PHJointDesc(desc);
                            // intrinsic
                            phSceneIf.AddCallbackForSubThread(() => {
                                //_[function_prep: 1586] 
                                //_[function_body: 1617] 
                                IntPtr ptrStep = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)new_lhs._thisArray[phSceneIf.sceneForStep], (IntPtr)new_rhs._thisArray[phSceneIf.sceneForStep], (IntPtr)new_ii, (IntPtr)new_desc);
                            });
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrBuffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)new_lhs._thisArray[phSceneIf.sceneForBuffer], (IntPtr)new_rhs._thisArray[phSceneIf.sceneForBuffer], (IntPtr)new_ii, (IntPtr)new_desc);
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrGet = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)new_lhs._thisArray[phSceneIf.sceneForGet], (IntPtr)new_rhs._thisArray[phSceneIf.sceneForGet], (IntPtr)new_ii, (IntPtr)new_desc);
                            if (ptrGet == IntPtr.Zero) { return null; }
                            PHJointIf obj = new PHJointIf(ptrGet, phSceneIf.sceneForGet);
                            if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) {
                                PH1DJointIf appropriate_type = new PH1DJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                                PHBallJointIf appropriate_type = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) {
                                PHFixJointIf appropriate_type = new PHFixJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                                PHSpringIf appropriate_type = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) {
                                PHMateIf appropriate_type = new PHMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) {
                                PHHingeJointIf appropriate_type = new PHHingeJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) {
                                PHSliderJointIf appropriate_type = new PHSliderJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) {
                                PHPathJointIf appropriate_type = new PHPathJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) {
                                PHGenericJointIf appropriate_type = new PHGenericJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) {
                                PHPointToPointMateIf appropriate_type = new PHPointToPointMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) {
                                PHPointToLineMateIf appropriate_type = new PHPointToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) {
                                PHPointToPlaneMateIf appropriate_type = new PHPointToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) {
                                PHLineToLineMateIf appropriate_type = new PHLineToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) {
                                PHPlaneToPlaneMateIf appropriate_type = new PHPlaneToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            return obj;
                        } else {
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrStep = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)lhs._thisArray[phSceneIf.sceneForStep], (IntPtr)rhs._thisArray[phSceneIf.sceneForStep], (IntPtr)ii, (IntPtr)desc);
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrBuffer = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)lhs._thisArray[phSceneIf.sceneForBuffer], (IntPtr)rhs._thisArray[phSceneIf.sceneForBuffer], (IntPtr)ii, (IntPtr)desc);
                            //_[function_prep: 1586] 
                            //_[function_body: 1617] 
                            IntPtr ptrGet = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)lhs._thisArray[phSceneIf.sceneForGet], (IntPtr)rhs._thisArray[phSceneIf.sceneForGet], (IntPtr)ii, (IntPtr)desc);
                            if (ptrGet == IntPtr.Zero) { return null; }
                            PHJointIf obj = new PHJointIf(ptrStep, ptrBuffer, ptrGet, phSceneIf.sceneForStep, phSceneIf.sceneForBuffer, phSceneIf.sceneForGet);
                            if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) {
                                PH1DJointIf appropriate_type = new PH1DJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                                PHBallJointIf appropriate_type = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) {
                                PHFixJointIf appropriate_type = new PHFixJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                                PHSpringIf appropriate_type = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) {
                                PHMateIf appropriate_type = new PHMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) {
                                PHHingeJointIf appropriate_type = new PHHingeJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) {
                                PHSliderJointIf appropriate_type = new PHSliderJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) {
                                PHPathJointIf appropriate_type = new PHPathJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) {
                                PHGenericJointIf appropriate_type = new PHGenericJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) {
                                PHPointToPointMateIf appropriate_type = new PHPointToPointMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) {
                                PHPointToLineMateIf appropriate_type = new PHPointToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) {
                                PHPointToPlaneMateIf appropriate_type = new PHPointToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) {
                                PHLineToLineMateIf appropriate_type = new PHLineToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) {
                                PHPlaneToPlaneMateIf appropriate_type = new PHPlaneToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                                return appropriate_type;
                            }
                            return obj;
                        }
                    }
                }
            } else {
                //_[function_prep: 1586] 
                //_[function_body: 1617] 
                IntPtr ptr = SprExport.Spr_PHSceneIf_CreateJoint((IntPtr)_thisArray[0], (IntPtr)lhs, (IntPtr)rhs, (IntPtr)ii, (IntPtr)desc);
                if (ptr == IntPtr.Zero) { return null; }
                PHJointIf obj = new PHJointIf(ptr, 0);
                if (obj.GetIfInfo() == PH1DJointIf.GetIfInfoStatic()) {
                    PH1DJointIf appropriate_type = new PH1DJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHBallJointIf.GetIfInfoStatic()) {
                    PHBallJointIf appropriate_type = new PHBallJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHFixJointIf.GetIfInfoStatic()) {
                    PHFixJointIf appropriate_type = new PHFixJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHSpringIf.GetIfInfoStatic()) {
                    PHSpringIf appropriate_type = new PHSpringIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHMateIf.GetIfInfoStatic()) {
                    PHMateIf appropriate_type = new PHMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHHingeJointIf.GetIfInfoStatic()) {
                    PHHingeJointIf appropriate_type = new PHHingeJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHSliderJointIf.GetIfInfoStatic()) {
                    PHSliderJointIf appropriate_type = new PHSliderJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHPathJointIf.GetIfInfoStatic()) {
                    PHPathJointIf appropriate_type = new PHPathJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHGenericJointIf.GetIfInfoStatic()) {
                    PHGenericJointIf appropriate_type = new PHGenericJointIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHPointToPointMateIf.GetIfInfoStatic()) {
                    PHPointToPointMateIf appropriate_type = new PHPointToPointMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHPointToLineMateIf.GetIfInfoStatic()) {
                    PHPointToLineMateIf appropriate_type = new PHPointToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHPointToPlaneMateIf.GetIfInfoStatic()) {
                    PHPointToPlaneMateIf appropriate_type = new PHPointToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHLineToLineMateIf.GetIfInfoStatic()) {
                    PHLineToLineMateIf appropriate_type = new PHLineToLineMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                if (obj.GetIfInfo() == PHPlaneToPlaneMateIf.GetIfInfoStatic()) {
                    PHPlaneToPlaneMateIf appropriate_type = new PHPlaneToPlaneMateIf(obj._thisArray[0], obj._thisArray[1], obj._thisArray[2]);
                    return appropriate_type;
                }
                return obj;
            }
            throw new InvalidOperationException();
        }
        */

    }
}
