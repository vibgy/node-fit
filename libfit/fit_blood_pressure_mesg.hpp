////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Dynastream Innovations Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2015 Dynastream Innovations Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 16.10Release
// Tag = development-akw-16.10.00-0
////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_BLOOD_PRESSURE_MESG_HPP)
#define FIT_BLOOD_PRESSURE_MESG_HPP

#include "fit_mesg.hpp"

namespace fit
{

class BloodPressureMesg : public Mesg
{
   public:
      BloodPressureMesg(void) : Mesg(Profile::MESG_BLOOD_PRESSURE)
      {
      }

      BloodPressureMesg(const Mesg &mesg) : Mesg(mesg)
      {
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns timestamp field
      // Units: s
      ///////////////////////////////////////////////////////////////////////
      FIT_DATE_TIME GetTimestamp(void) const
      {
         return GetFieldUINT32Value(253, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set timestamp field
      // Units: s
      ///////////////////////////////////////////////////////////////////////
      void SetTimestamp(FIT_DATE_TIME timestamp)
      {
         SetFieldUINT32Value(253, timestamp, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns systolic_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetSystolicPressure(void) const
      {
         return GetFieldUINT16Value(0, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set systolic_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetSystolicPressure(FIT_UINT16 systolicPressure)
      {
         SetFieldUINT16Value(0, systolicPressure, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns diastolic_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetDiastolicPressure(void) const
      {
         return GetFieldUINT16Value(1, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set diastolic_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetDiastolicPressure(FIT_UINT16 diastolicPressure)
      {
         SetFieldUINT16Value(1, diastolicPressure, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns mean_arterial_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetMeanArterialPressure(void) const
      {
         return GetFieldUINT16Value(2, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set mean_arterial_pressure field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetMeanArterialPressure(FIT_UINT16 meanArterialPressure)
      {
         SetFieldUINT16Value(2, meanArterialPressure, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns map_3_sample_mean field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetMap3SampleMean(void) const
      {
         return GetFieldUINT16Value(3, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set map_3_sample_mean field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetMap3SampleMean(FIT_UINT16 map3SampleMean)
      {
         SetFieldUINT16Value(3, map3SampleMean, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns map_morning_values field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetMapMorningValues(void) const
      {
         return GetFieldUINT16Value(4, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set map_morning_values field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetMapMorningValues(FIT_UINT16 mapMorningValues)
      {
         SetFieldUINT16Value(4, mapMorningValues, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns map_evening_values field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16 GetMapEveningValues(void) const
      {
         return GetFieldUINT16Value(5, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set map_evening_values field
      // Units: mmHg
      ///////////////////////////////////////////////////////////////////////
      void SetMapEveningValues(FIT_UINT16 mapEveningValues)
      {
         SetFieldUINT16Value(5, mapEveningValues, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns heart_rate field
      // Units: bpm
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT8 GetHeartRate(void) const
      {
         return GetFieldUINT8Value(6, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set heart_rate field
      // Units: bpm
      ///////////////////////////////////////////////////////////////////////
      void SetHeartRate(FIT_UINT8 heartRate)
      {
         SetFieldUINT8Value(6, heartRate, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns heart_rate_type field
      ///////////////////////////////////////////////////////////////////////
      FIT_HR_TYPE GetHeartRateType(void) const
      {
         return GetFieldENUMValue(7, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set heart_rate_type field
      ///////////////////////////////////////////////////////////////////////
      void SetHeartRateType(FIT_HR_TYPE heartRateType)
      {
         SetFieldENUMValue(7, heartRateType, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns status field
      ///////////////////////////////////////////////////////////////////////
      FIT_BP_STATUS GetStatus(void) const
      {
         return GetFieldENUMValue(8, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set status field
      ///////////////////////////////////////////////////////////////////////
      void SetStatus(FIT_BP_STATUS status)
      {
         SetFieldENUMValue(8, status, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns user_profile_index field
      // Comment: Associates this blood pressure message to a user.  This corresponds to the index of the user profile message in the blood pressure file.
      ///////////////////////////////////////////////////////////////////////
      FIT_MESSAGE_INDEX GetUserProfileIndex(void) const
      {
         return GetFieldUINT16Value(9, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set user_profile_index field
      // Comment: Associates this blood pressure message to a user.  This corresponds to the index of the user profile message in the blood pressure file.
      ///////////////////////////////////////////////////////////////////////
      void SetUserProfileIndex(FIT_MESSAGE_INDEX userProfileIndex)
      {
         SetFieldUINT16Value(9, userProfileIndex, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

};

} // namespace fit

#endif // !defined(FIT_BLOOD_PRESSURE_MESG_HPP)
