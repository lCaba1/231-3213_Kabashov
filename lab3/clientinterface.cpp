#include "clientinterface.h"

ClientInterface *ClientInterface::instance = nullptr;

ClientInterface::ClientInterface(QObject *parent)
    : QObject{parent}
{
    m_client = new QNetworkAccessManager(this);
}

void ClientInterface::responseReceived(QNetworkReply *reply) {
    qDebug() << "Server response: " << reply->readAll();
}

ClientInterface::~ClientInterface() {
    m_client->deleteLater();
}

ClientInterface *ClientInterface::getInstance()
{
    if(!instance)
        instance = new ClientInterface();

    return instance;

}

Order ClientInterface::getOrder(const int id) const
{
    QString request(QString("http://localhost/orders/%1/").arg(id));
    QNetworkReply* reply = m_client->get(QNetworkRequest(QUrl(request)));

    QEventLoop evtLoop;
    QObject::connect(
        m_client, &QNetworkAccessManager::finished,
        &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ошибка:" << reply->errorString();
        return Order();
    }

    QByteArray responseData = reply->readAll();

    // Парсим JSON
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
    QJsonObject jsonObj = jsonDoc.object();

    // Извлекаем данные из JSON
    int orderId = jsonObj["id"].toInt();
    int orderUser = jsonObj["user"].toInt();
    QString orderBase = jsonObj["base"].toString();
    QString orderQuote = jsonObj["quote"].toString();
    QString orderSide = jsonObj["side"].toString();
    double orderSize = jsonObj["size"].toDouble();
    double orderPrice = jsonObj["price"].toDouble();
    bool orderIsProcessed = jsonObj["is_processed"].toBool();
    QDateTime orderOpenedAt = QDateTime::fromString(jsonObj["opened_at"].toString(), Qt::ISODate);

    Order order(
        orderId,
        orderUser,
        orderBase,
        orderQuote,
        orderSide,
        orderSize,
        orderPrice,
        orderIsProcessed,
        orderOpenedAt
    );

    return order;
}

QList<Order> ClientInterface::getOrders() const
{
    QList<Order> ordersList;

    QString request("http://localhost/orders/");
    QNetworkReply* reply = m_client->get(QNetworkRequest(QUrl(request)));

    QEventLoop evtLoop;
    QObject::connect(
        m_client, &QNetworkAccessManager::finished,
        &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ошибка:" << reply->errorString();
        return ordersList; // Возвращаем пустой список
    }

    // Чтение и парсинг JSON
    QByteArray responseData = reply->readAll();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);
    QJsonArray jsonArray = jsonDoc.array();

    // Обработка каждого элемента массива
    for (const QJsonValue &value : jsonArray) {
        QJsonObject jsonObj = value.toObject();

        // Извлечение данных из JSON
        int orderId = jsonObj["id"].toInt();
        int orderUser = jsonObj["user"].toInt();
        QString orderBase = jsonObj["base"].toString();
        QString orderQuote = jsonObj["quote"].toString();
        QString orderSide = jsonObj["side"].toString();
        double orderSize = jsonObj["size"].toDouble();
        double orderPrice = jsonObj["price"].toDouble();
        bool orderIsProcessed = jsonObj["is_processed"].toBool();
        QDateTime orderOpenedAt = QDateTime::fromString(jsonObj["opened_at"].toString(), Qt::ISODate);

        // Создание объекта Order и добавление в список
        ordersList.append(Order(
            orderId,
            orderUser,
            orderBase,
            orderQuote,
            orderSide,
            orderSize,
            orderPrice,
            orderIsProcessed,
            orderOpenedAt
        ));
    }

    qDebug() << "Retrieved" << ordersList.size() << "orders";
    return ordersList;
}

Order ClientInterface::createOrder(Order& order)
{
    QString requestUrl("http://localhost/orders/");

    // 1. Создаем multiPart
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // 2. Добавляем части формы
    auto addPart = [&](const QString& name, const QString& value) {
        QHttpPart part;
        part.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"" + name + "\""));
        part.setBody(value.toUtf8());
        multiPart->append(part);
    };

    addPart("user", QString::number(order.user));
    addPart("base", order.base);
    addPart("quote", order.quote);
    addPart("side", order.side);
    addPart("size", QString::number(order.size));
    addPart("price", QString::number(order.price));
    addPart("is_processed", order.is_processed ? "true" : "false");
    addPart("opened_at", order.opened_at.toString(Qt::ISODate));

    // 3. Настраиваем запрос
    QNetworkRequest request;
    request.setUrl(QUrl(requestUrl));
    request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + multiPart->boundary());

    // 4. Отправляем запрос
    QNetworkReply* reply = m_client->sendCustomRequest(request, "POST", multiPart);
    multiPart->setParent(reply); // Важно для управления памятью

    // 5. Ожидаем ответа
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 6. Обработка результата
    Order result;
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            order.id = jsonObj["id"].toInt();
            result = order;
        }
    }

    return result;
}

Order ClientInterface::updateOrder(Order &order)
{
    QString requestUrl(QString("http://localhost/orders/%1/").arg(order.id));

    // 1. Создаем multiPart
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // 2. Добавляем части формы
    auto addPart = [&](const QString& name, const QString& value) {
        QHttpPart part;
        part.setHeader(QNetworkRequest::ContentDispositionHeader,
            QVariant("form-data; name=\"" + name + "\""));
        part.setBody(value.toUtf8());
        multiPart->append(part);
    };

    addPart("id", QString::number(order.id));
    addPart("user", QString::number(order.user));
    addPart("base", order.base);
    addPart("quote", order.quote);
    addPart("side", order.side);
    addPart("size", QString::number(order.size));
    addPart("price", QString::number(order.price));
    addPart("is_processed", order.is_processed ? "true" : "false");
    addPart("opened_at", order.opened_at.toString(Qt::ISODate));

    // 3. Настраиваем запрос
    QNetworkRequest request;
    request.setUrl(QUrl(requestUrl));
    request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + multiPart->boundary());

    // 4. Отправляем запрос
    QNetworkReply* reply = m_client->sendCustomRequest(request, "PATCH", multiPart);
    multiPart->setParent(reply); // Важно для управления памятью

    // 5. Ожидаем ответа
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 6. Обработка результата
    Order result;
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            order.id = jsonObj["id"].toInt();
            result = order;
        }
    }

    return result;
}

void ClientInterface::deleteOrder(const int id) const
{
    QString requestUrl = QString("http://localhost/orders/%1/").arg(id);

    QNetworkReply* reply = m_client->deleteResource(QNetworkRequest(QUrl(requestUrl)));

    QEventLoop evtLoop;
    QObject::connect(m_client, &QNetworkAccessManager::finished, &evtLoop, &QEventLoop::quit);
    evtLoop.exec();

    QByteArray responseData = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ошибка при удалении:" << reply->errorString();
        return;
    }

    qDebug() << "Успешно удалено";
}
