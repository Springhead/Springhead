using UnityEngine;
using System.Collections;

public class Differential
{
    public const float dt = 1.0f / 20.0f;

    public static float getDifferentiatedValue(float lastValue, float value){
        return (value - lastValue) / dt;
    }
    public static Vector3 getDifferentiatedValue(Vector3 lastValue, Vector3 value)
    {
        return (value - lastValue) / dt;
    }
}
