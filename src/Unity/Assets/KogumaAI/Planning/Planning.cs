using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Planning : KnowledgeSource {

    public BodyKnowledge bodyKnowledge = null;
    public Dictionary<System.Object, ObjectKnowledge> objectKnowledges = null;
    Queue<Behavior> behaviors = null; 

    public void init(BodyKnowledge bodyKnowledge, Dictionary<System.Object, ObjectKnowledge> objectKnowledges, Queue<Behavior> behaviors)
    {
        this.bodyKnowledge = bodyKnowledge;
        this.objectKnowledges = objectKnowledges;
        this.behaviors = behaviors;
    }

    public void step() { 
        //Get the largest attention value object knowledge
        float largestAttention = 0;
        ObjectKnowledge attentionedObjectKnowledge = null;
        foreach(KeyValuePair<System.Object, ObjectKnowledge> objectKnowledge in this.objectKnowledges){
            if (largestAttention < objectKnowledge.Value.attention)
            {
                largestAttention = objectKnowledge.Value.attention;
                attentionedObjectKnowledge = objectKnowledge.Value;
            }
        }

        if (attentionedObjectKnowledge == null) {
            return;
        }
        //Debug.Log("<color=red> Planning: </color>" + attentionedObjectKnowledge.name + " = " + attentionedObjectKnowledge.attention);

        LookBehavior newLookBehavior = new LookBehavior(attentionedObjectKnowledge.position);
        bool isNewLookBehaviorOld = false;
        foreach(Behavior behavior in behaviors){
            if (behavior.GetType() == typeof(LookBehavior) && newLookBehavior.CompareTo((LookBehavior) behavior) == 0)
            {
                isNewLookBehaviorOld = true;
                break;
            }
        }
        if (!isNewLookBehaviorOld)
        {
            //Debug.Log("newLookBehavior.targetPosition = " + newLookBehavior.targetPosition);
            behaviors.Enqueue(newLookBehavior);
        }


        ReachBehavior newReachBehavior = new ReachBehavior(attentionedObjectKnowledge.position);
        bool isNewReachBehaviorOld = false;
        foreach (Behavior behavior in behaviors) {
            if (behavior.GetType() == typeof(ReachBehavior) && newReachBehavior.CompareTo((ReachBehavior)behavior) == 0) {
                isNewReachBehaviorOld = true;
                break;
            }
        }
        if (!isNewReachBehaviorOld) {
            //Debug.Log("newLookBehavior.targetPosition = " + newLookBehavior.targetPosition);
            behaviors.Enqueue(newReachBehavior);
        }
    }
}
