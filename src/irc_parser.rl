#include <stdlib.h>
#include "irc_parser.h"

%%{

machine irc_parser;

space_ = " "+;

nonwhite = /./ - (" " | "\0" | "\r" | "\n");

chstring = /./ - (" " | "\b" | "\0" | "\r" | "\n" | ",");

channel = ("#" | "&") chstring;

user = nonwhite+;

host = "."+;

letter = [a-zA-Z];

number = [0-9];

special = "-" | "[" | "]" | "\\" | "`" | "^" | "{" | "}";

nick = letter (letter | number | special)*;

mask = ("#" | "$") chstring;

to_ = channel | user "@" host | nick | mask;

target = to_ ("," to_)*;

prefix = ":" (host | nick ("!" user)? ("@" host)?) space_;

command = letter+ | number number number;

middle = (nonwhite - ":") nonwhite*;

trailing = /./ - ("\0" | "\r" | "\n");

params = space_ (middle space_)* (":" trailing)?;

message = prefix? command params "\r\n";

main := message*;

}%%

%% write data;

struct irc_parser {
    int cs;
};

%% variable cs parser->cs;

irc_parser_t*
new_irc_parser()
{
    irc_parser_t* parser = malloc(sizeof(*parser));

    %% write init;

    return parser;
}

void
free_irc_parser(irc_parser_t* parser)
{
    free(parser);
}

int
irc_parser_feed(irc_parser_t* parser, const char* buff, size_t len)
{
    const char* p = buff;
    const char* pe = buff + len;

    %% write exec;

    if(parser->cs == irc_parser_error) {
        return -1;
    }

    return 0;
}
