using UnityEngine;
using System.Collections;
using SprCs;
using System.Runtime.InteropServices;
using System;

public class PHSceneBehaviour : SprBehaviour {
    static PHSdkIf phSdk = null;

    public PHSceneDescStruct desc = null;

    public bool enableIK = true;
    public bool enableStep = true;
    public bool enableUpdate = true;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHSceneDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHSceneDescStruct).ApplyTo(to as PHSceneDesc);
    }

    public override CsObject CreateDesc() {
        return new PHSceneDesc();
    }

    void OnGUI() {
        string text = "";
        GUI.TextArea(new Rect(10, 10, 600, 100), text);
    }

    /*
    int cnt = 0;
    delegate void FunctionPointerTarget(IntPtr x);
    void OnTimer(IntPtr x) {
        cnt++;
        print("Timer! : " + cnt);
    }
    */

    public override ObjectIf Build() {
        SEH_Exception.init();

        FWAppBehaviour appB = GetComponent<FWAppBehaviour>();

        PHSceneIf phScene;
        if (appB != null) {
            FWApp app = FWAppBehaviour.app;
            phSdk   = app.GetSdk().GetPHSdk();
            phScene = app.GetSdk().GetScene(0).GetPHScene();
            phScene.Clear();
            // phScene.SetDesc(desc);
        } else {
            phSdk = PHSdkIf.CreateSdk();
            phScene = phSdk.CreateScene(desc);
        }

        phScene.SetTimeStep(Time.fixedDeltaTime);

        if (enableIK) {
            phScene.GetIKEngine().Enable(true);
        }

        /*
        UTTimerIf timer = UTTimerIf.Create();
        print(timer.get());

        FunctionPointerTarget fp = new FunctionPointerTarget(OnTimer);
        var functionPointer = Marshal.GetFunctionPointerForDelegate(fp);
        GCHandle _gcHandle = GCHandle.Alloc(fp);
        timer.SetMode(UTTimerIf.Mode.MULTIMEDIA);
        timer.SetCallback(functionPointer);
        //timer.SetResolution(1);
        //timer.SetInterval(1);
        timer.Start();
        */

        return phScene;
    }

    void FixedUpdate () {
		if (sprObject!=null && enableStep) {
			(sprObject as PHSceneIf).Step ();
		}
    }

    void Update() {
        if (enableUpdate) {
            foreach (PHSolidBehaviour srb in gameObject.GetComponentsInChildren<PHSolidBehaviour>()) {
                srb.UpdatePose();
            }
            if (FWAppBehaviour.app != null) {
                FWAppBehaviour.app.PostRedisplay();
            }
        }
    }
}
