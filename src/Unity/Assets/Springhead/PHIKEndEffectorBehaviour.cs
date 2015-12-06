using UnityEngine;
using System.Collections;
using SprCs;

public class PHIKEndEffectorBehaviour : SpringheadBehaviour {
    public PHIKEndEffectorIf phIKee = null;
    public PHIKEndEffectorDescStruct phIKeeDescriptor = null;

    void Reset() {
        SetDLLPath();
        phIKeeDescriptor = new PHIKEndEffectorDesc();
        phIKeeDescriptor.targetLocalDirection = new Vec3d(0, 0, 1);
    }

    void Awake() {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        PHIKEndEffectorDesc dIKEE = phIKeeDescriptor;
        phIKee = phScene.CreateIKEndEffector(dIKEE);
        phIKee.SetName("ike:" + gameObject.name);
        phIKee.Enable(true);
        phIKee.EnableOrientationControl(true);

        PHSolidBehaviour solidBehaviour = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>();
        if (solidBehaviour != null && solidBehaviour.phSolid != null) {
            phIKee.AddChildObject(solidBehaviour.phSolid);
        }

        UpdateIKTargetPosition();
    }

    void Start () {
        PHIKBallActuatorBehaviour ba = gameObject.GetComponentInParent<PHIKBallActuatorBehaviour>();
        if (ba != null && ba.phIKAct != null && phIKee != null) {
            ba.phIKAct.AddChildObject(phIKee);
        }
    }

    void Update () {
        UpdateIKTargetPosition();
    }

    public void UpdateIKTargetPosition() {
        GameObject iktarget = GameObject.Find("IKTarget");
        if (iktarget != null) {
            Vector3 p = iktarget.transform.position;
            Quaternion q = iktarget.transform.rotation;
            if (phIKee != null) {                
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
