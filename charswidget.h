#ifndef CHARSWIDGET_H
#define CHARSWIDGET_H

#include "ui_charswidget.h"
#include <QWidget>
#include <QAction>
#include <QColor>
#include <QTextCharFormat>
#include <QModelIndex>
#include <QTextCursor>
#include <QTextListFormat>
#include <QTextBlockFormat>
#include <QSettings>

class TextEdit;
class EditorGraphicsView;

class CharsWidget : public QWidget, public Ui::CharsWidget
{
    Q_OBJECT

public:
	CharsWidget(float);
	~CharsWidget();
	void init_document();
	void insert_image(const QImage&, qreal, qreal);
	void insert_heading(const QString&);
	void insert_measurment(const QString&);
	void writeSettings(QSettings&);
	QAction * preview_a;
	QAction * print_a;
	QAction * export_a;
	QAction * new_a;
	QAction * settings_a;

public slots:
	void set_range(int);
	void set_font_size(double);
	void set_font_family(const QString&);
	void set_editor_font_size(double);
	void set_editor_font_family(const QString&);
	void set_editor_bold(bool);
	void set_editor_italic(bool);
	void set_editor_underline(bool);
	void set_editor_strike(bool);
	void set_editor_superscript(bool);
	void set_editor_subscript(bool);
	void toggle_merging(bool);
	void set_fg_color();
	void set_bg_color();
	void index_selected(QModelIndex);
	void append_acute_accent();
	void append_grave_accent();
	void append_left_double_angle();
	void append_right_double_angle();
	void append_ruble();
	void append_rupee();
	void append_lira();
	void append_euro();
	void append_number();
	void append_degree();
	void append_endash();
	void append_emdash();
	void append_lapki1();
	void append_lapki2();
	void append_tm();
	void reset_colors();
	void text_style(int);
	void text_color();
	void char_format_changed(const QTextCharFormat&);
	void cursor_position_changed();
	void alignment_changed(Qt::Alignment);
	bool maybe_save();
	bool file_export();
	void file_print();
	void new_document();
	void update_editor_settings();
	void set_format(int);
	void show_settings(bool);
	void set_editor_default_font(const QString&);
	void reset_block_settings();
	void insert_table(int, int);
	void insert_table2();
	void set_table_border_style(int);

private slots:
	void zoom_in();
	void zoom_out();
	void zoom_one();

private:
	void write_file(const QString&, const short);
	void update_text_color_icon(const QColor&);
	void update_fgcolor_icon(const QColor&);
	void update_bgcolor_icon(const QColor&);
	void list_format_changed(const QTextCursor&);
	void block_format_changed(const QTextCursor&);
	void table_changed(const QTextCursor&);
	void update_format();
	double     font_size;
	QString    font_family;
	QColor     fg_color;
	QColor     bg_color;
	TextEdit * textEdit;
	QTextListFormat::Style saved_list_style;
	QTextBlockFormat       saved_block_format;
	QTextCharFormat        saved_char_format;
	EditorGraphicsView * graphicsview;
	double view_scale;

protected:
	void readSettings();
};

#endif
