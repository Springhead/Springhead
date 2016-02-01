using UnityEngine;
using System.Collections;
using SprCs;

public class CDConvexMeshBehaviour : SpringheadBehaviour {
	public CDConvexMeshDescStruct meshDescripter = null;

	void Reset() {
		SetDLLPath();
		meshDescripter = new CDConvexMeshDesc();
	}

	void Awake(){
		PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
		PHSdkIf phSdk = phScene.GetSdk();

		Mesh mr = gameObject.GetComponent<MeshFilter>().mesh;
		PHSolidBehaviour solidBehaviour = gameObject.GetComponentInParent<PHSolidBehaviour>();
		if((mr != null)&&solidBehaviour != null && solidBehaviour.phSolid != null) {
			CDConvexMeshDesc cmesh= new CDConvexMeshDesc();
			//initial CDConvexMeshDesc by Unity Mesh
			for(int vi=0;vi<mr.vertices.Length;vi++)
			{
				Vector3 vU = mr.vertices[vi];
				Vec3f v = new Vec3f();
				v.x = vU.x;
				v.y = vU.y;
				v.z = vU.z;
				cmesh.vertices.push_back(v);
			}
			solidBehaviour.phSolid.AddShape(phSdk.CreateShape(CDConvexMeshIf.GetIfInfoStatic(), cmesh));
		}
	}

	public void OnValidate() {

	}
}
