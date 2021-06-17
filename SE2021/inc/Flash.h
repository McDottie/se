/**
* @file		lcd.h
* @brief	Contains the flash API.
* @version	1.1
* @date		12 Jan 2020
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef FLASH_H_
#define FLASH_H_

/** @defgroup IAP_RETURN_CODES
 * @{
 */
# define IAP_CMD_SUCESS 0
# define IAP_INVALID_COMMAND 1
# define IAP_SRC_ADDR_ERROR 2
# define IAP_DST_ADDR_ERROR 3
# define IAP_SRC_ADDR_NOT_MAPPED 4
# define IAP_DST_ADDR_NOT_MAPPED 5
# define IAP_COUNT_ERROR 6
# define IAP_INVALID_SECTOR 7
# define IAP_SECTOR_NOT_BLANK 8
# define IAP_SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9
# define IAP_COMPARE_ERROR 10
# define IAP_BUSY 11
/**
 * @}
 */

/*!Sectors sizes*/
#define SECTOR_SIZE 32768

/*!Sector 28 adrress*/
#define sector28  0x00070000

/*!Sector 29 adrress*/
#define sector29  0x00078000

/**
 * @brief Deletes the contents of a sector, or multiple sectors, of FLASH. To delete only one sector, the same sector number must be used for both parameters. 
 * */
unsigned int FLASH_EraseSectors(unsigned int startSector, unsigned int endSector);

/**
 * @brief Write the data block. This Function can only access sectors from sector 16 to 29
 * @param  srcAddr data block pointer
 * @param size block size
 * @param dstAddr Flash address 
 */
unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Compares the contents of the data block
 * @param srcAddr data block reference
 * @param size size in bytes of the data block
 * @param dstAddr data block to be compared to
 */
unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Writes an array to a data block. This Function can only access sectors from sector 16 to 29
 * @param  srcAddr array to be written
 * @param array_size size of the array
 * @param size block size
 * @param dstAddr Flash address 
 */
unsigned int FLASH_WriteArray( void *dstAddr, int srcAddr[],int array_size, unsigned int size);

int FLASH_WriteBlock(void *dstAddr, void *srcAddr, unsigned int size);


unsigned int FLASH_BlanckCheck(unsigned int startSector, unsigned int endSector);

#endif /* FLASH_H_ */
