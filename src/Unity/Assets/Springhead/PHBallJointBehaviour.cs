using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHBallJointBehaviour : PHJointBehaviour {
    public PHBallJointDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHBallJointDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHBallJointDescStruct).ApplyTo(to as PHBallJointDesc);
    }

    public override CsObject CreateDesc() {
        return new PHBallJointDesc();
    }

    public override PHJointIf CreateJoint(PHSolidIf soSock, PHSolidIf soPlug) {
        return phScene.CreateJoint(soSock, soPlug, PHBallJointIf.GetIfInfoStatic(), (PHBallJointDesc)desc);
    }
}
