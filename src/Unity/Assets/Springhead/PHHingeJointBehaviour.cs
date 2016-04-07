using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHHingeJointBehaviour : PHJointBehaviour {
    public PHHingeJointDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHHingeJointDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHHingeJointDescStruct).ApplyTo(to as PHHingeJointDesc);
    }

    public override CsObject CreateDesc() {
        return new PHHingeJointDesc();
    }

    public override PHJointIf CreateJoint(PHSolidIf soSock, PHSolidIf soPlug) {
        return phScene.CreateJoint(soSock, soPlug, PHHingeJointIf.GetIfInfoStatic(), (PHHingeJointDesc)desc);
    }
}
