Description Of Your Entry:

This is a smart texture magnification filtering shader program. It specialize in textures that contain several color-uniform areas with apparent borders between them(e.g., font & symbol textures, npr textures, etc). Its original idea belongs to Maxim Stepin(http://www.hiend3d.com/smartflt.html). In his site, you can find the original algorithm sketch and a executable & source code based on C++ and assembly. It basically uses 4 neighbor samples and 15 possible filter kernel patterns(Maxim's original idea used 14 patterns. I added 1 pattern for generally more pleasing result).

I used OpenGL Shading Language and OpenGL Shader Designer 1.5.4(http://www.typhoonlabs.com/). Filter kernel patterns can be precomputed and saved into alpha channel(a lot more faster) or determined dynamically at run-time by a shader logic(slow, but no preprocessing required and alpha channel can be used for other purposes). So there are 2 projects for each case(smartTextureFilteringPreprocessed.gdp and smartTextureFiltering.gdp). After installing OpenGL Shader Designer 1.5.4, copy shaders & textures folders to where Shader Designer has been installed(e.g. C:\Program Files\TyphoonLabs\ShaderDesigner\). Open one of forementioned project files and see how the filtering works as you zoom in.

There are also 3 utiliy programs developed by Visual C++ 6.0 in the smartTextureFilterUtils folder. createSmartLUT & createZoneLUT can be used to generate slut.tga & zlut.tga texture images in the textures folder respectively. slut.tga is a 2D look-up table which returns filter tap weights based on (sub-texel zone index, filter kernel pattern index(run-time determined or precomputed)) input pair. zlut.tga is a 2D look-up table for an efficient determination of the sub-texel zone index(This can be determined by shader operations also. So there is a version of function which does not use a texture also). preprocess.exe is for preprocessing a image so that no costly run-time filter kernel determination are needed. These util programs use DevIL(http://openil.sourceforge.net/) for image manipulations.

These shaders are tested on Radeon X800 PRO and GeforceFX 5700 LE and runned OK.

<Smart texture filtering algorithms>
- By using a given texture coordinate, samples four adjacent texels.
- Calculate several color variances and by comparing them to a threshold value determine a proper filter kernel pattern among following 15 possible cases.
// 0
// *-------*
// |       |
// |       |
// |       |
// *-------*
//

// 1
// *-------*
// |       |
// |-------|
// |       |
// *-------*
//

// 2
// *-------*
// |   |   |
// |   |   |
// |   |   |
// *-------*
//

// 3
// *-------*
// |       |
// |      /|
// |    /  |
// *-------*
//

// 4
// *-------*
// |       |
// |\      |
// |  \    |
// *-------*
//

// 5
// *-------*
// |  /    |
// |/      |
// |       |
// *-------*
//

// 6
// *-------*
// |    \  |
// |      \|
// |       |
// *-------*
//

// 7
// *-------*
// |   |   |
// |-------|
// |       |
// *-------*
//

// 8
// *-------*
// |       |
// |-------|
// |   |   |
// *-------*
//

// 9
// *-------*
// |   |   |
// |---|   |
// |   |   |
// *-------*
//

// 10
// *-------*
// |   |   |
// |   |---|
// |   |   |
// *-------*
//

// 11
// *-------*
// |    \  |
// |\     \|
// |  \    |
// *-------*
//

// 12
// *-------*
// |  /    |
// |/     /|
// |    /  |
// *-------*
//

// 13
// *-------*
// |   |   |
// |---|---|
// |   |   |
// *-------*
//

// 14
// *-------*
// |  / \  |
// |/     /|
// | \  /  |
// *-------*
//

!! There can be an ambiguity between 11 & 12 patterns unless you use some more samples. So if you just give some random preference over 11 or 12, it will induce some holes when a one-pixel width diagonal line is magnified. That's why the last kernel(14) is added to the original 14-pattern scheme.

- Determine a subtexel zone index by using the following layout.
// zone layout
// *-----------*
// | 0 / | \ 3 |
// | / 4 | 7 \ |
// |-----|-----|
// | \ 5 | 6 / |
// | 1 \ | / 2 |
// *-----------*
//

- By using the subtexel zone index & the pattern index, determine weight values for 4 samples.
- Considering weight values, combine 4 colors bilinearly and output the result as final color.

<Utils usage>
- createZoneLUT.exe
Just run, then there will be zlut.tga.
- createSmartLUT.exe
Just run, then there will be slut.tga.
- preprocess.exe [imagename]
Run with a image file, then there will be preprocessed.tga which is the preprocessed version of the input image.