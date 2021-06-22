#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include "textedit-qt6.h"
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include "textedit-qt5.h"
#else
#include "textedit-qt4.h"
#endif
#include <QVariant>
#include <QList>
#include <QTextDocumentWriter>
#include <QTextFrameFormat>
#include <QTextLength>
#include <QFileDialog>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QTextFrame>
#include <QTextTable>
#include <QTextTableFormat>
#include <QAbstractTextDocumentLayout>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QPainter>
#include <QScrollBar>
#include <QPrintPreviewDialog>
#if 0
#include <iostream>
#endif

static bool border__ = true;

TextEdit::TextEdit()
{
	setAcceptDrops(true);
	readSettings();
#if 0
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	const QScreen * d = qApp->primaryScreen();
	if (d)
	{
		std::cout <<
			d->logicalDotsPerInchX()  << " " <<
			d->logicalDotsPerInchY()  << " " <<
			d->physicalDotsPerInchX() << " " <<
			d->physicalDotsPerInchY() << std::endl;
	}
#else
	const QDesktopWidget * d = qApp->desktop();
	if (d)
	{
		std::cout <<
			d->logicalDpiX()  << " " <<
			d->logicalDpiY()  << " " <<
			d->physicalDpiX() << " " <<
			d->physicalDpiY() << std::endl;
	}
#endif
#endif
}

void TextEdit::new_document()
{
	document()->clear();
	set_paper_format(m_paper_format);
	QFont f = document()->defaultFont();
	f.setFamily(m_font_family);
	f.setPointSizeF(m_font_size);
	document()->setDefaultFont(f);
	QTextCharFormat ff;
	ff.setFontFamily(f.family());
	ff.setFontPointSize(m_font_size);
	merge_format(ff);
	document()->setModified(false);
	document()->clearUndoRedoStacks();
}

void TextEdit::append_text(const QString & s)
{
	QTextCursor cursor = textCursor();
	cursor.beginEditBlock();
	cursor.insertText(s);
	cursor.endEditBlock();
	setTextCursor(cursor);
}

TextEdit::~TextEdit()
{
}

// US Letter width 215.9 mm, length 279.4 mm
// A4        width 210   mm, length 297   mm
qreal TextEdit::mm2px(qreal mm, bool horis, bool logical)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	const QScreen * d = qApp->primaryScreen();
#else
	const QDesktopWidget * d = qApp->desktop();
#endif
	if (!d) return 0.0;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	const qreal x =
		horis
		? (
			logical
			?
			d->logicalDotsPerInchX()
			:
			d->physicalDotsPerInchX())
		: (
			logical
			?
			d->logicalDotsPerInchY()
			:
			d->physicalDotsPerInchY());
#else
	const qreal x =
		horis
		? (
			logical
			?
			d->logicalDpiX()
			:
			d->physicalDpiX())
		: (
			logical
			?
			d->logicalDpiY()
			:
			d->physicalDpiY());
#endif
	return (0.039370147*x*mm);
}

qreal TextEdit::px2mm(qreal px, bool horis, bool logical)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	const QScreen * d = qApp->primaryScreen();
#else
	const QDesktopWidget * d = qApp->desktop();
#endif
	if (!d) return 0.0;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	const qreal x =
		horis
		? (
			logical
			?
			d->logicalDotsPerInchX()
			:
			d->physicalDotsPerInchX())
		: (
			logical
			?
			d->logicalDotsPerInchY()
			:
			d->physicalDotsPerInchY());
#else
	const qreal x =
		horis
		? (
			logical
			?
			d->logicalDpiX()
			:
			d->physicalDpiX())
		: (
			logical
			?
			d->logicalDpiY()
			:
			d->physicalDpiY());
