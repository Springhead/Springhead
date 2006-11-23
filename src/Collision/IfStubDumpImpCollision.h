//	Do not edit. MakeTypeDesc.bat will update this file.
IF_IMP(CDShape, NamedObject);
IF_IMP(CDConvex, CDShape);
IF_IMP_BASE(CDFace);
IF_IMP(CDConvexMesh, CDConvex);
IF_IMP(CDSphere, CDConvex);
IF_IMP(CDCapsule, CDConvex);
IF_IMP(CDBox, CDConvex);
