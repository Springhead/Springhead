using UnityEditor;
using UnityEngine;
using System.Collections;

[CustomEditor(typeof(MeshRoundCone))]
public class RoundConeEditor : Editor {
	void OnSceneGUI () {
        MeshRoundCone mrc = target as MeshRoundCone;
        Transform trans = mrc.gameObject.transform;

        float defLeng = HandleUtility.GetHandleSize(Vector3.left);
        Vector3 pos = trans.position;
        Vector3 scale = trans.localScale;
        Quaternion rot = trans.rotation;

        Vector3 posOffset = new Vector3(mrc.length * 0.5f, 0, 0);

        EditorGUI.BeginChangeCheck();
        float r1 = Handles.RadiusHandle(rot, pos - rot * posOffset, mrc.r1);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Radius 1");
            mrc.r1 = r1;
            mrc.OnValidate();
        }

        EditorGUI.BeginChangeCheck();
        float r2 = Handles.RadiusHandle(rot, pos + rot * posOffset, mrc.r2);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Radius 2");
            mrc.r2 = r2;
            mrc.OnValidate();
        }

        EditorGUI.BeginChangeCheck();
        float length = Handles.ScaleSlider(mrc.length, pos, rot * new Vector3(0,1,0), rot, 1, 0.5f);
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Change Length");
            mrc.length = length;
            mrc.OnValidate();
        }
    }
}
