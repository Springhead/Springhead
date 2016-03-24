using UnityEngine;
using System.Collections;
using SprCs;

public class PHIKBallActuatorBehaviour : SpringheadBehaviour {
    public PHIKBallActuatorIf phIKAct = null;
    public PHIKBallActuatorDescStruct ballActuatorDescriptor = null;

    void Reset() {
        SetDLLPath();
        ballActuatorDescriptor = new PHIKBallActuatorDesc();
    }

    void Awake() {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();

        PHIKBallActuatorDesc dIK = ballActuatorDescriptor;
        phIKAct = phScene.CreateIKActuator(PHIKBallActuatorIf.GetIfInfoStatic(), dIK).Cast();
        phIKAct.SetName("ika:" + gameObject.name);
        phIKAct.Enable(true);

        PHBallJointBehaviour bj = gameObject.GetComponent<PHBallJointBehaviour>();
        if (bj != null && bj.phJoint != null) {
            phIKAct.AddChildObject(bj.phJoint);
        }
    }

    void Start () {
        PHBallJointBehaviour bj = gameObject.GetComponent<PHBallJointBehaviour>();
        if (bj != null && bj.phJoint != null && phIKAct != null) {
            PHBallJointBehaviour bjParent = bj.socket.GetComponentInChildren<PHBallJointBehaviour>();
            if (bjParent != null && bj != bjParent) {
                PHIKBallActuatorBehaviour ba = bjParent.GetComponent<PHIKBallActuatorBehaviour>();
                if (ba != null && ba.phIKAct != null && phIKAct != ba.phIKAct) {
                    ba.phIKAct.AddChildObject(phIKAct);
                }
            }
        }
        OnValidate();
    }

    public void OnValidate() {
        SetDLLPath();
        if (ballActuatorDescriptor == null) {
            ballActuatorDescriptor = new PHIKBallActuatorDesc();
        }
        if (phIKAct != null) {
            phIKAct.SetBias(ballActuatorDescriptor.bias > 0.1F ? ballActuatorDescriptor.bias : 0.1F);
            phIKAct.SetPullbackRate(ballActuatorDescriptor.pullbackRate > 0 ? ballActuatorDescriptor.pullbackRate : 0);
        }
    }
}
