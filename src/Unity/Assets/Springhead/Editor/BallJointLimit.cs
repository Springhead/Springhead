using UnityEditor;
using UnityEngine;
using System.Collections;

[CustomEditor(typeof(PHBallJointLimitBehavior))]
public class BallJointLimit : Editor {
    void OnSceneGUI() {
        /*
        PHBallJointLimitBehavior limit = target as PHBallJointLimitBehavior;
        Transform trans = limit.gameObject.transform;

        EditorGUI.BeginChangeCheck();
        Quaternion rot = Handles.
        if (EditorGUI.EndChangeCheck()) {
            Undo.RecordObject(target, "Free Rotate");
            limit.rot = rot;
            Debug.Log(limit.rot);
        }
        */
    }
}
