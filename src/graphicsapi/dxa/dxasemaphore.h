#pragma once

#include "common/uniquehandle.h"
#include "dxacommon.h"

namespace Dxa
{
struct Semaphore
{
    // in dx, fence can be used as semaphore

    struct SemaphoreHandleDeleter
    {
        void
        operator()(HANDLE handle)
        {
            assert(handle != nullptr);
            ::CloseHandle(handle);
        }
    };

    ComPtr<ID3D12Fence>                             m_dx_fence = nullptr;
    UINT64                                          m_expected_fence_value;
    UniquePtrHandle<HANDLE, SemaphoreHandleDeleter> m_fence_handle;

    Semaphore() {}

    Semaphore(Device * device, const std::string & name = "")
    {
        m_expected_fence_value = 1;

        // create fence
        DXCK(device->m_dx_device->CreateFence(m_expected_fence_value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_dx_fence)));

        // create fence handle
        *m_fence_handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!m_fence_handle.get())
        {
            Logger::Critical<true>("cannot create fence handle");
        }

        // name fence
        device->name_dx_object(m_dx_fence, name);
    }
};
} // namespace Dxa
