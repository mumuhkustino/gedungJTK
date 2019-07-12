#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "BmpLoader.h"
#include "Constants.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

unsigned int ID[6];
unsigned int A[ARRTX];

int cx, cy;

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=-1.0f;

// XZ position of the camera
float x=0.0f, y=10.0f, z=75.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;

bool lt1 = true, lt2 = true, atap = true, tiang = true;

void LoadTexture(const char* filename, int index) {
	BmpLoader bl(filename);
	glGenTextures(1, &ID[index]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, ID[index]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}

void LoadTextureA(const char* filename, int index) {
	BmpLoader bl(filename);
	glGenTextures(1, &A[index]);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, ID[SKYFRONT]);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Draw Back side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID[SKYBACK]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Draw Left side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID[SKYLEFT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Draw Right side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID[SKYRIGHT]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Draw Up side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID[SKYUP]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	// Draw Down side
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ID[SKYDOWN]);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  0,		z);
		glTexCoord2f(100.0f, 0.0f); glVertex3f(x,		  0,		z+length);
		glTexCoord2f(100.0f, 100.0f); glVertex3f(x+width, 0,		z+length); 
		glTexCoord2f(0.0f, 100.0f); glVertex3f(x+width, 0,		z);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
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
						convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
		}else {
				glTranslatef((TITIK_X_AWAL+convertToPx((UBIN_ANTAR_TIANG*i))), 0.0, (convertToPx(TITIK_TENGAH_Z)+coorz));
				glRotatef(-90, 1.0, 0.0, 0.0);
				glutSolidCylinder(convertToPx(3*PANJANGTIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT), 20, convertToPx(TINGGITIANG));
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
				glColor3ub(222,184,135);
				glTranslatef((TITIK_X_AWAL+convertToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, (convertToPx(TITIK_TENGAH_Z)+coorz));
				glRotatef(-90, 1.0, 0.0, 0.0);
				glutSolidCylinder(convertToPx(2*PANJANGTIANG), convertToPx(TINGGITIANG)+convertToPx(CORLT), 20, convertToPx(TINGGITIANG));
				glRotatef(90, 1.0, 0.0, 0.0);
				glTranslatef(-(TITIK_X_AWAL+convertToPx(26+(UBIN_ANTAR_TIANG*i))), 0.0, -(convertToPx(TITIK_TENGAH_Z)+coorz));
		} else {
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*i)),
						convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT),
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
			drawTiang(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(/*28+*/22+(UBIN_ANTAR_TIANG*20)),
							TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(/*28+*/22+(UBIN_ANTAR_TIANG*20)),
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
	//KIRI
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(-4)+convertToPx((UBIN_ANTAR_TIANG*7)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*8)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*2)/*+convertToPx(6),*/,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
	drawAtapVertical(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*7)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*8)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0)/*+convertToPx(6),*/,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*2));
	//Kanan
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*19)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(30+(UBIN_ANTAR_TIANG*20)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*2)/*+convertToPx(6),*/,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*19)),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*20)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0)/*+convertToPx(6),*/,
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*2));
						
	drawAtapVertical(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+(UBIN_ANTAR_TIANG*29)),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(30+(UBIN_ANTAR_TIANG*30)),
						convertToPx(TINGGITIANG)-convertToPx(CORLT),convertToPx(TINGGITIANG)-convertToPx(CORLT/2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*(-4)),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4));					
}

void drawTexDpn(float x1, float x2, float y1, float y2, float z1, float z2, char action) {
	if(action == '1') {
		glBegin(GL_QUADS);	
		//Front
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x1, y1, z2);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x2, y1, z2);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x2, y2, z2); 
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x1, y2, z2);
		glEnd();
	} else {
		glBegin(GL_QUADS);
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);
		glEnd();
	}
}

void drawTexPgr(float x1, float x2, float y1, float y2, float z1, float z2) {
	glBegin(GL_QUADS);
	//Right
		glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z1);
		glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z1);
		glTexCoord2f(1.0, 1.0);glVertex3f(x2, y2, z2); 
		glTexCoord2f(1.0, 0.0);glVertex3f(x2, y1, z2);
	glEnd();
}

