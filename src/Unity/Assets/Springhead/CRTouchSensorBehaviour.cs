using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using SprCs;

public class CRTouchSensorBehaviour : SprBehaviourBase
{
    public CRTouchSensorIf crTouchSensor = null;
    public CRTouchSensorDescStruct touchSensorDescriptor = null;

    void Reset()
    {
        SetDLLPath();

        touchSensorDescriptor = new CRTouchSensorDesc();
    }

    void Awake()
    {
        CRCreatureIf crCreature = gameObject.GetComponentInParent<CRCreatureBehaviour>().GetCRCreature();

        CRTouchSensorDesc d = touchSensorDescriptor;
        //Here needs cast to CRVisualSensorIf
        crTouchSensor = (CRTouchSensorIf)crCreature.CreateEngine(CRTouchSensorIf.GetIfInfoStatic(), d);
        crTouchSensor.SetName("ts:" + gameObject.name);

        //Here adds all bones to this body from this GameObject's childs
        //Search all childs
        Transform[] childGameObjects = this.gameObject.GetComponentsInChildren<Transform>();
        for (int i = 0; i < childGameObjects.Length; i++)
        {
            //Dont get itself
            if (childGameObjects[i] == this.gameObject.transform)
            {
                //Debug.Log("<color=blue>Body: </color>" + childGameObjects[i].gameObject.name);
                continue;
            }

            //real childs
            //If it has PHSolidBehaviour component, it is a valid bone, otherwise not
            if (childGameObjects[i].gameObject.GetComponent<PHSolidBehaviour>() == null)
            {
                //Debug.Log("<color=blue>Body: </color>" + childGameObjects[i].gameObject.name);
                continue;
            }

            //Add solid
            PHSolidIf phSolid = childGameObjects[i].gameObject.GetComponent<PHSolidBehaviour>().sprObject as PHSolidIf;
            crTouchSensor.AddChildObject(phSolid);

            //Debug.Log("<color=yellow>#: </color>" + i);
        }
    }

    void OnValidate()
    {
        SetDLLPath();
    }

    public List<CRContactInfo> GetTouchSensorData()
    {
        List<CRContactInfo> contactsInfo = new List<CRContactInfo>();
        crTouchSensor.Update();
        for (int i = 0; i < crTouchSensor.NContacts(); i++)
        {
            CRContactInfo info = crTouchSensor.GetContact(i);
            contactsInfo.Add(info);
        }
        return contactsInfo;
    }

    //This is a test function
    public void TestGetTouchSensorData()
    {
        crTouchSensor.Update();
        for (int i = 0; i < crTouchSensor.NContacts(); i++)
        {
            CRContactInfo info = crTouchSensor.GetContact(i);
            Debug.Log("<color=yellow>TouchSensor: </color>" + info.soOther.GetName());
        }
    }
}
