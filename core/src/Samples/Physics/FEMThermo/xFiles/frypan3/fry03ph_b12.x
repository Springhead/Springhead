xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh phFryPan3_b12{
 16;
 -0.08539;0.01161;0.17395;,
 -0.17763;0.01161;0.08060;,
 -0.15660;-0.03576;0.06576;,
 -0.07136;-0.03576;0.15224;,
 -0.15189;0.01062;0.06697;,
 -0.07180;0.01062;0.14810;,
 -0.05374;-0.02790;0.11682;,
 -0.12062;-0.02790;0.04892;,
 -0.08539;0.01161;0.17395;,
 -0.07180;0.01062;0.14810;,
 -0.15189;0.01062;0.06697;,
 -0.17763;0.01161;0.08060;,
 -0.07136;-0.03576;0.15224;,
 -0.15660;-0.03576;0.06576;,
 -0.12062;-0.02790;0.04892;,
 -0.05374;-0.02790;0.11682;;
 
 4;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;;
 
 MeshMaterialList {
  6;
  4;
  3,
  3,
  3,
  3;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   90.000000;
   0.940000;0.940000;0.940000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.555200;0.548800;0.564800;1.000000;;
   90.000000;
   0.940000;0.940000;0.940000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.796800;0.772000;0.800000;1.000000;;
   19.000000;
   0.230000;0.230000;0.230000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "tex\\mokume01s.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.733980;0.733980;0.733980;1.000000;;
   6.000000;
   0.260000;0.260000;0.260000;;
   0.178790;0.178790;0.178790;;
   TextureFilename {
    "negiyoko_tex3d_0.png";
   }
  }
  Material {
   0.705750;0.705750;0.705750;1.000000;;
   8.000000;
   0.360000;0.360000;0.360000;;
   0.141150;0.141150;0.141150;;
   TextureFilename {
    "negi_around31.PNG";
   }
  }
 }
 MeshNormals {
  5;
  -0.627771;-0.471264;0.619527;,
  0.300294;0.906715;-0.296128;,
  0.527879;0.671123;-0.520516;,
  0.025369;0.999364;-0.025056;,
  0.146330;-0.978674;-0.144171;;
  4;
  4;0,0,0,0;,
  4;1,1,2,2;,
  4;3,1,1,3;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  16;
  0.500000;0.000000;,
  0.583330;0.000000;,
  0.583330;1.000000;,
  0.500000;1.000000;,
  0.583330;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.583330;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
