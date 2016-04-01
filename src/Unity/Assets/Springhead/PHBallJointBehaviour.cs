using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHBallJointBehaviour : PHJointBehaviour {
    public PHBallJointDescStruct ballJointDescripter = null;

    public override void InitDesc() {
        ballJointDescripter = new PHBallJointDesc();
    }

    public override IfInfo GetIfInfo() {
        return PHBallJointIf.GetIfInfoStatic();
    }

    public override PHJointDesc BuildDesc(GameObject socket, GameObject plug) {
        return (PHBallJointDesc)ballJointDescripter;
    }
}
