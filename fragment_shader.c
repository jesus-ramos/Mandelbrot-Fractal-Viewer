uniform float zoom_factor;
uniform float min_real;
uniform float max_real;
uniform float min_imaginary;
uniform float max_imaginary;
uniform float height;
uniform float width;
uniform float real_factor;
uniform float imaginary_factor;

varying float x;
varying float y;

void main(void)
{
    float iter = 0.0;
    float max_square = 4.0;
    float square = 0.0;
    float r = 0.0;
    float i = 0.0;
    float rtemp = 0.0;
    float itemp = 0.0;
    float iter_max = 200.0 * zoom_factor;
    float color_val;
    float c_x = min_real + x * real_factor;
    float c_y = max_imaginary - y * imaginary_factor;

    while (iter < iter_max && square < max_square)
    {
        rtemp = r * r - i * i + c_x;
        itemp = 2.0 * r * i + c_y;
        r = rtemp;
        i = itemp;
        square = r * r + i * i;
        iter++;
    }

    color_val = iter / iter_max;

    /* BLUE */
    gl_FragColor = vec4(color_val, color_val, sin(color_val * 2.0), 1.0);
    /* GREEN */
    /* gl_FragColor = vec4(color_val, sin(color_val * 2.0), color_val, 1.0); */
    /* RED */
    /* gl_FragColor = vec4(sin(color_val * 2.0), color_val, color_val, 1.0); */
}
