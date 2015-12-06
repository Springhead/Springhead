using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Reflection;
using SprCs;

public class PHSolidBehaviour : SpringheadBehaviour {
    public PHSolidIf phSolid = null;
    public PHSolidDescStruct solidDescripter = null;
	public PHMaterialStruct  solidMaterial = null;

    void Reset() {
        SetDLLPath();
        solidDescripter = new PHSolidDesc();
        solidMaterial = new PHMaterial();
    }

    void Awake () {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour> ().GetPHScene ();
		PHSdkIf phSdk = phScene.GetSdk ();

		phSolid = phScene.CreateSolid (solidDescripter);
        phSolid.SetName("so:" + gameObject.name);

        Vector3 v = gameObject.transform.position;
		Quaternion q = gameObject.transform.rotation;
		phSolid.SetPose (new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));
	}

    public void UpdatePose () {
        if (phSolid != null) {
			Posed p = phSolid.GetPose ();
			gameObject.transform.position = new Vector3 ((float)p.px, (float)p.py, (float)p.pz);
            gameObject.transform.rotation = new Quaternion ((float)p.x, (float)p.y, (float)p.z, (float)p.w);
		}
	}

	public void OnValidate() {
        SetDLLPath();
        PHSolidDesc d = new PHSolidDesc ();

		if (solidDescripter == null) {
			solidDescripter = new PHSolidDesc();
		}
		if (solidMaterial == null) {
			solidMaterial = new PHMaterial();
		}

		if (phSolid != null) {
            phSolid.GetDesc (d);
			solidDescripter.ApplyTo (d);
            phSolid.SetDesc (d);

			for (int i=0; i< phSolid.NShape(); i++) {
                phSolid.GetShape (i).SetMaterial(solidMaterial);
			}
		}
    }
}
