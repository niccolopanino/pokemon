#pragma once
#include "IllegalParserStateException.h"
#include "UnexpectedCharSequenceException.h"
#include "UnexpectedEofException.h"
#include "XmlParserException.h"
#include <corecrt.h>

#define XMLPARSEERR(srcpos) pkmn::xml::XmlParserException(__LINE__, __FILEW__, srcpos)
#define XMLPARSEERRNOTE(srcpos, note) \
    pkmn::xml::XmlParserException(__LINE__, __FILEW__, srcpos, note)
#define XMLCHARSEQERR(srcpos, expected, actual) \
    pkmn::xml::UnexpectedCharSequenceException(__LINE__, __FILEW__, srcpos, expected, actual)
#define XMLEOFERR(srcpos) pkmn::xml::UnexpectedEofException(__LINE__, __FILEW__, srcpos)
#define XMLILLSTATEERR(srcpos, state) \
    pkmn::xml::IllegalParserStateException(__LINE__, __FILEW__, srcpos, state)
