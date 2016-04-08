using UnityEngine;
using System.Collections;

public class LowPassFilter
{
    public float factor;

    public LowPassFilter(float factor){
        this.factor = factor;
    }

    public float getFilteredValue(float lastValue, float rawValue)
    {
        if (lastValue == 0f)
        {
            return rawValue;
        }
        return this.factor * lastValue + (1.0f - this.factor) * rawValue;
    }
    public Vector3 getFilteredValue(Vector3 lastValue, Vector3 rawValue)
    {
        if (lastValue == null)
        {
            return rawValue;
        }
        return this.factor * lastValue + (1.0f - this.factor) * rawValue;
    }

    public static float getFilteredValue(float lastValue, float rawValue, float factor) {
        if (lastValue == 0f)
        {
            return rawValue;
        }
        return factor * lastValue + (1.0f - factor) * rawValue;
    }
    public static Vector3 getFilteredValue(Vector3 lastValue, Vector3 rawValue, float factor)
    {
        if (lastValue == null)
        {
            return rawValue;
        }
        return factor * lastValue + (1.0f - factor) * rawValue;
    }
}