void drawLantai(float x1, float x2, float y1, float y2, float z1, float z2, char action) {
	if(action == '1') {
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
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[TERAS]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(50.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(50.0, 30.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 30.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_QUADS);	
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	} else if(action == '3'){
		glColor3ub(222,184,135);
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
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[TERAS]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(50.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(50.0, 30.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 30.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[ATAPLT1]);
		glBegin(GL_QUADS);
		//Down
			glTexCoord2f(50.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(50.0, 20.0);glVertex3f(x1, y1, z1);
			glTexCoord2f(0.0, 20.0);glVertex3f(x2, y1, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

void drawLantai2(float x1, float x2, float y1, float y2, float z1, float z2, char action) {
	if(action == 'D') {
		glColor3ub(222,184,135);
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
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[TERAS]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(50.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(50.0, 30.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 30.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
//		glColor3ub(238,232,170);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[ATAPLT1]);
		glBegin(GL_QUADS);	
		//Down
			glTexCoord2f(50.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(50.0, 20.0);glVertex3f(x1, y1, z1);
			glTexCoord2f(0.0, 20.0);glVertex3f(x2, y1, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	} else {
		glColor3ub(222,184,135);
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
		glEnable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, A[TERAS]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(5, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(5, 5);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 5);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glColor3ub(222,184,135);
		glBegin(GL_QUADS);
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	}
}

void drawTaman(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
	glBegin(GL_QUADS);
	//Front
	glBegin(GL_QUADS);	
		glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z2);
		glTexCoord2f(50.0, 0.0);glVertex3f(x2, y1, z2);
		glTexCoord2f(50.0, 1.0);glVertex3f(x2, y2, z2); 
		glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z2);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
	glBegin(GL_QUADS);
	//Back
		glTexCoord2f(50.0, 0.0);glVertex3f(x1, y1, z1);
		glTexCoord2f(50.0, 1.0);glVertex3f(x1, y2, z1);
		glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z1); 
		glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z1);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
	glBegin(GL_QUADS);
	//Left
		glTexCoord2f(5.0, 0.0);glVertex3f(x1, y1, z2);
		glTexCoord2f(5.0, 1.0);glVertex3f(x1, y2, z2);
		glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z1); 
		glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z1);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_2D);
  	glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
	glBegin(GL_QUADS);
	//Right
		glTexCoord2f(5.0, 0.0);glVertex3f(x2, y1, z1);
		glTexCoord2f(5.0, 1.0);glVertex3f(x2, y2, z1);
		glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z2); 
		glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z2);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[RUMPUT]);
	glBegin(GL_QUADS);
	//Up
		glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
		glTexCoord2f(10.0, 0.0);glVertex3f(x2, y2, z2);
		glTexCoord2f(10.0, 10.0);glVertex3f(x2, y2, z1); 
		glTexCoord2f(0.0, 10.0);glVertex3f(x1, y2, z1);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	//Down
		glVertex3f(x1, y1, z2);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1); 
		glVertex3f(x2, y1, z2);
	glEnd();
}

