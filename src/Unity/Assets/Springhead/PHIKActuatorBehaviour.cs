using UnityEngine;
using System.Collections;
using SprCs;
using System;

/*
    
    - solidObj1 <PHSolid>
        - jointObj1 <PHJoint, PHIKAct>    
    - solidObj2 <PHSolid>
        - jointObj2 <PHJoint, PHIKAct> sock:solidObj1 plug:solidObj2

    ***

    - solidObj1 <PHSolid, PHJoint, PHIKAct> 
    - solidObj2 <PHSolid, PHJoint, PHIKAct> sock:solidObj1 plug:solidObj2

    ***

    - solidObj1 <PHSolid, PHJoint, PHIKAct>
        - solidObj2 <PHSolid, PHJoint, PHIKAct> sock:solidObj1 plug:solidObj2

    ***

    - solidObj1 <PHSolid>
        - jointObj1 <PHJoint, PHIKAct>
        - solidObj2 <PHSolid>
            - jointObj2 <PHJoint, PHIKAct>

    ***

    - solidObj1 <PHSolid, PHJoint, PHIKAct>
        - solidObj2 <PHSolid>
            - jointObj2 <PHJoint, PHIKAct>

*/

public abstract class PHIKActuatorBehaviour : SprSceneObjBehaviour {
    public override void Link() {
        // まず、このIKActuatorに付随する関節を探す。同じGameObjectに付いているはず
        PHJointBehaviour jo = gameObject.GetComponent<PHJointBehaviour>();

        if (jo != null && jo.sprObject != null && sprObject != null) {
            // 次に、関節の親関節を探す。関節のソケット剛体を探し、それを基準に探す
            PHJointBehaviour joParent = null;
            var jos = jo.socket.GetComponentsInChildren<PHJointBehaviour>();
            foreach (var j in jos) { if (j != jo) { joParent = j; break; } }

            if (joParent != null && jo != joParent) {
                // 親関節に付随するIKActuatorを親Actuatorとして登録する
                PHIKActuatorBehaviour act = joParent.GetComponent<PHIKActuatorBehaviour>();
                if (act != null && act.sprObject != null && sprObject != act.sprObject) {
                    act.sprObject.AddChildObject(sprObject);
                }
            }
        }
    }
}
