using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using SprCs;

public class VisualSensor : Sensor {

    public CRVisualSensorBehaviour crVisualSensorBehaviour = null;
    public BodyKnowledge bodyKnowledge = null;

    public Vector3 position;

    public void init(CRVisualSensorBehaviour crVisualSensorBehaviour, BodyKnowledge bodyKnowledge) {
        this.crVisualSensorBehaviour = crVisualSensorBehaviour;
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
    public List<CRVisualInfo> getData() {
        if (this.isEnabled && crVisualSensorBehaviour != null)
        {
            //Here removes the body's solids visual info
            List<CRVisualInfo> crVisualInfos =  crVisualSensorBehaviour.GetVisualSensorData();
            for (int i = crVisualInfos.Count; i > 0; i--) {
                //Debug.Log("crVisualInfos[i].ToString() = "+crVisualInfos[i].ToString());
                List<PHSolidIf> bodySolidIfs = bodyKnowledge.getSolids();
                //Debug.Log("bodySolidIfs.Count = "+bodySolidIfs.Count);
                if (bodySolidIfs.Contains(crVisualInfos[i - 1].solid)) {
                    //Debug.Log(crVisualInfos[i - 1].solid.GetName() + "is my body");
                    crVisualInfos.RemoveAt(i-1);
                }
            }
            return crVisualInfos;
        }
        else {
            return null;
        }
    }
	
}
