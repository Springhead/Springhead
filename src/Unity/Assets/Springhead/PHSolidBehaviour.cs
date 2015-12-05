using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Reflection;
using SprCs;

public class PHSolidBehaviour : SpringheadBehaviour {
    public PHSolidIf so = null;
    public PHIKEndEffectorIf ike = null;
    public PHSolidDescStruct solidDescripter = null;
	public PHMaterialStruct  solidMaterial = null;
    public bool ikEndEffector = false;
    public bool look = false;
    public PHIKEndEffectorDescStruct ikeeDescriptor = null;

    void Reset() {
        SetDLLPath();
        solidDescripter = new PHSolidDesc();
        solidMaterial = new PHMaterial();
        ikeeDescriptor = new PHIKEndEffectorDesc();
    }

    // Use this for initialization
    void Awake () {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour> ().GetPHScene ();
		PHSdkIf phSdk = phScene.GetSdk ();

		so = phScene.CreateSolid (solidDescripter);
        so.SetName("so:" + gameObject.name);

        Vector3 v = gameObject.transform.position;
		Quaternion q = gameObject.transform.rotation;
		so.SetPose (new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));

		BoxCollider bc = gameObject.GetComponent<BoxCollider> ();
		if (bc != null) {
			CDBoxDesc descBox = new CDBoxDesc ();
			Vector3 size = bc.size;
			Vector3 scale = gameObject.transform.lossyScale;
			descBox.boxsize = new Vec3f ((float)(size.x * scale.x), (float)(size.y * scale.y), (float)(size.z * scale.z));
			so.AddShape (phSdk.CreateShape (CDBoxIf.GetIfInfoStatic (), descBox));
		}

		SphereCollider sc = gameObject.GetComponent<SphereCollider> ();
		if (sc != null) {
			CDSphereDesc descSphere = new CDSphereDesc();
			Vector3 scale = gameObject.transform.lossyScale;
			descSphere.radius = sc.radius * (Mathf.Max (Mathf.Max (scale.x, scale.y), scale.z));
			so.AddShape (phSdk.CreateShape (CDSphereIf.GetIfInfoStatic (), descSphere));
		}

        if (ikEndEffector) {
            PHIKEndEffectorDesc dIKEE = ikeeDescriptor;
            ike = phScene.CreateIKEndEffector(dIKEE);
            ike.AddChildObject(so);
            ike.SetName("ike:" + gameObject.name);
            ike.Enable(true);
            ike.EnableOrientationControl(true);
            if (look) {
                ike.SetOriCtlMode(PHIKEndEffectorDesc.OriCtlMode.MODE_LOOKAT);
                ike.SetTargetLocalDirection(new Vec3d(0, 0, 1));
                ike.EnablePositionControl(false);
            }

            UpdateIKTargetPosition();
        }
	}

    void Start() {
        PHBallJointBehaviour bj = gameObject.GetComponentInParent<PHBallJointBehaviour>();
        if (bj != null && bj.ika != null && ike != null) {
            bj.ika.AddChildObject(ike);
        }
    }

    // Update is called once per frame
    public void Update() {
        UpdateIKTargetPosition();
    }

    public void UpdatePose () {
        if (so != null) {
			Posed p = so.GetPose ();
			gameObject.transform.position = new Vector3 ((float)p.px, (float)p.py, (float)p.pz);
            gameObject.transform.rotation = new Quaternion ((float)p.x, (float)p.y, (float)p.z, (float)p.w);
		}
	}

    public void UpdateIKTargetPosition() {
        GameObject iktarget = GameObject.Find("IKTarget");
        if (iktarget != null) {
            Vector3 p = iktarget.transform.position;
            Quaternion q = iktarget.transform.rotation;
            if (ike != null) {
                if (look) {
                    ike.SetTargetLookat(new Vec3f(p.x, p.y, p.z));
                } else {
                    ike.SetTargetPosition(new Vec3f(p.x, p.y, p.z));
                    ike.SetTargetOrientation(new Quaternionf(q.w, q.x, q.y, q.z));
                }
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

		if (so != null) {
			so.GetDesc (d);
			solidDescripter.ApplyTo (d);
			so.SetDesc (d);

			for (int i=0; i<so.NShape(); i++) {
				so.GetShape (i).SetMaterial(solidMaterial);
			}
		}

        if (look && ike != null) {
            ike.SetOriCtlMode(PHIKEndEffectorDesc.OriCtlMode.MODE_LOOKAT);
            ike.SetTargetLocalDirection(new Vec3d(0, 0, 1));
            ike.EnablePositionControl(false);
        }
    }
}
