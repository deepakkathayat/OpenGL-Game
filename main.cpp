#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include "camera.h"
#include "text3d.h"
#define ShowUpvector

CCamera Camera;

#include <GL/glut.h>

#include "imageloader.h"
#include "md2model.h"
#include "TexFont.h"

using namespace std;
typedef struct 
{
int X;
int Y;
int Z;
double U;
double V;
}VERTICES;

typedef struct{
	float x,y;
	float Zf;
}Q;
Q sphere[10];
const int space = 10;
const int VertexCount = (90 / space) * (360 / space) * 4;
VERTICES VERTEX0[VertexCount];
VERTICES VERTEX1[VertexCount];
VERTICES VERTEX2[VertexCount];
VERTICES VERTEX3[VertexCount];
VERTICES VERTEX4[VertexCount];
VERTICES VERTEX5[VertexCount];
VERTICES VERTEX6[VertexCount];
VERTICES VERTEX7[VertexCount];
VERTICES VERTEX8[VertexCount];
VERTICES VERTEX9[VertexCount];
VERTICES VERTEX[VertexCount];
#define MaxTime 500
int Timer[MaxTime+1];
int TIndex = MaxTime;

float currentTime,SpreviousTime,TpreviousTime;
int doubleBuffer = 1;
TexFont *txf;

int ViewW,ViewH;
float size = 1.0;
int SkyBox[6],StoryTex;
const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
vector < vector <float> > tiles;
int num_tiles = 28;
int c=0;
float a = 0.0f;
MD2Model* _model;
MD2Model* _tower;
int _textureId1,_textureId2,texture[2];
//The forward position of the guy relative to an arbitrary floor "tile"
float PosH= 0;
float PosW= 0;
int flagL=0;
int flagR=0;
bool jump=false,ascend=false,drown=false,Die=false;
bool Hit=false,Win=false,Start=true,Story=false,Instruct=false,tPersonView=true,towerView=false,freeView=false,tileView=false,heliView=false;
float vel_jump = 0;
float gravity = 0.3f;
float J_factor = 0.0f;
float maxV = 2;
float H=1.0f,W=7.0f,L=10.0f;
float Hrange = 1.0f;
float G = -2.0f,G1 = 2.0f, G2 = 6.0f, G3 = 10.0f, G4 = 14.0f, G5 = 18.0f, G6 = 22.0f;
float X = 2.0f,X1 = 27.0f, X2 = 52.0f, X3 = 77.0f;
float _guyPosZ= 0;
float _guyPosX= 0;
float _guyPosY= -2.0f + H;
float guyHeight = 7.85f;
float DeathView = 0.0f;
float moveTileY = 5.0f;
float moveTileX = 5.0f;
float moveTileZ = 5.0f;
vector <bool> goingUp(num_tiles,true);
vector <bool> goingRight(num_tiles,true);
vector <bool> goingFront(num_tiles,true);
vector <bool> present(num_tiles,true);
vector <time_t> Ttime(num_tiles,0);
int OnTile=0,tileIndex=0;
int xOrigin = -1;
float deltaAngle = 0.0f;
float deltaMove = 0;
// angle of rotation for the camera direction
float angle = 0.0f;
float _angle = 210.0f;
float ANGLE = 0;
//text
float Xtext = -5;
int SIndex = 0;
float _Angle = 220.0f;
float _scale,_Scale;
const char* STRS0[4] = {"Video", "Tutorials", "Rock", ".com"};
const char* STRS[11] = {"Once upon a time...there lived a noble Pharoah in the far away deserts of the Ancient Egypt of the Earth.", "He was known as Osiris, the God Of AfterLife.","A gentle and kind God, he welcomed every mortal on planet Earth into AfterLife with welcoming hands and a grand heart, only that they had to be buried in tombs residing in the Great Pyramids.", "Typhon, the God Of Evil, who lived in the Lost World, was a ruinous and disruptive God.","To destroy AfterLife and take away the mortals' right to AfterLife, he vowed to kill Osiris. But he could not touch him so long as Osiris lived on Earth.", "Naturally, he invited Osiris to his own kingdom, the Lost World. And Osiris, not knowing Typhon's intentions, accepted the invitation.", "Soon after Osiris arrived in the Lost World, Typhon killed him with his bare hands.","Osiris, being the God Of AfterLife, now entered into AfterLife, as a Zombie.", "But his soul is still trapped in the Lost World, and as long as his soul is not set free, he cannot save the AfterLife from the wrath of Typhon.","You are Osiris's trapped soul. And you must escape from the Lost World in time, as every mortal's AfterLife is at stake.","May the Gods be with you"};
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;

// XZ position of the camera
float x=0.0f, z=50.0f;
GLuint SkyloadTexture(Image*);

