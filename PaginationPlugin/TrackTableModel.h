#ifndef TRACKTABLEMODEL_H
#define TRACKTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <memory>

class TrackTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    using PageDataPtr = std::unique_ptr<QList<QList<QVariant>>>;

    explicit TrackTableModel(QObject *parent = nullptr);

    void setTracks(PageDataPtr tracks);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    const QVector<QString> headers
        {
            "Track ID", "Name", "Album ID", "Media Type ID",
            "Genre ID", "Composer", "Milliseconds", "Bytes", "Unit Price"
        };

    std::unique_ptr<QList<QList<QVariant>>> tracks;  // Хранение данных в unique_ptr
};

#endif // TRACKTABLEMODEL_H
