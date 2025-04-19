#include "py/runtime.h"
#include "psram_spi.h"

// Store the PSRAM config instance globally for now.
// For more advanced use, you could wrap this in a MicroPython object.
STATIC psram_spi_inst_t psram_inst;

// psram.init([pio=0, sm=-1, clkdiv=1.0, fudge=False])
STATIC mp_obj_t psram_init(size_t n_args, const mp_obj_t *args) {
    // Set defaults
    PIO pio = pio0;
    int sm = -1;
    float clkdiv = 1.0f;
    bool fudge = false;

    if (n_args > 0) {
        int pio_num = mp_obj_get_int(args[0]);
        pio = (pio_num == 1) ? pio1 : pio0;
    }
    if (n_args > 1) {
        sm = mp_obj_get_int(args[1]);
    }
    if (n_args > 2) {
        clkdiv = mp_obj_get_float(args[2]);
    }
    if (n_args > 3) {
        fudge = mp_obj_is_true(args[3]);
    }

    psram_inst = psram_spi_init_clkdiv(pio, sm, clkdiv, fudge);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(psram_init_obj, 0, 4, psram_init);

// psram.write32(addr, value)
STATIC mp_obj_t psram_write32(mp_obj_t addr_obj, mp_obj_t data_obj) {
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t data = mp_obj_get_int(data_obj);
    psram_spi_write32(&psram_inst, addr, data);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(psram_write32_obj, psram_write32);

// psram.read32(addr)
STATIC mp_obj_t psram_read32(mp_obj_t addr_obj) {
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t val = psram_spi_read32(&psram_inst, addr);
    return mp_obj_new_int_from_uint(val);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(psram_read32_obj, psram_read32);

STATIC const mp_rom_map_elem_t psram_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&psram_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_write32), MP_ROM_PTR(&psram_write32_obj) },
    { MP_ROM_QSTR(MP_QSTR_read32), MP_ROM_PTR(&psram_read32_obj) },
};

STATIC MP_DEFINE_CONST_DICT(psram_module_globals, psram_module_globals_table);

const mp_obj_module_t psram_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&psram_module_globals,
};

// Register the module to make it available in Python as "psram"
MP_REGISTER_MODULE(MP_QSTR_psram, psram_module);
