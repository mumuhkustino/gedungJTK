#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BmpLoader.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#define SKYFRONT 0 // SKY Front ID = 0
#define SKYBACK  1 // SKY Back  ID = 1
#define SKYLEFT  2 // SKY Left  ID = 2
#define SKYRIGHT 3 // SKY Right ID = 3
#define SKYUP    4 // SKY Up    ID = 4
#define SKYDOWN  5 // SKY Down  ID = 5 

#define UBINPIXEL 0.5
#define FPS 120

#define TITIK_X_AWAL -50
#define TITIK_TENGAH_Z -10

#define PANJANGTIANG 1
#define LEBARTIANG 1
#define TINGGITIANG 13
#define CORLT 3.25

#define PANJANG_RUANG_KECIL 15

#define UBIN_ANTAR_TIANG 14
#define JARAK_TIANG_KORIDOR 20
#define LEBAR_GEDUNG 70
#define Z_RUANGAN_BLKG 30
#define Z_RUANGAN_DEPAN 50
#define Z_TENGAH_LORONG 40

#define DERETAN_TIANG_PERTAMA 8
#define DERETAN_TIANG_KEDUA 20
#define DERETAN_TIANG_KETIGA 27

//#define COLOR1 0
//#define COLOR2 1
//#define COLOR3 0

#define COLOR1 0.89
#define COLOR2 0.831
#define COLOR3 0.761

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

