using UnityEngine;
using System.Collections;

public class ObjectMaker : MonoBehaviour {

    public GameObject phScene;
    public GameObject cubePrefab;

    public void makeCube() {
        GameObject cube = Instantiate(cubePrefab);
        cube.AddComponent<Rigidbody>();
        cube.transform.position = new Vector3(2, 20, -16);
        cube.name = "!!!!!!";
        cube.transform.parent = phScene.transform;
        cube.AddComponent<PHSolidBehaviour>();
        cube.AddComponent<CDBoxBehaviour>();
    }
}
