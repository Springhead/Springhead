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

    public override void InitDesc() {
        solidDescripter = new PHSolidDesc();
        solidMaterial = new PHMaterial();
    }

    void Awake () {
        if (!enabled) { return; }

        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour> ().GetPHScene ();
		PHSdkIf phSdk = phScene.GetSdk ();

		phSolid = phScene.CreateSolid (solidDescripter);
        phSolid.SetName("so:" + gameObject.name);

        Vector3 v = gameObject.transform.position;
		Quaternion q = gameObject.transform.rotation;
		phSolid.SetPose (new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));
	}

    public void Start() { }

    public void UpdatePose () {
        if (phSolid != null) {
            if (phSolid.IsDynamical()) {
                // Dynamicalな剛体はSpringheadのシミュレーション結果をUnityに反映
                Posed p = phSolid.GetPose();
                gameObject.transform.position = new Vector3((float)p.px, (float)p.py, (float)p.pz);
                gameObject.transform.rotation = new Quaternion((float)p.x, (float)p.y, (float)p.z, (float)p.w);
            } else {
                // Dynamicalでない剛体はUnityの位置をSpringheadに反映（操作可能）
                Vector3 v = gameObject.transform.position;
                Quaternion q = gameObject.transform.rotation;
                phSolid.SetPose(new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));
            }
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
