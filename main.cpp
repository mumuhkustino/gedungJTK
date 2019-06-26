#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BmpLoader.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#define SKYFRONT 0 // Give Front ID = 0
#define SKYBACK  1 // Give Back  ID = 1
#define SKYLEFT  2 // Give Left  ID = 2
#define SKYRIGHT 3 // Give Right ID = 3
#define SKYUP    4 // Give Up    ID = 4
#define SKYDOWN  5 // Give Down  ID = 5 

#define UBINPIXEL 0.5
#define FPS 120

#define TITIK_X_AWAL -50
#define TITIK_TENGAH_Z -10

#define PANJANGTIANG 1
#define LEBARTIANG 1
#define TINGGITIANG 14

#define UBIN_ANTAR_TIANG 14
#define LEBAR_GEDUNG 70
#define Z_RUANGAN_BLKG 30
#define Z_RUANGAN_DEPAN 50
#define Z_TENGAH_LORONG 40

#define DERETAN_TIANG_PERTAMA 8
#define DERETAN_TIANG_KEDUA 20
#define DERETAN_TIANG_KETIGA 27

#define COLOR1 1
#define COLOR2 0
#define COLOR3 0

//#define COLOR1 0.89
//#define COLOR2 0.831
//#define COLOR3 0.761

unsigned int ID[6];

// angle of rotation for the camera direction
float angle = 0.0f;


// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=-1.0f;

// XZ position of the camera
float x=0.0f, y=10.0f, z=15.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

