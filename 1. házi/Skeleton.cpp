//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Koczkás Péter
// Neptun : QCB102
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

const char* const vertexSource = R"(
	#version 330				
	precision highp float;		

	layout(location = 0) in vec2 vp;	
	layout(location = 1) in vec2 vUV;
	
	out vec2 textcoord;

	void main() {
		textcoord = vUV;
		gl_Position = vec4(vp.x, vp.y, 0, 1);		
	}
)";

const char* const fragmentSource = R"(
	#version 330			
	precision highp float;	
	
	uniform sampler2D textureUnit;
	uniform vec3 color;		
	out vec4 outColor;		
	in vec2 textcoord;

	void main() {
		outColor = texture(textureUnit, textcoord);
	}
)";

GPUProgram gpuProgram; 
unsigned int vaoCircle, vaoLines;	   
const int nv = 47;
const int np = 50;
const int nl = 62;
bool move;
vec3 startp;
unsigned int textureId;

class Graph {
public:

	vec3 points[np]; 
	vec3 lines[nl * 2]; 
	int lineshelp[nl * 2]; 
	vec3 pointshelp[np]; 
	vec3 circle[nv * np]; 
	vec4 color[np]; 


	void Setcircle() {
		glGenVertexArrays(1, &vaoCircle);	
		glBindVertexArray(vaoCircle);		
		glBufferData(GL_ARRAY_BUFFER, 	
			sizeof(circle),  
			circle,	      	
			GL_STATIC_DRAW);	

		glEnableVertexAttribArray(0);  
		glVertexAttribPointer(0,       
			3, GL_FLOAT, GL_FALSE, 
			0, NULL); 		     
	}
	void Setlines() {
		glGenVertexArrays(1, &vaoLines);	
		glBindVertexArray(vaoLines);		
		glBufferData(GL_ARRAY_BUFFER, 	
			sizeof(lines),  
			lines,	      	
			GL_STATIC_DRAW);	

		glEnableVertexAttribArray(0);  
		glVertexAttribPointer(0,       
			3, GL_FLOAT, GL_FALSE, 
			0, NULL); 		     
	}
	vec3 Transform(vec3 p, vec3 startp, float cX, float cY, float cZ) {
		float d0, d1, d2;
		vec3 v0, v1, v2, m, mirror, transformed;

		d0 = acoshf(-((cX * startp.x) + (cY * startp.y) - (cZ * startp.z)));
		v0 = vec3((cX - startp.x * coshf(d0)) / sinhf(d0), (cY - startp.y * coshf(d0)) / sinhf(d0), (cZ - startp.z * coshf(d0)) / sinhf(d0));
		m = vec3((startp.x * coshf(d0 / 2)) + (v0.x * sinhf(d0 / 2)), (startp.y * coshf(d0 / 2)) + (v0.y * sinhf(d0 / 2)), (startp.z * coshf(d0 / 2)) + (v0.z * sinhf(d0 / 2)));

		d1 = acoshf(-((p.x * startp.x) + (p.y * startp.y) - (p.z * startp.z)));
		v1 = vec3((startp.x - p.x * coshf(d1)) / sinhf(d1), (startp.y - p.y * coshf(d1)) / sinhf(d1), (startp.z - p.z * coshf(d1)) / sinhf(d1));
		mirror = vec3(p.x * coshf(2 * d1) + v1.x * sinhf(2 * d1), p.y * coshf(2 * d1) + v1.y * sinhf(2 * d1), p.z * coshf(2 * d1) + v1.z * sinhf(2 * d1));

		d2 = acoshf(-((mirror.x * m.x) + (mirror.y * m.y) - (mirror.z * m.z)));
		v2 = vec3((m.x - mirror.x * coshf(d2)) / sinhf(d2), (m.y - mirror.y * coshf(d2)) / sinhf(d2), (m.z - mirror.z * coshf(d2)) / sinhf(d2));
		transformed = vec3(mirror.x * coshf(2 * d2) + v2.x * sinhf(2 * d2), mirror.y * coshf(2 * d2) + v2.y * sinhf(2 * d2), mirror.z * coshf(2 * d2) + v2.z * sinhf(2 * d2));

		return transformed;

	}
	void MakeCircle(int i) {
		for (int j = 0; j < nv; j++) {
			float f = 2 * j * (M_PI / nv);
			circle[i * nv + j].x = cosf(f) * 0.03 + (points[i].x);
			circle[i * nv + j].y = sinf(f) * 0.03 + (points[i].y);
			circle[i * nv + j].z = sqrt((circle[i * nv + j].x * circle[i * nv + j].x) + (circle[i * nv + j].y * circle[i * nv + j].y) + 1);
			circle[i * nv + j].x = circle[i * nv + j].x / circle[i * nv + j].z;
			circle[i * nv + j].y = circle[i * nv + j].y / circle[i * nv + j].z;
			circle[i * nv + j].z = 1;
		}
	}
	void MakePoints() {
		for (int i = 0; i < np; i++) {
			points[i].x = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) * 2;
			points[i].y = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) * 2;
			points[i].z = sqrt(points[i].x * points[i].x + points[i].y * points[i].y + 1);
			for (int j = 0; j < i; j++) {
				while (abs(acoshf(-((points[j].x * points[i].x) + (points[j].y * points[i].y) - (points[j].z * points[i].z)))) < 0.2f) {
					points[i].x = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) * 2;
					points[i].y = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) * 2;
					points[i].z = sqrt(points[i].x * points[i].x + points[i].y * points[i].y + 1);


				}
			}

			pointshelp[i] = vec3(points[i].x, points[i].y, points[i].z);

			MakeCircle(i);
		}
		for (int i = 0; i < np; i++) {
			points[i].x = points[i].x / points[i].z;
			points[i].y = points[i].y / points[i].z;
			points[i].z = 1;
		}
	}
	void MakeLines() {
		int k1 = 0;
		int k2 = 0;

		for (int i = 0; i < nl * 2; i += 2) {
			k1 = rand() % np;
			k2 = rand() % np;
			while (k1 == k2) {
				k1 = rand() % np;
				k2 = rand() % np;
			}
			lines[i] = points[k1];
			lines[i + 1] = points[k2];
			lineshelp[i] = k1;
			lineshelp[i + 1] = k2;
		}
		int n = 1;
		while (n != 0) {
			n = 0;
			for (int i = 0; i < nl * 2; i += 2) {

				for (int j = 0; j < nl * 2; j += 2) {
					if (i != j) {

						if (((lines[i].x == lines[j].x && lines[i].y == lines[j].y)
							&& (lines[i + 1].x == lines[j + 1].x && lines[i + 1].y == lines[j + 1].y)) ||
							((lines[i].x == lines[j + 1].x && lines[i].y == lines[j + 1].y)
								&& (lines[i + 1].x == lines[j].x && lines[i + 1].y == lines[j].y))) {

							k1 = rand() % np;
							k2 = rand() % np;
							while (k1 == k2) {
								k1 = rand() % np;
								k2 = rand() % np;
							}
							lines[j] = points[k1];
							lines[j + 1] = points[k2];
							lineshelp[j] = k1;
							lineshelp[j + 1] = k2;
							n++;
						}
					}
				}


			}

		}
	}
	void DrawCircle() {

		glBindVertexArray(vaoCircle);  
		glBufferData(GL_ARRAY_BUFFER, 	
			sizeof(circle),  
			circle,	      	
			GL_STATIC_DRAW);	
		for (int i = 0; i < np; i++) {
			std::vector<vec4> image(1);
			image[0] = color[i];
			UploadTexture(1, 1, image);
			Drawtexture();
			glDrawArrays(GL_TRIANGLE_FAN, i * nv , nv );
		}
	}
	void DrawLines() {
		int location = glGetUniformLocation(gpuProgram.getId(), "textureUnit");
		std::vector<vec4> color(1);
		color[0] = vec4(1, 1, 0, 1);
		UploadTexture(1, 1, color);
		Drawtexture();

		glBindVertexArray(vaoLines);
		glBindVertexArray(vaoLines);		
		glBufferData(GL_ARRAY_BUFFER, 	
			sizeof(lines),  
			lines,	      	
			GL_STATIC_DRAW);	

		for (int i = 0; i < nl*2; i++) {
			glDrawArrays(GL_LINES, i * 2, 2);
		}
	}
	bool Cut(int j, int k1, int k2, vec3 l[]) {
		vec2 p1, p2, p3, p4;
		p1 = vec2(l[j].x / l[j].z, l[j].y / l[j].z);
		p2 = vec2(l[j + 1].x / l[j + 1].z, l[j + 1].y / l[j + 1].z);

		p3 = vec2(l[k1].x / l[k1].z, l[k1].y / l[k1].z);
		p4 = vec2(l[k2].x / l[k2].z, l[k2].y / l[k2].z);

		vec2 n1 = vec2(p2.y - p1.y, p1.x - p2.x);
		vec2 n2 = vec2(p4.y - p3.y, p3.x - p4.x);

		float e1 = ((n1.x * (p3.x - p1.x) + n1.y * (p3.y - p1.y)) * (n1.x * (p4.x - p1.x) + n1.y * (p4.y - p1.y)));
		float e2 = ((n2.x * (p1.x - p3.x) + n2.y * (p1.y - p3.y)) * (n2.x * (p2.x - p3.x) + n2.y * (p2.y - p3.y)));


		if (e1 < 0 && e2 < 0) {
			return true;
		}
		return false;
	}
	int AllCut(vec3 point[]) {
		int cut = 0;
		vec3 l[nl*2];
		for (int i = 0; i < nl*2; i += 2) {
			l[i] = point[lineshelp[i]];
			l[i+1]= point[lineshelp[i+1]];
			for (int j = 0; j < nl*2; j += 2) {
				if (j != i) {
					if (Cut(i, j, j + 1, l)) {
						cut++;
					}
				}
			}
		}
		return cut;
	}
	void Heuristic() {
		vec3 seged[np];
		int cut = 0;
		int oldcut = AllCut(pointshelp);
		for (int i = 0; i < np; i++) {
			points[i] = vec3(pointshelp[i].x, pointshelp[i].y, pointshelp[i].z);
			seged[i] = points[i];
		}
		for (int i = 0; i < np; i++) {
			for (int j = 0; j < 75; j++) {
				float x = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) *2;
				float y = ((((float)rand()) / (float)RAND_MAX) * 2 - 1) *2;
				seged[i] = vec3(x, y, sqrt(x * x + y * y + 1));
				cut = AllCut(seged);
				if (cut < oldcut) {
					points[i] = seged[i];
					oldcut =cut;
				}
				else seged[i] = points[i];
			}
			pointshelp[i] = vec3(points[i].x, points[i].y, points[i].z);
			MakeCircle(i);

		}
		for (int i = 0; i < nl*2; i++) {
			lines[i] = vec3(points[lineshelp[i]].x, points[lineshelp[i]].y, points[lineshelp[i]].z);
			lines[i].x /= lines[i].z;
			lines[i].y /= lines[i].z;
			lines[i].z = 1;
		}
		
	}
	void UploadTexture(int width, int height, std::vector<vec4>& image) {
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &image[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void Drawtexture() {
		int sampler = 0;
		int location = glGetUniformLocation(gpuProgram.getId(), "textureUnit");
		glUniform1i(location, sampler);
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	void MakeColor() {
		for (int i = 0; i < np; i++) {
			color[i] = vec4((1 - i * 0.02), (float)rand() / (float)RAND_MAX, 0.8 - i * 0.01, 1);
		}
	}
};

Graph graph;

void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);



	unsigned int vbo;		
	glGenBuffers(1, &vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	graph.MakePoints();

	graph.MakeLines();

	graph.MakeColor();

	graph.Setcircle();
	graph.Setlines();
	
	gpuProgram.create(vertexSource, fragmentSource, "outColor");
}

void onDisplay() {
	glClearColor(0, 0, 0, 0);    
	glClear(GL_COLOR_BUFFER_BIT); 

	graph.DrawLines();

	graph.DrawCircle();
	
	glutSwapBuffers(); 

}

void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == ' ') {
		graph.Heuristic();
		glutPostRedisplay();
	}
	
	
}