void drawJlnKoridor(float x1, float x2, float y1, float y2, float z1, float z2, char action) {
	if(action == 'H') {
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Front
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(50.0, 0.0);glVertex3f(x2, y1, z2);
			glTexCoord2f(50.0, 1.0);glVertex3f(x2, y2, z2); 
			glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Back
			glTexCoord2f(50.0, 0.0);glVertex3f(x1, y1, z1);
			glTexCoord2f(50.0, 1.0);glVertex3f(x1, y2, z1);
			glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Left
			glTexCoord2f(5.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(5.0, 1.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_TEXTURE_2D);
  		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Right
			glTexCoord2f(5.0, 0.0);glVertex3f(x2, y1, z1);
			glTexCoord2f(5.0, 1.0);glVertex3f(x2, y2, z1);
			glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z2); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_TEXTURE_2D);
  		glBindTexture(GL_TEXTURE_2D, A[TERASKORIDOR]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(100.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(100.0, 10.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 10.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	} else {
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Front
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(1.0, 0.0);glVertex3f(x2, y1, z2);
			glTexCoord2f(1.0, 1.0);glVertex3f(x2, y2, z2); 
			glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Back
			glTexCoord2f(1.0, 0.0);glVertex3f(x1, y1, z1);
			glTexCoord2f(1.0, 1.0);glVertex3f(x1, y2, z1);
			glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Left
			glTexCoord2f(10.0, 0.0);glVertex3f(x1, y1, z2);
			glTexCoord2f(10.0, 1.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(0.0, 1.0);glVertex3f(x1, y2, z1); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y1, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_TEXTURE_2D);
  		glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		glBegin(GL_QUADS);
		//Right
			glTexCoord2f(10.0, 0.0);glVertex3f(x2, y1, z1);
			glTexCoord2f(10.0, 1.0);glVertex3f(x2, y2, z1);
			glTexCoord2f(0.0, 1.0);glVertex3f(x2, y2, z2); 
			glTexCoord2f(0.0, 0.0);glVertex3f(x2, y1, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		
		glEnable(GL_TEXTURE_2D);
  		glBindTexture(GL_TEXTURE_2D, A[TERASKORIDOR]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(10.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(10.0, 30.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 30.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	}
}


void drawPintuLt1() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTURUANGAN]);
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))+convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))+convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))+convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))+convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))-convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))-convertToPx(0.1),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))-convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx((UBIN_ANTAR_TIANG*6))-convertToPx(0.1),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(12));
				
		drawTexPgr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))+convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))+convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(2),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))+convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))+convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(2),
						convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))-convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))-convertToPx(0.1),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))-convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*23))-convertToPx(0.1),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(12));
		drawTexPgr(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)+convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22)+convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(56),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(61));
		drawTexPgr(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)-convertToPx(0.1),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22)-convertToPx(0.1),
						convertToPx(CORLT/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(56),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(61));
						
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*6),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*6),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),'1');
					
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*6),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*6),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),'1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*8),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*8),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),
				convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*8),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*8),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),
				convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL)-convertToPx(0.1),'1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*11),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*11),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*11),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*11),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*9),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*9),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*15),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*15),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*15),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*15),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*13),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*13),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*18),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*18),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*18),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*18),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*16),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*16),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(Z_TENGAH_LORONG)+coorz+convertToPx(18)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(Z_TENGAH_LORONG)+coorz+convertToPx(18)-convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)+convertToPx(0.1),'1');				
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)-convertToPx(0.1),'1');								

		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),'1');			
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19.5),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),'1');			
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19.5),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(1+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(5+UBIN_ANTAR_TIANG*19),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(-5)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-1)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(12)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(12)+convertToPx(0.1),'1');				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(-5)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-1)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(12)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(12)-convertToPx(0.1),'1');								
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(-5)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					TITIK_X_AWAL+convertToPx(-1)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(-5)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					TITIK_X_AWAL+convertToPx(-1)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(28+UBIN_ANTAR_TIANG*22),
				TITIK_X_AWAL+convertToPx(33+UBIN_ANTAR_TIANG*22),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(23)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(23)+convertToPx(0.1),'1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(28+UBIN_ANTAR_TIANG*22),
				TITIK_X_AWAL+convertToPx(33+UBIN_ANTAR_TIANG*22),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(23)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(23)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(28+UBIN_ANTAR_TIANG*22),
					TITIK_X_AWAL+convertToPx(33+UBIN_ANTAR_TIANG*22),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(54)+convertToPx(0.1),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(54)+convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(28+UBIN_ANTAR_TIANG*22),
					TITIK_X_AWAL+convertToPx(33+UBIN_ANTAR_TIANG*22),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(54)-convertToPx(0.1),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(54)-convertToPx(0.1),'1');
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}


void drawGedungLt1() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor3f(0.0f,0.0f,0.0f);
	drawLantai(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(CORLT/2),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG), '1');
	glColor3ub(222,184,135);
	
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+(UBIN_ANTAR_TIANG*7)),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(38+(UBIN_ANTAR_TIANG*29)),
					convertToPx(0),convertToPx(CORLT/2)-convertToPx(0.5),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)+convertToPx(1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*5)-convertToPx(1),
					'H');
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+(UBIN_ANTAR_TIANG*7)),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(6+(UBIN_ANTAR_TIANG*8)),
					convertToPx(0),convertToPx(CORLT/2)-convertToPx(0.5),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)+convertToPx(1),
					'V');
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(15+(UBIN_ANTAR_TIANG*19)),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(20+(UBIN_ANTAR_TIANG*20)),
					convertToPx(0),convertToPx(CORLT/2)-convertToPx(0.5),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)+convertToPx(1),
					'V');
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(15+(UBIN_ANTAR_TIANG*29)),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(24+(UBIN_ANTAR_TIANG*30)),
					convertToPx(0),convertToPx(CORLT/2)-convertToPx(0.5),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*(-4)),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)+convertToPx(1),
					'V');
	drawPintuLt1();
					
	glColor3ub(222,184,135);
	drawLabKiriKiriGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKiriKananGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));

	drawLabKananKananGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz);
	drawLabKananKiriGedungLt1(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz,
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawRuanganPggrLabKiriKiri(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(PANJANG_RUANG_KECIL));
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTU_24]);
	drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),'1');
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glColor3ub(222,184,135);
	drawRuanganPggrLabKiriKanan(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	drawRuanganMedKiri(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(PANJANG_RUANG_KECIL),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					

	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	
	drawDepanKelas2C(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawKelas2C(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
			
	drawTeknisi(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	drawTU(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawLabTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawDepanLabTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawLabSBDTengah(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawKelasDepanLabSBD(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawWCLelakiLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));
	drawWCLelakiLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(6+UBIN_ANTAR_TIANG*19),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(18));
	
	drawWCWanitaLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(TITIK_TENGAH_Z)+coorz);			
	drawWCWanitaLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19.5),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(4),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	drawWCWanitaLt1(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(4),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTU_24]);
	drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(3),
					convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(3),'1');
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glColor3ub(222,184,135);
	drawRuangDekatMushola(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(23),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));
	drawRuangDekatMushola(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(TITIK_TENGAH_Z)+coorz);
	drawRuangMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*25),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(23),
				convertToPx(TITIK_TENGAH_Z)+coorz);

	drawRuangDepanMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*25),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(54));
	
	drawRuangDekatDepanMushola(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*22),
				convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
				convertToPx(TITIK_TENGAH_Z),
				convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(54));
				
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTU_24]);
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
					convertToPx(CORLT/2), convertToPx(TINGGITIANG)+convertToPx(CORLT),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)-convertToPx(6),'1');
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PGRKORIDOR]);
		drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1)+convertToPx(-10),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(CORLT/4),
					convertToPx(TITIK_TENGAH_Z),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(10),'H');
		drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1)+convertToPx(-10),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(CORLT/4),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(10),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,'H');
		drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1)+convertToPx(-10),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					convertToPx(0), convertToPx(CORLT/4),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(10),
					convertToPx(TITIK_TENGAH_Z),'V');
		drawJlnKoridor(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
					convertToPx(0), convertToPx(CORLT/4),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(10),
					convertToPx(TITIK_TENGAH_Z),'V');
					
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[JENDELA]);
	for(int i = 1; i < 26; i++) {
		if(i < 7) {
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),'1');
		} else if (i > 7 && i < 19) {
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),'1');
			if(i != 18) {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),'1');
			}
		} else if (i > 19) {
			if(i < 23) {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)-convertToPx(0.1),'1');
			} else {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
			}
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
						convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(0.1),'1');
		}
	}
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	
	glColor3ub(139,69,19);
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

