#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

#include "../tools/fifo.h"

int main(int ac, char **av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

TEST_GROUP(Fifo)
{
    fifo_t m_fifo_object;
    char m_buffer[64];
    char *m_buffer_start;

    TEST_SETUP()
    {
        m_buffer_start = &(m_buffer[16]);

        for (unsigned n = 0; n < (sizeof(m_buffer) / sizeof(m_buffer[0])); ++n)
        {
            m_buffer[n] = 0;
        }

        fifo_init(&m_fifo_object, m_buffer_start, 6);
    }
};

TEST(Fifo, emptyAfterInitialization)
{
    // Prepare
    fifo_init(&m_fifo_object, m_buffer, 32);
    // Test, Check
    UNSIGNED_LONGS_EQUAL(31, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(0, fifo_enqueued(&m_fifo_object));
}

TEST(Fifo, insertSingleByte)
{
    // Prepare
    // Test, Check
    UNSIGNED_LONGS_EQUAL(1, fifo_write(&m_fifo_object, (const void *)"a", 1));
    UNSIGNED_LONGS_EQUAL(4, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(1, fifo_enqueued(&m_fifo_object));
    MEMCMP_EQUAL("a", m_buffer_start, 1);
}

TEST(Fifo, insertMultipleBytes)
{
    // Prepare
    // Test, Check
    UNSIGNED_LONGS_EQUAL(3, fifo_write(&m_fifo_object, (const void *)"bcd", 3));
    UNSIGNED_LONGS_EQUAL(2, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(3, fifo_enqueued(&m_fifo_object));
    MEMCMP_EQUAL("bcd", m_buffer_start, 3);
}

TEST(Fifo, readSingleByte)
{
    char buffer[8];
    // Prepare
    fifo_write(&m_fifo_object, "xq", 2);
    // Test
    UNSIGNED_LONGS_EQUAL(1, fifo_read(&m_fifo_object, buffer, 1));
    // Check
    UNSIGNED_LONGS_EQUAL(4, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(1, fifo_enqueued(&m_fifo_object));
    MEMCMP_EQUAL("x", buffer, 1);
}

TEST(Fifo, readMultipleBytes)
{
    char buffer[8];
    // Prepare
    fifo_write(&m_fifo_object, "xqrt", 4);
    // Test
    UNSIGNED_LONGS_EQUAL(3, fifo_read(&m_fifo_object, buffer, 3));
    // Check
    UNSIGNED_LONGS_EQUAL(4, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(1, fifo_enqueued(&m_fifo_object));
    MEMCMP_EQUAL("xqr", buffer, 3);

}

TEST(Fifo, multipleReadsAndWrites)
{
    char buffer_1[8];
    char buffer_2[8];
    // Prepare
    fifo_write(&m_fifo_object, "ym", 2);
    fifo_read(&m_fifo_object, buffer_1, 1);
    // Test
    UNSIGNED_LONGS_EQUAL(2, fifo_write(&m_fifo_object, "no", 2));
    UNSIGNED_LONGS_EQUAL(2, fifo_read(&m_fifo_object, buffer_2, 2));
    // Check
    UNSIGNED_LONGS_EQUAL(4, fifo_free(&m_fifo_object));
    UNSIGNED_LONGS_EQUAL(1, fifo_enqueued(&m_fifo_object));
    MEMCMP_EQUAL("y", buffer_1, 1);
    MEMCMP_EQUAL("mn", buffer_2, 2);

}

/* REQUIREMENTS:
 * - Initialize the empty fifo with specified underlying buffer
 * - Empty circular fifo implies head = tail
 * - Querying the state of the fifo
 * - Single char can be pushed to the fifo, return failure when fifo is full
 * - Single char can be pulled out of fifo, return failure when fifo is empty
 * 
 */