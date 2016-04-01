using UnityEngine;
using System.Collections;
using SprCs;
using SprUnity;

public abstract class PHJointBehaviour : SpringheadBehaviour {
    public GameObject socket = null;
    public GameObject plug = null;
    public PHJointIf phJoint = null;

    // 関節で接続された剛体同士の衝突を無効ににするかどうか
    public bool disableCollision = false;

	// Use this for initialization
	void Awake () {
        if (!enabled) { return; }

        if (!socket) { socket = gameObject.transform.parent.GetComponentInParent<PHSolidBehaviour>().gameObject; }
        if (!plug)   { plug   = gameObject.GetComponentInParent<PHSolidBehaviour>().gameObject; }

        if (socket == null) { throw new ObjectNotFoundException("Socket object did not found for Joint", gameObject); }
        if (plug == null) { throw new ObjectNotFoundException("Plug object did not found for Joint", gameObject); }

        PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().phSolid;
        PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().phSolid;

        phJoint = GetPHScene().CreateJoint(soSock, soPlug, GetIfInfo(), BuildDesc(socket, plug));
        phJoint.SetName("jo:" + gameObject.name);

        phJoint.SetSocketPose(soSock.GetPose().Inv() * gameObject.transform.ToPose());
        phJoint.SetPlugPose(soPlug.GetPose().Inv() * gameObject.transform.ToPose());
    }

    void Start() {
        if (disableCollision) {
            PHSolidIf soSock = socket.GetComponent<PHSolidBehaviour>().phSolid;
            PHSolidIf soPlug = plug.GetComponent<PHSolidBehaviour>().phSolid;
            GetPHScene().SetContactMode(soSock, soPlug, PHSceneDesc.ContactMode.MODE_NONE);
        }
    }

    // Update is called once per frame
    void Update () {
	
	}

    public PHSceneIf GetPHScene() {
        PHSceneBehaviour pb = gameObject.GetComponentInParent<PHSceneBehaviour>();
        if (pb == null) { throw new ObjectNotFoundException("PHSceneBehaviour was not found in any tree parent", gameObject); }
        return pb.GetPHScene();
    }

    public PHSdkIf GetPHSdk() {
        return GetPHScene().GetSdk();
    }

    /// -- 派生クラスで実装するメソッド

    // 作成するジョイントの型情報を得る
    public abstract IfInfo GetIfInfo();

    // デスクリプタを作成する
    public abstract PHJointDesc BuildDesc(GameObject socket, GameObject plug);
}
