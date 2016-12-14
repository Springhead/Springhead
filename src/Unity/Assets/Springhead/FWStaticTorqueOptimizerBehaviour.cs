using UnityEngine;
using System.Collections;
using SprCs;
using System.Runtime.InteropServices;
using System;

public class FWStaticTorqueOptimizerBehaviour : MonoBehaviour {
    public static FWStaticTorqueOptimizer optimizer = null;
    bool bRunning = false;

    public double errorWeight = 10.0;
    public double stabilityWeight = 3.0;
    public double torqueWeight = 0.01;

    void Start() {
        if (optimizer == null) {
            optimizer = new FWStaticTorqueOptimizer();
            optimizer.SetScene(gameObject.GetComponent<PHSceneBehaviour>().sprObject as PHSceneIf);

            optimizer.SetErrorWeight(errorWeight);
            optimizer.SetStabilityWeight(stabilityWeight);
            optimizer.SetTorqueWeight(torqueWeight);

            optimizer.Init();
            bRunning = true;
        }
    }

    void FixedUpdate() {
        if (bRunning) {
            for (int i = 0; i < 1; i++) {
                optimizer.Iterate();
            }
            if (optimizer.TestForTermination()) {
                print("Finished");
                bRunning = false;
            }
            optimizer.TakeFinalValue();
            optimizer.ApplyResult(gameObject.GetComponent<PHSceneBehaviour>().sprObject as PHSceneIf);
        }
    }
}