void DisplaySphere (VERTICES z[],double R, GLuint texture){
	int b;
	glScalef (0.0125 * R, 0.0125 * R, 0.0125 * R);
	glRotatef (90, 1, 0, 0);
	glBindTexture (GL_TEXTURE_2D, texture);

	glBegin (GL_TRIANGLE_STRIP);
	for ( b = 0; b <= VertexCount; b++){
		glTexCoord2f (z[b].U, z[b].V);
		glVertex3f (z[b].X, z[b].Y, -z[b].Z);
	}

	for ( b = 0; b <= VertexCount; b++){
		glTexCoord2f (z[b].U, -z[b].V);
		glVertex3f (z[b].X, z[b].Y, z[b].Z);
	}
	glEnd();
}
void CreateSphere (VERTICES z[],double R, double H, double K, double Z) {
int n;
double a;
double b;
n = 0;
    
for( b = 0; b <= (90-space); b+=space){
    for( a = 0; a <= (360-space); a+=space){
z[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
z[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
z[n].Z = R * cos((b) / 180 * PI) - Z;
z[n].V = (2 * b) / 360;
z[n].U = (a) / 360;
n++;
z[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI
) - H;
z[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI
) + K;
z[n].Z = R * cos((b + space) / 180 * PI) - Z;
z[n].V = (2 * (b + space)) / 360;
z[n].U = (a) / 360;
n++;
z[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI
) - H;
z[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI
) + K;
z[n].Z = R * cos((b) / 180 * PI) - Z;
z[n].V = (2 * b) / 360;
z[n].U = (a + space) / 360;
n++;
z[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) /
180 * PI) - H;
z[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) /
180 * PI) + K;
z[n].Z = R * cos((b + space) / 180 * PI) - Z;
z[n].V = (2 * (b + space)) / 360;
z[n].U = (a + space) / 360;
n++;
    }
}
}
float computeScale0(const char* strs[4]) {
	float maxWidth = 0;
	for(int i = 0; i < 4; i++) {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}
	
	return 3.6f / maxWidth;
}
float computeScale(const char* strs[11]) {
	float maxWidth = 0;
	for(int i = 0; i < 11; i++) {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}
	
	return 11.6f / maxWidth;
}

void drawScene();
void 
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(drawScene);
  else
    glutIdleFunc(NULL);
}

void
bitmap_output(int x, int y, char *string, void *font)
{
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void
stroke_output(GLfloat x, GLfloat y, const char *format,...){
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(0.01, 0.01, 0.01);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}
void
smallstroke_output(GLfloat x, GLfloat y, const char *format,...){
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef(0.002, 0.002, 0.002);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

void cleanup() {
	t3dCleanup();
	delete _model;
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}
void initskybox()
{
	Image* image;
	image = loadBMP("left.bmp");
	SkyBox[3]=SkyloadTexture(image);
	delete image;
	image = loadBMP("back.bmp");
	SkyBox[5]=SkyloadTexture(image);
	delete image;
	image = loadBMP("right.bmp");
	SkyBox[2]=SkyloadTexture(image);
	delete image;
	image = loadBMP("front.bmp");
	SkyBox[4]=SkyloadTexture(image);
	delete image;
	image = loadBMP("top.bmp");
	SkyBox[0]=SkyloadTexture(image);
	delete image;
	image = loadBMP("front.bmp");
	SkyBox[1]=SkyloadTexture(image);
	delete image;
	image = loadBMP("StoryTex.bmp");
	StoryTex = SkyloadTexture(image);
	delete image;
}

void RenderSkybox()
{	
// djoubert187 _at_ hotmail.com
	// Begin DrawSkybox
	gluLookAt(	0.0f, 0.0f, 0.0f,
			Camera.Position.x+Camera.ViewDir.x, Camera.Position.y+Camera.ViewDir.y, Camera.Position.z+Camera.ViewDir.z,0.0f, 1.0f,  0.0f);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);	//turn off depth texting
	glDisable(GL_LIGHTING);	//turn off lighting, when making the skybox
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0,1.0f);
	// Save Current Matrix
	glPushMatrix();
	glLoadIdentity(); 

	// Second Move the render space to the correct position (Translate)
	glTranslatef(0,0,-5);
	glTranslatef(0,-5,0);
	// First apply scale matrix
	glScalef(15,15,15);
	// Common Axis Z - FRONT Side
	glEnable(GL_TEXTURE_2D);	//and turn on texturing
	glBindTexture(GL_TEXTURE_2D,SkyBox[5]);
	glBegin(GL_QUADS);      //and draw a face
                //back face
                glTexCoord2f(0,0);      //use the correct texture coordinate
                glVertex3f(size/2,size/2,size/2);       //and a vertex
                glTexCoord2f(1,0);      //and repeat it...
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(1,1);
                glVertex3f(-size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(size/2,-size/2,size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,SkyBox[3]);
        glBegin(GL_QUADS);     
                //left face
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(1,0);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,SkyBox[4]);
        glBegin(GL_QUADS);     
                //front face
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,-size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,SkyBox[2]);
        glBegin(GL_QUADS);     
                //right face
                glTexCoord2f(0,0);
                glVertex3f(size/2,size/2,-size/2);
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,SkyBox[0]);          
        glBegin(GL_QUADS);                      //top face
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,SkyBox[1]);               
        glBegin(GL_QUADS);     
                //bottom face
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(1,0);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();	
	glPopAttrib();
	glDisable(GL_TEXTURE_2D);
	
// Load Saved Matrix
	glPopMatrix();
} 

int spot(double a, double b, double c, double d, double e, double f){

	GLfloat mat_specular[] = { 0.3, 1.0, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { a,b,c, 1.0 };
	GLfloat spotDir[] = { d,e,f };
	glClearColor (0.5, 0.5, 0.5, 0.0);
	glShadeModel (GL_SMOOTH);
	glLightfv(GL_LIGHT0,GL_SPECULAR,mat_specular);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// Defining spotlight attributes
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,95.0);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,2.0);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	return 0;
}


