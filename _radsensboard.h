#include "esphome.h"
#include <RadSensBoard.h>
#include <CountsPerMinute.h>

#define SECONDS_PER_INTERVAL 2

using namespace esphome;

class MyRadSens: public PollingComponent {
 public:
  Sensor *IntensityDynamic_Sensor = new Sensor();
  Sensor *IntensityStatic_Sensor = new Sensor();
  Sensor *CurrentCPM_Sensor = new Sensor();
  Sensor *MaxCPM_Sensor = new Sensor();
  RadSensBoard myself;
  CountsPerMinute cpm;
  MyRadSens(): PollingComponent(SECONDS_PER_INTERVAL * 1000) {}

  void setup() override {
    myself.init();
    cpm.init(60 / SECONDS_PER_INTERVAL);
  }

  void update() override {
    if (myself.readData()) {}
      float IntensityDynamic = myself.getRadiationLevelDynamic();
      float IntensityStatic = myself.getRadiationLevelStatic();
      int counts = myself.getPulseCount();
      myself.resetPulseCount();
      cpm.add(counts);
      if (cpm.isReady()) {
          CurrentCPM_Sensor->publish_state(cpm.getCurrentCpm());
          MaxCPM_Sensor->publish_state(cpm.getMaximumCpm());
      }
      IntensityDynamic_Sensor->publish_state(IntensityDynamic*1000);
      IntensityStatic_Sensor->publish_state(IntensityStatic*1000);
  }
};
