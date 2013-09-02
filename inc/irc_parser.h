#ifndef IRC_PARSER_H
#define IRC_PARSER_H

typedef struct irc_parser irc_parser_t;

irc_parser_t*
new_irc_parser();

void
free_irc_parser(irc_parser_t*);

int
irc_parser_feed(irc_parser_t*, const char* buff, size_t len);

#endif
