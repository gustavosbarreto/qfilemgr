#ifndef _PLACESMODEL_HPP
#define _PLACESMODEL_HPP

#include <QAbstractItemModel>

class HalDevice;
class Place;

class PlacesModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles
    {
        PathRole = Qt::UserRole + 1,
        TypeRole,
        DataRole
    };

    enum Type
    {
        LocalDirectoryType,
        VolumeType
    };

    PlacesModel(QObject *parent = 0);

    QString currentPlacePath() const;

    QModelIndex index(const QString &path) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void updateCurrentPlacePath(const QModelIndex &index);
//    void deviceMounted(HalDevice *device);
//    void deviceUnmounted(HalDevice *device);

private:
    QList<Place *> m_places;
    QString m_currentPlacePath;

signals:
    void currentPlacePathChanged(const QString &);
};

#endif
