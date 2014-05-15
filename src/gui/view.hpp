#ifndef __VIEW__
#define __VIEW__

#include <QtOpenGL>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QTimer>

#include "vector.hpp"
#include "writer.hpp"

class PointSetView : public QGLWidget, protected QGLFunctions
{
Q_OBJECT

private:
	//Options
	bool m_optShowDomain;
	bool m_optShowSampleDot;
	bool m_optShowSampleDisk;
	float m_optSampleDiskRadius;
	
	//Data
	QTimer* m_timer;
	bool m_glContext;
	
	//Controller
	float m_spatialZoom;
	QVector2D m_spatialPosition;
	QVector2D m_lastMousePos;
	
	//OpenGL
	QGLShaderProgram m_shaderDefault;
	QGLShaderProgram m_shaderDot;
	QGLShaderProgram m_shaderDisk;
	QGLShaderProgram m_shaderDFunc;
		//Buffer Pointset
	QGLBuffer m_bufferPtsPosition;
	QGLBuffer m_bufferPtsColor;
	int m_bufferPtsSize;
		//Buffer domain
	QGLBuffer m_bufferDom;
	int m_bufferDomSize;

	float m_spaceSize;
	float m_densityMin;
	float m_densityMax;
	int m_dFuncIndex;
	int m_dFuncMode;
	int m_subdivFactor;
	
	int m_attributePosition;
	int m_attributeColor;
	
	QMatrix4x4 m_projectionMatrix;
	
	//Current pointset
	bool m_pointsetLoaded;
	float m_meanDistance;

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	
	void drawPointset();
	
	void mouseMoveEvent(QMouseEvent* mouseEvent);
	void wheelEvent(QWheelEvent* wheelEvent);
	
public:
	PointSetView(QWidget *parent = 0);
	~PointSetView();
	
	void showDomain(bool v);
	void showSampleDot(bool v);
	void showSampleDisk(bool v);
	void setSampleDiskRadius(float v);
	
	void removePointset();
	void updatePointset(WriterVector& writer, const float& spaceSize, const float& densityMin, const float& densityMax, const int& dFuncIndex, const int& dFuncMode);
	
public slots:
	void timeOutSlot();
	void setZoom(double);
	
signals:
	void pointsetCursorMoved(float x, float y);
	void zoomChanged(float z);
};

#endif