void onKeyboardUp(unsigned char key, int pX, int pY) {
}

void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	
	float cY = 1.0f - 2.0f * pY / windowHeight;
	float cZ = sqrt(cX * cX + cY * cY + 1);

	if (move) {
		for (int i = 0; i < np; i++) {
			graph.points[i] = vec3(graph.pointshelp[i].x, graph.pointshelp[i].y, graph.pointshelp[i].z);
			graph.points[i] = graph.Transform(graph.points[i], startp, cX, cY, cZ);
			graph.pointshelp[i] = vec3(graph.points[i].x, graph.points[i].y, graph.points[i].z);

			graph.MakeCircle(i);

		}
		for (int i = 0; i < nl*2; i++) {
			graph.lines[i] = vec3(graph.points[graph.lineshelp[i]].x, graph.points[graph.lineshelp[i]].y, graph.points[graph.lineshelp[i]].z);
			graph.lines[i].x /= graph.lines[i].z;
			graph.lines[i].y /= graph.lines[i].z;
			graph.lines[i].z = 1;
		}
		startp.x = cX;
		startp.y = cY;
		startp.z = cZ;
		glutPostRedisplay();
	}

}

void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	
	float cY = 1.0f - 2.0f * pY / windowHeight;

	char* buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; break;
	}
	startp.x = cX;
	startp.y = cY;
	startp.z = sqrt(startp.x * startp.x + startp.y * startp.y + 1);

	switch (button) {
	case GLUT_LEFT_BUTTON: {
		
		move = false;
		break;
	}
	case GLUT_MIDDLE_BUTTON: {
		
		move = false;
		break;
	}
	case GLUT_RIGHT_BUTTON: {
		
		move = true;
		break;
	}
	}

}

void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME);
}