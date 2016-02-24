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

        GameObject child = gameObject.transform.FindChild("Sphere").gameObject;
        if (child == null)
        {
            Debug.LogError("child is null");
        }
        SphereCollider sc1 = gameObject.GetComponent<SphereCollider>();
        SphereCollider sc2 = child.GetComponent<SphereCollider>();   
        PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (sc1 != null && sc2 != null && solidBehaviour != null && solidBehaviour.phSolid != null)
        {
            CDRoundConeDesc descRoundCone = new CDRoundConeDesc();
            Vector3 scale1 = gameObject.transform.lossyScale;
            Vector3 scale2 = child.transform.lossyScale;   
            Vector3 position1 = gameObject.GetComponent<Transform>().position;
            Vector3 position2 = child.GetComponent<Transform>().position;
            descRoundCone.radius = new Vec2f(sc1.radius * (Mathf.Max(Mathf.Max(scale1.x, scale1.y), scale1.z)), sc2.radius * (Mathf.Max(Mathf.Max(scale2.x, scale2.y), scale2.z)));
            descRoundCone.length = (position1-position2).magnitude;
            Debug.Log("radius is " + descRoundCone.radius.x + " " + descRoundCone.radius.y);
            Debug.Log("length is "+descRoundCone.length);
            solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDRoundConeIf.GetIfInfoStatic(), descRoundCone));
        }
    }

    public void OnValidate()
    {

    }
}
