void MAIN() {
    vec4 baseColor = texture(INPUT, INPUT_UV);
    if (edlRadius > 0.0) {
        float depthC = texture(DEPTH_TEXTURE, INPUT_UV).r;

        // Calculate the size of a single pixel based on the depth texture resolution
        vec2 texelSize = 1.0 / vec2(textureSize(DEPTH_TEXTURE, 0));

        // Define the 4 neighbor offsets (up, down, left, right) scaled by our radius.
        vec2 offsets[4];
        offsets[0] = vec2(edlRadius * texelSize.x, 0.0);
        offsets[1] = vec2(-edlRadius * texelSize.x, 0.0);
        offsets[2] = vec2(0.0, edlRadius * texelSize.y);
        offsets[3] = vec2(0.0, -edlRadius * texelSize.y);

        float depthSum = 0.0;

        for (int i = 0; i < 4; i++) {
            float depthN = texture(DEPTH_TEXTURE, INPUT_UV + offsets[i]).r;
            float difference = max(0.0, depthC - depthN);
            depthSum += difference;
        }

        // Calculate the darkening factor
        float shadingFactor = exp(-depthSum * edlStrength);
        FRAGCOLOR = vec4(baseColor.rgb * shadingFactor, baseColor.a);
    } else {
        FRAGCOLOR = baseColor;
    }
}
