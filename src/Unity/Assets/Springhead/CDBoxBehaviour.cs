using UnityEngine;
using System.Collections;
using SprCs;

public class CDBoxBehaviour : SpringheadBehaviour {
    public CDBoxDescStruct boxDescripter = null;

    void Reset() {
        SetDLLPath();
        boxDescripter = new CDBoxDesc();
    }

    void Awake () {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        PHSdkIf phSdk = phScene.GetSdk();

        BoxCollider bc = gameObject.GetComponent<BoxCollider>();
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (bc != null && solidBehaviour != null && solidBehaviour.phSolid != null) {
            CDBoxDesc descBox = new CDBoxDesc();
            Vector3 size = bc.size;
            Vector3 scale = gameObject.transform.lossyScale;
            descBox.boxsize = new Vec3f((float)(size.x * scale.x), (float)(size.y * scale.y), (float)(size.z * scale.z));
            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), descBox));
        }
    }

    public void OnValidate() {

    }
}
