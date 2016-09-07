#include "SketchGLCanvas.h"
#include <wx/dcclient.h>
#include <wx/Statusbr.h>
#include "cycleUtils.h"
#include <fstream>
//#include "stdwx.h"

BEGIN_EVENT_TABLE( SketchGLCanvas, wxGLCanvas )
	EVT_PAINT( SketchGLCanvas::OnPaint )
	EVT_MOUSE_EVENTS( SketchGLCanvas::OnMouse )
	EVT_IDLE( SketchGLCanvas::OnIdle )
	EVT_SIZE( SketchGLCanvas::OnSize )
//	EVT_KEY_DOWN( SketchGLCanvas::OnKeyDown )
END_EVENT_TABLE()

SketchGLCanvas::SketchGLCanvas(	wxWindow *parent, wxWindowID id,
		 const wxPoint& pos,
		 const wxSize& size, long style,
		 const wxString& name, int *attribList)
		 : wxGLCanvas( parent, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE , name )
{	m_initialized = false;	m_rotationTimes=1;	m_rotationCount = 0;}

SketchGLCanvas::~SketchGLCanvas ()
{
}

void SketchGLCanvas::Initialize()
{
		SetCurrent();

		GetClientSize( &m_width, &m_height );

		m_initialized = true;

		m_Eye[0] = 0.0f; m_Eye[1] = 0.0f; m_Eye[2] = -2.0f; //Actual code
		m_PerspectiveAngleDegrees = 45.0f;
		m_NearPlane = 0.01f;
		m_FarPlaneOffset = 100.0f;

		glViewport(0, 0, (GLint) m_width, (GLint) m_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-2, 2, -2, 2, m_NearPlane, m_NearPlane + m_FarPlaneOffset);

		gluPerspective(m_PerspectiveAngleDegrees,
			(GLfloat)m_width/(GLfloat)m_height,
			m_NearPlane,
			m_NearPlane + m_FarPlaneOffset);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//m_RotationMatrix.setIdentity(); //NO_MATRIX
		memset(m_RotationMatrix, 0.0, sizeof(double)*16);
		for (unsigned int i=0; i< 4; ++i){
			m_RotationMatrix[i+(4*i)] = 1.0;
		}

		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		//glShadeModel(GL_FLAT);
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		//glDepthFunc(GL_LESS);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			// Really Nice Point Smoothing

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_LINE_SMOOTH); //anti-aliased lines


		// **** rest of function is code from initGL function of Repousse standalone ****

		// ****** Lighting ******
//		GLfloat light_position0[] = { -2.0f, -1.0f, 5.0f, 0.0f };
		GLfloat light_position0[] = { -1.0f, -1.0f, 2.0f, 0.0f };
		GLfloat diffuse_light0[] = {0.7f,0.5f,0.5f,0.5f};
		GLfloat ambient_light0[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		//GLfloat specular_light0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat specular_light0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat As[4] = {0.1f, 0.1f, 0.1f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, As );
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);


