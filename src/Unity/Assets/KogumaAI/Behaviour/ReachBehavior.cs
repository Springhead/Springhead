using UnityEngine;
using System.Collections;

public class ReachBehavior : Behavior {
    public const float percentageDifferenceAllowed = 0.01f;

    public Vector3 targetPosition;

    public ReachBehavior(Vector3 targetPosition) {
        this.targetPosition = targetPosition;
    }

    public int CompareTo(ReachBehavior other) {
        if ((this.targetPosition - other.targetPosition).magnitude < percentageDifferenceAllowed)
        {
            return 0;
        }
        return -1;
    }
}
