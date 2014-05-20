#include "window.hpp"

#include <iostream>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(Sampler* sampler)
: m_sampler(sampler), m_view(NULL), m_lut_index(-1)
, m_seed(0), m_spaceSize(0.21), m_min_density(1), m_max_density(4e3), m_min_level(0), m_max_level(1), m_min_rank(0), m_max_rank(1), m_offsetInterpolate(true), m_dither(true)
, m_crop(true)
, m_DFunc(&dFunc_constant), m_image()//"data/img/butterfly.png")
{
	//~ qRegisterMetaType<PickedValue>("PickedValue");

	//main window properties
	this->resize(1024, 700);
	this->setWindowTitle(tr("Sampler"));

	//splitter ( view | options )
	QSplitter* splitter = new QSplitter();
	this->setCentralWidget(splitter);

	m_view = new PointSetView();
	createOptionPanel();
	splitter->addWidget(m_view);
	splitter->setStretchFactor(0,1);
	splitter->addWidget(options);
	splitter->setStretchFactor(1,0);

	setDensityMin();
	setDensityMax();

	createStatusBar();
	connect(m_view, SIGNAL(pointsetCursorMoved(float, float)), this, SLOT(showCoordinates(float, float)));
}

MainWindow::~MainWindow()
{
	if(m_view != NULL) delete m_view;
}


void MainWindow::createOptionPanel()
{
	options = new QWidget();
	optionsLayout = new QVBoxLayout();
	options->setLayout(optionsLayout);

	createOptionGenerate();
	optionsLayout->addWidget(generateGroup);

	createOptionParameterGroup();
	optionsLayout->addWidget(optionParameterGroup);

	createOptionDisplayGroup();
	optionsLayout->addWidget(optionDisplayGroup);

	optionsLayout->addStretch(1);

	createOptionQuitButton();
	optionsLayout->addWidget(quitButton);
}

void MainWindow::createOptionParameterGroup()
{
	optionParameterGroup = new QGroupBox(tr("Parameters"));
	optionParameterGroupLayout = new QFormLayout;
	optionParameterGroup->setLayout(optionParameterGroupLayout);
	createOptionLut();
	optionParameterGroupLayout->addRow(tr("Offset lut"), optionLut);
	createOptionDFunc();
	optionParameterGroupLayout->addRow(tr("Importance fct"), optionDFunc);
	createOptionImage();
	optionParameterGroupLayout->addRow(tr("Imp. image file"), optionImage);
	createOptionDensity();
	optionParameterGroupLayout->addRow(tr("Density (min/max)"), optionDensityGroup);
	createOptionSeed();
	optionParameterGroupLayout->addRow(tr("Init. seed"), optionSeed);
	createOptionSpaceSz();
	optionParameterGroupLayout->addRow(tr("Space size"), optionSpaceSz);
	createOptionLevel();
	optionParameterGroupLayout->addRow(tr("Level (min/max)"), optionLevelGroup);
	createOptionRank();
	optionParameterGroupLayout->addRow(tr("Rank (min/max)"), optionRankGroup);
	createOptionOffsetInterpolate();
	optionParameterGroupLayout->addRow(optionOffsetInterpolate);
	createOptionDither();
	optionParameterGroupLayout->addRow(optionDither);
}

void MainWindow::createOptionLut()
{
	optionLut = new QComboBox;
	optionLut->addItem(tr("Reference point"));
	optionLut->addItem(tr("Barycenter"));
	optionLut->addItem(tr("#1"));
	connect( optionLut, SIGNAL(currentIndexChanged(int)), this, SLOT( getLut(int) ) );
}

void MainWindow::createOptionDFunc()
{
	optionDFunc = new QComboBox;
	optionDFunc->addItem(tr("Constant Optim."));
	optionDFunc->addItem(tr("Constant"));
	optionDFunc->addItem(tr("Linear"));
	optionDFunc->addItem(tr("Quadratic"));
	optionDFunc->addItem(tr("Radial"));
	optionDFunc->addItem(tr("Zoneplate"));
	optionDFunc->addItem(tr("Contrast"));
	optionDFunc->addItem(tr("Image"));
	connect( optionDFunc, SIGNAL(currentIndexChanged(int)), this, SLOT( getDFunc(int) ) );
}