#endif
	return ((25.4/x)*px);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QPageSize::PageSizeId TextEdit::pagesize_from_qstring(const QString & s)
{
	QPageSize::PageSizeId p = static_cast<QPageSize::PageSizeId>(0);
	if      (s == QString("A4"))        p = QPageSize::A4;
	else if (s == QString("Letter"))    p = QPageSize::Letter;
	else if (s == QString("A0"))        p = QPageSize::A0;
	else if (s == QString("A1"))        p = QPageSize::A1;
	else if (s == QString("A2"))        p = QPageSize::A2;
	else if (s == QString("A3"))        p = QPageSize::A3;
	else if (s == QString("A5"))        p = QPageSize::A5;
	else if (s == QString("A6"))        p = QPageSize::A6;
	else if (s == QString("A7"))        p = QPageSize::A7;
	else if (s == QString("A8"))        p = QPageSize::A8;
	else if (s == QString("A9"))        p = QPageSize::A9;
	else if (s == QString("B0"))        p = QPageSize::B0;
	else if (s == QString("B1"))        p = QPageSize::B1;
	else if (s == QString("B2"))        p = QPageSize::B2;
	else if (s == QString("B3"))        p = QPageSize::B3;
	else if (s == QString("B4"))        p = QPageSize::B4;
	else if (s == QString("B5"))        p = QPageSize::B5;
	else if (s == QString("B6"))        p = QPageSize::B6;
	else if (s == QString("B7"))        p = QPageSize::B7;
	else if (s == QString("B8"))        p = QPageSize::B8;
	else if (s == QString("B9"))        p = QPageSize::B9;
	else if (s == QString("B10"))       p = QPageSize::B10;
	else if (s == QString("C5E"))       p = QPageSize::C5E;
	else if (s == QString("Comm10E"))   p = QPageSize::Comm10E;
	else if (s == QString("DLE"))       p = QPageSize::DLE;
	else if (s == QString("Executive")) p = QPageSize::Executive;
	else if (s == QString("Folio"))     p = QPageSize::Folio;
	else if (s == QString("Ledger"))    p = QPageSize::Ledger;
	else if (s == QString("Legal"))     p = QPageSize::Legal;
	else if (s == QString("Tabloid"))   p = QPageSize::Tabloid;
	return p;
}
#else
QPrinter::PageSize TextEdit::pagesize_from_qstring(const QString & s)
{
	QPrinter::PageSize p = static_cast<QPrinter::PageSize>(0);
	if      (s == QString("A4"))        p = QPrinter::A4;
	else if (s == QString("Letter"))    p = QPrinter::Letter;
	else if (s == QString("A0"))        p = QPrinter::A0;
	else if (s == QString("A1"))        p = QPrinter::A1;
	else if (s == QString("A2"))        p = QPrinter::A2;
	else if (s == QString("A3"))        p = QPrinter::A3;
	else if (s == QString("A5"))        p = QPrinter::A5;
	else if (s == QString("A6"))        p = QPrinter::A6;
	else if (s == QString("A7"))        p = QPrinter::A7;
	else if (s == QString("A8"))        p = QPrinter::A8;
	else if (s == QString("A9"))        p = QPrinter::A9;
	else if (s == QString("B0"))        p = QPrinter::B0;
	else if (s == QString("B1"))        p = QPrinter::B1;
	else if (s == QString("B2"))        p = QPrinter::B2;
	else if (s == QString("B3"))        p = QPrinter::B3;
	else if (s == QString("B4"))        p = QPrinter::B4;
	else if (s == QString("B5"))        p = QPrinter::B5;
	else if (s == QString("B6"))        p = QPrinter::B6;
	else if (s == QString("B7"))        p = QPrinter::B7;
	else if (s == QString("B8"))        p = QPrinter::B8;
	else if (s == QString("B9"))        p = QPrinter::B9;
	else if (s == QString("B10"))       p = QPrinter::B10;
	else if (s == QString("C5E"))       p = QPrinter::C5E;
	else if (s == QString("Comm10E"))   p = QPrinter::Comm10E;
	else if (s == QString("DLE"))       p = QPrinter::DLE;
	else if (s == QString("Executive")) p = QPrinter::Executive;
	else if (s == QString("Folio"))     p = QPrinter::Folio;
	else if (s == QString("Ledger"))    p = QPrinter::Ledger;
	else if (s == QString("Legal"))     p = QPrinter::Legal;
	else if (s == QString("Tabloid"))   p = QPrinter::Tabloid;
	return p;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QString TextEdit::qstring_from_pagesize(const QPageSize::PageSizeId s)
{
	QString p("");
	if      (s == QPageSize::A4)       p = QString("A4");
	else if (s == QPageSize::Letter)   p = QString("Letter");
	else if (s == QPageSize::A0)       p = QString("A0");
	else if (s == QPageSize::A1)       p = QString("A1");
	else if (s == QPageSize::A2)       p = QString("A2");
	else if (s == QPageSize::A3)       p = QString("A3");
	else if (s == QPageSize::A5)       p = QString("A5");
	else if (s == QPageSize::A6)       p = QString("A6");
	else if (s == QPageSize::A7)       p = QString("A7");
	else if (s == QPageSize::A8)       p = QString("A8");
	else if (s == QPageSize::A9)       p = QString("A9");
	else if (s == QPageSize::B0)       p = QString("B0");
	else if (s == QPageSize::B1)       p = QString("B1");
	else if (s == QPageSize::B2)       p = QString("B2");
	else if (s == QPageSize::B3)       p = QString("B3");
	else if (s == QPageSize::B4)       p = QString("B4");
	else if (s == QPageSize::B5)       p = QString("B5");
	else if (s == QPageSize::B6)       p = QString("B6");
	else if (s == QPageSize::B7)       p = QString("B7");
	else if (s == QPageSize::B8)       p = QString("B8");
	else if (s == QPageSize::B9)       p = QString("B9");
	else if (s == QPageSize::B10)      p = QString("B10");
	else if (s == QPageSize::C5E)      p = QString("C5E");
	else if (s == QPageSize::Comm10E)  p = QString("Comm10E");
	else if (s == QPageSize::DLE)      p = QString("DLE");
	else if (s == QPageSize::Executive)p = QString("Executive");
	else if (s == QPageSize::Folio)    p = QString("Folio");
	else if (s == QPageSize::Ledger)   p = QString("Ledger");
	else if (s == QPageSize::Legal)    p = QString("Legal");
	else if (s == QPageSize::Tabloid)  p = QString("Tabloid");
	return p;
}
#else
QString TextEdit::qstring_from_pagesize(const QPrinter::PageSize s)
{
	QString p("");
	if      (s == QPrinter::A4)       p = QString("A4");
	else if (s == QPrinter::Letter)   p = QString("Letter");
	else if (s == QPrinter::A0)       p = QString("A0");
	else if (s == QPrinter::A1)       p = QString("A1");
	else if (s == QPrinter::A2)       p = QString("A2");
	else if (s == QPrinter::A3)       p = QString("A3");
	else if (s == QPrinter::A5)       p = QString("A5");
	else if (s == QPrinter::A6)       p = QString("A6");
	else if (s == QPrinter::A7)       p = QString("A7");
	else if (s == QPrinter::A8)       p = QString("A8");
	else if (s == QPrinter::A9)       p = QString("A9");
	else if (s == QPrinter::B0)       p = QString("B0");
	else if (s == QPrinter::B1)       p = QString("B1");
	else if (s == QPrinter::B2)       p = QString("B2");
	else if (s == QPrinter::B3)       p = QString("B3");
	else if (s == QPrinter::B4)       p = QString("B4");
	else if (s == QPrinter::B5)       p = QString("B5");
	else if (s == QPrinter::B6)       p = QString("B6");
	else if (s == QPrinter::B7)       p = QString("B7");
	else if (s == QPrinter::B8)       p = QString("B8");
	else if (s == QPrinter::B9)       p = QString("B9");
	else if (s == QPrinter::B10)      p = QString("B10");
	else if (s == QPrinter::C5E)      p = QString("C5E");
	else if (s == QPrinter::Comm10E)  p = QString("Comm10E");
	else if (s == QPrinter::DLE)      p = QString("DLE");
	else if (s == QPrinter::Executive)p = QString("Executive");
	else if (s == QPrinter::Folio)    p = QString("Folio");
	else if (s == QPrinter::Ledger)   p = QString("Ledger");
	else if (s == QPrinter::Legal)    p = QString("Legal");
	else if (s == QPrinter::Tabloid)  p = QString("Tabloid");
	return p;
}
#endif

bool TextEdit::canInsertFromMimeData(const QMimeData * m) const
{
	return (m->hasImage() || m->hasUrls() ||
		QTextEdit::canInsertFromMimeData(m));
}

void TextEdit::insertFromMimeData(const QMimeData * m)
{
	static int x = 1;
	if (m->hasImage())
	{
		QUrl u(QString("dropped_image_%1").arg(x++));
		dropImage(u, qvariant_cast<QImage>(m->imageData()));
	}
	else if (m->hasUrls())
	{
		foreach (QUrl u, m->urls())
		{
			QFileInfo fi(u.toLocalFile());
			if (QImageReader::supportedImageFormats()
					.contains(fi.suffix()
						.toLower()
						.toLatin1()))
			{
				dropImage(u, QImage(fi.filePath()));
			}
			else
			{
				dropTextFile(u);
			}
		}
	}
	else if (m->hasHtml())
	{
		QTextCursor      c  = textCursor();
		QTextBlockFormat bf = c.blockFormat();
		QTextCharFormat  cf = c.blockCharFormat();
		c.beginEditBlock();
		c.insertHtml(m->html());
		c.insertBlock(bf, cf);
		c.endEditBlock();
		setTextCursor(c);
	}
	else
	{
		QTextEdit::insertFromMimeData(m);
	}
#if 0
	set_paper_format(m_paper_format);
#endif
}

void TextEdit::dropImage(const QUrl & u, const QImage & i)
{
	if (i.isNull()) return;
	const double h = m_page_height_px -
		(m_page_y_px + m_margins_px_top + m_margins_px_bottom + m_page_b_px);
	const double w = m_page_width_px -
		(m_page_x_px + m_margins_px_left + m_margins_px_right + m_page_r_px);
	if (i.height() >= h || i.width() >= w)
	{
		QImage i2(i);
		if (i.height() >= (int)h && i.width() >= (int)w)
		{
			i2 = i2.scaledToHeight((int)h, Qt::SmoothTransformation);
			if (i2.width() >= (int)w)
			{
				i2 = i2.scaledToWidth((int)w, Qt::SmoothTransformation);
			}
		}
		else if (i.width() >= (int)w)
		{
			i2 = i2.scaledToWidth((int)w, Qt::SmoothTransformation);
		}
		else if (i.height() >= (int)h)
		{
			i2 = i2.scaledToHeight((int)h, Qt::SmoothTransformation);
		}
		document()->addResource(QTextDocument::ImageResource, u, i2);
		QTextCursor cursor = textCursor();
		cursor.beginEditBlock();
		cursor.insertImage(u.toString());
		cursor.endEditBlock();
		setTextCursor(cursor);
	}
	else
	{
		document()->addResource(QTextDocument::ImageResource, u, i);
		QTextCursor cursor = textCursor();
		cursor.beginEditBlock();
		cursor.insertImage(u.toString());
		cursor.endEditBlock();
		setTextCursor(cursor);
	}
}

void TextEdit::dropTextFile(const QUrl & u)
{
	QFile f(u.toLocalFile());
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextCursor cursor = textCursor();
		cursor.beginEditBlock();
		cursor.insertText(f.readAll());
		cursor.endEditBlock();
		setTextCursor(cursor);
	}
}

