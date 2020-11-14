#include "stdafx.h"
#include "minimalgraph.h"
#include "moc_minimalgraph.h"

#include <QtWidgets/QStylePainter>
#include <QtWidgets/QStyleOptionFocusRect>

#include <iostream>
#include <sstream>

constexpr int yAxisMargin = 30;//‚±‚±‚ªyŽ²‚Æ‚ÌŠÔ

std::string Val_to_str(double pVal) {
	std::ostringstream oss;
	oss << pVal;
	return oss.str();
}

MinimalGraph::Graph::Graph(QWidget *parent) : QWidget(parent), mXaxis(&mPlotRect), mYaxis(&mPlotRect)
{
	setBackgroundRole(QPalette::Dark);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MinimalGraph::Graph::~Graph()
{}

void MinimalGraph::Graph::paintEvent(QPaintEvent *event) {
	QStylePainter painter(this);
	painter.drawPixmap(0, 0, mPixmap);
	if (hasFocus()) {//“ü‚ç‚È‚¢H•s–¾
		QStyleOptionFocusRect option;
		option.initFrom(this);
		option.backgroundColor = palette().light().color();
		//option.backgroundColor = palette().dark().color();
		painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
	}
}

void MinimalGraph::Graph::init() {
	//pixmap init
	mPixmap = QPixmap(size());
	mPixmap.fill(Qt::white);

	//painter init
	mPainter.begin(&mPixmap);
	mPainter.initFrom(this);
	mPainter.setRenderHint(QPainter::HighQualityAntialiasing, true);
	mPainter.end();

	//pen init
	mDotLinePen.setColor(Qt::black);
	mDotLinePen.setStyle(Qt::DotLine);

	mSolidLinePen.setColor(Qt::black);
	mSolidLinePen.setStyle(Qt::SolidLine);

	//rect init
	QRect allrect(0, 0, width(), height());
	mRect = allrect;

	int Margin = 10;
	int TickMarksWidth = 5;
	QFontMetrics fm(font());
	QRect rect(yAxisMargin + Margin,
		Margin,
		width() - (yAxisMargin + (2 * Margin)),
		height() - (TickMarksWidth + (fm.height()) + (2 * Margin)));

	mPlotRect = rect;

	if (!mRect.isValid()) {
		throw std::runtime_error("some error message");
	}
	if (!mPlotRect.isValid()) {
		throw std::runtime_error("some error message");
	}

	//axis
	int xnumTicks = 5;
	double xlabelMAX = 3.1415 * 2;
	double xlabelMIN = 0;
	mXaxis.setTicks(xnumTicks);
	mXaxis.setLabelMax(xlabelMAX);
	mXaxis.setLabelMin(xlabelMIN);

	int ynumTicks = 4;
	double ylabelMAX = 1.0;
	double ylabelMIN = -1.0;
	mYaxis.setTicks(ynumTicks);
	mYaxis.setLabelMax(ylabelMAX);
	mYaxis.setLabelMin(ylabelMIN);


	////˜g‚Ì•`ŽÊ¨‚Ç‚¤‚¹–ˆ‰ñÁ‚¦‚é‚Ì‚Å•`‰æ‚Ì‚Ù‚¤‚ÅB
	//mPainter.setPen(mSolidLinePen);
	//mPainter.drawLine(mPlotRect.left(), mPlotRect.top(), mPlotRect.left(), mPlotRect.bottom());
	//mPainter.drawRect(mPlotRect.adjusted(0, 0, -1, -1));


}

void MinimalGraph::Graph::setXrange(double pXmax, double pXmin) {
	mXaxis.setLabelMax(pXmax);
	mXaxis.setLabelMin(pXmin);
}

void MinimalGraph::Graph::setYrange(double pYmax, double pYmin) {
	mYaxis.setLabelMax(pYmax);
	mYaxis.setLabelMin(pYmin);
}

void MinimalGraph::Graph::addPlot(const std::string &pName, const QColor &pColor) {
	GraphData graph;
	graph.mName = pName;
	graph.mColor = pColor;
	mGraphData.push_back(graph);
}

void MinimalGraph::Graph::clearPlot() {
	mPixmap.fill(Qt::white);
}

void MinimalGraph::Graph::addXYData(int pPlotNum, double &pX, double &pY) {
	XYPair data;
	data.mX = pX;
	data.mY = pY;
	mGraphData.at(pPlotNum).mPlotXYData.push_back(data);
}

void MinimalGraph::Graph::drawGraph() {

	mPainter.begin(&mPixmap);

	QFontMetrics fm(font());
	
	//˜g‚Ì•`ŽÊ
	mPainter.setPen(mSolidLinePen);
	mPainter.drawLine(mPlotRect.left(), mPlotRect.top(), mPlotRect.left(), mPlotRect.bottom());
	mPainter.drawRect(mPlotRect.adjusted(0, 0, -1, -1));

	mPainter.end();

	mAxisPainter.begin(&mPixmap);

	//XŽ²‚ÌŽ²ü•`ŽÊ
	for (int i = 0; i <= mXaxis.getTicks(); ++i) {
		int x = mPlotRect.left() + (i * (mPlotRect.width() - 1) / mXaxis.getTicks());
		double labelVal = mXaxis.getLabelVal(i);
		QString xlabelstr;
		xlabelstr = QString(Val_to_str(labelVal).c_str());
		int textWidth = fm.width(xlabelstr);
		int textXPoint = x - textWidth / 2;

		// X Ticks			
		mAxisPainter.setPen(mDotLinePen);
		mAxisPainter.drawLine(x, mPlotRect.top(), x, mPlotRect.bottom());

		// X Label
		mAxisPainter.drawText(textXPoint,
			mPlotRect.bottom() + 10,
			textWidth,
			fm.height(),
			Qt::AlignTop,
			xlabelstr);

		//X’·‚¢•â•Ž²
		mAxisPainter.setPen(mSolidLinePen);
		mAxisPainter.drawLine(x, mPlotRect.top(), x, mPlotRect.top() + 10);//ã‚Ìü
		mAxisPainter.drawLine(x, mPlotRect.bottom() - 10, x, mPlotRect.bottom());//‰º‚Ìü
	}

	//X•â•–Ú·ü•`ŽÊ
	//X subsidiary scale mark
	//ƒXƒ^ƒCƒ‹Žw’è
	mAxisPainter.setPen(mSolidLinePen);
	int subXnumTicks = 20;
	for (int i = 0; i <= subXnumTicks; ++i) {
		int x = mPlotRect.left() + (i * (mPlotRect.width() - 1) / subXnumTicks);
		double labelVal = x;
		mAxisPainter.drawLine(x, mPlotRect.top(), x, mPlotRect.top() + 5);//ã‚Ìü
		mAxisPainter.drawLine(x, mPlotRect.bottom() - 5, x, mPlotRect.bottom());//‰º‚Ìü
	}

	/////////////////////////////

	//YŽ²‚ÌŽ²ü•`ŽÊ
	for (int j = 0; j <= mYaxis.getTicks(); ++j) {
		int y = mPlotRect.bottom() - (j * (mPlotRect.height() - 1) / mYaxis.getTicks());

		double ylabelVal = mYaxis.getLabelVal(j);
		QString ylabelstr;
		ylabelstr = QString(Val_to_str(ylabelVal).c_str());
		int YtextWidth = fm.width(ylabelstr);
		int textYPoint = y - fm.height() / 2;

		//Y Ticks
		mAxisPainter.setPen(mDotLinePen);
		mAxisPainter.drawLine(mPlotRect.left(), y, mPlotRect.right(), y);

		//Y Label
		mAxisPainter.drawText(mPlotRect.left() - yAxisMargin,
			textYPoint,
			YtextWidth,
			fm.height(),
			Qt::AlignRight,
			ylabelstr);

		//Y’·‚¢•â•Ž²
		mAxisPainter.setPen(mSolidLinePen);
		mAxisPainter.drawLine(mPlotRect.left(), y, mPlotRect.left() + 10, y);//¶
		mAxisPainter.drawLine(mPlotRect.right() - 10, y, mPlotRect.right(), y);//‰E

	}

	//Y•â•–Ú·ü
	//Y subsidiary scale mark
	//ƒXƒ^ƒCƒ‹Žw’è
	mAxisPainter.setPen(mSolidLinePen);
	int subYnumTicks = 20;
	for (int j = 0; j <= subYnumTicks; ++j) {
		int y = mPlotRect.bottom() - (j * (mPlotRect.height() - 1) / subYnumTicks);
		mAxisPainter.drawLine(mPlotRect.left(), y, mPlotRect.left() + 5, y);//¶
		mAxisPainter.drawLine(mPlotRect.right() - 5, y, mPlotRect.right(), y);//‰E
	}

	/////////////////////////////

	mAxisPainter.end();

	mPlotPainter.begin(&mPixmap);

	//ƒOƒ‰ƒt•`ŽÊ
	std::for_each(mGraphData.begin(), mGraphData.end(), [&](GraphData graphdata) {
		
		//xy to pixel convertion
		QPolygonF polygonF;
		std::for_each(graphdata.mPlotXYData.begin(), graphdata.mPlotXYData.end(), [&](XYPair xypair) {
			double xplotval = mXaxis.CalcPlotPoint(xypair.mX);
			double yplotval = mYaxis.CalcPlotPoint(xypair.mY);
			polygonF << QPointF(xplotval, yplotval);
		});

		//draw start
		QPen pen(graphdata.mColor, 1);
		mPlotPainter.setPen(pen);
		mPlotPainter.setClipRect(mPlotRect);
		mPlotPainter.drawPolyline(polygonF);

	});
	
	mPlotPainter.end();
	
	update();

}
