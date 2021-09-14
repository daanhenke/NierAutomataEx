#include <vector>
#include <Windows.h>
#include "misc/pattern.hh"

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))
inline
bool isMatch(const PBYTE addr, const PBYTE pat, const PBYTE msk)
{
	size_t n = 0;
	while (addr[n] == pat[n] || msk[n] == (BYTE)'?') {
		if (!msk[++n]) {
			return true;
		}
	}
	return false;
}

PBYTE findPattern(const PBYTE rangeStart, DWORD len, const char* pattern)
{
	size_t l = strlen(pattern);
	PBYTE patt_base = static_cast<PBYTE>(_alloca(l >> 1));
	PBYTE msk_base = static_cast<PBYTE>(_alloca(l >> 1));
	PBYTE pat = patt_base;
	PBYTE msk = msk_base;
	l = 0;
	while (*pattern) {
		if (*pattern == ' ')
			pattern++;
		if (!*pattern)
			break;
		if (*(PBYTE)pattern == (BYTE)'\?') {
			*pat++ = 0;
			*msk++ = '?';
			pattern += ((*(PWORD)pattern == (WORD)'\?\?') ? 2 : 1);
		}
		else {
			*pat++ = getByte(pattern);
			*msk++ = 'x';
			pattern += 2;
		}
		l++;
	}
	*msk = 0;
	pat = patt_base;
	msk = msk_base;
	for (DWORD n = 0; n < (len - l); ++n)
	{
		if (isMatch(rangeStart + n, patt_base, msk_base)) {
			return rangeStart + n;
		}
	}
	return NULL;
}

uintptr_t FindPattern(const char* module_name, const char* signature)
{
	auto module_handle = GetModuleHandleA(module_name);
	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS64>(module_handle + dos_header->e_lfanew);

	auto module_size = nt_header->OptionalHeader.SizeOfImage;

	unsigned int references = 0;
	return (uintptr_t)findPattern((PBYTE)module_handle, module_size, signature);
}