/**
* name: Vlastna alokacia pamate 
* author: Michal Slovik
* version: 1.0
* program: 
*/

#include <stdio.h>
#include <string.h>

/*global definition*/
#define HEAD_SIZE 4		//velkost hlavicky
#define FULL_OF_MEMORY(num)	( 1 & (num))	// sluzi pri prechadzani celej pamate
#define IS_ODD(num)	((num) & 1)		// & bitovy sucin, test ci je num-cislo neparne
#define NEG(num)	((num) &~1)		// bitova negacia, namiesto 1/0


//#define DEBUG 

void *start_of_memory = NULL;	// ukazovatel na zaciatok pamate
/*other function*/

// @ ptr - ukazovatel, ktory ukazuje na zaciatok celej pamate
// @ size - velkost celej pamate s ktorou mozeme potom pracovat
void memory_init(void *ptr, unsigned int size)
{

	if ( ptr != NULL )			// ak je pamat prazdna
	{
		start_of_memory = ptr;  // zaciatok pamate ukazuje na tento zaciatok
		int *tmp = (int*) start_of_memory;

		*tmp = IS_ODD(size) ? (size - 1 ) : (size ); // do hlavicky dava iba parnu hodnotu, ak je posledny bit 1 == OBSADENE, 0==FREE
		*(tmp + 1) = *tmp - ( 2 * HEAD_SIZE);
	}
}

void *memory_alloc(unsigned int size)
{
	int Rest_size;	// zvysok pamate
	// ak je neparna velkost v hlavicke bude mat ulozeny udaj [size + 1]
	int change_size = size + IS_ODD(size);	// ak je neparna velkost 

	char *tmp = (int*)start_of_memory;
	char *size_of_memory = tmp + *(int*)tmp;
	int  *block_of_memory = (int*)tmp + 1;

	

	// ukazuje hned na prvu hlavicku
	tmp = block_of_memory;

	while ( (FULL_OF_MEMORY(*block_of_memory) ) || ( (change_size) > *block_of_memory ))
	{
		tmp += HEAD_SIZE + *block_of_memory - FULL_OF_MEMORY(*block_of_memory);
		block_of_memory = tmp;
		// ak uz je mimo pamate
		if (block_of_memory >= size_of_memory )
		{
			return NULL;
		}
	}
	// zvysok rest_size vlozi zvysok pamate
	// teda vytvori dalsi blok pamate nastaveny ako prazdny
	Rest_size = *block_of_memory - HEAD_SIZE - change_size;

	if ( (size_of_memory) >= (tmp + HEAD_SIZE + change_size) )
	{
		if ( Rest_size < (HEAD_SIZE / 2) )
		{
			*block_of_memory = *block_of_memory | 1;
			return block_of_memory + 1;
		} else {
			*block_of_memory = change_size;
			*block_of_memory = *block_of_memory | 1;	
			// na zostatok pamate vlozi prazdny blok
			// ptr , ktory ukazuje na zaciatok alokovanej pamate
			int *zero_ptr = block_of_memory;
			tmp += HEAD_SIZE + (*(int*)tmp -1 );	// posunie o velkost hlavicky plus velkost samotneho bloku 
			block_of_memory = tmp;
			// do hlavicky vloz hodnotu
			*block_of_memory = Rest_size;
#ifdef DEBUG
	printf("Allok: uspesne alokoval pamat\n");
#endif
			
			return zero_ptr + 1; // ukazovatel na zaciatok alokovanej pamate
		}
	}
	// v pripade neuspeneho alokovania pamate vrat NULL
	return NULL;
}


