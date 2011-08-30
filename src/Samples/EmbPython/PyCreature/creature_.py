#!/usr/bin/env python-
# -*- coding: utf-8 -*

def rad(deg):
    return deg / 180.0 * 3.1415926

def number_of(obj):
    return range(obj) if obj else range(0)

right_eye = crBody.FindByLabel("right_eye")
right_eye.SetViewArea(Posed(1,0,0,0,0,0,0), rad(120), rad(60))

class PerceptionObject:
    def __init__(self, worldsolid, umvelt):
        self.worldsolid = worldsolid
        desc = PHSolidDesc()
        worldsolid.GetDesc(desc)
        self.percsolid  = umvelt.CreateSolid(desc)
        for i in number_of(worldsolid.NShape()):
            self.percsolid.AddShape(worldsolid.GetShape(i))
            self.percsolid.SetShapePose(i, worldsolid.GetShapePose(i))

    def update(self):
        # これやるとなぜか落ちる
        # stat = PHSolidState()
        # self.worldsolid.GetState(stat)
        # self.percsolid.SetState(stat)

        self.percsolid.SetPose(self.worldsolid.GetPose())
        self.percsolid.SetVelocity(self.worldsolid.GetVelocity())


class Perception:
    def __init__(self, world, umvelt):
        self.world  = world
        self.umvelt = umvelt
        self.pObjs  = []
        
    def update(self):
        for i in number_of(right_eye.NVisibleSolids()):
            pObj = None

            solid = right_eye.GetVisibleSolid(i)
            for po in self.pObjs:
                if po.worldsolid.GetName() == solid.GetName():
                    pObj = po

            if pObj == None:
                pObj = PerceptionObject(solid, umvelt)
                self.pObjs.append(pObj)

            pObj.update()

perception = Perception(scene, umvelt)

while True:
    perception.update()