void handleKeypress(unsigned char key, int x, int y) {
	
	if(key == 27){
		cleanup();
		exit(0);
	}
	else if(key == ' ' && Start == true){
		Start = false;
		Story = true;
		SIndex = 0;
		Xtext = -5;
	}
	else if(key == ' ' && Story == true){
		Story = false;
		TIndex = MaxTime;
	}
	else if(key == 'j' && !jump && !Die && !Win){
		maxV = 2;
		jump = true;
		ascend = true;
	}
	else if(key == ' ' && !jump && !Die && !Win){
		maxV = 3;
		jump = true;
		ascend = true;
	}
	else if(key == 'w'){
		Camera.MoveForward( -0.1 ) ;
	}
	else if(key == 's'){
		Camera.MoveForward( 0.1 ) ;
	}

	else if(key == 'a'){
		Camera.RotateY(5.0);
	}
	else if(key == 'd'){
		Camera.RotateY(-5.0);
	}
	else if(key == 'q'){
		Camera.RotateX(5.0);
	}
	else if(key == 'z'){
		Camera.RotateX(-5.0);
	}
	else if(key == 'e'){
		Camera.StrafeRight(-0.1);
	}
	else if(key == 'c'){
		Camera.StrafeRight(0.1);
	}
	else if(key == 'f'){
		Camera.MoveUpward(-0.3);
	}
	else if(key == 'r'){
		Camera.MoveUpward(0.3);
	}
	else if(key == 'm'){
		Camera.RotateZ(-5.0);
	}
	else if(key == 'n'){
		Camera.RotateZ(5.0);
	}
	
	else if(key == 'v'){
		c = (c+1)%5;
		if(c==0){ //third person view
			tPersonView = true;
			towerView = false;
			freeView = false;
			tileView = false;
			heliView = false;
		}
		else if(c==1){ //tower view
			towerView = true;
			tPersonView =false;
			freeView = false;
			tileView = false;
			heliView = false;
		}
		else if(c==2){ //tile view
			tileView = true;
			freeView = false;
			tPersonView = false;
			towerView = false;
			heliView = false;
		}
		else if(c==3){ //free view
			freeView = true;
			tileView = false;
			tPersonView = false;
			towerView = false;
			heliView = false;
		}
		else if(c==4){ //heli view
			heliView = true;
			freeView = false;
			tileView = false;
			tPersonView = false;
			towerView = false;
		}
	}
	else if(key == 't' && tileView){
			tileIndex = (tileIndex + 1)%num_tiles;
	}
	else if(key == 'i' && Instruct == false)
		Instruct = true;
	else if(key == 'i' && Instruct == true)
		Instruct = false;
}

//Makes the image into a texture, and returns the id of the texture
GLuint SkyloadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
	
	return textureId;
}
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,image->pixels);
	
	return textureId;
}
GLuint SloadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
 GL_LINEAR_MIPMAP_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
 GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
GL_REPEAT );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, image->width, image->height, 
GL_RGB, GL_UNSIGNED_BYTE, image->pixels );
	
	return textureId;
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1);
	
	Image* image = loadBMP("fireTex.bmp");
	texture[0] = SloadTexture(image);
	delete image;
	image = loadBMP("earth.bmp");
	texture[1] = SloadTexture(image);
	delete image;
	srand(time(NULL));
	for(int k=0;k<10;k++){
		sphere[k].x = (rand()%150)/10.0;
		sphere[k].y = (rand()%150)/10.0;
		sphere[k].Zf = 0;
	}
		CreateSphere(VERTEX0,70,sphere[0].x,sphere[0].y,0);
		CreateSphere(VERTEX1,70,sphere[1].x,sphere[1].y,0);
		CreateSphere(VERTEX2,70,sphere[2].x,sphere[2].y,0);
		CreateSphere(VERTEX3,70,sphere[3].x,sphere[3].y,0);
		CreateSphere(VERTEX4,70,sphere[4].x,sphere[4].y,0);
		CreateSphere(VERTEX5,70,sphere[5].x,sphere[5].y,0);
		CreateSphere(VERTEX6,70,sphere[6].x,sphere[6].y,0);
		CreateSphere(VERTEX7,70,sphere[7].x,sphere[7].y,0);
		CreateSphere(VERTEX8,70,sphere[8].x,sphere[8].y,0);
		CreateSphere(VERTEX9,70,sphere[9].x,sphere[9].y,0);
		CreateSphere(VERTEX,70,0,0,0);
		for(int i = 0; i<=MaxTime; i++)
			Timer[i] = i;
	initskybox();
	t3dInit();
	
	//Load the model
	_model = MD2Model::load("Archvile.md2");
	if (_model != NULL) {
		_model->setAnimation("run");
	}
	else{
		printf("Model Not Loaded\n");
	}
	
	//Load the floor texture
	image = loadBMP("water2.bmp");
	_textureId1 = loadTexture(image);
	delete image;
	//Load the tile texture
	image = loadBMP("drock079.bmp");
	_textureId2 = loadTexture(image);
	delete image;
	
	//Load the tower
	_tower = MD2Model::load("farao.md2");
	if (_tower != NULL) {
		_tower->setAnimation("run");
	}
	else{
		printf("Model Not Loaded\n");
	}
}
void handleResize(int w, int h) {
	ViewW = w;
	ViewH = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
}

