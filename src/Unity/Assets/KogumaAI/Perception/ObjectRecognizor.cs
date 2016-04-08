using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using SprCs;

public class ObjectRecognizor : Perception {

    public VisualSensor visualSensor = null;
    public TouchSensor touchSensor = null;
    public Dictionary<System.Object, ObjectKnowledge> objectKnowledges = null;

    public float POSITION_LPF_FACTOR_1 = 0.5f;
    public float POSITION_LPF_FACTOR_2 = 0.5f;
    public float CONTACT_FORCE_LPF_FACTOR_1 = 0.3f;
    public float CONTACT_FORCE_LPF_FACTOR_2 = 0.5f;
    public float CONTACT_POSITION_LPF_FACTOR_1 = 1.0f;
    public float CONTACT_POSITION_LPF_FACTOR_2 = 0.5f;
    public float SENSOR_POSITION_LPF_FACTOR = 0.5f;

    public float VELOCITY_TO_ATTENTION_FACTOR = 4.0f;
    public float VELOCITY_SIDE_ATTENTION_FACTOR = 2.0f;
    public float VELOCITY_AWAY_ATTENTION_FACTOR = 1.0f;

    public float CONTACT_POINT_VELOCITY_ATTENTION_FACTOR = 2.0f;
    public float CONTACT_FORCE_ATTENTION_FACTOR = 1.0f;
    public float CONTACT_FORCE_VELOCITY_ATTENTION_FACTOR = 1.0f;

    public void init(VisualSensor visualSensor, TouchSensor touchSensor, Dictionary<System.Object, ObjectKnowledge> objectKnowledges)
    {
        this.visualSensor = visualSensor;
        this.touchSensor = touchSensor;
        this.objectKnowledges = objectKnowledges;
    }

    public void step() {
        this.proccessTouchData();
        this.proccessVisualData();
        
        //Compute the total attention of object knowledge
        foreach(KeyValuePair<System.Object, ObjectKnowledge> objectKnowledge in objectKnowledges){
            objectKnowledge.Value.attention = objectKnowledge.Value.visualAttention + objectKnowledge.Value.touchAttention;
        }
    }

    public void proccessVisualData() {
        List<CRVisualInfo> crVisualInfos = visualSensor.getData();

        if (crVisualInfos == null)
        {
            return;
        }

        //Update visualSensor position using LPF
        Vector3 lastVisualSensorPositionValue = visualSensor.position;
        Vector3 rawVisualSensorPositionValue = new Vector3((float)visualSensor.crVisualSensorBehaviour.crVisualSensor.GetPose().PosX(),
                                                             (float)visualSensor.crVisualSensorBehaviour.crVisualSensor.GetPose().PosY(),
                                                             (float)visualSensor.crVisualSensorBehaviour.crVisualSensor.GetPose().PosZ());
        visualSensor.position = LowPassFilter.getFilteredValue(lastVisualSensorPositionValue, rawVisualSensorPositionValue, SENSOR_POSITION_LPF_FACTOR);

        //Here needs to update objectKnowledges...
        for (int i = 0; i < crVisualInfos.Count; i++)
        {
            CRVisualInfo crVisualInfo = crVisualInfos[i];

            string key = crVisualInfo.solid.ToString();

            //Firstly, try to find the responding object knowledge in memory
            //Here use solid pointer as the id of object knowledge
            if (!objectKnowledges.ContainsKey(key))
            {
                //Add new object into knowledge
                ObjectKnowledge newObjectKnowledge = new ObjectKnowledge();
                newObjectKnowledge.id = crVisualInfo.solid;
                newObjectKnowledge.name = crVisualInfo.solid.GetName();
                objectKnowledges.Add(key, newObjectKnowledge);
                Debug.Log("<color=green> ObjectRecognizor: </color>" + newObjectKnowledge.name);
            }
            //Find existed object knowledge, update visual info
            //Here transform double into float, needs to be discussing
            Vector3 lastPositionValue = objectKnowledges[key].position;
            Vector3 rawPositionValue = new Vector3((float)crVisualInfo.posWorld.x, (float)crVisualInfo.posWorld.y, (float)crVisualInfo.posWorld.z);
            objectKnowledges[key].position = LowPassFilter.getFilteredValue(lastPositionValue, rawPositionValue, POSITION_LPF_FACTOR_1);

            Vector3 lastVelocityValue = objectKnowledges[key].velocity;
            Vector3 rawVelocityValue = Differential.getDifferentiatedValue(lastPositionValue, objectKnowledges[key].position);
            objectKnowledges[key].velocity = LowPassFilter.getFilteredValue(lastVelocityValue, rawVelocityValue, POSITION_LPF_FACTOR_2);

            //Get direction vector from this object to visual sensor
            Vector3 objectToVisualSensorVector = objectKnowledges[key].position - visualSensor.position;
            Vector3 objectToVisualSensorVelocity = Vector3.Project(objectKnowledges[key].velocity, objectToVisualSensorVector);
            //Then compute the velocity towards visual sensor
            Vector3 objectSideVisualSensorVelocity = objectKnowledges[key].velocity - objectToVisualSensorVelocity;

            float visualAttentionTo = 0f;
            float visualAttentionAway = 0f;
            float visualAttentionSide = VELOCITY_SIDE_ATTENTION_FACTOR * objectSideVisualSensorVelocity.magnitude;
            if (Vector3.Angle(objectToVisualSensorVelocity, objectToVisualSensorVector) < 90f)
            {
                visualAttentionTo = VELOCITY_TO_ATTENTION_FACTOR * objectToVisualSensorVelocity.magnitude;
            }
            else
            {
                visualAttentionAway = VELOCITY_AWAY_ATTENTION_FACTOR * objectToVisualSensorVelocity.magnitude;
            }

            objectKnowledges[key].visualAttention = visualAttentionTo + visualAttentionAway + visualAttentionSide;
        }
    }

