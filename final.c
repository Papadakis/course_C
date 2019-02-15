/*A8anasios Papadakis	993 papadaki@uth.gr*/

/* Biblio8hkes pou xrhsiopoioume. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* To onoma tou boh8htikou arxeiou. */
#include "getWord.c"

#define CHARACTERS 26	/* Plh8os grammatwn */ 
#define INTOLETTER 97	/* Boh8htikh metavlhth. ( int->char ) */

/* Lista sthn opoia 8a apo8hkeutoun oi lexeis tou lexikou. */
typedef struct wordNode
{
    char *string;
    struct wordNode *next;
    
} Word;

/* Lista sthn opoia apo8hkeuontai oi kathgories twn lexewn. */
typedef struct categoryNode
{
    char *antiproswpos;	//Antiproswpos.
    int numberOfWords;	//To plh8os twn lexwn pou einai apo8hkeumenes se authn thn kathgoria.
    Word **lexeis;	//Pinakas deiktwn pros tis lexeis ths kathgorias.
    struct categoryNode *next;
    
} Category;


/*Prototupa Sunarthsewn: 
 *epexhxhsh sthn arxh ka8e sunarthshs,oxi kata thn roh tou kwdika.*/
int validateArguments( int, char ** );	//Elegxoi gia to input tou xrhsth.
void showExecutionGuide();	//Boh8htikh sunarthsh gia thn emfanish mhnumatwn.
Word *initLexicon( int, char * );	//Arxikopoihsh listas-lexikou.
Category *initCategoryList( Word * );	//Arxikopoihsh listas kathgoriwn.
Category *createCategoryList( char, Category * );	//Kataskeuh newn kathgoriwn me vash ton antiproswpo.
Category *searchCategoryList( Category *, int *, char *, Word * );	//Diatrexei thn lista gia na brei enan komvo me ton idio antiproswpo.
Category *addCategoryToList( Category *, char *, Word * );	//Dhmiourgia kainourgiwn komvwn listas tupou Category.
void startGame( int, Category * );	//Sunarthsh pou periexei thn epilogh grammatos tou paikth se ka8e guro kai ton katallhlo elegxo pou akolou8ei gia thn epivevaiwsh tou nikhth h htthmenou. 
Category *findCategoryWithMostWords( Category * );	//Anixneush tou komvou me tis perissoteres lexeis ka8ws kai diagrafei tous axrhstous komvous.
void deleteCategoryNode( Category * );	//Free ,thn dunamika desmeumenh mnhmh twn klasewn isodunamias.
void deleteWordList( Word * );	//Free ,thn dunamika desmeumenh mnhmh twn lexewn tou lexikou.


/* MAIN */
int main( int argc, char *argv[] )
{
    Word *wordHead;
    Category *categoryHead;
    char option;	//Boh8htikh metavlhth pou apo8hkeuetai h epilogh tou xrhsth an 8elei na sunexisei h oxi.
    int rounds;	//Boh8htikh metavlhth pou apo8hkeuei tous gurous pou dikaioutai o paikths.
    
    /* Elegxos gia to swsto input. */
    if ( !validateArguments( argc, argv ) )
        return 0;
    
    /* Arxikopoihsh ths listas-lexikou. */
    wordHead = initLexicon( atoi( argv[1] ), argv[3] );
    
    /* Elegxos gia to mege8os ths lexhs. p.x. To 25 pou den uparxei lexh ,ara prepei na upologisoume kai thn periptwsh pou den uparxei lexh! */
    if ( wordHead == NULL )
    {
        printf( "There are no words of %d length in the dictionary.\n", atoi( argv[1] ) );
        return 0;
    }
    
    /* Arxikopoioume thn voh8htikh metavlhth. */
    rounds = atoi( argv[ 2 ] );
    
    /* Emfanish katallhlwn mhnumatwn meta to telos ka8e gurou. */
    do
    {
        categoryHead = initCategoryList( wordHead );	//Arxikopoihsh ths listas me tis kathgories twn lexewn.
        startGame( rounds, categoryHead );	//Klhsh sunarthshs.

        do
        {
            printf( "Play again? (y/n): " );
            scanf( " %c", &option );
            
            if ( ( option != tolower( 'Y' ) ) && ( option != tolower( 'N' ) ) )
                printf( "Please enter y for yes or n for no.\n" );
            
        } while ( ( option != tolower( 'Y' ) ) && ( option != tolower( 'N' ) ) );
        
        

    } while ( option == tolower( 'Y' ) );
    
   /* Free dunamika desmeumenhs mnhnhs. */
    deleteWordList( wordHead );
    
    return 0;
}

