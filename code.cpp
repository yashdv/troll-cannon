#include<iostream>
#include<cstdlib>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <GL/glut.h>
#include<cstdio>
#include<cstring>
#include<sstream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"

using namespace std;

# define pi 3.14

float start_time = 0, g = 0.000004, COR = 0.7, del = -0.0002, delsx = -0.05, zoom=1.0, panx=1.0;
int start_projectile = 0, LEVEL = 1, score = 100,lock = 0, flow = 0;
GLuint texture_ground_Id, texture_troll_Id, texture_tr_Id, texture_sh_Id, texture_intro_Id, texture_congo_Id;

float myabs(float a)
{
	if(a<0)
		return -a;
	return a;
}

class bullet
{
	public:
		float bulletRad, thetaCopy, u, ux, uy, x0, y0, x, y, vx, vy, copyx0, copyy0;

		bullet();
		void make_bullet();
};

class cannon
{
	public:
		float cannonBaseX, cannonBaseY, cannonL, cannonRad, theta;

		cannon();
		void make_cannon();
		void make_cannon_base();
};

class speedometer
{
	public:
		float x1,x2,y1,y2;
		speedometer();

		void display_speed();
};

class trampolene
{
	public:
		float x1,x2,y1,y2;
		float x3,x4,y3,y4;

		trampolene(float,float,float,float);
		void make_tram();
		void collision();
};

class balloon
{
	public:
		float rad,x1,y1,flag;

		balloon();
		void setP(float,float,float);
		void make_balloon();
		void collision();
		void collision2();
};

class floater
{
	public:
		float x1,y1,rad,drop,speed,xlim1,xlim2,theta,arrowx,arrowy,arrowl,droptime,ht;
		float copyx,copyy;

		floater(float,float);
		void resetF();
		void movefloat();
		void cut();
};

balloon target[30];
trampolene tram1(7,8,1.15,1.45);
speedometer s;
bullet b;
cannon c;
floater f1(9,5.2),f2(8,5.2),f3(7,5.2);

cannon::cannon()
{
	cannonBaseX = 0.3;
	cannonBaseY = 1.25;
	cannonL = 1.0;
	cannonRad = 0.1;
	theta = 0;
}

bullet::bullet()
{
	bulletRad = 0.1;
	u = 0.0032;
	ux = 0;
	uy = 0;
	thetaCopy = 0;
	copyx0 = x0 = 0;
	copyy0 = y0 = 0;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
}

trampolene::trampolene(float a,float b, float c, float d)
{
	x1 = a;
	x2 = b;
	y1 = c;
	y2 = d;
}

balloon::balloon()
{
	flag = 0;
}

floater::floater(float a,float b)
{
	copyx = x1 = a;
	copyy = y1 = b;
	ht = y1 - 1;
	drop = 0;
	xlim1 = 2.5;
	xlim2 = 9;
	speed = 0.03;
	rad = 0.2;
	theta = 0;
	arrowx = x1;
	arrowy = y1 - 1;
	arrowl = 0.2;
	droptime = 0;
}