void mouseMove(int x, int y) {
	if(heliView){
         // this will only be true when the left button is down
         if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
	}
}

void mouseButton(int button, int state, int x, int y) {
	if(heliView){
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
	if(button == 3){
		if(state==GLUT_UP){ 
			return;
		}
		deltaMove = 1.5f;
	}
	if(button == 4){
		if(state==GLUT_UP){ 
			return;
		}
		deltaMove = -1.5f;
	}
	}
}
void checkFall(){
	int i;
	for(i=0;i<num_tiles;i++){
		float x = tiles[i][0];
		float y = tiles[i][1];
		float z = tiles[i][2];

		if(_guyPosX<=x && _guyPosX>=x-L){
			if(_guyPosZ<=z && _guyPosZ>=z-W){
				if((_guyPosY+J_factor)>=y){
					_guyPosY=y+H;
					OnTile = i;
					if(tiles[i][1] == -100 && OnTile==i){
						jump = false;
						break;
					}
					if(OnTile == 27){
						Win = true;
						_model->setAnimation("res");
					}
					return;
				}
			}
		}
	}
		if(!jump){
			Die = true;
			if(drown)
				_model->setAnimation("res");
			else
				_model->setAnimation("pain");
		}
		return;
}
void checkBall(){
	int i;
	for(i=0;i<10;i++){
		if(_guyPosX >= sphere[i].x-2 && _guyPosX <= sphere[i].x + 2){
			if(_guyPosY + guyHeight >= sphere[i].y && _guyPosY <= sphere[i].y){
				if(sphere[i].Zf > 48 && sphere[i].Zf < 52){
					TIndex -= 5;
					if(TIndex<=0 && !Die){ 
						Die = true;
						drown = true;
						jump = false;
						_model->setAnimation("pain");
					}
					Hit = true;
					break;
				}
			}
		}
	}
	return;
}

void StoryScreen(){
float size = 2.7;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)ViewW / (float)ViewH, 1.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);	//turn off depth texting
	glDisable(GL_LIGHTING);	//turn off lighting, when making the skybox
	glDisable(GL_BLEND);
	glColor4f(1.0, 1.0, 1.0,1.0f);
	glPushMatrix();
	// Second Move the render space to the correct position (Translate)
	glTranslatef(0,0,-5);
	glTranslatef(0,-5,0);
	// First apply scale matrix
	glScalef(15,15,15);
	// Common Axis Z - FRONT Side
		glEnable(GL_TEXTURE_2D);	
        glBindTexture(GL_TEXTURE_2D,StoryTex);
        glBegin(GL_QUADS);     
                //front face
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,-size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,-size/2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopAttrib();
		glPushMatrix();
	
	if(SIndex<10){
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	glTranslatef(Xtext, 0, 0);
    glScalef(_Scale, _Scale, _Scale);
    glColor3f(0.81f, 0.71f, 0.23f);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, -5, -2.5f / _Scale);
		t3dDraw3D(STRS[SIndex], 0, 0, 0.2f);
		glPopMatrix();
	}
	else{
	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
    glScalef(_Scale, _Scale, _Scale);
    glColor3f(0.3f, 1.0f, 0.3f);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, -5, -2.5f / _Scale);
		t3dDraw3D(STRS[SIndex], 0, 0, 0.2f);
		glPopMatrix();
	}
	glPopMatrix();
}

