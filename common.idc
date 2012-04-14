static print_str(addr)
{
	auto str = "";
	auto b;

	b = DbgByte(addr);
	while(b != 0) {
		str = str + sprintf("%c", b);
		addr = addr + 1;
		b = DbgByte(addr);
	}

	Message("%s\n", str);
	return 1;
}

static print_hex(addr, len)
{
	auto str = "";
	auto i, j, bchar;

	if(len >= 20000 || len < 0) {
		Message("print_hex(%x, %i) called with len >= 20000 || len < 0\n", addr, len);
		return;
	}

	for(i = 0; i < len; i = i + 1) {
		if((i % 16) == 0) {
			if(i != 0) {
				str = str + "   ";
				for(j = -16; j <= -1; j = j + 1) {
					bchar = DbgByte(addr + i + j);
					if(bchar < 0x20 || bchar >= 0x7f) {
						str = str + ".";
					} else {
						str = str + sprintf("%c", bchar);
					}
				}
				str = str + "\n";
			}
			str = str + sprintf("%08X  ", addr + i);
		}

		str = str + sprintf("%02x ", DbgByte(addr + i));
	}

	for(i = 0; i < 3 * (1 + (16 - (((len - 1) % 16) + 1))); i = i + 1) {
		str = str + " ";
	}
	for(j = -1 * (((len - 1) % 16) + 1); j <= -1; j = j + 1) {
		bchar = DbgByte(addr + len + j);
		if(bchar < 0x20 || bchar >= 0x7f) {
			str = str + ".";
		} else {
			str = str + sprintf("%c", bchar);
		}
	}

	Message("%s\n", str);
	return 1;
}
