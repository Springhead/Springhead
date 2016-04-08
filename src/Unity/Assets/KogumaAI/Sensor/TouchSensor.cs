using System.Collections;
using System.Collections.Generic;
using SprCs;

public class TouchSensor : Sensor {
    public CRTouchSensorBehaviour crTouchSensorBehaviour = null;
    public BodyKnowledge bodyKnowledge = null;



    public void init(CRTouchSensorBehaviour crTouchSensorBehaviour, BodyKnowledge bodyKnowledge)
    {
        this.crTouchSensorBehaviour = crTouchSensorBehaviour;
        this.bodyKnowledge = bodyKnowledge;

        //Here setting crVisualSensor based on body parameters
        //Here might initiate multiple CRVisualSensors, it depends on bodyKnowledge
    }


    /// <summary>
    /// Get visual data from the springhead visual sensor
    /// </summary>
    /// <returns>
    /// List of visual information
    /// </returns>
    public List<CRContactInfo> getData() {
        if (this.isEnabled && crTouchSensorBehaviour != null)
        {
            return crTouchSensorBehaviour.GetTouchSensorData();
        }
        else
        {
            return null;
        }
    }
}
