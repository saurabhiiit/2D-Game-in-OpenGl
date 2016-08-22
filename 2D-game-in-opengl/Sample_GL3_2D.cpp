#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>  
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define PI 3.14159
const float DEG2RAD = 3.14159/180;
using namespace std;

float triangle_rotation = 0;
float rectangle_rotation = 0;
float rectangle_rot_dir = 1;
float circle1_rotation = 0;
void handleKeypress2(int key, int x, int y);
float power=10;
float x=-8;
float y=-7;
int flag =0;
int flag2=0;
int flag3=0;
int flag4=0;
int flag5=0;
int flag6=0;
int flag7=0;
int flag8=0;
int flag9=0;
int flag0=0;
int flag11=0;
int lflag=0;
int lflag1=0;
int lflag2=0;

int score=0;
float t = 0.1;
float t1 =0;
float t2=0;
float t3=0;
float t4=0;
float t5=0;
float x2=-4;
float y2=8;
float x3=6;
float y3=4 ;
float x4=0;
float y4=0;
float x5=2;
float y5=-12;



float ux;
float uy;
float theta=0;
float ballr= 0.25;
float distx1;
float disty1;
float tempx;
float tempy;
float sx1;
float sx2;
float xm;
float ym;
float xm1;
float ym1;
float xm2;
float ym2;
float xm3;
float ym3;

float level=1;
float u_xn = -10.0;
float u_xp = 10.0;
float u_yn = -10.0;
float u_yp = 10.0;


double xpos;
double ypos;
double yoffset;
double xoffset;
double yoffset1;


struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint MatrixID;
} Matrices;

GLuint programID;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
	std::string Line = "";
	while(getline(VertexShaderStream, Line))
	    VertexShaderCode += "\n" + Line;
	VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
	std::string Line = "";
	while(getline(FragmentShaderStream, Line))
	    FragmentShaderCode += "\n" + Line;
	FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
	    0,                  // attribute 0. Vertices
	    3,                  // size (x,y,z)
	    GL_FLOAT,           // type
	    GL_FALSE,           // normalized?
	    0,                  // stride
	    (void*)0            // array buffer offset
	    );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
	    1,                  // attribute 1. Color
	    3,                  // size (r,g,b)
	    GL_FLOAT,           // type
	    GL_FALSE,           // normalized?
	    0,                  // stride
	    (void*)0            // array buffer offset
	    );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
	color_buffer_data [3*i] = red;
	color_buffer_data [3*i + 1] = green;
	color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/

float triangle_rot_dir = 1;
//float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

void updateProjection()
{
 Matrices.projection = glm::ortho(u_xn, u_xp, u_yn, u_yp, 0.1f, 500.0f);
}

void zoomin()
{
    u_xn *= 0.8;
    u_xp *= 0.8;
    u_yn *= 0.8;
    u_yp *= 0.8;
    updateProjection();
}

void zoomout()
{
    u_xn *= 1.25;
    u_xp *= 1.25;
    u_yn *= 1.25;
    u_yp *= 1.25;
    updateProjection();
}

void panleft()
{
    if(u_xn!=-10 && u_xp!=10 )
    {
        u_xn -= 2.0;
        u_xp -= 2.0;
        //u_yn += 1.5;
        //u_yp -= 1.5;
        updateProjection();
    }

}


void panright()
{
    if(u_xp!=-10 && u_xn!=10)
    {
        u_xn += 2.0;
        u_xp += 2.0;
 //   u_yn -= 1.5;
   // u_yp += 1.5;
    updateProjection();
    }
}