void TextEdit::insertImage(const QImage & i)
{
	static int x = 1;
	if (i.isNull()) return;
	QUrl u(QString("inserted_image_%1").arg(x++));
	const double h = m_page_height_px -
		(m_page_y_px + m_margins_px_top + m_margins_px_bottom + m_page_b_px);
	const double w = m_page_width_px -
		(m_page_x_px + m_margins_px_left + m_margins_px_right + m_page_r_px);
	if (i.height() >= h || i.width() >= w)
	{
		QImage i2(i);
		if (i.height() >= (int)h && i.width() >= (int)w)
		{
			i2 = i2.scaledToHeight((int)h, Qt::SmoothTransformation);
			if (i2.width() >= (int)w)
			{
				i2 = i2.scaledToWidth((int)w, Qt::SmoothTransformation);
			}
		}
		else if (i.width() >= (int)w)
		{
			i2 = i2.scaledToWidth((int)w, Qt::SmoothTransformation);
		}
		else if (i.height() >= (int)h)
		{
			i2 = i2.scaledToHeight((int)h, Qt::SmoothTransformation);
		}
		document()->addResource(QTextDocument::ImageResource, u, i2);
	}
	else
	{
		document()->addResource(QTextDocument::ImageResource, u, i);
	}
	QTextCursor cursor = textCursor();
	cursor.beginEditBlock();
	cursor.insertImage(u.toString());
	cursor.endEditBlock();
	setTextCursor(cursor);
}