    public void proccessTouchData() {
        List<CRContactInfo> crContactInfos = touchSensor.getData();

        if (crContactInfos == null)
        {
            return;
        }

        for (int i = 0; i < crContactInfos.Count; i++)
        {
            CRContactInfo crContactInfo = crContactInfos[i];

            string key = crContactInfo.soOther.ToString();

            //Similar as visual info
            //Here soOther means the solid of object
            if (!objectKnowledges.ContainsKey(key))
            {
                //Add new object into knowledge
                ObjectKnowledge newObjectKnowledge = new ObjectKnowledge();
                newObjectKnowledge.id = crContactInfo.soOther;
                newObjectKnowledge.name = crContactInfo.soOther.GetName();
                objectKnowledges.Add(key, newObjectKnowledge);
                Debug.Log("<color=green> ObjectRecognizor: </color>" + newObjectKnowledge.name);
            }
            //Update touch info
            Vector3 lastContactPointValue = objectKnowledges[key].contactPoint;
            Vector3 rawConstactPointValue = new Vector3((float)crContactInfo.pos.x, (float)crContactInfo.pos.y, (float)crContactInfo.pos.z);
            objectKnowledges[key].contactPoint = LowPassFilter.getFilteredValue(lastContactPointValue, rawConstactPointValue, CONTACT_POSITION_LPF_FACTOR_1);

            Vector3 lastContactPointVelocity = objectKnowledges[key].contactPointVelocities.lastestItem;
            Vector3 rawConstactPointVelocity = Differential.getDifferentiatedValue(lastContactPointValue, objectKnowledges[key].contactPoint);
            objectKnowledges[key].contactPointVelocities.Enqueue(LowPassFilter.getFilteredValue(lastContactPointVelocity, rawConstactPointVelocity, CONTACT_POSITION_LPF_FACTOR_2));

            Vector3 lastContactForceValue = objectKnowledges[key].contactForces.lastestItem;
            Vector3 rawContactForceValue = new Vector3((float)crContactInfo.force.x, (float)crContactInfo.force.y, (float)crContactInfo.force.z);
            objectKnowledges[key].contactForces.Enqueue(LowPassFilter.getFilteredValue(lastContactForceValue, rawContactForceValue, CONTACT_FORCE_LPF_FACTOR_1));

            Vector3 lastContactForceVelocity = objectKnowledges[key].contactForceVelocities.lastestItem;
            Vector3 rawConstactForceVelocity = Differential.getDifferentiatedValue(lastContactForceValue, objectKnowledges[key].contactForces.lastestItem);
            objectKnowledges[key].contactPointVelocities.Enqueue(LowPassFilter.getFilteredValue(lastContactForceVelocity, rawConstactForceVelocity, CONTACT_FORCE_LPF_FACTOR_2));

            float touchAttentionPointVelocity = CONTACT_POINT_VELOCITY_ATTENTION_FACTOR * GainAdjuster.getHistoryNormalizedValue(objectKnowledges[key].contactPointVelocities);
            float touchAttentionForce = CONTACT_FORCE_ATTENTION_FACTOR * GainAdjuster.getHistoryNormalizedValue(objectKnowledges[key].contactForces);
            float touchAttentionForceVelocity = CONTACT_FORCE_VELOCITY_ATTENTION_FACTOR * GainAdjuster.getHistoryNormalizedValue(objectKnowledges[key].contactForceVelocities);

            objectKnowledges[key].touchAttention = touchAttentionPointVelocity + touchAttentionForce + touchAttentionForceVelocity;
        }
    }
}
