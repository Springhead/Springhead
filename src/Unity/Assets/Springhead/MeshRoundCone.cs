using UnityEngine;
using System.Collections;

public class MeshRoundCone : MonoBehaviour {
    public float length = 2.0f;
    public float r1 = 0.5f;
    public float r2 = 1.0f;
    public enum Pivot { Center, R1, R2 };
    public Pivot pivot = Pivot.Center;

    [HideInInspector]
    public int split = 32;
    [HideInInspector]
    public int slice = 16;

    void OnValidate() {
        Reshape();
    }

    public void Reshape () {
        MeshFilter mf = gameObject.GetComponent<MeshFilter>();
        Mesh mesh = mf.sharedMesh;

        if (mesh != null) {
            Vector3[] vertices = mesh.vertices;

            int sphereVtxs = split * (slice + 1);
            int sphereTris = 2 * split * slice;

            int coneVtxs = split * 2;
            int coneTris = split * 2;

            float offset1, offset2;
            if (pivot == Pivot.Center) {
                offset1 = -0.5f * length;
                offset2 = +0.5f * length;
            } else if (pivot == Pivot.R1) {
                offset1 = 0;
                offset2 = length;
            } else {
                offset1 = -length;
                offset2 = 0;
            }

            // Make Vertices
            /// -- Sphere
            for (int n = 0; n < 2; n++) {
                float r = (n == 0) ? r1 : r2;
                float c = (n == 0) ? offset1 : offset2;
                for (int j = 0; j < slice + 1; j++) {
                    for (int i = 0; i < split; i++) {
                        vertices[(split * j + i) + n * sphereVtxs].x = r * Mathf.Cos(Mathf.Deg2Rad * 180.0f * j / slice) + c;
                        vertices[(split * j + i) + n * sphereVtxs].y = r * Mathf.Sin(Mathf.Deg2Rad * 360.0f * i / split) * Mathf.Sin(Mathf.Deg2Rad * 180.0f * j / slice);
                        vertices[(split * j + i) + n * sphereVtxs].z = r * Mathf.Cos(Mathf.Deg2Rad * 360.0f * i / split) * Mathf.Sin(Mathf.Deg2Rad * 180.0f * j / slice);
                    }
                }
            }
            /// -- Cone
            float cr1 = r1, cr2 = r2;
            float cx1 = offset1, cx2 = offset2;
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
                    vertices[(split * n + i) + (2 * sphereVtxs)].x = c;
                    vertices[(split * n + i) + (2 * sphereVtxs)].y = r * Mathf.Sin(Mathf.Deg2Rad * 360.0f * i / split);
                    vertices[(split * n + i) + (2 * sphereVtxs)].z = r * Mathf.Cos(Mathf.Deg2Rad * 360.0f * i / split);
                }
            }

            mesh.vertices = vertices;
            mesh.RecalculateNormals();
            mesh.RecalculateBounds();
        }
    }
}
