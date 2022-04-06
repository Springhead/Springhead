using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SprCs {
    public partial class FWSceneIf : SceneIf {
        // InitInNewThreadをした場合、PHSceneなどが自動で作成されるため、CreateCSInstanceを呼ぶ必要がある
        public PHSceneIf GetPHScene() {
            IntPtr ptr = SprExport.Spr_FWSceneIf_GetPHScene((IntPtr)_thisArray[0]);
            if (ptr == IntPtr.Zero) { return null; }
            PHSceneIf obj = PHSceneIf.CreateCSInstance(ptr);
            return obj;
        }
    }
}
