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

#ifndef TIMELINEDOCK_H
#define TIMELINEDOCK_H

#include <QDockWidget>
#include <QQuickWidget>
#include <QApplication>
#include "models/multitrackmodel.h"
#include "sharedframe.h"

namespace Ui {
class TimelineDock;
}
namespace Timeline {
class UpdateCommand;
class TrimCommand;
}
class UndoHelper;

class TimelineDock : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(QList<int> selection READ selection WRITE setSelection NOTIFY selectionChanged)

public:
    explicit TimelineDock(QWidget *parent = 0);
    ~TimelineDock();

    enum TrimLocation {
        TrimInPoint,
        TrimOutPoint
    };

    MultitrackModel* model() { return &m_model; }
    int position() const { return m_position; }
    void setPosition(int position);
    Mlt::ClipInfo* getClipInfo(int trackIndex, int clipIndex);
    Mlt::Producer* producerForClip(int trackIndex, int clipIndex);
    int clipIndexAtPlayhead(int trackIndex = -1);
    int clipIndexAtPosition(int trackIndex, int position);
    void chooseClipAtPosition(int position, int * trackIndex, int * clipIndex);
    void setCurrentTrack(int currentTrack);
    int currentTrack() const;
    int clipCount(int trackIndex) const;
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void makeTracksShorter();
    void makeTracksTaller();
    void setSelection(QList<int> selection = QList<int>(), int trackIndex = -1, bool isMultitrack = false);
    QList<int> selection() const;
    void saveAndClearSelection();
    void restoreSelection();
    void selectClipUnderPlayhead();
    int centerOfClip(int trackIndex, int clipIndex);
    bool isTrackLocked(int trackIndex) const;
    void trimClipAtPlayhead(TrimLocation location, bool ripple);
    bool isRipple() const;
    Q_INVOKABLE bool isMultitrackSelected() const { return m_selection.isMultitrackSelected; }
    Q_INVOKABLE int selectedTrack() const { return m_selection.selectedTrack; }

signals:
    void currentTrackChanged();
    void selectionChanged();
    void seeked(int position);
    void positionChanged();
    void clipOpened(Mlt::Producer* producer);
    void dragging(const QPointF& pos, int duration);
    void dropped();
    void dropAccepted(const QString &xml);
    void fadeInChanged(int duration);
    void fadeOutChanged(int duration);
    void selected(Mlt::Producer* producer);
    void clipClicked();
    void showStatusMessage(QString);
    void clipCopied();
    void clipMoved(int fromTrack, int toTrack, int clipIndex, int position);
    void filteredClicked();

public slots:
    void addAudioTrack();
    void addVideoTrack();
    void onShowFrame(const SharedFrame& frame);
    void onSeeked(int position);
    void append(int trackIndex);
    void remove(int trackIndex, int clipIndex);
    bool mergeClipWithNext(int trackIndex, int clipIndex, bool dryrun);
    void lift(int trackIndex, int clipIndex);
    void removeSelection(bool withCopy = false);
    void liftSelection();
    void selectTrack(int by);
    void selectTrackHead(int trackIndex);
    void selectMultitrack();
    void copyClip(int trackIndex, int clipIndex);
    void setTrackName(int trackIndex, const QString& value);
    void toggleTrackMute(int trackIndex);
    void toggleTrackHidden(int trackIndex);
    void setTrackComposite(int trackIndex, bool composite);
    void setTrackLock(int trackIndex, bool lock);
    bool moveClip(int fromTrack, int toTrack, int clipIndex, int position);
    void onClipMoved(int fromTrack, int toTrack, int clipIndex, int position);
    bool trimClipIn(int trackIndex, int clipIndex, int delta, bool ripple);
    bool trimClipOut(int trackIndex, int clipIndex, int delta, bool ripple);
    void insert(int trackIndex, int position = -1, const QString &xml = QString());
    void overwrite(int trackIndex, int position = -1, const QString &xml = QString());
    void appendFromPlaylist(Mlt::Playlist* playlist);
    void splitClip(int trackIndex = -1, int clipIndex = -1);
    void fadeIn(int trackIndex, int clipIndex = -1, int duration = -1);
    void fadeOut(int trackIndex, int clipIndex = -1, int duration = -1);
    void seekPreviousEdit();
    void seekNextEdit();
    void seekInPoint(int clipIndex);
    void clearSelectionIfInvalid();
    void insertTrack();
    void removeTrack();
    void onProducerChanged(Mlt::Producer*);
    void emitSelectedFromSelection();
    void remakeAudioLevels(int trackIndex, int clipIndex, bool force = true);
    void commitTrimCommand();
    void onRowsInserted(const QModelIndex& parent, int first, int last);
    void onRowsRemoved(const QModelIndex& parent, int first, int last);

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    bool isBlank(int trackIndex, int clipIndex);
    void pulseLockButtonOnTrack(int trackIndex);

    Ui::TimelineDock *ui;
    QQuickWidget m_quickView;
    MultitrackModel m_model;
    int m_position;
    Timeline::UpdateCommand* m_updateCommand;
    bool m_ignoreNextPositionChange;
    struct Selection {
        QList<int> selectedClips;
        int selectedTrack;
        bool isMultitrackSelected;
    };
    Selection m_selection;
    Selection m_savedSelection;
    QScopedPointer<Timeline::TrimCommand> m_trimCommand;
    QScopedPointer<UndoHelper> m_undoHelper;
    int m_trimDelta;

private slots:
    void load(bool force = false);
    void onTopLevelChanged(bool floating);
};

#endif // TIMELINEDOCK_H