void LoadTexture(const char* filename, int index) {
	BmpLoader bl(filename);
	glGenTextures(1, &ID[index]);
	glBindTexture(GL_TEXTURE_2D, ID[index]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}

float convertUbinToPx(float ubin) {
	return ubin*UBINPIXEL;
}

void drawGrid() {															
	for(float i = -500; i <= 500; i += 5) {
		glBegin(GL_LINES);
			glColor3ub(150, 190, 150);						
			glVertex3f(-500, 0, i);					
			glVertex3f(500, 0, i);
			glVertex3f(i, 0,-500);							
			glVertex3f(i, 0, 500);
		glEnd();
	}
}

void drawSkybox(float x, float y, float z, float width, float height, float length) {
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, ID[SKYFRONT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
//		glVertex3f(x,		  y,		z+length);
//		glVertex3f(x,		  y+height, z+length);
//		glVertex3f(x+width, y+height, z+length); 
//		glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, ID[SKYBACK]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
//		glVertex3f(x+width, y,		z);
//		glVertex3f(x+width, y+height, z); 
//		glVertex3f(x,		  y+height,	z);
//		glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, ID[SKYLEFT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
//		glVertex3f(x,		  y+height,	z);
//		glVertex3f(x,		  y+height,	z+length); 
//		glVertex3f(x,		  y,		z+length);
//		glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, ID[SKYRIGHT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
//		glVertex3f(x+width, y,		z);
//		glVertex3f(x+width, y,		z+length);
//		glVertex3f(x+width, y+height,	z+length); 
//		glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, ID[SKYUP]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
//		glVertex3f(x+width, y+height, z);
//		glVertex3f(x+width, y+height, z+length); 
//		glVertex3f(x,		  y+height,	z+length);
//		glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, ID[SKYDOWN]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
//		glVertex3f(x,		  y,		z);
//		glVertex3f(x,		  y,		z+length);
//		glVertex3f(x+width, y,		z+length); 
//		glVertex3f(x+width, y,		z);
	glEnd();

}

void drawLabKiriKiriGedungLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3f(COLOR1, COLOR2, COLOR3);
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	
	//Up
//		glVertex3f(x1, y2, z2);
//		glVertex3f(x2, y2, z2);
//		glVertex3f(x2, y2, z1); 
//		glVertex3f(x1, y2, z1);
	
	//Down
//		glVertex3f(x1, y1, z2);
//		glVertex3f(x1, y1, z1);
//		glVertex3f(x2, y1, z1); 
//		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLabKiriKananGedungLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3f(COLOR1, COLOR2, COLOR3);
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLabKananKananGedungLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3f(COLOR1, COLOR2, COLOR3);
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLabKananKiriGedungLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3f(COLOR1, COLOR2, COLOR3);
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawTiang(float x1, float x2, float y1, float y2, float z1, float z2) {
	
//	glColor3f(0.89f, 0.831f, 0.761f);
	glColor3f(COLOR1, COLOR2, COLOR3);
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);
	
	//Down
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawTiangJTK() {
	int i;
	int coorz;
	/*Depan*/
	for(i = 1; i < DERETAN_TIANG_PERTAMA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z));
	}
	
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z));
					
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z));
	}
	
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z));
					
	for(i = 21; i < DERETAN_TIANG_KETIGA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z));
	}
	/*Depan*/
	
	/*Sekat Lab Kiri*/
	coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG) + convertUbinToPx(Z_TENGAH_LORONG);
	for(i = 2; i < DERETAN_TIANG_PERTAMA; i++) {
		if(i != DERETAN_TIANG_PERTAMA-1){
			drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
						TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
						convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
						convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
						convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
		}else {
			glColor3f(0.89f, 0.831f, 0.761f);
			glTranslatef((TITIK_X_AWAL+convertUbinToPx((UBIN_ANTAR_TIANG*i))), 0.0, (convertUbinToPx(TITIK_TENGAH_Z)+coorz));
			glRotatef(-90, 1.0, 0.0, 0.0);
			glutSolidCylinder(convertUbinToPx(3*PANJANGTIANG), convertUbinToPx(TINGGITIANG), 20, convertUbinToPx(TINGGITIANG));
			glRotatef(90, 1.0, 0.0, 0.0);
			glTranslatef(-(TITIK_X_AWAL+convertUbinToPx((UBIN_ANTAR_TIANG*i))), 0.0, -(convertUbinToPx(TITIK_TENGAH_Z)+coorz));
		}
	}
	/*Sekat Lab Kiri*/
	
	/*Depan Tengah Dalam*/
	coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG) + convertUbinToPx(Z_RUANGAN_DEPAN);
	//Lebar Kiri
	i=1;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	//
	i=8;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	}
	//Lebar Kanan
	i=26;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	//
	/*Depan Tengah Dalam*/
	
	/*Belakang Tengah Dalam*/
	coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG) + (convertUbinToPx(Z_RUANGAN_BLKG));
	//Lebar Kiri
	i=1;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	//
	i=8;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	}
	//Lebar Kanan
	i=26;
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	//
	/*Belakang Tengah Dalam*/
	
	/*Sekat Lab Kanan*/
	coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG) + convertUbinToPx(Z_TENGAH_LORONG);
	for(i = 20; i < DERETAN_TIANG_KETIGA-1; i++) {
		if(i < 23) {
			glColor3f(0, 0, 1);
			glTranslatef((TITIK_X_AWAL+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, (convertUbinToPx(TITIK_TENGAH_Z)+coorz));
			glRotatef(-90, 1.0, 0.0, 0.0);
			glutSolidCylinder(convertUbinToPx(3*PANJANGTIANG), convertUbinToPx(TINGGITIANG), 20, convertUbinToPx(TINGGITIANG));
			glRotatef(90, 1.0, 0.0, 0.0);
			glTranslatef(-(TITIK_X_AWAL+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, -(convertUbinToPx(TITIK_TENGAH_Z)+coorz));
		} else {
			drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
						TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
						convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
						convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
						convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
		}
	}
	/*Sekat Lab Kanan*/
	
	/*Belakang*/
	coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG);
	for(i = 1; i < DERETAN_TIANG_PERTAMA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx((UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	}
	
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
					
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	}
	
	drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
					
	for(i = 21; i < DERETAN_TIANG_KETIGA; i++) {
		drawTiang(TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	}
	/*Belakang*/
}

void drawGedungLt1() {
	int coorz = convertUbinToPx(TITIK_TENGAH_Z) - convertUbinToPx(LEBAR_GEDUNG) + convertUbinToPx(Z_TENGAH_LORONG);
	drawLabKiriKiriGedungLt1(TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(UBIN_ANTAR_TIANG*6),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKiriKananGedungLt1(TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(UBIN_ANTAR_TIANG*6),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz-convertUbinToPx(Z_TENGAH_LORONG));
					
	drawLabKananKananGedungLt1(TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+UBIN_ANTAR_TIANG*26),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z),
					convertUbinToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKananKiriGedungLt1(TITIK_X_AWAL+convertUbinToPx(PANJANGTIANG)+convertUbinToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertUbinToPx(-PANJANGTIANG)+convertUbinToPx(26+UBIN_ANTAR_TIANG*26),
					convertUbinToPx(0), convertUbinToPx(TINGGITIANG),
					convertUbinToPx(TITIK_TENGAH_Z)+coorz,
					convertUbinToPx(-LEBARTIANG)+convertUbinToPx(TITIK_TENGAH_Z)+coorz-convertUbinToPx(Z_TENGAH_LORONG));
}

void drawGedungJTK() {
	drawSkybox(0, 0, 0, 500, 500, 500);
	drawGrid();
	
	drawTiangJTK();
	drawGedungLt1();
	
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 1000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

void renderScene(void) {
	glEnable(GL_TEXTURE_2D);

	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, y, z,
			x+lx, y+ly,  z+lz,
			0.0f, 1.0f,  0.0f);
	
//	LoadTexture("front.bmp", SKYFRONT);
//	LoadTexture("back.bmp", SKYBACK);
//	LoadTexture("down.bmp", SKYDOWN);
//	LoadTexture("up.bmp", SKYUP);
//	LoadTexture("right.bmp", SKYRIGHT);
//	LoadTexture("left.bmp", SKYLEFT); 

	drawGedungJTK();

	glutSwapBuffers();
	glDisable(GL_TEXTURE_2D);
	
} 

void processNormalKeys(unsigned char key, int xx, int yy) { 	

        if (key == 27)
              exit(0);
}

void pressKey(int key, int xx, int yy) {

       switch (key) {
       		 case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
			 case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
             case GLUT_KEY_UP : deltaMove = 0.5f; break;
             case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
             case GLUT_KEY_PAGE_UP : ly += 0.25f; break;
             case GLUT_KEY_PAGE_DOWN : ly -= 0.25f; break;
             case GLUT_KEY_HOME : y += 1.0f; break;
             case GLUT_KEY_END : y -= 1.0f; break;
             case GLUT_KEY_F1 : x -= 1.0f; break;
             case GLUT_KEY_F2 : x += 1.0f; break;
       }
}

void releaseKey(int key, int x, int y) { 	

        switch (key) {
             case GLUT_KEY_LEFT :
			 case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
			 case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0;break;
        }
} 

//void timer(int) {
//    glutPostRedisplay();
//    glutTimerFunc(1000/FPS,timer,0);
//}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,768);
	glutCreateWindow("GedungJTK");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(pressKey);
	
//	glutIgnoreKeyRepeat(1);
//	glutKeyboardFunc(processNormalKeys);
	glutSpecialUpFunc(releaseKey);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glutTimerFunc(0,timer,0);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
