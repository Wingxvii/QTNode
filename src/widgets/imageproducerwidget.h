/*
 * Copyright (c) 2012-2017 Meltytech, LLC
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

#ifndef IMAGEPRODUCERWIDGET_H
#define IMAGEPRODUCERWIDGET_H

#include <QWidget>
#include "abstractproducerwidget.h"
#include "mltcontroller.h"

namespace Ui {
    class ImageProducerWidget;
}

class ImageProducerWidget : public QWidget, public AbstractProducerWidget
{
    Q_OBJECT

public:
    explicit ImageProducerWidget(QWidget *parent = 0);
    ~ImageProducerWidget();

    // AbstractProducerWidget overrides
    Mlt::Producer* newProducer(Mlt::Profile&);
    virtual void setProducer(Mlt::Producer*);

signals:
    void producerChanged(Mlt::Producer*);
    void producerReopened();

public slots:
    void setOutPoint(int duration);

private slots:
    void on_resetButton_clicked();

    void on_aspectNumSpinBox_valueChanged(int );

    void on_aspectDenSpinBox_valueChanged(int );

    void on_durationSpinBox_editingFinished();

    void on_sequenceCheckBox_clicked(bool checked);

    void on_repeatSpinBox_editingFinished();

    void on_defaultDurationButton_clicked();
    
private:
    Ui::ImageProducerWidget *ui;
    int m_defaultDuration;

    void reopen(Mlt::Producer* p);
    void recreateProducer();
};

#endif // IMAGEPRODUCERWIDGET_H