void drawPintuLt2() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTURUANGAN]);
	
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),	//r dosen
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(2),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(10));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(2),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(10));
				
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),	//r dosen
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(12),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(20));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(12),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(20));
				
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),	//r dosen
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)-convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(14));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6)+convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(6),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(14));
	
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*25)-convertToPx(0.1), //mushola
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*25)-convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(6));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*25)+convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*25)+convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(6));
					
		drawTexPgr(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20)+convertToPx(0.1),	//elib
					TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20)+convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(5),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(11));
		drawTexPgr(TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20)-convertToPx(0.1),
					TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20)-convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(5),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(11));
					
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20)-convertToPx(0.1),	//pinggir rsg kiri
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20)-convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(6));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20)+convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20)+convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(6));
		drawTexPgr(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)-convertToPx(0.1),	//pinggir rsg kanan
					TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)-convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(6));
		drawTexPgr(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)+convertToPx(0.1),
					TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21)+convertToPx(0.1),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(6));		
			
	
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*6),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*6),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(5+UBIN_ANTAR_TIANG*8),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*8),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(5+UBIN_ANTAR_TIANG*8),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(11+UBIN_ANTAR_TIANG*8),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),//wc lelaki lt2
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(8)-convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),'1');					
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(4+UBIN_ANTAR_TIANG*19.5),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19.5),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(4+UBIN_ANTAR_TIANG*19.5),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19.5),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(4)-convertToPx(0.1),'1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),//wc perempuan lt2
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(13)-convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*19),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)-convertToPx(0.1),'1');					
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(4+UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19.5),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)+convertToPx(0.1),'1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(4+UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*19.5),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(18)-convertToPx(0.1),'1');				
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*23),	//pinggir kantin
					TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*23),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*23),
					TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*23),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(32+UBIN_ANTAR_TIANG*21),	//rsg
				TITIK_X_AWAL+convertToPx(38+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(32+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(38+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*25),	//rsg
					TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*25),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*25),
					TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*25),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*25),	//depan rsg
				TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*25),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(18+UBIN_ANTAR_TIANG*25),
				TITIK_X_AWAL+convertToPx(22+UBIN_ANTAR_TIANG*25),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');				
//
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*11),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*11),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*11),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*11),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*9),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*9),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(3+UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(9+UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');

		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*15),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*15),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*15),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*15),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
				
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*18),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*18),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*18),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*18),
					convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)+convertToPx(Z_RUANGAN_BLKG)-convertToPx(0.1), '1');
		
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)+convertToPx(0.1), '1');
		drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(8+UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT), convertToPx(2*TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(Z_TENGAH_LORONG)-convertToPx(0.1), '1');
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}


