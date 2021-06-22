#define USE_DOC_SCENE
#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include "textedit-qt6.h"
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include "textedit-qt5.h"
#else
#include "textedit-qt4.h"
#endif
#ifdef USE_DOC_SCENE
#include "editorgraphicsview.h"
#endif
#include "charswidget.h"
#include "charsmodel.h"
#include "ranges.h"
#include <QToolBar>
#include <QList>
#include <QVariant>
#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QFontInfo>
#include <QColorDialog>
#include <QPalette>
#include <QTextList>
#include <QTextTable>
#include <QTextDocumentWriter>
#include <QFileDialog>
#include <QByteArray>
#include <QDir>
#include <QMessageBox>
#include <QButtonGroup>
#include <QPrinter>
#include <QPrintDialog>
#include <QAbstractPrintDialog>
#include <QAbstractTextDocumentLayout>
#include "matrixbutton.h"
#include "tabledialog.h"
#if 1
#include <iostream>
#endif

CharsWidget::CharsWidget(float si)
{
	setupUi(this);
	view_scale = 1.0;
	const QSize s = QSize((int)(18*si),(int)(18*si));
	format_comboBox->setIconSize(s);
	e_b_toolButton->setIconSize(s);
	e_i_toolButton->setIconSize(s);
	e_underline_toolButton->setIconSize(s);
	e_strike_toolButton->setIconSize(s);
	superscript_toolButton->setIconSize(s);
	subscript_toolButton->setIconSize(s);
	color_toolButton->setIconSize(s);
	alignleft_toolButton->setIconSize(s);
	aligncenter_toolButton->setIconSize(s);
	alignright_toolButton->setIconSize(s);
	alignjustify_toolButton->setIconSize(s);
	acute_accent_toolButton->setIconSize(s);
	grave_accent_toolButton->setIconSize(s);
	lkav_toolButton->setIconSize(s);
	rkav_toolButton->setIconSize(s);
	lapki1_toolButton->setIconSize(s);
	lapki2_toolButton->setIconSize(s);
	number_toolButton->setIconSize(s);
	deg_toolButton->setIconSize(s);
	endash_toolButton->setIconSize(s);
	emdash_toolButton->setIconSize(s);
	tm_toolButton->setIconSize(s);
	c_toolButton->setIconSize(s);
	r_toolButton->setIconSize(s);
	rupee_toolButton->setIconSize(s);
	euro_toolButton->setIconSize(s);
	rub_toolButton->setIconSize(s);
	lira_toolButton->setIconSize(s);
	reset_block_toolButton->setIconSize(s);
	fg_toolButton->setIconSize(s);
	bg_toolButton->setIconSize(s);
	reset_toolButton->setIconSize(s);
#if 1
	rub_toolButton->hide();
	rupee_toolButton->hide();
	lira_toolButton->hide();
	euro_toolButton->hide();
#endif
	table_frame->setEnabled(false);
#ifdef USE_DOC_SCENE
	textEdit = new TextEdit();
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
#else
	textEdit = new TextEdit();
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
#endif
	MatrixButton * mb = new MatrixButton(si);
	QToolBar * toolbar = new QToolBar(toolbar_frame);
	preview_a = new QAction(QIcon(":/bitmaps/eye.svg"),
		QString("Preview"), toolbar);
	preview_a->setCheckable(false);
	print_a = new QAction(QIcon(":/bitmaps/printer.svg"),
		QString("Print"), toolbar);
	print_a->setCheckable(false);
	export_a = new QAction(QIcon(":/bitmaps/save2.svg"),
		QString("Export"), toolbar);
	export_a->setCheckable(false);
	new_a = new QAction(QIcon(":/bitmaps/delete2.svg"),
		QString("New document"), toolbar);
	new_a->setCheckable(false);
	settings_a = new QAction(QIcon(":/bitmaps/tool.svg"),
		QString("Settings"), toolbar);
	settings_a->setCheckable(true);
	settings_a->setChecked(true);
	{
		QPalette p1 = QApplication::palette();
		p1.setColor(QPalette::Text, Qt::black);
		p1.setColor(QPalette::Base, Qt::white);
		textEdit->setPalette(p1);
	}
#ifdef USE_DOC_SCENE
	graphicsview = new EditorGraphicsView();
	graphicsview->set_textedit(textEdit);
	//
	QVBoxLayout * vl0 = new QVBoxLayout(text_frame);
	vl0->addWidget(graphicsview);
	vl0->setSpacing(0);
	vl0->setContentsMargins(0,0,0,0);
#else
	graphicsview = NULL;
	QVBoxLayout * vl0 = new QVBoxLayout(text_frame);
	vl0->setSpacing(0);
	vl0->setContentsMargins(0, 0, 0, 0);
	vl0->addWidget(textEdit);
#endif
	QVBoxLayout * l1 = new QVBoxLayout(matrix_frame);
	l1->setContentsMargins(0,0,0,0);
	l1->setSpacing(0);
	l1->addWidget(mb);
	toolbar->setIconSize(QSize((int)(18*si),(int)(18*si)));
	toolbar->setOrientation(Qt::Horizontal);
	toolbar->setFloatable(false);
	toolbar->setMovable(false);
	if (toolbar->layout())
	{
		toolbar->layout()->setContentsMargins(0, 0, 0, 0);
		toolbar->layout()->setSpacing(6);
	}
	QVBoxLayout * l = new QVBoxLayout(toolbar_frame);
	l->setContentsMargins(0,0,0,0);
	l->setSpacing(0);
	l->addWidget(toolbar);
	metrics_checkBox->hide();
	r_toolButton->hide();
	c_toolButton->hide();
	border_style_comboBox->addItem(QString(""));
	border_style_comboBox->addItem(QString("None"));
	border_style_comboBox->addItem(QString("Dotted"));
	border_style_comboBox->addItem(QString("Dashed"));
	border_style_comboBox->addItem(QString("Solid"));
	border_style_comboBox->addItem(QString("Double"));
	border_style_comboBox->addItem(QString("DotDash"));
	border_style_comboBox->addItem(QString("DotDotDash"));
	border_style_comboBox->addItem(QString("Groove"));
	border_style_comboBox->addItem(QString("Ridge"));
	border_style_comboBox->addItem(QString("Inset"));
	border_style_comboBox->addItem(QString("Outset"));
	QWidget * spacer2 = new QWidget(this);
	spacer2->setSizePolicy(
		QSizePolicy::Expanding,QSizePolicy::Preferred);
	toolbar->addWidget(spacer2);
	toolbar->addAction(preview_a);
	toolbar->addAction(print_a);
	toolbar->addAction(export_a);
	toolbar->addAction(new_a);
	toolbar->addAction(settings_a);
	saved_list_style   = static_cast<QTextListFormat::Style>(0);
	saved_block_format = QTextBlockFormat();
	saved_char_format  = QTextCharFormat();
	{
		QSettings settings(
			QSettings::IniFormat,
			QSettings::UserScope,
			QApplication::organizationName(),
			QApplication::applicationName());
		settings.setFallbacksEnabled(true);
		settings.beginGroup(QLatin1String("TextEdit"));
		const QString font_family = settings.value(QString("font_family"),
#if defined _WIN32
			QString("Calibri")
#elif defined __APPLE__
			QString("Helvetica")
#else
			QString("sans")
#endif
			).toString();
		const double font_size = settings.value(QString("font_size"), 12.0).toDouble();
		const QString ps = settings.value(QString("page_size"), QString("A4")).toString();
		const QString po = settings.value(QString("page_orient"), QString("Portrait")).toString();
		const int m_mm_l = static_cast<int>(settings.value(QString("margins_mm_left"),  15.0).toDouble());
		const int m_mm_t = static_cast<int>(settings.value(QString("margins_mm_top"),   15.0).toDouble());
		const int m_mm_r = static_cast<int>(settings.value(QString("margins_mm_right"), 10.0).toDouble());
		const int m_mm_b = static_cast<int>(settings.value(QString("margins_mm_bottom"),15.0).toDouble());
		const int metrics = settings.value(QString("design_metrics"), 1).toInt();
		settings.endGroup();
		ff_lineEdit->setText(font_family);
		fs_doubleSpinBox->setValue(font_size);
		const int idx0 = paper_comboBox->findText(ps);
		paper_comboBox->setCurrentIndex(idx0);
		const int idx1 = orientation_comboBox->findText(po);
		orientation_comboBox->setCurrentIndex(idx1);
		margin_spinBox->setValue(m_mm_l);
		top_spinBox->setValue(m_mm_t);
		right_spinBox->setValue(m_mm_r);
		bottom_spinBox->setValue(m_mm_b);
		if (metrics == 1) metrics_checkBox->setChecked(true);
		else metrics_checkBox->setChecked(false);
	}
	for (unsigned int x = 0; x < ranges_size; ++x)
	{
		QList<QVariant> l;
		l.push_back(QVariant(ranges[x].from));
		l.push_back(QVariant(ranges[x].to));
		comboBox->addItem(ranges[x].name, QVariant(l));
	}
	{
		QPixmap pp(24, 24);
		pp.fill(Qt::black);
		color_toolButton->setIcon(pp);
	}
	style_comboBox->addItem("Standard");
	style_comboBox->addItem("Bullet List  (Disc)");
	style_comboBox->addItem("Bullet List  (Circle)");
	style_comboBox->addItem("Bullet List  (Square)");
	style_comboBox->addItem("Ordered List (Decimal)");
	style_comboBox->addItem("Ordered List (Alpha lower)");
	style_comboBox->addItem("Ordered List (Alpha upper)");
	style_comboBox->addItem("Ordered List (Roman lower)");
	style_comboBox->addItem("Ordered List (Roman upper)");
	if (QApplication::isLeftToRight())
		alignleft_toolButton->setChecked(true);
	else
		alignright_toolButton->setChecked(true);
	QButtonGroup * grp = new QButtonGroup(this);
	grp->addButton(alignleft_toolButton);
	grp->addButton(aligncenter_toolButton);
	grp->addButton(alignright_toolButton);
	grp->addButton(alignjustify_toolButton);
	grp->setExclusive(true);
	//
	//
	readSettings();
	//
	//
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
	connect(
		fontComboBox, SIGNAL(currentTextChanged(const QString&)),
		this, SLOT(set_font_family(const QString&)));
	connect(
		e_fontComboBox, SIGNAL(currentTextChanged(const QString&)),
		this, SLOT(set_editor_font_family(const QString&)));
#else
	connect(
		fontComboBox, SIGNAL(currentIndexChanged(const QString&)),
		this, SLOT(set_font_family(const QString&)));
	connect(
		e_fontComboBox, SIGNAL(currentIndexChanged(const QString&)),
		this, SLOT(set_editor_font_family(const QString&)));
#endif
	connect(
		comboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(set_range(int)));
	connect(
		doubleSpinBox, SIGNAL(valueChanged(double)),
		this, SLOT(set_font_size(double)));
	connect(
		e_doubleSpinBox, SIGNAL(valueChanged(double)),
		this, SLOT(set_editor_font_size(double)));
	connect(
		merge_checkBox, SIGNAL(toggled(bool)),
		this, SLOT(toggle_merging(bool)));
	connect(
		e_b_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_bold(bool)));
	connect(
		e_i_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_italic(bool)));
	connect(
		e_underline_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_underline(bool)));
	connect(
		e_strike_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_strike(bool)));
	connect(
		superscript_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_superscript(bool)));
	connect(
		subscript_toolButton, SIGNAL(toggled(bool)),
		this, SLOT(set_editor_subscript(bool)));
	connect(
		fg_toolButton, SIGNAL(clicked()),
		this, SLOT(set_fg_color()));
	connect(
		bg_toolButton, SIGNAL(clicked()),
		this, SLOT(set_bg_color()));
	connect(
		reset_toolButton, SIGNAL(clicked()),
		this, SLOT(reset_colors()));
	connect(
		tableView, SIGNAL(pressed(QModelIndex)),
		this, SLOT(index_selected(QModelIndex)));
	connect(
		acute_accent_toolButton, SIGNAL(clicked()),
		this, SLOT(append_acute_accent()));
	connect(
		grave_accent_toolButton, SIGNAL(clicked()),
		this, SLOT(append_grave_accent()));
	connect(
		rkav_toolButton, SIGNAL(clicked()),
		this, SLOT(append_right_double_angle()));
	connect(
		lkav_toolButton, SIGNAL(clicked()),
		this, SLOT(append_left_double_angle()));
	connect(
		rub_toolButton, SIGNAL(clicked()),
		this, SLOT(append_ruble()));
	connect(
		rupee_toolButton, SIGNAL(clicked()),
		this, SLOT(append_rupee()));
	connect(
		lira_toolButton, SIGNAL(clicked()),
		this, SLOT(append_lira()));
	connect(
		euro_toolButton, SIGNAL(clicked()),
		this, SLOT(append_euro()));
	connect(
		number_toolButton, SIGNAL(clicked()),
		this, SLOT(append_number()));
	connect(
		deg_toolButton, SIGNAL(clicked()),
		this, SLOT(append_degree()));
	connect(
		endash_toolButton, SIGNAL(clicked()),
		this, SLOT(append_endash()));
	connect(
		emdash_toolButton, SIGNAL(clicked()),
		this, SLOT(append_emdash()));
	connect(
		lapki1_toolButton, SIGNAL(clicked()),
		this, SLOT(append_lapki1()));
	connect(
		lapki2_toolButton, SIGNAL(clicked()),
		this, SLOT(append_lapki2()));
	connect(
		tm_toolButton, SIGNAL(clicked()),
		this, SLOT(append_tm()));
	connect(
		style_comboBox, SIGNAL(activated(int)),
		this, SLOT(text_style(int)));
	connect(
		color_toolButton, SIGNAL(clicked()),
		this, SLOT(text_color()));
	connect(
		textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
		this, SLOT(char_format_changed(QTextCharFormat)));
	connect(
		textEdit, SIGNAL(cursorPositionChanged()),
		this, SLOT(cursor_position_changed()));
	connect(
		alignleft_toolButton, SIGNAL(toggled(bool)),
		textEdit, SLOT(text_alignleft(bool)));
	connect(
		aligncenter_toolButton, SIGNAL(toggled(bool)),
		textEdit, SLOT(text_aligncenter(bool)));
	connect(
		alignright_toolButton, SIGNAL(toggled(bool)),
		textEdit, SLOT(text_alignright(bool)));
	connect(
		alignjustify_toolButton, SIGNAL(toggled(bool)),
		textEdit, SLOT(text_alignjustify(bool)));
	connect(
		export_a, SIGNAL(triggered()),
		this, SLOT(file_export()));
	connect(
		preview_a, SIGNAL(triggered()),
		textEdit, SLOT(trigger_print_preview()));
	connect(
		print_a, SIGNAL(triggered()),
		this, SLOT(file_print()));
	connect(
		new_a, SIGNAL(triggered()),
		this, SLOT(new_document()));
	connect(
		settings_a, SIGNAL(triggered(bool)),
		this, SLOT(show_settings(bool)));
	connect(
		margin_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(update_left_margin(int)));
	connect(
		top_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(update_top_margin(int)));
	connect(
		right_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(update_right_margin(int)));
	connect(
		bottom_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(update_bottom_margin(int)));
	connect(
		format_comboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(set_format(int)));
	connect(
		indent_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(set_indent(int)));
	connect(
		text_indent_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_text_indent(double)));
	connect(
		p_lm_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_block_left_margin(double)));
	connect(
		p_tm_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_block_top_margin(double)));
	connect(
		p_rm_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_block_right_margin(double)));
	connect(
		p_bm_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_block_bottom_margin(double)));
	connect(
		p_line_h_spinBox, SIGNAL(valueChanged(int)),
		textEdit, SLOT(set_block_line_height(int)));
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
	connect(
		paper_comboBox, SIGNAL(currentTextChanged(const QString&)),
		textEdit, SLOT(set_paper_size(const QString&)));
	connect(
		orientation_comboBox, SIGNAL(currentTextChanged(const QString&)),
		textEdit, SLOT(set_paper_orientation(const QString&)));
