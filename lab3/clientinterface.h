#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QHttpPart>

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>
#include <QString>

#include <QStandardPaths>
#include <QAbstractNetworkCache>
#include <QNetworkDiskCache>

#include <QDir>

class Order {
public:
    int id = -1;
    int user;
    QString base;
    QString quote;
    QString side;
    double size;
    double price;
    bool is_processed;
    QDateTime opened_at;

    Order() = default;

    Order(
        int id,
        int user,
        const QString &base,
        const QString &quote,
        const QString &side,
        double size,
        double price,
        bool is_processed,
        const QDateTime &opened_at
    ) {
        this->id = id;
        this->user = user;
        this->base = base;
        this->quote = quote;
        this->side = side;
        this->size = size;
        this->price = price;
        this->is_processed = is_processed;
        this->opened_at = opened_at;
    }

    void display() {
        qDebug() << "id: " << id;
        qDebug() << "user: " << user;
        qDebug() << "base: " << base;
        qDebug() << "quote: " << quote;
        qDebug() << "side: " << side;
        qDebug() << "size: " << size;
        qDebug() << "price: " << price;
        qDebug() << "is_processed: " << is_processed;
        qDebug() << "opened_at: " << opened_at.toString();
        qDebug() << '\n';
    }
private:
};

class ClientInterface : public QObject
{
    Q_OBJECT
public:
    ~ClientInterface();
    static ClientInterface * getInstance();
    static ClientInterface * instance ;

    Order getOrder(const int id) const;
    QList<Order> getOrders() const;
    Order createOrder(Order& order);
    Order updateOrder(Order& order);
    void deleteOrder(const int id) const;

signals:
public slots:
    void responseReceived(QNetworkReply *reply);

private:
    explicit ClientInterface(QObject *parent = nullptr);

    Q_DISABLE_COPY(ClientInterface)

    QNetworkAccessManager * m_client = nullptr;
};

#endif // CLIENTINTERFACE_H