void drawGedungLt2() {
	float coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG) + convertToPx(Z_TENGAH_LORONG);
	
	glColor3ub(222,184,135);
	
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8),'D');
					
	//Tangga Kanan Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8),'L');
	//Tangga Kiri Keluar
	drawLantai2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(CORLT), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(8), 'L');
	
	drawRuangDosenBsr(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG));
					
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, A[PINTU_24]);
	drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),'1');
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	drawPintuLt2();
	
	glColor3ub(222,184,135);
	
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	
	drawRuangDosenKcl(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
					
	drawRuangDapurDosen(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
					
	drawRuangPgrDapurDosen(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
					convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrRuangServer(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawRuangServer(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	drawGudangLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrGudangLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*9),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*12),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawRuangRapat(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	drawDpnRuangRapat(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*13),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawRuangKelasOS(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawPgrRuangElib(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*16),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawRuangElib(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
				
	drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),'0');
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[PINTU_24]);
	drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),'1');
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glColor3ub(222,184,135);
	
	drawWCLelakiLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(8),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	drawWCLelakiLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19.5),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(4),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	drawWCLelakiLt2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(4),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);	
	
	drawWCWanitaLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(13));
	drawWCWanitaLt2(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19.5),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(18));
	drawWCWanitaLt2(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*19.5),
				TITIK_X_AWAL+convertToPx(UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(18));

	drawRuangKelasDpnRSG(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*23),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26 ),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
				
	drawRuangKelasPgrKantin(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20), //Lebih kiri dari tiang
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*23),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz);
	
	drawTexDpn(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz,'0');
	
	glColor3ub(222,184,135);
	drawRuangMushola(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*25),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	drawRuangRSG(TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21), //Tengah Tengah tiang
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[JENDELA]);
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26)+convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26)+convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
				convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG));
		drawTexPgr(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26)-convertToPx(0.1),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*26)-convertToPx(0.1),
				convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
				convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG));
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glColor3ub(222,184,135);
	
	drawRuangPgrRSG(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)-convertToPx(10),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN));

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[JENDELALT2]);
	for(int i = 1; i < 26; i++) {
		if(i < 7) {
			if(i != 6) {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
			}
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),'1');
			drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),'1');
		} else if (i > 6 && i < 19) {
			if(i != 7) {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
			} else {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
			}
			if(i != 18) {
				if(i != 7) {
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),'1');
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),'1');
				} else {
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),'1');
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),'1');
				}
			}
		} else if (i > 18) {
			if(i != 19) {
				if(i != 20) {
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
					drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
								TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
								convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
								convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
				}
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG/2)+convertToPx(CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),'1');
			} else {
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
							TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
							convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT),
							convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),
							convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)-convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz-convertToPx(0.1),'1');
				drawTexDpn(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(13+UBIN_ANTAR_TIANG*i),
						TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*(i+1)),
						convertToPx(TINGGITIANG)+convertToPx(1.5*CORLT),
						convertToPx(TINGGITIANG)+convertToPx(TINGGITIANG)+convertToPx(2*CORLT),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),
						convertToPx(TITIK_TENGAH_Z)+convertToPx(-LEBARTIANG)+coorz+convertToPx(0.1),'1');
			}
		}
	}
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);

}

void drawTembokTangga(float x1, float x2, float y1, float y2, float z1, float z2, char action) {
	if(action == 'L') {//lurus
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
	} else if(action == 'M') { //miring
		//Front
		glBegin(GL_QUADS);
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);
			glVertex3f(x2, y2, z1); 
			glVertex3f(x1, y2, z1);
		//Back
			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y2, z1);
			glVertex3f(x2, y2, z1); 
			glVertex3f(x2, y1, z1);
		//Left
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y1, z1);
			glVertex3f(x1, y1, z1);
		//Right
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z2); 
			glVertex3f(x2, y1, z2);
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	} else if (action == 'T') { //teras tengah tengah anak tangga
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
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y1, z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
  		glBindTexture(GL_TEXTURE_2D, A[TERAS]);
		glBegin(GL_QUADS);
		//Up
			glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
			glTexCoord2f(5.0, 0.0);glVertex3f(x2, y2, z2);
			glTexCoord2f(5.0, 5.0);glVertex3f(x2, y2, z1); 
			glTexCoord2f(0.0, 5.0);glVertex3f(x1, y2, z1);
		glEnd();
		glColor4f(1,1,1,1);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		glColor3ub(160,82,45);
	} else {	//tutup atas miring
		//Front
		glBegin(GL_QUADS);
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);
			glVertex3f(x2, y1+convertToPx(CORLT/2), z2);
			glVertex3f(x1, y1+convertToPx(CORLT/2), z2);
		//Back
			glVertex3f(x1, y2-convertToPx(CORLT/2), z1);
			glVertex3f(x1, y2, z1);
			glVertex3f(x2, y2, z1); 
			glVertex3f(x2, y2-convertToPx(CORLT/2), z1);
		//Left
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1+convertToPx(CORLT/2), z2);
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2-convertToPx(CORLT/2), z1);
		//Right
			glVertex3f(x2, y1, z2);
			glVertex3f(x2, y1+convertToPx(CORLT/2), z2);
			glVertex3f(x2, y2, z2); 
			glVertex3f(x2, y2-convertToPx(CORLT/2), z2);
		//Up
			glVertex3f(x1, y1+convertToPx(CORLT/2), z2);
			glVertex3f(x2, y1+convertToPx(CORLT/2), z2);
			glVertex3f(x2, y2, z1); 
			glVertex3f(x1, y2, z1);
		//Down
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y2-convertToPx(CORLT/2), z1);
			glVertex3f(x2, y2-convertToPx(CORLT/2), z1); 
			glVertex3f(x2, y1, z2);
		glEnd();
	}
}

