using UnityEngine;
using System.Collections;
using SprCs;

public class PHSpringBehavior : PHJointBehaviour {
    public PHSpringDescStruct springDescripter = null;

    public override void InitDesc() {
        springDescripter = new PHSpringDesc();
        springDescripter.spring = new Vec3d(10.0, 10.0, 10.0);
        springDescripter.damper = new Vec3d(1.0, 1.0, 1.0);
        springDescripter.springOri = 10.0;
        springDescripter.damperOri = 1.0;
    }

    public override IfInfo GetIfInfo() {
        return PHSpringIf.GetIfInfoStatic();
    }

    public override PHJointDesc BuildDesc(GameObject socket, GameObject plug) {
        return (PHSpringDesc)springDescripter;
    }
}