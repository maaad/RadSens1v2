#include "esphome.h"

#include "CG_RadSens.h"

#define SECONDS_PER_INTERVAL 5

#ifdef RS_DEFAULT_I2C_ADDRESS 
#undef RS_DEFAULT_I2C_ADDRESS
#define RS_DEFAULT_I2C_ADDRESS 0x66
#endif 

using namespace esphome;

class CountsPerMinute {
  int m_currentCpm;
  int m_maximumCpm;
  int m_intervalsPerMinute;
  int * m_intervalCounts;
  int m_currentInterval;
  int m_initialInterval;

  public:

    CountsPerMinute() {}

  void init(int intervalsPerMinute) {
    this -> m_currentCpm = 0;
    this -> m_maximumCpm = 0;
    this -> m_intervalsPerMinute = intervalsPerMinute;
    this -> m_intervalCounts = new int[this -> m_intervalsPerMinute];
    for (int i = 0; i < this -> m_intervalsPerMinute; i++) {
      this -> m_intervalCounts[i] = 0;
    }
    this -> m_currentInterval = 0;
    this -> m_initialInterval = 0;
  }

  bool isReady() {
    return this -> m_initialInterval >= this -> m_intervalsPerMinute;
  }

  int getCurrentCpm() {
    return this -> m_currentCpm;
  }

  int getMaximumCpm() {
    return this -> m_maximumCpm;
  }

  void resetCpm() {
    this -> m_maximumCpm = 0;
  }

  void add(int count) {
    if (this -> isReady()) {
      this -> m_currentCpm = this -> m_currentCpm - this -> m_intervalCounts[m_currentInterval] + count;
    } else {
      this -> m_currentCpm += count;
      this -> m_initialInterval++;
    }

    if (this -> m_maximumCpm < this -> m_currentCpm) {
      this -> m_maximumCpm = this -> m_currentCpm;
    }

    this -> m_intervalCounts[m_currentInterval] = count;

    m_currentInterval++;
    if (m_currentInterval >= m_intervalsPerMinute) {
      m_currentInterval = 0;
    }
  }

};

class MyRadSens: public PollingComponent, public CustomAPIDevice {
  public: Sensor * IntensityDynamic_Sensor = new Sensor();
  Sensor * IntensityStatic_Sensor = new Sensor();
  Sensor * CurrentCPM_Sensor = new Sensor();
  Sensor * MaxCPM_Sensor = new Sensor();
  Sensor * Sensivity_Sensor = new Sensor();
  CG_RadSens myself {
    RS_DEFAULT_I2C_ADDRESS
  };
  CountsPerMinute cpm;
  MyRadSens(): PollingComponent(SECONDS_PER_INTERVAL * 1000) {}

  uint32_t pulsesPrev = 0;
  int current_sensivity = 0;

  void on_set_sensivity(int sensivity) {
    myself.setSensitivity(sensivity);
    ESP_LOGD("Sensivity", "Set to %d", sensivity);
  }

  void on_reset_cpm() {
    cpm.resetCpm();
    ESP_LOGD("CPM", "Reset successful");
  }

  void setup() override {
    myself.init();
    myself.setLedState(true);
    myself.setSensitivity(105);
    cpm.init(60 / SECONDS_PER_INTERVAL);
    register_service( & MyRadSens::on_reset_cpm, "reset_cpm" );
    register_service( & MyRadSens::on_set_sensivity, "set_sensivity", {
      "sensivity"
    });
  }

  void update() override {
    float IntensityDynamic = myself.getRadIntensyDynamic();
    float IntensityStatic = myself.getRadIntensyStatic();
    int Sensivity = myself.getSensitivity();
    int Pulses = myself.getNumberOfPulses();
    if (Pulses > pulsesPrev) {
      cpm.add(Pulses - pulsesPrev);
    }
    if (cpm.isReady()) {
      CurrentCPM_Sensor -> publish_state(cpm.getCurrentCpm());
      MaxCPM_Sensor -> publish_state(cpm.getMaximumCpm());
    }
    if (IntensityDynamic != 0) {
      IntensityDynamic_Sensor -> publish_state(IntensityDynamic);
    }
    if (IntensityStatic != 0) {
      IntensityStatic_Sensor -> publish_state(IntensityStatic);
    }
    if (current_sensivity != Sensivity) {
      Sensivity_Sensor -> publish_state(Sensivity);
    }
    pulsesPrev = Pulses;
    current_sensivity = Sensivity;
  }
};