void LoadTextureA(const char* filename, int index, unsigned int A[]) {
	BmpLoader bl(filename);
	glGenTextures(1, &A[index]);
	glBindTexture(GL_TEXTURE_2D, A[index]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}

float convertToPx(float value) {
	return value*UBINPIXEL;
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
//	glBindTexture(GL_TEXTURE_2D, ID[SKYFRONT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);	
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glVertex3f(x,		  y,		z+length);
		glVertex3f(x,		  y+height, z+length);
		glVertex3f(x+width, y+height, z+length); 
		glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
//	glBindTexture(GL_TEXTURE_2D, ID[SKYBACK]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
		glVertex3f(x+width, y,		z);
		glVertex3f(x+width, y+height, z); 
		glVertex3f(x,		  y+height,	z);
		glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
//	glBindTexture(GL_TEXTURE_2D, ID[SKYLEFT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
		glVertex3f(x,		  y+height,	z);
		glVertex3f(x,		  y+height,	z+length); 
		glVertex3f(x,		  y,		z+length);
		glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
//	glBindTexture(GL_TEXTURE_2D, ID[SKYRIGHT]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
		glVertex3f(x+width, y,		z);
		glVertex3f(x+width, y,		z+length);
		glVertex3f(x+width, y+height,	z+length); 
		glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
//	glBindTexture(GL_TEXTURE_2D, ID[SKYUP]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glVertex3f(x+width, y+height, z);
		glVertex3f(x+width, y+height, z+length); 
		glVertex3f(x,		  y+height,	z+length);
		glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
//	glBindTexture(GL_TEXTURE_2D, ID[SKYDOWN]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);		
//		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
//		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
//		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length); 
//		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
		glVertex3f(x,		  y,		z);
		glVertex3f(x,		  y,		z+length);
		glVertex3f(x+width, y,		z+length); 
		glVertex3f(x+width, y,		z);
	glEnd();

}

void drawLabKiriKiriGedungLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
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
	float coorz;
	/*Depan*/
	for(i = 1; i < DERETAN_TIANG_PERTAMA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
	}
	
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
					
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
	}
	
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
					
	for(i = 21; i < DERETAN_TIANG_KETIGA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
	}
	/*Depan*/
	
	/*Sekat Lab Kiri*/
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	for(i = 2; i < DERETAN_TIANG_PERTAMA; i++) {
		if(i != DERETAN_TIANG_PERTAMA-1){
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
						convertToPx(0), convertToPx(TINGGITIANG),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
		}else {
//			glColor3f(0.89f, 0.831f, 0.761f);
			glTranslatef((TITIK_X_AWAL+convertToPx((UBIN_ANTAR_TIANG*i))), 0.0, (convertToPx(TITIK_TENGAH_Z)+coorz));
			glRotatef(-90, 1.0, 0.0, 0.0);
			glutSolidCylinder(convertToPx(3*PANJANGTIANG), convertToPx(TINGGITIANG), 20, convertToPx(TINGGITIANG));
			glRotatef(90, 1.0, 0.0, 0.0);
			glTranslatef(-(TITIK_X_AWAL+convertToPx((UBIN_ANTAR_TIANG*i))), 0.0, -(convertToPx(TITIK_TENGAH_Z)+coorz));
		}
	}
	/*Sekat Lab Kiri*/
	
	/*Depan Tengah Dalam*/
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_RUANGAN_DEPAN);
	//Lebar Kiri
	i=1;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	//
	i=8;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	}
	//Lebar Kanan
	i=26;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	//
	/*Depan Tengah Dalam*/
	
	/*Belakang Tengah Dalam*/
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + (convertToPx(Z_RUANGAN_BLKG));
	//Lebar Kiri
	i=1;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	//
	i=8;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	}
	//Lebar Kanan
	i=26;
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	//
	/*Belakang Tengah Dalam*/
	
	/*Sekat Lab Kanan*/
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	for(i = 20; i < DERETAN_TIANG_KETIGA-1; i++) {
		if(i < 23) {
//			glColor3f(0, 0, 1);
			glTranslatef((TITIK_X_AWAL+convertToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, (convertToPx(TITIK_TENGAH_Z)+coorz));
			glRotatef(-90, 1.0, 0.0, 0.0);
			glutSolidCylinder(convertToPx(2*PANJANGTIANG), convertToPx(TINGGITIANG), 20, convertToPx(TINGGITIANG));
			glRotatef(90, 1.0, 0.0, 0.0);
			glTranslatef(-(TITIK_X_AWAL+convertToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, -(convertToPx(TITIK_TENGAH_Z)+coorz));
		} else {
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
						convertToPx(0), convertToPx(TINGGITIANG),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
		}
	}
	/*Sekat Lab Kanan*/
	
	/*Belakang*/
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	for(i = 1; i < DERETAN_TIANG_PERTAMA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	}
	
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
					
	for(i = 9; i < DERETAN_TIANG_KEDUA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	}
	
	drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
					
	for(i = 21; i < DERETAN_TIANG_KETIGA; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
					convertToPx(0), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	}
	/*Belakang*/
	
	////////////	LORONG LUAR	VERTICAL	////////////	
	for(i = 1; i < 6; i++) {
		if(i != 1) {
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*12+*/(UBIN_ANTAR_TIANG*7)),
							TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*12+*/(UBIN_ANTAR_TIANG*7)),
							convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),	// i
							convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));	// i
		}
	
		drawTiang((TITIK_X_AWAL)+convertToPx(-PANJANGTIANG)+convertToPx(/*15+*/22+(UBIN_ANTAR_TIANG*7)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*15+*/22+(UBIN_ANTAR_TIANG*7)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*19)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*19)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
		if(i != 1) {
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*28+*/33+(UBIN_ANTAR_TIANG*19)),
							TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*28+*/33+(UBIN_ANTAR_TIANG*19)),
							convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
							convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
		}
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*29)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*29)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*28+*/26+(UBIN_ANTAR_TIANG*30)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*28+*/26+(UBIN_ANTAR_TIANG*30)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
	}
	
	for(i = 0; i > -5 ; i--) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*29)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*25+*/13+(UBIN_ANTAR_TIANG*29)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*28+*/26+(UBIN_ANTAR_TIANG*30)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*28+*/26+(UBIN_ANTAR_TIANG*30)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*i));
	}

	////////////	LORONG LUAR	VERTICAL	////////////
	
	////////////	LORONG LUAR	HORIZONTAL	////////////
	for(i = 7; i < 14; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5));
	}
	for(i = 16; i < 22; i++) {
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
						
		drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((JARAK_TIANG_KORIDOR*i)),
						convertToPx(0), convertToPx(TINGGITIANG)-convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5));
	}
	////////////	LORONG LUAR	HORIZONTAL	////////////
	
}

