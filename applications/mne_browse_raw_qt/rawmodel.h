//=============================================================================================================
/**
* @file     rawmodel.h
* @author   Florian Schlembach <florian.schlembach@tu-ilmenau.de>;
*           Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>;
*           Jens Haueisen <jens.haueisen@tu-ilmenau.de>
* @version  1.0
* @date     January, 2014
*
* @section  LICENSE
*
* Copyright (C) 2014, Florian Schlembach, Christoph Dinh, Matti Hamalainen and Jens Haueisen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    This class represents the model of the model/view framework of mne_browse_raw_qt application.
*           It is derived from QAbstractTableModel so the virtual functions rowCount(),columnCount() and data()
*           needed to be reimplemented. The delegate requests the data for any individual table cell by
*           invoking data(QModelIndex index, int role) and a certain role. DisplayRole is the standard role
*           for requesting the plain data. Other roles such as BackgroundRole are requested to fill a cell
*           with a certain background color with respect to the individual index.
*           For further information see [1].
*
*           The way how the data is organized is totally up to the model. In our case, the raw and processed
*           data is stored in stored in the two matrices m_data[i] and m_procData[i] (both are QList that contains MatrixXdR), respectively. (nchans x m_iWindowSize)
*           The data is loaded and displayed blockwise. If the user scrolls close (meaning distanced smaller than
*           m_iReloadPos) to the loaded edge, the subsequent block is loaded from the fiff file. The maximum number
*           of loaded window blocks is determined by the parameter m_maxWindows. If m_maxWindows is reached and another
*           block is to be loaded, the first or last block (depending on whether the user scrolls to the right or left edge)
*           is removed from m_data, pretty much like a circular buffer. The logic of the reloading is managed by the
*           slot updateScrollPos, which obtains the value from the horizontal QScrollBar being part of the connected TableView.
*
*           In order to not freeze the GUI when reloading new data or filtering data, the RawModel class makes heavy use
*           of the QtConcurrent features. [2]
*           Therefore, the methods updateOperatorsConcurrently() and readSegment() is run in a background-thread. Once the results
*           are ready the m_operatorFutureWatcher and m_reloadFutureWatcher emits a signal that is connect to the slots
*           insertProcessedData() and insertReloadedData(), respectively.
*
*           MNEOperators such as FilterOperators are stored in m_Operators. The MNEOperators that are applied to any
*           individual channel are stored in the QMap m_assignedOperators.
*
*           [1] http://qt-project.org/doc/qt-5/QAbstractTableModel.html
*           [2] http://qt-project.org/doc/qt-5.0/qtconcurrent/qtconcurrent-index.html
*
*/

#ifndef RAWMODEL_H
#define RAWMODEL_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "types.h"


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QDebug>
#include <QAbstractTableModel>
#include <QSettings>
#include <QMetaEnum>

#include <QBrush>
#include <QPalette>

#include <QtConcurrent>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================

#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Eigen/unsupported/FFT>

#ifndef EIGEN_FFTW_DEFAULT
#define EIGEN_FFTW_DEFAULT
#endif


//*************************************************************************************************************
//=============================================================================================================
// MNE INCLUDES
//=============================================================================================================

#include <fiff/fiff.h>
#include <mne/mne.h>
#include <utils/parksmcclellan.h>
#include "filteroperator.h"


//=============================================================================================================
// USED NAMESPACES

using namespace MNE_BROWSE_RAW_QT;

using namespace Eigen;
using namespace MNELIB;
using namespace UTILSLIB;
using namespace FIFFLIB;

//=============================================================================================================

class RawModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RawModel(QObject *parent);
    RawModel(QFile& qFile, QObject *parent);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //METHODS
    /**
    * loadFiffData loads fiff data file.
    *
    * @param p_IODevice fiff data file to write
    */
    bool loadFiffData(QFile& qFile);

    /**
     * writeFiffData writes a new fiff data file
     * @param p_IODevice fiff data file to write
     * @return
     */
    bool writeFiffData(QFile &qFile);

    //VARIABLES
    bool m_bFileloaded; /**< true when a Fiff file is loaded */

    //Fiff data structure
    QList<MatrixXdR> m_data; /**< List that holds the fiff matrix data <n_channels x n_samples> */
    QList<MatrixXdR> m_procData; /**< List that holds the processed fiff matrix data <n_channels x n_samples> */
    QList<MatrixXdR> m_times; /**< List that holds the time axis [in secs] */

    QList<FiffChInfo> m_chInfolist; /**< List of FiffChInfo objects that holds the corresponding channels information */
    FiffInfo m_fiffInfo; /**< fiff info of whole fiff file */

    //Filter operators
    QMap<QString,QSharedPointer<MNEOperator> > m_Operators; /**< generated MNEOperator types (FilterOperator,PCA etc.) */
    QMap<int,QSharedPointer<MNEOperator> > m_assignedOperators; /**< Map of MNEOperator types to channels*/

    //Settings
    QSettings m_qSettings;

    qint32 m_iAbsFiffCursor; /**< Cursor that points to the current position in the fiff data file [in samples] */
    qint32 m_iCurAbsScrollPos; /**< the current (absolute) ScrollPosition in the fiff data file */

    qint32 m_iWindowSize; /**< Length of window to load [in samples] */
    qint32 m_reloadPos; /**< Distance that the current window needs to be off the ends of m_data[i] [in samples] */
    qint8 m_maxWindows; /**< number of windows that are at maximum remained in m_data */
    qint16 m_iFilterTaps; /**< Number of Filter taps */

    QSharedPointer<FiffIO> m_pfiffIO; /**< FiffIO objects, which holds all the information of the fiff data (excluding the samples!) */

private:
    //METHODS
    /**
     * genStdFilters generates a set of standard FilterOperators
     */
    void genStdFilterOps();

    /**
    * Loads fiff infos to m_chInfolist abd m_fiffInfo.
    *
    */
    void loadFiffInfos();

    /**
     * clearModel clears all model's members
     */
    void clearModel();

    /**
     * resetPosition reset the position of the current m_iAbsFiffCursor if a ScrollBar position is selected, whose data is not yet loaded.
     *
     * @param position where the new data block of the fiff file should be loaded.
     */
    void resetPosition(qint32 position);

    /**
     * reloadFiffData
     *
     * @param before
     */
    void reloadFiffData(bool before);

    /**
     * @brief readSegment is the wrapper method to read a segment from the raw fiff file
     * @param from the start point to read from the file
     * @param to the end point to read from the file
     * @return the data and times matrices
     */
    QPair<MatrixXd,MatrixXd> readSegment(fiff_int_t from, fiff_int_t to);

    //VARIABLES
    //Reload control
    bool m_bStartReached; /**< signals, whether the start of the fiff data file is reached */
    bool m_bEndReached; /**< signals, whether the end of the fiff data file is reached */
    bool m_bReloadBefore; /**< bool value indicating if data was reloaded before (1) or after (0) the existing data */

    //Concurrent reloading
    QFutureWatcher<QPair<MatrixXd,MatrixXd> > m_reloadFutureWatcher; /**< QFutureWatcher for watching process of reloading fiff data */
    bool m_bReloading; /**< signals when the reloading is ongoing */

    //Concurrent processing
//    QFutureWatcher<QPair<int,RowVectorXd> > m_operatorFutureWatcher; /**< QFutureWatcher for watching process of applying Operators to reloaded fiff data */
    QFutureWatcher<void> m_operatorFutureWatcher; /**< QFutureWatcher for watching process of applying Operators to reloaded fiff data */
    QList<QPair<int,RowVectorXd> > m_listTmpChData; /**< contains pairs with a channel number and the corresponding RowVectorXd */
    bool m_bProcessing; /**< true when processing in a background-thread is ongoing*/

    QMutex m_Mutex; /**< mutex for locking against simultaenous access to shared objects > */

signals:
    /**
     * dataReloaded is emitted when data reloading has finished in the background-thread
     */
    void dataReloaded();

    /**
     * scrollBarValueChange is emitted if a change of the horizontal ScrollBar value is requested by the RawModel
     * @param pos is the requested relative scroll position (0 equals beginning of ScrollBar)
     */
    void scrollBarValueChange(int pos);

