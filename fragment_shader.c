uniform float zoom_factor;

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
    float iter_max = 255.0;
    float color_val;

    while (iter < iter_max && square < max_square)
    {
        rtemp = r * r - i * i + x;
        itemp = 2.0 * r * i + y;
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
