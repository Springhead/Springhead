using UnityEngine;
using System.Collections;
using SprCs;

public class PHSpringBehavior : SpringheadBehaviour
{
    public PHSpringIf phJoint = null;
    public PHSpringDescStruct SpringDescripter = null;
    public GameObject socket = null;
    public GameObject plug = null;

    void Reset()
    {
        SetDLLPath();
        SpringDescripter = new PHSpringDesc();
        //SpringDescripter.spring = 5000;
        //SpringDescripter.damper = 100;
    }

    void Awake()
    {
        PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();

        if (!socket)
        {
            socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject;
        }
        if (!plug)
        {
            plug = gameObject.GetComponentInChildren<PHSolidBehaviour>().gameObject;
        }

        if (socket && plug)
        {
            PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().phSolid;
            PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().phSolid;

            Vector3 v = gameObject.transform.position;
            Quaternion q = gameObject.transform.rotation;
            Posed poseJoint = new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z);
            Posed poseSock = soSock.GetPose();
            Posed posePlug = soPlug.GetPose();

            PHSpringDesc d = SpringDescripter;
            d.poseSocket = poseSock.Inv() * poseJoint;
            d.posePlug = posePlug.Inv() * poseJoint;
            d.spring = new Vec3d(10.0, 10.0, 10.0);
            d.damper = new Vec3d(1.0, 1.0, 1.0);
            d.springOri = 10.0;
            d.damperOri = 1.0;
                    
            phJoint = phScene.CreateJoint(soSock, soPlug, PHSpringIf.GetIfInfoStatic(), d).Cast();
            phJoint.SetName("jo:" + gameObject.name);
        }
    }
}
