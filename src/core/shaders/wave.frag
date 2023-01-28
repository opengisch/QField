#version 440
layout(location = 0)out highp vec2 qt_TexCoord0;
struct S {
    sampler2D source;
    lowp float qt_Opacity;
    highp float frequency;
    highp float amplitude;
    highp float time;
};
uniform S;
void main() {
    highp vec2 texCoord = qt_TexCoord0;
    texCoord.y = S.amplitude * sin(time * frequency + texCoord.x * 6.283185) + texCoord.y;
    gl_FragColor = texture2D(source, texCoord) * qt_Opacity;
}"
