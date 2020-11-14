#pragma once

//GUI
#include <QtWidgets/QWidget>
#include <QtWidgets/QStylePainter>

//DATA
#include <vector>

namespace MinimalGraph {

	class XAxis
	{
	private:

		//(Val -> Plot位置 をこのクラスで処理するために...)プロットテーブルの座標系を登録する。
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
		double getLabelVal(int pTick) {//ラベル値はynumTicksから直接作らないとキリが悪い
			double xlabelrate = (mLabelMax - mLabelMin) / mTicks;
			double xlabelVal = mLabelMin + xlabelrate * pTick;
			return xlabelVal;
		};

		double CalcPlotPoint(double &pVal) {
			//null check
			if (!mPlotRect) return 0;
			//比を使って計算。 (vmax - vmin):(max - pval)=(pixmax - pixmin):X[pixel]
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

		//(Val -> Plot位置 をこのクラスで処理するために...)プロットテーブルの座標系を登録する。
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
		double getLabelVal(int pTick) {//ラベル値はynumTicksから直接作らないとキリが悪い
			double ylabelrate = (mLabelMax - mLabelMin) / mTicks;
			double ylabelVal = mLabelMin + ylabelrate * pTick;
			return ylabelVal;
		};

		double CalcPlotPoint(double &pVal) {
			//null check
			if (!mPlotRect) return 0;
			//比を使って計算。 (vmax - vmin):(max - pval)=(pixmax - pixmin):X[pixel]
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
		std::vector<XYPair> mPlotXYData;
	};

	class Graph : public QWidget
	{
		Q_OBJECT
	private:
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

		//グラフのデータ(x,y)
		std::vector<GraphData> mGraphData;

	protected:
		void paintEvent(QPaintEvent *event);
	public:
		Graph(QWidget *parent = 0);
		~Graph();

		void init();

		void setXrange(double pXmax, double pXmin);
		void setYrange(double pYmax, double pYmin);

		//Plot(グラフ)追加
		void addPlot(const std::string &pName, const QColor &pColor);

		//XYのデータ登録
		void addXYData(int pPlotNum, double &pX, double &pY);

		void clearPlot();
		void drawGraph();

	};
	/////////////////////////////////////////


}