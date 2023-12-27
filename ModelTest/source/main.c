#include <NEMain.h>

// First you have to put the .bin files in the data folder. This will generate
// (after doing "make") some files named "binfilename_bin.h". For example,
// "model.bin" will generate a file named "model_bin.h". You have to include
// this in "main.c".
//
// The name you will have to use is "binfilename_bin". For example, for loading
// "model.bin" you will have to use:
//
//     NE_ModelLoadStaticMesh(Model, (u32 *)binfilename_bin);
//
#include "object_bin.h"
#include "texture_tex_bin.h"

int angle = 0;

NE_Camera *Camera;
NE_Model *Model;
NE_Material *Material;

void Draw3DScene(void)
{
    NE_CameraUse(Camera);
    NE_ModelDraw(Model);
}

int main(void)
{
    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_HBLANK, NE_HBLFunc);

    // Init Nitro Engine in normal 3D mode
    NE_Init3D();

    // libnds uses VRAM_C for the text console, reserve A and B only
    NE_TextureSystemReset(0, 0, NE_VRAM_AB);
    // Init console in non-3D screen
    consoleDemoInit();

    // Allocate space for the objects we'll use
    Model = NE_ModelCreate(NE_Static);
    Camera = NE_CameraCreate();
    Material = NE_MaterialCreate();

    // Set coordinates for the camera
    NE_CameraSet(Camera,
                 0, 0, 1,  // Position
                  0, 0, 0,  // Look at
                  0, 1, 0); // Up direction

    // Load mesh from RAM and assign it to the object "Model".
    NE_ModelLoadStaticMesh(Model, (u32 *)object_bin);
    // Load a RGB texture from RAM and assign it to "Material".
    NE_MaterialTexLoad(Material, NE_RGB5, 32, 32, NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T,
                       (u8 *)texture_tex_bin);

    // Assign texture to model...
    NE_ModelSetMaterial(Model, Material);

    // Assign Rotation
    NE_ModelRotate(Model, 0,0,0);

    // We set up a light and its color
    NE_LightSet(0, NE_White, -0.5, -0.5, -0.5);

    while (1)
    {
        // Wait for next frame
        NE_WaitForVBL(0);

        // Get keys information
        scanKeys();
        uint32_t keys = keysHeld();

        // Rotate model using the pad
        if (keys & KEY_UP && angle > -92){
            angle -= 3;
            NE_CameraRotateFree(Camera, -3, 0, 0);
        }
        else if (keys & KEY_DOWN && angle < 92){
            angle += 3;
            NE_CameraRotateFree(Camera, 3, 0, 0);
        }
        if (keys & KEY_LEFT)
            NE_CameraRotateFree(Camera, 0, -3, 0);
        else if (keys & KEY_RIGHT)
            NE_CameraRotateFree(Camera, 0, 3, 0);

        //move forward and back
        if (keys & KEY_A)
            NE_CameraMoveFree(Camera, 0.05, 0, 0);
        else if (keys & KEY_B)
            NE_CameraMoveFree(Camera, -0.05, 0, 0);

        printf("\x1b[0;0HPad: Rotate.\nA/B: Move forward/back.");

        NE_Process(Draw3DScene);
    }

    return 0;
}
