//=============================================================================================================
/**
* @file     fiff_cov.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2012
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    FiffCov class declaration.
*
*/

#ifndef FIFF_COV_H
#define FIFF_COV_H

//*************************************************************************************************************
//=============================================================================================================
// FIFF INCLUDES
//=============================================================================================================

#include "fiff_global.h"
#include "fiff_proj.h"
#include "fiff_types.h"
#include "fiff_info.h"


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QSharedDataPointer>
#include <QSharedPointer>
#include <QString>
#include <QStringList>



//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================

#include <Eigen/Core>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE MNELIB
//=============================================================================================================

namespace FIFFLIB
{

//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Eigen;


//=============================================================================================================
/**
* Fiff cov data, which corresponds to a covariance data matrix
*
* @brief covariance data
*/
class FIFFSHARED_EXPORT FiffCov : public QSharedData
{
public:
    typedef QSharedPointer<FiffCov> SPtr;               /**< Shared pointer type for FiffCov. */
    typedef QSharedPointer<const FiffCov> ConstSPtr;    /**< Const shared pointer type for FiffCov. */
    typedef QSharedDataPointer<FiffCov> SDPtr;       /**< Shared data pointer type for FiffCov. */

    //=========================================================================================================
    /**
    * Constructs the covariance data matrix.
    */
    FiffCov();

    //=========================================================================================================
    /**
    * Constructs a covariance data matrix, by reading from a IO device.
    *
    * @param[in] p_IODevice     IO device to read from the evoked data set.
    */
    FiffCov(QIODevice &p_IODevice);

    //=========================================================================================================
    /**
    * Copy constructor.
    *
    * @param[in] p_FiffCov   Covariance data matrix which should be copied
    */
    FiffCov(const FiffCov &p_FiffCov);

    //=========================================================================================================
    /**
    * Destroys the covariance data matrix.
    */
    ~FiffCov();

    //=========================================================================================================
    /**
    * Initializes the covariance data matrix.
    */
    void clear();

    //=========================================================================================================
    /**
    * python pick_channels_cov
    *
    * Pick channels from covariance matrix
    *
    * @param[in] p_include  List of channels to include (if empty, include all available). (optional)
    * @param[in] p_exclude  Channels to exclude (if empty, do not exclude any). (optional)
    *
    * @return Covariance solution restricted to selected channels.
    */
    FiffCov pick_channels(const QStringList &p_include = defaultQStringList, const QStringList &p_exclude = defaultQStringList);

    //=========================================================================================================
    /**
    * Prepare noise covariance matrix. Before creating inverse operator.
    *
    * @param[in] p_info     measurement info
    * @param[in] p_chNames  Channels which should be taken into account
    *
    * @return the prepared noise covariance matrix
    */
    FiffCov prepare_noise_cov(const FiffInfo& p_info, const QStringList& p_chNames) const;

    //=========================================================================================================
    /**
    * Regularize noise covariance matrix
    *
    * This method works by adding a constant to the diagonal for each channel type separatly.
    * Special care is taken to keep the rank of the data constant.
    *
    * @param[in] p_info     The measurement info (used to get channel types and bad channels).
    * @param[in] p_fMag      Regularization factor for MEG magnetometers.
    * @param[in] p_fGrad     Regularization factor for MEG gradiometers.
    * @param[in] p_fEeg      Regularization factor for EEG.
    * @param[in] p_bProj     Apply or not projections to keep rank of data.
    * @param[in] p_exclude  List of channels to mark as bad. If None, bads channels are extracted from both info['bads'] and cov['bads'].
    *
    */
    FiffCov regularize(const FiffInfo& p_info, float p_fMag = 0.1, float p_fGrad = 0.1, float p_fEeg = 0.1, bool p_bProj = true, QStringList p_exclude = defaultQStringList) const;

    //=========================================================================================================
    /**
    * Assignment Operator
    *
    * @param[in] rhs     FiffCov which should be assigned.
    */
    FiffCov& operator= (const FiffCov &rhs);

public:
    fiff_int_t  kind;       /**< Covariance kind -> fiff_constants.h */
    bool diag;              /**< If the covariance is stored in a diagonal order. */
    fiff_int_t dim;         /**< Dimension of the covariance (dim x dim). */
    QStringList names;      /**< Channel names. */
    MatrixXd data;          /**< Covariance data */
    QList<FiffProj> projs;  /**< List of available ssp projectors. */
    QStringList bads;       /**< List of bad channels. */
    fiff_int_t nfree;       /**< Number of degrees of freedom. */
    VectorXd eig;           /**< Vector of eigenvalues. */
    MatrixXd eigvec;        /**< Matrix of eigenvectors. */
};

} // NAMESPACE

#endif // FIFF_COV_H
