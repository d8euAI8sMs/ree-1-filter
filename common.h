#pragma once

#include <array>

#include "AFC.h"

const double pi = 3.1415926535897932384626433832795;

const double min_frequency = 0;
const double max_frequency = 10000;

const size_t sample_count    = 0x4000;
const size_t frequency_count = 10000;

const double gain_no   = 1;
const double gain_base = 02.004955947;
const double gain_adv  = 11.522151900;

typedef unsigned long /*long*/       dds_samplerate_t;              // Raw DDS samplerate
typedef unsigned short               adc_samplerate_t;              // Raw ADC samplerate
typedef unsigned short               adc_raw_t;                     // Raw ADC input
typedef adc_raw_t                    adc_data_t[sample_count];      // Raw ADC batch input
typedef double                       volt_data_t[sample_count];     // ADC input in volts
typedef double                       fr_data_t[frequency_count];    // Frequency Response points
typedef std::pair < double, double > pair_t;

inline double adc2volt(adc_raw_t raw, double gain)
{
    if (raw & 0x0800)
    {
        raw |= 0xF000;
    }
    return ((10. * (short) raw) / 2047.) / gain;
}

inline double dds2frequency(dds_samplerate_t samplerate)
{
    return (samplerate * 33.333333) / (1LL << 32);
}

inline dds_samplerate_t frequency2dds(double samplerate)
{
    return (dds_samplerate_t) ((samplerate / 33.333333) * (1LL << 32));
}

inline void adc2volt(adc_data_t raw, volt_data_t volts, double gain)
{
    for (size_t i = 0; i < sample_count; i++)
    {
        volts[i] = adc2volt(raw[i], gain);
    }
}

inline pair_t fourier_at(volt_data_t volts, double frequency)
{
    pair_t complex;
    complex.first = complex.second = 0;

    double samplerate = GetMaxADCSamplerate();

    for (size_t i = 0; i < sample_count; i++)
    {
        complex.first  += volts[i] * std::cos(2 * pi * frequency * i / samplerate);
        complex.second -= volts[i] * std::sin(2 * pi * frequency * i / samplerate);
    }

    return std::make_pair(std::sqrt(complex.first * complex.first + complex.second * complex.second),
                          std::atan2(complex.second, complex.first));
}