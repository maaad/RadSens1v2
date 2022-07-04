Description
==============
ESPHome custom component sample for Climateguard RadSens


Install
===============

.. code-block:: console 

 git clone https://github.com/maaad/RadSens1v2 /usr/share/hassio/homeassistant/esphome/RadSens1v2

Add to device config

.. code-block:: yaml

 esphome:
   ...
  libraries:
    - Wire
    - RadSens1v2=https://github.com/climateguard/RadSens
  includes: 
    - RadSens1v2/_radSens1v2.h  

  i2c:

 ...

 sensor:
   - platform: custom
     lambda: |-
       auto rad_sens = new RadSens();
       App.register_component(rad_sens);
       return {rad_sens->NumberOfPulses_Sensor, rad_sens->IntensityDynamic_Sensor, rad_sens->IntensityStatic_Sensor};
     sensors:
       - name: "Number of Pulses"
         accuracy_decimals: 1
         unit_of_measurement: imp
       - name: "Dynamic intensity"
         id: dynamic_intensity
         accuracy_decimals: 1
         unit_of_measurement: μR/h
       - name: "Static intensity"
         accuracy_decimals: 1
         unit_of_measurement: μR/h


References
===============

https://esphome.io/components/esphome.html?highlight=include
https://esphome.io/custom/custom_component.html
https://github.com/climateguard/RadSens
