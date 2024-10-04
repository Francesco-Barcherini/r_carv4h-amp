#include <reg.h>
#include <inttypes.h>
#include <serial.h>
#include <gic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cbfw_printf.h>
#include <psci.h>
#include <gen_timer.h>
#include <ttbl.h>


#define INTF_READ

// Entry point of the guest
//extern void __start();
uint64_t timestamp_t0;

extern const uint64_t _bfw_start_point;
#define RAM_SIZE                0x20000000  /* Physical space size */
static volatile uint64_t pagetable_created = 0;

char vect[0x800000] __attribute__((aligned (0x800000)));

/* Setup stage-1 pagetables and enable guest MMU */
// static void setup_mmu()
// {
//     if(create_pagetable_flat_single_section((paddr_t)&_bfw_start_point, RAM_SIZE) < 0){
//         bprintf("Something went wrong on creating memory pagetable\n");
//     }

//     // if this fails but the previous succeeded, it could be because the previous mapped a
//     // too large area, which also covered the one reserved to devices
//     if(pagetable_add_device_section(GICD_BASE_ADDRESS, GICD_SIZE) < 0){
//         bprintf("Something went wrong on adding the gic distributor to the pagetable\n");
//     }

//     if(pagetable_add_device_section(GICC_BASE_ADDRESS, GICC_SIZE) < 0){
//         bprintf("Something went wrong on adding the gic cpu interface to the pagetable\n");
//     }

//     if(pagetable_add_device_section(UART_BASE_REG, UART_SIZE) < 0){
//         bprintf("Something went wrong on adding the uart to the pagetable\n");
//     }

//     if(enable_mmu() < 0){
//         bprintf("Something went wrong on enabling the mmu\n");
//     } else {
//         pagetable_created++;
//     }
// }



void getInterferedWrite(unsigned int size) {

    register uint64_t temp asm("x0");
    register uint64_t end asm("x1") = (uint64_t)vect + size;

    for(temp = (uint64_t)vect; temp < end; temp += 64) {
        *((volatile uint64_t*)temp) = temp;
    }
}

void getInterferedRead(unsigned int size) {

    register uint64_t temp asm("x0");
    register uint64_t end asm("x1") = (uint64_t)vect + size;

    for(temp = (uint64_t)vect; temp < end; temp += 64) {
        asm volatile("ldr x2, [%0]" :: "r"(temp) : "x2");
    }
}


void getInterferedHybrid(unsigned int size) {

    register uint64_t temp asm("x0");
    register uint64_t end asm("x1") = (uint64_t)vect + size;

    for(temp = (uint64_t)vect; temp < end; temp += 64) {
        *((volatile uint64_t*)temp) = temp;
        asm volatile("ldr x2, [%0]" :: "r"(temp) : "x2");
    }
}



void secondary_cores_main(void){

    bprintf("Hello. I am the Guest 1 from a secondary core\n");

    // arm_gic_init();

    /***************************/
    /* PUT YOUR USER CODE HERE */
    /*   FOR THE SECOND CORE   */
    /***************************/

    while(1) asm volatile("wfi");
}


void memory_main()
{
    // setup_mmu();

    /* Add some delay */
    for(uint64_t i = 0; i < 10000000; i++);

    bprintf("Hello. I am the Guest 1\n");

    // bprintf("Guest: initializing GIC\n");
    // arm_gic_init();

    /* Turning on the secondary core */
    //psci_cpu_on(1, (void*)__start);

    bprintf(WELCOME_STRING);

    /***************************/
    /* PUT YOUR USER CODE HERE */
    /*   FOR THE FIRST CORE    */
    /***************************/

    for (unsigned int size = 0x1000; size <= 0x100000; size += 0x8000) {
        uint64_t time_start, total_time = 0;

        for (int i = 0; i < 10000; i++) {

            time_start = gen_timer_get_ptimestamp();

#if defined(INTF_READ)
            getInterferedRead(size);
#elif defined(INTF_HYBRID)
            getInterferedHybrid(size);
#else
            getInterferedWrite(size);
#endif
            total_time += gen_timer_get_ptimestamp() - time_start;

            /* Add some delay */
            for(uint64_t i = 0; i < 1000000ULL; i++);

        }

        bprintf("%d, %llu\r\n", size, total_time);
    }

    while(1){
        asm volatile("wfi");
    }
    
    /* Turning off the entire VM */
    // psci_system_off();

}