#else
	connect(
		paper_comboBox, SIGNAL(currentIndexChanged(const QString&)),
		textEdit, SLOT(set_paper_size(const QString&)));
	connect(
		orientation_comboBox, SIGNAL(currentIndexChanged(const QString&)),
		textEdit, SLOT(set_paper_orientation(const QString&)));
#endif
	connect(
		metrics_checkBox, SIGNAL(toggled(bool)),
		textEdit, SLOT(set_design_metrics(bool)));
	connect(
		fs_doubleSpinBox, SIGNAL(valueChanged(double)),
		textEdit, SLOT(set_default_font_pt(double)));
	connect(
		ff_lineEdit, SIGNAL(textEdited(const QString&)),
		this, SLOT(set_editor_default_font(const QString&)));
	connect(
		reset_block_toolButton, SIGNAL(clicked()),
		this, SLOT(reset_block_settings()));
	connect(
		mb, SIGNAL(matrix_selected(int, int)),
		this, SLOT(insert_table(int, int)));
	connect(
		mb->p_action, SIGNAL(triggered()),
		this, SLOT(insert_table2()));
	connect(
		border_style_comboBox, SIGNAL(currentIndexChanged(int)),
		this, SLOT(set_table_border_style(int)));
#ifdef USE_DOC_SCENE
	connect(
		zoomin_toolButton, SIGNAL(clicked()),
		this, SLOT(zoom_in()));
	connect(
		zoomout_toolButton, SIGNAL(clicked()),
		this, SLOT(zoom_out()));
	connect(
		zoomone_toolButton, SIGNAL(clicked()),
		this, SLOT(zoom_one()));
