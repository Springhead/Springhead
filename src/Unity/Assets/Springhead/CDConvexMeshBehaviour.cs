using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDConvexMeshBehaviour : CDShapeBehaviour {
    public override CsObject descStruct {
        get { return null; }
        set { }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
    }

    public override CsObject CreateDesc() {
        return null;
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
		Mesh mesh = shapeObject.GetComponent<MeshFilter>().mesh;

        CDConvexMeshDesc d = new CDConvexMeshDesc();
        // Initialize CDConvexMeshDesc by Unity Mesh
        for (int vi = 0; vi < mesh.vertices.Length; vi++) {
            Vector3 vU = mesh.vertices[vi];
            Vec3f v = new Vec3f();
            v.x = vU.x;
            v.y = vU.y;
            v.z = vU.z;
            d.vertices.push_back(v);
        }

        return phSdk.CreateShape(CDConvexMeshIf.GetIfInfoStatic(), d);
	}
}
