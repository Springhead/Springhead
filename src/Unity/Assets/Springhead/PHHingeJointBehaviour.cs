using UnityEngine;
using System.Collections;
using SprCs;

public class PHHingeJointBehaviour : PHJointBehaviour {
    public PHHingeJointDescStruct hingeJointDescripter = null;

    public override void InitDesc() {
        hingeJointDescripter = new PHHingeJointDesc();
    }

    public override IfInfo GetIfInfo() {
        return PHHingeJointIf.GetIfInfoStatic();
    }

    public override PHJointDesc BuildDesc(GameObject socket, GameObject plug) {
        return (PHHingeJointDesc)hingeJointDescripter;
    }
}