//		GLfloat light_position1[] = { 2.0f, 1.0f, 0.5f, 0.0f };
		GLfloat light_position1[] = { 1.0f, 1.0f, 2.0f, 0.0f };
		//GLfloat light_position1[] = { -.4f, 2.6f, .4f, 0.0f };
		GLfloat diffuse_light1[] = {0.4f,0.2f,0.2f,1.0f};
		//GLfloat specular_light1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat specular_light1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat ambient_light1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);

		// GLfloat light_position2[] = { FLT_MAX, FLT_MAX, FLT_MAX, 0.0f };
		GLfloat light_position2[] = { -1.0f, -1.0f, 2.0f, 0.0f };
		GLfloat diffuse_light2[] = {0.0f,0.0f,0.0f,1.0f};
		GLfloat ambient_light2[] = { .75, .75, .75, 1.0f };
		GLfloat specular_light2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_light2);
		glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light2);
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_light2);

		// GLfloat light_position3[] = { -FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f };
		GLfloat light_position3[] = { 1.0f, 1.0f, -2.0f, 0.0f };
		GLfloat diffuse_light3[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat specular_light3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat ambient_light3[] = { 1, 1, 1, 1.0f };
		glLightfv(GL_LIGHT3, GL_POSITION, light_position3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse_light3);
		glLightfv(GL_LIGHT3, GL_SPECULAR, specular_light3);
		glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_light3);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);

		// enable color tracking
		bool isTwoSided = true;

		if (isTwoSided){
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		} else {
			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		}
		glEnable(GL_COLOR_MATERIAL);


		// ****** Material properties ******
	//	GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f};
		GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f};
	//	GLfloat mat_shininess[] = {1.0f };
		GLfloat mat_shininess[] = {1.0f };
		//GLfloat mat_diffuseB[] = { 0.6f, 0.6f, 0.8f, 1.0f };
		GLfloat mat_diffuseB[] = { 0.8f, 0.75f, 0.35f, 1.0f };
		GLfloat mat_ambient_front[] = { 0.15f, 0.12f, .02f, 1.0f };
		GLfloat mat_ambient_back[] = { 1.0f, 1.0f, 1.0f, 1.0f };

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , mat_diffuseB);
		glMaterialfv(GL_FRONT, GL_AMBIENT , mat_ambient_front);
		glMaterialfv(GL_BACK, GL_AMBIENT , mat_ambient_back);

		if (isTwoSided){
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		} else {
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		}


//		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_LIGHTING);

		m_isRotate = false;		m_frameRate = 50;		m_timeCurr = clock();
		m_timePrev = m_timeCurr-100;
		m_rotationCount=0;
		m_frames=0;
		m_shotNum=0;
}