/* Sunarthsh pou epexergazetai ta orismata kai elegxei oles tis parametrous.
* argv[1]=mhkos lexhs.
* argv[2]=plh8os gurwn.
* argv[3]=onoma arxeiou ( lexiko ). */
int validateArguments( int totalArguments, char *argumentsList[] )
{
 
    int wordLength;
    int validLength = 0; //Boh8htikh metavlhth. ( 0=F, 1=T )
    int rounds;
    
    /* Elegxoi gia to plh8os twn orismatwn. */
    if ( totalArguments < 4 )
    {
        printf( "Insufficient arguments.\n" );
        showExecutionGuide();
        
        return 0;
    }
    else if ( totalArguments > 4 )
    {
        printf( "Too many arguments.\n" );
        showExecutionGuide();
        
        return 0;
    }
    else
    {
	/* Elegxos gia to mege8os ths lexhs. */
        do
        {
	    /* Ypologismos mhkous lexhs me thn voh8eia ths atoi. */
            wordLength = atoi( argumentsList[ 1 ] );
            
            if ( !( ( wordLength >= 1 ) && ( wordLength <= 29 ) && ( wordLength != 26 ) && ( wordLength != 27 ) ) )
            {
                printf( "There are no words of length %d in the dictionary.\nSpecify a different length: ", wordLength );
                scanf( "%s", argumentsList[ 1 ] );
                
            }
            else
                validLength = 1;
            
        } while ( !validLength );
        
	/* Elegxos gia to plh8os twn gurwn. */
        do
        {
            rounds = atoi( argumentsList[ 2 ] );
            
            if ( rounds < 1 )
            {
                printf( "You may not have fewer than one turns.\nSpecify a different number of turns: " );
                scanf( "%s", argumentsList[ 2 ] );
            }
            
        } while ( rounds < 1 );
        
    }
    
    return 1;
}

/* Emfanish katallhlwn mhnumatwn. */
void showExecutionGuide()
{
    printf( "./hangman LENGTH TURNS DICTIONARY\n" );
    printf( "\tLENGTH: requested length of mystery word. Must be >1, <30, !=26, !=27\n" );
    printf( "\tTURNS: requested number of turns. Must be >0\n" );
    printf( "\tDICTIONARY: name of a dictionary file\n" );
    
}

/* Arxikopoihsh listas-lexikou kai epistrefei enan deikth ( root ) -h kefalh ths. */
Word *initLexicon( int wordLength, char *filename )
{
    char *lexh;
    Word *root = NULL;
    
    /*Diavazei ka8e lexh tou arxeiou,thn elegxei me to swsto mhkos lexhs kai thn apo8hkeuei sthn lista.*/
    while ( ( lexh = getWord( filename ) ) )
    {
        if ( wordLength == strlen( lexh ) )
        {
            Word *word = ( Word * ) malloc( sizeof( Word ) );
    
	    /*Elegxos gia swsth desmeush mnhmhs.*/
	    if ( word == NULL ) 
	    {
	      printf( "No memory!\n" );
	      exit ( 1 );  
	    }
	    
	    /* Apo8hkeush lexhs kai dhmiourgia epomenou komvou. */
            word->string = lexh;
            word->next = root;
            
            root = word;
        }
    }
    
    return root;
        
}

