using UnityEngine;
using System.Collections;
using SprCs;

public class CRCreatureBehaviour : SprSceneObjBehaviour{

    public CRCreatureDescStruct desc = null;

    static CRSdkIf crSdk = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as CRCreatureDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as CRCreatureDescStruct).ApplyTo(to as CRCreatureDesc);
    }

    public override CsObject CreateDesc() {
        return new CRCreatureDesc();
    }

    public override ObjectIf Build() {
        if (crSdk == null) {
            crSdk = CRSdkIf.CreateSdk();
        }
        CRCreatureDesc d = desc;
        CRCreatureIf crCreature = crSdk.CreateCreature(CRCreatureIf.GetIfInfoStatic(), d);
        crCreature.SetName("cr:" + gameObject.name);

        //PHSceneIf phScene = gameObject.GetComponentInParent<PHSceneBehaviour>().GetPHScene();
        crCreature.AddChildObject(phScene);

        return crCreature;
    }

    void FixedUpdate()
    {
        if (this.sprObject as CRCreatureIf != null)
        {
            (this.sprObject as CRCreatureIf).Step();
        }
    }

    public CRCreatureIf GetCRCreature() { return this.sprObject as CRCreatureIf; }
}