void StartScreen(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, ViewW, 0, ViewH);
  glScalef(1, -1, 1);
  glTranslatef(0, -ViewH, 0);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 0.1, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  gluLookAt(0.0, 0.0, 4.0,  /* eye is at (0,0,30) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in postivie Y direction */
  glPushMatrix();
  glTranslatef(0, 0, -4);
  glRotatef(50, 0, 1, 0);
  glColor3f(0.85,0.53,0.10);
  stroke_output(-3.2, 0.2, "The Lost World");
  glColor3f(0.85,0.85,0.95);
  smallstroke_output(-2.5, -1.5, "A survival game");
  smallstroke_output(-2.5, -2.1, "By Deepak Kathayat");
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();
}
void TimeScreen(){
  char str[100];
  glPushMatrix();
	glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	glScalef(5,5,5);
    sprintf(str,"%d",Timer[TIndex]);
	for(int i = 0; i < 1; i++) {
  		glColor3f(0.184314f,0.184314f,0.309804f);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, 0, -2.5f / _Scale);
		t3dDraw3D(str, 0, 0, 0.2f);
		glPopMatrix();
	}
	glPopMatrix();
}
void DeathScreen(){
  char str[] = "Your soul shall remain trapped forever, Osiris";
  char str1[] = "AfterLife is Destroyed";
  glPushMatrix();
	
    glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f,0.25f,0.0f);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(-3, 0, -2.5f / _Scale);
		t3dDraw3D(str, 0, 0, 0.2f);
		glTranslatef(0, -2, 0);
		t3dDraw3D(str1, 0, 0, 0.2f);
		glPopMatrix();
	glPopMatrix();
}
void WinScreen(){
  char str[] = "Osiris, you truly are the God of Gods";
  char str1[] = "AfterLife is now safe";
  glPushMatrix();
	
    glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f,0.25f,0.0f);
		glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glTranslatef(2, 0, -2.0f / _Scale);
		t3dDraw3D(str1, 0, 0, 0.2f);
		glTranslatef(0, -5, 0);
		t3dDraw3D(str, 0, 0, 0.2f);
		glPopMatrix();
	glPopMatrix();
}
void IScreen(){
  char str[] = "Instructions";
  char str1[] = "1. Movement     Arrow Keys";
  char str2[] = "2. Short Jump     j";
  char str3[] = "3. Long Jump     SpaceBar";
  char str4[] = "4. Change Camera View     v     ";
  char str5[] = "5. Change Tile in TileView      t";
  char str6[] = "6. Exit     Esc";
  char str7[] = "Free View Controls";
  char str8[] = "1. Move Forward/Backward     s/w";
  char str9[] = "2. Move Up/Down     r/f";
  char str10[] = "3. Strafe Right/Left      c/e";
  char str11[] = "4. Rotate X     q/z";
  char str12[] = "5. Rotate Y     a/d";
  char str13[] = "6. Rotate Z     n/m";
  glPushMatrix();
    glRotatef(18.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(180, 0.0f, 1.0f, 0.0f);
	glColor3f(1.0f,0.25f,0.0f);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0, 3, -4.0f / _Scale);
	t3dDraw3D(str, 0, 0, 0.2f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(-20, 2, -5.5f / _Scale);
	t3dDraw3D(str1, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str2, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str3, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str4, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str5, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str6, 0, 0, 0.2f);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(20, 2, -5.5f / _Scale);
	t3dDraw3D(str7, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str8, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str9, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str10, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str11, 0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str12,0, 0, 0.2f);
	glTranslatef(0, -1.5, 0);
	t3dDraw3D(str13, 0, 0, 0.2f);
	glPopMatrix();
	glPopMatrix();
}
void displaySphere(Q &s,VERTICES V[]){
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
    
	glClearDepth(1);
	glPushMatrix();
    glTranslatef(s.x,s.y,_guyPosZ+50-s.Zf);
    //glTranslatef(0,0, 0);
	//printf("guyX = %f, guyY = %f, guyZ = %f || x = %f, y = %f, z = %f\n",_guyPosX,_guyPosY,_guyPosZ,sphere[0].x,sphere[0].y,sphere[0].Zf);
	glRotatef(ANGLE,0,1,0);
    DisplaySphere(V,0.5, texture[0]);
    glPopMatrix();
	glPopAttrib();
	ANGLE++;
	s.Zf += (5.0+ rand()%5)/10.0;
	if(s.Zf>60){
		if(rand()%2)
			s.x = tiles[OnTile][0] + (-1.0*(rand()%150))/10.0;
		else s.x = tiles[OnTile][0] + (1.0*(rand()%150))/10.0;
		if(rand()%2)
			s.y = tiles[OnTile][1] + (-1.0*(rand()%150))/10.0;
		else s.y = tiles[OnTile][1] + (1.0*(rand()%150))/10.0;
		s.Zf = 0;
	}
}
void displayBigSphere(VERTICES V[]){
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
    
	glClearDepth(1);
	glPushMatrix();
    glTranslatef(tiles[27][0]-W/2,tiles[27][1]+L/2,tiles[27][2]);
	glRotatef(ANGLE,0,1,0);
    DisplaySphere(V,5, texture[1]);
    glPopMatrix();
	glPopAttrib();
	ANGLE++;
}
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(Start){
	StartScreen();
 }else if(Story){
		StoryScreen();
 }else{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)ViewW / (float)ViewH, 1.0, 200.0);
