#ifndef KDATABASE_H
#define KDATABASE_H

#include <QObject>

class kDatabase : public QObject
{
    Q_OBJECT
public:
    explicit kDatabase(QObject *parent = nullptr);

signals:

};

#endif // KDATABASE_H
