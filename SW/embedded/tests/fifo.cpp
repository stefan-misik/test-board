#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "../tools/fifo.h"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

TEST_GROUP(Fifo)
{
    fifo_t m_fifo_object;
    char m_buffer[64];

    TEST_SETUP()
    {
        for (unsigned n = 0; n < (sizeof(m_buffer) / sizeof(m_buffer[0])); ++ n)
        {
            m_buffer[n] = 0;
        }

        fifo_init(&m_fifo_object, &(m_buffer[16]), 8);
    }
};

TEST(Fifo, emptyAfterInitialization)
{
    fifo_init(&m_fifo_object, m_buffer, 32);
    LONGS_EQUAL(32, fifo_free(&m_fifo_object));
    LONGS_EQUAL(0, fifo_enqueued(&m_fifo_object));
}

TEST(Fifo, insertSingleByte)
{
    LONGS_EQUAL(1, fifo_write(&m_fifo_object, (const void *)"a", 1));
    LONGS_EQUAL(7, fifo_free(&m_fifo_object));
    LONGS_EQUAL(1, fifo_enqueued(&m_fifo_object));
    BYTES_EQUAL('a', m_buffer[16]);
}

/* REQUIREMENTS:
 * - Initialize the empty fifo with specified underlying buffer
 * - Empty circular fifo implies head = tail
 * - Querying the state of the fifo
 * - Single char can be pushed to the fifo, return failure when fifo is full
 * - Single char can be pulled out of fifo, return failure when fifo is empty
 * 
 */