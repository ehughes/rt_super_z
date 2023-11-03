
/* Auto generated code. Do not modify.*/
#include <zephyr/kernel.h>
#include <zephyr/linker/linker-defs.h>
#include <zephyr/kernel_structs.h>
#include <kernel_internal.h>

extern char __ram_text_reloc_start[];
extern char __ram_text_rom_start[];
extern char __ram_text_reloc_size[];

void data_copy_xip_relocation(void)
{

	z_early_memcpy(&__ram_text_reloc_start, &__ram_text_rom_start,
		           (size_t) &__ram_text_reloc_size);


}

void bss_zeroing_relocation(void)
{
return;
}