#else
	zoomin_toolButton->hide();
	zoomout_toolButton->hide();
	zoomone_toolButton->hide();
	zoom_label->hide();
#endif
}

CharsWidget::~CharsWidget()
{
}

void CharsWidget::closeEvent(QCloseEvent * e)
{
#if 0
	CharsModel * x = static_cast<CharsModel*>(tableView->model());
	if (x)
	{
		delete x;
		x = NULL;
	}
#endif
	e->accept();
}

void CharsWidget::init_document()
{
	textEdit->new_document();
#ifdef USE_DOC_SCENE
	graphicsview->resize(text_frame->width(),text_frame->height());
	graphicsview->update_geometry(QSizeF(graphicsview->width()/view_scale,graphicsview->height()/view_scale));
	graphicsview->updateGeometry();
	qApp->processEvents();
#endif
}

void CharsWidget::insert_image(const QImage & i, qreal vs, qreal hs)
{
	if (vs == 0 && hs == 0)
	{
		textEdit->insertImage(i);
	}
	else
	{
		QRect rect(0, 0, i.width() - vs, i.height() - hs);
		textEdit->insertImage(i.copy(rect));
	}
}

void CharsWidget::insert_heading(const QString & s)
{
}

void CharsWidget::insert_measurment(const QString & s)
{
}

void CharsWidget::writeSettings(QSettings & settings)
{
	if (textEdit) textEdit->writeSettings(settings);
	const int fg_r = fg_color.red();
	const int fg_g = fg_color.green();
	const int fg_b = fg_color.blue();
	const int bg_r = bg_color.red();
	const int bg_g = bg_color.green();
	const int bg_b = bg_color.blue();
	settings.beginGroup(QLatin1String("CharsWidget"));
	settings.setValue(QLatin1String("font_size"), QVariant(font_size));
	settings.setValue(QLatin1String("font_family"), QVariant(font_family));
	settings.setValue(QLatin1String("fg_r"), QVariant(fg_r));
	settings.setValue(QLatin1String("fg_g"), QVariant(fg_g));
	settings.setValue(QLatin1String("fg_b"), QVariant(fg_b));
	settings.setValue(QLatin1String("bg_r"), QVariant(bg_r));
	settings.setValue(QLatin1String("bg_g"), QVariant(bg_g));
	settings.setValue(QLatin1String("bg_b"), QVariant(bg_b));
	settings.setValue(
		QLatin1String("idx"), QVariant(comboBox->currentIndex()));
	settings.endGroup();
}

