
#include "../port.h"
#include "b_config.h"
#include "b_os.h"

#if (defined(_UNITY_ENABLE) && (_UNITY_ENABLE == 1))

void setUp(void)
{
    static uint8_t init_f = 0;
    if (init_f == 0)
    {
        init_f = 1;
        port_init();
        bInit();
    }
}

void tearDown(void)
{
    ;
}

void testbMallocbFree()
{
    int   i = 0, m_size;
    void* addr[10];
    for (i = 0; i < 10; i++)
    {
        srand(bHalGetSysTick() + i);
        m_size = rand() % 512;
        if (m_size == 0)
        {
            m_size = 1;
        }
        // b_log("malloc %d bytes\r\n", m_size);
        addr[i] = bMalloc(m_size);
        TEST_ASSERT_NOT_EQUAL(NULL, addr[i]);
    }
    bHalDelayMs(5);
    b_log("size:%d\r\n", bGetFreeSize());
    for (i = 0; i < 10; i++)
    {
        bFree(addr[i]);
        addr[i] = NULL;
    }
}

void bMallocFailedHook()
{
    b_log("===========================================\r\n");
    while (1)
        ;
}

int main(void)
{
    int i = 0;
    UnityBegin("test_malloc_ut.c");

    for (i = 0; i < 3000; i++)
    {
        RUN_TEST(testbMallocbFree);
    }

    return (UnityEnd());
}

#endif
