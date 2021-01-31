#pragma once

//GUI
#include <QtWidgets/QWidget>
#include <QtWidgets/QStylePainter>

//DATA
#include <vector>
#include <deque>

#include <iostream>
#include <sstream>

namespace MinimalGraph {

	//util func
	inline std::string Val_to_str(double pVal) {
		std::ostringstream oss;
		oss << pVal;
		return oss.str();
	};

	//class
	class XAxis
	{
	private:

		//(Val -> Plot�ʒu �����̃N���X�ŏ������邽�߂�...)�v���b�g�e�[�u���̍��W�n��o�^����B
		QRect * mPlotRect;
		int mTicks;
		double mLabelMax;
		double mLabelMin;
	public:
		XAxis(QRect *pPlotRect)
			: mPlotRect(pPlotRect)
		{};
		~XAxis() {};

		void setTicks(int pTicks) { mTicks = pTicks; };
		void setLabelMax(double &pLabelMax) { mLabelMax = pLabelMax; };
		void setLabelMin(double &pLabelMin) { mLabelMin = pLabelMin; };

		int getTicks() { return mTicks; };
		double getLabelMax() { return mLabelMax; };
		double getLabelMin() { return mLabelMin; };
		double getLabelVal(int pTick) {//���x���l��ynumTicks���璼�ڍ��Ȃ��ƃL��������
			double xlabelrate = (mLabelMax - mLabelMin) / mTicks;
			double xlabelVal = mLabelMin + xlabelrate * pTick;
			return xlabelVal;
		};

		double CalcPlotPoint(double &pVal) {
			//null check
			if (!mPlotRect) return 0;
			//����g���Čv�Z�B (vmax - vmin):(max - pval)=(pixmax - pixmin):X[pixel]
			double diffFromMax = pVal - mLabelMin;
			//double plotheight = mPlotRect->bottom() - mPlotRect->top();
			double plotheight = mPlotRect->right() - mPlotRect->left();
			double labeldiff = mLabelMax - mLabelMin;
			double result = diffFromMax * plotheight / labeldiff + mPlotRect->left();
			return result;
		};

	};

	class YAxis
	{
	private:

		//(Val -> Plot�ʒu �����̃N���X�ŏ������邽�߂�...)�v���b�g�e�[�u���̍��W�n��o�^����B
		QRect *mPlotRect;
		int mTicks;
		double mLabelMax;
		double mLabelMin;


	public:
		YAxis(QRect *pPlotRect)
			: mPlotRect(pPlotRect)
		{};
		~YAxis() {};

		void setTicks(int pTicks) { mTicks = pTicks; };
		void setLabelMax(double &pLabelMax) { mLabelMax = pLabelMax; };
		void setLabelMin(double &pLabelMin) { mLabelMin = pLabelMin; };

		int getTicks() { return mTicks; };
		double getLabelVal(int pTick) {//���x���l��ynumTicks���璼�ڍ��Ȃ��ƃL��������
			double ylabelrate = (mLabelMax - mLabelMin) / mTicks;
			double ylabelVal = mLabelMin + ylabelrate * pTick;
			return ylabelVal;
		};

		double CalcPlotPoint(double &pVal) {
			//null check
			if (!mPlotRect) return 0;
			//����g���Čv�Z�B (vmax - vmin):(max - pval)=(pixmax - pixmin):X[pixel]
			double diffFromMax = mLabelMax - pVal;
			double plotheight = mPlotRect->bottom() - mPlotRect->top();
			double labeldiff = mLabelMax - mLabelMin;
			double result = diffFromMax * plotheight / labeldiff + mPlotRect->top();
			return result;
		};

	};

	struct XYPair {
	public:
		double mX;
		double mY;
	};

	struct GraphData {
	public:
		std::string mName;
		QColor mColor;
		//std::vector<XYPair> mPlotXYData;
		std::deque<XYPair> mPlotXYData;
	};

	class Graph : public QWidget
	{
		Q_OBJECT
	private:
	protected:
		QRect mRect;
		QRect mPlotRect;
		
		QPixmap mPixmap;

		//Painter
		QPainter mPainter;
		QPainter mAxisPainter;
		QPainter mPlotPainter;

		QPen mDotLinePen;
		QPen mSolidLinePen;

		XAxis mXaxis;
		YAxis mYaxis;

		//�O���t�̃f�[�^(x,y)
		std::vector<GraphData> mGraphData;

	protected:
		void paintEvent(QPaintEvent *event);
		void resizeEvent(QResizeEvent *event);

	public:
		Graph(QWidget *parent = 0);
		~Graph();

		void init();

		void setXrange(double pXmax, double pXmin);
		void setYrange(double pYmax, double pYmin);

		//Plot(�O���t)�ǉ�
		void addPlot(const std::string &pName, const QColor &pColor);

		//XY�̃f�[�^�o�^
		void addXYData(int pPlotNum, double &pX, double &pY);
		void addXYData(int pPlotNum, double &pX, double &pY, int pSizeLimit);

		void clearPlot();
		void drawGraph();

	};
	/////////////////////////////////////////


}