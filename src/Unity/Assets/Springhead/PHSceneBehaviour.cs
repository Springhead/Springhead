using UnityEngine;
using System.Collections;
using SprCs;

public class PHSceneBehaviour : SpringheadBehaviour {
    public PHSceneDescStruct phSceneDescriptor = null;

    static PHSdkIf phSdk = null;
	PHSceneIf phScene = null;

    void Reset() {
        SetDLLPath();
        phSceneDescriptor = new PHSceneDesc();
    }

    void OnGUI() {
        string text = "";
        GUI.TextArea(new Rect(10, 10, 600, 100), text);
    }

    void Awake () {
        if (phSdk==null) {
			phSdk = PHSdkIf.CreateSdk ();
		}

        PHSceneDesc d = phSceneDescriptor;
		phScene = phSdk.CreateScene (d);

        phScene.SetTimeStep(Time.fixedDeltaTime);
        phScene.GetIKEngine().Enable(true);
	}

    void FixedUpdate () {
		if (phScene!=null) {
			phScene.Step ();
            for (int i=0; i<phScene.NJoints(); i++) {
                PHBallJointIf j = phScene.GetJoint(i).Cast();
            }
		}
	}

    void Update() {
        foreach (PHSolidBehaviour srb in gameObject.GetComponentsInChildren<PHSolidBehaviour>()) {
            srb.UpdatePose();
        }
    }

    void OnValidate() {
        SetDLLPath();
        if (phScene != null) {
            phScene.SetDesc(phSceneDescriptor);
        }
    }

    public PHSceneIf GetPHScene() { return phScene; }
}
