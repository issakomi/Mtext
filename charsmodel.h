#ifndef CHARSMODEL_H
#define CHARSMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QList>
#include <QMap>
#include <QColor>

class SymbolInfo
{
public:
	SymbolInfo() : symbol(QString("")), code(QString("")) {}
	SymbolInfo(const QString & s, const QString & c) :
		symbol(s),
		code(c) {}
	~SymbolInfo() {}
	QString symbol;
	QString code;
	SymbolInfo& operator=(const SymbolInfo & o)
	{
		symbol = o.symbol;
		code   = o.code;
		return *this;
	}
};

class CharsModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	CharsModel();
	virtual ~CharsModel() override;
	void set_range(const unsigned int, const unsigned int, const bool);
	void set_fg_color(const QColor);
	void set_bg_color(const QColor);
	QColor get_fg_color() const;
	QColor get_bg_color() const;
	int rowCount(const QModelIndex(&) = QModelIndex()) const override;
	int columnCount(const QModelIndex(&) = QModelIndex()) const override;
	QVariant data(const QModelIndex&, int = Qt::DisplayRole) const override;

private:
	QList< QMap<int, SymbolInfo> > model_data;
	int rows_count;
	QColor fg_color;
	QColor bg_color;
};

#endif // CHARSMODEL_H