void drawTanggaSelatan(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3ub(160,82,45);
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
	glEnd();
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[TERAS]);
	glBegin(GL_QUADS);
	//Up
		glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
		glTexCoord2f(2.0, 0.0);glVertex3f(x2, y2, z2);
		glTexCoord2f(2.0, 0.30);glVertex3f(x2, y2, z1); 
		glTexCoord2f(0.0, 0.30);glVertex3f(x1, y2, z1);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glColor3ub(160,82,45);
}

void drawTanggaUtara(float x1, float x2, float y1, float y2, float z1, float z2) {
	glColor3ub(160,82,45);
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
	glEnd();
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, A[TERAS]);
	glBegin(GL_QUADS);
	//Up
		glTexCoord2f(0.0, 0.0);glVertex3f(x1, y2, z2);
		glTexCoord2f(2.0, 0.0);glVertex3f(x2, y2, z2);
		glTexCoord2f(2.0, 0.30);glVertex3f(x2, y2, z1); 
		glTexCoord2f(0.0, 0.30);glVertex3f(x1, y2, z1);
	glEnd();
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glColor3ub(160,82,45);
}


void drawTangga() {
	float y = 0, ty = (TINGGITIANG+CORLT)/ANAK_TANGGA, z = 10, tz = (TINGGITIANG+2*CORLT)/ANAK_TANGGA;
	
	glColor3ub(222,184,135);
	//KANAN
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(15+(UBIN_ANTAR_TIANG*20)),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(20+(UBIN_ANTAR_TIANG*22)),
					convertToPx(0),convertToPx(CORLT/4),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)+convertToPx(1),
					'V');
	
	drawTaman(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(10+(UBIN_ANTAR_TIANG*8)),
			TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(13+(UBIN_ANTAR_TIANG*19)),
			convertToPx(0),convertToPx(CORLT/4),
			convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0.8),
			convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)-convertToPx(1));
		
	glColor3ub(222,184,135);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),	//KIRI
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'M');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), 
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)+convertToPx(TINGGITIANG/2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	glColor3ub(160,82,45);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(ty*ANAK_TANGGA/2), convertToPx(ty*(ANAK_TANGGA/2+1)),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(30),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'T');
	for(int i = 0; i < ANAK_TANGGA/2; i++) {
		drawTanggaSelatan(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(33+UBIN_ANTAR_TIANG*20),
				convertToPx(y)+convertToPx(ty*i), convertToPx(y)+convertToPx(ty*(i+1)), 
				convertToPx(z)+convertToPx(tz*i), convertToPx(z)+convertToPx(tz*(i+1)));
		drawTanggaUtara(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(33+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(y)+convertToPx(ty*(25-i)), convertToPx(y)+convertToPx(ty*(26-i)), 
				convertToPx(z)+convertToPx(tz*(i)), convertToPx(z)+convertToPx(tz*(i+1)));
	}
	glColor3ub(222,184,135);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),	//KANAN
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), 
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)+convertToPx(TINGGITIANG/2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'M');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
				
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
	glColor3ub(160,82,45);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*20),	//ATAS
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+UBIN_ANTAR_TIANG*21),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'A');
	
	//KIRI
	drawJlnKoridor(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+(UBIN_ANTAR_TIANG*5)),
					TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(6+(UBIN_ANTAR_TIANG*7)),
					convertToPx(0),convertToPx(CORLT/4),
					convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0),
					convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*2)+convertToPx(1),
					'V');
	drawTaman(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*22)),
			TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(26+(UBIN_ANTAR_TIANG*26)),
			convertToPx(0),convertToPx(CORLT/4),
			convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*0.8),
			convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(JARAK_TIANG_KORIDOR*4)-convertToPx(1));
					
	glColor3ub(222,184,135);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20)-convertToPx(0.1),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'M');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), 
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)+convertToPx(TINGGITIANG/2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	glColor3ub(160,82,45);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(ty*ANAK_TANGGA/2), convertToPx(ty*(ANAK_TANGGA/2+1)),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(30),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'T');
	for(int i = 0; i < ANAK_TANGGA/2; i++) {
		drawTanggaSelatan(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(y)+convertToPx(ty*i), convertToPx(y)+convertToPx(ty*(i+1)), 
				convertToPx(z)+convertToPx(tz*i), convertToPx(z)+convertToPx(tz*(i+1)));
		drawTanggaUtara(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(7+UBIN_ANTAR_TIANG*6),
				convertToPx(y)+convertToPx(ty*(25-i)), convertToPx(y)+convertToPx(ty*(26-i)), 
				convertToPx(z)+convertToPx(tz*(i)), convertToPx(z)+convertToPx(tz*(i+1)));
	}
	glColor3ub(222,184,135);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2), 
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)+convertToPx(TINGGITIANG/2),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2)-convertToPx(0.1),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'M');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				convertToPx(0), convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
	drawTembokTangga(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(TINGGITIANG/2), convertToPx(TINGGITIANG),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'L');
	glColor3ub(160,82,45);
	drawTembokTangga(TITIK_X_AWAL+convertToPx(-PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*6),
				TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*7),
				convertToPx(TINGGITIANG)+convertToPx(CORLT)+convertToPx(CORLT/2),
				convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT),
				convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(20),
				convertToPx(LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(40),'A');
}

