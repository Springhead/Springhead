using UnityEngine;
using System.Collections;
using SprCs;

public class CDRoundConeBehavior : SpringheadBehaviour
{
    public CDRoundConeDescStruct roundConeDescripter = null;

    void Reset()
    {
        SetDLLPath();
        roundConeDescripter = new CDRoundConeDesc();
    }

    void Awake()
    {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        PHSdkIf phSdk = phScene.GetSdk();
                                                                                      
        GameObject child = gameObject.GetComponentInChildren<CDSphereBehaviour>().gameObject;
        if (child == null)
        {
            Debug.LogError("child is null");
        }
        SphereCollider sc1 = gameObject.GetComponent<SphereCollider>();
        SphereCollider sc2 = child.GetComponent<SphereCollider>();
        Transform tr1 = gameObject.GetComponent<Transform>();
        Transform tr2 = child.GetComponent<Transform>();
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (sc1 != null && sc2 != null && tr1 != null && tr2 != null && solidBehaviour != null && solidBehaviour.phSolid != null)
        {
            CDRoundConeDesc descRoundCone = new CDRoundConeDesc();
            Vector3 scale1 = gameObject.transform.lossyScale;
            Vector3 scale2 = child.transform.lossyScale; 

            descRoundCone.radius = new Vec2f(sc2.radius * (Mathf.Max(Mathf.Max(scale2.x, scale2.y), scale2.z)), sc1.radius * (Mathf.Max(Mathf.Max(scale1.x, scale1.y), scale1.z)));
            descRoundCone.length = (tr2.position-tr1.position).magnitude; 

            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDRoundConeIf.GetIfInfoStatic(), descRoundCone));
            double dposx = descRoundCone.length * Mathf.Sin(tr1.rotation.x * Mathf.Deg2Rad);
            double dposy = descRoundCone.length * Mathf.Cos(tr1.rotation.x * Mathf.Deg2Rad) * Mathf.Cos(tr1.rotation.z * Mathf.Deg2Rad);
            double dposz = descRoundCone.length * Mathf.Sin(tr1.rotation.z * Mathf.Deg2Rad);                                             
            Vec3d p = new Vec3d(dposx/2, dposy/2, dposz/2);            
            Quaterniond q = Quaterniond.Rot(90.0 * Mathf.Deg2Rad, new Vec3d(1.0, 0.0, 0.0));
            Quaterniond qx = Quaterniond.Rot(-tr1.rotation.x * Mathf.Deg2Rad, new Vec3d(1.0, 0.0, 0.0));
            Quaterniond qy = Quaterniond.Rot(-tr1.rotation.y * Mathf.Deg2Rad, new Vec3d(0.0, 1.0, 0.0));
            Quaterniond qz = Quaterniond.Rot(-tr1.rotation.z * Mathf.Deg2Rad, new Vec3d(0.0, 0.0, 1.0));
            q = q * qx * qy * qz;
            solidBehaviour.phSolid.SetShapePose(solidBehaviour.phSolid.NShape() - 1, new Posed(q.w, q.x, q.y, q.z, p.x, p.y, p.z));
        }
    }                                             
    public void OnValidate()
    {
    }
}