void CharsWidget::readSettings()
{
	const QColor fg = QApplication::palette().color(QPalette::Text);
	const QColor bg = QApplication::palette().color(QPalette::Base);
	int fg_r = fg.red();
	int fg_g = fg.green();
	int fg_b = fg.blue();
	int bg_r = bg.red();
	int bg_g = bg.green();
	int bg_b = bg.blue();
	QSettings settings(
		QSettings::IniFormat,
		QSettings::UserScope,
		QApplication::organizationName(),
		QApplication::applicationName());
	settings.setFallbacksEnabled(true);				
	settings.beginGroup(QLatin1String("CharsWidget"));
	font_size = settings.value(QLatin1String("font_size"), 24).toDouble();
	font_family = settings.value(
		QLatin1String("font_family"), QString("")).toString();
	fg_r = settings.value(QLatin1String("fg_r"), fg_r).toInt();
	fg_g = settings.value(QLatin1String("fg_g"), fg_g).toInt();
	fg_b = settings.value(QLatin1String("fg_b"), fg_b).toInt();
	bg_r = settings.value(QLatin1String("bg_r"), bg_r).toInt();
	bg_g = settings.value(QLatin1String("bg_g"), bg_g).toInt();
	bg_b = settings.value(QLatin1String("bg_b"), bg_b).toInt();
	int idx = settings.value(QLatin1String("idx"), -1).toInt();
	settings.endGroup();
	doubleSpinBox->setValue(font_size);
	if (font_family.isEmpty())
	{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		const QStringList f = QFontDatabase::families();
#else
		QFontDatabase database;
		const QStringList f = database.families();
#endif
		if (f.contains(QString("Symbola")))
		{
			font_family = QString("Symbola");
			if (idx == -1) idx = 280;
		}
		else
		{
			QFont font = QApplication::font();
			font_family = font.family();
		}
	}
	if (idx == -1) idx = 1;
	comboBox->setCurrentIndex(idx);
	const int fidx = fontComboBox->findText(font_family);
	fontComboBox->setCurrentIndex(fidx);
	fg_color = QColor(fg_r, fg_g, fg_b, 255);
	bg_color = QColor(bg_r, bg_g, bg_b, 255);
	update_fgcolor_icon(fg_color);
	update_bgcolor_icon(bg_color);
	set_range(idx);
}

void CharsWidget::set_range(int x)
{
	const QVariant v = comboBox->itemData(x);
	const QList<QVariant> l = v.toList();
	if (l.size() != 2) return;
	CharsModel * old = static_cast<CharsModel*>(tableView->model());
	CharsModel * model = new CharsModel();
	QFont font = QApplication::font();
	font.setPointSizeF(font_size);
	font.setFamily(font_family);
	const bool symbola = (font_family == QString("Symbola"));
	if (merge_checkBox->isChecked())
#if 0
		font.setStyleStrategy(QFont::PreferOutline);
#else
		font.setStyleStrategy(QFont::PreferDefault);
#endif
	else
		font.setStyleStrategy(QFont::NoFontMerging);
	tableView->setFont(font);
	const int h = qMax(24, QFontMetrics(font).height()   + 12);
	const int w = qMax(24, QFontMetrics(font).maxWidth() + 12);
	model->set_fg_color(fg_color);
	model->set_bg_color(bg_color);
	model->set_range(l.at(0).toUInt(), l.at(1).toUInt(), symbola);
	tableView->setModel(model);
	for (int i = 0; i < 16; ++i)
	{
		tableView->setColumnWidth(i, w);
	}
	for (int i = 0; i < model->rowCount(); ++i)
	{
		tableView->setRowHeight(i, h);
	}
	if (model->rowCount() > 0)
	{
		tableView->showRow(0);
	}
	tableView->update();
	if (old)
	{
		delete old;
		old = NULL;
	}
}

void CharsWidget::set_font_size(double x)
{
	font_size = x;
	set_range(comboBox->currentIndex());
}

void CharsWidget::set_font_family(const QString & x)
{
	font_family = x;
	set_range(comboBox->currentIndex());
}