void TextEdit::set_table_border_style(QTextFrameFormat::BorderStyle s)
{
	QTextCursor c = textCursor();
	QTextTable * t = c.currentTable();
	if (t)
	{
		QTextTableFormat f = t->format();
		if (f.isValid())
		{
			f.setBorderStyle(s);
			t->setFormat(f);
			emit cursorPositionChanged();
		}
	}
}

void TextEdit::text_alignleft(bool t)
{
	if (t) setAlignment(Qt::AlignLeft|Qt::AlignAbsolute);
}

void TextEdit::text_aligncenter(bool t)
{
	if (t) setAlignment(Qt::AlignHCenter);
}

void TextEdit::text_alignright(bool t)
{
	if (t) setAlignment(Qt::AlignRight|Qt::AlignAbsolute);
}

void TextEdit::text_alignjustify(bool t)
{
	if (t) setAlignment(Qt::AlignJustify);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void TextEdit::set_paper_format(QPageSize::PageSizeId x)
#else
void TextEdit::set_paper_format(QPrinter::PaperSize x)
#endif
{
	QSizeF page_size;
	{
		QPrinter p(QPrinter::HighResolution);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		p.setPageSize(QPageSize(x));
		p.setPageOrientation(m_paper_orientaion);
		const QRectF s    = p.paperRect(QPrinter::Millimeter);
#else
		p.setPaperSize(x);
		p.setOrientation(m_paper_orientaion);
		const QSizeF s    = p.paperSize(QPrinter::Millimeter);
#endif
		m_paper_width_px  = mm2px(s.width(),  true);
		m_paper_height_px = mm2px(s.height(), false);
		page_size         = QSizeF(m_paper_width_px, m_paper_height_px);
		QRectF page_r     = p.pageRect(QPrinter::Millimeter);
		m_page_width_px   = mm2px(page_r.width(),  true);
		m_page_height_px  = mm2px(page_r.height(), false);
		m_page_x_px       = mm2px(page_r.x(), true);  // printer margin top
		m_page_y_px       = mm2px(page_r.y(), false); // printer margin right
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		const QMarginsF margins = p.pageLayout().margins();
#if 0
		const qreal l = margins.left();
		const qreal t = margins.top();
#endif
		const qreal r = margins.right();
		const qreal b = margins.bottom();
#else
		qreal l, t, r, b;
		p.getPageMargins(&l, &t, &r, &b, QPrinter::Millimeter);
#endif
		m_page_r_px = mm2px(r, true);
		m_page_b_px = mm2px(b, false);
#if 0
		QRectF paper_r    = p.paperRect(QPrinter::Millimeter);
		std::cout << "Paper: "
			<< paper_r.width() <<  "mm "
			<< paper_r.height() << "mm, x="
			<< paper_r.x() << " y()=" << paper_r.y() << std::endl;
		std::cout << "Page:  "
			<< page_r.width()  <<  "mm "
			<< page_r.height()  << "mm, x="
			<< page_r.x()  << " y()=" << page_r.y()  << std::endl;
		std::cout << "Printer margins (mm) "
			<< l << " " << t << " " << r << " " << b << std::endl;
#endif
	}
	if (m_design_metrics) document()->setUseDesignMetrics(true);
	document()->setPageSize(page_size);
	document()->setDocumentMargin(0);
	document()->setIndentWidth(mm2px(1, true));
	QTextFrameFormat ff = document()->rootFrame()->frameFormat();
	ff.setLeftMargin(  m_margins_px_left   + m_page_x_px);
	ff.setRightMargin( m_margins_px_right  + m_page_r_px);
	ff.setTopMargin(   m_margins_px_top    + m_page_y_px);
	ff.setBottomMargin(m_margins_px_bottom + m_page_b_px);
	ff.setWidth(QTextLength(QTextLength::FixedLength, m_page_width_px));
	ff.setPadding(0);
	ff.setBorder(1);
	if (border__)
	{
		ff.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
		ff.setBorderBrush(Qt::darkGray);
	}
	else
	{
		ff.setBorderStyle(QTextFrameFormat::BorderStyle_None);
	}
	document()->rootFrame()->setFrameFormat(ff);
#if 0
	std::cout
		<< "leftMargin()="    << ff.leftMargin()
		<< " topMargin()="    << ff.topMargin()
		<< " rightMargin()="  << ff.rightMargin()
		<< " bottomMargin()=" << ff.bottomMargin()
		<< " padding()=" << ff.padding()
		<< " width().rawValue()=" << ff.width().rawValue() << std::endl;
#endif
}

void TextEdit::paintEvent(QPaintEvent * e)
{
	QTextEdit::paintEvent(e);
#if 1
	const qreal h = height();
	QPainter p(viewport());
	QPen pen(Qt::lightGray, 0);
	pen.setStyle(Qt::DotLine);
	p.setPen(pen);
	p.drawLine(m_paper_width_px, 0, m_paper_width_px, h);
#endif
#if 0
	const qreal w = m_page_x_px + m_margins_px_left;
	const qreal i = document()->indentWidth();
	QTextCursor c = textCursor();
	QTextBlockFormat bf = c.blockFormat();
	const qreal i0 = w + bf.indent()*i;
	const qreal i1 = i0 + bf.textIndent();
	QPen pen1(Qt::black, 0);
	pen1.setStyle(Qt::DotLine);
	p.setPen(pen1);
	p.drawLine(i0, 0, i0, h);
	p.drawLine(i1, 0, i1, h);
#endif
}

void TextEdit::resizeEvent(QResizeEvent * e)
{
	QTextEdit::resizeEvent(e);
	set_paper_format(m_paper_format);
}

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
void TextEdit::print(QPagedPaintDevice * p)
#else
void TextEdit::print(QPrinter * p)
#endif
{
	border__ = false;
	set_paper_format(m_paper_format);
	QTextEdit::print(p);
	border__ = true;
	set_paper_format(m_paper_format);
}

void TextEdit::merge_format(
	const QTextCharFormat & f)
{
	QTextCursor cursor = textCursor();
#if 1
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
#endif
	cursor.mergeCharFormat(f);
	setTextCursor(cursor);
	mergeCurrentCharFormat(f);
}

void TextEdit::update_settings()
{
	set_paper_format(m_paper_format);
	repaint();
}

void TextEdit::preview_print(QPrinter * p)
{
	this->print(p);
}

void TextEdit::trigger_print_preview()
{
	QPrinter * p = new QPrinter(QPrinter::HighResolution);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	p->setPageSize(QPageSize(m_paper_format));
	p->setPageOrientation(m_paper_orientaion);
#else
	p->setPageSize(m_paper_format);
	p->setOrientation(m_paper_orientaion);
#endif
	QPrintPreviewDialog * preview = new QPrintPreviewDialog(p);
	connect(
		preview, SIGNAL(paintRequested(QPrinter*)),
		this,    SLOT(preview_print(QPrinter*)));
	preview->exec();
	delete preview;
	delete p;
}

void TextEdit::update_left_margin(int x)
{
	m_margins_mm_left = (qreal)x;
	m_margins_px_left = TextEdit::mm2px(m_margins_mm_left, true);
	set_paper_format(m_paper_format);
	repaint();
}

void TextEdit::update_top_margin(int x)
{
	m_margins_mm_top = (qreal)x;
	m_margins_px_top = TextEdit::mm2px(m_margins_mm_top, false);
	set_paper_format(m_paper_format);
	repaint();
}

void TextEdit::update_right_margin(int x)
{
	m_margins_mm_right = (qreal)x;
	m_margins_px_right = TextEdit::mm2px(m_margins_mm_right, true);
	set_paper_format(m_paper_format);
	repaint();
}

void TextEdit::update_bottom_margin(int x)
{
	m_margins_mm_bottom = (qreal)x;
	m_margins_px_bottom = TextEdit::mm2px(m_margins_mm_bottom, false);
	set_paper_format(m_paper_format);
	repaint();
}

void TextEdit::set_default_font(const QString & f)
{
	m_font_family = f;
}

void TextEdit::set_default_font_pt(double x)
{
	m_font_size = x;
}

void TextEdit::set_indent(int x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setIndent(x);
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_text_indent(double x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setTextIndent(TextEdit::mm2px(x, true));
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_block_left_margin(double x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setLeftMargin(TextEdit::mm2px(x, true));
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_block_top_margin(double x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setTopMargin(TextEdit::mm2px(x, false));
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_block_right_margin(double x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setRightMargin(TextEdit::mm2px(x, true));
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_block_bottom_margin(double x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setBottomMargin(TextEdit::mm2px(x, false));
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_block_line_height(int x)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	if (x == 100)
	{
		bf.setLineHeight(0, QTextBlockFormat::SingleHeight);
	}
	else
	{
		bf.setLineHeight((qreal)x, QTextBlockFormat::ProportionalHeight);
	}
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::reset_block()
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextBlockFormat bf = c.blockFormat();
	bf.setIndent(0);
	bf.setTextIndent(0);
	bf.setLeftMargin(0);
	bf.setTopMargin(0);
	bf.setRightMargin(0);
	bf.setBottomMargin(0);
	bf.setLineHeight(0, QTextBlockFormat::SingleHeight);
	c.mergeBlockFormat(bf);
	c.endEditBlock();
	emit cursorPositionChanged();
}

void TextEdit::set_design_metrics(bool t)
{
	m_design_metrics = t;
	document()->setUseDesignMetrics(m_design_metrics);
}

void TextEdit::insert_table(int rows, int cols)
{
	QTextCursor c = textCursor();
	c.beginEditBlock();
	QTextTableFormat f;
	QVector<QTextLength> v;
	const qreal tmp0 = 100.0/cols;
	for (int x = 0; x < cols; ++x)
	{
		v.push_back(QTextLength(QTextLength::PercentageLength, tmp0));
	}
	f.setCellPadding(4);
	f.setColumnWidthConstraints(v);
	c.insertTable(rows, cols, f);
	c.endEditBlock();
	setTextCursor(c);
	emit cursorPositionChanged();
}

void TextEdit::set_paper_size(const QString & s)
{
	m_paper_format = TextEdit::pagesize_from_qstring(s);
	update_settings();
}

void TextEdit::set_paper_orientation(const QString & s)
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	QPageLayout::Orientation po = QPageLayout::Portrait;
	if (s == QString("Landscape")) po = QPageLayout::Landscape;
#else
	QPrinter::Orientation po = QPrinter::Portrait;
	if (s == QString("Landscape")) po = QPrinter::Landscape;
#endif
	m_paper_orientaion = po;
	update_settings();
}

void TextEdit::readSettings()
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
	const double m_mm_l = settings.value(QString("margins_mm_left"),  15.0).toDouble();
	const double m_mm_t = settings.value(QString("margins_mm_top"),   15.0).toDouble();
	const double m_mm_r = settings.value(QString("margins_mm_right"), 10.0).toDouble();
	const double m_mm_b = settings.value(QString("margins_mm_bottom"),15.0).toDouble();
#if 0
	const int design_metrics = settings.value(QString("design_metrics"), 1).toInt();
#endif
	settings.endGroup();
	m_font_family = font_family.trimmed();
	m_font_size = font_size;
#if 0
	m_design_metrics = (design_metrics == 1);
#else
	m_design_metrics = true;
#endif
	m_paper_format = pagesize_from_qstring(ps);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	if (po == QString("Landscape"))
		m_paper_orientaion = QPageLayout::Landscape;
	else
		m_paper_orientaion = QPageLayout::Portrait;
#else
	if (po == QString("Landscape"))
		m_paper_orientaion = QPrinter::Landscape;
	else
		m_paper_orientaion = QPrinter::Portrait;
#endif
	m_margins_mm_left   = m_mm_l;
	m_margins_mm_top    = m_mm_t;
	m_margins_mm_right  = m_mm_r;
	m_margins_mm_bottom = m_mm_b;
	m_margins_px_left   = mm2px(m_mm_l, true);
	m_margins_px_top    = mm2px(m_mm_t, false);
	m_margins_px_right  = mm2px(m_mm_r, true);
	m_margins_px_bottom = mm2px(m_mm_b, false);
}

void TextEdit::writeSettings(QSettings & settings)
{
	QString p = qstring_from_pagesize(m_paper_format);
	QString po("Portrait");
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	if (m_paper_orientaion == QPageLayout::Landscape)
		po = QString("Landscape");
#else
	if (m_paper_orientaion == QPrinter::Landscape)
		po = QString("Landscape");
#endif
	settings.beginGroup(QLatin1String("TextEdit"));
	settings.setValue(QString("font_family"),      QVariant(m_font_family));
	settings.setValue(QString("font_size"),        QVariant(m_font_size));
	settings.setValue(QString("page_size"),        QVariant(p));
	settings.setValue(QString("page_orient"),      QVariant(po));
	settings.setValue(QString("margins_mm_left"),  QVariant(m_margins_mm_left));
	settings.setValue(QString("margins_mm_top"),   QVariant(m_margins_mm_top));
	settings.setValue(QString("margins_mm_right"), QVariant(m_margins_mm_right));
	settings.setValue(QString("margins_mm_bottom"),QVariant(m_margins_mm_bottom));
#if 0
	settings.setValue(QString("design_metrics"),   m_design_metrics ? QVariant((int)1) : QVariant((int)0));
#endif
	settings.endGroup();
}

void TextEdit::contextMenuEvent(QContextMenuEvent * e)
{
	e->accept();
}
