#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb9d24654, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x53d1ad97, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0x524888f4, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x4101d598, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x4f23410, __VMLINUX_SYMBOL_STR(power_supply_get_property) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0xa5a3ccaf, __VMLINUX_SYMBOL_STR(power_supply_get_by_name) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FB730AFD5D57E568067191A");
