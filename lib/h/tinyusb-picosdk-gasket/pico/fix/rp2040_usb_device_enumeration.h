/* This trick cannot possibly work when an OS is present.
 * The "fix" is hopelessly coupled with SDK functionality to provide delays, and is called from the ISR
 *
 * Use the other workaround: a dedicated hub for the pico device.
*/
#error "Won't work"
#include "rp2_common/pico_fix/rp2040_usb_device_enumeration/include/pico/fix/rp2040_usb_device_enumeration.h"
