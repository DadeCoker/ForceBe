
#include "../port.h"
#include "b_config.h"
#include "b_os.h"

#if (defined(_UNITY_ENABLE) && (_UNITY_ENABLE == 1))

#define KEY_NUMBER (30)
#define KEY_LEN (10)
#define VALUE_LEN (32)

typedef struct
{
    char key[KEY_LEN + 1];
    char value[VALUE_LEN];
} KVUnit_t;

static KVUnit_t sgTestTable[KEY_NUMBER];

bKV_INSTANCE(gKVInstance, bTESTFLASH, 0, 4096 * 10, 4096);

void setUp(void)
{
    static uint8_t init_f = 0;
    if (init_f == 0)
    {
        init_f = 1;
        port_init();
        bInit();
        bKVInit(&gKVInstance);
    }
}

void tearDown(void)
{
    ;
}

void testbKVSetValue(void)
{
    /* All of these should pass */
    int i = 0;
    for (i = 0; i < KEY_NUMBER; i++)
    {
        TEST_ASSERT_EQUAL(
            0, bKVSetValue(&gKVInstance, sgTestTable[i].key, sgTestTable[i].value, VALUE_LEN));
    }
}

void testbKVGetValueLength(void)
{
    int i = 0, len = 0;
    for (i = 0; i < KEY_NUMBER; i++)
    {
        len = bKVGetValueLength(&gKVInstance, sgTestTable[i].key);
        TEST_ASSERT_EQUAL(VALUE_LEN, len);
    }
}

void testbKVGetValue(void)
{
    /* All of these should pass */
    int     i = 0;
    uint8_t value[VALUE_LEN];
    for (i = 0; i < KEY_NUMBER; i++)
    {
        bKVGetValue(&gKVInstance, sgTestTable[i].key, value, VALUE_LEN, NULL);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(value, sgTestTable[i].value, VALUE_LEN);
    }
}

int main(void)
{
    int i = 0, j = 0, c = 0;
    UnityBegin("test_kv_ut.c");

    memset(sgTestTable, 0, sizeof(sgTestTable));

    for (i = 0; i < KEY_NUMBER; i++)
    {
        sprintf(sgTestTable[i].key, "%d", i);
        for (j = 0; j < VALUE_LEN; j++)
        {
            sgTestTable[i].value[j] = i + j;
        }
        printf("key:%s\r\n", sgTestTable[i].key);
    }

    for (c = 0; c < 1000; c++)
    {
        RUN_TEST(testbKVSetValue);
        RUN_TEST(testbKVGetValueLength);
        RUN_TEST(testbKVGetValue);

        for (i = 0; i < KEY_NUMBER; i++)
        {
            for (j = 0; j < VALUE_LEN; j++)
            {
                sgTestTable[i].value[j] = j + c + 1;
            }
        }
    }
    return (UnityEnd());
}

#endif