void MainWindow::createOptionImage()
{
	optionImage = new QPushButton();//"butterfly.png");
	m_image_dir.setPath("./");
	connect( optionImage, SIGNAL(clicked(bool)), this, SLOT( getImageFile() ) );
	optionImage->setEnabled(false);
}

void MainWindow::createOptionSeed()
{
	optionSeed = new QSpinBox;
	optionSeed->setRange(0, 407);
	optionSeed->setSingleStep(1);
	optionSeed->setValue(m_seed);
	connect( optionSeed, SIGNAL(valueChanged(int)), this, SLOT( getSeed(int) ) );
}

void MainWindow::createOptionDensity()
{
	optionDensityGroup = new QWidget;
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	optionDensityGroup->setLayout(layout);

	optionDensityMin = new QLineEdit(QString().setNum(m_min_density));
	optionDensityMax = new QLineEdit(QString().setNum(m_max_density));
	optionDensityMin->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	optionDensityMax->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	layout->addWidget(optionDensityMin);
	layout->addWidget(optionDensityMax);

	optionDensityValidator = new QIntValidator(0, 1e9, this);
	optionDensityMin->setValidator(optionDensityValidator);
	optionDensityMax->setValidator(optionDensityValidator);
	optionDensityMin->setEnabled(false);

	connect( optionDensityMin, SIGNAL(editingFinished()), this, SLOT( getDensityMin() ) );
	connect( optionDensityMax, SIGNAL(editingFinished()), this, SLOT( getDensityMax() ) );
}

void MainWindow::createOptionSpaceSz()
{
	optionSpaceSz = new QLineEdit(QString().setNum(m_spaceSize));
	optionSpaceSzValidator = new QDoubleValidator(0., 10., 3, this);
	optionSpaceSz->setValidator(optionSpaceSzValidator);
	connect( optionSpaceSz, SIGNAL(editingFinished()), this, SLOT( getSpaceSz() ) );
}

void MainWindow::createOptionLevel()
{
	optionLevelGroup = new QWidget;
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	optionLevelGroup->setLayout(layout);

	optionLevelMin = new QSpinBox;
	optionLevelMax = new QSpinBox;
	layout->addWidget(optionLevelMin);
	layout->addWidget(optionLevelMax);

	optionLevelMin->setRange(0, 10);
	optionLevelMax->setRange(0, 10);
	optionLevelMin->setSingleStep(1);
	optionLevelMax->setSingleStep(1);
	optionLevelMin->setValue(m_min_level);
	optionLevelMax->setValue(m_max_level);
	optionLevelMin->setEnabled(false);

	connect( optionLevelMin, SIGNAL(valueChanged(int)), this, SLOT( getLevelMin(int) ) );
	connect( optionLevelMax, SIGNAL(valueChanged(int)), this, SLOT( getLevelMax(int) ) );
}

void MainWindow::createOptionRank()
{
	optionRankGroup = new QWidget;
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	optionRankGroup->setLayout(layout);

	optionRankMin = new QSpinBox;
	optionRankMax = new QSpinBox;
	layout->addWidget(optionRankMin);
	layout->addWidget(optionRankMax);

	optionRankMin->setRange(0, 35);
	optionRankMax->setRange(0, 35);
	optionRankMin->setSingleStep(1);
	optionRankMax->setSingleStep(1);
	optionRankMin->setValue(m_min_rank);
	optionRankMax->setValue(m_max_rank);
	optionRankMin->setEnabled(false);

	connect( optionRankMin, SIGNAL(valueChanged(int)), this, SLOT( getRankMin(int) ) );
	connect( optionRankMax, SIGNAL(valueChanged(int)), this, SLOT( getRankMax(int) ) );
}
void MainWindow::createOptionOffsetInterpolate()
{
	optionOffsetInterpolate = new QCheckBox(tr("&Interpolate offsets"));
	optionOffsetInterpolate->setChecked(m_offsetInterpolate);
	connect( optionOffsetInterpolate, SIGNAL(stateChanged(int)), this, SLOT( getOffsetInterpolate(int) ) );
}
void MainWindow::createOptionDither()
{
	optionDither = new QCheckBox(tr("&Dithering"));
	optionDither->setChecked(m_dither);
	connect( optionDither, SIGNAL(stateChanged(int)), this, SLOT( getDither(int) ) );
}

