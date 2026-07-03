/* possible tags of the elements
 * in a z-value stream */ 
enum zstreamtag {
  SOL     = 01, /* solution exists */
  NOSOL   = 02, /* solution does not exist */
  NEWLINE = 04, /* newline when stream converted
                   to string and printed */
  END     = 010 /* end of stream */
};

/* z-value stream element */
struct zstreamelem {
  enum zstreamtag tag;
  union {
    double height; /*      for SOL */
    char   c;      /* ' '  for NOSOL,
                      '\n' for NEWLINE,
                      '\0' for END
                      'e'  for invalid
                      (e.g. trying to
                      combine tag=SOL
                      with content.c) */
  } content;
};

/* return char corresponding to input
 * zstreamtag */
static char tagtochar(enum zstreamtag tag) {
  switch (tag) {
    case NOSOL:   return ' ';
    case NEWLINE: return '\n';
    case END:     return '\0';
    default:      return 'e'; /* invalid */
  }
}

/* Generates z-value stream element elem with
 * elem.tag=tag and elem.content.c given by
 * chartotag(tag). DON'T USE FOR GENERATING
 * ONE WITH .tag=SOL! If tag=SOL, one gets 
 * a zstreamelem with .content.c='e' which
 * signals invalid zstreamelem. */
static struct zstreamelem tagtoelem(enum zstreamtag tag) {
  struct zstreamelem elem;

  elem.tag = tag;
  elem.content.c = chartotag(tag);
  return elem;
}
