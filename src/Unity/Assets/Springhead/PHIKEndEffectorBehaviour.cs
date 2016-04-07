using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class PHIKEndEffectorBehaviour : SprSceneObjBehaviour {
    public PHIKEndEffectorDescStruct desc = null;
    public GameObject iktarget = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHIKEndEffectorDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHIKEndEffectorDescStruct).ApplyTo(to as PHIKEndEffectorDesc);
    }

    public override CsObject CreateDesc() {
        PHIKEndEffectorDesc d = new PHIKEndEffectorDesc();
        d.targetLocalDirection = new Vec3d(0, 0, 1);
        return d;
    }

    public override ObjectIf Build() {
        PHIKEndEffectorIf phIKee = phScene.CreateIKEndEffector((PHIKEndEffectorDesc)desc);
        phIKee.SetName("ike:" + gameObject.name);
        phIKee.Enable(true);
        phIKee.EnableOrientationControl(true);

        PHSolidBehaviour solidBehaviour = gameObject.GetComponent<PHSolidBehaviour>();
        if (solidBehaviour != null && solidBehaviour.sprObject != null) {
            phIKee.AddChildObject(solidBehaviour.sprObject);
        }

        UpdateIKTargetPosition();

        return phIKee;
    }

    public override void Link() {
        PHIKBallActuatorBehaviour ba = gameObject.GetComponentInChildren<PHIKBallActuatorBehaviour>();
        if (ba != null && ba.sprObject != null && sprObject != null) {
            ba.sprObject.AddChildObject(sprObject);
        }
    }

    void Update () {
        UpdateIKTargetPosition();
    }

    public void UpdateIKTargetPosition() {
        if (iktarget != null) {
            Vector3 p = iktarget.transform.position;
            Quaternion q = iktarget.transform.rotation;
            if (sprObject != null) {
                PHIKEndEffectorIf phIKee = sprObject as PHIKEndEffectorIf;
                if (phIKee.GetOriCtlMode() == PHIKEndEffectorDesc.OriCtlMode.MODE_LOOKAT) {
                    phIKee.SetTargetLookat(new Vec3f(p.x, p.y, p.z));
                } else {
                    phIKee.SetTargetPosition(new Vec3f(p.x, p.y, p.z));
                    phIKee.SetTargetOrientation(new Quaternionf(q.w, q.x, q.y, q.z));
                }
            }
        }
    }
}