void MainWindow::createOptionDisplayGroup()
{
	optionDisplayGroup = new QGroupBox(tr("Display options"));
	optionDisplayGroupLayout = new QFormLayout;
	optionDisplayGroup->setLayout(optionDisplayGroupLayout);
	createOptionSampleShape();
	optionDisplayGroupLayout->addRow(tr("Sample shape"), optionSampleShape);
	optionDisplayGroupLayout->addWidget(optionDiskRadius);
	createOptionSampleColor();
	optionDisplayGroupLayout->addRow(tr("Sample colors"), optionSampleColor);
	createOptionBackground();
	optionDisplayGroupLayout->addRow(tr("Background"), optionBackground);
	createOptionCrop();
	optionDisplayGroupLayout->addRow(optionCrop);
}
void MainWindow::createOptionSampleShape()
{
	optionSampleShape = new QComboBox();
	optionSampleShape->addItem(tr("Point"));
	optionSampleShape->addItem(tr("Disk"));
	optionSampleShape->setCurrentIndex(1);
	connect( optionSampleShape, SIGNAL(currentIndexChanged(int)), this, SLOT( getSampleShape(int) ) );
	optionDiskRadius = new QDoubleSpinBox;
	optionDiskRadius->setRange(0.01, 3.);
	optionDiskRadius->setSingleStep(0.1);
	optionDiskRadius->setValue(0.1);
	connect( optionDiskRadius, SIGNAL(valueChanged(double)), this, SLOT( getDiskRadius(double) ) );
}
void MainWindow::createOptionSampleColor()
{
	optionSampleColor = new QComboBox();
	optionSampleColor->addItem(tr("None"));
	optionSampleColor->addItem(tr("Subdivision Level"));
	optionSampleColor->addItem(tr("Parent tile"));
	optionSampleColor->addItem(tr("Rank"));
	connect( optionSampleColor, SIGNAL(currentIndexChanged(int)), this, SLOT( getSampleColor(int) ) );
}
void MainWindow::createOptionBackground()
{
	optionBackground = new QComboBox();
	optionBackground->addItem(tr("None"));
	optionBackground->addItem(tr("Automatic"));
	optionBackground->addItem(tr("Density"));
	optionBackground->addItem(tr("Subdivision Level"));
	optionBackground->addItem(tr("Rank"));
	connect( optionBackground, SIGNAL(currentIndexChanged(int)), this, SLOT( getBackground(int) ) );
}
void MainWindow::createOptionCrop()
{
	optionCrop = new QCheckBox(tr("&Crop samples outside domain"));
	optionCrop->setChecked(m_crop);
	connect( optionCrop, SIGNAL(stateChanged(int)), this, SLOT( getCrop(int) ) );
}

void MainWindow::createOptionGenerate()
{
	generateGroup = new QWidget;
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0,0,0,0);
	generateGroup->setLayout(layout);

	generateButton = new QPushButton();
	generateButton->setText(tr("Generate"));
	layout->addWidget(generateButton);
	connect(generateButton, SIGNAL(clicked(bool)), this, SLOT(generatePointset()));

	generateAuto = new QCheckBox(tr("&Auto"));
	generateAuto->setFixedWidth(60);
	layout->addWidget(generateAuto);
	connect( generateAuto, SIGNAL(stateChanged(int)), this, SLOT( getAuto(int) ) );

}

