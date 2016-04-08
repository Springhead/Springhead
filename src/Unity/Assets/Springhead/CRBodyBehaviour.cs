using UnityEngine;
using System.Collections;
using SprCs;

public class CRBodyBehaviour : SprBehaviour
{
    public CRBodyDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as CRBodyDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as CRBodyDescStruct).ApplyTo(to as CRBodyDesc);
    }

    public override CsObject CreateDesc() {
        return new CRBodyDesc();
    }

    public override ObjectIf Build() {
        CRCreatureIf crCreature = gameObject.GetComponentInParent<CRCreatureBehaviour>().GetCRCreature();

        CRBodyIf crBody = crCreature.CreateBody(CRBodyIf.GetIfInfoStatic(), desc);
        crBody.SetName("bo:" + gameObject.name);

        //Here adds all bones to this body from this GameObject's childs
        //Search all childs
        Transform[] childGameObjects = this.gameObject.GetComponentsInChildren<Transform>();
        for (int i = 0; i < childGameObjects.Length; i++) {

            //Dont get itself
            if (childGameObjects[i] == this.gameObject.transform) {
                //Debug.Log("<color=blue>Body: </color>" + childGameObjects[i].gameObject.name);
                continue;
            }

            //real childs
            //If it has PHSolidBehaviour component, it is a valid bone, otherwise not
            if (childGameObjects[i].gameObject.GetComponent<PHSolidBehaviour>() == null) {
                //Debug.Log("<color=blue>Body: </color>" + childGameObjects[i].gameObject.name);
                continue;
            }

            //Create a new bone for valid bone
            CRBoneDescStruct boneDescriptor = new CRBoneDesc();
            CRBoneIf crBone = (CRBoneIf)crBody.CreateObject(CRBoneIf.GetIfInfoStatic(), boneDescriptor);
            crBone.SetName("bn:" + childGameObjects[i].gameObject.name);
            //Debug.Log("<color=red>Bone: </color>" + crBone.GetName());

            //Add solid
            PHSolidIf phSolid = childGameObjects[i].gameObject.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
            crBone.AddChildObject(phSolid);

            //Add joint if it exists
            PHBallJointBehaviour phBallJointBehaviour = childGameObjects[i].gameObject.GetComponent<PHBallJointBehaviour>();
            if (phBallJointBehaviour != null) {
                crBone.AddChildObject(phBallJointBehaviour.sprObject as PHJointIf);
            }

            //Add Actuator if it exists
            PHIKBallActuatorBehaviour phIKBallActuatorBehaviour = childGameObjects[i].gameObject.GetComponent<PHIKBallActuatorBehaviour>();
            if (phIKBallActuatorBehaviour != null) {
                crBone.AddChildObject(phIKBallActuatorBehaviour.sprObject as PHIKBallActuatorIf);
            }

            //Add EndEffect if it exists
            PHIKEndEffectorBehaviour phIKEndEffectorBehaviour = childGameObjects[i].gameObject.GetComponent<PHIKEndEffectorBehaviour>();
            if (phIKEndEffectorBehaviour != null) {
                crBone.AddChildObject(phIKEndEffectorBehaviour.sprObject as PHIKEndEffectorIf);
            }
        }

        return crBody;
    }

    public CRBodyIf GetCRBody() { return this.sprObject as CRBodyIf; }
}
