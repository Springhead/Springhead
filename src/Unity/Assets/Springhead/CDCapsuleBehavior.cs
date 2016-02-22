using UnityEngine;
using System.Collections;
using SprCs;

public class CDCapsuleBehavior : SpringheadBehaviour{
    public CDCapsuleDescStruct capsuleDescripter = null;

    void Reset(){
        SetDLLPath();
        capsuleDescripter = new CDCapsuleDesc();
    }

    void Awake(){       
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        PHSdkIf phSdk = phScene.GetSdk();
         
        CapsuleCollider cc = gameObject.GetComponent<CapsuleCollider>();
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (cc != null && solidBehaviour != null && solidBehaviour.phSolid != null)
        {
            CDCapsuleDesc descCapsule = new CDCapsuleDesc();
            Vector3 scale = gameObject.transform.lossyScale;
            descCapsule.radius = cc.radius * (Mathf.Max(Mathf.Max(scale.x, scale.y), scale.z));
            descCapsule.length = cc.height * scale.y;// cc.radius * (Mathf.Max(Mathf.Max(scale.x, scale.y), scale.z));
            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDCapsuleIf.GetIfInfoStatic(), descCapsule));
        }
        
    }

    public void OnValidate()
    {

    }
}