void SketchGLCanvas::screenShot(){

	int windowWidth, windowHeight;
	GetClientSize(&windowWidth, &windowHeight);
	byte*  bmpBuffer = (byte*)malloc(windowWidth*windowHeight*3);
	char fileName[400],fileName2[400];
	if  (bmpBuffer!=NULL){
		glReadBuffer(GL_BACK); 
		glReadPixels((GLint)0,  (GLint)0, (GLint)windowWidth-1, (GLint)windowHeight-1, GL_RGB,  GL_UNSIGNED_BYTE, bmpBuffer);
		for (int i=0;i<windowWidth*windowHeight*3;i+=3) { 
			byte tmp=*(bmpBuffer+i);
			*(bmpBuffer+i)=*(bmpBuffer+i+2);
			*(bmpBuffer+i+2) = tmp;
		}
		strcpy(fileName,m_cycleUtils->m_filename.mb_str());
		for(int i=strlen(fileName)-1;i>0;i--){
			if(fileName[i]=='.') fileName[i]='\0';
		}
		char shotNum[32];
		_itoa_s(m_shotNum,shotNum,10);
		strcat(fileName,"_");
		strcat(fileName,shotNum);
		strcpy(fileName2,fileName);
		strcat(fileName,".bmp");
		m_shotNum++;
		//sprintf(fileName, "frames/frame_%d.bmp", m_frameno);
		FILE *filePtr = fopen(fileName,  "wb");
		if (filePtr!=NULL) {
			BITMAPFILEHEADER  bitmapFileHeader;
			BITMAPINFOHEADER  bitmapInfoHeader;
			bitmapFileHeader.bfType =  ((WORD) ('M' << 8) | 'B'); ;  //"BM"
			bitmapFileHeader.bfSize =  windowWidth*windowHeight*3+sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER);
			bitmapFileHeader.bfReserved1 =  0;
			bitmapFileHeader.bfReserved2 = 0;
			bitmapFileHeader.bfOffBits  =
				sizeof(BITMAPFILEHEADER) +  sizeof(BITMAPINFOHEADER);
			bitmapInfoHeader.biSize =  sizeof(BITMAPINFOHEADER);
			bitmapInfoHeader.biWidth =  windowWidth-1;
			bitmapInfoHeader.biHeight =  windowHeight-1;
			bitmapInfoHeader.biPlanes = 1;
			bitmapInfoHeader.biBitCount  = 24;
			bitmapInfoHeader.biCompression =  BI_RGB;
			bitmapInfoHeader.biSizeImage = 0;
			bitmapInfoHeader.biXPelsPerMeter  = 0; // ?
			bitmapInfoHeader.biYPelsPerMeter = 0; //  ?
			bitmapInfoHeader.biClrUsed = 0;
			bitmapInfoHeader.biClrImportant =  0;

			fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1,  filePtr);
			fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1,  filePtr);
			for(int i=0;i<windowWidth*windowHeight*3;i++) 
				fputc(bmpBuffer[i], filePtr); 
		//	fwrite(bmpBuffer, windowWidth*windowHeight*3, 1,  filePtr);
			fflush(filePtr);
			fclose(filePtr);
		}
		free(bmpBuffer);
	}
	
	std::fstream File;
	strcat(fileName2,".viewPoint");
	File.open(fileName2,std::ios::app);
	File<<m_Eye[0]<<" "<<m_Eye[1]<<" "<<m_Eye[2]<<endl;
	for(int i=0;i<16;i++)
	 File<<m_RotationMatrix[i]<<" ";
}
void SketchGLCanvas::setViewPort()
{
	wxString fileName = wxFileSelector(wxT("Open ViewPort"), (const wxChar *) NULL,
		(const wxChar *) NULL, wxT("ViewPort"),
		wxT("viewpoint files (*.viewPoint)|*.viewPoint"),wxOPEN);

	if(strlen(fileName)==0){
		return;
	}

	std::ifstream reader(fileName);
	if (!reader.good())
		return ;

	reader >>m_Eye[0]>>m_Eye[1]>>m_Eye[2] ;
	for(int i=0;i<16;i++)
		reader >>m_RotationMatrix[i];

	reader.close();
}
//------------------------------------------------------
void SketchGLCanvas::setRotateTimes(int val)
{
	m_rotationTimes = val;
}
void SketchGLCanvas::rotateModel(){
	if (m_isRotate){
		m_rotationCount+=1;
		m_rotationCount%=362;
		//float rot[3]={0};
		float rot = 1;

		//rot[1] = 0.5;
		//rot[0] = 1;
		//transfer this rotation into the rotation matrix for the scene
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		if ( m_rotationCount < 361) {
			glRotated(rot, 0,1.0,0);// Y axis
			//glRotated(rot [1], 1.0,0,0);
		}
		else{
			if(m_rotationTimes==1)
				m_isRotate=false;
			else{
				m_rotationTimes--;m_rotationCount--;
			}
			m_rotationCount=0;
		}

/*
		else if ( m_rotationCount <= (180+360)) {
			glRotated(rot, 1.0,0,0);// Y axis
			if (m_rotationCount == (180+360))
				m_rotationCount=0;
		}		

*/
		glMultMatrixd(m_RotationMatrix); //accummulate the previous transformations
		glGetDoublev(GL_MODELVIEW_MATRIX, m_RotationMatrix); //update
		glPopMatrix();
	}
}
void SketchGLCanvas::Render()
{
	SetCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//------------------------------
	//	view transformation
	//-----------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (m_Eye[0], m_Eye[1], m_Eye[2]);	// I uncommented this

	//m_RotationMatrix.glMultMatrix();// NO_MATRIX
	glMultMatrixd(m_RotationMatrix);

	//glPushMatrix();
	glColor3d(0.5, 0.5, 1.0);
	// Fatemeh: I commented this "glTranslate" out, since we "uncommented
	// the other glTranslate a couple lines above
	//glTranslatef(0,0,-0.15f);

	//matrix4 deviceRotation;
	//deviceRotation.setIdentity();
	////get the rotation matrix from Sixense

	m_cycleUtils->Render();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glFlush();
	SwapBuffers();

}


