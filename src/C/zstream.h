/* Possible tags of the elements
 * in a z-value stream. In naming
 * and documentation, EMPTY,
 * NEWLINE and END are called
 * 'non-solution tags'. */ 
enum zstreamtag {
  EMPTY   = 01, /* no solution or to be
                   initialized later */
  SOL     = 02, /* solution exists */
  NEWLINE = 04, /* newline when stream converted
                   to string and printed */
  END     = 010 /* end of stream */
};

/* z-value stream element */
struct zstreamelem {
  enum zstreamtag tag;
  union {
    double height; /*      for SOL */
    char   c;      /* ' '  for EMPTY,
                      '\n' for NEWLINE,
                      '\0' for END */
  } content;
};

/* Return char corresponding to input
 * non-solution zstreamtag. SOL as
 * input is invalid and causes the
 * char corresponding to EMPTY to be
 * returned.*/
static char tagtochar(enum zstreamtag tag) 
{
  switch (tag) {
    case EMPTY:   return ' ';
    case NEWLINE: return '\n';
    case END:     return '\0';
    default:      return ' '; /* invalid */
  }
}

/* Generates z-value stream element elem with
 * elem.tag=tag (input) and elem.content.c 
 * given by chartotag(tag). 
 *
 * DON'T USE FOR GENERATING zstreamelem
 * WITH .tag=SOL ! If input tag=SOL, one
 * gets zstreamelem with .tag=SOL and
 * .content.c=' ' which is an invalid
 * combination. */
static struct zstreamelem tagtononsol(enum zstreamtag tag) 
{
  struct zstreamelem elem;

  elem.tag = tag;
  elem.content.c = chartotag(tag);
  return elem;
}

/* Points to the beginning of an array
 * where each element has type
 *   struct zstreamelem
 * and where the last element is of
 * the form
 *   { END, { '\0' } }. */
typedef struct zstreamelem *Zstreamptr;

/* Initializes the input array zstream
 * with znummax amount of elements to 
 * have the last element in form
 *   { END, { '\0' } }.
 * 
 * RETURN: pointer Zstreamptr to the
 * first element in the stream.
 *
 * NOTE: THIS IS THE ONLY ALLOWED WAY 
 * TO MAKE A POINTER Zstreamptr
 * (in addition to assigning a
 * Zstreamptr pointer to another
 * Zstreamptr). This is because all
 * uses of Zstreamptr assume that it 
 * points to an element in an array 
 * that ends with
 *   { END, { '\0' } }. */
static Zstreamptr zstreaminit(struct zstreamelem zstream[],
    int znummax) 
{
  zstream[--znummax] = tagtononsol(END);
  return &zstream[0];
}
