#include "DriverBase.h"
#include <Windows.h>
#include <assert.h>

#include <pkn/stl/string>

#include "../base/types.h"

namespace pkn
{

    DriverBase::~DriverBase()
    {
        this->close();
    }

    bool DriverBase::open(const wchar_t *device_name)
    {
        if (this->is_opened())
            return true;
        //assert(!this->is_opened());
        //auto name = estr_t(make_estr(UR"(\??\)")).to_wstring() + device_name;
        auto name = stl::wstring(LR"(\??\)") + device_name;

        auto h = CreateFileW(name.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (INVALID_HANDLE_VALUE == h)
            return false;

        _handle = h;
        return true;
    }

    void DriverBase::close()
    {
        if (is_opened())
        {
            CloseHandle(_handle);
            _handle = INVALID_HANDLE_VALUE;
        }
    }

    bool DriverBase::ioctl(
        uint32_t code,
        void *input,
        uint32_t input_size,
        void *output,
        uint32_t *poutput_size
    ) const noexcept
    {
        return DeviceIoControl(_handle, code, input, input_size, output, poutput_size != nullptr ? *poutput_size : 0, (LPDWORD)poutput_size, nullptr);
    }

}