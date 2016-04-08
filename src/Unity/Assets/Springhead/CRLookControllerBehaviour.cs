using UnityEngine;
using System.Collections;
using SprCs;

public class CRLookControllerBehaviour : SprBehaviourBase
{
    public GameObject head;
    public GameObject eyeLeft;
    public GameObject eyeRight;

    public CRReachControllerIf headCRReachController = null;
    public CRReachControllerDescStruct controllerDescriptor = null;

    public Vector2 eyeLeftCenter = new Vector2(-0.05f, 0.08f);
    public Vector2 eyeRightCenter = new Vector2(-0.05f, -0.08f);
    public float eyeAngleLimit = Mathf.Deg2Rad * 25f;
    
    void Reset()
    {
        SetDLLPath();
        controllerDescriptor = new CRReachControllerDesc();
        controllerDescriptor.margin = 0;
        controllerDescriptor.targPos.x = 0f;
        controllerDescriptor.targPos.y = 0f;
        controllerDescriptor.targPos.z = -20f;
    }

    void Awake()
    {
        CRCreatureIf crCreature = gameObject.GetComponentInParent<CRCreatureBehaviour>().GetCRCreature();
        CRReachControllerDesc d = controllerDescriptor;
        headCRReachController = (CRReachControllerIf)crCreature.CreateEngine(CRReachControllerIf.GetIfInfoStatic(), d);
        headCRReachController.SetName("c:" + gameObject.name);

        PHIKEndEffectorBehaviour headPHIKEndEffectorBehaviour = head.GetComponent<PHIKEndEffectorBehaviour>();
        headCRReachController.SetIKEndEffector(headPHIKEndEffectorBehaviour.sprObject as PHIKEndEffectorIf, 0);
    }

    public void setTarget(Vector3 position){
        PHSolidIf eyeLeftPHSolid = eyeLeft.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
        PHSolidIf eyeRightPHSolid = eyeRight.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
        PHBallJointIf eyeLeftPHBallJoint = eyeLeft.GetComponent<PHBallJointBehaviour>().sprObject as PHBallJointIf;
        PHBallJointIf eyeRightPHBallJoint = eyeRight.GetComponent<PHBallJointBehaviour>().sprObject as PHBallJointIf;

        Vector3 eyeCenterPosition = (new Vector3((float)eyeLeftPHSolid.GetPose().Pos().x,
                                                (float)eyeLeftPHSolid.GetPose().Pos().y,
                                                (float)eyeLeftPHSolid.GetPose().Pos().z) +
                                    new Vector3((float)eyeRightPHSolid.GetPose().Pos().x,
                                                (float)eyeRightPHSolid.GetPose().Pos().y,
                                                (float)eyeRightPHSolid.GetPose().Pos().z)) * 0.5f;
        Vector3 eyeCenterToTargetVector = position - eyeCenterPosition;
        eyeCenterToTargetVector.Normalize();
        Quaternion headOrientationInverse = new Quaternion();
        headOrientationInverse.w = (float)(head.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf).GetPose().Ori().Inv().w;
        headOrientationInverse.x = (float)(head.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf).GetPose().Ori().Inv().x;
        headOrientationInverse.y = (float)(head.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf).GetPose().Ori().Inv().y;
        headOrientationInverse.z = (float)(head.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf).GetPose().Ori().Inv().z;
        eyeCenterToTargetVector = headOrientationInverse * eyeCenterToTargetVector;
        
        Vector3 eyeLeftToTargetVector = position - new Vector3((float)eyeLeftPHSolid.GetPose().Pos().x,
                                                               (float)eyeLeftPHSolid.GetPose().Pos().y,
                                                               (float)eyeLeftPHSolid.GetPose().Pos().z);
        eyeLeftToTargetVector.Normalize();
        eyeLeftToTargetVector = headOrientationInverse * eyeLeftToTargetVector;
        Vector3 eyeRightToTargetVector = position - new Vector3((float)eyeRightPHSolid.GetPose().Pos().x,
                                                               (float)eyeRightPHSolid.GetPose().Pos().y,
                                                               (float)eyeRightPHSolid.GetPose().Pos().z);
        eyeRightToTargetVector.Normalize();
        eyeRightToTargetVector = headOrientationInverse * eyeRightToTargetVector;

        float eyeY = Mathf.Atan2(-eyeCenterToTargetVector.z, -eyeCenterToTargetVector.y);
        float eyeLeftX = Mathf.Atan2(eyeLeftToTargetVector.x, -eyeLeftToTargetVector.y);
        float eyeRightX = Mathf.Atan2(eyeRightToTargetVector.x, -eyeRightToTargetVector.y);

        Vector2 eyeLeftMoveVector = new Vector2(eyeLeftX, eyeY);
        Vector2 eyeRightMoveVector = new Vector2(eyeRightX, eyeY);

        if ((eyeLeftMoveVector - eyeLeftCenter).magnitude > eyeAngleLimit)
        {
            eyeLeftMoveVector = (eyeLeftMoveVector - eyeLeftCenter).normalized * eyeAngleLimit;
        }
        if ((eyeRightMoveVector - eyeRightCenter).magnitude > eyeAngleLimit)
        {
            eyeRightMoveVector = (eyeRightMoveVector - eyeRightCenter).normalized * eyeAngleLimit;
        }

        eyeLeftMoveVector.y = (eyeLeftMoveVector.y + eyeRightMoveVector.y) / 2f;
        eyeRightMoveVector.y = eyeLeftMoveVector.y;

        Quaternion eyeLeftMoveQuaternion = Quaternion.Euler(eyeLeftMoveVector.y * Mathf.Rad2Deg, 0, eyeLeftMoveVector.x * Mathf.Rad2Deg);
        Quaternion eyeRightMoveQuaternion = Quaternion.Euler(eyeRightMoveVector.y * Mathf.Rad2Deg, 0, eyeRightMoveVector.x * Mathf.Rad2Deg);

        eyeLeftPHBallJoint.SetTargetPosition(new Quaterniond(eyeLeftMoveQuaternion.w, eyeLeftMoveQuaternion.x, eyeLeftMoveQuaternion.y, eyeLeftMoveQuaternion.z));
        eyeRightPHBallJoint.SetTargetPosition(new Quaterniond(eyeRightMoveQuaternion.w, eyeRightMoveQuaternion.x, eyeRightMoveQuaternion.y, eyeRightMoveQuaternion.z));

        headCRReachController.SetFinalPos(new Vec3d(position.x, position.y, position.z));
    }

    void OnValidate()
    {
        SetDLLPath();
    }
}
