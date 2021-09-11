#include "utilities.hh"

#include <vector>

uintptr_t FindPattern(const char* module_name, const char* signature)
{
    std::vector<int> sig_bytes;
    char* sig_start = const_cast<char*>(signature);
    char* sig_end = sig_start + strlen(signature);

    for (auto current = sig_start; current < sig_end; current++)
    {
        if (*current == '?')
        {
            current++;
            if (*current == '?') current++;
            sig_bytes.push_back(-1);
        }
        else
        {
            sig_bytes.push_back(strtoul(current, &current, 16));
        }
    }

    auto module_handle = GetModuleHandleA(module_name);
    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_header = reinterpret_cast<PIMAGE_NT_HEADERS64>(module_handle + dos_header->e_lfanew);

    auto module_size = nt_header->OptionalHeader.SizeOfImage;
    auto scan_ptr = reinterpret_cast<char*>(module_handle);

    auto sig_size = sig_bytes.size();
    auto sig_data = sig_bytes.data();

    for (auto i = 0; i < module_size - sig_size; i++)
    {
        bool failed = false;

        for (auto j = 0; j < sig_size; j++)
        {
            if (scan_ptr[i + j] != sig_data[j] && sig_data[j] != -1)
            {
                failed = true;
                break;
            }
        }

        if (! failed) return reinterpret_cast<uintptr_t>(&scan_ptr[i]);
    }

    return 0;
}

unsigned int CalculateRGB(double ratio)
{
    int normalized = int(ratio * 256 * 6);

    int x = normalized % 256;

    int red = 0, grn = 0, blu = 0;
    switch(normalized / 256)
    {
    case 0: red = 255;      grn = x;        blu = 0;       break;//red
    case 1: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
    case 2: red = 0;        grn = 255;      blu = x;       break;//green
    case 3: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
    case 4: red = x;        grn = 0;        blu = 255;     break;//blue
    case 5: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
    }

    return red + (grn << 8) + (blu << 16);
}