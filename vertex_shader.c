uniform float real;
uniform float w;
uniform float imaginary;
uniform float h;

varying float x;
varying float y;

void main(void)
{
    x = clamp(gl_Vertex.x, 0.0, 1.0) * w + real;
    y = clamp(gl_Vertex.y, 0.0, 1.0) * h + imaginary;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