void bullet::make_bullet()
{
	float t;
	if(start_projectile == 0)
	{
		thetaCopy = c.theta;
		x0 = c.cannonBaseX + (c.cannonL-bulletRad)*cos(thetaCopy*(pi/180));
		y0 = c.cannonBaseY + (c.cannonL-bulletRad)*sin(thetaCopy*(pi/180)); 
		x = x0;
		y = y0;
		copyx0 = x0;
		copyy0 = y0;
		ux = u*cos(thetaCopy*(pi/180));
		uy = u*sin(thetaCopy*(pi/180));
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_troll_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	glColor3f (1,1,1);
	glBegin(GL_POLYGON);
	for(int i=0;i<360;i++)
	{
		glTexCoord2f(0.5 + 0.55*cos(i*(pi/180)), 0.5 + 0.55*sin(i*(pi/180)));
		glVertex3f(x + 0.1*cos(i*(pi/180)), y + 0.1*sin(i*(pi/180)),0);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cannon::make_cannon()
{
	if(theta > 88)
		theta = 88;
	if(theta < 0)
		theta = 0;
	glRotatef(theta,0,0,1);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_sh_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,-cannonRad,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(cannonL,-cannonRad,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(cannonL,cannonRad,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,cannonRad,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
/*
	glColor3f (0,0,1.0);
	glBegin(GL_QUADS); 
	glVertex2f(0,-cannonRad);
	glVertex2f(0,cannonRad);
	glVertex2f(cannonL,cannonRad);
	glVertex2f(cannonL,-cannonRad);
	glEnd();
	*/
}

void cannon::make_cannon_base()
{
	glPushMatrix();
	glTranslatef(cannonBaseX,cannonBaseY,0);    
	glBegin(GL_TRIANGLE_FAN);
	glColor3f (0.0, 1.0, 0.0);
	for(int i=0;i<360;i++)
	{   
		glVertex2f(cannonRad*cos(i*(pi/180)),cannonRad*sin(i*(pi/180)));
	}   
	glEnd();
	make_cannon();
}

speedometer::speedometer()
{
	x1 = 0.2;
	x2 = 0.25;
	y1 = 5.5;
	y2 = 5.6;
}
void speedometer::display_speed()
{
	glColor3f (138/255.0,43/255.0,226/255.0);
	glBegin(GL_QUADS); 
	glVertex2f(x1,y1);
	glVertex2f(x1,y2);
	glVertex2f(x2,y2);
	glVertex2f(x2,y1);
	glEnd();
}

void trampolene::make_tram()
{
//	/*
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_tr_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x1,y1,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x2,y1,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x2,y2,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x1,y2,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
//*/
 /*
	glBegin(GL_QUADS);
	glColor3f (1,1,1);
	glBegin(GL_QUADS); 
	glVertex2f(x1,y1);
	glVertex2f(x2,y1);
	glVertex2f(x2,y2);
	glVertex2f(x1,y2);
	glEnd();
	*/
}

void trampolene::collision()
{
	if(b.y>=y1 && b.y<=y2+0.05)
	{
		if(b.x+b.bulletRad > x1 && b.x+b.bulletRad <= x1+0.2)
		{
			start_time = glutGet(GLUT_ELAPSED_TIME);
			b.ux *= -1;
			b.x0 = x1-b.bulletRad;
			b.y0 = b.y;
			b.x = b.x0;
		}
	}
	if(b.x>x1+0.1 && b.x<=x2+0.05)
	{
		if(b.y+b.bulletRad <= y2+0.05)
		{
			start_time = glutGet(GLUT_ELAPSED_TIME);
			b.x0 = b.x;
			b.y0 = y2;
			b.uy = -b.vy*2;
			b.y = y2;
		}
	}
}

void balloon::setP(float a,float b, float c)
{
	x1 = a;
	y1 = b;
	rad = c;
}

void balloon::make_balloon()
{
	if(flag == 1.0)
		return;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f (1, 215/255, 0.0);
	for(int i=0;i<360;i++)
	{   
		glVertex2f(x1 + rad*cos(i*(pi/180)),y1 + rad*sin(i*(pi/180)));
	}
	glEnd();
}

void balloon::collision()
{
	if( (b.x-x1)*(b.x-x1)+(b.y-y1)*(b.y-y1) <= (rad+b.bulletRad)*(rad+b.bulletRad) )
	{
		flag = 1;
	}
}

void balloon::collision2()
{
	if(f1.arrowx>(x1-rad) && f1.arrowx<(x1+rad))
		if(f1.arrowy>(y1-rad) && f1.arrowy<(y1+rad))
			flag = 1;
	if(f2.arrowx>(x1-rad) && f2.arrowx<(x1+rad))
		if(f2.arrowy>(y1-rad) && f2.arrowy<(y1+rad))
			flag = 1;
	if(f3.arrowx>(x1-rad) && f3.arrowx<(x1+rad))
		if(f3.arrowy>(y1-rad) && f3.arrowy<(y1+rad))
			flag = 1;
}

void put_target1()
{
	target[0].setP(9,3,0.2);
	target[0].make_balloon();
	target[1].setP(9.5,3.4,0.2);
	target[1].make_balloon();
	target[2].setP(10,3.8,0.2);
	target[2].make_balloon();
}

void floater::resetF()
{
	x1 = copyx;
	y1 = copyy;
	speed = myabs(speed);
}

void endchecklevel2();
void floater::movefloat()
{
	float t;
	if(drop == 1)
	{
		x1 += speed;
		y1 += myabs(speed)/2;

	}
	else
	{
		if(x1 >= xlim2)
		{
			speed = -myabs(speed);
			x1 = xlim2 - 0.01;
		}
		if(x1 <= xlim1)
		{
			speed = myabs(speed);
			x1 = xlim1 + 0.01;
		}
		x1 += speed;
	}

	if(drop == 0)
	{
		glColor3f (0,0,0);
		glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x1,y1-1);
		glEnd();
	}

	glBegin(GL_TRIANGLE_FAN);
	glColor3f (30/255, 44/255, 1);
	for(int i=0;i<360;i++)
	{   
		glVertex2f(
			   x1 + (rad + 0.1*sin(theta*(pi/100)))*cos(i*(pi/180)),
			   y1 + (rad + 0.1*sin(theta*(pi/100)))*sin(i*(pi/180))  
			  );
	}
	glEnd();
	theta += 0.5;

	if(drop == 0)
	{
		arrowx = x1; arrowy = y1-1;
	}
	else if(drop == 1 && arrowy>-1)
	{
		arrowx += speed;
		t = glutGet(GLUT_ELAPSED_TIME) - droptime;
		arrowy = ht - 0.5*g*t*t;

	}

	if(arrowy >= 1.36)
	{
		glColor3f (1,0,0);
		glBegin(GL_LINES);
		glVertex2f(arrowx,arrowy);
		glVertex2f(arrowx,arrowy-arrowl);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(arrowx,arrowy-arrowl);
		glVertex2f(arrowx+arrowl/2,arrowy-arrowl/2);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(arrowx,arrowy-arrowl);
		glVertex2f(arrowx-arrowl/2,arrowy-arrowl/2);
		glEnd();
	}
	else
	{
		endchecklevel2();
	}

}

void floater::cut()
{
	if(b.y>y1-1 && b.y<y1)
	{
		if(b.x>x1 && b.x-x1 < 0.2)
		{
			drop = 1;
			droptime = glutGet(GLUT_ELAPSED_TIME);
		}
	}
}

void put_target2()
{
	target[0].setP(3,1.35,0.2);
	target[0].make_balloon();
	target[1].setP(4,1.35,0.2);
	target[1].make_balloon();
	target[2].setP(5,1.35,0.2);
	target[2].make_balloon();
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
			0,                            //0 for now
			GL_RGB,                       //Format OpenGL uses for image
			image->width, image->height,  //Width and height
			0,                            //The border of the image
			GL_RGB, //GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
			//as unsigned numbers
			image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	Image* image = loadBMP("back.bmp");
	texture_ground_Id = loadTexture(image);
	delete image;
	Image* image2 = loadBMP("troll.bmp");
	texture_troll_Id = loadTexture(image2);
	delete image2;
	Image* image3 = loadBMP("trampo.bmp");
	texture_tr_Id = loadTexture(image3);
	delete image3;
	Image* image4 = loadBMP("shoot.bmp");
	texture_sh_Id = loadTexture(image4);
	delete image4;
	Image* image5 = loadBMP("intro.bmp");
	texture_intro_Id = loadTexture(image5);
	delete image5;
	Image* image6 = loadBMP("congo.bmp");
	texture_congo_Id = loadTexture(image6);
	delete image6;
}

void background_image()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_ground_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(11.5,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(11.5,6,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0,6,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	string ans;
	stringstream con;
	con << score;
	ans = con.str();
	for(int i=0; i<ans.size(); i++)
	{
	glRasterPos2f(s.x1 + i*0.1,s.y1-0.1);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , ans[i]);
	}

}

void intro()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_intro_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.5,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(10,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(10,5.5,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5,5.5,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void congo()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_congo_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.5,0,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(10,0,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(10,5.5,0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5,5.5,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void updatebullet()
{
	float t = glutGet(GLUT_ELAPSED_TIME) - start_time;
	b.x = b.x0 + b.ux*t;
	b.vy = b.uy - g*t;
	b.y = b.y0 + b.uy*t - 0.5*g*t*t;

}

void level1()
{
	int i;
	c.make_cannon_base();
	b.make_bullet();
	s.display_speed();
	tram1.make_tram();
	put_target1();
	for(i=0; i<3; i++)
	{
		target[i].collision();
	}
	if(start_projectile == 1)
	{
		updatebullet();
		tram1.collision();
	}
}

void change2level2()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-5.55,-2.88,-5.0);
	sleep(2);
	LEVEL++;
	flow++;
	lock = 0;
}

void level2()
{	
	int i;
	c.make_cannon_base();
	b.make_bullet();
	s.display_speed();
	f1.movefloat();
	f2.movefloat();
	f3.movefloat();
	f1.cut();
	f2.cut();
	f3.cut();
	put_target2();
	for(i=0; i<3; i++)
	{
		target[i].collision2();
	}
	if(start_projectile == 1)
	{
		updatebullet();
	}
}

void change2level3()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT );//| GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.55,-2.88,-5.0);
	sleep(2);
	LEVEL++;
	flow++;
	cout << "flow " << flow << endl;
	lock = 0;
}

void endchecklevel1()
{
	int i;
	for(i=0; i<3; i++)
		if(target[i].flag == 0)
			break;
	if(i != 3)
	{
		for(i=0; i<3; i++)
			target[i].flag = 0;
		score--;
	}
	else
	{
		for(i=0; i<3; i++)
			target[i].flag = 0;
		change2level2();
	}
}

void endchecklevel2()
{
	if(f1.arrowy>=1.25 || f2.arrowy>=1.25 || f3.arrowy>=1.25)
	{
		if(f1.arrowy!=f1.copyy-1 && f2.arrowy!=f2.copyy-1 && f3.arrowy!=f3.copyy-1)
			return;
	}
	int i;
	for(i=0; i<3; i++)
		if(target[i].flag == 0)
			break;
	if(i != 3)
	{
		for(i=0; i<3; i++)
			target[i].flag = 0;
		f1.resetF();
		f2.resetF();
		f3.resetF();
		f1.drop = f2.drop = f3.drop = 0;
		score--;
	}
	else
	{
		for(i=0; i<3; i++)
		{
			target[i].flag = 0;
			f1.drop = f2.drop = f3.drop = 0;
			change2level3();
		}
	}

}


void reload_cannon()
{
	b.x0 = b.copyx0;
	b.y0 = b.copyy0;
	start_projectile = 0;
	if(LEVEL == 1)
		endchecklevel1();
}

void display (void)
{
	float t;
	int i;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT );//| GL_DEPTH_BUFFER_BIT);

	glScalef(zoom,zoom,zoom);
	if(zoom!=1)
		glTranslatef(panx,0,0);
	
	glTranslatef(-5.55,-2.88,-5.0);

	background_image();

	if(flow == 0)
		intro();
	else if(flow == 1)
		level1();
	else if(flow == 2)
		congo();
	else if(flow == 3)
		level2();
	else if(flow >= 4)
		cannon();

	if(start_projectile == 1)
	{
		if((b.ux < 0.00032 && b.uy < 0.000250) || b.x > 12 || b.x<0)
		{
			b.ux = 0;
			b.uy = 0;
			b.vy = 0;
			b.y = 1.23;
			reload_cannon();
		}
		else if(b.y <= 1.25)
		{
			start_time = glutGet(GLUT_ELAPSED_TIME);
			b.x0 = b.x;
			b.y0 = 1.25;
			b.uy = -b.vy;
			b.uy *= COR;
			b.ux *= 0.8;
		}
	}

	glutSwapBuffers();
}

void reshape (int width, int height) {
	//Following command decides how much part of window to be used for rendering.
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int, int) {

	if((int)key == 27)
		exit(0);
	else if(key == 'a')
		c.theta++;
	else if(key == 'b')
		c.theta--;
	else if(key == 'f' && start_projectile == 0 && b.u < 0.0076)
	{
		b.u += 0.0002;
		s.x2 += 0.05;
	}
	else if(key == 's' && start_projectile == 0 && b.u > 0.0032)
	{
		b.u -= 0.0002;
		s.x2 -= 0.05;
	}
	else if(key ==' ')
	{
		if(start_projectile == 0)
		{
			start_projectile = 1;
			start_time = glutGet(GLUT_ELAPSED_TIME);
		}
	}
	else if(key == 'z'){
		zoom+=0.5;
	}
	else if(key == 'x'){
		if(zoom>1)
			zoom-=0.5;
	}
	else if(key == 'c'){
		panx++;
	}
	else if(key == 'v'){
		panx--;
	}
	else if(key == 'n' && lock==0)
	{
		lock = 1;
		flow++;
	}
}

void mouse (int button, int state, int mx, int my) 
{
	if (state == GLUT_DOWN) 
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if(start_projectile == 0)
			{
				if(b.u >= 0.0076 || b.u <= 0.0032)
				{
					del = -del;
					delsx = -delsx;
				}
				b.u += del;
				s.x2 += delsx;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			if(start_projectile == 0)
			{
				start_projectile = 1;
				start_time = glutGet(GLUT_ELAPSED_TIME);
			}
		}
	}
}
void mouse_passive(int x, int y)
{
	y = 1000-y;
	x = x-51;
	y = y-216;
	if(x>0 && y>0)
		c.theta = (atan(y/x))*(180/pi);
	printf("%f\n",c.theta);
}

int main (int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB );//| GLUT_DEPTH);
	glutInitWindowSize (1920, 1000);
	glutInitWindowPosition (0, 0);

	glutCreateWindow ("GAmE.cpp");

	initRendering();

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutMouseFunc (mouse);
	glutPassiveMotionFunc(mouse_passive);

	glutMainLoop();
	return 0;
}
