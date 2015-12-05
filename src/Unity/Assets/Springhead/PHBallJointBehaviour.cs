using UnityEngine;
using System.Collections;
using SprCs;

public class PHBallJointBehaviour : SpringheadBehaviour {
    public PHBallJointIf jo = null;
    public PHIKBallActuatorIf ika = null;
    public PHBallJointDescStruct ballJointDescripter = null;
    public PHIKBallActuatorDescStruct ballActuatorDescriptor = null;
    public GameObject socket = null;
    public GameObject plug = null;

    void Reset() {
        SetDLLPath();
        ballJointDescripter = new PHBallJointDesc();
        ballActuatorDescriptor = new PHIKBallActuatorDesc();
    }

    // Use this for initialization
    void Awake () {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();

        if (!socket) {
            socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject;
        }
        if (!plug) {
            plug = gameObject.GetComponentInChildren<PHSolidBehaviour>().gameObject;
        }

        if (socket && plug) {
            PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().so;
            PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().so;

            Vector3 v = gameObject.transform.position;
            Quaternion q = gameObject.transform.rotation;
            Posed poseJoint = new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z);
            Posed poseSock = soSock.GetPose();
            Posed posePlug = soPlug.GetPose();

            PHBallJointDesc d = ballJointDescripter;
            d.poseSocket = poseSock.Inv() * poseJoint;
            d.posePlug   = posePlug.Inv() * poseJoint;
            d.spring = 5000;
            d.damper =  100;

            jo = phScene.CreateJoint(soSock, soPlug, PHBallJointIf.GetIfInfoStatic(), d).Cast();
            jo.SetName("jo:" + gameObject.name);

            PHIKBallActuatorDesc dIK = ballActuatorDescriptor;
            ika = phScene.CreateIKActuator(PHIKBallActuatorIf.GetIfInfoStatic(), dIK).Cast();
            ika.AddChildObject(jo);
            ika.SetName("ika:" + gameObject.name);
            ika.Enable(true);
        }
    }

    void Start() {
        PHBallJointBehaviour bj = gameObject.transform.parent.GetComponentInParent<PHBallJointBehaviour>();
        if (bj != null && bj.ika != null && ika != null) {
            bj.ika.AddChildObject(ika);
        }
        OnValidate();
    }

    public void OnValidate() {
        SetDLLPath();
        if (ika != null) {
            ika.SetBias(ballActuatorDescriptor.bias > 0.1F ? ballActuatorDescriptor.bias : 0.1F);
            ika.SetPullbackRate(ballActuatorDescriptor.pullbackRate > 0 ? ballActuatorDescriptor.pullbackRate : 0);
        }
    }
}
