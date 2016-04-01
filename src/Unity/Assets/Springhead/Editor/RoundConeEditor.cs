using UnityEditor;
using UnityEngine;
using System.Collections;

[CustomEditor(typeof(MeshRoundCone))]
[CanEditMultipleObjects]
public class RoundConeEditor : Editor {
    public override void OnInspectorGUI() {
        DrawDefaultInspector();
    }

    void OnSceneGUI () {
        MeshRoundCone mrc = target as MeshRoundCone;
        Transform trans = mrc.gameObject.transform;

        float defLeng = HandleUtility.GetHandleSize(Vector3.left);
        Vector3 pos = trans.position;
        Vector3 scale = trans.localScale;
        Quaternion rot = trans.rotation;

        float offset1, offset2;
        if (mrc.pivot == MeshRoundCone.Pivot.Center) {
            offset1 = -0.5f * mrc.length;
            offset2 = +0.5f * mrc.length;
        } else if (mrc.pivot == MeshRoundCone.Pivot.R1) {
            offset1 = 0;
            offset2 = mrc.length;
        } else {
            offset1 = -mrc.length;
            offset2 = 0;
        }

        EditorGUI.BeginChangeCheck();
        float r1 = Handles.RadiusHandle(rot, pos + rot * new Vector3(offset1, 0, 0), mrc.r1);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Radius 1");
            mrc.r1 = r1;
            mrc.Reshape();
        }

        EditorGUI.BeginChangeCheck();
        float r2 = Handles.RadiusHandle(rot, pos + rot * new Vector3(offset2, 0, 0), mrc.r2);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Radius 2");
            mrc.r2 = r2;
            mrc.Reshape();
        }

        EditorGUI.BeginChangeCheck();
        float length = Handles.ScaleSlider(mrc.length, pos, rot * new Vector3(0, 1, 0), rot, 1, 0.5f);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Length");
            mrc.length = length;
            mrc.Reshape();
        }
    }
}
