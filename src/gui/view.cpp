#include <iostream>
#include <sstream>
#include "config.h"
#include "view.hpp"

#define DIR_SHADERS PROJECT_SOURCE_DIR"/data/shader/"

PointSetView::PointSetView(QWidget *parent) :
	m_timer(NULL),
	m_glContext(false),
	m_optShowDomain(true),
	m_optShowSampleDot(false),
	m_optShowSampleDisk(true),
	m_optSampleDiskRadius(0.1f),
	m_spatialZoom(2.0f),
	m_spatialPosition(0., 0.),
	m_pointsetLoaded(false),
	m_bufferPtsPosition(QGLBuffer::VertexBuffer),
	m_bufferPtsColor(QGLBuffer::VertexBuffer),
	m_spaceSize(0.21),
	m_dFuncIndex(0),
	m_dFuncMode(0),
	m_subdivFactor(37),
	QGLWidget(parent)
{
	//Anti aliasing
	QGLFormat newFormat = this->format();
	newFormat.setSampleBuffers(true);
	newFormat.setSamples(16);
	this->setFormat(newFormat);

	setMouseTracking(true);

	//Timer
	const int framesPerSecond = 30;
	if(framesPerSecond > 0)
	{
		int timerInterval = 1000 / framesPerSecond;
		m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
		m_timer->start(timerInterval);
	}
}

PointSetView::~PointSetView()
{
	if(m_timer != NULL) delete m_timer;
}

void PointSetView::removePointset()
{
	m_bufferPtsPosition.destroy();
	m_pointsetLoaded = false;
}

void PointSetView::updatePointset(WriterVector& writer, const float& spaceSize, const float& densityMin, const float& densityMax, const int& dFuncIndex, const int& dFuncMode)
{
	if(m_glContext)
	{		
		m_spaceSize = spaceSize;
		m_dFuncIndex = dFuncIndex;
		m_dFuncMode = dFuncMode;
		m_densityMin = densityMin;
		m_densityMax = densityMax;
		m_meanDistance = 2*spaceSize/std::sqrt(writer.pts().size());

		//Domain
		{
			std::vector<float> domainPolygon;
			domainPolygon.push_back(-spaceSize);
			domainPolygon.push_back(-spaceSize);
			domainPolygon.push_back(-spaceSize);
			domainPolygon.push_back(spaceSize);
			domainPolygon.push_back(spaceSize);
			domainPolygon.push_back(spaceSize);
			domainPolygon.push_back(spaceSize);
			domainPolygon.push_back(-spaceSize);
			domainPolygon.push_back(-spaceSize);
			domainPolygon.push_back(-spaceSize);

			m_bufferDomSize = 5; //Nombre de sommet
			int vboSz = m_bufferDomSize*sizeof(float)*2;
			const void* vboData = &domainPolygon[0];

			m_bufferDom.destroy();
			m_bufferDom.create();
			m_bufferDom.bind();
			m_bufferDom.allocate(vboData, vboSz);
			m_bufferDom.release();
		}

		//Pointset
		if(writer.colors().size() >0)
		{
			m_bufferPtsSize = writer.pts().size();
			int vboSz = m_bufferPtsSize*sizeof(float)*2;
			const void* vboPos = &writer.pts().at(0);

			m_bufferPtsPosition.destroy();
			m_bufferPtsPosition.create();
			m_bufferPtsPosition.bind();
			m_bufferPtsPosition.allocate(vboPos, vboSz);
			m_bufferPtsPosition.release();

			int vboColorSz = m_bufferPtsSize*sizeof(float)*4;
			const void* vboColor = &writer.colors().at(0);

			m_bufferPtsColor.destroy();
			m_bufferPtsColor.create();
			m_bufferPtsColor.bind();
			m_bufferPtsColor.allocate(vboColor, vboColorSz);
			m_bufferPtsColor.release();
		}

		m_pointsetLoaded = true;
	}
}