void drawAtapGedungHorizontal(float x1, float x2, float y1, float y2, float z1, float z2, float p1, float p2){
	glBegin(GL_QUADS);
		glTexCoord2f(10.0f, 0.0f);
	  	glVertex3f(x1, y1, z1);
	  	glTexCoord2f(10.0f, 1.0f);
	  	glVertex3f(x2, y2, z2);
	  	glTexCoord2f(0.0f, 1.0f);
	  	glVertex3f(x2+p2, y2, z2);
	  	glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x1+p1, y1, z1);
	glEnd();
}

void drawAtapGedungVertical(float x1, float x2, float y1, float y2, float z1, float z2, float p1, float p2){
	glBegin(GL_QUADS);
		glTexCoord2f(10.0f, 0.0f);
	   	glVertex3f(x1, y1, z1);
	   	glTexCoord2f(10.0f, 1.0f);
	   	glVertex3f(x2, y2, z2);
	   	glTexCoord2f(0.0f, 1.0f);
	   	glVertex3f(x2, y2, z2+p2);
	   	glTexCoord2f(0.0f, 0.0f);
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
	
	glColor3ub(222,184,135);
	drawLantai(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(2+UBIN_ANTAR_TIANG*0),
					TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(32+UBIN_ANTAR_TIANG*26),
					convertToPx(2*TINGGITIANG)+convertToPx(2*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT)-convertToPx(0.1),
					convertToPx(TITIK_TENGAH_Z)+convertToPx(6),
					convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8), '3');
					
	glColor3f(222,184,135);
	drawLangitLangit(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(34+UBIN_ANTAR_TIANG*26),
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT),convertToPx(2*TINGGITIANG)+convertToPx(2+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(Z_TENGAH_LORONG)-convertToPx(8));
	
	glColor3f(0.2,0.2,0.2);
	//Bagian Atap Bawah
	coorz = convertToPx(TITIK_TENGAH_Z) - convertToPx(LEBAR_GEDUNG);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, A[GENTENG]);
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0), 
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2), 
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN), 
						convertToPx(34+UBIN_ANTAR_TIANG*26), convertToPx(26+UBIN_ANTAR_TIANG*23));	//DEPAN
	glColor3f(128,128,128);
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*0), 
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2), 
						convertToPx(2*TINGGITIANG)+convertToPx(3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz-convertToPx(8),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
						convertToPx(34+UBIN_ANTAR_TIANG*26), convertToPx(26+UBIN_ANTAR_TIANG*23)); //BELAKANG
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
					
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, A[GENTENG]);	
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
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	//Bagian Atap Atas
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, A[GENTENG]);
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
						convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(20+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_DEPAN)-convertToPx(10),
						convertToPx(26+UBIN_ANTAR_TIANG*23), convertToPx(26+UBIN_ANTAR_TIANG*25));//DEPAN
	glColor3f(128,128,128);
	drawAtapGedungHorizontal(TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*2),
						TITIK_X_AWAL+convertToPx(PANJANGTIANG)+convertToPx(UBIN_ANTAR_TIANG*1),
						convertToPx(2*TINGGITIANG)+convertToPx(12+3*CORLT), convertToPx(2*TINGGITIANG)+convertToPx(20+3*CORLT),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG),
						convertToPx(-LEBARTIANG)+convertToPx(TITIK_TENGAH_Z)+coorz+convertToPx(Z_RUANGAN_BLKG)+convertToPx(10),
						convertToPx(26+UBIN_ANTAR_TIANG*23), convertToPx(26+UBIN_ANTAR_TIANG*25));//BELAKANG
	glColor4f(1,1,1,1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void drawGedungJTK() {

	drawSkybox(0, 0, 0-75, 350, 350, 275+200);
//	drawGrid();
	
	if(tiang) {
		glColor3ub(222,184,135);
		drawTiangJTK();
	}
	if(lt1) {
		glColor3ub(222,184,135);
		drawGedungLt1();
	}
	if(lt2) {
		glColor3ub(160,82,45);
		drawAtapKoridor();
		drawGedungLt2();
		drawTangga();
	}
	if(atap) {
		drawAtapJTK();
	}
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
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);
		
//	glMatrixMode(GL_MODELVIEW);//
//		GLfloat lightPosition[] = {10.0f, 0.5f, 10.0f, 0.0f};//
//		GLfloat lightDirection[] = {1.0, 0.0, 0.0};
//		GLfloat lightAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};//
//		GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};//
//		GLfloat shine[] = {10};//
//		
//	glClearColor(0.0, 0.0, 0.0, 0.0);//
//	glShadeModel(GL_SMOOTH);//
//	
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightSpecular);//
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);//
//	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);//
//	
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightAmbient);//
//	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);//
//	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);//
//	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);//
//	
//	
//	glEnable(GL_LIGHTING);//
//	glEnable(GL_LIGHT1);//

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, y, z,
			x+lx, y+ly,  z+lz,
			0.0f, 1.0f,  0.0f);
	drawGedungJTK();
	
	glDisable(GL_COLOR_MATERIAL);//
	
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) { 	

        if (key == 27)
              exit(0);
}

