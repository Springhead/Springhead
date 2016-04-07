using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Reflection;
using SprCs;

public class PHSolidBehaviour : SprSceneObjBehaviour {
    public PHSolidDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHSolidDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHSolidDescStruct).ApplyTo(to as PHSolidDesc);
    }

    public override CsObject CreateDesc() {
        return new PHSolidDesc();
    }

    public override ObjectIf Build() {
		PHSolidIf so = phScene.CreateSolid (desc);
        so.SetName("so:" + gameObject.name);

        Vector3 v = gameObject.transform.position;
		Quaternion q = gameObject.transform.rotation;
		so.SetPose (new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));

        return so;
    }

    public void UpdatePose () {
        if (sprObject != null) {
            PHSolidIf so = sprObject as PHSolidIf;
            if (so.IsDynamical()) {
                // Dynamicalな剛体はSpringheadのシミュレーション結果をUnityに反映
                Posed p = so.GetPose();
                gameObject.transform.position = new Vector3((float)p.px, (float)p.py, (float)p.pz);
                gameObject.transform.rotation = new Quaternion((float)p.x, (float)p.y, (float)p.z, (float)p.w);
            } else {
                // Dynamicalでない剛体はUnityの位置をSpringheadに反映（操作可能）
                Vector3 v = gameObject.transform.position;
                Quaternion q = gameObject.transform.rotation;
                so.SetPose(new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));
            }
        }
	}
}
