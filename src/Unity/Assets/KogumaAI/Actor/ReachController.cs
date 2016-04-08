using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ReachController : Actor {
    public CRReachControllerBehaviour crReachControllerBehaviour;
    Queue<Behavior> behaviors = null;

    public void init(CRReachControllerBehaviour crReachControllerBehaviour, Queue<Behavior> behaviors) {
        this.crReachControllerBehaviour = crReachControllerBehaviour;
        this.behaviors = behaviors;
    }

    public void step() {
        if(behaviors.Count == 0){
            return;
        }
        Behavior topBehavior = behaviors.Peek();
        if (topBehavior.GetType() != typeof(ReachBehavior)) {
            return;
        }
        behaviors.Dequeue();
        ReachBehavior currentReachBehavior = (ReachBehavior)topBehavior;
        //Debug.Log("currentLookBehavior.targetPosition = " + currentLookBehavior.targetPosition);
        //Debug.Log("behaviors.Count = " + behaviors.Count);
        this.crReachControllerBehaviour.setTarget(currentReachBehavior.targetPosition);
    }
}