/* Arxikopoihsh listas kathgoriwn( klaseis isodunamias ). */
Category *initCategoryList( Word *lexicon )
{
    Word *curr;
    Category *root;
    int wordLength, i;
    
    /* Arxikopoihsh. */
    root = NULL;
    
    /* Desmeush xwrou. */
    Category *category = ( Category * ) malloc( sizeof( Category ) );
    
    if ( category == NULL ) 
    {
      printf( "No memory!\n" );
      exit ( 1 );  
    }
    
    /* Arxikopoihsh. */
    wordLength = strlen( lexicon->string );
    
    /* Desmeush xwrou gia ton kainourgio pinaka ( deiktwn ). */
    category->antiproswpos = ( char * ) malloc( sizeof( char ) * ( wordLength + 1 ) );
    
    if ( category->antiproswpos == NULL ) 
    {
      printf( "No memory!\n" );
      exit ( 1 );  
    }
    
    /* Gemisma ths listas apo paules. */
    for ( i = 0; i < wordLength; i++ )
        category->antiproswpos[i] = '-';
    
    category->antiproswpos[i] = '\0';
    /* Arxikopoihsh. */
    category->numberOfWords = 0;
    
    /* Prospelash kathgorias gia ton upologismo tou plh8ous twn lexewn se authn. */
    for ( curr = lexicon; curr != NULL; curr = curr->next )
        category->numberOfWords++;
    
    /* Dunamikh desmeush gia ton pinaka pou periexei deiktes pros tis lexeis. */
    category->lexeis = ( Word ** ) malloc( sizeof( Word * ) * category->numberOfWords );
    
    if ( category->lexeis == NULL ) 
    {
      printf( "No memory!\n" );
      exit ( 1 );  
    }
    
    /* Pros8hkh deiktwn ston pinaka. */
    for ( curr = lexicon, i = 0; curr != NULL; curr = curr->next, i++ )
        category->lexeis[ i ] = curr;
    
    category->next = NULL;
    root = category;
    
    /* Epistrofh kefalhs. */
    return root;
}

/* Dhmiourgia newn kathgoriwn me vash ton antiproswpo , ka8ws epistrefei thn plh8esterh kathgoria. */
Category *createCategoryList( char guessedLetter, Category *oldHead )
{
    Category *root = NULL; //Arxikopoihsh gia thn nea lista kathgoriwn.
    int i, j;	//Boh8htikes metavlhtes.
    int wordLength = strlen( oldHead->antiproswpos );	//Arxikopoihsh tou mege8ous tou antiproswpou.
    int found = 0;	//Boh8htikh metavlhth.
    char *representative;	//Antiproswpos.
    
    /* Desmeush mnhmhs gia ton antiproswpo. */
    representative = ( char * ) malloc( sizeof( char ) * ( wordLength + 1 ) );
    
    if (representative == NULL) 
    {
      printf( "No memory!\n" );
      exit ( 1 ); 
    }
    
    /* Gia ka8e lexh pou einai apo8hkeumenh ston komvo ths arxikhs listas. */
    for ( i = 0; i < oldHead->numberOfWords; i++ )
    {
        /* Elegxos ka8e grammatos ths apo8hkeumenhs lexhs. */
        for ( j = 0; j < wordLength; j++ )
        {
            /* An o xrhsths mantepsei swsta (dld ta duo grammata tairiazoun),tote antiproswpos=mantepsia. */
            if ( oldHead->lexeis[i]->string[j] == guessedLetter )
                representative[j] = guessedLetter;
            else
                representative[j] = oldHead->antiproswpos[j];
        }
        
        representative[j] = '\0';
        
        /* Klhsh sunarthshs. 8eloume na epistrepsei 2 apotelesmata a)root kai to b)found ( thn dieu8unsh ). */
        root = searchCategoryList( root, &found, representative, oldHead->lexeis[i] );
        
        /* Se periptwsh pou den vre8ei kamia kathgoria me to sugkekrimeno antiproswpo. */
        if ( found == 0 )
        {
	    /* Klhsh sunarthshs. */
            root = addCategoryToList( root, representative, oldHead->lexeis[i] );
        }
        
        found = 0;
    }
    
     /* Free dunamika desmeumenhs mnhmhs. */
     deleteCategoryNode( oldHead );
    
    /* Epistrofh deikth se category me to megisto mege8os. */
    return root;
}

/* Diatrexei thn lista gia na brei enan komvo pou na exei ton idio antiproswpo.
* An bre8ei tote apo8hkeuontai oi aparaithtes plhrofories,epistrefei thn arxh tou
*ths listas me found="True" , alliws epistrefei pali thn arxh alla me found="False". */
Category *searchCategoryList( Category *head, int *found, char *representative, Word *lexh )
{
    Category *curr;
    
    /*Prospelavnei thn nea lista.*/
    for ( curr = head; curr != NULL; curr = curr->next )
    {
        /*Elegxos gia thn periptwsh pou ena pedio exei idio antiproswpo.*/
        if ( strcmp( curr->antiproswpos, representative ) == 0 )
        {
            curr->numberOfWords++;
            /*Auxhsh twn lexewn se auton ton komvo ths listas kai tautoxrona dunamikh desmeush mnhmhs-elgxos.*/
            curr->lexeis = ( Word ** ) realloc( curr->lexeis, sizeof( Word * ) * curr->numberOfWords );
	    
	    if (curr->lexeis == NULL) 
	    {
	      printf( "No memory!\n" );
	      exit ( 1 ); 
	    }

            curr->lexeis[ curr->numberOfWords - 1 ] = lexh;
            *found = 1; //Parametros pou shmainei "True".
            
            return head;
        }
    }
    
    *found = 0; //Parametros pou shmainei "False".
    
    return head;
}