public slots:
    /**
     * updateScrollPos checks, whether the actual position of the QScrollBar demands for a fiff data reload (depending on m_reloadPos and m_iCurAbsScrollPos)
     *
     * @param value the position of QScrollBar
     */
    void updateScrollPos(int value);

    /**
     * markChBad marks the selected channels as bad/good in m_chInfolist
     * @param chlist is the list of indices that are selected for marking
     * @param status, status=1 -> mark as bad, status=0 -> mark as good
     */
    void markChBad(QModelIndexList chlist, bool status);

    /**
     * applyOperator applies assigend operators to channel
     * @param index selects the channel to process
     * @param filter
     */
    void applyOperator(QModelIndex chan, const QSharedPointer<MNEOperator> &operatorPtr, bool reset=false);

    /**
     * applyOperator applies operators to channels
     * @param chlist selects the channels to process
     * @param filter
     */
    void applyOperator(QModelIndexList chlist, const QSharedPointer<MNEOperator> &operatorPtr, bool reset=false);

    /**
     * applyOperatorsConcurrently updates all applied MNEOperators to a given RowVectorXd and modifies it in-place
     * @param chdata[in,out] represents the channel data as a RowVectorXd
     */
    void applyOperatorsConcurrently(QPair<int, RowVectorXd>& chdata);

    /**
     * updateOperators updates all set operator to channels according to m_assignedOperators
     * @param chan the channel to which the operators shall be updated
     */
    void updateOperators(QModelIndex chan);

    /**
     * updateOperators is an overloaded function to update the operators to a channel list
     * @param chlist
     */
    void updateOperators(QModelIndexList chlist);

    /**
     * updateOperators is an overloaded function that updates all channels according to m_assignedOperators
     */
    void updateOperators();

    /**
     * undoFilter undoes the filtering operation for filter operations of the type
     * @param chlist selects the channels to filter
     * @param type determines the filter type TPassType to choose for the undo operation
     */
    void undoFilter(QModelIndexList chlist, const QSharedPointer<MNEOperator> &filterPtr);

    /**
     * undoFilter undoes the filtering operation for all filter operations
     * @param chlist selects the channels to filter
     */
    void undoFilter(QModelIndexList chlist);

    /**
     * undoFilter undoes the filtering operation for all filter operations for all channels
     */
    void undoFilter();

private slots:
    /**
     * insertReloadedData inserts the reloaded data when the background has finished the operation
     * @param dataTimesPair contains the reloaded matrices of the data and times so it can be inserted into m_data and m_times
     */
    void insertReloadedData(QPair<MatrixXd,MatrixXd> dataTimesPair);

    /**
     * updateOperatorsConcurrently runs the processing of the MNEOperators in a background-thread
     */
    void updateOperatorsConcurrently();

    /**
     * insertProcessedData inserts the processed data into m_procData when background-thread has finished (this method would be used for QtConcurrent::mapped)
     * @param index represents the row index in m_procData
     */
    void insertProcessedData(int index);

    /**
     * insertProcessedData inserts the processed data into m_procData when background-thread has finished (this method would be used for QtConcurrent::map)
     */
    void insertProcessedData();

//inline
public:
    /**
     * sizeOfFiffData
     *
     * @return the size of the total data contained in the loaded Fiff file
     */
    inline qint32 sizeOfFiffData() {
        if(!m_pfiffIO->m_qlistRaw.empty())
            return (m_pfiffIO->m_qlistRaw[0]->last_samp-m_pfiffIO->m_qlistRaw[0]->first_samp);
        else return 0;
    }

    /**
     * firstSample
     *
     * @return the first sample of the loaded Fiff file
     */
    inline qint32 firstSample() const {
        if(!m_pfiffIO->m_qlistRaw.empty())
            return m_pfiffIO->m_qlistRaw[0]->first_samp;
        else return 0;
    }

    /**
     * lastSample
     *
     * @return the last sample of the loaded Fiff file
     */
    inline qint32 lastSample() const {
        if(!m_pfiffIO->m_qlistRaw.empty())
            return m_pfiffIO->m_qlistRaw[0]->last_samp;
        else return 0;
    }

    /**
     * sizeOfPreloadedData
     *
     * @return size of loaded m_data
     */
    inline qint32 sizeOfPreloadedData() const {
        if(!m_data.empty()) {
            return m_data.size()*m_iWindowSize;
        }
        else return 0;
    }

    /**
     * relFiffCursor
     *
     * @return the relative cursor in the fiff file
     */
    inline qint32 relFiffCursor() const {
        return (m_iAbsFiffCursor - m_pfiffIO->m_qlistRaw[0]->first_samp);
    }

    /**
     * absFiffCursor (introduced for consistency reasons)
     *
     * @return the absolute cursor in the fiff file
     */
    inline qint32 absFiffCursor() const {
        return m_iAbsFiffCursor;
    }

};

Q_DECLARE_METATYPE(MatrixXdR);
Q_DECLARE_METATYPE(RowVectorPair);
Q_DECLARE_METATYPE(QList<RowVectorPair>);


#endif // RAWMODEL_H