void PointSetView::initializeGL()
{
	m_glContext = true;

	//Default shader
	m_shaderDefault.addShaderFromSourceFile(QGLShader::Vertex, DIR_SHADERS"default.vert.glsl");
	m_shaderDefault.addShaderFromSourceFile(QGLShader::Fragment, DIR_SHADERS"default.frag.glsl");
	if(!m_shaderDefault.link())
	{
		QString error = m_shaderDefault.log();
		std::cerr << error.toUtf8().constData() << std::endl;
	}

	//Dot shader
	m_shaderDot.addShaderFromSourceFile(QGLShader::Vertex, DIR_SHADERS"dot.vert.glsl");
	m_shaderDot.addShaderFromSourceFile(QGLShader::Fragment, DIR_SHADERS"dot.frag.glsl");
	if(!m_shaderDot.link())
	{
		QString error = m_shaderDot.log();
		std::cerr << error.toUtf8().constData() << std::endl;
	}

	//Disk shader
	m_shaderDisk.addShaderFromSourceFile(QGLShader::Vertex, DIR_SHADERS"disk.vert.glsl");
	m_shaderDisk.addShaderFromSourceFile(QGLShader::Geometry, DIR_SHADERS"disk.geom.glsl");
	m_shaderDisk.addShaderFromSourceFile(QGLShader::Fragment, DIR_SHADERS"disk.frag.glsl");
	if(!m_shaderDisk.link())
	{
		QString error = m_shaderDisk.log();
		std::cerr << error.toUtf8().constData() << std::endl;
	}

	//Default shader
	m_shaderDFunc.addShaderFromSourceFile(QGLShader::Vertex, DIR_SHADERS"dfunc.vert.glsl");
	m_shaderDFunc.addShaderFromSourceFile(QGLShader::Fragment, DIR_SHADERS"dfunc.frag.glsl");
	if(!m_shaderDFunc.link())
	{
		QString error = m_shaderDFunc.log();
		std::cerr << error.toUtf8().constData() << std::endl;
	}

	m_attributePosition = m_shaderDisk.attributeLocation("a_position");
	m_attributeColor = m_shaderDisk.attributeLocation("a_color");

	glViewport(0, 0, width(), height());
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
}

void PointSetView::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
}

void PointSetView::drawPointset()
{
	m_projectionMatrix.setToIdentity();
	if(height() < width())
	{
		m_projectionMatrix.scale(2.0*height()/width(), -2.0, 1.0);
	}
	else
	{
		m_projectionMatrix.scale(2.0, -2.0*width()/height(), 1.0);
	}
	m_projectionMatrix.scale(m_spatialZoom, -m_spatialZoom, 1.0);
	m_projectionMatrix.translate(-m_spatialPosition.x(), -m_spatialPosition.y(), 0.0);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!m_pointsetLoaded) return;

	glPointSize(2.0);

	m_shaderDFunc.bind();
	m_shaderDFunc.setUniformValue("u_projectionMatrix", m_projectionMatrix);
	m_shaderDFunc.setUniformValue("u_spaceSize", m_spaceSize);
	m_shaderDFunc.setUniformValue("u_subdivFactor", m_subdivFactor);
	m_shaderDFunc.setUniformValue("u_dfuncIndex", m_dFuncIndex);
	m_shaderDFunc.setUniformValue("u_dfuncMode", m_dFuncMode);
	m_shaderDFunc.setUniformValue("u_dmin", m_densityMin);
	m_shaderDFunc.setUniformValue("u_dmax", m_densityMax);
	glBegin(GL_QUADS);
	glVertex2f(-m_spaceSize, -m_spaceSize);
	glVertex2f(-m_spaceSize, m_spaceSize);
	glVertex2f(m_spaceSize, m_spaceSize);
	glVertex2f(m_spaceSize, -m_spaceSize);
	glEnd();
	m_shaderDFunc.release();

	if(m_optShowSampleDot)
	{
		m_shaderDot.bind();
		m_shaderDot.setUniformValue("u_projectionMatrix", m_projectionMatrix);

		m_bufferPtsPosition.bind();
		m_shaderDot.enableAttributeArray(m_attributePosition);
		m_shaderDot.setAttributeBuffer(m_attributePosition, GL_FLOAT, 0, 2);
		m_bufferPtsPosition.release();

		m_bufferPtsColor.bind();
		m_shaderDot.enableAttributeArray(m_attributeColor);
		m_shaderDot.setAttributeBuffer(m_attributeColor, GL_FLOAT, 0, 4);
		m_bufferPtsColor.release();

		glDrawArrays(GL_POINTS, 0, m_bufferPtsSize);

		m_shaderDot.release();
	}

	if(m_optShowSampleDisk)
	{
		m_shaderDisk.bind();
		m_shaderDisk.setUniformValue("u_projectionMatrix", m_projectionMatrix);
		m_shaderDisk.setUniformValue("u_diskRadius", m_optSampleDiskRadius*m_meanDistance);

		m_bufferPtsPosition.bind();
		m_shaderDisk.enableAttributeArray(m_attributePosition);
		m_shaderDisk.setAttributeBuffer(m_attributePosition, GL_FLOAT, 0, 2);
		m_bufferPtsPosition.release();

		m_bufferPtsColor.bind();
		m_shaderDisk.enableAttributeArray(m_attributeColor);
		m_shaderDisk.setAttributeBuffer(m_attributeColor, GL_FLOAT, 0, 4);
		m_bufferPtsColor.release();


		glDrawArrays(GL_POINTS, 0, m_bufferPtsSize);

		m_shaderDefault.release();
	}

	if(m_optShowDomain)
	{
		m_shaderDefault.bind();
		m_shaderDefault.setUniformValue("u_projectionMatrix", m_projectionMatrix);
		m_shaderDefault.setUniformValue("u_color", QVector4D(0.8, 0.8, 0.8, 1.0));

		m_bufferDom.bind();
		m_shaderDefault.enableAttributeArray(m_attributePosition);
		m_shaderDefault.setAttributeBuffer(m_attributePosition, GL_FLOAT, 0, 2);
		m_bufferDom.release();

		glDrawArrays(GL_LINE_STRIP, 0, m_bufferDomSize);

		m_shaderDefault.release();
	}
}

