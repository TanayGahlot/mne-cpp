//=============================================================================================================
/**
* @file     realtimemultisamplearray.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2013
*
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the declaration of the RealTimeMultiSampleArray class.
*
*/

#ifndef REALTIMEMULTISAMPLEARRAY_H
#define REALTIMEMULTISAMPLEARRAY_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../xmeas_global.h"
#include "sngchnmeasurement.h"


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QVector>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE XMEASLIB
//=============================================================================================================

namespace XMEASLIB
{


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

//using namespace IOBuffer;


//=========================================================================================================
/**
* DECLARE CLASS RealTimeMultiSampleArray
*
* @brief The RealTimeMultiSampleArray class is the base class of every RealTimeMultiSampleArray Measurement.
*/
class XMEASSHARED_EXPORT RealTimeMultiSampleArray : public SngChnMeasurement
{
public:
    typedef QSharedPointer<RealTimeMultiSampleArray> SPtr;               /**< Shared pointer type for RealTimeMultiSampleArray. */
    typedef QSharedPointer<const RealTimeMultiSampleArray> ConstSPtr;    /**< Const shared pointer type for RealTimeMultiSampleArray. */

    //=========================================================================================================
    /**
    * Constructs a RealTimeMultiSampleArray.
    *
    * @param [in] uiNumChannels the number of channels.
    */
    RealTimeMultiSampleArray(unsigned int uiNumChannels);
    //=========================================================================================================
    /**
    * Destroys the RealTimeMultiSampleArray.
    */
    virtual ~RealTimeMultiSampleArray();

    //=========================================================================================================
    /**
    * Sets the minimal value. If current value to set is smaller, current value is set to minimal value.
    *
    * @param [in] minValue minimal value.
    */
    inline void setMinValue(double minValue);
    //=========================================================================================================
    /**
    * Returns the minimal value.
    *
    * @return the minimal value.
    */
    inline double getMinValue() const;

    //=========================================================================================================
    /**
    * Sets the maximal value. If value to set is bigger, current value is set to maximal value.
    *
    * @param [in] maxValue maximal value.
    */
    inline void setMaxValue(double maxValue);
    //=========================================================================================================
    /**
    * Returns the maximal value.
    *
    * @return the maximal value.
    */
    inline double getMaxValue() const;

    //=========================================================================================================
    /**
    * Sets the sampling rate of the RealTimeMultiSampleArray Measurement.
    *
    * @param [in] dSamplingRate the sampling rate of the RealTimeMultiSampleArray.
    */
    inline void setSamplingRate(double dSamplingRate);
    //=========================================================================================================
    /**
    * Returns the sampling rate of the RealTimeMultiSampleArray Measurement.
    *
    * @return the sampling rate of the RealTimeMultiSampleArray.
    */
    inline double getSamplingRate() const;

    //=========================================================================================================
    /**
    * Sets the number of channels of .
    *
    * @param [in] uiNumChannels the number of channels.
    */
    inline void setNumChannels(unsigned int uiNumChannels);

    //=========================================================================================================
    /**
    * Returns the number of channels of .
    *
    * @return the number of values which are gathered before a notify() is called.
    */
    inline unsigned int getNumChannels() const;

    //=========================================================================================================
    /**
    * Sets the number of sample vectors which should be gathered before attached observers are notified by calling the Subject notify() method.
    *
    * @param [in] ucMultiArraySize the number of values.
    */
    inline void setMultiArraySize(unsigned char ucMultiArraySize);
    //=========================================================================================================
    /**
    * Returns the number of values which should be gathered before attached observers are notified by calling the Subject notify() method.
    *
    * @return the number of values which are gathered before a notify() is called.
    */
    inline unsigned char getMultiArraySize() const;
    //=========================================================================================================
    /**
    * Returns the gathered multi sample array.
    *
    * @return the current multi sample array.
    */
    inline const QVector< QVector<double> >& getMultiSampleArray();

