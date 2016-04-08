using UnityEngine;
using System;
using System.Collections;


public class ObjectKnowledge : Knowledge {

    public const int MAX_HISTORY_COUNT = 100;

    public System.Object id;
    public string name;
    public string category;
    public Vector3 position;
    public Vector3 velocity;
    public Vector3 orientation;


    //Springhead data paramter
    public Vector3 contactPoint;
    public FixedSizedQueue<Vector3> contactForces = new FixedSizedQueue<Vector3>(MAX_HISTORY_COUNT);
    public FixedSizedQueue<Vector3> contactPointVelocities = new FixedSizedQueue<Vector3>(MAX_HISTORY_COUNT);
    public FixedSizedQueue<Vector3> contactForceVelocities = new FixedSizedQueue<Vector3>(MAX_HISTORY_COUNT);

    //Specified AI paramter
    public float visualAttention;
    public float touchAttention;
    public float attention;
}
