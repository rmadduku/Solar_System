// ************************
// PhongData.cpp
// ************************


#include "PhongData.h"
#include "EduPhong.h"
#include "LinearR4.h"
#include "GlGeomSphere.h"
#include "GlShaderMgr.h"
#include "TextureProj.h"
#include "MyGeometries.h"

#define GLEW_STATIC
#include "GL\glew.h" 
#include "GLFW\glfw3.h" 
#include "LinearR3.h"		// Adjust path as needed.
#include "MathMisc.h"       // Adjust path as needed
#include "RgbImage.h"

extern phGlobal globalPhongData;

// It is suggested to use
//     myMaterial[0] for your initial
//     myMaterial[1] for the ground plane
//     myMaterial[2] for the surface of rotation
phMaterial myMaterials[3];

// There are (up to) four lights. (EduPhong supports up to eight right now.)
// They are enabled/disabled by TextureProj.cpp code (already written)
// myLights[0], myLights[1], myLights[2] are the three lights above the scene.
// myLights[3] is the spotlight.
phLight myLights[4];

GlGeomSphere myLightSphere(4,4); // Small sphere showing the position of a light.
phMaterial myEmissiveMaterials;   // Use for small spheres showing the location of the lights.
phMaterial materialUndeTexture;

// Suggested positions for the lights. It is OK to change them if it fits in your scene better.
// Especially, you may need to move them higher or lower!
VectorR3 myLightPositions[3] = {
    VectorR3(0.0,0.0, 0.0),
    VectorR3(0.0, 0.0, 0.0),
    VectorR3(0.0, 0.0, 0.0),
};

// Global Lighting parameters
void MySetupGlobalLight()
{
    globalPhongData.NumLights = 4;     // Should be enough lights for most 155A programming projects

    // FEEL FREE TO CHANGE THIS VALUE IF IT HELPS YOUR SCENE LOOK BETTER (E.G. IN LOW LIGHT)
    globalPhongData.GlobalAmbientColor.Set(0.01, 0.01, 0.01);
    globalPhongData.LoadIntoShaders();
}

/*unsigned int TextureName[NumTextures];     // Texture names generated by OpenGL
const char* TextureFile[NumTextures] = {
    "sun.bmp",
    "mercury.bmp",
    "venus.bmp",
    "earth_clouds.bmp",
    "moon.bmp",
    "mars.bmp",
    "belt.bmp",
    "jupitermap.bmp",
    "saturn.bmp",
    "rings.bmp",
    "neptune.bmp",
    "uranus.bmp"
};
void SetupForTextures2()
{
    // This material goes under the textures.
    // IF YOU WISH, YOU MAY DEFINE MORE THAN ONE OF THESE FOR DIFFERENT GEOMETRIES
    materialUndeTexture.SpecularColor.Set(0.9, 0.9, 0.9);
    materialUndeTexture.AmbientColor.Set(0.3, 0.3, 0.3);
    materialUndeTexture.DiffuseColor.Set(0.7, 0.7, 0.7);       // Increase or decrease to adjust brightness
    materialUndeTexture.SpecularExponent = 40.0;

    // ***********************************************
    // Load texture maps
    // ***********************************************
    RgbImage texMap;

    glUseProgram(shaderProgramBitmap);
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(NumTextures, TextureName);
    for (int i = 0; i < NumTextures; i++) {
        texMap.LoadBmpFile(TextureFile[i]);            // Read i-th texture from the i-th file.
        glBindTexture(GL_TEXTURE_2D, TextureName[i]);  // Bind (select) the i-th OpenGL texture

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set best quality filtering.   Also see below for disabling mipmaps.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // Requires that mipmaps be generated (see below)
        // You may also try GL_LINEAR_MIPMAP_NEAREST -- try looking at the wall from a 30 degree angle, and look for sweeping transitions.

        // Store the texture into the OpenGL texture named TextureNames[i]
        int textureWidth = texMap.GetNumCols();
        int textureHeight = texMap.GetNumRows();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texMap.ImageData());
#if 1
        // Use mipmaps  (Best!)
        glGenerateMipmap(GL_TEXTURE_2D);
#else
        // Don't use mipmaps.  Try moving away from the brick wall a great distance
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#endif

    }

    // Make sure that the shaderProgramBitmap uses the GL_TEXTURE_0 texture.
    glUseProgram(shaderProgramBitmap);
    glUniform1i(glGetUniformLocation(shaderProgramBitmap, "theTextureMap"), 0);
    glActiveTexture(GL_TEXTURE0);


}
*/


