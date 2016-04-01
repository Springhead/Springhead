using UnityEngine;
using System.Collections;
using SprCs;
using System.Runtime.InteropServices;

public class PHSceneBehaviour : SpringheadBehaviour {
    public PHSceneDescStruct phSceneDescriptor = null;

    static PHSdkIf phSdk = null;
	PHSceneIf phScene = null;

    public override void InitDesc() {
        phSceneDescriptor = new PHSceneDesc();
    }

    void OnGUI() {
        string text = "";
        GUI.TextArea(new Rect(10, 10, 600, 100), text);
    }

    void Awake () {
        SEH_Exception.init();

        PHSceneDesc pd = phSceneDescriptor;

        FWAppBehaviour appB = GetComponent<FWAppBehaviour>();

        if (appB != null) {
            FWApp app = FWAppBehaviour.app;
            phSdk   = app.GetSdk().GetPHSdk();
            phScene = app.GetSdk().GetScene(0).GetPHScene();
            phScene.Clear();
            phScene.SetDesc(pd);
        } else {
            phSdk = PHSdkIf.CreateSdk();
            phScene = phSdk.CreateScene(pd);
        }

        phScene.SetTimeStep(Time.fixedDeltaTime);
        phScene.GetIKEngine().Enable(true);

    }

    void FixedUpdate () {
		if (phScene!=null) {
			phScene.Step ();
		}
    }

    void Update() {
        foreach (PHSolidBehaviour srb in gameObject.GetComponentsInChildren<PHSolidBehaviour>()) {
            srb.UpdatePose();
        }
        if (FWAppBehaviour.app != null) {
            FWAppBehaviour.app.PostRedisplay();
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
