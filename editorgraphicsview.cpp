#include <QGraphicsScene>
#include <QTransform>
#include <QScrollBar>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include "textedit-qt6.h"
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include "textedit-qt5.h"
#else
#include "textedit-qt4.h"
#endif 
#include "editorgraphicsview.h"

EditorGraphicsView::EditorGraphicsView()
{
	item = NULL;
	text = NULL;
	view_scale = 1.0;
	scene = new QGraphicsScene();
	setScene(scene);
	setBackgroundBrush(QBrush(qApp->palette().color(QPalette::Window)));
	setCacheMode(CacheNone);
	setViewportUpdateMode(FullViewportUpdate);
	setTransformationAnchor(NoAnchor);
	setDragMode(NoDrag);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	horizontalScrollBar()->setValue(0);
	verticalScrollBar()->setValue(0);
}

EditorGraphicsView::~EditorGraphicsView()
{
}

void EditorGraphicsView::set_textedit(TextEdit * t)
{
	text = t;
	item = scene->addWidget(t);
	set_view_scale(view_scale);
	update_geometry(QSizeF(width()/view_scale, height()/view_scale));
}

void EditorGraphicsView::set_view_scale(double s)
{
	view_scale = s;
	QTransform t = QTransform();
	t = t.scale(view_scale, view_scale);
	this->setTransform(t);
}

void EditorGraphicsView::update_geometry(const QSizeF & s)
{
	if (item)
	{
		item->setPos(0,0);
		item->setGeometry(QRect(0, 0, s.width(), s.height()));
	}
	horizontalScrollBar()->setValue(0);
	verticalScrollBar()->setValue(0);
}

void EditorGraphicsView::resizeEvent(QResizeEvent * e)
{
	QGraphicsView::resizeEvent(e);
	update_geometry(QSizeF(width()/view_scale, height()/view_scale));
}

void EditorGraphicsView::wheelEvent(QWheelEvent * e)
{
	QGraphicsView::wheelEvent(e);
	horizontalScrollBar()->setValue(0);
	verticalScrollBar()->setValue(0);
	e->ignore();
}

