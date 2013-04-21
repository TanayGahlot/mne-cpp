//=============================================================================================================
/**
* @file     realtimesamplearraychinfo.h
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
* @brief    Contains the declaration of the RealTimeSampleArrayChInfo class.
*
*/

#ifndef REALTIMESAMPLEARRAYCHINFO_H
#define REALTIMESAMPLEARRAYCHINFO_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../xmeas_global.h"

//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QString>


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
* DECLARE CLASS RealTimeSampleArray
*
* @brief The RealTimeSampleArray class is the base class of every RealTimeSampleArray Measurement.
*/
class XMEASSHARED_EXPORT RealTimeSampleArrayChInfo
{
public:

    //=========================================================================================================
    /**
    * Constructs a RealTimeSampleArrayChInfo.
    */
    RealTimeSampleArrayChInfo();

    //=========================================================================================================
    /**
    * Destroys the RealTimeSampleArrayChInfo.
    */
    virtual ~RealTimeSampleArrayChInfo();

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
    * Sets the unit of the RealTimeSampleArray data.
    *
    * @param [in] unit of the data.
    */
    inline void setUnit(const QString& unit);

    //=========================================================================================================
    /**
    * Returns the unit of the RealTimeSampleArray measurement.
    *
    * @return the unit of the data of measurement.
    */
    inline const QString& getUnit() const;

private:
    QString             m_qStringChName;            /**< Holds the channel name.*/
    double              m_dMinValue;                /**< Holds the minimal value.*/
    double              m_dMaxValue;                /**< Holds the maximal value.*/
    QString             m_qString_Unit;             /**< Holds unit of the data of the measurement.*/

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================

inline void RealTimeSampleArrayChInfo::setMinValue(double minValue)
{
    m_dMinValue = minValue;
}


//*************************************************************************************************************

inline double RealTimeSampleArrayChInfo::getMinValue() const
{
    return m_dMinValue;
}


//*************************************************************************************************************

inline void RealTimeSampleArrayChInfo::setMaxValue(double maxValue)
{
    m_dMaxValue = maxValue;
}


//*************************************************************************************************************

inline double RealTimeSampleArrayChInfo::getMaxValue() const
{
    return m_dMaxValue;
}


//*************************************************************************************************************

inline void RealTimeSampleArrayChInfo::setUnit(const QString& unit)
{
    m_qString_Unit = unit;
}


//*************************************************************************************************************

inline const QString& RealTimeSampleArrayChInfo::getUnit() const
{
    return m_qString_Unit;
}

} // NAMESPACE

#endif // REALTIMESAMPLEARRAYCHINFO_H