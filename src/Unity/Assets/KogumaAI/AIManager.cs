using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AIManager : MonoBehaviour {

    //Knowledge (or Memory?) section
    BodyKnowledge bodyKnowledge = null;
    Dictionary<System.Object, ObjectKnowledge> objectKnowledges = null;
    Queue<Behavior> behaviors = null; 
    //List<ObjectKnowledge> objectKnowledges = null;

    //Knowledge source section
    VisualSensor visualSensor = null;
    TouchSensor touchSensor = null;
    ObjectRecognizor objectRecognizor = null;
    Planning planning = null;
    LookController lookController = null;
    ReachController reachController = null;

    public void init(){
        bodyKnowledge = new BodyKnowledge();
        objectKnowledges = new Dictionary<System.Object, ObjectKnowledge>();
        behaviors = new Queue<Behavior>();

        visualSensor = new VisualSensor();
        touchSensor = new TouchSensor();

        objectRecognizor = new ObjectRecognizor();

        planning = new Planning();
        lookController = new LookController();
        reachController = new ReachController();

        //Get Springhead entry, initiate each responding parts
        CRBodyBehaviour crBodyBehaviour = gameObject.GetComponentInChildren<CRBodyBehaviour>();
        bodyKnowledge.init(crBodyBehaviour);


        CRVisualSensorBehaviour crVisualSensorBehaviour = gameObject.GetComponentInChildren<CRVisualSensorBehaviour>();
        visualSensor.init(crVisualSensorBehaviour, bodyKnowledge);


        CRTouchSensorBehaviour crTouchSensorBehaviour = gameObject.GetComponentInChildren<CRTouchSensorBehaviour>();
        touchSensor.init(crTouchSensorBehaviour, bodyKnowledge);

        
        objectRecognizor.init(visualSensor, touchSensor, objectKnowledges);


        planning.init(bodyKnowledge, objectKnowledges ,behaviors);

        CRLookControllerBehaviour crLookControllerBehaviour = gameObject.GetComponentInChildren<CRLookControllerBehaviour>();
        lookController.init(crLookControllerBehaviour, behaviors);

        CRReachControllerBehaviour crReachControllerBehaviour = gameObject.GetComponentInChildren<CRReachControllerBehaviour>();
        reachController.init(crReachControllerBehaviour, behaviors);
        //Other init
    }

    /// <summary>
    /// step the whole AI module
    /// </summary>
    public void step() {
        visualSensor.step();
        touchSensor.step();
        objectRecognizor.step();
        planning.step();
        lookController.step();
        reachController.step();
    }

    void Awake() {
        init();
    }

    void FixedUpdate() {
        step();
    }

    public void TestAIModule(){
        Debug.Log("AIManager: I remember these objects: ");
        foreach (KeyValuePair<System.Object, ObjectKnowledge> objectKnowledge in objectKnowledges)
        {
            Debug.Log("[" + objectKnowledge.Value.name + ": " + objectKnowledge.Value.attention + "] ");
        }
    }
}