void PointSetView::paintGL()
{
	drawPointset();
}

void PointSetView::mouseMoveEvent(QMouseEvent* mouseEvent)
{	
	QMatrix4x4 matrixMousePos = m_projectionMatrix.inverted();

	//View move
	{
		QVector3D pos(mouseEvent->pos().x()/(float)width(), mouseEvent->pos().y()/(float)height(), 0.0);
		pos.setX(pos.x()*2.0-1.0);
		pos.setY((1.0-pos.y())*2.0-1.0);

		QVector3D posTrans3d = matrixMousePos * pos;
		QVector2D posTrans(posTrans3d.x(), posTrans3d.y());

		QVector3D lastPosTrans3d = matrixMousePos * QVector3D(m_lastMousePos.x(), m_lastMousePos.y(), 0.0);
		QVector2D lastPosTrans(lastPosTrans3d.x(), lastPosTrans3d.y());

		m_lastMousePos.setX(pos.x());
		m_lastMousePos.setY(pos.y());

		if(mouseEvent->buttons() == Qt::LeftButton)
		{
			m_spatialPosition -= (posTrans - lastPosTrans);
		}
	}

	//Mouse position information
	{
		QVector3D pos(mouseEvent->pos().x()/(float)width(), mouseEvent->pos().y()/(float)height(), 0.0);
		pos.setX(pos.x()*2.0-1.0);
		pos.setY((1.0-pos.y())*2.0-1.0);

		pos = matrixMousePos * pos;

		emit pointsetCursorMoved(pos.x(), pos.y());
	}
}

void PointSetView::wheelEvent(QWheelEvent* wheelEvent)
{
	float v = wheelEvent->delta()/1500.0f;
	v = std::max(std::min(v, 1.0f), -1.0f);

	m_spatialZoom *= 1.0+v;
	emit zoomChanged(m_spatialZoom);
}

void PointSetView::timeOutSlot()
{
	updateGL();
}

void PointSetView::showDomain(bool v)
{
	m_optShowDomain = v;
}

void PointSetView::showSampleDot(bool v)
{
	m_optShowSampleDot = v;
}

void PointSetView::showSampleDisk(bool v)
{
	m_optShowSampleDisk = v;
}

void PointSetView::setSampleDiskRadius(float v)
{
	m_optSampleDiskRadius = v;
}

void PointSetView::setZoom(double z)
{
	m_spatialZoom = z;

	emit zoomChanged(z);
}
