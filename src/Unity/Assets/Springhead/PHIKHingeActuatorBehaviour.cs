using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHIKHingeActuatorBehaviour : PHIKActuatorBehaviour {
    public PHIKHingeActuatorDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHIKHingeActuatorDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHIKHingeActuatorDescStruct).ApplyTo(to as PHIKHingeActuatorDesc);
    }

    public override CsObject CreateDesc() {
        return new PHIKHingeActuatorDesc();
    }

    public override ObjectIf Build() {
        PHIKHingeActuatorIf phIKAct = phScene.CreateIKActuator(PHIKHingeActuatorIf.GetIfInfoStatic(), (PHIKHingeActuatorDesc)desc).Cast();
        phIKAct.SetName("ika:" + gameObject.name);
        phIKAct.Enable(true);

        PHHingeJointBehaviour hj = gameObject.GetComponent<PHHingeJointBehaviour>();
        if (hj != null && hj.sprObject != null) {
            phIKAct.AddChildObject(hj.sprObject);
        }

        return phIKAct;
    }
}
