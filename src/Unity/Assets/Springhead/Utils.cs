using UnityEngine;
using System.Collections;

public class SpringheadBehaviour : MonoBehaviour {
    [System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
    private static extern bool SetDllDirectory(string lpPathName);

    protected void SetDLLPath() {
        SetDllDirectory(Application.dataPath + "/Assets/Springhead/Plugins");
        SetDllDirectory(Application.dataPath + "/Springhead/Plugins");
    }
}
