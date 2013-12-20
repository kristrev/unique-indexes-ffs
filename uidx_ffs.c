#include <stdio.h>
#include <string.h>

#define NUM_INDEXES 256

#if (NUM_INDEXES < 32)
    #define NUM_ELEMENTS 1
//If number of indexes is not divisble by 32, need to add one more element to
//store the "remainder-indexes"
#elif (NUM_INDEXES & 0x1F)
    #define NUM_ELEMENTS ((NUM_INDEXES>>5) + 1)
#else
    #define NUM_ELEMENTS (NUM_INDEXES>>5)
#endif

#define NO_AVAILABLE_INDEX 0

//Optimization. This value stores the 
static unsigned int last_unset_index;

//Returns first available index or zero if no found
static int get_available_index(unsigned int *indexes){
    int i;
    int retval = 0;
    int offset = 0;

    //Check for last unset index
    if(last_unset_index){
        retval = ffs(indexes[last_unset_index]);
        indexes[last_unset_index] ^= (1 << retval - 1);
        retval += (32*last_unset_index);

        //Keep filling this index until it is full
        if(!indexes[last_unset_index])
            last_unset_index = 0;
    }

    for(i = 0; i<NUM_ELEMENTS; i++){
        //A value of zero means that the indexes represented by this int are all
        //used
        if(!indexes[i]){
            offset += 32;
            continue;
        }

        //This operation can be done using some bit-fiddling too, but this code
        //is more readable
        retval = ffs(indexes[i]);

        //Least significant bit has position 1, but "index" 0 when setting
        indexes[i] ^= (1 << (retval - 1));
        retval += offset;
        break;
    }
  
    //Make sure we don't return any indexes larger than our limit (dont care if
    //they are set or not, so I dont have to update the loop)
    if(retval > NUM_INDEXES)
        return 0;
    else
        return retval;
}

//index is assumed to be what is returned by get_available_index. So for example
//index 32 is stored in bit 31 (when setting/unsetting least significant bit has
//index zero).
static void unset_index(unsigned int *indexes, unsigned int index){
    //Find the element index (multiple of 32)
    unsigned int element_index = (index - 1) >> 5;

    //Find the bit
    unsigned int element_bit = (index - 1) & 0x1F;

    if(element_index >= NUM_ELEMENTS){
        printf("Index is out of range\n");
        return;
    }

    printf("%u %u\n", element_index, element_bit);

    indexes[element_index] ^= (1 << element_bit);
    last_unset_index = element_index;
}

int main(void){
    unsigned int indexes[NUM_ELEMENTS];
    unsigned int index = 0, i = 0;
    last_unset_index = 0;

    printf("# idxs %u # elements in array %u\n", NUM_INDEXES, NUM_ELEMENTS);

    //Remember ffs gives me the index of the first set bit. So, bit set = index
    //available and bot not set = index unavailable.
    memset(indexes, 0xFF, sizeof(indexes));
    for(i=0; i<=NUM_INDEXES; i++){
        index = get_available_index(indexes);
        printf("Index: %u %u\n", index, i);
    }

    unset_index(indexes, 33);
    index = get_available_index(indexes);
    printf("Index: %u\n", index);

}
