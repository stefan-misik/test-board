/**
 * \file    keypad.h
 * \author  Stefan Misik
 * \brief   Driver for keypad
 *     
 */

#ifndef KEYPAD_H
#define KEYPAD_H


/**
 * @brief Initialize the keypad driver
 * 
 */
void keypad_init(
    void
);

/**
 * @brief Get pressed key
 * 
 * @return Character representing pressed key, null character if no key is
 *         pressed
 */
char keypad_get(
    void
);


#endif /* KEYPAD_H */

