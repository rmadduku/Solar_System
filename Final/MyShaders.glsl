// ***************************
// Shader programs to be used in conjunction with the
//  Phong lighting shaders of EduPhong.glsl
// Are first used with Project #6, Math 155A, Winter 2019
//
// Revision: Feb 23, 2019
// ***************************

// #beginglsl ...  #endglsl mark begining and end of code blocks.
// Syntax for #beginglsl is:
//
//   #beginglsl <shader-type> <shader-code-name>
//
// where <shader-type> is
//      vertexshader or fragmentshader or geometryshader or codeblock,
// and <shader-code-name> is used to compile/link the shader into a 
//      shader program.
// A codeblock is meant to be used as a portion of a larger shader.

// *****************************
// applyTextureMap - code block
//    applyTextureMap() is called after the Phong lighting is calculated.
//        - It returns a vec4 color value, which is used as the final pixel color.
//    Inputs: (all global variables)
//        - nonspecColor and specularColor (global variables, vec3 objects)
//        - theTexCoords (the texture coordinates, a vec2 object)
//    Returns a vec4:
//       - Will be used as the final fragment color
// *****************************
#beginglsl codeblock MyProcTexture
// vec3 nonspecColor;		// These items already declared 
// vec3 specularColor;
// vec2 theTexCoords;

uniform sampler2D theTextureMap;	// An OpenGL texture map

bool InFshape( vec2 pos );	// Function prototype

vec4 applyTextureFunction() {
	vec2 wrappedTexCoords = fract(theTexCoords);	// Wrap s,t to [0,1].
	if ( InFshape(wrappedTexCoords) ) {		
		return vec4( 0, 0.0, 0, 1 );                // Black color inside the "F"
	}
	else {
		vec3 combinedPhongColor = nonspecColor+specularColor;
        return vec4(combinedPhongColor, 1.0f);   // Use the Phong light colors
	}
}

// *******************************
// Recognize the interior of an "F" shape
//   Input "pos" contains s,t  texture coordinates.
//   Returns: true if inside the "F" shape.
//            false otherwise
// ******************************

bool InFshape( vec2 pos ) {
	float armWidth = 0.05;       // Width of the F's arms
	if(pos.y <= 0.5+0.5*armWidth && pos.y >= 0.5-0.5*armWidth  )
		return true;
	if(pos.y <= 0.2+0.5*armWidth && pos.y >= 0.2-0.5*armWidth  )
		return true;
	if(pos.y <= 0.8+0.5*armWidth && pos.y >= 0.8-0.5*armWidth  )
		return true;
	if(pos.x <= 0.5+0.5*armWidth && pos.x >= 0.5-0.5*armWidth  )
		return true;
	if(pos.x <= 0.2+0.5*armWidth && pos.x >= 0.2-0.5*armWidth  )
		return true;
	if(pos.x <= 0.8+0.5*armWidth && pos.x >= 0.8-0.5*armWidth  )
		return true;
	return false;
}

#endglsl
