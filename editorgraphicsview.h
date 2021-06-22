#ifndef EDITORGRAPHICSVIEW_H
#define EDITORGRAPHICSVIEW_H

#include <QtGlobal>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QSizeF>

class TextEdit;

class EditorGraphicsView : public QGraphicsView
{
Q_OBJECT
public:
	EditorGraphicsView();
	~EditorGraphicsView();
	void set_textedit(TextEdit*);
	void set_view_scale(double);
	void update_geometry(const QSizeF&);

protected:
	void resizeEvent(QResizeEvent*) override;
	void wheelEvent(QWheelEvent*) override;

private:
	QGraphicsScene * scene;
	QGraphicsProxyWidget * item;
	TextEdit * text;
	double view_scale;
};

#endif
