#include "OpenGLWindow.h"
#include"ui_QtWidgetsApplication1.h"
#include <QOpenGLWidget>
#include<qmessagebox.h>
#include <cmath>


int lineCounter = 0;
int rectangleCounter = 0;
int circleCounter = 0;
int polylineCounter = 0;
int polylineCounter2 = 0;


float lineStartX = -0.0f;
float lineStartY = -0.0f;
float lineEndX = 0.0f;
float lineEndY = 0.0f;

double p1x = 0;
double p1y = 0;
double p2x = 0;
double p2y = 0;

bool isDrawingLine = false;
bool OpenGLWindow::drawLineMode = false;
bool OpenGLWindow::drawCircleMode = false;
bool OpenGLWindow::drawRectangleMode = false;
bool OpenGLWindow::highlightMode = false;
bool OpenGLWindow::drawPolylineMode = false;
bool OpenGLWindow::drawPolylineMode2 = false;

OpenGLWindow::OpenGLWindow(QWidget* parent)
	: QOpenGLWidget(parent)
{

}

void OpenGLWindow::initializeGL()
{
	initializeOpenGLFunctions();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	shaderProgram = new QOpenGLShaderProgram();
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	shaderProgram->link();

	//shaderProgram->bind();
	//shaderProgram->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f)); // White color
	//shaderProgram->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f)); // White 
}

void OpenGLWindow::paintGL()
{
	glLoadIdentity();
	shaderProgram->bind();

		shaderProgram->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f)); // White 
		for (const auto& item : itemList) {
			for (const auto& line : item.itemEdgeList)
			{
				glBegin(GL_LINES);
				glVertex2f(line.startX, line.startY);
				glVertex2f(line.endX, line.endY);
				glEnd();
			}
		}

		// HighLight Intersection Point
		// 
		//for (const auto& line : db.edgeList)
		//{
		//	glBegin(GL_LINES);
		//	glVertex2f(line.startX, line.startY);
		//	glVertex2f(line.endX, line.endY);
		//	glEnd();
		//}
		
		

		if (highlightMode) {
			shaderProgram->bind();
			shaderProgram->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f)); // Red 
			for (const auto& item : highlightItemList) {
				for (const auto& line : item.itemEdgeList)
				{

					glBegin(GL_LINES);
					glVertex2f(line.startX, line.startY);
					glVertex2f(line.endX, line.endY);
					glEnd();

				}
			}
			
			
			for (const auto& point : it.intersectionPointList) {
				float x1, x2, y1, y2;
				
				for (int i = 0; i < 36; i++)
				{
					
					float theta = 2.0f * 3.14 * float(i) / float(36);

					x1 = point.x - 0.025 * cos(theta);
					y1 = point.y - 0.025 * sin(theta);

					theta = 2.0f * 3.14 * float(i + 1) / float(36);
					x2 = point.x - 0.025 * cos(theta);
					y2 = point.y - 0.025 * sin(theta);
					shaderProgram->bind();
					shaderProgram->setUniformValue("color", QVector4D(0.0f, 1.0f, 1.0f, 1.0f)); // Red 
					glBegin(GL_LINES);
					glVertex2f(x1, y1);
					glVertex2f(x2, y2);
					glEnd();

				}
				
			}
			//db.highlightList.clear();
			highlightMode = false;
			//it.intersectionPointList.clear();
			//update();
		}
	shaderProgram->release();
}

void OpenGLWindow::resizeGL(int width, int height)
{

}

void OpenGLWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{

		// Update line coordinates based on mouse click position
		float x = event->x();
		float y = height() - event->y(); // Flip the y-coordinate

		if (!isDrawingLine)
		{

			lineStartX = -1.0f + (2.0f * x / width());
			lineStartY = -1.0f + (2.0f * y / height());
			lineEndX = lineStartX;
			lineEndY = lineStartY;
			isDrawingLine = true;
		}
		else
		{

			lineEndX = -1.0f + (2.0f * x / width());
			lineEndY = -1.0f + (2.0f * y / height());
			isDrawingLine = false;
		}

		if (drawLineMode) {
			drawLine();

		}
		if (drawCircleMode) {
			drawCircle();

		}
		if (drawRectangleMode) {
			drawRectangle();

		}
		if (drawPolylineMode) {
			drawPolyline();
		}
		if (drawPolylineMode2) {
			drawPolyline2();
		}



		//update(); // Trigger a repaint
	}
}

