uniform mat4 blendMatrix[4];

attribute vec4 weights;
attribute vec4 matrixIndices;
attribute vec4 numMatrix;


void main()
{	
	vec4 weight    	= weights;
	vec4 index     	= matrixIndices;
	vec4 position	= vec4( 0.0, 0.0, 0.0, 0.0 );
		
  	for( float i = 0.0; i < numMatrix.x; i += 1.0 )
    {
    	position = position + weight.x * (blendMatrix[int(index.x)] * gl_Vertex);
 		index  = index.yzwx;
   	 	weight = weight.yzwx;    
    }
	
	gl_FrontColor = vec4(1.0, 1.0, 1.0, 1.0);	
	gl_Position = gl_ModelViewProjectionMatrix * position;

}

