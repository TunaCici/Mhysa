//
// Created by Tuna Cici on 17.05.2023.
//

/*
 * ATS - Address Translation Scheme
 *
 * Source: GWU OS: Page-tables by Gabrial Parmer
 *
 * Virtual Addresses are divided into:
 * Page Number (p): The idx of the page.
 * Page Offset (d): The offset added to the base page[idx] address
 *
 * An [Unrealistic] Example 64-bit Virtual Address (PAGE_SIZE is 4096 Bytes (12-Bits))
 *
 * ________________________________________________ | ________________
 *
 * 000010100101001101010101111111111000000010010100 | 0000000111100011  (Binary)
 *                      A5355FF8094                 |       01E3        (Hexadecimal)
 *
 * Page Number (p) is 11353041371284 (Decimal)
 * Page Offset (d) is 483 (Decimal)
 */

#include <bit>
#include <cstdint>
#include <tuple>

/*
 * The values below are hard-coded, and they are strictly tied together.
 * If either SYSTEM_BIT or PAGE_SIZE changes the PAGE_MASK needs change as well!!!
 */
#define SYSTEM_BIT 64u /* Two options: 32-bit or 64-bit */
#define PAGE_SIZE 4096u /* Bytes */
#define PAGE_OFFSET_MASK 0x0000000000000FFF /* Last 12-bits */
#define PAGE_INDEX_MASK 0xFFFFFFFFFFFFF000 /* First 54-bits */

/* I know that I should not 'return' two uint64's buttttt LETS DO IT ANYWAY... */
std::tuple<std::uint64_t, std::uint64_t>
translate_64(const std::uint64_t &virt_addr) {
    std::uint64_t page_index = {};
    std::uint64_t page_offset = {};

    page_offset = virt_addr & PAGE_OFFSET_MASK;
    page_index = virt_addr & PAGE_INDEX_MASK;
    page_index = page_index >> (std::bit_width(PAGE_SIZE) - 1u);

    return {page_index, page_offset};
}

