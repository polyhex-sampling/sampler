#ifndef __WINDOW__
#define __WINDOW__

#include <QMainWindow>

#include "densityFunction.hpp"
#include "sampler.hpp"
#include "view.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	private:
		Sampler* m_sampler;
		WriterVector m_write;
		int m_lut_index;
		float (*m_DFunc)(const Point&);
		Image m_image;
		QDir m_image_dir;
		float m_min_density;
		float m_max_density;
		short unsigned int m_seed;
		float m_spaceSize;
		short unsigned int m_min_level;
		short unsigned int m_max_level;
		short unsigned int m_min_rank;
		short unsigned int m_max_rank;
		bool m_offsetInterpolate;
		bool m_dither;
		bool m_crop;
		bool m_gen_auto;

		QSplitter* splitter;

		//Pointset Viewer
		PointSetView* m_view;

		//Option Panel
		QWidget* options;
		QVBoxLayout* optionsLayout;

		//option:Parameter Group
		QGroupBox* optionParameterGroup;
		QFormLayout* optionParameterGroupLayout;
		//option:param: lut
		QComboBox* optionLut;
		//option:param: importance function
		QComboBox* optionDFunc;
		//option:param: image load
		QPushButton* optionImage;
		//option:param: init seed
		QSpinBox* optionSeed;
		//option:param: density
		QWidget* optionDensityGroup;
		QLineEdit* optionDensityMin;
		QLineEdit* optionDensityMax;
		QValidator* optionDensityValidator;
		//option:param: space size
		QLineEdit* optionSpaceSz;
		QValidator* optionSpaceSzValidator;
		//option:param: level
		QWidget* optionLevelGroup;
		QSpinBox* optionLevelMin;
		QSpinBox* optionLevelMax;
		//option:param: rank
		QWidget* optionRankGroup;
		QSpinBox* optionRankMin;
		QSpinBox* optionRankMax;
		//option:param: rank offset interpolation
		QCheckBox* optionOffsetInterpolate;
		//option:param: dither samples on 2 subdivision level
		QCheckBox* optionDither;

		//option:Display Group
		QGroupBox* optionDisplayGroup;
		QFormLayout* optionDisplayGroupLayout;
		//option:display: shape switch
		QComboBox* optionSampleShape;
		QDoubleSpinBox* optionDiskRadius;
		//option:display: color switch
		QComboBox* optionSampleColor;
		//option:display: background switch
		QComboBox* optionBackground;
		//option:display: crop bool
		QCheckBox* optionCrop;

		//option: Generate
		QWidget* generateGroup;
		QPushButton* generateButton;
		QCheckBox* generateAuto;

		//option: Quit button
		QPushButton* quitButton;

		//StatusBar
		QLabel* statusPositionLabel;
		QLabel* statusDensityLabel;
		QLabel* statusLevelsLabel;

		void createOptionPanel();
		void createOptionParameterGroup();
		void createOptionLut();
		void createOptionDFunc();
		void createOptionImage();
		void createOptionDensity();
		void createOptionSeed();
		void createOptionSpaceSz();
		void createOptionLevel();
		void createOptionRank();
		void createOptionOffsetInterpolate();
		void createOptionDither();
		void createOptionDisplayGroup();
		void createOptionSampleShape();
		void createOptionSampleColor();
		void createOptionBackground();
		void createOptionCrop();
		void createOptionGenerate();
		void createOptionQuitButton();
		void createStatusBar();

	public:
		MainWindow(Sampler* sampler);
		~MainWindow();

		void setDensityMin();
		void setDensityMax();
		void setSpaceSz(float spaceSize);
		void setLevelMin();
		void setLevelMax();
		void setRankMin();
		void setRankMax();

	public slots:
		void getLut(int index);
		void getDFunc(int index);
		void getImageFile();
		void getDensityMin();
		void getDensityMax();
		void getSeed(int seed);
		void getSpaceSz();
		void getLevelMin(int level);
		void getLevelMax(int level);
		void getRankMin(int rank);
		void getRankMax(int rank);
		void getOffsetInterpolate(int state);
		void getDither(int state);
		void getSampleShape(int index);
		void getDiskRadius(double rad);
		void getSampleColor(int index);
		void getBackground(int index);
		void getCrop(int state);
		void getAuto(int state);
		void showCoordinates(float x, float y);

		void generatePointset();
		void updatePointset();

};

#endif
