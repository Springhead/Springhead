using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHSliderJointBehavior : PHJointBehaviour {
    public PHSliderJointDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHSliderJointDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHSliderJointDescStruct).ApplyTo(to as PHSliderJointDesc);
    }

    public override CsObject CreateDesc() {
        return new PHSliderJointDesc();
    }

    public override PHJointIf CreateJoint(PHSolidIf soSock, PHSolidIf soPlug) {
        return phScene.CreateJoint(soSock, soPlug, PHSliderJointIf.GetIfInfoStatic(), (PHSliderJointDesc)desc);
    }
}
