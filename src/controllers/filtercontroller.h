/*
 * Copyright (c) 2014-2018 Meltytech, LLC
 * Author: Brian Matherly <code@brianmatherly.com>
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

#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QFuture>
#include "models/metadatamodel.h"
#include "models/attachedfiltersmodel.h"
#include "qmltypes/qmlmetadata.h"
#include "qmltypes/qmlfilter.h"

class QTimerEvent;


class FilterController : public QObject
{
    Q_OBJECT

public:
    explicit FilterController(QObject* parent = 0);
    MetadataModel* metadataModel();
    AttachedFiltersModel* attachedModel();

    QmlMetadata* metadataForService(Mlt::Service *service);
    QmlFilter* currentFilter() const { return m_currentFilter.data(); }

protected:
    void timerEvent(QTimerEvent*);

signals:
    void currentFilterAboutToChange();
    void currentFilterChanged(QmlFilter* filter, QmlMetadata* meta, int index);
    void statusChanged(QString);
    void filterChanged(Mlt::Filter*);

public slots:
    void setProducer(Mlt::Producer *producer = 0);
    void setCurrentFilter(int attachedIndex, bool isNew = false);
    void onFadeInChanged();
    void onFadeOutChanged();
    void onFilterInChanged(int delta, Mlt::Filter* filter = 0);
    void onFilterOutChanged(int delta, Mlt::Filter* filter = 0);

private slots:
    void handleAttachedModelChange();
    void handleAttachedModelAboutToReset();
    void addMetadata(QmlMetadata*);
    void handleAttachedRowsRemoved(const QModelIndex & parent, int first, int last);
    void handleAttachedRowsInserted(const QModelIndex & parent, int first, int last);
    void handleAttachDuplicateFailed(int index);
    void onQmlFilterChanged();

private:
    void loadFilterMetadata();

    QFuture<void> m_future;
    QScopedPointer<QmlFilter> m_currentFilter;
    Mlt::Filter* m_mltFilter;
    MetadataModel m_metadataModel;
    AttachedFiltersModel m_attachedModel;
    int m_currentFilterIndex;
};

#endif // FILTERCONTROLLER_H
