Description
==============
ESPHome custom component sample for Climateguard RadSens 1v2

.. note::
    Possible problems with Radsens 1v5 boards `workaround <https://github.com/maaad/RadSens1v2/issues/3#issuecomment-1289578773>`_

Install
===============

.. code-block:: console 

 git clone https://github.com/maaad/RadSens1v2 /usr/share/hassio/homeassistant/esphome/RadSens1v2

Add to device config for climateguard/RadSens

.. code-block:: yaml

 esphome:
   ...
  libraries:
    - Wire
    - "climateguard/ClimateGuard RadSens"
  includes: 
    - RadSens1v2/_CG_RadSens.h

  i2c:

 ...

 sensor:
   - platform: custom
     lambda: |-
       auto rad_sens = new MyRadSens();
       App.register_component(rad_sens);
       return {rad_sens->IntensityDynamic_Sensor, rad_sens->IntensityStatic_Sensor, rad_sens->CurrentCPM_Sensor, rad_sens->MaxCPM_Sensor};
     sensors:
       - name: "Dynamic intensity"
         id: dynamic_intensity
         accuracy_decimals: 1
         unit_of_measurement: μR/h
       - name: "Static intensity"
         accuracy_decimals: 1
         unit_of_measurement: μR/h
       - name: "Current CPM"
         accuracy_decimals: 1
         unit_of_measurement: CPM
       - name: "Max CPM"
         accuracy_decimals: 1
         unit_of_measurement: CPM


References
===============

https://esphome.io/components/esphome.html?highlight=include
https://esphome.io/custom/custom_component.html
https://github.com/climateguard/RadSens
