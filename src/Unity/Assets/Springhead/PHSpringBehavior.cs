using UnityEngine;
using System.Collections;
using SprCs;

public class PHSpringBehavior : PHJointBehaviour {
    public PHSpringDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHSpringDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHSpringDescStruct).ApplyTo(to as PHSpringDesc);
    }

    public override CsObject CreateDesc() {
        PHSpringDesc d = new PHSpringDesc();
        d.spring = new Vec3d(10.0, 10.0, 10.0);
        d.damper = new Vec3d(1.0, 1.0, 1.0);
        d.springOri = 10.0;
        d.damperOri = 1.0;
        return d;
    }

    public override PHJointIf CreateJoint(PHSolidIf soSock, PHSolidIf soPlug) {
        return phScene.CreateJoint(soSock, soPlug, PHSpringIf.GetIfInfoStatic(), (PHSpringDesc)desc);
    }
}