#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float pixelSize;
uniform vec2 resolution;

// Optional: adjust saturation of final color
vec3 sat(vec3 rgb, float adjustment) {
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

void main()
{
    //// Convert pixelSize into normalized screen units
    //vec2 normalizedPixelSize = pixelSize / resolution;
    //vec2 uv = TexCoords;
    //
    //// Get pixel block coordinates
    //vec2 uvPixel = normalizedPixelSize * floor(uv / normalizedPixelSize);
    //
    //// Sample base color
    //vec4 color = texture(screenTexture, uvPixel);
    //
    //// Compute brightness (luminance)
    //float luma = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    //
    //// Determine the UV position inside the pixel block
    //vec2 cellUV = fract(uv / normalizedPixelSize);
    //float lineWidth = 0.0;
    //
    //// Map luminance to line width
    //if (luma > 0.0) lineWidth = 1.0;
    //if (luma > 0.3) lineWidth = 0.7;
    //if (luma > 0.5) lineWidth = 0.5;
    //if (luma > 0.7) lineWidth = 0.3;
    //if (luma > 0.9) lineWidth = 0.1;
    //if (luma > 0.99) lineWidth = 0.0;
    //
    //float yStart = 0.05;
    //float yEnd = 0.95;
    //
    //
    //
    //// Determine where to draw black lines
    //bool isLine = (cellUV.y > yStart && cellUV.y < yEnd && cellUV.x > 0.0 && cellUV.x < lineWidth);
    //
    //if (isLine)
    //    color = mix(color, vec4(color.r/2, color.g/2, color.b/2, 1.0), 1.0);
    //
    //
    //
    //
    //color.rgb = sat(color.rgb, 1.0);

    FragColor = texture(screenTexture, TexCoords);
}
