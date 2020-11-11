using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SprCs {
    public partial class Vec3d : CsObject {
        public Vec3d(Vec3d v) { _this = SprExport.Spr_new_Vec3d_4(v.x, v.y, v.z); _flag = true; }
    }
    public partial class Vec6d : CsObject {
        public Vec6d(Vec6d v) { _this = SprExport.Spr_new_Vec6d_7(v.vx, v.vy, v.vz, v.wx, v.wy, v.wz); _flag = true; }
    }
    public partial class Quaterniond : CsObject {
        public Quaterniond(Quaterniond q) { _this = SprExport.Spr_new_Quaterniond_5(q.w, q.x, q.y, q.z); _flag = true; }
    }
    public partial class Posed : CsObject {
        public Posed(Posed p) { _this = SprExport.Spr_new_Posed_9(p.w, p.x, p.y, p.z, p.px, p.py, p.pz); _flag = true; }
    }
    public partial class ObjectIf : CsObject {
        public IntPtr _this2;
    }

    public partial class SceneObjectIf : NamedObjectIf {
        public PHSceneIf phSceneIf = null;
        public PHSceneIf GetCSPHSceneIf() { // DelChildObject対策のため，phSceneIfは保存されるように
            if (phSceneIf != null) {
                return phSceneIf;
            }
            ObjectIf objectIf = this as ObjectIf;
            //Console.WriteLine("SceneObjectIf.GetCSPHScenIf phSceneIf null");
            //Console.WriteLine(objectIf.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
            IntPtr ptr = IntPtr.Zero;
            if (_thisArray[0] != IntPtr.Zero) {
                //Console.WriteLine("_thisArray[0] not Zero");
                ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_thisArray[0]);
            } else if (_thisArray[1] != IntPtr.Zero) {
                //Console.WriteLine("_thisArray[1] not Zero");
                ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_thisArray[1]);
            } else if (_thisArray[2] != IntPtr.Zero) {
                //Console.WriteLine("_thisArray[2] not Zero");
                ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr)_thisArray[2]);
            }
            if (ptr == IntPtr.Zero) {
                Console.WriteLine("GetCSPHSceneIf null");
                return null;
            }
            return phSceneIf = PHSceneIf.GetCSInstance(ptr);
        }

        public override bool DelChildObject(ObjectIf o) {
            ObjectIf objectIf = this as ObjectIf;
            if (objectIf.GetIfInfo() == null) {
                Console.WriteLine("objectIf.GetIfInfo() null");
            } else {
                Console.WriteLine("SceneObjectIf.DelChildObject(overrided) " + objectIf.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
            }
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            if (phSceneIf.multiThreadMode) {
                char[] ret0 = new char[_thisNumber] { (char)1, (char)1, (char)1 };
                lock (phSceneIf.phSceneLock) {
                    phSceneIf.isSetFunctionCalledInSubThread = true;
                    if (phSceneIf.isStepThreadExecuting) {
                        phSceneIf.AddCallbackForSubThread(() => {
                            SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)o._thisArray[phSceneIf.sceneForStep]);
                        });
                        ret0[phSceneIf.sceneForBuffer] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)o._thisArray[phSceneIf.sceneForBuffer]);
                        ret0[phSceneIf.sceneForGet] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)o._thisArray[phSceneIf.sceneForGet]);
                    } else {
                        //Console.WriteLine("SetDesc(override)" + " not isStepping " + objectIf.GetIfInfo().ClassName());
                        for (int num = 0; num < _thisNumber; num++) {
                            ret0[num] = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[num], (IntPtr)o._thisArray[num]);
                        }
                    }
                }
                if (ret0[0] == 0 || ret0[1] == 0 || ret0[2] == 0) {
                    Console.Write("failed DelChildObject");
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
        }
        public override bool GetDesc(CsObject desc) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            char ret = (char)0; // <!!> これいいのか？
            ObjectIf objectIf = this as ObjectIf;
            if (phSceneIf.multiThreadMode) {
                var currentThread = Thread.CurrentThread;
                if (currentThread == phSceneIf.stepThread) {
                    if (_thisArray[phSceneIf.sceneForStep] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり,PHIKEnginが呼んでるな
                                                                             //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)desc);
                    } else {
                        //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                        ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_this, (IntPtr)desc);
                    }
                } else if (currentThread == phSceneIf.subThread) {
                    lock (phSceneIf.phSceneLock) {
                        if (_thisArray[phSceneIf.sceneForGet] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり
                                                                                //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                            ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)desc);
                        } else {
                            //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                            ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_this, (IntPtr)desc);
                        }
                    }
                }
            } else {
                ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[0], (IntPtr)desc);
            }
            return (ret == 0) ? false : true;
        }
        public override void SetDesc(CsObject desc) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            ObjectIf objectIf = this as ObjectIf;
            if (phSceneIf.multiThreadMode) {
                lock (phSceneIf.phSceneLock) {
                    phSceneIf.isSetFunctionCalledInSubThread = true;
                    if (phSceneIf.isStepThreadExecuting) {
                        Console.WriteLine("SetDesc(override)" + " isStepping " + objectIf.GetIfInfo().ClassName());
                        phSceneIf.AddCallbackForSubThread(() => {
                            //Console.WriteLine("SetDesc(override)" + " isStepping in Callback " + objectIf.GetIfInfo().ClassName());
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)desc);
                        });
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)desc);
                        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)desc);
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
    }
}
