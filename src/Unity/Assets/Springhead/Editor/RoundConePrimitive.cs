using UnityEditor;
using UnityEngine;
using System.Collections;

public class RoundConePrimitive : MonoBehaviour {
    static Material default_material = null;

    [MenuItem("GameObject/3D Object/Round Cone")]
	static void OnMenu () {
        GameObject      rc = new GameObject("RoundCone");
        Mesh          mesh = new Mesh();
        MeshRenderer    mr = rc.AddComponent<MeshRenderer>();
        MeshFilter      mf = rc.AddComponent<MeshFilter>();
        MeshRoundCone  mrc = rc.AddComponent<MeshRoundCone>();

        int split = mrc.split, slice = mrc.slice;

        int sphereVtxs = split * (slice + 1);
        int sphereTris = 2 * split * slice;

        int coneVtxs = split * 2;
        int coneTris = split * 2;

        Vector3[] vertices  = new Vector3[2 * sphereVtxs + coneVtxs];
        int[]     triangles = new int[(2 * sphereTris + coneTris) * 3];

        // 頂点位置の設定は後でMeshRoundCone.Reshape()が行う

        // Make Triangles
        /// -- Sphere
        int cnt = 0;
        for (int n = 0; n < 2; n++) {
            for (int i = 0; i < split; i++) {
                for (int j = 0; j < slice; j++) {
                    int i0 = i;
                    int i1 = (i == split - 1) ? 0 : i + 1;

                    triangles[cnt + 0] = split * (j + 0) + i1 + n * sphereVtxs;
                    triangles[cnt + 1] = split * (j + 1) + i0 + n * sphereVtxs;
                    triangles[cnt + 2] = split * (j + 0) + i0 + n * sphereVtxs;
                    cnt += 3;

                    triangles[cnt + 0] = split * (j + 0) + i1 + n * sphereVtxs;
                    triangles[cnt + 1] = split * (j + 1) + i1 + n * sphereVtxs;
                    triangles[cnt + 2] = split * (j + 1) + i0 + n * sphereVtxs;
                    cnt += 3;
                }
            }
        }
        /// -- Cone
        for (int i = 0; i < split; i++) {
            int i0 = i;
            int i1 = (i == split - 1) ? 0 : i + 1;

            triangles[cnt + 0] = split * 0 + i0 + 2 * sphereVtxs;
            triangles[cnt + 1] = split * 1 + i0 + 2 * sphereVtxs;
            triangles[cnt + 2] = split * 0 + i1 + 2 * sphereVtxs; 
            cnt += 3;

            triangles[cnt + 0] = split * 1 + i0 + 2 * sphereVtxs;
            triangles[cnt + 1] = split * 1 + i1 + 2 * sphereVtxs;
            triangles[cnt + 2] = split * 0 + i1 + 2 * sphereVtxs; 
            cnt += 3;
        }

        mesh.vertices  = vertices;
        mesh.triangles = triangles;

        mesh.RecalculateNormals();
        mesh.RecalculateBounds();

        mf.sharedMesh = mesh;

        if (default_material == null) {
            foreach (var i in Resources.FindObjectsOfTypeAll<Material>()) {
                if (i.name=="Default-Material") { default_material = i; break; }
            }
        }

        mr.sharedMaterial = default_material;

        // 頂点位置の設定
        mrc.Reshape();
    }
}
