#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
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
	{ 0xee0f3dda, "module_layout" },
	{ 0x94f12b5d, "register_sysctl_table" },
	{ 0x1b6314fd, "in_aton" },
	{ 0x7db57751, "proc_dointvec" },
	{ 0x3c6467b8, "nf_register_hook" },
	{ 0xb72397d5, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x83767ca9, "proc_dostring" },
	{ 0xbbd5f353, "unregister_sysctl_table" },
	{ 0xdd0eb66a, "nf_unregister_hook" },
	{ 0x7d50a24, "csum_partial" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2457638B2CD9908F1E37E2B");
