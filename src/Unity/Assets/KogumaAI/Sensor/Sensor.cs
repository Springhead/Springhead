using UnityEngine;
using System.Collections;

public class Sensor : KnowledgeSource {

	public bool isEnabled = true;

	public void enable(bool enable){
		this.isEnabled = enable;
	}

    public void getData() { }
}
