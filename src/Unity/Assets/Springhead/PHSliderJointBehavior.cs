using UnityEngine;
using System.Collections;
using SprCs;

public class PHSliderJointBehavior : SpringheadBehaviour
{
    public PHSliderJointIf phJoint = null;
    public PHSliderJointDescStruct sliderJointDescripter = null;
    public GameObject socket = null;
    public GameObject plug = null;

    void Reset()
    {
        SetDLLPath();
        sliderJointDescripter = new PHSliderJointDesc();
        //SliderJointDescripter.spring = 5000;
        //SliderJointDescripter.damper = 100;
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
            
            PHSliderJointDesc d = sliderJointDescripter;
            d.poseSocket = poseSock.Inv() * poseJoint;
            d.posePlug = posePlug.Inv() * poseJoint;
            //sliderJointDescripter.spring = 3;
            //sliderJointDescripter.damper = 0;
            
            phJoint = phScene.CreateJoint(soSock, soPlug, PHSliderJointIf.GetIfInfoStatic(), d).Cast();
            phJoint.SetName("jo:" + gameObject.name);
        }
    }
}
