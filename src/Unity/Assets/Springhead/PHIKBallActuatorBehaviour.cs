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

        PHBallJointBehaviour jointBehaviour = gameObject.transform.parent.GetComponentInParent<PHBallJointBehaviour>();
        if (jointBehaviour != null && jointBehaviour.phJoint != null) {
            phIKAct.AddChildObject(jointBehaviour.phJoint);
        }
    }

    void Start () {
        PHIKBallActuatorBehaviour ba = gameObject.transform.parent.GetComponentInParent<PHIKBallActuatorBehaviour>();
        if (ba != null && ba.phIKAct != null && phIKAct != null) {
            ba.phIKAct.AddChildObject(phIKAct);
        }
        OnValidate();
    }

    public void OnValidate() {
        SetDLLPath();
        if (phIKAct != null) {
            phIKAct.SetBias(ballActuatorDescriptor.bias > 0.1F ? ballActuatorDescriptor.bias : 0.1F);
            phIKAct.SetPullbackRate(ballActuatorDescriptor.pullbackRate > 0 ? ballActuatorDescriptor.pullbackRate : 0);
        }
    }
}