// FREE
// funkcia na uvolnenie pamate
// funkcia vyhlada pointer ,ktore ukazuje do pamate(pametoveho bloku)
// pozrie sa na hlavicku predchádzajucu tomuto bloku pamate, 
// ak nie je posledny bit volny(volny==1 , obsadene==0) uvolni ho
// 
// po uvolneni bloku dochadza este ku fragmentacii pamete,
// takze volne bloky sa usporiadavaju a spajaju dohromady ( ak je to mozne )
// aby sme mohli poskytnut co najviac pamate
 int memory_free(void *valid_ptr)
{
	// void == 1 byte
	// char == 1 byte
	// int  == 4 byte 
	int count = 0;	// kontrola uspesneho vyprazdenia pamate
	char *tmpChar = (char*)start_of_memory;
	char *size_of_memory = tmpChar + *(int*)start_of_memory;

	// defaultne pointre aby sme nemuseli stale pretypovovat
	char *next_p_char;
	int *heavy_next;
	int *next_p_int;

	// kontrola ci prichodzi pointer je spravny
	// kontroluje si prichodi pointer nie je mimo rozsahu pamate
	if  (((int*)tmpChar > valid_ptr) && (size_of_memory < valid_ptr))
		return 1;

	next_p_int = valid_ptr; // ukazuje na pointer
	next_p_int = next_p_int - 1;	//a posunie sa na hlavicku
	next_p_char = next_p_int;	// a ukazuje nan char

	// tmpChar - ukazuje na start_of_memory
	// 
	// postupne prechadza pamat, tak ze skace od hlavicky ku hlavicke
	// a hlada konkretny ukazovatel s hlavickou
	// ak by presiel az za koniec pamate a nenasiel ho vrati null
	for ( tmpChar = tmpChar + HEAD_SIZE ; tmpChar < size_of_memory; ) 
	{
		if ( next_p_char == tmpChar )
		{
			count++;
			// v pripade ze ho uz najde moze skoncit prehladavanie
// break;?
		}
		tmpChar += *(int*)tmpChar - FULL_OF_MEMORY(*(int*)tmpChar) + HEAD_SIZE;
	}

	// nenasiel pointer na danej hlavicke
	if (count == 0)
	{
		return 1;  // a vracia neuspech
	}

	// negacia - sluzi na ako bitova negacia ( jednickovy doplnok ) ( heurot str. 258 )
	// prevracia nulove bity na jednickove a naopak
	// takze prevrati posledny bit, ktory urcuje obsadenost pamate
	*next_p_int = NEG(*next_p_int);
	next_p_char += *next_p_int + HEAD_SIZE;
	heavy_next = next_p_char;	//pomocny pointer

	//fragmentacia po kazdom uvoleni bloku
	// to znamena su dva bloky nasledujuce rovno po sebe 
	// spoji ich do jedneho suvisleho bloku s jednou hlavickou oznacujucou ich ako volne bloky
	while (( (size_of_memory) > heavy_next ) && !(IS_ODD(*heavy_next))) 
	{
		*next_p_int +=  *heavy_next + HEAD_SIZE;
		next_p_char += *heavy_next + HEAD_SIZE;
		heavy_next = next_p_char;
	}
	// uspesne uvolnil pamat
#ifdef DEBUG
	printf("Free: uspesne uvolnil pamat\n");
#endif
	return 0;
 }

// funkcia na kontrolu pointra
// skontroluje ci ide o platny smernik
int memory_check(void *ptr)
{
	char *start_Char = (char*)start_of_memory;
	char *size_of_memory = start_Char + *(int*)start_of_memory;

	char *next_p_char;
	int *heavy_next;
	int *next_p_int;

	// ak ide o neplatny smernik ukonci a vracia 0
	if (!ptr) // nesmia byt == NULL
		return 0;

	// ak by smernik ukazoval mimo pamate 
	if  (((int*)start_Char > ptr) && (size_of_memory < ptr))
		return 0;

	// prechadza postupne pamatov  po hlavickach
	// ako nahle narazi na odpovedajuci smernik
	// zisti ci bola tato hlavicka v minulosti allokovana
	// ak ano vrati 1 inak vracia 0

	next_p_int = ptr; // ukazuje na pointer
	next_p_int = next_p_int - 1;	//a posunie sa na hlavicku
	next_p_char = next_p_int;	// a ukazuje nan char

	// prechadzanie po pamati po hlavickach
	for ( start_Char = start_Char + HEAD_SIZE ; start_Char < size_of_memory; ) 
	{
		if ( next_p_char == start_Char)
		{
			// pozrie a zisti ci posledny bit bol/alebo je zapleny
		if ( NEG(*next_p_int) != *next_p_char) 
		{
			
#ifdef DEBUG
	printf("Check: GOOD nasel pointer\n");
#endif						
			
			return 1;
			}
		}
		// navysenie o velkost bloku 
		start_Char += *(int*)start_Char - FULL_OF_MEMORY(*(int*)start_Char) + HEAD_SIZE;
	}

#ifdef DEBUG
	printf("Check: BAD nenasiel spravny pointer\n");
#endif
	
	// nenasiel ziadny vyhovujuci pointer a vracia neuspech
	return 0;
}

/*main function*/
int main()
{
	char *ptr;
	char region[100];
	memory_init(region, 100);

	ptr = (char*)memory_alloc(10);
	
	memory_check(ptr);
	
	if ( ptr )
	{
		memset(ptr, 0, 10);
	}
	
	if ( ptr )
	{
		memory_free(ptr);
	}
	
	memory_check(ptr);

	return 0;

}
