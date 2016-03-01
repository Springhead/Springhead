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
        Transform tr = gameObject.GetComponent<Transform>();
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (cc != null && tr!=null &&solidBehaviour != null && solidBehaviour.phSolid != null)
        {
            CDCapsuleDesc descCapsule = new CDCapsuleDesc();
            Vector3 scale = gameObject.transform.lossyScale;
            Vector3 position = gameObject.GetComponent<Transform>().position;
            descCapsule.radius = cc.radius * (Mathf.Max(scale.x, scale.z));
            descCapsule.length = cc.height * scale.y - descCapsule.radius * 2;
            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDCapsuleIf.GetIfInfoStatic(), descCapsule));
            Vec3d p = new Vec3d(0.0,0.0,0.0);
            Quaterniond q = Quaterniond.Rot(90.0 * Mathf.Deg2Rad, new Vec3d(1.0, 0.0, 0.0));
            Quaterniond qx = Quaterniond.Rot(-tr.rotation.x * Mathf.Deg2Rad, new Vec3d(1.0, 0.0, 0.0));
            Quaterniond qy = Quaterniond.Rot(-tr.rotation.y * Mathf.Deg2Rad, new Vec3d(0.0, 1.0, 0.0));
            Quaterniond qz = Quaterniond.Rot(-tr.rotation.z * Mathf.Deg2Rad, new Vec3d(0.0, 0.0, 1.0));
            q = q * qx * qy * qz;
            solidBehaviour.phSolid.SetShapePose(solidBehaviour.phSolid.NShape()-1, new Posed(q.w,q.x,q.y,q.z, p.x,p.y,p.z));
        }                                   
    }

    public void OnValidate()
    {

    }
}