void SketchGLCanvas::OnPaint ( wxPaintEvent &WXUNUSED( event ) )
{
	// must always be here
	wxPaintDC dc( this );
	if( ! m_initialized ){
		Initialize();
	}
	Render();
}
std::vector<AML::double3> SketchGLCanvas::computeRay(int mouseX,int mouseY){
	// ********* ray from eye to mouse location *********
	GLint aViewport[4];
	GLdouble matMV[16], matProj[16];
	GLdouble wx, wy, wz;  //  temp world x, y, z coords

	glGetIntegerv (GL_VIEWPORT, aViewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, matMV);
	glGetDoublev (GL_PROJECTION_MATRIX, matProj);
	//  note viewport[3] is height of window in pixels
	mouseY = aViewport[3] - (GLint) mouseY - 1;
	//printf ("Coordinates at cursor are (%4d, %4d)\n", mouseX, mouseY);

	gluUnProject ((GLdouble) mouseX, (GLdouble) mouseY, 0.0,
		matMV, matProj, aViewport, &wx, &wy, &wz);
	AML::double3 lineP0(wx,wy,wz);
	//printf ("World coords at z=0.0 are (%f, %f, %f)\n", wx, wy, wz);

	gluUnProject ((GLdouble) mouseX, (GLdouble) mouseY, 1.0, matMV, matProj, aViewport, &wx, &wy, &wz);
	AML::double3 lineP1(wx,wy,wz);
	//printf ("World coords at z=1.0 are (%f, %f, %f)\n", wx, wy, wz);

	std::vector<AML::double3> ray;
	ray.push_back(lineP0);ray.push_back(lineP1);
	return ray;
}
void SketchGLCanvas::pickArc(int mouseX, int mouseY,enum OperationType oT)
{
	std::vector<AML::double3> ray;
	ray=computeRay(mouseX,mouseY);

	// ************ find curve vertex closest to the ray *********
	m_cycleUtils->pickArc(ray[0],ray[1],oT);
}
void SketchGLCanvas::pickCycle(int mouseX, int mouseY,enum OperationType oT)
{
	std::vector<AML::double3> ray;
	ray=computeRay(mouseX,mouseY);

	// ************ find curve vertex closest to the ray *********
	m_cycleUtils->pickCycle(ray[0],ray[1],oT);
}
void SketchGLCanvas::OnMouse ( wxMouseEvent &event )
{
	int x = event.GetX();
	int y = event.GetY();
	m_cycleUtils->m_isViewing = false;

	SetFocus();
	//for visualization;
	if ( event.Dragging()) {
		if (event.LeftIsDown() )
		{
			//convert the mouse clicked locations to lie between [-1,1] for both X and Y
			AML::Scalar halfWidth = m_width/2.0;
			AML::Scalar halfHeight = m_height/2.0;
			AML::Scalar xNormalized = (x-halfWidth)/halfWidth;
			AML::Scalar yNormalized = (halfHeight-y)/halfHeight;
			AML::Scalar oldXNormalized = (m_lastx-halfWidth)/halfWidth;
			AML::Scalar oldYNormalized = (halfHeight-m_lasty)/halfHeight;
			//rotation rot = gml::trackball<AML::Scalar, gml::UNOPT>(oldXNormalized, oldYNormalized, xNormalized, yNormalized);// NO_MATRIX
			//matrix4 M; // NO_MATRIX
			//rot.toMatrix( M ); // NO_MATRIX
			//matrix4 oldRotM = m_RotationMatrix; // NO_MATRIX
			//m_RotationMatrix.mul(M,oldRotM); // NO_MATRIX

			// rotates screen
			float rot[3]={0};
			//rot[0] -= (m_lasty - y) * 0.5;
			//rot[1] -= (m_lastx - x) * 0.5;
			rot[1] -= (m_lasty - y) * 0.5;
			rot[0] -= (m_lastx - x) * 0.5;
			//------------------------------------------------------------------------
			// If rotation angle is greater of 360 or lesser than -360,
			// resets it back to zero.
			//------------------------------------------------------------------------
			for (unsigned int i=0;i<3;i++)
				if (rot[i] > 360 || rot[i] < -360)
					rot[i] = 0;

			//transfer this rotation into the rotation matrix for the scene
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			//glRotated(-rot [0], 0,1.0,0);
			//glRotated(-rot [1], 1.0,0,0);
			glRotated(rot [0], 0,1.0,0);
			glRotated(rot [1], 1.0,0,0);
			glMultMatrixd(m_RotationMatrix); //accummulate the previous transformations
			glGetDoublev(GL_MODELVIEW_MATRIX, m_RotationMatrix); //update
			glPopMatrix();
		}
		else if (event.RightIsDown()) {
			// translate the screen, z axis gives the idea of zooming in and out
			m_Eye[2] -= (m_lasty - y) * 0.01; // here I multiply by a 0.05 factor to slow down the zoom
		}
		else if (event.MiddleIsDown()) 
		{
			m_Eye[0] -= (m_lastx - x) * 0.01f;
			m_Eye[1] += (m_lasty - y) * 0.01f;
		}
	}
	//for curve network;
	if (!m_cycleUtils->m_cycleDiscovery.m_curveNet.arcs.empty())
	{
		if ( event.ControlDown()) {
			OperationType mode = VIEW;
			pickArc(event.GetX(), event.GetY(),mode);
		}
		if ( event.ControlDown() && event.LeftIsDown()) {
			OperationType mode = SELECT;
			pickArc(event.GetX(), event.GetY(),mode);
		}
		if ( event.ControlDown() && event.RightIsDown()) {
			OperationType mode = ADD;
			pickArc(event.GetX(), event.GetY(),mode);
		}
		if ( event.AltDown()) {
			OperationType mode = VIEW;
			pickCycle(event.GetX(), event.GetY(),mode);
		}
		if ( event.AltDown() && event.LeftIsDown()) {
			OperationType mode = SELECT;
			pickCycle(event.GetX(), event.GetY(),mode);
		}

		if(event.AltDown()||event.ControlDown()||event.ShiftDown())
			m_cycleUtils->m_isViewing = true;
		else
			m_cycleUtils->m_isViewing = false;

	}


	m_lastx = x;
	m_lasty = y;
	Render();
}
void SketchGLCanvas::OnIdle ( wxIdleEvent &event )
{
	if(m_isRotate){
		rotateModel();
		Refresh(false);
//		wxWakeUpIdle();
	//	Sleep(10);
//		SetTimer(1,40,NULL);

	}
}

void SketchGLCanvas::OnSize ( wxSizeEvent &event )
{
	if( ! m_initialized ){
		Initialize();
		m_initialized = true;
	}

	GetClientSize(&m_width, &m_height);

	glViewport(0, 0, (GLint) m_width, (GLint) m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, m_NearPlane, m_NearPlane + m_FarPlaneOffset);

	gluPerspective(m_PerspectiveAngleDegrees,
				   (GLfloat)m_width/(GLfloat)m_height,
				   m_NearPlane,
				   m_NearPlane + m_FarPlaneOffset);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMultMatrixd(m_RotationMatrix);

	wxGLCanvas::OnSize( event );
}

/*
void SketchGLCanvas::OnKeyDown(wxKeyEvent &event)
{
	int keycode = event.m_keyCode;
	if ( keycode == 127){
		GeometryType gT = SHOWSINGLECURVE;
		m_cycleUtils->rollBack(enviroment,gT);
		OperationType oT = DEL;
		m_cycleUtils->updateCurveNetRaw(oT);
	}
	Render();
}*/