using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GainAdjuster{
    public static float getHistoryNormalizedValue(FixedSizedQueue<Vector3> items){
        Vector3 maxItem = new Vector3();
        foreach (Vector3 item in items)
        {
            if (maxItem.magnitude < item.magnitude)
            {
                maxItem = item;
            }
        }
        //Prevent from being NaN
        if (maxItem.magnitude == 0f)
        {
            return 0.01f;
        }
        return items.lastestItem.magnitude / maxItem.magnitude;
    }
}
