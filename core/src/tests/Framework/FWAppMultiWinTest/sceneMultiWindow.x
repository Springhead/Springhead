xof 0302txt 0064
FWScene fwScene1{
  PHScene phScene1{
    0.01;0;;
    0;-9.8;0;;
    20;
    PHSolid soBlock1{
      0;0;0;;
      0;0;0;;
      1;0;0;0;0;-0.5;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      ;
      20;1,0,0,0,1,0,0,0,1;;
      0;0;0;;
      FALSE;
      PHFrame{
        1;0;0;0;0;0;0;;
        CDBox cdBox1{
          0.4;0.4;0.4;1;;
          ;
          1;1;1;;
        }
      }
    }
  }
  GRScene grScene1{
    GRFrame world_frame0{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1;;
      GRFrame grFrameWorld1{
        ;
        1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
      }
    }
    GRLight light1{
      ;
      0.9;0.9;0.9;1;;
      0.5;0.5;0.5;1.9;;
      0.1;0.1;0.1;1;;
      1;1;1;0;;
      10000;1;0;0;0;0;0;;
      0;0;0;
    }
    GRFrame grFrameBlock1{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
    }
  }
  FWObject fwBlock1{
    {soBlock1}
    {grFrameBlock1}
  }
}
FWScene fwScene2{
  PHScene phScene2{
    0.01;0;;
    0;-9.8;0;;
    20;
    PHSolid soSphere2{
      0;0;0;;
      0;0;0;;
      1;0;0;0;0;-0.5;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      ;
      20;1,0,0,0,1,0,0,0,1;;
      0;0;0;;
      FALSE;
      PHFrame{
        1;0;0;0;0;0;0;;
        CDSphere cdSphere2{
          0.4;0.4;0.4;1;;
          ;
          1;
        }
      }
    }
  }
  GRScene grScene2{
    GRFrame world_frame0{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1;;
      GRFrame grFrameWorld2{
        ;
        1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
      }
    }
    GRLight light2{
      ;
      0.9;0.9;0.9;1;;
      0.5;0.5;0.5;1.9;;
      0.1;0.1;0.1;1;;
      1;1;1;0;;
      10000;1;0;0;0;0;0;;
      0;0;0;
    }
    GRFrame grFrameSphere2{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
    }
  }
  FWObject fwSphere2{
    {soSphere2}
    {grFrameSphere2}
  }
}
FWScene fwScene3{
  PHScene phScene3{
    0.01;0;;
    0;-9.8;0;;
    20;
    PHSolid soCapsule3{
      0;0;0;;
      0;0;0;;
      1;0;0;0;0;-0.5;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      0;0;0;;
      ;
      20;1,0,0,0,1,0,0,0,1;;
      0;0;0;;
      FALSE;
      PHFrame{
        1;0;0;0;0;0;0;;
        CDCapsule cdCapsule3{
          0.4;0.4;0.4;1;;
          ;
          1;1;
        }
      }
    }
  }
  GRScene grScene3{
    GRFrame world_frame0{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1;;
      GRFrame grFrameWorld3{
        ;
        1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
      }
    }
    GRLight light3{
      ;
      0.9;0.9;0.9;1;;
      0.5;0.5;0.5;1.9;;
      0.1;0.1;0.1;1;;
      1;1;1;0;;
      10000;1;0;0;0;0;0;;
      0;0;0;
    }
    GRFrame grFrameCapsule3{
      ;
      1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1;;
    }
  }
  FWObject fwCapsule3{
    {soCapsule3}
    {grFrameCapsule3}
  }
}
