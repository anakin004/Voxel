#include "Perlin.h"
#include <iostream>


#define M_PI       3.14159265358979323846   // pi


vector2 randomGradient(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix + iy * 71; // Introduced y to make it more varied
    a *= 3284157443;

    unsigned b = a ^ (a >> s);
    b *= 1911520717;

    a ^= (b << s) | (b >> (w - s));
    a *= 2048419325;

    float random = (a & 0xFFFF) / static_cast<float>(0xFFFF); // Normalize to [0, 1]
    vector2 v;
    v.x = cos(random * 2 * M_PI); // Generate gradient in unit circle
    v.y = sin(random * 2 * M_PI);

    return v;
}

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}

float dotGridGradient(int ix, int iy, float x, float y) {
    vector2 gradient = randomGradient(ix, iy);

    // Distance vector
    float dx = x - ix;
    float dy = y - iy;

    return (dx * gradient.x + dy * gradient.y);
}

float perlin(float x, float y) {
    
    x /= 256.f;
    y /= 256.f;

    int x0 = static_cast<int>(floor(x));
    int y0 = static_cast<int>(floor(y));

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Interpolation weights
    float sx = x - x0;
    float sy = y - y0;

    // Interpolate between bottom two corners
    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    // Interpolate between top two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sy);

    // Final interpolation between the two previously interpolated values
    // 
      return interpolate(ix0, ix1, sy);
}

float generatePerlinNoise(int worldX, int worldZ) {
    float val = 0.0f;
    float freq = 1.0f;
    float amp = 1.0f;
    float totalAmp = 0.0f;

    for (int i = 0; i < 8; i++) { // Octaves
        val += pnoise2(worldX/32.f * freq, worldZ/32.f * freq, 16, 16) * amp; 
        totalAmp += amp;

        freq *= 2.0f;
        amp *= 0.5f;  // Decrease amplitude over time
    }

    if (totalAmp > 0) { 
        val /= totalAmp;
    }

    // Adjust the output to desired range
    //int finalHeight = (val + 1.0f) * 0.5f * 30.0f; // Scale appropriately
    int finalHeight = val * 25.0f; // Scale appropriately

    return (float)finalHeight;
}