void OpenGLWindow::drawCircle() {
	circleCounter++;

	int segments = 120;

	DataBase::Item circles;

	// centre = line start point  radius calc form end point //
	// ** calculate the radius ** //
	double radius = sqrt(pow(abs(lineStartX - lineEndX), 2) + pow(abs(lineStartY - lineEndY), 2));


	circles.entityName = "Circle";
	circles.itemNumber = circleCounter / 2;

	if (circleCounter % 2 == 0) {
		// ** segment calculations and adding em to the lines vector ** //
		for (int i = 0; i < segments; i++)
		{
			float theta = 2.0f * 3.14 * float(i) / float(segments);

			float x1 = lineStartX - radius * cos(theta);
			float y1 = lineStartY - radius * sin(theta);

			theta = 2.0f * 3.14 * float(i + 1) / float(segments);
			float x2 = lineStartX - radius * cos(theta);
			float y2 = lineStartY - radius * sin(theta);

			circles.startX = x1;
			circles.startY = y1;
			circles.endX = x2;
			circles.endY = y2;
			db.edgeList.push_back(circles);
			circles.itemEdgeList.push_back(circles);

		}
		itemList.push_back(circles);
	}

	OpenGLWindow::update();                                     // ** UPDATE (REDO) THE PAINTGL ** //
}

void OpenGLWindow::drawRectangle() {
	rectangleCounter++;

	DataBase::Item rectangle;

	rectangle.entityName = "Rectangle";
	rectangle.itemNumber = rectangleCounter / 2;

	if (rectangleCounter % 2 == 0) {
		rectangle.startX = lineStartX;
		rectangle.startY = lineStartY;
		rectangle.endX = lineStartX;
		rectangle.endY = lineEndY;
		db.edgeList.push_back(rectangle);
		rectangle.itemEdgeList.push_back(rectangle);

		rectangle.startX = lineStartX;
		rectangle.startY = lineStartY;
		rectangle.endX = lineEndX;
		rectangle.endY = lineStartY;
		db.edgeList.push_back(rectangle);
		rectangle.itemEdgeList.push_back(rectangle);

		rectangle.startX = lineStartX;
		rectangle.startY = lineEndY;
		rectangle.endX = lineEndX;
		rectangle.endY = lineEndY;
		db.edgeList.push_back(rectangle);
		rectangle.itemEdgeList.push_back(rectangle);

		rectangle.startX = lineEndX;
		rectangle.startY = lineStartY;
		rectangle.endX = lineEndX;
		rectangle.endY = lineEndY;
		db.edgeList.push_back(rectangle);
		rectangle.itemEdgeList.push_back(rectangle);

		itemList.push_back(rectangle);
	}

	OpenGLWindow::update();                                     // ** UPDATE (REDO) THE PAINTGL ** //
}

void OpenGLWindow::drawLine() {
	lineCounter++;

	DataBase::Item line;

	line.itemNumber = lineCounter / 2;
	line.entityName = "Line";
	line.startX = lineStartX;
	line.startY = lineStartY;
	line.endX = lineEndX;
	line.endY = lineEndY;

	if (lineCounter % 2 == 0) {
		db.edgeList.push_back(line);
		line.itemEdgeList.push_back(line);
		itemList.push_back(line);
	}

	OpenGLWindow::update();                                     // ** UPDATE (REDO) THE PAINTGL ** //
}

void OpenGLWindow::renderIntersection() {

	for (int j = 0; j < it.intersectionPointList.size(); j++)
	{

		for (int i = 0; i < 3; i++)
		{
			float theta = 2.0f * 3.14 * float(i) / float(3);

			float x1 = it.intersectionPointList[j].x - 0.05 * cos(theta);
			float y1 = it.intersectionPointList[j].y - 0.05 * sin(theta);

			theta = 2.0f * 3.14 * float(i + 1) / float(3);
			float x2 = it.intersectionPointList[j].x - 0.05 * cos(theta);
			float y2 = it.intersectionPointList[j].y - 0.05 * sin(theta);

			DataBase::Item intersectionPoint;
			intersectionPoint.entityName = "Intersection-Point";
			intersectionPoint.startX = x1;
			intersectionPoint.startY = y1;
			intersectionPoint.endX = x2;
			intersectionPoint.endY = y2;
			db.edgeList.push_back(intersectionPoint);

		}
	}

	OpenGLWindow::update();
}

