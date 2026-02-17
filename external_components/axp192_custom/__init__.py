import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_ADDRESS

DEPENDENCIES = ["i2c"]

axp192_custom_ns = cg.esphome_ns.namespace("axp192_custom")
AXP192Custom = axp192_custom_ns.class_("AXP192Custom", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(AXP192Custom),
    cv.Optional(CONF_ADDRESS, default=0x34): cv.int_range(min=0, max=0xFF),
}).extend(i2c.i2c_device_schema(0x34))


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield i2c.register_i2c_device(var, config)
    yield cg.register_component(var, config)
