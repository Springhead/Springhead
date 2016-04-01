using UnityEngine;
using System.Collections;
using SprCs;

//----------------------------------------//
//  未完成のプログラムです                  //
//----------------------------------------//

public class PHBallJointLimitBehavior : SpringheadBehaviour {
    public PHBallJointIf phJoint = null;
    //public PHBallJointConeLimitIf phJointConeLimit = null;
    public PHBallJointDescStruct ballJointDescripter = null;
    public PHBallJointConeLimitDescStruct ballJointConeLimitDescripter = null;
    public GameObject socket = null;
    public GameObject plug = null;

    public override void InitDesc() {
        ballJointDescripter = new PHBallJointDesc();
        ballJointConeLimitDescripter = new PHBallJointConeLimitDesc();
        //ballJointDescripter.spring = 5000;
        //ballJointDescripter.damper = 100;
    }

    void Awake() {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();

        if (!socket) {
            socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject;
        }
        if (!plug) {
            plug = gameObject.GetComponentInChildren<PHSolidBehaviour>().gameObject;
        }

        if (socket && plug) {
            PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().phSolid;
            PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().phSolid;

            Vector3 v = gameObject.transform.position;
            Quaternion q = gameObject.transform.rotation;
            Posed poseJoint = new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z);
            Posed poseSock = soSock.GetPose();
            Posed posePlug = soPlug.GetPose();

            PHBallJointDesc d = ballJointDescripter;
            d.poseSocket = poseSock.Inv() * poseJoint;
            d.posePlug = posePlug.Inv() * poseJoint;
            ballJointDescripter.spring = 5000;
            ballJointDescripter.damper = 100;

            PHBallJointConeLimitDesc d2 = ballJointConeLimitDescripter;
            d2.limitSwing.x = 0;
            d2.limitSwing.y = 1;

            phJoint = phScene.CreateJoint(soSock, soPlug, PHBallJointIf.GetIfInfoStatic(), d).Cast();
            //phJointConeLimit = phScene.CreateJoint(soSock, soPlug, PHBallJointConeLimitIf.GetIfInfoStatic(), d).Cast();  
            phJoint.SetName("jo:" + gameObject.name);
            //phJointConeLimit.SetSwingRange(d2.limitSwing);
        }
    }
}