/* Dhmiourgia kainourgiwn komvwn listas ( tupou Category ) me thn boh8eia tou antiproswpou. Epistrofh kainourgia kefalhs. 
* Kaleitai kata thn dhmiourgia listas ( createCategoryList ). */
Category *addCategoryToList( Category *head, char *representative, Word *lexh )
{
    /* Dunamikh desmeush xwrou gia thn lista mas( klaseis isodunamias ). */
    Category *curr = ( Category * ) malloc( sizeof( Category ) );
    
    if (curr == NULL) 
    {
      printf( "No memory!\n" );
      exit(1) ;
    }
    
    /* Arxikopoihsh tou length me to mhkos tou antiproswpou (+1) gia to /0. */
    int length = strlen( representative );
    
    /* Desmeush katallhlou xwrou gia thn dhmiourgia kainourgias kathgorias (char). */
    curr->antiproswpos = ( char * ) malloc( sizeof( char ) * ( length+1 ) );
    
    if (curr->antiproswpos == NULL) 
    {
      printf( "No memory!\n" );
      exit(1) ;
    }

    /* Antigrafoume ton antiproswpo sto katallhlo pedio tou kainourgiou komvou. */
    strcpy( curr->antiproswpos, representative );

    /* Kai auxanoume to plh8os twn lexewn kata 1. */
    curr->numberOfWords = 1;
    
    /* Desmeush xwrou gia thn lexh ( Word ). */
    curr->lexeis = ( Word ** ) malloc( sizeof( Word * ) );
    
    if (curr->lexeis == NULL) 
    {
      printf( "No memory!\n" );
      exit( 1 );
    }
    
    /* Katallhles arxikopoihseis gia thn kainourgia lista kai epistrofh ths kefalhs. */
    curr->lexeis[0] = lexh;
    
    curr->next = head;
    
    head = curr;
    
    return head;
}

