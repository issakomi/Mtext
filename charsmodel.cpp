#include "charsmodel.h"
#include <QtGlobal>

CharsModel::CharsModel()
{
	rows_count = 0;
	fg_color = QColor(0,0,0,255);
	bg_color = QColor(255,255,255,255);
}

CharsModel::~CharsModel()
{
}

void CharsModel::set_range(
	const unsigned int from,
	const unsigned int to,
	const bool symbola)
{
	model_data.clear();
	rows_count = 0;
	for (unsigned int x = from; x < to; x += 16)
	{
		QMap<int, SymbolInfo> m;	
		for (int z = 0; z < 16; ++z)
		{
			bool set_empty = false;
			unsigned int k = x+z;
			const QString code =
				QString("%1").arg(k, 5, 16, QChar('0')).toUpper();
			if (k < 0x20)
			{
				k += 0x2400;
			}
			else if (k == 0x20)
			{
				k = 0x2420;
			}
			else if (k == 0x7F)
			{
				k = 0x2421;
			}
			else if (k >= 0x80 && k <= 0xA0)
			{
				if (k == 0x80 || k == 0x81)
				{
					if (symbola) k = 0xF0080;
					else set_empty = true;
				}
				else
				{
					if (symbola) k += 0xF0000;
					else set_empty = true;
				}
			}
			else if (k >= 0x02000 && k <= 0x0206F)
			{
				if ((k >= 0x02000 && k <= 0x0200F) ||
					k == 0x02011                   ||
					(k >= 0x02028 && k <= 0x0202F) ||
					(k >= 0x0205F && k <= 0x02064) ||
					(k >= 0x02066 && k <= 0x0206F))
				{
					if (symbola) k += 0xF0000;
					else set_empty = true;
				}
			}
			else if (k >= 0x01D100 && k <= 0x1D1FF)
			{
				if (k == 0x1D159 || (k >= 0x1D173 && k <= 0x1D17A))
				{
					if (symbola) k += 0xE0000;
					else set_empty = true;
				}
			}
			if (set_empty)
			{
				m[z] = SymbolInfo(QString(""), code);
			}
			else
			{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
				m[z] = SymbolInfo(QString::fromUcs4(reinterpret_cast<char32_t*>(&k), 1), code);
#else
				m[z] = SymbolInfo(QString::fromUcs4(&k, 1), code);
#endif
			}
		}
		model_data << m;
		rows_count++;
	}
}

void CharsModel::set_fg_color(const QColor x)
{
	fg_color = x;
}

void CharsModel::set_bg_color(const QColor x)
{
	bg_color = x;
}

QColor CharsModel::get_fg_color() const
{
	return fg_color;
}

QColor CharsModel::get_bg_color() const
{
	return bg_color;
}

int CharsModel::rowCount(const QModelIndex &) const
{
	return rows_count;
}

int CharsModel::columnCount(const QModelIndex &) const
{
	return 16;
}

QVariant CharsModel::data(const QModelIndex & index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		const SymbolInfo & s =
			model_data.at(index.row()).value(index.column());
		return QVariant(s.symbol);
	}
	else if (role == Qt::ToolTipRole)
	{
		const SymbolInfo & s =
			model_data.at(index.row()).value(index.column());
		return QVariant(s.code);
	}
	else if (role == Qt::ForegroundRole)
	{
		return QVariant(fg_color);
	}
	else if (role == Qt::BackgroundRole)
	{
		return QVariant(bg_color);
	}
	return QVariant();
}

