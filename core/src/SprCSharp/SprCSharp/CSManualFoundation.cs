using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

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
        public PHSceneIf GetCSPHSceneIf() {
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
            return PHSceneIf.GetCSInstance(ptr);
        }

        //public bool DelChildObject(ObjectIf o) {
        //    Console.WriteLine("DelChildObject(overrided) " + this.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
        //    char ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
        //    Console.WriteLine("DelChildObject _thisArray[0]");
        //    if (_thisArray[1] != IntPtr.Zero) {
        //        char ret1 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[1], (IntPtr)o._thisArray[1]);
        //        Console.WriteLine("DelChildObject _thisArray[1]");
        //    }
        //    if (_thisArray[2] != IntPtr.Zero) {
        //        char ret2 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[2], (IntPtr)o._thisArray[2]);
        //        Console.WriteLine("DelChildObject _thisArray[1]");
        //    }
        //    //char ret2 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr) _this, (IntPtr) o._this2); // 上手くいかない
        //    if (ret0 == 0) {
        //        Console.Write("failed DelChildObject");
        //        return false;
        //    } else {
        //        return true;
        //    }
        //    //return (ret0 == 0||ret1 == 0 || ret2 == 0) ? false : true;
        //}
        public override bool GetDesc(CsObject desc) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            char ret = (char)0; // <!!> これいいのか？
            ObjectIf objectIf = this as ObjectIf;
            lock (phSceneIf.phSceneForGetSetLock) {
                if (_thisArray[phSceneIf.sceneForGet] != IntPtr.Zero) { // sceneForGet以外作られてない可能性あり
                    //Console.WriteLine("GetDesc(override) _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                    ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)desc);
                } else {
                    //Console.WriteLine("GetDesc(override) null _thisArrayClassName " + objectIf.GetIfInfo().ClassName());
                    ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_this, (IntPtr)desc);
                }
            }
            return (ret == 0) ? false : true;
        }
        public override void SetDesc(CsObject desc) {
            PHSceneIf phSceneIf = GetCSPHSceneIf();
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
            ObjectIf objectIf = this as ObjectIf;
            lock (phSceneIf.phSceneForGetSetLock) {
                if (phSceneIf.isStepping) {
                        //Console.WriteLine("SetDesc(override)" + " isStepping " + objectIf.GetIfInfo().ClassName());
                    phSceneIf.AddWaitUntilNextStepCallback(() => {
                        //Console.WriteLine("SetDesc(override)" + " isStepping in Callback " + objectIf.GetIfInfo().ClassName());
                        if (_thisArray[phSceneIf.sceneForStep] != IntPtr.Zero) { // こっちにCDShapeも入りえる
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForStep], (IntPtr)desc);
                        }
                        if (_thisArray[phSceneIf.sceneForBuffer] != IntPtr.Zero) {
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForBuffer], (IntPtr)desc);
                        }
                    });
                    SprExport.Spr_ObjectIf_SetDesc((IntPtr)_thisArray[phSceneIf.sceneForGet], (IntPtr)desc);
                } else {
                    //Console.WriteLine("SetDesc(override)" + " not isStepping " + objectIf.GetIfInfo().ClassName());
                    foreach (var _this in _thisArray) {
                        if (_this != IntPtr.Zero) {
                            SprExport.Spr_ObjectIf_SetDesc((IntPtr)_this, (IntPtr)desc);
                        }
                    }
                }
            }
        }
    }
}
