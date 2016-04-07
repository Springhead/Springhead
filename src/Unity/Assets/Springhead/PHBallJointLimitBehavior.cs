using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHBallJointLimitBehavior : SprSceneObjBehaviour {
    public PHBallJointConeLimitDescStruct desc = null;

    public GameObject jointObject = null;

    public Quaternion rot = new Quaternion(0,0,0,1);

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHBallJointConeLimitDesc; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHBallJointConeLimitDescStruct).ApplyTo(to as PHBallJointConeLimitDesc);
    }

    public override CsObject CreateDesc() {
        PHBallJointConeLimitDesc d = new PHBallJointConeLimitDesc();
        d.bEnabled = true;
        return d;
    }

    public override ObjectIf Build() {
        PHBallJointIf jo = null;

        var b = (jointObject ? jointObject : gameObject).GetComponent<PHBallJointBehaviour>();
        if (!b) { return null; }

        jo = b.sprObject as PHBallJointIf;
        if (jo == null) { return null; }

        PHBallJointConeLimitDesc d = desc;
        PHBallJointLimitIf lim = jo.CreateLimit(PHBallJointConeLimitIf.GetIfInfoStatic(), d);

        return lim;
    }

    public override void OnValidate() {
        Initialize();

        // LimitにSetDescしても効果がなかったので直接セット
        if (sprObject != null) {
            PHBallJointConeLimitIf limit = sprObject as PHBallJointConeLimitIf;
            limit.Enable(desc.bEnabled);
            limit.SetDamper(desc.damper);
            limit.SetLimitDir(desc.limitDir);
            limit.SetSpring(desc.spring);
            limit.SetSwingRange(desc.limitSwing);
            limit.SetSwingDirRange(desc.limitSwingDir);
            limit.SetTwistRange(desc.limitTwist);
        }
    }
}