// Gets called a lot since Position needs updating time view changes.
void MySetupLights()
{

    myLightSphere.InitializeAttribLocations(vertPos_loc, vertNormal_loc, vertTexCoords_loc);
    myLightSphere.Remesh(meshRes, meshRes);
    // First light (light #0).
    myLights[0].AmbientColor.Set(0.4, 0.4, 0.2);    // Gray color
    myLights[0].DiffuseColor.Set(0.4, 0.4, 0.2);      // Very light gray
    myLights[0].SpecularColor.Set(0.9,0.9,0.5);   // Very light gray
    myLights[0].IsEnabled = true;                   // BE SURE TO ENABLE YOUR LIGHTS

    // Second light (light #1)
    myLights[1].AmbientColor.Set(0.4, 0.4, 0.2);    // Yellow color
    myLights[1].DiffuseColor.Set(0.4, 0.4, 0.2);    // Yellow
    myLights[1].SpecularColor.Set(0.9, 0.9, 0.5);   // White
    myLights[1].IsEnabled = true;                     // BE SURE TO ENABLE YOUR LIGHTS

    // Third light (light #2)
    myLights[2].AmbientColor.Set(0.4, 0.4, 0.2);    // Yellow color
    myLights[2].DiffuseColor.Set(0.4, 0.4, 0.2);      // Very light gray
    myLights[2].SpecularColor.Set(0.9, 0.9, 0.5);   // Very light gray
    myLights[2].IsEnabled = true;                     // BE SURE TO ENABLE YOUR LIGHTS

    myLights[3].DiffuseColor.Set(0.6, 0.6, 0.6);      // Very light gray
    myLights[3].SpotCosCutoff = 0.95f;
    myLights[3].IsSpotLight = true;
    myLights[3].SpotExponent = 1.0f;
    myLights[3].IsEnabled = true;                   // BE SURE TO ENABLE YOUR LIGHTS
    

}

void LoadAllLights() 
{
    myLights[0].SetPosition(viewMatrix, myLightPositions[0]);
    myLights[0].LoadIntoShaders(0); 
    myLightSphere.Remesh(meshRes, meshRes);

    /*/myLights[1].SetPosition(viewMatrix, myLightPositions[1]);
    myLights[1].LoadIntoShaders(1);  

    myLights[2].SetPosition(viewMatrix, myLightPositions[2]);
    myLights[2].LoadIntoShaders(2);  

    myLights[3].SetPosition(viewMatrix, VectorR3(0.0, 6.0, 4.0));
    myLights[3].SetSpotlightDirection(viewMatrix, VectorR3(0.0, -1.0, -0.5));
    myLights[3].LoadIntoShaders(3);
    */
}

// *******************************************
// In this routine, you must set the material properties for your three surfaces.
// Make the Emissive Color values ALL EQUAL TO ZERO.
// It is suggested that Ambient and Diffuse colors for a material are equal to each other, 
//         or at least are scalar multiples of each other.
// It is suggested that the Specular Color is a white or gray.
// *******************************************
// FEEL FREE TO CHANGE THESE VALUES FOR YOUR PROGRAMMING PROJECT!!
void MySetupMaterials()
{
    // myMaterials[0]: Material for the initial
    //myMaterials[0].EmissiveColor.Set(0.4f, 0.9f, 0.4f);   // THIS SHOULD CHANGE TO 0,0,0 SO IT HAS NO EMISSION
    myMaterials[0].AmbientColor.Set(0.2f, 0.45f, 0.2f);
    myMaterials[0].DiffuseColor.Set(0.2f, 0.45f, 0.2f);
    myMaterials[0].SpecularColor.Set(1.0,1.0,1.0);
    myMaterials[0].SpecularExponent = 50.0;

    // myMaterials[1]: Material for the ground plane
    // myMaterials[1].EmissiveColor.Set(1.0f, 0.4f, 0.4f);   // THIS SHOULD CHANGE TO 0,0,0 SO IT HAS NO EMISSION
    /*myMaterials[1].AmbientColor.Set(0.5f, 0.2f, 0.2f);
    myMaterials[1].DiffuseColor.Set(1.0f, 0.4f, 0.4f);
    myMaterials[1].SpecularColor.Set(0.8, 0.8, 0.8);
    myMaterials[1].SpecularExponent = 75.0;

    // myMaterials[2]: Material for the surface of rotation
    //myMaterials[2].EmissiveColor.Set(1.0f, 0.8f, 0.4f);   // THIS SHOULD CHANGE TO 0,0,0  SO IT HAS NO EMISSION
    myMaterials[2].AmbientColor.Set(0.5f, 0.4f, 0.2f);
    myMaterials[2].DiffuseColor.Set(1.0f, 0.8f, 0.4f);
    myMaterials[2].SpecularColor.Set(1.0,1.0,1.0);
    myMaterials[2].SpecularExponent = 50.0;
    */
    

}

// Purely emissive spheres showing placement of the light[0]
// Use the light's diffuse color as the emissive color
// Use the light's position as the sphere's position
void MyRenderSpheresForLights() {
   float matEntries[16];	// Holds 16 floats (since cannot load doubles into a shader that uses floats)
   phMaterial myEmissiveMaterial;
   myLightSphere.Remesh(meshRes, meshRes);
   for (int i = 0; i < 1; i++) {
        if (myLights[i].IsEnabled) {
            LinearMapR4 modelviewMat = viewMatrix;
            modelviewMat.Mult_glTranslate(myLightPositions[i].x, myLightPositions[i].y,myLightPositions[i].z);
# if 1
            modelviewMat.Mult_glScale(20);
#else
            modelviewMat.Mult_glScale(4);
#endif
            modelviewMat.DumpByColumns(matEntries);
            glUniformMatrix4fv(modelviewMatLocation, 1, false, matEntries);
            //myEmissiveMaterial.EmissiveColor = myLights[i].DiffuseColor;
            myEmissiveMaterial.EmissiveColor = VectorR3 (1,1,1);
            glBindTexture(GL_TEXTURE_2D, TextureNames[0]);     // Choose sun image texture
            glUniform1i(applyTextureLocation, true);           // Enable applying the texture!
            myEmissiveMaterial.LoadIntoShaders();
            myLightSphere.Render();
            glUniform1i(applyTextureLocation, false);           // disable applying the texture!
        }
    }
}