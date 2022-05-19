/** Name: Andrea Li
 * Student ID: 13990268
 * Course: Datastructuur
 * Institution: University of Amsterdam
 *
 * hash_func.c:
 * Functions in this file are used to calculate the hash value of a input
 * string. These functions will be used in main.c
 *
*/

/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(unsigned char *str)
{
    return (unsigned long)*str;
}
/**
 * This function calculates the hash value of a input string.
 *
 * @param key input string
 * @return hash value
 */
unsigned long hash_function(unsigned char *key)
{
    unsigned h = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        h = h * 31 + (unsigned)key[i];
    }
    return h;
}
