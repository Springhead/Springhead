using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHSliderJointBehavior : PHJointBehaviour {
    public PHSliderJointDescStruct sliderJointDescripter = null;

    public override void InitDesc() {
        sliderJointDescripter = new PHSliderJointDesc();
    }

    public override IfInfo GetIfInfo() {
        return PHSliderJointIf.GetIfInfoStatic();
    }

    public override PHJointDesc BuildDesc(GameObject socket, GameObject plug) {
        return (PHSliderJointDesc)sliderJointDescripter;
    }
}
