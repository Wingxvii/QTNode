/*
 * Copyright (c) 2013-2018 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
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

#ifndef QMLMETADATA_H
#define QMLMETADATA_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QUrl>
#include <QQmlListProperty>

class QmlKeyframesParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name NOTIFY changed)
    Q_PROPERTY(QString property MEMBER m_property NOTIFY changed)
    /// If isSimple this parameter cannot use full keyframes while simple is in use.
    Q_PROPERTY(bool isSimple MEMBER m_isSimple NOTIFY changed)
    Q_PROPERTY(bool isCurve MEMBER m_isCurve NOTIFY changed)
    Q_PROPERTY(double minimum MEMBER m_minimum NOTIFY changed)
    Q_PROPERTY(double maximum MEMBER m_maximum NOTIFY changed)

public:
    explicit QmlKeyframesParameter(QObject* parent = 0);

    QString name() const { return m_name; }
    QString property() const { return m_property; }
    bool isSimple() const { return m_isSimple; }
    bool isCurve() const { return m_isCurve; }
    double minimum() const { return m_minimum; }
    double maximum() const { return m_maximum; }

signals:
    void changed();

private:
    QString m_name;
    QString m_property;
    bool m_isSimple;
    bool m_isCurve;
    double m_minimum;
    double m_maximum;
};

class QmlKeyframesMetadata : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool allowTrim MEMBER m_allowTrim NOTIFY changed)
    Q_PROPERTY(bool allowAnimateIn MEMBER m_allowAnimateIn NOTIFY changed)
    Q_PROPERTY(bool allowAnimateOut MEMBER m_allowAnimateOut NOTIFY changed)
    Q_PROPERTY(QQmlListProperty<QmlKeyframesParameter> parameters READ parameters NOTIFY changed)

public:
    explicit QmlKeyframesMetadata(QObject *parent = 0);

    bool allowTrim() const { return m_allowTrim; }
    bool allowAnimateIn() const { return m_allowAnimateIn; }
    bool allowAnimateOut() const { return m_allowAnimateOut; }

    QQmlListProperty<QmlKeyframesParameter> parameters() { return QQmlListProperty<QmlKeyframesParameter>(this, m_parameters); }
    int parameterCount() const { return m_parameters.count(); }
    QmlKeyframesParameter *parameter(int index) const { return m_parameters[index]; }

signals:
    void changed();

private:
    bool m_allowTrim;
    bool m_allowAnimateIn;
    bool m_allowAnimateOut;
    QList<QmlKeyframesParameter *> m_parameters;
};


class QmlMetadata : public QObject
{
    Q_OBJECT
    Q_ENUMS(PluginType)
    Q_PROPERTY(PluginType type READ type WRITE setType)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY changed)
    Q_PROPERTY(QString mlt_service READ mlt_service WRITE set_mlt_service)
    Q_PROPERTY(bool needsGPU READ needsGPU WRITE setNeedsGPU NOTIFY changed)
    Q_PROPERTY(QString qml READ qmlFileName WRITE setQmlFileName)
    Q_PROPERTY(QString vui READ vuiFileName WRITE setVuiFileName)
    Q_PROPERTY(QUrl qmlFilePath READ qmlFilePath )
    Q_PROPERTY(QUrl vuiFilePath READ vuiFilePath )
    Q_PROPERTY(bool isAudio READ isAudio WRITE setIsAudio NOTIFY changed)
    Q_PROPERTY(bool isHidden READ isHidden WRITE setIsHidden NOTIFY changed)
    Q_PROPERTY(bool isFavorite READ isFavorite WRITE setIsFavorite NOTIFY changed)
    Q_PROPERTY(QString gpuAlt READ gpuAlt WRITE setGpuAlt NOTIFY changed)
    Q_PROPERTY(bool allowMultiple READ allowMultiple WRITE setAllowMultiple)
    Q_PROPERTY(bool isClipOnly READ isClipOnly WRITE setIsClipOnly)
    Q_PROPERTY(bool isGpuCompatible READ isGpuCompatible() WRITE setIsGpuCompatible)
    Q_PROPERTY(QmlKeyframesMetadata* keyframes READ keyframes NOTIFY changed)

public:
    enum PluginType {
        Filter,
        Producer,
        Transition
    };

    explicit QmlMetadata(QObject *parent = 0);
    void loadSettings();

    PluginType type() const { return m_type; }
    void setType(PluginType);
    QString name() const { return m_name; }
    void setName(const QString&);
    QString mlt_service() const { return m_mlt_service; }
    void set_mlt_service(const QString&);
    QString uniqueId() const;
    bool needsGPU() const { return m_needsGPU; }
    void setNeedsGPU(bool);
    QString qmlFileName() const { return m_qmlFileName; }
    void setQmlFileName(const QString&);
    QString vuiFileName() const { return m_vuiFileName; }
    void setVuiFileName(const QString&);
    QDir path() const { return m_path; }
    void setPath(const QDir& path);
    QUrl qmlFilePath() const;
    QUrl vuiFilePath() const;
    bool isAudio() const { return m_isAudio; }
    void setIsAudio(bool isAudio);
    bool isHidden() const { return m_isHidden; }
    void setIsHidden(bool isHidden);
    bool isFavorite() const { return m_isFavorite; }
    void setIsFavorite(bool isFavorite);
    QString gpuAlt() const { return m_gpuAlt; }
    void setGpuAlt(const QString&);
    bool allowMultiple() const { return m_allowMultiple; }
    void setAllowMultiple(bool allowMultiple);
    bool isClipOnly() const { return m_isClipOnly; }
    void setIsClipOnly(bool isClipOnly);
    bool isGpuCompatible() const { return m_isGpuCompatible; }
    void setIsGpuCompatible(bool isCompatible) { m_isGpuCompatible = isCompatible; }
    QmlKeyframesMetadata* keyframes() { return &m_keyframes; }

signals:
    void changed();

private:
    PluginType m_type;
    QString m_name;
    QString m_mlt_service;
    bool m_needsGPU;
    QString m_qmlFileName;
    QString m_vuiFileName;
    QDir m_path;
    bool m_isAudio;
    bool m_isHidden;
    bool m_isFavorite;
    QString m_gpuAlt;
    bool m_allowMultiple;
    bool m_isClipOnly;
    bool m_isGpuCompatible;
    QmlKeyframesMetadata m_keyframes;
};

#endif // QMLMETADATA_H
