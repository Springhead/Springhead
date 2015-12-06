using UnityEngine;
using System.Collections;
using SprCs;

public class CDSphereBehaviour : SpringheadBehaviour {
    public CDSphereDescStruct sphereDescripter = null;

    void Reset() {
        SetDLLPath();
        sphereDescripter = new CDSphereDesc();
    }

    void Awake() {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        PHSdkIf phSdk = phScene.GetSdk();

        SphereCollider sc = gameObject.GetComponent<SphereCollider>();
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (sc != null && solidBehaviour != null && solidBehaviour.phSolid != null) {
            CDSphereDesc descSphere = new CDSphereDesc();
            Vector3 scale = gameObject.transform.lossyScale;
            descSphere.radius = sc.radius * (Mathf.Max(Mathf.Max(scale.x, scale.y), scale.z));
            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDSphereIf.GetIfInfoStatic(), descSphere));
        }
    }

    public void OnValidate() {

    }
}
