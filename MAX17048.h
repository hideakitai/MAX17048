#pragma once
#ifndef MAX17048_H
#define MAX17048_H

#ifdef TEENSYDUINO
#include <i2c_t3.h>
#else
#include <Wire.h>
#endif

namespace EmbeddedDevices
{
    template <int CELL>
    class MAX17048
    {
        const uint8_t I2C_ADDR = 0x36;

        enum class REG
        {
            VCELL = 0x02,
            SOC = 0x04,
            MODE = 0x06,
            VERSION = 0x08,
            HIBRT = 0x0A,
            CONFIG = 0x0C,
            VALRT = 0x14,
            CRATE = 0x16,
            VRESET_ID = 0x18,
            STATUS = 0x1A,
            TABLE = 0x40,
            CMD = 0xFE
        };

    public:

        uint16_t adc() { return read(REG::VCELL); }
        float voltage() { return (float)read(REG::VCELL) * 78.125f * (float)CELL / 1000000.f; }

        uint8_t percent() { return (uint8_t)(read(REG::SOC) / 256); }
        float accuratePercent() { return (float)read(REG::SOC) / 256.f; }

        void quickStart(bool b) { /* TODO: */ }
        bool quickStart() { /* TODO: */ }

        void enSleep(bool b) { /* TODO: */ }
        bool isSleepable() { /* TODO: */ }

        void mode(bool quick_start, bool sleep) { /* TODO: */ }
        uint16_t mode() { /* TODO: */ return read(REG::MODE); }

        uint8_t version() { return (uint8_t)read(REG::VERSION); }

        uint16_t hibernate() { /* TODO: */ return read(REG::HIBRT); }
        uint8_t hibernateActTh() { /* TODO: */ return (uint8_t)((read(REG::HIBRT) >> 8) & 0x00FF); }
        uint8_t hibernateHibTh() { /* TODO: */ return (uint8_t)((read(REG::HIBRT) >> 0) & 0x00FF); }
        void hibernate(uint8_t act_th, uint8_t hib_th) { /* TODO: */ write((uint8_t)REG::HIBRT, (((uint16_t)act_th << 8) | (uint16_t)hib_th)); }

        void rcomp(uint8_t value) { /* TODO: */ }
        void sleep(bool b) { /* TODO: */ }
        bool alertSOC() { /* TODO: */ return false; }
        bool alertStatus() { /* TODO: */ return false; }
        void alertSOCThreashold(uint8_t th) { /* TODO: */ return false; } // 1-32%, xx% = (32 - th), default 4% (0x1C)

        void alertVoltageThreashold(uint8_t min, uint8_t max) { /* TODO: */ }
        void alertVoltageThMax(uint8_t max) { /* TODO: */ }
        void alertVoltageThMin(uint8_t min) { /* TODO: */ }

        float crate() { return (float)read(REG::CRATE) * 0.208f; } // % per hour

        void vresetid() { /* TODO: */ }
        uint8_t id() { /* TODO: */ }
        uint8_t vreset() { /* TODO: */ }
        uint8_t dis() { /* TODO: */ }

        uint8_t status() { return read(REG::STATUS); }
        bool highVoltage() { /* TODO: */ }
        bool lowVoltage() { /* TODO: */ }
        bool resetVoltage() { /* TODO: */ }
        bool lowSOC() { /* TODO: */ }
        bool chnageSOC() { /* TODO: */ }

        bool reset() { write(REG::CMD, 0x5400); }

    private:

        void write(const REG reg, const bool stop = true)
        {
            Wire.beginTransmission(I2C_ADDR);
            Wire.write((uint8_t)reg);
            Wire.endTransmission(stop);
        }

        void write(const REG reg, const uint16_t data, const bool stop = true)
        {
            Wire.beginTransmission(I2C_ADDR);
            Wire.write((uint8_t)reg);
            Wire.write((data & 0xFF00) >> 8);
            Wire.write((data & 0x00FF) >> 0);
            Wire.endTransmission(stop);
        }

        uint16_t read(const REG reg)
        {
            write(reg, false);
            Wire.requestFrom((uint8_t)I2C_ADDR, (uint8_t)2); // 2byte R/W only
            uint16_t data = (uint16_t)((Wire.read() << 8) & 0xFF00);
            data |= (uint16_t)(Wire.read() & 0x00FF);
            return data;
        }

        bool b_quick_start {false};
        bool b_sleep {false};
    };

}

using MAX17048 = EmbeddedDevices::MAX17048<1>;
using MAX17049 = EmbeddedDevices::MAX17048<2>;

#endif // MAX17048_H
