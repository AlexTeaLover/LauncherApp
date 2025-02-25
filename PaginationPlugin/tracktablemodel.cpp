#include "tracktablemodel.h"

TrackTableModel::TrackTableModel(QObject *parent)
    : QAbstractTableModel{parent}, tracks(std::make_unique<QList<QList<QVariant>>>())
{
}

void TrackTableModel::setTracks(PageDataPtr newTracks)
{
    beginResetModel();
    tracks = std::move(newTracks);
    endResetModel();
}

int TrackTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tracks ? tracks->size() : 0;
}

int TrackTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headers.size();
}

QVariant TrackTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || !tracks)
        return QVariant();

    return (*tracks)[index.row()][index.column()];
}

QVariant TrackTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && section >= 0 && section < headers.size())
    {
        return headers[section];
    }

    if (orientation == Qt::Vertical)
    {
        return QString::number(section + 1);
    }

    return QVariant();
}
