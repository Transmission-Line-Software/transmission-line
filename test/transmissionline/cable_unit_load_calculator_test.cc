// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable_unit_load_calculator.h"

#include "gtest/gtest.h"

#include "base/convert_units.h"

class CableUnitLoadCalculatorTest : public ::testing::Test {
 protected:
  CableUnitLoadCalculatorTest() {

    c_.set_diameter_cable(1.108);
    c_.set_weight_unit_cable(1.094);
  }

  CableUnitLoadCalculator c_;
};

TEST_F(CableUnitLoadCalculatorTest, UnitCableLoad) {

  Vector2d load_unit;

  // ice only
  WeatherLoadCase case_ice;
  case_ice.description = "1-0-0";
  case_ice.density_ice = 57.3;
  case_ice.pressure_wind = 0;
  case_ice.temperature_cable = 0;
  case_ice.thickness_ice = 1;

  load_unit = c_.UnitCableLoad(case_ice);
  EXPECT_EQ(0, supportfunctions::Round(load_unit.x(), 3));
  EXPECT_EQ(3.729, supportfunctions::Round(load_unit.y(), 3));

  // wind only
  WeatherLoadCase case_wind;
  case_wind.description = "0-25-0";
  case_wind.density_ice = 0;
  case_wind.pressure_wind = 25;
  case_wind.temperature_cable = 0;
  case_wind.thickness_ice = 0;

  load_unit = c_.UnitCableLoad(case_wind);
  EXPECT_EQ(2.308, supportfunctions::Round(load_unit.x(), 3));
  EXPECT_EQ(1.094, supportfunctions::Round(load_unit.y(), 3));

  // ice and wind
  WeatherLoadCase case_both;
  case_both.description = "0.5-8-0";
  case_both.density_ice = 57.3;
  case_both.pressure_wind = 8;
  case_both.temperature_cable = 0;
  case_both.thickness_ice = 0.5;

  load_unit = c_.UnitCableLoad(case_both);
  EXPECT_EQ(1.405, supportfunctions::Round(load_unit.x(), 3));
  EXPECT_EQ(2.099, supportfunctions::Round(load_unit.y(), 3));
}

TEST_F(CableUnitLoadCalculatorTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}