void OpenGLWindow::reset() {
	db.edgeList.clear();
	db.highlightList.clear();
	itemList.clear();
	it.intersectionPointList.clear();
	highlightItemList.clear();
	lineCounter = 0;
	rectangleCounter = 0;
	circleCounter = 0;
	polylineCounter = 0;
	OpenGLWindow::update();
}

void OpenGLWindow::highlight() {

	shaderProgram->setUniformValue("color", QVector4D(0.0f, 1.0f, 0.0f, 1.0f)); // Yellow color

}

void OpenGLWindow::drawPolyline2() {
	polylineCounter2++;

	
	DataBase::Item polyline;

	polyline.entityName = "Polyline(4->pt)";
	polyline.itemNumber = polylineCounter2 / 4;

	// ** 4 Points as input ** //
	if (polylineCounter2 % 4 == 0) {
		double i = 1.0 / 10.0;
		for (double u = 0; u < 0.9; u += i)
		{
			double x = (p1x * pow((1 - u), 3)) + 3 * (p2x * (pow((1 - u), 2)) * (u)) + 3 * (lineStartX * (1 - u) * (pow((u), 2))) + (lineEndX * pow(u, 3));
			double y = (p1y * pow((1 - u), 3)) + 3 * (p2y * (pow((1 - u), 2)) * (u)) + 3 * (lineStartY * (1 - u) * (pow((u), 2))) + (lineEndY * pow(u, 3));

			double v = u + 0.1;
			double x2 = (p1x * pow((1 - v), 3)) + 3 * (p2x * (pow((1 - v), 2)) * (v)) + 3 * (lineStartX * (1 - v) * (pow((v), 2))) + (lineEndX * pow(v, 3));
			double y2 = (p1y * pow((1 - v), 3)) + 3 * (p2y * (pow((1 - v), 2)) * (v)) + 3 * (lineStartY * (1 - v) * (pow((v), 2))) + (lineEndY * pow(v, 3));

			DataBase::Edge edge;
			edge.startX = x;
			edge.startY = y;
			edge.endX = x2;
			edge.endY = y2;

			polyline.itemEdgeList.push_back(edge);
			
		}
		itemList.push_back(polyline);
		
	}
	else {
		if (polylineCounter2 % 2 == 0 && polylineCounter2 % 4 != 0) {
			p1x = lineStartX;
			p1y = lineStartY;
			p2x = lineEndX;
			p2y = lineEndY;
		}
		
	}
	OpenGLWindow::update();
}

void OpenGLWindow::drawPolyline() {
	polylineCounter++;


	DataBase::Item polyline;

	polyline.entityName = "Polyline";
	polyline.itemNumber = polylineCounter / 3;

	// ** 3 Points as input ** //
	if (polylineCounter % 3 == 0) {
		double i = 1.0 / 10.0;
		for (double u = 0; u < 0.9; u += i)
		{
			double x = (p1x * pow((1 - u), 2)) + 2 * (p2x * (1 - u) * (u)) + (lineStartX * pow(u, 2));
			double y = (p1y * pow((1 - u), 2)) + 2 * (p2y * (u) * (1 - u)) + (lineStartY * pow(u, 2));

			double v = u + 0.1;
			double x2 = (p1x * pow((1 - v), 2)) + 2 * (p2x * (1 - v) * (v)) + (lineStartX * pow(v, 2));
			double y2 = (p1y * pow((1 - v), 2)) + 2 * (p2y * (v) * (1 - v)) + (lineStartY * pow(v, 2));

			DataBase::Edge edge;
			edge.startX = x;
			edge.startY = y;
			edge.endX = x2;
			edge.endY = y2;

			polyline.itemEdgeList.push_back(edge);

		}
		itemList.push_back(polyline);
		isDrawingLine = false;
	}
	else {
		p1x = lineStartX;
		p1y = lineStartY;
		p2x = lineEndX;
		p2y = lineEndY;
	}
	OpenGLWindow::update();
}