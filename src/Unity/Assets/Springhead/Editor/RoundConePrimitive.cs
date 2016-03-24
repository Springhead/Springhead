using UnityEditor;
using UnityEngine;
using System.Collections;

public class RoundConePrimitive : MonoBehaviour {
    static Material default_material = null;

    [MenuItem("GameObject/3D Object/Round Cone")]
	static void OnMenu () {
        GameObject   rc = new GameObject("RoundCone");
        MeshRenderer mr = rc.AddComponent<MeshRenderer>();
        MeshFilter   mf = rc.AddComponent<MeshFilter>();
        MeshRoundCone mrc = rc.AddComponent<MeshRoundCone>();

        Mesh mesh = new Mesh();

        float length = mrc.length, r1 = mrc.r1, r2 = mrc.r2;
        int split = mrc.split, slice = mrc.slice;

        int sphereVtxs = split * (slice + 1);
        int sphereTris = 2 * split * slice;

        int coneVtxs = split * 2;
        int coneTris = split * 2;

        Vector3[] vertices  = new Vector3[2 * sphereVtxs + coneVtxs];
        int[]     triangles = new int[(2 * sphereTris + coneTris) * 3];

        // Make Vertices
        /// -- Sphere
        for (int n = 0; n < 2; n++) {
            float r = (n == 0) ? r1 : r2;
            float c = (n == 0) ? -0.5f*length : +0.5f*length;
            for (int j = 0; j < slice + 1; j++) {
                for (int i = 0; i < split; i++) {
                    vertices[(split * j + i) + n * sphereVtxs] = new Vector3(
                        r * Mathf.Cos(Mathf.Deg2Rad * 180.0f * j / slice) + c,
                        r * Mathf.Sin(Mathf.Deg2Rad * 360.0f * i / split) * Mathf.Sin(Mathf.Deg2Rad * 180.0f * j / slice),
                        r * Mathf.Cos(Mathf.Deg2Rad * 360.0f * i / split) * Mathf.Sin(Mathf.Deg2Rad * 180.0f * j / slice)
                    );
                }
            }
        }
        /// -- Cone
        float cr1 = r1, cr2 = r2;
        float cx1 = -0.5f * length, cx2 = +0.5f * length;
        if (r1 > r2) {
            float cos = (r1 - r2) / length;
            cx1 += r1 * cos;
            cx2 += r2 * cos;
            cr1 = r1 * Mathf.Sqrt(1 - cos * cos);
            cr2 = r2 * Mathf.Sqrt(1 - cos * cos);
        } else {
            float cos = (r2 - r1) / length;
            cx1 -= r1 * cos;
            cx2 -= r2 * cos;
            cr1 = r1 * Mathf.Sqrt(1 - cos * cos);
            cr2 = r2 * Mathf.Sqrt(1 - cos * cos);
        }
        for (int n = 0; n < 2; n++) {
            float r = (n == 0) ? cr1 : cr2;
            float c = (n == 0) ? cx1 : cx2;
            for (int i = 0; i < split; i++) {
                vertices[(split * n + i) + (2 * sphereVtxs)] = new Vector3(
                    c,
                    r * Mathf.Sin(Mathf.Deg2Rad * 360.0f * i / split),
                    r * Mathf.Cos(Mathf.Deg2Rad * 360.0f * i / split)
                );
            }
        }

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
    }
}
