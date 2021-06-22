#ifndef TEXTEDIT_QT4__H
#define TEXTEDIT_QT4__H

#include <QTextEdit>
#include <QUrl>
#include <QMimeData>
#include <QImage>
#include <QPrinter>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextFrameFormat>
#include <QApplication>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QContextMenuEvent>
#include <QSettings>

class TextEdit : public QTextEdit
{
Q_OBJECT
public:
	TextEdit();
	~TextEdit();
	void new_document();
	void append_text(const QString &);
	static qreal mm2px(qreal, bool, bool=true);
	static qreal px2mm(qreal, bool, bool=true);
	static QPrinter::PageSize pagesize_from_qstring(const QString&);
	static QString qstring_from_pagesize(const QPrinter::PageSize);
    bool canInsertFromMimeData(const QMimeData*) const override;
    void insertFromMimeData(const QMimeData*) override;
	void insertImage(const QImage&);
	void set_table_border_style(QTextFrameFormat::BorderStyle);
	void writeSettings(QSettings&);
	QPrinter::PaperSize m_paper_format;
	QPrinter::Orientation m_paper_orientaion;
	qreal m_paper_width_px;
	qreal m_paper_height_px;
	qreal m_page_width_px;
	qreal m_page_height_px;
	qreal m_page_x_px;
	qreal m_page_y_px;
	qreal m_page_r_px;
	qreal m_page_b_px;
	qreal m_margins_px_left;
	qreal m_margins_px_top;
	qreal m_margins_px_right;
	qreal m_margins_px_bottom;
	qreal m_margins_mm_left;
	qreal m_margins_mm_top;
	qreal m_margins_mm_right;
	qreal m_margins_mm_bottom;
	QString m_font_family;
	qreal   m_font_size;
	bool    m_design_metrics;

public slots:
	void text_alignleft(bool);
	void text_aligncenter(bool);
	void text_alignright(bool);
	void text_alignjustify(bool);
	void set_paper_format(QPrinter::PaperSize);
	void merge_format(const QTextCharFormat&);
	void update_settings();
	void preview_print(QPrinter*);
	void trigger_print_preview();
	void print(QPrinter*);
	void update_left_margin(int);
	void update_top_margin(int);
	void update_right_margin(int);
	void update_bottom_margin(int);
	void set_default_font(const QString &);
	void set_default_font_pt(double);
	void set_indent(int);
	void set_text_indent(double);
	void set_block_left_margin(double);
	void set_block_top_margin(double);
	void set_block_right_margin(double);
	void set_block_bottom_margin(double);
	void set_block_line_height(int);
	void reset_block();
	void insert_table(int,int);
	void set_design_metrics(bool);
	void set_paper_size(const QString &);
	void set_paper_orientation(const QString &);

private:
	void dropImage(const QUrl&, const QImage&);
	void dropTextFile(const QUrl&);

protected:
	void paintEvent(QPaintEvent*) override;
	void resizeEvent(QResizeEvent*) override;
	void contextMenuEvent(QContextMenuEvent*) override;
	void readSettings();
};

#endif
