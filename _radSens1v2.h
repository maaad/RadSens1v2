#include "esphome.h"
#include "radSens1v2.h"

using namespace esphome;

class MyRadSens: public PollingComponent {
 public:
  Sensor *IntensityDynamic_Sensor = new Sensor();
  Sensor *IntensityStatic_Sensor = new Sensor();
  Sensor *NumberOfPulses_Sensor = new Sensor();
  ClimateGuard_RadSens1v2 myself{RS_DEFAULT_I2C_ADDRESS};
  MyRadSens(): PollingComponent(5000) {}

  void setup() override {
    myself.radSens_init();
    myself.setLedState(true);
    myself.setSensitivity(105);
  }

  void update() override {
    uint32_t NumberOfPulses = myself.getNumberOfPulses();
    NumberOfPulses_Sensor->publish_state(NumberOfPulses);
    float IntensityDynamic = myself.getRadIntensyDynamic();
    IntensityDynamic_Sensor->publish_state(IntensityDynamic);
    float IntensityStatic = myself.getRadIntensyStatic();
    IntensityStatic_Sensor->publish_state(IntensityStatic);
  }
};