void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_REPEAT or action== GLFW_RELEASE) {
	switch (key) {
	    case GLFW_KEY_UP:
		//if(flag!=1)
		{
		    rectangle_rot_dir=1;
		    rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;
		}
		/*if(flag!=1)
		  circle1_rotation = circle1_rotation + 10;*/
		break;
	    case GLFW_KEY_DOWN:
		//if(flag!=1)
		{
		    rectangle_rot_dir=-1;
		    rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;
		}
		/*      }
			if(flag!=1)
			circle1_rotation = circle1_rotation + 10*rectangle_rot_dir;
		 */		break;

	case GLFW_KEY_W:
		rectangle_rot_dir=1;
		rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;


		/*rectangle_rot_dir=1;
		  rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;
		  circle1_rotation = circle1_rotation + 10;*/
		break;

	case GLFW_KEY_S:
		rectangle_rot_dir=-1;
		rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;

		/*rectangle_rot_dir=-1;
		  rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;
		  circle1_rotation = circle1_rotation + 10*rectangle_rot_dir;*/
		break;    

    case GLFW_KEY_O:
        zoomout();
        break; 

    case GLFW_KEY_I:
        zoomin();
        break;   

    case GLFW_KEY_L:
        panleft();
        break; 

    case GLFW_KEY_K:
        panright();
        break;  


	case GLFW_KEY_SPACE:
		//cout<<  cos(60*(PI/180)) << "\n";
		//stime=glfwGetTime();
		theta= rectangle_rotation;
		//cout<< glfwGetTime();
		flag=1;
		break;  
	case GLFW_KEY_LEFT:
		if(power<0)
		{

		}
		else if(flag!=1 && power >6)
		{
		    power = power -2;
		}     
		break;
	case GLFW_KEY_RIGHT:
		if(flag!=1 && power<24)
		    power = power+2;
		break;  

	case GLFW_KEY_A:
		if(power<0)
		{

		}
		else if(flag!=1 && power >6)
		{
		    power = power -2;
		}     
		break;
	case GLFW_KEY_D:
		if(flag!=1 && power<24)
		    power = power+2;
		break;              

	default:
		break;
    }
}
else if (action == GLFW_PRESS) 
{
    switch (key) 
    {
	case GLFW_KEY_ESCAPE:
	    quit(window);
	    break;
	default:
	    break;
    }
}
}


void collision ()
{
    float distx= abs(x2 - x-2.3);

    float disty= abs(y2 - y);
    float rectr = 0.708 ;

    float distr = rectr + ballr;
    // float distc = sqrt(pow(distx,2)+pow(disty,2));
    if( distx<=distr && disty<=distr)
    {
	//power = power* -1;
    flag7=1;
	//cout<<" Hurray \n";
	//cout << "x2\t" <<x2<<"\t"<< x<<"\tdistx\n" ;
    }

}


void collision1 ()
{
    //cout<<" hip hip Hurray \n";
    distx1= abs(x3 - x-2.3);

    disty1= abs(y3 - y);

    //float rectr = 0.708 ;
    //float distr = rectr + ballr;
    // float distc = sqrt(pow(distx,2)+pow(disty,2));
    if( distx1<=7.25  && disty1<=1)
    {
	//uy=-uy;
	/*   if(power<=16)
	     power=power/1.018;
	     else if(power==18)
	     power=power/1.125;
	     else if(power==20)
	     power=power=1.4;

	     tempx=x3-x-2.3;
	     tempy=y2-y;
	     if(flag5==0)
	     {
	     float sy=y-0.25;
	     sx2= (tan(theta*(PI/180)) + sqrt((pow(tan(theta*(PI/180)),2))- (80*pow(power,2)*sy*cos(theta*(PI/180)))))/20;
	     sx1= (tan(theta*(PI/180)) - sqrt((pow(tan(theta*(PI/180)),2))- (80*pow(power,2)*sy*cos(theta*(PI/180)))))/20;
	     flag5=1;
	     }
	//power = power* -1;
	 */
        // flag4++;
	flag4=1;
	//cout<<" hip hip Hurray \n";
	//cout << "x2\t" <<x2<<"\t"<< x<<"\tdistx\n" ;
    }


}

void collision2 ()
{
   
     if(sqrt((pow((8.5-x),2)+pow((-8-y),2)))<0.8)

{
    flag0=1;
    flag9=1;
     cout<< "FINAL SCORE\t" <<score+400 ;
        exit(0);
    //cout<<" hip hip Hurray 11111111111111111111111\n";
}
  

}