void pressKey(int key, int xx, int yy) {

       switch (key) {
       		 case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
			 case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
             case GLUT_KEY_UP : deltaMove = 1.5f; break;
             case GLUT_KEY_DOWN : deltaMove = -1.5f; break;
             case GLUT_KEY_PAGE_UP : ly += 0.25f; break;
             case GLUT_KEY_PAGE_DOWN : ly -= 0.25f; break;
             case GLUT_KEY_HOME : y += 1.0f; break;
             case GLUT_KEY_END : y -= 1.0f; break;
             case GLUT_KEY_F1 : (lt1 ? lt1=false: lt1=true ); break;
             case GLUT_KEY_F2 : (lt2 ? lt2=false: lt2=true ); break;
             case GLUT_KEY_F3 : (atap ? atap=false: atap=true ); break;
             case GLUT_KEY_F4 : (tiang ? tiang=false: tiang=true ); break;
       }
}

void releaseKey(int key, int x, int y) { 	

        switch (key) {
             case GLUT_KEY_LEFT :
			 case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
			 case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0.0f;break;
        }
} 

void initGL() {
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
    glDepthFunc(GL_LEQUAL);
      
    LoadTexture("front.bmp", SKYFRONT);
	LoadTexture("back.bmp", SKYBACK);
	LoadTexture("down.bmp", SKYDOWN);
	LoadTexture("up.bmp", SKYUP);
	LoadTexture("right.bmp", SKYRIGHT);
	LoadTexture("left.bmp", SKYLEFT); 
	LoadTextureA("pintu-24.bmp", PINTU_24); 
	LoadTextureA("genteng.bmp", GENTENG); 
	LoadTextureA("teras.bmp", TERAS);
	LoadTextureA("kaca.bmp", JENDELA);
	LoadTextureA("kaca2.bmp", JENDELALT2);
	LoadTextureA("atap1.bmp", ATAPLT1);
	LoadTextureA("pgrkoridor.bmp", PGRKORIDOR);
	LoadTextureA("teraskoridor.bmp", TERASKORIDOR);
	LoadTextureA("pintu_kelas.bmp", PINTURUANGAN);
	LoadTextureA("rumput.bmp", RUMPUT);
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000/120, timer, 0);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,768);
	glutCreateWindow("GedungJTK");

	initGL();
	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(0,timer,0);
	glutIdleFunc(renderScene);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
