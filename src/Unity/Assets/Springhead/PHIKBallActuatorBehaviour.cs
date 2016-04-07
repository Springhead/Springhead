using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHIKBallActuatorBehaviour : SprSceneObjBehaviour {
    public PHIKBallActuatorDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHIKBallActuatorDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHIKBallActuatorDescStruct).ApplyTo(to as PHIKBallActuatorDesc);
    }

    public override CsObject CreateDesc() {
        return new PHIKBallActuatorDesc();
    }

    public override ObjectIf Build() {
        PHIKBallActuatorIf phIKAct = phScene.CreateIKActuator(PHIKBallActuatorIf.GetIfInfoStatic(), (PHIKBallActuatorDesc)desc).Cast();
        phIKAct.SetName("ika:" + gameObject.name);
        phIKAct.Enable(true);

        PHBallJointBehaviour bj = gameObject.GetComponent<PHBallJointBehaviour>();
        if (bj != null && bj.sprObject != null) {
            phIKAct.AddChildObject(bj.sprObject);
        }

        return phIKAct;
    }

    public override void Link () {
        PHBallJointBehaviour bj = gameObject.GetComponent<PHBallJointBehaviour>();
        if (bj != null && bj.sprObject != null && sprObject != null) {
            PHBallJointBehaviour bjParent = bj.socket.GetComponentInChildren<PHBallJointBehaviour>();
            if (bjParent != null && bj != bjParent) {
                PHIKBallActuatorBehaviour ba = bjParent.GetComponent<PHIKBallActuatorBehaviour>();
                if (ba != null && ba.sprObject != null && sprObject != ba.sprObject) {
                    ba.sprObject.AddChildObject(sprObject);
                }
            }
        }
    }
}