    //=========================================================================================================
    /**
    * Sets the unit of the RealTimeMultiSampleArray data.
    *
    * @param [in] unit of the data.
    */
    inline void setUnit(const QString& unit);
    //=========================================================================================================
    /**
    * Returns the unit of the RealTimeMultiSampleArray measurement.
    *
    * @return the unit of the data of measurement.
    */
    inline const QString& getUnit() const;

    //=========================================================================================================
    /**
    * Not used.
    * This method is inherited by Measurement.
    *
    * @param [in] not used
    */
    virtual void setValue(double) {;};
    //=========================================================================================================
    /**
    * Not used.
    * This method is inherited by Measurement.
    *
    * @return always -1, because values are not used.
    */
    virtual double getValue() const {return -1;};

    //=========================================================================================================
    /**
    * Attaches a value to the sample array vector.
    * This method is inherited by Measurement.
    *
    * @param [in] v the value which is attached to the sample array vector.
    */
    virtual void setVector(QVector<double> v);
    //=========================================================================================================
    /**
    * Returns the current value set.
    * This method is inherited by Measurement.
    *
    * @return the last attached value.
    */
    virtual QVector<double> getVector() const;

private:
    double              m_dMinValue;                /**< Holds the minimal value.*/
    double              m_dMaxValue;                /**< Holds the maximal value.*/
    double              m_dSamplingRate;            /**< Holds sampling rate of the RealTimeSampleArray.*/
    QString             m_qString_Unit;             /**< Holds unit of the data of the measurement.*/
    unsigned int        m_uiNumChannels;               /**< Holds the number of channels.*/
    QVector<double>     m_vecValue;                 /**< Holds the current attached sample vector.*/
    unsigned char       m_ucMultiArraySize;         /**< Holds sample size of the multi sample array.*/
    QVector< QVector<double> >     m_matSamples;    /**< Holds the multi sample array.*/
};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline void RealTimeMultiSampleArray::setMinValue(double minValue)
{
    m_dMinValue = minValue;
}


//*************************************************************************************************************

inline double RealTimeMultiSampleArray::getMinValue() const
{
    return m_dMinValue;
}


//*************************************************************************************************************

inline void RealTimeMultiSampleArray::setMaxValue(double maxValue)
{
    m_dMaxValue = maxValue;
}


//*************************************************************************************************************

inline double RealTimeMultiSampleArray::getMaxValue() const
{
    return m_dMaxValue;
}


//*************************************************************************************************************

inline void RealTimeMultiSampleArray::setSamplingRate(double dSamplingRate)
{
    m_dSamplingRate = dSamplingRate;
}


//*************************************************************************************************************

inline double RealTimeMultiSampleArray::getSamplingRate() const
{
    return m_dSamplingRate;
}


//*************************************************************************************************************

inline void RealTimeMultiSampleArray::setNumChannels(unsigned int uiNumChannels)
{
    m_uiNumChannels = uiNumChannels;
}


//*************************************************************************************************************

inline unsigned int RealTimeMultiSampleArray::getNumChannels() const
{
    return m_uiNumChannels;
}

//*************************************************************************************************************

inline void RealTimeMultiSampleArray::setMultiArraySize(unsigned char ucMultiArraySize)
{
    //Obsolete unsigned char can't be bigger
//    if(ucArraySize > 255)
//        m_ucArraySize = 255;
//    else
        m_ucMultiArraySize = ucMultiArraySize;
}


//*************************************************************************************************************

unsigned char RealTimeMultiSampleArray::getMultiArraySize() const
{
    return m_ucMultiArraySize;
}


//*************************************************************************************************************

inline const QVector< QVector<double> >& RealTimeMultiSampleArray::getMultiSampleArray()
{
    return m_matSamples;
}


//*************************************************************************************************************

inline void RealTimeMultiSampleArray::setUnit(const QString& unit)
{
    m_qString_Unit = unit;
}


//*************************************************************************************************************

inline const QString& RealTimeMultiSampleArray::getUnit() const
{
    return m_qString_Unit;
}

} // NAMESPACE

#endif // REALTIMEMULTISAMPLEARRAY_H
