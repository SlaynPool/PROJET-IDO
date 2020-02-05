#pragma once
#include "stdint.h"

/////////// Super Simple Fixed Point Lowpass ////////////////

class LPF
{
  private:
    int32_t RawData;
    int32_t SmoothDataINT;
    int32_t SmoothDataFP;
    int Beta;     // Length = 16
    int FP_Shift; //Number of fractional bits

  public:
    LPF(int Beta_, int FP_Shift_)
    {
      Beta = Beta_;
      FP_Shift = FP_Shift_;
    }

    LPF(int Beta_)
    {
      Beta = Beta_;
    }

    LPF()
    {
      Beta = 3;
      FP_Shift = 3;
    }

    void clear() {
      RawData = 0;
      SmoothDataINT = 0;
      SmoothDataFP = 0;
    }

    int32_t update(int32_t Indata)
    {
      RawData = Indata;
      RawData <<= FP_Shift; // Shift to fixed point
      SmoothDataFP = (SmoothDataFP << Beta) - SmoothDataFP;
      SmoothDataFP += RawData;
      SmoothDataFP >>= Beta;
      // Don't do the following shift if you want to do further
      // calculations in fixed-point using SmoothData
      SmoothDataINT = SmoothDataFP >> FP_Shift;
      return SmoothDataINT;
    }
};
