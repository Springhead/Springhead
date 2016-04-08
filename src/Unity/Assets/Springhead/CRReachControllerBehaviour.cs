using UnityEngine;
using System.Collections;
using SprCs;

public class CRReachControllerBehaviour : SprBehaviourBase
{

    public GameObject handLeft;
    public GameObject handRight;

    public CRReachControllerIf handLeftCRReachController = null;
    public CRReachControllerIf handRightCRReachController = null;
    public CRReachControllerDescStruct handLeftControllerDescriptor = null;
    public CRReachControllerDescStruct handRightControllerDescriptor = null;

    void Reset()
    {
        SetDLLPath();
        handLeftControllerDescriptor = new CRReachControllerDesc();
        handLeftControllerDescriptor.margin = 0;
        handLeftControllerDescriptor.targPos.x = 0f;
        handLeftControllerDescriptor.targPos.y = 0f;
        handLeftControllerDescriptor.targPos.z = -20f;

        handRightControllerDescriptor = new CRReachControllerDesc();
        handRightControllerDescriptor.margin = 0;
        handRightControllerDescriptor.targPos.x = 0f;
        handRightControllerDescriptor.targPos.y = 0f;
        handRightControllerDescriptor.targPos.z = -20f;
    }

    void Awake()
    {
        CRCreatureIf crCreature = gameObject.GetComponentInParent<CRCreatureBehaviour>().GetCRCreature();
        CRReachControllerDesc d1 = handLeftControllerDescriptor;
        CRReachControllerDesc d2 = handRightControllerDescriptor;


        handLeftCRReachController = (CRReachControllerIf)crCreature.CreateEngine(CRReachControllerIf.GetIfInfoStatic(), d1);
        handLeftCRReachController.SetName("c:" + gameObject.name);
        PHIKEndEffectorBehaviour handLeftPHIKEndEffectorBehaviour = handLeft.GetComponent<PHIKEndEffectorBehaviour>();
        handLeftCRReachController.SetIKEndEffector(handLeftPHIKEndEffectorBehaviour.sprObject as PHIKEndEffectorIf, 0);


        handRightCRReachController = (CRReachControllerIf)crCreature.CreateEngine(CRReachControllerIf.GetIfInfoStatic(), d2);
        handRightCRReachController.SetName("c:" + gameObject.name);
        PHIKEndEffectorBehaviour handRightPHIKEndEffectorBehaviour = handRight.GetComponent<PHIKEndEffectorBehaviour>();
        handRightCRReachController.SetIKEndEffector(handRightPHIKEndEffectorBehaviour.sprObject as PHIKEndEffectorIf, 0);
    }

    public void setTarget(Vector3 position)
    {
        handLeftCRReachController.SetFinalPos(new Vec3d(position.x, position.y, position.z));
        handRightCRReachController.SetFinalPos(new Vec3d(position.x, position.y, position.z));
    }

    void OnValidate()
    {
        SetDLLPath();
    }
}
