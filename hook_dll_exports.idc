// Hooks all the exported functions given a DLL's base.

#include <idc.idc>

static get_str(addr)
{
	auto str = "";

	if(!hasValue(GetFlags(addr))) {
		return "";
	}

	for(;; addr = addr + 1) {
		if(Byte(addr) == 0) {
			return str;
		} else {
			str = str + sprintf("%c", Byte(addr));
		}
	}
}

static do_hook(addr)
{
	DelBpt(addr);
// Make a new software breakpoint with BPT_BRK set
	AddBptEx(addr, 1, BPT_SOFT);
// If you just want the breakpoint to report that it's been hit but not actually stop every time
//	SetBptAttr(addr, BPTATTR_FLAGS, GetBptAttr(addr, BPTATTR_FLAGS) & ~BPT_BRK);
}

static hook_dll_exports(dll_base)
{
	auto verbose = 0;

	if(dll_base == BADADDR || Word(dll_base) != 0x5a4d) {
		Warning("0x%08x is not a valid DLL base address", dll_base);
		return 0;
	}

	auto rva = dll_base + Dword(dll_base + 0x3c);
	auto ied = dll_base + Dword(rva + 0x78);
	if(verbose) { Message("RVA: 0x%08X   IED: 0x%08X\n", rva, ied); }

	auto num_functions = Dword(ied + 0x18);  // NumberOfNames
	auto addr_functions = dll_base + Dword(ied + 0x1c);  // AddressOfFunctions
	auto addr_names = dll_base + Dword(ied + 0x20);  // AddressOfNames
	
	auto i;

	for(i = 0; i < num_functions; i = i + 1) {
		auto name = dll_base + Dword(addr_names + (i * 4));
		auto addr = dll_base + Dword(addr_functions + (i * 4));
		
		if(verbose) { Message("Hooking %s (%a): 0x%08x\n", get_str(name), addr, addr); }
		do_hook(addr);
	}
}

static main()
{
	auto addr = AskAddr(BADADDR, "Enter DLL base address");

	return hook_dll_exports(addr);
}
