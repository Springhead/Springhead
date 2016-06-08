using UnityEngine;
using System.Collections.Generic;
using System.Linq;

public class MinJerkRotTraj {
    public Quaternion startRot = new Quaternion();
    public Quaternion finalRot = new Quaternion();
    public float startTime = 0;
    public float finalTime = 1;

    public Quaternion GetCurrDeltaRot(float t) {
        Quaternion deltaRot = finalRot * Quaternion.Inverse(startRot);

        if (t < startTime) { return startRot; }
        if (finalTime < t) { return deltaRot; }
        if (startTime == finalTime) { return deltaRot; }

        float tau = (t - startTime) / (finalTime - startTime);

        float s = 10 * Mathf.Pow(tau, 3) - 15 * Mathf.Pow(tau, 4) + 6 * Mathf.Pow(tau, 5);

        float angle; Vector3 axis;
        deltaRot.ToAngleAxis(out angle, out axis);
        angle *= s;

        return Quaternion.AngleAxis(angle, axis);
    }
}

public class LookBehavibour : MonoBehaviour {
    // Input
    public GameObject lookTarget = null;
    public Vector3 lookTargetPosition = new Vector3();

    // Output
    public GameObject controlTarget = null;

    // Parameter
    public float subMovInterval = 0.2f; // [s]
    public float subMovDuration = 0.4f; // [s]

    public float marginAngle =   0.0f; // [deg]
    public float limitAngle  = 360.0f; // [deg]
    public GameObject parentObject = null; // Limitの基準となるフレーム

    // 要素動作キュー
    private Queue<MinJerkRotTraj> subMovements = new Queue<MinJerkRotTraj>();

    // Timer
    private float currTime = 0;
    private float waitTime = 0;

    // 
    private Quaternion baseRot = new Quaternion();

    // Use this for initialization
    void Start() {
        if (controlTarget == null) {
            controlTarget = gameObject;
        }

        // 最初に要素動作キューに現在姿勢を入れておく
        MinJerkRotTraj subMov = new MinJerkRotTraj();
        subMov.startTime = currTime;
        subMov.finalTime = currTime + subMovDuration;
        subMov.startRot = controlTarget.transform.rotation;
        subMov.finalRot = controlTarget.transform.rotation;
        subMovements.Enqueue(subMov);

        // 
        baseRot = controlTarget.transform.rotation;
    }

    // FixedUpdate is called once per step
    void FixedUpdate() {
        // intervalの時間が経過していたら目標位置の追加を行う
        waitTime -= Time.fixedDeltaTime;
        if (waitTime <= 0) {
            waitTime = subMovInterval;

            // 目標位置から目標視線方向ベクトルを計算
            if (lookTarget != null) {
                lookTargetPosition = lookTarget.transform.position;
            }
            Vector3 lookDir = (lookTargetPosition - controlTarget.transform.position).normalized;

            // 現在の視線方向も計算（マージンの計算用）
            Vector3 currDir = gameObject.transform.TransformDirection(new Vector3(0, 0, 1));
            float currAngle = Mathf.Acos(Vector3.Dot(currDir, lookDir)) * Mathf.Rad2Deg;

            if (currAngle > marginAngle) { // 目標方向がマージンの外に出ていたら
                // マージンをとった目標位置を計算
                Vector3 pullbackAxis = Vector3.Cross(lookDir, currDir);
                Quaternion pullbackRot = Quaternion.AngleAxis(marginAngle, pullbackAxis);
                lookDir = pullbackRot * lookDir;

                // 視線方向ベクトルをクォータニオンに変換
                Quaternion lookRot = new Quaternion(); lookRot.SetLookRotation(lookDir);

                // 新規要素動作をエンキュー
                MinJerkRotTraj subMov = new MinJerkRotTraj();
                subMov.startTime = currTime;
                subMov.finalTime = currTime + subMovDuration;
                subMov.startRot = subMovements.Last().finalRot;
                subMov.finalRot = lookRot;
                subMovements.Enqueue(subMov);

                // 終了した要素動作をデキュー
                while (subMovements.Count > 0 && subMovements.First().finalTime < currTime) {
                    subMovements.Dequeue();
                }
            }
        }

        // 現在時刻における視線姿勢を計算
        Quaternion total = subMovements.First().startRot;
        foreach (var sub in subMovements) {
            total = sub.GetCurrDeltaRot(currTime) * total;
        }

        // リミットを適用
        if (parentObject != null) {
            Vector3 parentDir = parentObject.transform.TransformDirection(new Vector3(0, 0, 1));
            Vector3 totalDir  = total * new Vector3(0, 0, 1);
            float pullbackAngle = Mathf.Acos(Vector3.Dot(parentDir, totalDir)) * Mathf.Rad2Deg - limitAngle;
            if (pullbackAngle > 0) {
                Vector3 pullbackAxis = Vector3.Cross(totalDir, parentDir);
                Quaternion pullbackRot = Quaternion.AngleAxis(pullbackAngle, pullbackAxis);
                total = pullbackRot * total;
            }
        }

        // 動作対象の姿勢に反映
        controlTarget.transform.rotation = total * baseRot;

        // 時刻を進める
        currTime += Time.fixedDeltaTime;
    }
}
