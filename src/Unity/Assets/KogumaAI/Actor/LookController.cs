using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LookController : Actor {

    public CRLookControllerBehaviour crLookControllerBehaviour;
    Queue<Behavior> behaviors = null;

    public void init(CRLookControllerBehaviour crLookControllerBehaviour, Queue<Behavior> behaviors)
    {
        this.crLookControllerBehaviour = crLookControllerBehaviour;
        this.behaviors = behaviors;
    }

    public void step(){
        if (behaviors.Count == 0) {
            return;
        }
        Behavior topBehavior = behaviors.Peek();
        if (topBehavior.GetType() != typeof(LookBehavior))
        {
            return;
        }
        behaviors.Dequeue();
        LookBehavior currentLookBehavior = (LookBehavior)topBehavior;
        //Debug.Log("currentLookBehavior.targetPosition = " + currentLookBehavior.targetPosition);
        //Debug.Log("behaviors.Count = " + behaviors.Count);
        this.crLookControllerBehaviour.setTarget(currentLookBehavior.targetPosition);
    }

}