void CharsWidget::set_editor_font_size(double x)
{
	QTextCharFormat f;
	f.setFontPointSize(x);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_font_family(const QString & x)
{
	QTextCharFormat f;
	f.setFontFamily(x);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_bold(bool t)
{
	QTextCharFormat f;
	f.setFontWeight(t ? QFont::Bold : QFont::Normal);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_italic(bool t)
{
	QTextCharFormat f;
	f.setFontItalic(t);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_underline(bool t)
{
	QTextCharFormat f;
	f.setFontUnderline(t);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_strike(bool t)
{
	QTextCharFormat f;
	f.setFontStrikeOut(t);
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_superscript(bool t)
{
	QTextCharFormat f;
	if (t)
	{
		f.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
	}
	else
	{
		f.setVerticalAlignment(QTextCharFormat::AlignNormal);
	}
	textEdit->merge_format(f);
}

void CharsWidget::set_editor_subscript(bool t)
{
	QTextCharFormat f;
	if (t)
	{
		f.setVerticalAlignment(QTextCharFormat::AlignSubScript);
	}
	else
	{
		f.setVerticalAlignment(QTextCharFormat::AlignNormal);
	}
	textEdit->merge_format(f);
}

void CharsWidget::toggle_merging(bool)
{
	set_range(comboBox->currentIndex());
}

void CharsWidget::set_fg_color()
{
	CharsModel * m = qobject_cast<CharsModel*>(tableView->model());
	if (!m) return;
	const QColor c = QColorDialog::getColor(m->get_fg_color());
	fg_color = c;
	update_fgcolor_icon(c);
	set_range(comboBox->currentIndex());
}

void CharsWidget::set_bg_color()
{
	CharsModel * m = qobject_cast<CharsModel*>(tableView->model());
	if (!m) return;
	const QColor c = QColorDialog::getColor(m->get_bg_color());
	bg_color = c;
	update_bgcolor_icon(c);
	set_range(comboBox->currentIndex());
}

void CharsWidget::index_selected(QModelIndex index)
{
	CharsModel * m = qobject_cast<CharsModel*>(tableView->model());
	if (!m) return;
	QVariant v = m->data(index, Qt::DisplayRole);
	QTextCursor cursor = textEdit->textCursor();
	cursor.beginEditBlock();
	cursor.insertText(v.toString());
	cursor.endEditBlock();
	textEdit->setTextCursor(cursor);
}

void CharsWidget::append_acute_accent()
{
	textEdit->append_text(QString(QChar(0x301)));
}

void CharsWidget::append_grave_accent()
{
	textEdit->append_text(QString(QChar(0x300)));
}

void CharsWidget::append_left_double_angle()
{
	textEdit->append_text(QString(QChar(0xAB)));
}

void CharsWidget::append_right_double_angle()
{
	textEdit->append_text(QString(QChar(0xBB)));
}

void CharsWidget::append_ruble()
{
	textEdit->append_text(QString(QChar(0x20BD)));
}

void CharsWidget::append_rupee()
{
	textEdit->append_text(QString(QChar(0x20A8)));
}

void CharsWidget::append_lira()
{
	textEdit->append_text(QString(QChar(0x20BA)));
}

void CharsWidget::append_euro()
{
	textEdit->append_text(QString(QChar(0x20AC)));
}

void CharsWidget::append_number()
{
	textEdit->append_text(QString(QChar(0x2116)));
}

void CharsWidget::append_degree()
{
	textEdit->append_text(QString(QChar(0xB0)));
}

void CharsWidget::append_endash()
{
	textEdit->append_text(QString(QChar(0x2013)));
}

void CharsWidget::append_emdash()
{
	textEdit->append_text(QString(QChar(0x2014)));
}

void CharsWidget::append_lapki1()
{
	textEdit->append_text(QString(QChar(0x201E)));
}

void CharsWidget::append_lapki2()
{
	textEdit->append_text(QString(QChar(0x201C)));
}

void CharsWidget::append_tm()
{
	textEdit->append_text(QString(QChar(0x2122)));
}

void CharsWidget::reset_colors()
{
	fg_color = QApplication::palette().color(QPalette::Text);
	bg_color = QApplication::palette().color(QPalette::Base);
	update_fgcolor_icon(fg_color);
	update_bgcolor_icon(bg_color);
	set_range(comboBox->currentIndex());
}

void CharsWidget::text_style(int i)
{
	QTextCursor c = textEdit->textCursor();
	if (i != 0)
	{
		QTextListFormat::Style style = QTextListFormat::ListDisc;
		switch (i)
		{
			case 1:
				style = QTextListFormat::ListDisc;
				break;
			case 2:
				style = QTextListFormat::ListCircle;
				break;
			case 3:
				style = QTextListFormat::ListSquare;
				break;
			case 4:
				style = QTextListFormat::ListDecimal;
				break;
			case 5:
				style = QTextListFormat::ListLowerAlpha;
				break;
			case 6:
				style = QTextListFormat::ListUpperAlpha;
				break;
			case 7:
				style = QTextListFormat::ListLowerRoman;
				break;
			case 8:
				style = QTextListFormat::ListUpperRoman;
				break;
			default:
				break;
		}
		saved_list_style = style;
		c.beginEditBlock();
		QTextListFormat lf;
		if (c.currentList())
			lf = c.currentList()->format();
		else
			lf.setIndent(5);
		lf.setStyle(style);
		c.createList(lf);
		c.endEditBlock();
	}
	else
	{
		saved_list_style = static_cast<QTextListFormat::Style>(0);
		QTextBlockFormat b;
		b.setObjectIndex(-1);
		c.mergeBlockFormat(b);
	}
	block_format_changed(const_cast<const QTextCursor&>(c));
}

void CharsWidget::update_text_color_icon(const QColor & c)
{
	QPixmap p(24, 24);
	p.fill(c);
	color_toolButton->setIcon(p);
}

void CharsWidget::update_fgcolor_icon(const QColor & c)
{
	QPixmap p(24, 24);
	p.fill(c);
	fg_toolButton->setIcon(p);
}

void CharsWidget::update_bgcolor_icon(const QColor & c)
{
	QPixmap p(24, 24);
	p.fill(c);
	bg_toolButton->setIcon(p);
}

void CharsWidget::text_color()
{
	QColor c =
		QColorDialog::getColor(
			textEdit->textColor(),
			this);
	if (!c.isValid()) return;
	update_text_color_icon(c);
	QTextCharFormat f;
	f.setForeground(c);
	textEdit->merge_format(f);
}

void CharsWidget::char_format_changed(const QTextCharFormat & f)
{
	if (saved_char_format == f) return;
	e_fontComboBox->blockSignals(true);
	e_doubleSpinBox->blockSignals(true);
	e_b_toolButton->blockSignals(true);
	e_i_toolButton->blockSignals(true);
	e_underline_toolButton->blockSignals(true);
	e_strike_toolButton->blockSignals(true);
	superscript_toolButton->blockSignals(true);
	subscript_toolButton->blockSignals(true);
    const QFont & ff = f.font();
	e_fontComboBox->setCurrentIndex(e_fontComboBox->findText(
		QFontInfo(ff).family()));
	e_doubleSpinBox->setValue(ff.pointSizeF());
	e_b_toolButton->setChecked(ff.bold());
	e_i_toolButton->setChecked(ff.italic());
	e_underline_toolButton->setChecked(ff.underline());
	e_strike_toolButton->setChecked(ff.strikeOut());
	switch(f.verticalAlignment())
	{
	case QTextCharFormat::AlignSuperScript:
		superscript_toolButton->setChecked(true);
		subscript_toolButton->setChecked(false);
		break;
	case QTextCharFormat::AlignSubScript:
		superscript_toolButton->setChecked(false);
		subscript_toolButton->setChecked(true);
		break;
	default:
		superscript_toolButton->setChecked(false);
		subscript_toolButton->setChecked(false);
		break;
	}
    update_text_color_icon(f.foreground().color());
	update_format();
	saved_char_format = f;
	e_fontComboBox->blockSignals(false);
	e_doubleSpinBox->blockSignals(false);
	e_b_toolButton->blockSignals(false);
	e_i_toolButton->blockSignals(false);
	e_underline_toolButton->blockSignals(false);
	e_strike_toolButton->blockSignals(false);
	superscript_toolButton->blockSignals(false);
	subscript_toolButton->blockSignals(false);
}

void CharsWidget::cursor_position_changed()
{
    alignment_changed(textEdit->alignment());
	const QTextCursor c = textEdit->textCursor();
	list_format_changed(c);
	block_format_changed(c);
	table_changed(c);
}

void CharsWidget::alignment_changed(Qt::Alignment a)
{
	alignleft_toolButton->blockSignals(true);
	aligncenter_toolButton->blockSignals(true);
	alignright_toolButton->blockSignals(true);
	alignjustify_toolButton->blockSignals(true);
	if (a & Qt::AlignLeft)
	{
		alignleft_toolButton->setChecked(true);
	}
	else if (a & Qt::AlignHCenter)
	{
		aligncenter_toolButton->setChecked(true);
	}
	else if (a & Qt::AlignRight)
	{
		alignright_toolButton->setChecked(true);
	}
	else if (a & Qt::AlignJustify)
	{
		alignjustify_toolButton->setChecked(true);
	}
	alignleft_toolButton->blockSignals(false);
	aligncenter_toolButton->blockSignals(false);
	alignright_toolButton->blockSignals(false);
	alignjustify_toolButton->blockSignals(false);
}

void CharsWidget::list_format_changed(const QTextCursor & c)
{
	const QTextList * l = c.currentList();
	if (!l)
	{
		if (style_comboBox->currentIndex() != 0)
		{
			style_comboBox->blockSignals(true);
			style_comboBox->setCurrentIndex(0);
			style_comboBox->blockSignals(false);
		}
		saved_list_style = static_cast<QTextListFormat::Style>(0);
	}
	else
	{
		const QTextListFormat f = l->format();
		const QTextListFormat::Style s = f.style();
		if (s != saved_list_style || saved_list_style == 0)
		{
			style_comboBox->blockSignals(true);
			switch (s)
			{
				case QTextListFormat::ListDisc:
					style_comboBox->setCurrentIndex(1);
					break;
				case QTextListFormat::ListCircle:
					style_comboBox->setCurrentIndex(2);
					break;
				case QTextListFormat::ListSquare:
					style_comboBox->setCurrentIndex(3);
					break;
				case QTextListFormat::ListDecimal:
					style_comboBox->setCurrentIndex(4);
					break;
				case QTextListFormat::ListLowerAlpha:
					style_comboBox->setCurrentIndex(5);
					break;
				case QTextListFormat::ListUpperAlpha:
					style_comboBox->setCurrentIndex(6);
					break;
				case QTextListFormat::ListLowerRoman:
					style_comboBox->setCurrentIndex(7);
					break;
				case QTextListFormat::ListUpperRoman:
					style_comboBox->setCurrentIndex(8);
					break;
				default: // error
					break;
			}
			saved_list_style = s;
			style_comboBox->blockSignals(false);
		}
	}
}

void CharsWidget::block_format_changed(const QTextCursor & c)
{
	const QTextBlockFormat & bf = c.blockFormat();
	if (saved_block_format != bf)
	{
		const int    ind  = bf.indent();
		const double ind1 = TextEdit::px2mm(bf.textIndent(),   true);
		const double lm   = TextEdit::px2mm(bf.leftMargin(),   true);
		const double tm   = TextEdit::px2mm(bf.topMargin(),    false);
		const double rm   = TextEdit::px2mm(bf.rightMargin(),  true);
		const double bm   = TextEdit::px2mm(bf.bottomMargin(), false);
		const int    ht   = (int)bf.lineHeightType();
		int          lh   = (int)bf.lineHeight();
		if (ht == (int)QTextBlockFormat::SingleHeight)
		{
			lh = 100;
		}
		if (indent_spinBox->value() != ind)
		{
			indent_spinBox->blockSignals(true);
			indent_spinBox->setValue(ind);
			indent_spinBox->blockSignals(false);
		}
		//if (!itk::Math::FloatAlmostEqual(text_indent_doubleSpinBox->value(), ind1))
		if (text_indent_doubleSpinBox->value() != ind1) // FIXME
		{
			text_indent_doubleSpinBox->blockSignals(true);
			text_indent_doubleSpinBox->setValue(ind1);
			text_indent_doubleSpinBox->blockSignals(false);
		}
		if (p_line_h_spinBox->value() != lh)
		{
			p_line_h_spinBox->blockSignals(true);
			p_line_h_spinBox->setValue(lh);
			p_line_h_spinBox->blockSignals(false);
		}
		//if (!itk::Math::FloatAlmostEqual(p_lm_doubleSpinBox->value(), lm))
		if (p_lm_doubleSpinBox->value() != lm) // FIXME
		{
			p_lm_doubleSpinBox->blockSignals(true);
			p_lm_doubleSpinBox->setValue(lm);
			p_lm_doubleSpinBox->blockSignals(false);
		}
		//if (!itk::Math::FloatAlmostEqual(p_tm_doubleSpinBox->value(), tm))
		if (p_tm_doubleSpinBox->value() != tm) // FIXME
		{
			p_tm_doubleSpinBox->blockSignals(true);
			p_tm_doubleSpinBox->setValue(tm);
			p_tm_doubleSpinBox->blockSignals(false);
		}
		//if (!itk::Math::FloatAlmostEqual(p_rm_doubleSpinBox->value(), rm))
		if (p_rm_doubleSpinBox->value() != rm) // FIXME
		{
			p_rm_doubleSpinBox->blockSignals(true);
			p_rm_doubleSpinBox->setValue(rm);
			p_rm_doubleSpinBox->blockSignals(false);
		}
		//if (!itk::Math::FloatAlmostEqual(p_bm_doubleSpinBox->value(), bm))
		if (p_bm_doubleSpinBox->value() != bm) // FIXME
		{
			p_bm_doubleSpinBox->blockSignals(true);
			p_bm_doubleSpinBox->setValue(bm);
			p_bm_doubleSpinBox->blockSignals(false);
		}
		saved_block_format = bf;
	}
}

void CharsWidget::table_changed(const QTextCursor & c)
{
	const QTextTable * t = c.currentTable();
	if (t)
	{
		if (!table_frame->isEnabled())
		{
			table_frame->setEnabled(true);
		}
		border_style_comboBox->blockSignals(true);
		QTextTableFormat f = t->format();
		if (f.isValid())
		{
			QTextFrameFormat::BorderStyle s = f.borderStyle();
			switch(s)
			{
			case QTextFrameFormat::BorderStyle_None:
				border_style_comboBox->setCurrentIndex(1);
				break;
			case QTextFrameFormat::BorderStyle_Dotted:
				border_style_comboBox->setCurrentIndex(2);
				break;
			case QTextFrameFormat::BorderStyle_Dashed:
				border_style_comboBox->setCurrentIndex(3);
				break;
			case QTextFrameFormat::BorderStyle_Solid:
				border_style_comboBox->setCurrentIndex(4);
				break;
			case QTextFrameFormat::BorderStyle_Double:
				border_style_comboBox->setCurrentIndex(5);
				break;
			case QTextFrameFormat::BorderStyle_DotDash:
				border_style_comboBox->setCurrentIndex(6);
				break;
			case QTextFrameFormat::BorderStyle_DotDotDash:
				border_style_comboBox->setCurrentIndex(7);
				break;
			case QTextFrameFormat::BorderStyle_Groove:
				border_style_comboBox->setCurrentIndex(8);
				break;
			case QTextFrameFormat::BorderStyle_Ridge:
				border_style_comboBox->setCurrentIndex(9);
				break;
			case QTextFrameFormat::BorderStyle_Inset:
				border_style_comboBox->setCurrentIndex(10);
				break;
			case QTextFrameFormat::BorderStyle_Outset:
				border_style_comboBox->setCurrentIndex(11);
				break;
			default:
				border_style_comboBox->setCurrentIndex(0);
				break;
			}
		}
		else
		{
			border_style_comboBox->setCurrentIndex(0);
		}
		border_style_comboBox->blockSignals(false);
	}
	else
	{
		if (table_frame->isEnabled())
		{
			table_frame->setEnabled(false);
		}
		if (border_style_comboBox->currentIndex() != 0)
		{
			border_style_comboBox->blockSignals(true);
			border_style_comboBox->setCurrentIndex(0);
			border_style_comboBox->blockSignals(false);
		}
	}
}

bool CharsWidget::maybe_save()
{
	if (!textEdit->document()->isModified()) return true;
	QMessageBox::StandardButton r;
	r = QMessageBox::warning(
		this,
		QString("Application"),
		QString(
			"The document has been modified.\n"
			"Do you want to save your changes?"),
		QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
	if (r == QMessageBox::Save)
	{
		return file_export();
	}
	else if (r == QMessageBox::Cancel)
	{
		return false;
	}
	return true;
}

// TODO
bool CharsWidget::file_export()
{
/*
	QFileDialog d(this);
	d.setFileMode(QFileDialog::AnyFile);
	d.setAcceptMode(QFileDialog::AcceptSave);
	d.setNameFilter(QString(
		"All Files (*);;"
		"PDF (*.pdf);;"
		"DICOM (*.dcm);;"
		"Open Document files (*.odt);;"
		"Text files (*.txt);;"
		"Html files (*.html);;"
		));
	QStringList fileNames;
	if (d.exec()) fileNames = d.selectedFiles();
	if (fileNames.empty()) return false;
	QString f = fileNames.at(0);
*/
	QString f = QFileDialog::getSaveFileName(
		this,
		//QString("Export, format by extension (pdf, dcm, odt, txt, html)"),
		QString("Export, format by extension (pdf, odt, txt, html)"),
		QString(),
		QString(
			"All Files (*);;"
			"PDF (*.pdf);;"
			//"DICOM (*.dcm);;"
			"Open Document files (*.odt);;"
			"Text files (*.txt);;"
			"Html files (*.html);;"
			));
	if (f.isEmpty()) return false;
	if (f.endsWith(".pdf", Qt::CaseInsensitive))
	{
		write_file(f, 10);
	}
	else if (f.endsWith(".odt", Qt::CaseInsensitive))
	{
		write_file(f, 1);
	}
	else if (f.endsWith(".txt", Qt::CaseInsensitive))
	{
		write_file(f, 0);
	}
	else if (f.endsWith(".html", Qt::CaseInsensitive))
	{
		write_file(f, 2);
	}
	//else if (f.endsWith(".dcm", Qt::CaseInsensitive))
	//{
	//	write_file(f, 11);
	//}
	else
	{
		f += QString(".pdf");
		write_file(f, 10);
	}
	textEdit->document()->setModified(false);
	return true;
}

void CharsWidget::write_file(const QString & f, const short ff)
{
	if (ff == 10)
	{
		QPrinter p(QPrinter::HighResolution);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		p.setPageSize(QPageSize(textEdit->m_paper_format));
		p.setPageOrientation(textEdit->m_paper_orientaion);
#else
		p.setPageSize(textEdit->m_paper_format);
		p.setOrientation(textEdit->m_paper_orientaion);
#endif
		p.setOutputFormat(QPrinter::PdfFormat);
		p.setOutputFileName(f);
		textEdit->print(&p);
		textEdit->document()->setModified(false);
	}
	//else if (ff == 11)
	//{
	//	QPrinter p(QPrinter::HighResolution);
	//	p.setPageSize(textEdit->m_paper_format);
	//	p.setOrientation(textEdit->m_paper_orientaion);
	//	p.setOutputFormat(QPrinter::PdfFormat);
	//	p.setOutputFileName(f);
	//	textEdit->print(&p);
	//	textEdit->document()->setModified(false);
	//}
	else
	{
		QTextDocumentWriter writer(f);
		if (ff == 1)
		{
			writer.setFormat("ODT");
		}
		else if (ff == 0)
		{
			writer.setFormat("plaintext");
		}
		else if (ff == 2)
		{
			writer.setFormat("HTML");
		}
		if (writer.write(textEdit->document()))
		{
			textEdit->document()->setModified(false);
		}
	}
}

void CharsWidget::file_print()
{
	QPrinter p(QPrinter::HighResolution);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	p.setPageSize(QPageSize(textEdit->m_paper_format));
	p.setPageOrientation(textEdit->m_paper_orientaion);
#else
	p.setPageSize(textEdit->m_paper_format);
	p.setOrientation(textEdit->m_paper_orientaion);
#endif
	QPrintDialog * d = new QPrintDialog(&p, this);
	if (textEdit->textCursor().hasSelection())
	{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		d->setOption(QAbstractPrintDialog::PrintSelection);
#else
		d->addEnabledOption(QAbstractPrintDialog::PrintSelection);
#endif
	}
	d->setWindowTitle(QString("Print Document"));
	if (d->exec() == QDialog::Accepted)
	{
		textEdit->print(&p);
	}
	delete d;
}

void CharsWidget::new_document()
{
	if (maybe_save())
	{
		indent_spinBox->blockSignals(true);
		text_indent_doubleSpinBox->blockSignals(true);
		p_line_h_spinBox->blockSignals(true);
		p_lm_doubleSpinBox->blockSignals(true);
		p_tm_doubleSpinBox->blockSignals(true);
		p_rm_doubleSpinBox->blockSignals(true);
		p_bm_doubleSpinBox->blockSignals(true);
		indent_spinBox->setValue(0);
		text_indent_doubleSpinBox->setValue(0);
		p_line_h_spinBox->setValue(100);
		p_lm_doubleSpinBox->setValue(0);
		p_tm_doubleSpinBox->setValue(0);
		p_rm_doubleSpinBox->setValue(0);
		p_bm_doubleSpinBox->setValue(0);
		textEdit->new_document();
		indent_spinBox->blockSignals(false);
		text_indent_doubleSpinBox->blockSignals(false);
		p_line_h_spinBox->blockSignals(false);
		p_lm_doubleSpinBox->blockSignals(false);
		p_tm_doubleSpinBox->blockSignals(false);
		p_rm_doubleSpinBox->blockSignals(false);
		p_bm_doubleSpinBox->blockSignals(false);
#ifdef USE_DOC_SCENE
		graphicsview->resize(text_frame->width(),text_frame->height());
		graphicsview->update_geometry(QSizeF(graphicsview->width()/view_scale,graphicsview->height()/view_scale));
		graphicsview->updateGeometry();
		qApp->processEvents();
#endif
	}
}

void CharsWidget::update_editor_settings()
{
	textEdit->update_settings();
}

void CharsWidget::set_format(int x)
{
	if (x == 0) return;
	double pt = textEdit->m_font_size;
	bool   b  = false;
	bool   c  = false;
	switch(x)
	{
	case 2: // title
		pt = 28.0;
		b  = true;
		c  = true;
		break;
	case 3: // subtitle
		pt = 18.0;
		b  = true;
		c  = true;
		break;
	case 4: // heading 1
		pt = 18.2;
		b  = true;
		c  = false;
		break;
	case 5: // heading 2
		pt = 16.1;
		b  = true;
		c  = false;
		break;
	case 6: // heading 3
		pt = 14.1;
		b  = true;
		c  = false;
		break;
	case 7: // heading
		pt = 14.0;
		b  = false;
		c  = false;
		break;
	case 8: // footnote
		pt = 10.0;
		b  = false;
		c  = false;
		break;
	case 1:
	default:
		break;
	}
	e_doubleSpinBox->blockSignals(true);
	e_b_toolButton->blockSignals(true);
	e_i_toolButton->blockSignals(true);
	e_underline_toolButton->blockSignals(true);
	e_strike_toolButton->blockSignals(true);
	alignleft_toolButton->blockSignals(true);
	aligncenter_toolButton->blockSignals(true);
	alignright_toolButton->blockSignals(true);
	alignjustify_toolButton->blockSignals(true);
	superscript_toolButton->blockSignals(true);
	subscript_toolButton->blockSignals(true);
	e_doubleSpinBox->setValue(pt);
	e_b_toolButton->setChecked(b);
	e_i_toolButton->setChecked(false);
	e_underline_toolButton->setChecked(false);
	e_strike_toolButton->setChecked(false);
	superscript_toolButton->setChecked(false);
	subscript_toolButton->setChecked(false);
	QTextCharFormat f;
	f.setFontPointSize(pt);
	f.setFontWeight(b ? QFont::Bold : QFont::Normal);
	f.setFontItalic(false);
	f.setFontUnderline(false);
	f.setFontStrikeOut(false);
	f.setVerticalAlignment(QTextCharFormat::AlignNormal);
	textEdit->merge_format(f);
	if (c)
	{
		alignleft_toolButton->setChecked(false);
		aligncenter_toolButton->setChecked(true);
		alignright_toolButton->setChecked(false);
		alignjustify_toolButton->setChecked(false);
		textEdit->text_aligncenter(true);
	}
	else
	{
		if (QApplication::isLeftToRight())
		{
			alignleft_toolButton->setChecked(true);
			aligncenter_toolButton->setChecked(false);
			alignright_toolButton->setChecked(false);
			alignjustify_toolButton->setChecked(false);
			textEdit->text_alignleft(true);
		}
		else
		{
			alignleft_toolButton->setChecked(false);
			aligncenter_toolButton->setChecked(false);
			alignright_toolButton->setChecked(true);
			alignjustify_toolButton->setChecked(false);
			textEdit->text_alignright(true);
		}
	}
	e_doubleSpinBox->blockSignals(false);
	e_b_toolButton->blockSignals(false);
	e_i_toolButton->blockSignals(false);
	e_underline_toolButton->blockSignals(false);
	e_strike_toolButton->blockSignals(false);
	alignleft_toolButton->blockSignals(false);
	aligncenter_toolButton->blockSignals(false);
	alignright_toolButton->blockSignals(false);
	alignjustify_toolButton->blockSignals(false);
	superscript_toolButton->blockSignals(false);
	subscript_toolButton->blockSignals(false);
}

void CharsWidget::update_format()
{
	if (format_comboBox->currentIndex() != 0)
	{
		format_comboBox->blockSignals(true);
		format_comboBox->setCurrentIndex(0);
		format_comboBox->blockSignals(false);
	}
}

void CharsWidget::show_settings(bool t)
{
	if (t) settings_tabWidget->show();
	else   settings_tabWidget->hide();
	qApp->processEvents();
}

void CharsWidget::set_editor_default_font(const QString & s)
{
	textEdit->set_default_font(s.trimmed());
}

void CharsWidget::reset_block_settings()
{
	indent_spinBox->blockSignals(true);
	text_indent_doubleSpinBox->blockSignals(true);
	p_lm_doubleSpinBox->blockSignals(true);
	p_tm_doubleSpinBox->blockSignals(true);
	p_rm_doubleSpinBox->blockSignals(true);
	p_bm_doubleSpinBox->blockSignals(true);
	p_line_h_spinBox->blockSignals(true);
	indent_spinBox->setValue(0);
	text_indent_doubleSpinBox->setValue(0);
	p_lm_doubleSpinBox->setValue(0);
	p_tm_doubleSpinBox->setValue(0);
	p_rm_doubleSpinBox->setValue(0);
	p_bm_doubleSpinBox->setValue(0);
	p_line_h_spinBox->setValue(100);
	textEdit->reset_block();
	indent_spinBox->blockSignals(false);
	text_indent_doubleSpinBox->blockSignals(false);
	p_lm_doubleSpinBox->blockSignals(false);
	p_tm_doubleSpinBox->blockSignals(false);
	p_rm_doubleSpinBox->blockSignals(false);
	p_bm_doubleSpinBox->blockSignals(false);
	p_line_h_spinBox->blockSignals(false);
}

void CharsWidget::insert_table(int r, int c)
{
	textEdit->insert_table(r, c);
}

void CharsWidget::insert_table2()
{
	bool ok = false;
	int r = 2, c = 2;
	TableDialog * d = new TableDialog();
	if (d->exec() == QDialog::Accepted)
	{
		ok = true;
		r = d->get_rows();
		c = d->get_columns();
	}
	delete d;
	if (ok) textEdit->insert_table(r, c);
	qApp->processEvents();
}

void CharsWidget::set_table_border_style(int i)
{
	if (i == 0) return;
	QTextFrameFormat::BorderStyle s =
		static_cast<QTextFrameFormat::BorderStyle>(0);
	bool ok = true;
	switch(i)
	{
	case 1:
		s = QTextFrameFormat::BorderStyle_None;
		break;
	case 2:
		s = QTextFrameFormat::BorderStyle_Dotted;
		break;
	case 3:
		s = QTextFrameFormat::BorderStyle_Dashed;
		break;
	case 4:
		s = QTextFrameFormat::BorderStyle_Solid;
		break;
	case 5:
		s = QTextFrameFormat::BorderStyle_Double;
		break;
	case 6:
		s = QTextFrameFormat::BorderStyle_DotDash;
		break;
	case 7:
		s = QTextFrameFormat::BorderStyle_DotDotDash;
		break;
	case 8:
		s = QTextFrameFormat::BorderStyle_Groove;
		break;
	case 9:
		s = QTextFrameFormat::BorderStyle_Ridge;
		break;
	case 10:
		s = QTextFrameFormat::BorderStyle_Inset;
		break;
	case 11:
		s = QTextFrameFormat::BorderStyle_Outset;
		break;
	default:
		ok = false;
		break;
	}
	if (ok) textEdit->set_table_border_style(s);
}

void CharsWidget::zoom_in()
{
	view_scale += 0.2;
	if (view_scale > 5.0) view_scale = 5.0;
#ifdef USE_DOC_SCENE
	graphicsview->set_view_scale(view_scale);
	graphicsview->resize(text_frame->width(),text_frame->height());
	graphicsview->update_geometry(QSizeF(graphicsview->width()/view_scale,graphicsview->height()/view_scale));
#endif
}

void CharsWidget::zoom_out()
{
	view_scale -= 0.2;
	if (view_scale < 0.2) view_scale = 0.2;
#ifdef USE_DOC_SCENE
	graphicsview->set_view_scale(view_scale);
	graphicsview->resize(text_frame->width(),text_frame->height());
	graphicsview->update_geometry(QSizeF(graphicsview->width()/view_scale,graphicsview->height()/view_scale));
#endif
}

void CharsWidget::zoom_one()
{
	view_scale = 1.0;
#ifdef USE_DOC_SCENE
	graphicsview->set_view_scale(view_scale);
	graphicsview->resize(text_frame->width(),text_frame->height());
	graphicsview->update_geometry(QSizeF(graphicsview->width(),graphicsview->height()));
#endif
}