void drawRuanganPggrLabKiriKiri(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuanganPggrLabKiriKanan(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuanganMedKiri(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawKelas2C(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawDepanKelas2C(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawTeknisi(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawTU(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawLabTengah(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawDepanLabTengah(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawPinggirLabTengah(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawLabSBDTengah(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawKelasDepanLabSBD(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawWCLelakiLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawWCWanitaLt1(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangMushola(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangDekatMushola(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangDepanMushola(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangDekatDepanMushola(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawAtapVertical(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawAtapHorizontal(float x1, float x2, float y1, float y2, float z1, float z2) {
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


void drawAtapKoridor() {
	drawAtapHorizontal(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(-4)+convertToPx((UBIN_ANTAR_TIANG*7)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(30+(UBIN_ANTAR_TIANG*30)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)-convertToPx(4),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5)+convertToPx(4));
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(-4)+convertToPx((UBIN_ANTAR_TIANG*7)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(17+(UBIN_ANTAR_TIANG*8)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0)+convertToPx(6),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+(UBIN_ANTAR_TIANG*19)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(30+(UBIN_ANTAR_TIANG*20)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0)+convertToPx(6),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+(UBIN_ANTAR_TIANG*29)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(30+(UBIN_ANTAR_TIANG*30)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*(-4)),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));					
}

void drawTembok(float x1, float x2, float y1, float y2, float z1, float z2) {
	glBegin(GL_QUADS);	
	//Front
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	glEnd();
}

void drawTembokKanan(float x1, float x2, float y1, float y2, float z1, float z2) {
	glBegin(GL_QUADS);	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLantai(float x1, float x2, float y1, float y2, float z1, float z2) {
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
	glColor3f(COLOR1,COLOR2,COLOR3);
	glBegin(GL_QUADS);
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);
	glEnd();
	glColor3f(0,0,0);
	glBegin(GL_QUADS);	
	//Down
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLantai2(float x1, float x2, float y1, float y2, float z1, float z2) {
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
	glColor3f(COLOR1,COLOR2,COLOR3);
	glBegin(GL_QUADS);
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);
	glEnd();
	glColor3f(1,5,0);
	glBegin(GL_QUADS);	
	//Down
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1); 
		glVertex3f(x2, y1, z2);
	glEnd();
}


void drawGedungLt1() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor3f(0,0,0);
	drawLantai(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	glColor3f(0, 1, 1);
	drawTiangJTK();
	glColor3f(1, 0, 2);
	
	drawLabKiriKiriGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKiriKananGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawLabKananKananGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKananKiriGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawRuanganPggrLabKiriKiri(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL));
	
	drawTembok(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6));
			
	drawRuanganPggrLabKiriKanan(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawRuanganMedKiri(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					

	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	
	drawDepanKelas2C(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawKelas2C(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
			
	drawTeknisi(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	drawTU(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawLabTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawDepanLabTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawLabSBDTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawKelasDepanLabSBD(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawWCLelakiLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));
	
	drawWCWanitaLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawTembok(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(6));
				
	drawRuangDekatMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(23),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));
	drawRuangDekatMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	drawRuangMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*25),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(23),
				convertToPx(TITIK_TENGAH_Z)+coorz);

	drawRuangDepanMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*25),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(54));
	
	drawRuangDekatDepanMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(54));
				
	drawTembok(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22),
					convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6));
				
	drawAtapKoridor();
				
}

void drawRuangDosenBsr(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangDosenKcl(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangDapurDosen(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangPgrDapurDosen(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawPgrRuangServer(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangServer(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawPgrGudangLt2(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawGudangLt2(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangRapat(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawDpnRuangRapat(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangKelasOS(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawPgrRuangElib(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawWCLelakiLt2(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawWCWanitaLt2(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangElib(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangKelasDpnRSG(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangKelasPgrKantin(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangRSG(float x1, float x2, float y1, float y2, float z1, float z2) {
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

void drawRuangPgrRSG(float x1, float x2, float y1, float y2, float z1, float z2) {
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


void drawGedungLt2() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor3f(1,5,0);
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	//Kanan Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	//Tangga Kanan Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	//Tengah Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	//Tangga Kiri Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	//Kiri Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(-8)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(TINGGITIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
					
					
	glColor3f(1, 0, 2);
	
	drawRuangDosenBsr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawTembok(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
					
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	
	drawRuangDosenKcl(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
					
	drawRuangDapurDosen(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
					
	drawRuangPgrDapurDosen(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrRuangServer(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawRuangServer(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	drawGudangLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrGudangLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawRuangRapat(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	drawDpnRuangRapat(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawRuangKelasOS(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrRuangElib(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawRuangElib(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawTembok(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z));
	
	drawWCLelakiLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawWCWanitaLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));

	drawRuangKelasDpnRSG(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*23),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26 ),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawRuangKelasPgrKantin(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20), //Lebih kiri dari tiang
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*23),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawTembok(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);

	drawRuangRSG(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21), //Tengah Tengah tiang
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
			
	drawTembokKanan(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG));
	
	drawRuangPgrRSG(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(10),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));

}

void drawTanggaSelatan(float x1, float x2, float y1, float y2, float z1, float z2) {
	/*//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z2);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z2);
	
	//Right
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y2, z2); 
	
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);

	glEnd();*/
	//Front
	glBegin(GL_QUADS);	
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z1);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z1);
	
	//Left
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z1); 
		glVertex3f(x1, y1, z1);
	
	//Right
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y2, z2);
	
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);
	
	//Down
//		glVertex3f(x1, y1, z2);
//		glVertex3f(x1, y1, z1);
//		glVertex3f(x2, y1, z1); 
//		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawTanggaUtara(float x1, float x2, float y1, float y2, float z1, float z2) {
	
	glBegin(GL_QUADS);	
	//Front
		glVertex3f(x1, y1, z2);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x1, y2, z2);
	
	//Back
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x2, y1, z2);
	
	//Left
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y2, z2);
	
	//Right
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z1);
		glVertex3f(x2, y2, z2); 
		glVertex3f(x2, y2, z2); 
	
	//Up
		glVertex3f(x1, y2, z2);
		glVertex3f(x2, y2, z2);
		glVertex3f(x2, y2, z1); 
		glVertex3f(x1, y2, z1);
	glEnd();
}

void drawTangga() {

	glColor3f(1,0,0);
	drawTanggaUtara(20, 25, 20, 25, 20, 25);
	glColor3f(0,1,0);
	drawTanggaSelatan(40, 45, 40, 45, 40, 45);
	
}

void drawAtapGedungHorizontal(float x1, float x2, float y1, float y2, float z1, float z2, float p1, float p2){
 	glBegin(GL_QUADS);
	  	glVertex3f(x1, y1, z1);
	  	glVertex3f(x2, y2, z2);
	  	glVertex3f(x2+p2, y2, z2);
		glVertex3f(x1+p1, y1, z1);
	glEnd();
}

void drawAtapGedungVertical(float x1, float x2, float y1, float y2, float z1, float z2, float p1, float p2){
	glBegin(GL_QUADS);
	   	glVertex3f(x1, y1, z1);
	   	glVertex3f(x2, y2, z2);
	   	glVertex3f(x2, y2, z2+p2);
   		glVertex3f(x1, y1, z1+p1);
	glEnd();
}

void drawLangitLangit(float x1, float x2, float y1, float y2, float z1, float z2) {
	//Down
	glBegin(GL_QUADS);
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawAtapJTK() {
	
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor3f(0,0,0);
	drawLantai(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawLangitLangit(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT),convertToPx(2*TINGGITIANG)+convertToPx(2+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	
	glColor3f(0.2,0.2,0.2);
	//Bagian Atap Bawah
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0), 
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2), 
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN), 
						convertToPx(34+UBIN_ANTAR_TIANG*26), convertToPx(26+UBIN_ANTAR_TIANG*23));	//DEPAN
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0), 
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2), 
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG), 
						convertToPx(34+UBIN_ANTAR_TIANG*26), convertToPx(26+UBIN_ANTAR_TIANG*23)); //BELAKANG
						
	drawAtapGedungVertical(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2),
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBAR_GEDUNG)-convertToPx(16),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(10)); //KIRI
	drawAtapGedungVertical(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*24),
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBAR_GEDUNG)-convertToPx(16),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(10)); //KANAN
	
	 //Bagian Atap Atas
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2),
							TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
							convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(20+3*CORLT),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(10),
							convertToPx(26+UBIN_ANTAR_TIANG*23), convertToPx(26+UBIN_ANTAR_TIANG*25));//DEPAN
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2),
							TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
							convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(20+3*CORLT),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
							convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG)+convertToPx(10),
							convertToPx(26+UBIN_ANTAR_TIANG*23), convertToPx(26+UBIN_ANTAR_TIANG*25));//BELAKANG
}

void drawGedungJTK() {
	drawSkybox(0, 0, 0, 500, 500, 500);
	drawGrid();
	
//	glColor3f(0.89f, 0.831f, 0.761f);
//	glColor3f(COLOR1, COLOR2, COLOR3);
	
	drawGedungLt1();
	drawGedungLt2();
	drawAtapJTK();
	
	drawTangga();
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

//	LoadTexture("front.bmp", SKYFRONT);
//	LoadTexture("back.bmp", SKYBACK);
//	LoadTexture("down.bmp", SKYDOWN);
//	LoadTexture("up.bmp", SKYUP);
//	LoadTexture("right.bmp", SKYRIGHT);
//	LoadTexture("left.bmp", SKYLEFT); 
	
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