void MainWindow::createOptionQuitButton()
{
	quitButton = new QPushButton();
	quitButton->setText(tr("Quit"));
	connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

void MainWindow::createStatusBar()
{
	statusBar();
	statusPositionLabel = new QLabel(tr("pos: (-, -)"));
	statusDensityLabel = new QLabel(tr("| dens: - (-)"));
	statusLevelsLabel = new QLabel(tr("| lvl: -, rank: -"));
	statusPositionLabel->setFixedWidth(129);
	statusDensityLabel->setFixedWidth(160);
	statusLevelsLabel->setFixedWidth(110);
	statusBar()->addWidget(statusPositionLabel);
	statusBar()->addWidget(statusDensityLabel);
	statusBar()->addWidget(statusLevelsLabel);
}


void MainWindow::setDensityMin()
{
	m_min_density = computeDensity(m_min_level, m_min_rank, m_spaceSize, m_sampler->tiling().subdivFactor());
	optionDensityMin->setText(QString().setNum((float)m_min_density, 'g', 3));
}

void MainWindow::setDensityMax()
{
	m_max_density = computeDensity(m_max_level, m_max_rank, m_spaceSize, m_sampler->tiling().subdivFactor());
	optionDensityMax->setText(QString().setNum((float)m_max_density, 'g', 3));
}

void MainWindow::setSpaceSz(float spaceSize)
{
	m_spaceSize = spaceSize;
	optionSpaceSz->setText(QString().setNum(m_spaceSize));
}

void MainWindow::setLevelMin()
{
	m_min_level = computeLvlMinusHalfRank(m_min_density, m_spaceSize, m_sampler->tiling().subdivFactor());
	optionLevelMin->setValue(m_min_level);
}

void MainWindow::setLevelMax()
{
	m_max_level = computeLvlMinusHalfRank(m_max_density, m_spaceSize, m_sampler->tiling().subdivFactor());
	optionLevelMax->setValue(m_max_level);
}

void MainWindow::setRankMin()
{
	m_min_rank = computeRank(m_min_density, m_spaceSize, m_min_level, m_sampler->tiling().subdivFactor());
	optionRankMin->setValue(m_min_rank);
}

void MainWindow::setRankMax()
{
	m_max_rank = computeRank(m_max_density, m_spaceSize, m_max_level, m_sampler->tiling().subdivFactor());
	optionRankMax->setValue(m_max_rank);
}

void MainWindow::getLut(int index)
{
	m_lut_index = index-2;
	if(m_gen_auto) generatePointset();
}

void MainWindow::getDFunc(int index)
{
	bool constant = true;
	bool image = false;
	switch(index)
	{
		case 0:
			m_DFunc = &dFunc_constant;
			constant = false;
			break;
		case 1:
			m_DFunc = &dFunc_constant;
			break;
		case 2:
			m_DFunc = &dFunc_linear;
			break;
		case 3:
			m_DFunc = &dFunc_quadra;
			break;
		case 4:
			m_DFunc = &dFunc_radial;
			break;
		case 5:
			m_DFunc = &dFunc_zoneplate;
			break;
		case 6:
			m_DFunc = &dFunc_contrast;
			break;
		case 7:
			m_DFunc = &(m_image.dFunc);
			image = true;
			break;
	}
	optionDensityMin->setEnabled(constant);
	optionLevelMin->setEnabled(constant);
	optionRankMin->setEnabled(constant);
	optionCrop->setEnabled(!constant);
	optionImage->setEnabled(image);

	if(m_gen_auto) generatePointset();
}

void MainWindow::getImageFile()
{
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), m_image_dir.path(), tr("Image (*.*)"));
	if(!fname.isNull())
	{
		QFileInfo file(fname);
		m_image.load(file.filePath().toStdString());
		optionImage->setText(file.fileName());
		m_image_dir = file.dir();
		if(m_gen_auto) generatePointset();
	}
}