GLfloat mat_specular[] = { 0.3, 0.3, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	gluLookAt( _guyPosX , _guyPosY - DeathView+1,_guyPosZ+7,_guyPosX, (guyHeight/2)+_guyPosY,  _guyPosZ,0.0f, 1.0f,  0.0f);
	RenderSkybox();
	glPopMatrix();
	glPushMatrix();
	if(tPersonView){
		glTranslatef(0.0f, 0.0f, -25.0f);
		glRotatef(-180, 0.0f, 1.0f, 0.0f);
		gluLookAt( _guyPosX , _guyPosY - DeathView+1,_guyPosZ+7,_guyPosX, (guyHeight/2)+_guyPosY,  _guyPosZ,0.0f, 1.0f,  0.0f);
	}
	else if(freeView){
		Camera.Render();
		glTranslatef(0.0f, 0.0f, -25.0f);
		glRotatef(-180, 0.0f, 1.0f, 0.0f);
		gluLookAt( _guyPosX , _guyPosY - DeathView+1,_guyPosZ+7,_guyPosX, (guyHeight/2)+_guyPosY,  _guyPosZ,0.0f, 1.0f,  0.0f);
	}else if(towerView){
		gluLookAt( 140.0f , 74.0f, -85.0f,139.0f, 74, -85, 0.15f, 1.0f,  0.4f);
		glTranslatef(0.0f, 0.0f, -25.0f);
		glRotatef(-180, 0.0f, 1.0f, 0.0f);
		gluLookAt( _guyPosX , _guyPosY - DeathView+1,_guyPosZ+7,_guyPosX, (guyHeight/2)+_guyPosY,  _guyPosZ,0.0f, 1.0f,  0.0f);
	}else if(tileView){
		gluLookAt( tiles[tileIndex][0] , tiles[tileIndex][1]+H+(guyHeight/2), tiles[tileIndex][2],_guyPosX, (guyHeight/2)+_guyPosY, _guyPosZ, 0.0f, 1.0f,  0.0f);
	}
	else if(heliView){
		if (deltaMove)
			computePos(deltaMove);
		Camera.Render();
	gluLookAt(	x, 10.0f, z,
			x+lx, 10.0f,  z+lz,
			0.0f, 1.0f,  0.0f);
	}
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
		
	GLfloat lightColor0[] = {0.8f, 0.2f, 0.3f, 1.0f};
	GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightPos[] = {_guyPosX, _guyPosY, _guyPosZ, 0.0f};
	if(Hit){
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	Hit = false;
	}
	else{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	}
	spot( tiles[OnTile][0]-L/2,tiles[OnTile][1]+H+guyHeight/2,tiles[OnTile][2]-W/2,-2200,-3800-_guyPosZ,-185);

	if(_tower != NULL){
		glPushMatrix();
		glRotatef(-90.0f, 1.0f,0.0f, 0.0f);
		glScalef(0.05f, 0.05f, 0.05f);
		glTranslatef(-2200.0f,-_guyPosZ-3800.0f,-185.0f);
		glRotatef(90.0f, 0.0f,0.0f, 1.0f);
		_tower->draw();
		glPopMatrix();
	}
	//Draw the guy
	if (_model != NULL) {
		glPushMatrix();
		glTranslatef(0.0f, _guyPosY+J_factor, 0.0f);
		glTranslatef(0.0f,0.0f, _guyPosZ);
		glTranslatef(_guyPosX, 0.0f, 0.0f);
	glRotatef(-90.0f, 1.0f,0.0f, 0.0f);
	glRotatef(-90.0f, 0.0f,0.0f, 1.0f);
	glRotatef(-a, 0.0f, 0.0f, 1.0f);
	glScalef(0.1f, 0.1f, 0.1f);
    _model->draw();
	glPopMatrix();
	}
	checkFall();
	if(!Win)
		checkBall();
	//Draw the floor
	glPushMatrix();
	glTranslatef(0.0f, G-3.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f((2000+PosW)/FLOOR_TEXTURE_SIZE, PosH / FLOOR_TEXTURE_SIZE);
	//glTexCoord2f(0,1);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2f((2000+PosW) / FLOOR_TEXTURE_SIZE, (2000 + PosH) / FLOOR_TEXTURE_SIZE);
//	glTexCoord2f(0,0);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(PosW / FLOOR_TEXTURE_SIZE, (2000 + PosH) / FLOOR_TEXTURE_SIZE);
//	glTexCoord2f(1,0);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(PosW / FLOOR_TEXTURE_SIZE, PosH / FLOOR_TEXTURE_SIZE);
//	glTexCoord2f(1,1);
	glVertex3f(1000.0f, 0.0f, -1000.0f);
	
	glEnd();
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glDisable(GL_TEXTURE_2D);
	
	glTranslatef(0.0f, 5.0f, 1.0f);

	for(int i=0;i<num_tiles;i++){
	
	float x=tiles[i][0],y=tiles[i][1],z=tiles[i][2];
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUAD_STRIP);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(x-L, y+H, z);   //V8
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x-L, y+H,z-W);   //V6
	glTexCoord2f(1.0, 0.0);
	glVertex3f( x,y+H, z);   //V2
	glTexCoord2f(1.0, 1.0);
	glVertex3f( x, y+H,z-W);   //V4
	glTexCoord2f(1.0, 1.0);
	glVertex3f( x,y+H, z);   //V2
	glTexCoord2f(1.0, 0.0);
	glVertex3f( x,y,z);   //V1
	glTexCoord2f(0.0, 1.0);
	glVertex3f( x, y+H,z-W);   //V4
	glTexCoord2f(0.0, 0.0);
	glVertex3f( x,y,z-W);   //V3
	glTexCoord2f(1.0, 1.0);
	glVertex3f(x-L, y+H,z-W);   //V6
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x-L,y,z-W);   //V5
	glTexCoord2f(0.0, 1.0);
	glVertex3f(x-L, y+H, z);   //V8
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x-L,y, z);   //V7
	glTexCoord2f(1.0, 1.0);
	glVertex3f( x, y+H, z);   //V2
	glTexCoord2f(1.0, 0.0);
	glVertex3f( x,y, z);   //V1
		
	glEnd();
	glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	if(!Win){
		displayBigSphere(VERTEX);
	
	displaySphere(sphere[0],VERTEX0);
	displaySphere(sphere[1],VERTEX1);
	displaySphere(sphere[2],VERTEX2);
	displaySphere(sphere[3],VERTEX3);
	displaySphere(sphere[4],VERTEX4);
	displaySphere(sphere[5],VERTEX5);
	displaySphere(sphere[6],VERTEX6);
	displaySphere(sphere[7],VERTEX7);
	displaySphere(sphere[8],VERTEX8);
	displaySphere(sphere[9],VERTEX9);
	}
	glPopMatrix();
	if(!Die && !Win)
		TimeScreen();
	else if(Die)
		DeathScreen();
	}
	if(Instruct)
		IScreen();
	if(Win == true)
		WinScreen();
	glutSwapBuffers();
}
void UpDown(int i,float level){
	if(goingUp[i]){
		tiles[i][1]+=0.05f;
		if(OnTile==i)
			_guyPosY = tiles[i][1]+H;
		if(tiles[i][1]>=level+moveTileY)
			goingUp[i] = false;
	}
	if(!goingUp[i]){
		tiles[i][1]-=0.05f;
		if(OnTile==i)
			_guyPosY = tiles[i][1]+H;
		if(tiles[i][1]<=level-moveTileY)
			goingUp[i] = true;
	}
}
void RightLeft(int i, float level){
	if(goingRight[i]){
		tiles[i][0]-=0.05f;
		if(OnTile==i){
			if(_guyPosX>=tiles[i][0]){
				_guyPosX = tiles[i][0];
			}
		}
		if(tiles[i][0]<=level-moveTileX)
			goingRight[i] = false;
	}
	if(!goingRight[i]){
		tiles[i][0]+=0.05f;
		if(OnTile==i){
			if(_guyPosX<=tiles[i][0]-L){
				_guyPosX = tiles[i][0]-L;
			}
		}
		if(tiles[i][0]>=level+moveTileX)
			goingRight[i] = true;
	}
}
void FrontBack(int i, float level){
	if(goingFront[i]){
		tiles[i][2]-=0.05f;
		if(OnTile==i){
			if(_guyPosZ>=tiles[i][2]){
				_guyPosZ = tiles[i][2];
			}
		}
		if(tiles[i][2]<=level-moveTileZ)
			goingFront[i] = false;
	}
	if(!goingFront[i]){
		tiles[i][2]+=0.05f;
		if(OnTile==i){
			if(_guyPosZ<=tiles[i][2]-W){
				_guyPosZ = tiles[i][2]-W;
			}
		}
		if(tiles[i][2]>=level+moveTileZ)
			goingFront[i] = true;
	}
}
void TileMovement(){
	
	//11th Tile - Up and Down
	UpDown(10,G2);
	
	//2nd Tile - Right and Left
	RightLeft(1,X);
	
	//13th Tile - Right and Left
	RightLeft(12,X1);
	
	//6th Tile - Up and Down
	UpDown(5,G);

	//18th Tile - Front and Back
	FrontBack(17,275);

	//22nd Tile - Up and Down
	UpDown(21,G4);
	
	//23rd Tile - Front and Back
	FrontBack(22,275+moveTileZ+90);
	
	//25th Tile - Up and Down
	UpDown(24,G4);

	//15th Tile - Disappear
	if((time(NULL)-Ttime[14])>3 && present[14]){
		tiles[14][0]=X;
		tiles[14][1]=-100;
		tiles[14][2]=225;
		present[14]=false;
		Ttime[14] = time(NULL);
	}
	else if((time(NULL) - Ttime[14])>7 && !present[14]){
		tiles[14][0]=X;
		tiles[14][1]=G2;
		tiles[14][2]=225;
		present[14]=true;
		Ttime[14] = time(NULL);
	}
}
void update(int value) {
	_Angle -= 0.5f;
	Xtext += 0.03f;
	

	if(jump){
		if(vel_jump<maxV && ascend){
			vel_jump+=gravity;
			J_factor+=vel_jump;
		}
		else if(vel_jump>maxV){
			ascend=0;
			vel_jump-=gravity;
			J_factor-=vel_jump;
		}
		else if(vel_jump<maxV && !ascend){
			vel_jump-=gravity;
			J_factor-=vel_jump;
			if(vel_jump<0){
				jump=false;
				J_factor=0;
				vel_jump=0;
			}
		}
		if(a == 0)
			_guyPosZ += 1.3f;
		else if(a == 180)
			_guyPosZ -= 1.3f;
		else if(a == -90)
			_guyPosX += 1.3f;
		else if(a == 90)
			_guyPosX -= 1.3f;
		if (_model != NULL) {
			_model->advance(0.045f);
		}
	}
	
	//Tile Movement
	TileMovement();

	PosH += 0.1f;
	if(Die){
		if(G<=_guyPosY+guyHeight)
			G = G + 0.2f;
		else {_model->advance(0.045f);
				DeathView += 0.01f;
				if(DeathView >=2 && drown == false){
					_model->setAnimation("res");
					G = G + 0.5f;
					drown = true;
				}
				if(drown){
					_guyPosY-=0.1f;
					if(_guyPosY<-2)
						exit(0);
				}
		}
	}
	if(Win){
				_model->advance(0.045f);
				DeathView -= 0.01f;
				if(DeathView < -10)
					exit(0);
		}
	 //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME))
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	if(!Start && !Story && !Win){
	//  Calculate time passed
	int TtimeInterval = currentTime - TpreviousTime;
	if(TtimeInterval > 1000 && !Die){
		TpreviousTime = currentTime;
	TIndex--;
	if(TIndex<=0 && !Die){ 
		Die = true;
		drown = true;
		jump = false;
		_model->setAnimation("pain");
	}
	}
	}
	if(Story){
	int StimeInterval = currentTime - SpreviousTime;
	if(StimeInterval > 15000){
		SpreviousTime = currentTime;
		SIndex++;
		_Angle = 220;
		Xtext = -5;
		if(SIndex > 10){
			Story = false;
			TIndex = MaxTime;
		}
	}
	}
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

void handleKeypress2(int key, int x, int y) {
	if(!Die && !Win){
    if (key == GLUT_KEY_UP){
			a=0;
			_guyPosZ += 0.8f;
		if (_model != NULL) {
			_model->advance(0.025f);
		}
		}
    if (key == GLUT_KEY_DOWN){
			a=180;
			_guyPosZ -= 0.8f;
		if (_model != NULL) {
			_model->advance(0.025f);
		}
	}
    if (key == GLUT_KEY_LEFT){
			a=-90;
			_guyPosX += 0.8f;
		if (_model != NULL) {
			_model->advance(0.025f);
		}
		}
    if (key == GLUT_KEY_RIGHT){
			a=90;
			_guyPosX -= 0.8f;
		if (_model != NULL) {
			_model->advance(0.025f);
		}
		}
	}
}

void tileFunction(){
	tiles.resize(num_tiles);
	//Height 1, z gap = 20, x gap = 25
	//1st tile
	tiles[0].push_back(X);
	tiles[0].push_back(G);
	tiles[0].push_back(5);
	//2nd tile
	tiles[1].push_back(X);
	tiles[1].push_back(G);
	tiles[1].push_back(25);
	//3rd tile
	tiles[2].push_back(X);
	tiles[2].push_back(G);
	tiles[2].push_back(45);
	//4th tile
	tiles[3].push_back(X1);
	tiles[3].push_back(G);
	tiles[3].push_back(45);
	//5th tile
	tiles[4].push_back(X2);
	tiles[4].push_back(G);
	tiles[4].push_back(45);
	//6th tile
	tiles[5].push_back(X2);
	tiles[5].push_back(G);
	tiles[5].push_back(65);
	//7th tile
	tiles[6].push_back(X2);
	tiles[6].push_back(G);
	tiles[6].push_back(85);
	
	//Height 2, changing the x,z distance by 35,30 on transition
	//8th tile
	tiles[7].push_back(X2);
	tiles[7].push_back(G1);
	tiles[7].push_back(115);
	//9th tile
	tiles[8].push_back(X1);
	tiles[8].push_back(G1);
	tiles[8].push_back(115);
	
	//Height 3, changing the x,z distance by 35,30 on transition
	//10th tile
	tiles[9].push_back(X1);
	tiles[9].push_back(G2);
	tiles[9].push_back(145);
	//11th tile
	tiles[10].push_back(X1);
	tiles[10].push_back(G2);
	tiles[10].push_back(165);
	//12th tile
	tiles[11].push_back(X1);
	tiles[11].push_back(G2);
	tiles[11].push_back(185);
	//13th tile
	tiles[12].push_back(X1);
	tiles[12].push_back(G2);
	tiles[12].push_back(205);
	//14th tile
	tiles[13].push_back(X);
	tiles[13].push_back(G2);
	tiles[13].push_back(205);
	//15th tile
	tiles[14].push_back(X);
	tiles[14].push_back(G2);
	tiles[14].push_back(225);
	//16th tile
	tiles[15].push_back(X);
	tiles[15].push_back(G3);
	tiles[15].push_back(255);
	//17th tile
	tiles[16].push_back(X-25);
	tiles[16].push_back(G3);
	tiles[16].push_back(255);
	//18th tile
	tiles[17].push_back(X-25);
	tiles[17].push_back(G3);
	tiles[17].push_back(275);
	//19th tile
	tiles[18].push_back(X-25);
	tiles[18].push_back(G3);
	tiles[18].push_back(275+moveTileZ+20);
	//20th tile
	tiles[19].push_back(X);
	tiles[19].push_back(G3);
	tiles[19].push_back(275+moveTileZ+20);
	//21st tile
	tiles[20].push_back(X);
	tiles[20].push_back(G4);
	tiles[20].push_back(275+moveTileZ+50);
	//22nd tile
	tiles[21].push_back(X);
	tiles[21].push_back(G4);
	tiles[21].push_back(275+moveTileZ+70);
	//23rd tile
	tiles[22].push_back(X);
	tiles[22].push_back(G4);
	tiles[22].push_back(275+moveTileZ+90);
	//24th tile
	tiles[23].push_back(X);
	tiles[23].push_back(G4);
	tiles[23].push_back(275+2*moveTileZ+110);
	//25th tile
	tiles[24].push_back(X);
	tiles[24].push_back(G4);
	tiles[24].push_back(275+2*moveTileZ+130);
	//26th tile
	tiles[25].push_back(X);
	tiles[25].push_back(G5);
	tiles[25].push_back(275+2*moveTileZ+150);
	//27th tile
	tiles[26].push_back(X);
	tiles[26].push_back(G5);
	tiles[26].push_back(275+2*moveTileZ+180);
	//28th tile
	tiles[27].push_back(X);
	tiles[27].push_back(G6);
	tiles[27].push_back(275+2*moveTileZ+200);
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(5000, 1000);
	
	glutCreateWindow("The Lost World");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0, 0, 0);
    glLineWidth(3.0);
	initRendering();
	tileFunction();
	_scale = computeScale0(STRS0);
	_Scale = computeScale(STRS);
    glutVisibilityFunc(visible);
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleKeypress2);
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}