/* Boh8htikh sunarthsh ,ousiastika einai ekeinh pou kanei tous elegxous gia tous gurous kai to plh8os twn grammatwn pou apomenoun,
* kai emfanizei ta katallhla mhnumata. */
void startGame( int guesses, Category *head )
{
    /* Dhlwsh kai arxikopoihsh tou pinaka xarakthrwn se null. */
    char usedLetters[CHARACTERS] = { };	//Xrhsimopoihmenoi xarakthres.
    char unusedLetters[CHARACTERS];	//Antistoixa.
    char letter;	//Boh8htikes metavlhtes.
    int i,win;
    int validLetter = 1;

    /* Arxikopoihsh twn grammatwn pou den xrhsimopoioume. */
    /* Xrhsh ths ka8olikhs metavlhths gia thn metatroph tou akeraiou se xarakthra.*/
    for ( i = 0; i < CHARACTERS; i++ )
        unusedLetters[i] = i + INTOLETTER;
    /* Ektupwsh grammatwn */
    for ( ; guesses > 0; guesses-- )
    {
        printf( "Remaining guesses: %d\n", guesses );
        printf( "Used letters: " );
        for ( i = 0; i < CHARACTERS; i++ )
        {
            if ( usedLetters[i] != 0 )
                printf( "%c ", usedLetters[ i ] );
            
        }
        
        printf( "\n" );
        printf( "Unused letters: " );
        for ( i = 0; i < CHARACTERS; i++ )
        {
            if ( unusedLetters[i] != 0 )
                printf( "%c ", unusedLetters[ i ] );
        }
        
        printf( "\n" );
        printf( "Word: " );
        printf( "%s\n", head->antiproswpos );
        printf( "Guess a letter: " );
        scanf( " %c", &letter );
        
	/* Elegxos gia to swsto input. */
        while ( !(  ( ( letter >= 'a' ) && ( letter <= 'z' ) ) || ( ( letter >= 'A' ) && ( letter <= 'Z' ) ) ) )
        {
            printf( "Guess a letter: " );
            scanf( " %c", &letter );
            
        }
        
        /* Metatroph kefalaiou se mikro. */
        letter = tolower( letter );
        
	/* Elegxos epanalhpshs se ka8e guro. */
        for ( i = 0; i < CHARACTERS; i++ )
        {
            if ( usedLetters[ i ] == letter )
            {
                printf( "You have already guessed this letter\n" );
                validLetter = 0;
                break;
            }
        }
        
        if ( validLetter == 0 )
            guesses++;
        else
        {
	    /* Dhmiourgoume thn lista me ton katallhlo antiproswpo. */
            usedLetters[ letter - INTOLETTER ] = letter;
            unusedLetters[ letter - INTOLETTER ] = 0;
            
            head = createCategoryList( letter, head );
                 
	    /* Epistrofh komvou pou periexei ton antproswpo me tis perissoteres lexeis. */
            head = findCategoryWithMostWords( head );
	    
            /* Elegxos gia na doume an exei kerdisei o paikths.Prepei o antiproswpos na einai idios me head->lexeis
	    *ama einai kerdisei o paikths o antiproswpos den 8a exei paules kai 8a einai h swsth lexh. */
            if ( strcmp( head->antiproswpos, head->lexeis[0]->string ) == 0 )
	    {
		win=1;
                break;
	    }
        }
        
        printf( "\n\n" );
        validLetter = 1;
        
    }
    
    /* Emfanish katallhlwn mhnumatwn. */
    if ( win==1 )
        printf( "YOU WON! The word is %s\n", head->antiproswpos );
    else
        printf( "YOU LOST! The word was %s\n", head->lexeis[0]->string );
        
    /* Klhsh sunarhshs apodesmeushs dunamikhs mnhmhs. */
    deleteCategoryNode( head );
}

/* Anixneush tou komvou me tis perissoteres lexeis kai epistrefei thn kefalh. */
Category *findCategoryWithMostWords( Category *head )
{
    Category *curr, *maxNode;
    Category *prev = NULL;
    int max = 0;
    
    /* Prospelash ths listas kai euresh tou megaluterou komvou. */
    for ( curr = head; curr != NULL; curr = curr->next )
    {
        if ( curr->numberOfWords > max )
        {
            max = curr->numberOfWords;
            maxNode = curr;
        }
        
    }

    /* Emfanish mhnumatos plh8ous twn lexewn se authn thn kathgoria. */
    printf( "Category size: %d\n", maxNode->numberOfWords );
    
    /* Epeidh den 8a xrhsimopoihsoume olous tous komvous ,diagrafoume autous pou den 8a xreiastoume. */
    curr = head;
    
    while ( curr != NULL )
    {
        if ( curr != maxNode )
        {
	    /* Katallhles meta8eseis gia to swsto delete komvwn. */
            if ( curr == head )
            {
                head = head->next;
		/* Klhsh sunarthshs. */
                deleteCategoryNode( curr );
                curr = head;
                continue;
            }
            else if ( curr->next == NULL )
            {
                prev->next = NULL;
                /* Klhsh sunarthshs. */
                deleteCategoryNode( curr );
                break;
            }
            else
            {
                prev->next = curr->next;
		/* Klhsh sunarthshs. */
                deleteCategoryNode( curr );
                curr = prev->next;
                continue;
            }
        }
        
        prev = curr;
        curr = curr->next;
    }

    return head;
}

/* Apodesmeush ths dunamika desmeumenhs mnhmhs ,gia tis klaseis isodunamias. */
void deleteCategoryNode( Category *Node )
{
    /* Prwta ta pedia ,meta olh thn lista. */
    free( Node->antiproswpos );
    free( Node->lexeis);
    free( Node );
}

/* Apodesmeush ths dunamika desmeumenhs mnhmhs ,gia tis lista twn lexew tou lexikou. */
void deleteWordList( Word *head )
{
    Word *curr, *temp;
    
    curr = head;
    
    /* Oso h lista einai gemath ,adeiaze. */
    while ( curr != NULL )
    {
        temp = curr->next;
        free( curr->string );
        free( curr );
        curr = temp;
    }
    
    head = NULL;
    
}



