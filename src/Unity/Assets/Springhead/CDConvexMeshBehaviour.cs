using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDConvexMeshBehaviour : CDShapeBehaviour {
	public CDConvexMeshDescStruct meshDescripter = null;

	public override void InitDesc() {
		meshDescripter = new CDConvexMeshDesc();
	}

    public override CDShapeIf CreateShape(GameObject shapeObject) {
		Mesh mesh = shapeObject.GetComponent<MeshFilter>().mesh;

        // Initialize CDConvexMeshDesc by Unity Mesh
        CDConvexMeshDesc cmesh = new CDConvexMeshDesc();
        for (int vi = 0; vi < mesh.vertices.Length; vi++) {
            Vector3 vU = mesh.vertices[vi];
            Vec3f v = new Vec3f();
            v.x = vU.x;
            v.y = vU.y;
            v.z = vU.z;
            cmesh.vertices.push_back(v);
        }

        return GetPHSdk().CreateShape(CDConvexMeshIf.GetIfInfoStatic(), cmesh);
	}

	public void OnValidate() {
	}
}
