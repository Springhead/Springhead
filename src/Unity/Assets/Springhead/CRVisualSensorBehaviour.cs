using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using SprCs;

public class CRVisualSensorBehaviour : SprBehaviourBase{

    public GameObject head;

    public CRVisualSensorIf crVisualSensor = null;
    public CRVisualSensorDescStruct visualSensorDescriptor = null;

    public Vector3 rotateAngleVector;

    public bool debugMode;
    public GameObject visualSensorVisualizer;

    void Reset() {
        SetDLLPath();
        visualSensorDescriptor = new CRVisualSensorDesc();

        visualSensorDescriptor.range = new Vec2d((Math.PI / 180) * (90), (Math.PI / 180) * (60));
        visualSensorDescriptor.centerRange = new Vec2d((Math.PI / 180) * (10), (Math.PI / 180) * (10));
        visualSensorDescriptor.limitDistance = 100;
    }

    void Awake() {
        Posed posed = new Posed();
        Quaternion rotateAngle = Quaternion.Euler(rotateAngleVector);
        posed.w = rotateAngle.w;
        posed.x = rotateAngle.x;
        posed.y = rotateAngle.y;
        posed.z = rotateAngle.z;
        visualSensorDescriptor.pose = posed;

        CRCreatureIf crCreature = gameObject.GetComponentInParent<CRCreatureBehaviour>().GetCRCreature();

        CRVisualSensorDesc d = visualSensorDescriptor;
        //Here needs cast to CRVisualSensorIf
        crVisualSensor = (CRVisualSensorIf)crCreature.CreateEngine(CRVisualSensorIf.GetIfInfoStatic(), d);
        crVisualSensor.SetName("vs:"+gameObject.name);
        //Here depends on the GameObject that has PHSolidBehaviour
        PHSolidIf phSolid = head.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
        crVisualSensor.SetSolid(phSolid);

        if(this.debugMode){
            this.visualSensorDirectionVisualize();
        }
    }

    public void visualSensorDirectionVisualize() {
        
        
        
        Quaternion visualSensorOri = new Quaternion((float)this.crVisualSensor.GetPose().Ori().x
                                                    ,(float)this.crVisualSensor.GetPose().Ori().y
                                                    ,(float)this.crVisualSensor.GetPose().Ori().z
                                                    ,(float)this.crVisualSensor.GetPose().Ori().w);
        Vector3 visualSensorPosition = new Vector3(
                                     (float)this.crVisualSensor.GetSolid().GetCenterPosition().x
                                     ,(float)this.crVisualSensor.GetSolid().GetCenterPosition().y
                                     ,(float)this.crVisualSensor.GetSolid().GetCenterPosition().z);
        GameObject visualizer = (GameObject)Instantiate(visualSensorVisualizer, visualSensorPosition, visualSensorOri);
        visualizer.transform.parent = head.transform;
    }

    void OnValidate()
    {
        SetDLLPath();
    }

    public List<CRVisualInfo> GetVisualSensorData() {
        List<CRVisualInfo> visiblesInfo = new List<CRVisualInfo>();
        crVisualSensor.Update();
        for (int i = 0; i < crVisualSensor.NVisibles(); i++)
        {
            CRVisualInfo info = crVisualSensor.GetVisible(i);
            visiblesInfo.Add(info);
        }
        return visiblesInfo;
    }

    //This is a test function
    public void TestGetVisualSensorData(){
        crVisualSensor.Update();
        for (int i = 0; i < crVisualSensor.NVisibles();i++ )
        {
            CRVisualInfo info = crVisualSensor.GetVisible(i);
            Debug.Log("<color=green>VisualSensor: </color>" + info.solid.GetName());
        }
        crVisualSensor.GetPose();
    }
}