/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
    switch (key) {
	case 'Q':
	case 'q':
	    quit(window);
	    break;
	default:
	    break;
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
	    //if (action == GLFW_RELEASE)
	    theta= rectangle_rotation;
	    //cout<< glfwGetTime();
	    flag=1;
	    break; 
	    /*triangle_rot_dir *= -1;
	      rectangle_rot_dir=1;
	      rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;
	      break;*/
	case GLFW_MOUSE_BUTTON_RIGHT:
	    if (action == GLFW_RELEASE) {
		rectangle_rot_dir *= -1;
	    }
	    break;
	default:
	    break;
    }
}
/*
   static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
   {
   if(xpos>glfwGetCursorPos(GLFWwindow* window, double xpos, double ypos))
   {
   rectangle_rot_dir=1;
   rectangle_rotation = rectangle_rotation + 10*rectangle_rot_dir;

   }

   }

 */


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset1>yoffset)
    {
	zoomin();
    }
    else if(yoffset1<yoffset)
    {

    zoomout();
	//power=power+5;
    }
    yoffset1=yoffset;


}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
       is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    GLfloat fov = 90.0f;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
       glLoadIdentity ();
       gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.0f);

}

VAO *triangle, *rectangle, *rectangle1, *circle, *triangle1, *circle1, *rectangle2, *rectangle3,* rectangle4, *circle2;

