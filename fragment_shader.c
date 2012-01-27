uniform float zoom;

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
    float iter_max = 200.0 * zoom;

    while (iter < iter_max && square < max_square)
    {
        rtemp = r * r - i * i + x;
        itemp = 2.0 * r * i + y;
        r = rtemp;
        i = itemp;
        square = r * r + i * i;
        iter++;
    }

    gl_FragColor = vec4(iter / iter_max, iter / iter_max, sin(iter / iter_max  * 2.0), 1.0);
}