void MainWindow::getDensityMin()
{
	m_min_density = optionDensityMin->text().toUInt();
	setLevelMin();
	setRankMin();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getDensityMax()
{
	m_max_density = optionDensityMax->text().toUInt();
	setLevelMax();
	setRankMax();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getSeed(int seed)
{
	m_seed = seed;
	optionSeed->setValue(m_seed);
	if(m_gen_auto) generatePointset();
}

void MainWindow::getSpaceSz()
{
	m_spaceSize = optionSpaceSz->text().toFloat();
	setDensityMin();
	setDensityMax();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getLevelMin(int level)
{
	m_min_level = level;
	setDensityMin();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getLevelMax(int level)
{
	m_max_level = level;
	setDensityMax();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getRankMin(int rank)
{
	m_min_rank = rank;
	setDensityMin();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getRankMax(int rank)
{
	m_max_rank = rank;
	setDensityMax();
	if(m_gen_auto) generatePointset();
}

void MainWindow::getOffsetInterpolate(int state)
{
	m_offsetInterpolate = state;
	if(m_gen_auto) generatePointset();
}

void MainWindow::getDither(int state)
{
	m_dither = state;
	if(m_gen_auto) generatePointset();
}

void MainWindow::getSampleShape(int index)
{
	m_view->showSampleDot(false);
	m_view->showSampleDisk(false);
	optionDiskRadius->setEnabled(false);
	switch(index)
	{
		case 0:
			m_view->showSampleDot(true);
			break;
		case 1:
			m_view->showSampleDisk(true);
			optionDiskRadius->setEnabled(true);
			break;
	}
	if(m_gen_auto) updatePointset();
}

void MainWindow::getDiskRadius(double rad)
{
	m_view->setSampleDiskRadius(rad);
	if(m_gen_auto) updatePointset();
}

void MainWindow::getSampleColor(int index)
{
	m_write.setColorization(index);
	if(m_gen_auto) generatePointset();
}

void MainWindow::getBackground(int index)
{
	if(m_gen_auto) updatePointset();
}

void MainWindow::getCrop(int state)
{
	m_crop = state;
	if(m_gen_auto) generatePointset();
}

void MainWindow::getAuto(int state)
{
	m_gen_auto = state;
	generateButton->setEnabled(!m_gen_auto);
}

void MainWindow::showCoordinates(float x, float y)
{
	QString str = QString("pos: (%1, %2)").arg(QString::number(x, 'f', 3), 6).arg(QString::number(y, 'f', 3), 6);
	statusPositionLabel->setText(str);

	float dbase = m_DFunc(Point(x,y)/(2.*m_spaceSize));
	float d = (m_max_density-m_min_density)*dbase+m_min_density;
	str = QString(" | dens: %1 (%2)").arg(QString::number(float(d), 'e', 2), 6).arg(QString::number(float(dbase), 'f', 2), 4);
	statusDensityLabel->setText(str);

	int l = computeLvlMinusHalfRank(d, m_spaceSize, m_sampler->tiling().subdivFactor());
	int r = computeRank(d, m_spaceSize, l, m_sampler->tiling().subdivFactor());
	str = QString(" | lvl: %1, rank: %2").arg(QString::number(l), 2).arg(QString::number(r), 2);
	statusLevelsLabel->setText(str);
}

void MainWindow::generatePointset()
{
	if( !( optionDFunc->currentIndex() == 7 && m_image.isNull() ) )
	{
		if(optionDFunc->currentIndex()==0)
			m_sampler->generateUniform(m_max_level, m_max_rank, m_lut_index, m_write, m_seed, m_spaceSize, m_crop);
		else
			m_sampler->generateAdaptive(m_DFunc, m_min_density, m_max_density, m_lut_index, m_write, m_seed, m_spaceSize, m_offsetInterpolate, m_dither);
		updatePointset();
	}
}

void MainWindow::updatePointset()
{
	int backIndex = optionDFunc->currentIndex()-1;
	int backMode = optionBackground->currentIndex();
	switch(backMode)
	{
		case 0:
			backIndex = 0;
			break;
		case 1:
			backMode = optionSampleColor->currentIndex();
			break;
		case 2:
			backMode = 0;
			break;
		case 3:
			backMode = 1;
			break;
		case 4:
			backMode = 3;
			break;
		default:
			backIndex = 0;
			backMode = 0;
	}
	m_view->updatePointset(m_write, m_spaceSize, m_min_density, m_max_density, backIndex, backMode );
}
