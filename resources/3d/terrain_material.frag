void MAIN() {
    vec2 uv = UV0;

    // Prevent divide-by-zero errors if scale ever hits exactly 0.0
    float safeScale = max(materialScale, 0.0001);

    // Calculate half the scale to find the distance from the center to the edges
    float halfScale = safeScale * 0.5;

    // Define the boundaries based on your offset and scale
    vec2 minBound = materialCenter - vec2(halfScale);
    vec2 maxBound = materialCenter + vec2(halfScale);
    vec2 texUV = (uv - minBound) / safeScale;

    // Check if the current pixel falls within the map's area
    bool inX = (uv.x >= minBound.x) && (uv.x <= maxBound.x);
    bool inY = (uv.y >= minBound.y) && (uv.y <= maxBound.y);

    if (inX && inY) {
        vec4 mapColor = texture(materialTexture, texUV);

        // Convert the map texture from sRGB to Linear color space
        mapColor.rgb = pow(mapColor.rgb, vec3(2.2));

        BASE_COLOR = mapColor;

    } else {
        // We are outside the map. Draw the background and grid.
        vec4 bgColor = vec4(0.706, 0.706, 0.706, 1.0);
        vec4 lineColor = vec4(0.627, 0.627, 0.627, 1.0);
        float lineThickness = 0.05;

        if (fract(texUV.x * gridDensity) < lineThickness || fract(texUV.y * gridDensity) < lineThickness) {
            BASE_COLOR = lineColor;
        } else {
            BASE_COLOR = bgColor;
        }
    }

    if (edlRadius > 0.0) {
        // Get the size of the depth texture to calculate texel size
        vec2 texSize = vec2(textureSize(DEPTH_TEXTURE, 0));
        vec2 texelSize = 1.0 / texSize;

        // Calculate the screen-space UV coordinates for the current fragment
        vec2 screenUV = FRAGCOORD.xy * texelSize;

        float depthC = texture(DEPTH_TEXTURE, screenUV).r;

        // Define the 4 neighbor offsets (up, down, left, right)
        vec2 offsets[4];
        offsets[0] = vec2(edlRadius * texelSize.x, 0.0);
        offsets[1] = vec2(-edlRadius * texelSize.x, 0.0);
        offsets[2] = vec2(0.0, edlRadius * texelSize.y);
        offsets[3] = vec2(0.0, -edlRadius * texelSize.y);

        float depthSum = 0.0;

        for (int i = 0; i < 4; i++) {
            float depthN = texture(DEPTH_TEXTURE, screenUV + offsets[i]).r;
            float difference = max(0.0, depthC - depthN);
            depthSum += difference;
        }

        // Calculate the darkening factor and apply it to the BASE_COLOR
        float shadingFactor = exp(-depthSum * edlStrength);
        BASE_COLOR = vec4(BASE_COLOR.rgb * shadingFactor, BASE_COLOR.a);
    }

    ROUGHNESS = 0.9;
    SPECULAR_AMOUNT = 0.0;
    METALNESS = 0.0;
}
