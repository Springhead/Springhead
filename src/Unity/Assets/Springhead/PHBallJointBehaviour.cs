using UnityEngine;
using System.Collections;
using SprCs;

public class PHBallJointBehaviour : SpringheadBehaviour {
    public PHBallJointIf phJoint = null;
    public PHBallJointDescStruct ballJointDescripter = null;
    public GameObject socket = null;
    public GameObject plug = null;

    void Reset() {
        SetDLLPath();
        ballJointDescripter = new PHBallJointDesc();
        //ballJointDescripter.spring = 5000;
        //ballJointDescripter.damper = 100;
    }

    void Awake () {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();

        if (!socket) {
            socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject;
        }
        if (!plug) {
            plug = gameObject.GetComponentInParent<PHSolidBehaviour>().gameObject;
        }
        print("joint " + name + " : socket = " + socket.name + " plug = " + plug.name);

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
            d.posePlug   = posePlug.Inv() * poseJoint;

            phJoint = phScene.CreateJoint(soSock, soPlug, PHBallJointIf.GetIfInfoStatic(), d).Cast();
            phJoint.SetName("jo:" + gameObject.name);
        } else {
            if (!socket) { print("socket not found for " + name); }
            if (!plug) { print("plug not found for " + name); }
        }
    }
}
