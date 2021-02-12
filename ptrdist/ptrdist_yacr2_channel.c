/*
 *
 * channel.c
 *
 */


#define CHANNEL_CODE


/*
 *
 * Includes.
 *
 */

#include <stdlib.h>
#include "types.h"
#include "channel.h"

/* inlined file content */
/*****************************************************************************/
struct RowDataTy {
  ulong col;
  ulong bot;
  ulong top;
};

static const struct RowDataTy input2[] = {
{1,0,0},
{2,0,0},
{3,0,0},
{4,0,0},
{5,21,18},
{6,12,53},
{7,40,9},
{8,50,24},
{9,55,50},
{10,7,59},
{11,32,35},
{12,23,30},
{13,15,28},
{14,33,2},
{15,23,55},
{16,14,11},
{17,36,18},
{18,34,22},
{19,20,53},
{20,53,56},
{21,17,21},
{22,0,0},
{23,46,6},
{24,19,33},
{25,31,9},
{26,53,35},
{27,17,46},
{28,19,47},
{29,51,33},
{30,19,14},
{31,29,41},
{32,23,41},
{33,1,48},
{34,39,51},
{35,18,25},
{36,47,42},
{37,2,1},
{38,24,2},
{39,30,9},
{40,37,55},
{41,12,44},
{42,29,46},
{43,39,28},
{44,58,40},
{45,38,59},
{46,4,45},
{47,43,16},
{48,31,17},
{49,7,50},
{50,16,15},
{51,0,0},
{52,32,6},
{53,0,0},
{54,32,44},
{55,0,0},
{56,20,45},
{57,0,0},
{58,10,39},
{59,0,0},
{60,60,43},
{61,0,0},
{62,27,4},
{63,0,0},
{64,1,58},
{65,0,0},
{66,56,2},
{67,0,0},
{68, 3,46},
{69,0,0},
{70,22,13},
{71,1,26},
{72,22,40},
{73,2,6},
{74,57,15},
{75,45,59},
{76,21,25},
{77,28,9},
{78, 5,37},
{79,50,23},
{80,26,8},
{81,14,14},
{82,3,24},
{83,16,31},
{84,46,6},
{85,52,7},
{86,59,9},
{87,42,3},
{88,8,4},
{89,0,11},
{90,27,24},
{91,25,10},
{92,5,58},
{93,60,57},
{94,43,13},
{95,49,5},
{96,0,42},
{97,49,34},
{98,0,27},
{99,10,60},
{100,47,0},
{101,60,54},
{102,10,30},
{103,13,43},
{104,34,25},
{105,36,48},
{106,49,16},
{107,38,0},
{108,0,0},
{109,32,58},
{110,0,42},
{111,42,5},
{112,54,52},
{113,0,27},
{114,0,58},
{115,0,0}
};

static unsigned consumedCount = 0;

void inline_fopen(void) {
  consumedCount = 0;
}

//stat = inline_fscanf(&col, &bot, &top);
int inline_fscanf(ulong* colptr, ulong* botptr, ulong* topptr)
{
  if (consumedCount < (sizeof(input2)/sizeof(input2[0]))) {
    *colptr = input2[consumedCount].col;
    *botptr = input2[consumedCount].bot;
    *topptr = input2[consumedCount].top;
    consumedCount+=1;
    return 3;
  }
  return -1;
}

/*****************************************************************************/

/*
 *
 * Code.
 *
 */

void
BuildChannel(void)
{
    /*
     * What is the length dimension of the channel?
     * That is, how many columns in the channel?
     */
    DimensionChannel();

    /*
     * What are the particulargs of the channel?
     * That is, what are the nets and pin locations?
     */
    DescribeChannel();

    /*
     * What is the maximum density of the channel?
     * That is, what is the lower bound on the number of tracks
     * required to route the channel?
     */
    DensityChannel();
}

void
DimensionChannel(void)
{
    ulong	line;
    ulong	dim;
    ulong	net;
    ulong	col;
    ulong	bot;
    ulong	top;
    long	stat;

    /*
     * Open channel description file.
     */
    inline_fopen();
    
    /*
     * Scan the file to find the last column
     * number.  The channel file description
     * contains non-negative integers in the
     * format...
     *
     * [column #] [bottom net #] [top net #]
     */
    line = 0;
    dim = 0;
    net = 0;
    do {
	line++;
	stat = inline_fscanf(&col, &bot, &top);
	if (stat != -1) {
	    if (stat == 3) {
		/*
		 * Update column #.
		 */
		if (col > dim) {
		    dim = col;
		}

		/*
		 * Determine how many channel nets.
		 */
		if (bot > net) {
		    net = bot;
		}
		if (top > net) {
		    net = top;
		}
	    }
	    else {
		/*
		 * Error in channel file description.
		 */
		printf("Error:\n");
		printf("\tChannel file description invalid at line %d.\n", line);
		printf("\tIncorrect number of specifiers.\n");
		exit(1);
	    }
	}
    } while (stat != -1);

    /*
     * Check channel dimension.
     */
    if (dim == 0) {
	/*
	 * Error in channel file description.
	 */
	printf("Error:\n");
	printf("\tChannel description invalid.\n");
	printf("\tChannel has null dimension.\n");
	exit(1);
    }

    /*
     * Set global channel info.
     */
    channelColumns = dim;
    channelNets = net;
}

