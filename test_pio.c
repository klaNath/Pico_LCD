
#include "pico/stdlib.h"
#include "hardware/pio.h"

//PIO Assemble 
#include "test_pio.pio.h"


//LOGO File
#include "bmp.h"

int main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    //ほとんどの命令メモリを使うため、Pico Wが使うPIOを避ける必要がある
    PIO pio = pio0;

    // PIOのメモリを格納する
    uint offset_hsync = pio_add_program(pio, &ParallelTFT_hsync_clk_program);
    uint offset_vsync = pio_add_program(pio, &ParallelTFT_vsync_program);
    uint offset_data = pio_add_program(pio, &ParallelTFT_data_program);

    // 空いているステートマシンを割り当て
    uint sm_h = pio_claim_unused_sm(pio, true);
    uint sm_v = pio_claim_unused_sm(pio, true);
    uint sm_d = pio_claim_unused_sm(pio, true);

    //先にData出力用SMを初期化し、横Pixel数-1の値を入れておく
    ParallelTFT_data_program_init(pio, sm_d, offset_data, 4);
    pio_sm_put_blocking(pio, sm_d, 399u);

    //クロックと同期信号出力用SMを初期化、有効可する
    ParallelTFT_hsync_clk_program_init(pio, sm_h, offset_hsync, 21, 22);
    ParallelTFT_vsync_program_init(pio, sm_v, offset_vsync, 20);

    //各SMのクロック分周器を同期させる。
    pio_enable_sm_mask_in_sync(pio, (1u << sm_h) | (1u << sm_v)| (1u << sm_d));

    while (true) {
        // Blink

        if(pio_interrupt_get(pio, 1)){
            for(uint i = 0; i < (FLEXWORKS_HEIGHT) * FLEXWORKS_WIDTH; i++){
                pio_sm_put_blocking(pio, sm_d, BMP_LOGO[i]);
            }
        }
    }
}
