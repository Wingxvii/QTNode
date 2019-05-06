/*
 * Copyright (c) 2014-2016 Meltytech, LLC
 * Author: Brian Matherly <pez4brian@yahoo.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QMLFILE_H
#define QMLFILE_H

#include <QObject>
#include <QUrl>

class QmlFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString fileName READ getFileName)
    Q_PROPERTY(QString path READ getPath)

public:
    explicit QmlFile(QObject *parent = 0);
    QString getUrl();
    void setUrl(const QString& text);
    QString getFileName();
    QString getPath();
    Q_INVOKABLE void copyFromFile(QString source);
    Q_INVOKABLE bool exists();
    Q_INVOKABLE QString suffix();

signals:
    void urlChanged(const QUrl &url);

private:
    QUrl m_url;
};

#endif // QMLFILE_H