void
DescribeChannel(void)
{
    ulong	line;
    ulong	col;
    ulong	bot;
    ulong	top;
    long	stat;

    /*
     * Top terminals of channel.
     */
    TOP = (ulong *)malloc((channelColumns+1) * sizeof(ulong));

    /*
     * Bottom terminals of channel.
     */
    BOT = (ulong *)malloc((channelColumns+1) * sizeof(ulong));

    /*
     * Initialize terminals of channel.
     */
    for (col = 0; col <= channelColumns; col++) {
	TOP[col] = 0;
	BOT[col] = 0;
    }

    /*
     * Open channel description file.
     */
    inline_fopen();

    /*
     * Scan the file to find the last column
     * number.  The channel file description
     * contains non-negative integers in the
     * format...
     *
     * [column #] [bottom net #] [top net #]
     */
    line = 0;
    do {
	line++;
	stat = inline_fscanf(&col, &bot, &top);
	if (stat != -1) {
	    if (stat == 3) {
		/*
		 * Build column.
		 */
		if (col > channelColumns) {
		    /*
		     * Error in channel file description.
		     */
		    printf("Error:\n");
		    printf("\tChannel file description invalid at line %d.\n", line);
		    printf("\tColumn number out of range.\n");
		    exit(1);
		}
		else {
		    /*
		     * Doit.
		     */
		    BOT[col] = bot;
		    TOP[col] = top;
		}
	    }
	    else {
		/*
		 * Error in channel file description.
		 */
		printf("Error:\n");
		printf("\tChannel file description invalid at line %d.\n", line);
		printf("\tIncorrect number of specifiers.\n");
		exit(1);
	    }
	}
    } while (stat != -1);
}

void
DensityChannel(void)
{
    ulong	init;
    ulong	which;
    ulong 	col;
    ulong 	bound;
    ulong	boundColumn;

    /*
     * Allocate track dimension structures.
     */
    FIRST = (ulong *)malloc((channelNets+1) * sizeof(ulong));
    LAST = (ulong *)malloc((channelNets+1) * sizeof(ulong));
    DENSITY = (ulong *)malloc((channelColumns+1) * sizeof(ulong));
    CROSSING = (ulong *)malloc((channelNets+1) * sizeof(ulong));

    /*
     * Initialize track dimension structures.
     */
    for (init = 0; init <= channelNets; init++) {
	FIRST[init] = 0;
	LAST[init] = 0;
	CROSSING[init] = 0;
    }
    for (init = 0; init <= channelColumns; init++) {
	DENSITY[init] = 0;
    }

    /*
     * Compute the span for each net.
     */
    for (which = 1; which <= channelNets; which++) {
	/*
	 * Compute first.  This is the location of
	 * the first column for this net.
	 */
	for (col = 1; col <= channelColumns; col++) {
	    if ((BOT[col] == which) || (TOP[col] == which)) {
		FIRST[which] = col;
		break;
	    }
	}

	/*
	 * Compute last.  This is the location of
	 * the last column for this net.
	 */
	for (col = channelColumns; col >= 1; col--) {
	    if ((BOT[col] == which) || (TOP[col] == which)) {
		LAST[which] = col;
		break;
	    }
	}

	/*
	 * Increment the track for each column
	 * of the channel which this net spans.
	 */
	for (col = FIRST[which]; col <= LAST[which]; col++) {
	    DENSITY[col]++;
	}
    }

    /*
     * Compute lower bound on channel tracks.
     */
#ifdef SCOTT
    bound = 0;
    for (col = 1; col <= channelColumns; col++) {
	if (DENSITY[col] > bound) {
	    bound = DENSITY[col];
	    boundColumn = col;
	}
    }
#else
    bound = 0;
    for (col = channelColumns; col >= 1; col--) {
	if (DENSITY[col] > bound) {
	    bound = DENSITY[col];
	    boundColumn = col;
	}
    }
#endif

    /*
     * Set global channel info.
     */
    channelTracks = bound;			/* tracks available for routing */
    channelDensity = bound;			/* max channel density */
    channelDensityColumn = boundColumn;		/* column of max channel density */
}









