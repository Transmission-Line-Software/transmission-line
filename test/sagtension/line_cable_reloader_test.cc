// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"
#include "models/base/units.h"

class LineCableReloaderTest : public ::testing::Test {
 protected:
  LineCableReloaderTest() {
    // gets line cable from factory
    linecable_ = factory::BuildLineCable();

    // builds reloaded weather case
    weathercase_reloaded_ = new WeatherLoadCase();
    weathercase_reloaded_->description = "0-0-60";
    weathercase_reloaded_->thickness_ice = 0;
    weathercase_reloaded_->density_ice = 0;
    weathercase_reloaded_->pressure_wind = 0;
    weathercase_reloaded_->temperature_cable = 60;

    // builds fixture object
    l_.set_line_cable(linecable_);
    l_.set_weathercase_reloaded(weathercase_reloaded_);
    l_.set_condition_reloaded(CableConditionType::kInitial);
  }

  ~LineCableReloaderTest() {
    factory::DestroyLineCable(linecable_);
    delete weathercase_reloaded_;
  }

  // allocated dependency objects
  LineCable* linecable_;
  WeatherLoadCase* weathercase_reloaded_;

  // test object
  LineCableReloader l_;
};

TEST_F(LineCableReloaderTest, CatenaryReloaded) {
  Catenary3d catenary = l_.CatenaryReloaded();
  EXPECT_EQ(6000, helper::Round(catenary.tension_horizontal(), 0));

  // more reloading tests are done in the horizontal tension test
}

TEST_F(LineCableReloaderTest, LoadStretchTest) {
  double value = -999999;

  value = l_.LoadStretch(CableConditionType::kInitial);
  EXPECT_EQ(0, helper::Round(value, 0));

  value = l_.LoadStretch(CableConditionType::kLoad);
  EXPECT_EQ(12180, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionAverageComponent) {
  double value = -999999;

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3174, helper::Round(value, 0));

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2838, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionHorizontal) {
  double value = -999999;

  // changes the constraint type and limit to initial condition
  linecable_->constraint.condition = CableConditionType::kInitial;
  linecable_->constraint.limit = 6000;
  l_.set_line_cable(linecable_);

  weathercase_reloaded_->description = "0-0-60";
  weathercase_reloaded_->thickness_ice = 0;
  weathercase_reloaded_->density_ice = 0;
  weathercase_reloaded_->pressure_wind = 0;
  weathercase_reloaded_->temperature_cable = 60;
  l_.set_weathercase_reloaded(weathercase_reloaded_);

  // reloads at same case as constraint, checks all conditions
  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(6000, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(5561, helper::Round(value, 0));

  // reloads at high temperature case, checks all conditions
  weathercase_reloaded_->description = "0-0-212";
  weathercase_reloaded_->thickness_ice = 0;
  weathercase_reloaded_->density_ice = 0;
  weathercase_reloaded_->pressure_wind = 0;
  weathercase_reloaded_->temperature_cable = 212;
  l_.set_weathercase_reloaded(weathercase_reloaded_);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(4702, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(4528, helper::Round(value, 0));

  // reloads at high load case, checks all conditions
  weathercase_reloaded_->description = "1-8-0";
  weathercase_reloaded_->thickness_ice =
    units::ConvertLength(1, units::LengthConversionType::kInchesToFeet);
  weathercase_reloaded_->density_ice = 57.3;
  weathercase_reloaded_->pressure_wind = 8;
  weathercase_reloaded_->temperature_cable = 0;
  l_.set_weathercase_reloaded(weathercase_reloaded_);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(17126, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(17126, helper::Round(value, 0));

  // changes the constraint type and limit to stretched condition
  linecable_->constraint.condition = CableConditionType::kLoad;
  linecable_->constraint.limit = 5561.5;
  l_.set_line_cable(linecable_);

  // reloads at same case as constraint, checks all conditions
  weathercase_reloaded_->description = "0-0-60";
  weathercase_reloaded_->thickness_ice = 0;
  weathercase_reloaded_->density_ice = 0;
  weathercase_reloaded_->pressure_wind = 0;
  weathercase_reloaded_->temperature_cable = 60;
  l_.set_weathercase_reloaded(weathercase_reloaded_);

  l_.set_condition_reloaded(CableConditionType::kInitial);
  value = l_.TensionHorizontal();
  EXPECT_EQ(6000, helper::Round(value, 0));

  l_.set_condition_reloaded(CableConditionType::kLoad);
  value = l_.TensionHorizontal();
  EXPECT_EQ(5562, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, TensionHorizontalComponent) {
  double value = -999999;

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3174, helper::Round(value, 0));

  value = l_.TensionAverageComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2838, helper::Round(value, 0));
}

TEST_F(LineCableReloaderTest, Validate) {
  EXPECT_TRUE(l_.Validate(true, nullptr));
}
