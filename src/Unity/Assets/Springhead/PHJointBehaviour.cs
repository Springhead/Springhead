using UnityEngine;
using System.Collections;
using SprCs;
using SprUnity;

public abstract class PHJointBehaviour : SprSceneObjBehaviour {
    public GameObject socket = null;
    public GameObject plug = null;

    // 関節で接続された剛体同士の衝突を無効ににするかどうか
    public bool disableCollision = false;

    // Use this for initialization
    public override ObjectIf Build() {
        if (!socket) { socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject; }
        if (!plug)   { plug   = gameObject.GetComponentInParent<PHSolidBehaviour>().gameObject; }

        if (socket == null) { throw new ObjectNotFoundException("Socket object did not found for Joint", gameObject); }
        if (plug == null) { throw new ObjectNotFoundException("Plug object did not found for Joint", gameObject); }

        PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
        PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;

        PHJointIf jo = CreateJoint(soSock, soPlug);

        jo.SetName("jo:" + gameObject.name);

        jo.SetSocketPose(soSock.GetPose().Inv() * gameObject.transform.ToPose());
        jo.SetPlugPose(soPlug.GetPose().Inv() * gameObject.transform.ToPose());

        return jo;
    }

    public override void Link() {
        if (disableCollision) {
            PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
            PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
            phScene.SetContactMode(soSock, soPlug, PHSceneDesc.ContactMode.MODE_NONE);
        }
    }

    /// -- 派生クラスで実装するメソッド

    // 関節を作成する
    public abstract PHJointIf CreateJoint(PHSolidIf soSock, PHSolidIf soPlug);
}