// Creates the triangle object used in this sample code
void createTriangle2 ()
{
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    static const GLfloat vertex_buffer_data [] = {
	0, 0,0, // vertex 0
	-0.5,-1,0, // vertex 1
	.5,-1,0, // vertex 2
    };

    static const GLfloat color_buffer_data [] = {
	0,0,0, // color 0
	0,0,0, // color 1
	0,0,0, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

// Creates the rectangle object used in this sample code
void createRectangle ()
{
    // GL3 accepts only Triangles. Quads are not supported
    static const GLfloat vertex_buffer_data [] = {
	-12,-8.5,0, // vertex 1
	8,-8.5,0, // vertex 2
	8, -10,0, // vertex 3

	8, -10,0, // vertex 3
	-12, -10,0, // vertex 4
	-12,-8.5,0  // vertex 1
    };

    static const GLfloat color_buffer_data [] = {
	255,255,0, // color 1
	255,255,0, // color 2
	255,255,0, // color 3

	255,255,0, // color 3
	255,255,0, // color 4
	255,255,0  // color 1
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}
void createRectangle1 ()
{
    // GL3 accepts only Triangles. Quads are not supported static
    const GLfloat vertex_buffer_data [] = {
	0,-0.5,0, // vertex 1
	1.3,-0.25,0, // vertex 2
	1.3,0.25,0, // vertex 3

	1.3,0.25,0, // vertex 3
	0,0.5,0, // vertex 4
	0,-0.5,0  // vertex 1

	    /* 1, 0,0,
	       1.5,0,0,
	       1,-0.5,0

	     */
    };

    static const GLfloat color_buffer_data [] = {
	0,0,0, // color 1
	0,0,0, // color 2
	0,0,0, // color 3

	0,0,0, // color 3
	0,0,0, // color 4
	0,0,0  // color 1
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle1 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}



void createTriangle1 ()
{
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    static const GLfloat vertex_buffer_data [] = {
	0, 0,0, // vertex 0
	-.5,-1,0, // vertex 1
	.5,-1,0, // vertex 2
    };

    static const GLfloat color_buffer_data [] = {
	0,0,0, // color 0
	0,0,0, // color 1
	0,0,0, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    triangle1 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void createRectangle2 ()
{
    // GL3 accepts only Triangles. Quads are not supported static
    const GLfloat vertex_buffer_data [] = {
	-0.5,-0.5,0, // vertex 1
	0.5,-0.5,0, // vertex 2
	0.5, 0.5,0, // vertex 3

	0.5, 0.5,0, // vertex 3
	-0.5, 0.5,0, // vertex 4
	-0.5,-0.5,0  // vertex 1

	    /* 1, 0,0,
	       1.5,0,0,
	       1,-0.5,0

	     */
    };

    static const GLfloat color_buffer_data [] = {
	1,0,0, // color 1
	0,0,1, // color 2
	0,1,0, // color 3

	0,1,0, // color 3
	0.3,0.3,0.3, // color 4
	1,0,0  // color 1
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}   
void createRectangle3 ()
{
    // GL3 accepts only Triangles. Quads are not supported static
    const GLfloat vertex_buffer_data [] = {
	-7,-0.25,0, // vertex 1
	7,-0.25,0, // vertex 2
	2, 0.25,0, // vertex 3

	7, 0.25,0, // vertex 3
	-7, 0.25,0, // vertex 4
	-7,-0.25,0  // vertex 1

	    /* 1, 0,0,
	       1.5,0,0,
	       1,-0.5,0

	     */
    };

    static const GLfloat color_buffer_data [] = {
	1,0,0, // color 1
	0,0,1, // color 2
	0,1,0, // color 3

	0,1,0, // color 3
	0.3,0.3,0.3, // color 4
	1,0,0  // color 1
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle3 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}   

void createRectangle4 ()
{
    // GL3 accepts only Triangles. Quads are not supported static
    const GLfloat vertex_buffer_data [] = {
    -0.25,-2,0, // vertex 1
    0.25,-2,0, // vertex 2
    0.25, 2,0, // vertex 3

    0.25, 2,0, // vertex 3
    -0.25, 2,0, // vertex 4
    -0.25,-2,0  // vertex 1

        /* 1, 0,0,
           1.5,0,0,
           1,-0.5,0

         */
    };

    static const GLfloat color_buffer_data [] = {
    1,0,0, // color 1
    0,0,1, // color 2
    0,1,0, // color 3

    0,1,0, // color 3
    0.3,0.3,0.3, // color 4
    1,0,0  // color 1
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    rectangle4 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
} 


void createCircle(){
    int i,j;
    GLfloat x=0.0,y=0.0;
    GLfloat radius=0.5;
    // GLfloat PI=3.14f;
    double triangleAmount = 20;
    int triangle = 20; //# of triangles used to draw circle
    //  cout<<"Draw circle";

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    //glBegin(GL_TRIANGLE_FAN);
    //glVertex2f(x, y); // center of circle
    GLfloat vertex_buffer_data[500];
    GLfloat color_buffer_data [500];
    for(i = 0,j=0; i <= 3*triangle;i++) {
	vertex_buffer_data[j++]= x + (radius * cos((i *  twicePi) / triangleAmount)); 
	vertex_buffer_data[j++]=y + (radius * sin((i * twicePi) / triangleAmount));
	vertex_buffer_data[j++]=0; 
	vertex_buffer_data[j++]= x + (radius * cos(((i+1) *  twicePi) / triangleAmount)); 
	vertex_buffer_data[j++]=y + (radius * sin(((i+1) * twicePi) / triangleAmount));
	vertex_buffer_data[j++]=0;
	vertex_buffer_data[j++]=x;
	vertex_buffer_data[j++]=y;
	vertex_buffer_data[j++]=0;
    }
    for(i = 0; i <= 9*triangle;i++) 
	color_buffer_data[i]=0.0;

    circle=create3DObject(GL_TRIANGLES,60,vertex_buffer_data,color_buffer_data,GL_FILL);  
}

void createCircle1(){
    int i,j;
    GLfloat x=2.3,y=0.0;
    GLfloat radius=0.25;
    // GLfloat PI=3.14f;
    double triangleAmount = 20;
    int triangle = 20; //# of triangles used to draw circle
    //  cout<<"Draw circle";

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    //glBegin(GL_TRIANGLE_FAN);
    //glVertex2f(x, y); // center of circle
    GLfloat vertex_buffer_data[500];
    GLfloat color_buffer_data [500];
    for(i = 0,j=0; i <= 3*triangle;i++) {
	vertex_buffer_data[j++]= x + (radius * cos((i *  twicePi) / triangleAmount)); 
	vertex_buffer_data[j++]=y + (radius * sin((i * twicePi) / triangleAmount));
	vertex_buffer_data[j++]=0; 
	vertex_buffer_data[j++]= x + (radius * cos(((i+1) *  twicePi) / triangleAmount)); 
	vertex_buffer_data[j++]=y + (radius * sin(((i+1) * twicePi) / triangleAmount));
	vertex_buffer_data[j++]=0;
	vertex_buffer_data[j++]=x;
	vertex_buffer_data[j++]=y;
	vertex_buffer_data[j++]=0;
    }
    for(i = 0; i <= 9*triangle;i++) 
	color_buffer_data[i]=0.0;

    circle1=create3DObject(GL_TRIANGLES,60,vertex_buffer_data,color_buffer_data,GL_FILL);  
}

void createCircle2(){
    int i,j;
    GLfloat x=0.0,y=0.0;
    GLfloat radius=0.4;
    // GLfloat PI=3.14f;
    double triangleAmount = 20;
    int triangle = 20; //# of triangles used to draw circle
    //  cout<<"Draw circle";

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    //glBegin(GL_TRIANGLE_FAN);
    //glVertex2f(x, y); // center of circle
    GLfloat vertex_buffer_data[500];
    GLfloat color_buffer_data [500];
    for(i = 0,j=0; i <= 3*triangle;i++) {
    vertex_buffer_data[j++]= x + (radius * cos((i *  twicePi) / triangleAmount)); 
    vertex_buffer_data[j++]=y + (radius * sin((i * twicePi) / triangleAmount));
    vertex_buffer_data[j++]=0; 
    vertex_buffer_data[j++]= x + (radius * cos(((i+1) *  twicePi) / triangleAmount)); 
    vertex_buffer_data[j++]=y + (radius * sin(((i+1) * twicePi) / triangleAmount));
    vertex_buffer_data[j++]=0;
    vertex_buffer_data[j++]=x;
    vertex_buffer_data[j++]=y;
    vertex_buffer_data[j++]=0;
    }
    for(i = 0; i <= 9*triangle;i++) 
    color_buffer_data[i]=0.0;

    circle2=create3DObject(GL_TRIANGLES,60,vertex_buffer_data,color_buffer_data,GL_FILL);  
}

void createTriangle ()
{
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    static const GLfloat vertex_buffer_data [] = {
        0, 1,0, // vertex 0
        -1,-1,0, // vertex 1
        1,-1,0, // vertex 2
    };

    static const GLfloat color_buffer_data [] = {
        1,0,0, // color 0
        0,1,0, // color 1
        0,0,1, // color 2
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
}




// Now you need to hand over the coordinates to gl here in your preferred way,
float camera_rotation_angle = 90;
//float rectangle_rotation = 0;
//float triangle_rotation = 0;

float x1=-10.3;
float xf;


/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw ()
{
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    //  Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    //  Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    //  Don't change unless you are sure!!
    glm::mat4 MVP;	// MVP = Projection * View * Model

    // Load identity to model matrix


    /* Render your scene */

    // rectangle is the ground surface
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translateRectangl = glm::translate (glm::vec3(2, 0, 0));        // glTranslatef
    //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= translateRectangl;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    draw3DObject(rectangle);

    //cannon is the rectangle1
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateRectangle = glm::translate (glm::vec3(-8, -7.5, 0));        // glTranslatef
    glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateRectangle * rotateRectangle);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    draw3DObject(rectangle1);

    // Collide


    if(level==1)
{
    if(x2<12 && flag2==0)
    {
	//cout << "hekko"<<"\n";
	x2=x2+0.1;

    }
    else if(x2>=12)
    { 
	flag2=1;
	x2=x2-0.1;
    }
    else if(flag2==1 && x2>-12)
    {
	x2=x2-0.1;
    }
    else if(flag2==1 && x2<=-12)
    {
	//cout << "hekko"<<"\n";   
	x2=x2+0.1;
	flag2=0;
    }        
}
    Matrices.model = glm::mat4(1.0f);

    //cout << "x2 \t" <<x2 <<"\n";
    //if(flag4<1)
    glm::mat4 translateRectangle2 = glm::translate (glm::vec3(x2, y2, 0));        // glTranslatef
    //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= translateRectangle2;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(flag7==1 && level==1)
    {
        
        score=100;
       // cout<<score<<"\n";
        //level=level+1;

        flag7=0;
        lflag=1;
        level=2;

    }
    /*else if(level==2)
    {
        score=500;
        flag7=0;
        lflag1=1;

        cout<<score<<"\n";
    }*/

   //if(level==2) 
    if(flag7<1 )
    draw3DObject(rectangle2);
    
    if(score==100)
    {
        level=2;
    }
 
   

    // circle is the cannon bottom 
    Matrices.model = glm::mat4(1.0f);

    glm::mat4 translateTriangle1 = glm::translate (glm::vec3(-8, -7.5, 0));        // glTranslatef
    glm::mat4 rotateTriangle1 = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateTriangle1 * rotateTriangle1);
    //Matrices.model *= translateTriangle1;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    draw3DObject(triangle1);

    // Increment angles
    float increments = 1;
    draw3DObject(circle);

    // cannon ball is the circle 1
    float y1=-7.5;
    Matrices.model = glm::mat4(1.0f);
    //   flag4=0;
    if(flag==1) 
    {
	if(flag4==0)
	{
	    if(y>=-8 && power>0.8)
	    {   


		uy=(power*sin(theta*(PI/180)));
		ux=power*cos(theta*(PI/180));
		x= x1+(ux*t);
		y=  y1 + (uy*t-5*t*t);
		xf=x;
		xm=x;
		ym=y;
        xm3=x;
        ym3=y;
		
		t=t+0.05;
	    } 
	    else if (y<-8 )
	    {
		x1=x;
		y=-8;
		
		power=power/2; 	
		t=0;

	    }
	    else 
	    {
		//glfwSetTime(0);
		x1=-10.3;
		power=10;
		flag=0;
		flag6=0;
		// flag4=0;
	    }
	}
	else if(flag4==1)
	{
	    if(flag6==0 )
	    {
		
		if(y>=-8  && y<=4)
		{
		    x= xm+(ux*t1);

		    y=  ym + (-0.3*uy*t1-5*t1*t1);
		    //cout << "x\t"<< ux<<"\thello\n";
		    //cout << "y\t"<< uy<<"\thello\n";
		    t1=t1+0.05;
		    //xf=x;
		}
		else if(y>4)
		{
    		    xm1=x;
		    ym1=y;

		    flag6=1;


		    
		}
		else if(y<-8)
		{
		    flag4=0;
		    t1=0;
		}
		else
		{
		    //cout << "hello dude\n";
		    flag4=0;
		}
	    }
	    else if(flag6==1)
	    {

		if(y>=4 && power>0.8 && x3-x-2.3<=7.25)
		{   


		    uy=(power*sin(theta*(PI/180)));
		    ux=power*cos(theta*(PI/180));
		    x= xm1+(ux*0.5*t3);
		    y=  ym1 + (uy*0.5*t3-5*t3*t3);
		    
		    t3=t3+0.05;
		    //flag4=0;
		} 
		else if (y<4 && flag4==1)
		{
		    //if((abs(x3)-abs(x+2.3))<0)
		    if(power>0.8)
		    {
			xm1=x;
			y=4;
			flag5=0;
			xm2=x;
			ym2=y;
		
			power=power/2;  
			t3=0;
			
			//flag4=0;
		    }
		    else
		    {
			x1=-10.3;
			power=10;
			flag=0;
			flag6=0;
		    }
		    

		}

	    }

	}



	collision();
	collision1();
    collision2();
	/*
	   if(flag==1)
	   {
	   x=sx2;
	   t=t+((sx2-sx1)/(cos(theta*(PI/180))*power));
	   }*/

	glm::mat4 translateCircle1 = glm::translate (glm::vec3(x, y, 0.0f));
	// glTranslatef
	glm::mat4 rotateCircle1 = glm::rotate((float)(circle1_rotation*M_PI/180.0f), glm::vec3(0 ,0,1));  // rotate about vector (1,0,0)
	//glm::mat4 circleTransform = translateTriangle ;
	Matrices.model *= translateCircle1;
	Matrices.model *= rotateCircle1;
	//Matrices.model *= triangleTransform; 
	MVP = VP * Matrices.model; // MVP = p * V * M 
	//  Don't change unless you are sure!!
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// draw3DObject draws the VAO given to it using current MVP matrix
	//if( flag4==0 or x<sx1 or x>sx2)
	draw3DObject(circle1);
    }
    Matrices.model = glm::mat4(1.0f);

    //cout << "x2 \t" <<x2 <<"\n";
    glm::mat4 translateRectangle3 = glm::translate (glm::vec3(x3, y3, 0));        // glTranslatef
    //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= translateRectangle3;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //if(flag8!=0)
    draw3DObject(rectangle3);

    if(level>1)
    {

       

    glm::mat4 translateCircle2 = glm::translate (glm::vec3(x5, y5+.3, 0.0f));
    // glTranslatef
    //glm::mat4 rotateCircle1 = glm::rotate((float)(circle1_rotation*M_PI/180.0f), glm::vec3(0 ,0,1));  // rotate about vector (1,0,0)
    //glm::mat4 circleTransform = translateTriangle ;
    Matrices.model *= translateCircle2;
    //Matrices.model *= rotateCircle1;
    //Matrices.model *= triangleTransform; 
    MVP = VP * Matrices.model; // MVP = p * V * M 
    //  Don't change unless you are sure!!
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // draw3DObject draws the VAO given to it using current MVP matrix
    //if( flag4==0 or x<sx1 or x>sx2)


    if(flag0!=1)
    {
        draw3DObject(circle2);
        flag11=1;
    }   
     Matrices.model = glm::mat4(1.0f);




    glm::mat4 translateTriangle = glm::translate (glm::vec3(8, -7.5, 0));        // glTranslatef
    //glm::mat4 rotateTriangle1 = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    //glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateTriangle);
    //Matrices.model *= translateTriangle1;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    if(flag0!=1)
    draw3DObject(triangle);

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateRectangle4 = glm::translate (glm::vec3(-8, 2, 0));        // glTranslatef
   
    if(lflag1==1 && flag0==1 )
    {
        cout<< "FINAL SCORE\n" <<score+400 ;
        exit(0);


    }
    /*else if(flag7==1)
    {
         cout<< "FINAL SCORE\n" <<score ;
    }*/

}



    /* Initialise glfw window, I/O callbacks and the renderer to use */
    /* Nothing to Edit here */
}
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
	exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwGetPrimaryMonitor() --> for full screen window

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", glfwGetPrimaryMonitor(), NULL);
   // window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
	glfwTerminate();
	exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
       is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    /* double xpos, ypos;
       glfwGetCursorPos(window, &xpos, &ypos);
       cout<<"xpos\t"<< xpos<<"\n";
       cout<<"ypos\t"<< ypos<<"\n";
     */
    /*float xpos=0;
      float ypos=0;
      glfwSetCursorPosCallback(window, cursor_pos_callback);*/

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
    createRectangle ();
    createRectangle1 ();
    createTriangle1 ();
    createRectangle2 ();
    createRectangle3 ();
    createRectangle4 ();
    createCircle ();
    createCircle1 ();
    createCircle2 ();

    // createCircle();

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (0.1f, 0.5f, 1.0f, 1.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
    int width = 1000;
    int height = 1000;

    GLFWwindow* window = initGLFW(width, height);

    initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;


    glfwGetCursorPos(window, &xpos, &ypos);

    double xpos1=xpos;
    double ypos1=ypos;	
    //glfwSetScrollCallback(window, scroll_callback);

    //yoffset1 = yoffset; 



    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(window, &xpos, &ypos);
	if( ypos>ypos1)
	{
	    if(flag!=1)
	    {
		rectangle_rot_dir=-1;
		rectangle_rotation = rectangle_rotation + 5*rectangle_rot_dir;
	    }

	}
	else if(ypos<ypos1 )
	{
	    if(flag!=1)
	    {
		rectangle_rot_dir=1;
		rectangle_rotation = rectangle_rotation + 5*rectangle_rot_dir;
	    }
	}

	xpos1=xpos;
	ypos1=ypos;

	/*if(flag==0)
	  glfwSetScrollCallback(window, scroll_callback);
	 */
	// OpenGL Draw commands

	draw();

	// Swap Frame Buffer in double buffering
	glfwSwapBuffers(window);

	// Poll for Keyboard and mouse events
	glfwPollEvents();

	// Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
	current_time = glfwGetTime(); // Time in seconds
	if ((current_time - last_update_time) >= 0.5) { // atleast 0.5s elapsed since last frame
	    // do something every 0.5 seconds ..
	    last_update_time = current_time;
	}
    }

    glfwTerminate();
    if(flag0==1 && flag4==1 && level>1)
    {
        cout<< "FINAL SCORE\n" <<score+400 ;
        exit(0);

    }
    else if(flag4==1)
    {
        cout<< "FINAL SCORE\n" <<score;
    }
    else
    {
        cout<< "FINAL SCORE\n" << " 0";
    }
    exit(EXIT_SUCCESS);
}
