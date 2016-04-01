using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDBoxBehaviour : CDShapeBehaviour {
    public CDBoxDescStruct boxDescripter = null;

    public override void InitDesc() {
        boxDescripter = new CDBoxDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        BoxCollider bc = shapeObject.GetComponent<BoxCollider>();
        if (bc == null) { throw new ObjectNotFoundException("CDBoxBehaviour requires BoxCollider", shapeObject); }

        CDBoxDesc descBox = new CDBoxDesc();
        Vector3 size = bc.size;
        Vector3 scale = gameObject.transform.lossyScale;
        descBox.boxsize = new Vec3f((float)(size.x * scale.x), (float)(size.y * scale.y), (float)(size.z * scale.z));

        return GetPHSdk().CreateShape(CDBoxIf.GetIfInfoStatic(), descBox);
    }

    public void OnValidate() {

    }
}
