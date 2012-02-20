#define GL_GLEXT_PROTOTYPES

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#if !defined(GL_WHEEL_UP)
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

#define FRAG_SHADER_FILE "fragment_shader.c"
#define VERT_SHADER_FILE "vertex_shader.c"

#define ZOOM_FACTOR 0.5

GLuint vert, frag;
GLuint program;

int last_x = -1;
int last_y = -1;

float real = -2.0;
float w = 3.0;
float imaginary = -1.0;
float h = 2.0;

float win_w = 1024;
float win_h = 768;

float zoom_val = 1.0;

void set_uniform_var(char *name, float val)
{
    GLint loc = glGetUniformLocation(program, name);
    glUniform1f(loc, val);
}

char *read_shader_file(char *shader_file)
{
    FILE *fp;
    char *data;
    int f;
    int size;

    f = open(shader_file, O_RDONLY);
    if (!f)
        goto file_error;
    size = lseek(f, 0, SEEK_END);
    close(f);

    fp = fopen(shader_file, "rt");
    if (!fp)
        goto file_error;

    data = (char *)malloc(sizeof(char) * (size + 1));
    if (!data)
    {
        printf("No memory could be allocated for shader text\n");
        exit(1);
    }
    size = fread(data, sizeof(char), size, fp);
    data[size] = '\0';

    fclose(fp);

    return data;

file_error:
    printf("Failed to read shader file: %s\n", shader_file);
    exit(1);
}

void compile_shader(GLuint shader, char *data)
{
    GLint buff_len;
    GLsizei str_len;
    GLint compiled;
    GLchar *log;
    int len = strlen(data);

    glShaderSource(shader, 1, (const GLchar **)&data, &len);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        printf("Good job you broke the shader\n");

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &buff_len);
        if (buff_len > 1)
        {
            log = malloc(buff_len);

            glGetShaderInfoLog(shader, buff_len, &str_len, log);
            printf("%s\n", log);
            free(log);
        }
    }
}

void create_shaders()
{
    char *vdata;
    char *fdata;

    vert = glCreateShader(GL_VERTEX_SHADER);
    frag = glCreateShader(GL_FRAGMENT_SHADER);

    vdata = read_shader_file(VERT_SHADER_FILE);
    fdata = read_shader_file(FRAG_SHADER_FILE);

    compile_shader(vert, vdata);
    compile_shader(frag, fdata);

    free(vdata);
    free(fdata);
}

void link_shaders()
{
    GLint res;
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (res == GL_FALSE)
        printf("Linker failed cuz you suck?\n");
}

void init_shaders()
{
    program = glCreateProgram();

    create_shaders();

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    link_shaders();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glUseProgram(program);
    
    set_uniform_var("real", real);
    set_uniform_var("w", w);
    set_uniform_var("imaginary", imaginary);
    set_uniform_var("h", h);
    set_uniform_var("zoom_factor", zoom_val);

    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, win_h, 0.0);
    glVertex3f(win_w, win_h, 0.0);
    glVertex3f(win_w, 0.0, 0.0);
    glEnd();
    
    glUseProgram(0);

    glutSwapBuffers();
}

void zoom(int button, int x, int y)
{
    if (button == GLUT_WHEEL_UP)
    {
        real += (float)x / win_w * w - w * 0.25;
        w *= 0.5;
        imaginary += (float)y / win_h * h - h * 0.25;
        h *= 0.5;
        zoom_val += ZOOM_FACTOR;
    }
    else
    {
        real += (float)x / win_w * w - w;
        w *= 2.0;
        imaginary += (float)y / win_h * h - h;
        h *= 2.0;
        zoom_val -= ZOOM_FACTOR;
        zoom_val = (zoom_val < 1.0) ? 1.0 : zoom_val;
    }

    glutPostRedisplay();
}

void move(int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        last_x = -1;
        last_y = -1;
    }
    else
    {
        last_x = x;
        last_y = y;
    }
}

void handle_mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_WHEEL_UP:
        case GLUT_WHEEL_DOWN:
            zoom(button, x, y);
        case GLUT_LEFT_BUTTON:
            move(state, x, y);
    }
}

void motion(int x, int y)
{
    int dx, dy;

    if (last_x != -1)
    {
        dx = x - last_x;
        dy = last_y - y;;

        last_x = x;
        last_y = y;

        real -= w / win_w * dx;
        imaginary -= h / win_h * dy;

        glutPostRedisplay();
    }
}

void handle_keyboard_special(int key, int x, int y)
{
    GLint val;
    
    switch (key)
    {
        case GLUT_KEY_UP:
            val = GLUT_WHEEL_UP;
            break;
        case GLUT_KEY_DOWN:
            val = GLUT_WHEEL_DOWN;
            break;
        default:
            return;
    }

    zoom(val, win_w / 2, win_h / 2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(win_w, win_h);
    glutCreateWindow("Mandlebrot Set");
    
    glutDisplayFunc(display);
    glutMouseFunc(handle_mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(handle_keyboard_special);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, win_w, 0.0, win_h, 0, 0.01);
    glMatrixMode(GL_MODELVIEW);
    
    init_shaders();

    glutMainLoop();

    return 0;
}
