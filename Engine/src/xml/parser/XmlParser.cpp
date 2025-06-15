#include "ParserState.h"
#include "XmlParser.h"
#include "../elements/SourcePosition.h"
#include "../elements/XmlComment.h"
#include "../elements/XmlDeclaration.h"
#include "../elements/XmlDocument.h"
#include "../elements/XmlElement.h"
#include "../elements/XmlProcessingInstruction.h"
#include "../elements/XmlText.h"
#include "../error/XmlExceptionMacros.h"
#include "../../util/Macros.h"
#include <corecrt.h>
#include <cctype>
#include <fstream>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace pkmn::xml
{
    bool XmlParser::s_initialized;
    std::unordered_set<wchar_t> XmlParser::s_whitespaces;

    XmlDocument XmlParser::parse_file(const std::wstring& path)
    {
        return XmlParser(path).read_xml_doc();
    }

    void XmlParser::init()
    {
        if (s_initialized)
            return;

        // init whitespace character set
        s_whitespaces.insert(L' ');
        s_whitespaces.insert(L'\t');
        s_whitespaces.insert(L'\n');
        s_whitespaces.insert(L'\r');

        s_initialized = true;
    }

    XmlParser::XmlParser(const std::wstring& path) noexcept
        : m_src_pos({ path, 0u, 0u }), m_state(ParserState::e_null)
    {
        XmlParser::init();
    }

    void XmlParser::set_state(ParserState next_state) noexcept
    {
        m_state = next_state;
    }

    void XmlParser::push_state(ParserState next_state)
    {
        m_previous_states.push(m_state);
        m_state = next_state;
    }

    void XmlParser::pop_state() noexcept
    {
        m_state = ParserState::e_null;
        if (!m_previous_states.empty()) {
            m_state = m_previous_states.top();
            m_previous_states.pop();
        }
    }

    wchar_t XmlParser::wpeek(bool dont_throw_weof)
    {
        wint_t win = m_wif.eof() ? WEOF : m_wif.peek();
        if (!dont_throw_weof && win == WEOF) {
            m_wif.close();
            throw XMLEOFERR(m_src_pos);
        }
        wchar_t wc = (wchar_t)win;
        return wc;
    }

    wchar_t XmlParser::wread(bool dont_throw_weof)
    {
        bool streamend = m_wif.eof();
        wint_t win = streamend ? WEOF : m_wif.get();
        if (!dont_throw_weof && win == WEOF) {
            m_wif.close();
            throw XMLEOFERR(m_src_pos);
        }

        wchar_t wc = (wchar_t)win;

        // update source position
        if (wc == L'\n') {
            m_line_lengths.push_back(m_src_pos.m_col - 1u);
            ++m_src_pos.m_line;
            m_src_pos.m_col = 1u;
        }
        else if (!streamend)
            ++m_src_pos.m_col;

        return wc;
    }

    void XmlParser::read_whitespace(bool dont_throw_weof)
    {
        wchar_t wc;
        while (true) {
            wc = wpeek(dont_throw_weof);
            if (s_whitespaces.find(wc) == s_whitespaces.end())
                break;
            else
                wread(dont_throw_weof);
        }
    }

    std::wstring XmlParser::read_identifier(const std::unordered_set<wchar_t>& special_delims)
    {
        std::wstring buf;
        wchar_t wc;
        while (true) {
            wc = wpeek();
            if (s_whitespaces.find(wc) != s_whitespaces.end()
                || special_delims.find(wc) != special_delims.end())
            {
                // stop reading, if whitespace or other given delimiters were reached
                if (buf.empty())
                    throw XMLCHARSEQERR(m_src_pos, L"A valid xml name", std::wstring(1u, wc));
                return buf;
            }

            // add character to buffer and extract it from stream
            buf.push_back(wc);
            wread();
        }
    }

    XmlAttribute XmlParser::read_attribute(const std::optional<std::wstring>& expected_attr_name)
    {
        read_whitespace();

        // extract attribute identifier from stream
        SourcePosition pos = m_src_pos;
        std::unordered_set<wchar_t> delims;
        delims.insert(L'=');
        std::wstring name = read_identifier(delims);
        if (expected_attr_name && name != *expected_attr_name) {
            m_wif.close();
            throw XMLCHARSEQERR(pos, L"Attribute name " + *expected_attr_name, name);
        }

        // extract whitespace and '=' from stream
        read_whitespace();
        pos = m_src_pos;
        wchar_t wc = wread();
        if (wc != L'=') {
            m_wif.close();
            throw XMLCHARSEQERR(pos, L"=", std::wstring(1u, wc));
        }
        read_whitespace();

        // get opening single or double quote from stream
        bool double_quoted;
        pos = m_src_pos;
        wc = wread();
        if (wc == L'\'')
            double_quoted = false;
        else if (wc == L'\"')
            double_quoted = true;
        else {
            m_wif.close();
            throw XMLCHARSEQERR(pos, L"\' or \"", std::wstring(1u, wc));
        }

        // extract attribute value from stream
        std::wstring value;
        while (true)
        {
            wc = wread();

            if (s_whitespaces.find(wc) != s_whitespaces.end()) {
                m_wif.close();
                throw XMLPARSEERR(pos, L"Attribute values must not contain whitespace");
            }

            if ((double_quoted && wc == L'\"') || (!double_quoted && wc == L'\''))
                break;
            else
                value.push_back(wc);
        }

        return XmlAttribute(std::move(name), std::move(value));
    }

    XmlDeclaration XmlParser::read_xml_decl(SourcePosition decl_pos)
    {
        // check for closing of xml declaration tag
        const auto check_end = [&]()
        {
            read_whitespace();
            wchar_t wc = wpeek();
            if (wc == L'?')
            {
                wread();
                SourcePosition posx = m_src_pos;
                wc = wread();
                if (wc == L'>')
                    return true;
                else {
                    m_wif.close();
                    throw XMLCHARSEQERR(posx, L">", std::wstring(1u, wc));
                }
            }
            return false;
        };

        // check for premature closing of xml declaration tag
        if (check_end()) {
            m_wif.close();
            throw XMLPARSEERR(m_src_pos, L"The XML declaration is missing a version attribute");
        }

        std::wstring version = read_attribute(L"version").m_value;

        if (check_end())
            return XmlDeclaration(decl_pos, std::move(version));

        read_whitespace();
        SourcePosition pos = m_src_pos;
        XmlAttribute attr = read_attribute();
        bool has_encoding = false;
        std::wstring encoding;
        std::optional<bool> standalone = { };

        if (attr.m_name == L"encoding") {
            encoding = attr.m_value;
            has_encoding = true;
        }
        else if (attr.m_name == L"standalone")
        {
            if (attr.m_value == L"yes")
                standalone = true;
            else if (attr.m_value == L"no")
                standalone = false;
            else {
                pos = m_src_pos;
                pos.m_col -= attr.m_value.length() + 1;
                m_wif.close();
                throw XMLCHARSEQERR(pos, L"yes or no", attr.m_value);
            }
        }
        else {
            m_wif.close();
            throw XMLCHARSEQERR(pos, L"encoding or standalone", attr.m_name);
        }

        pos = m_src_pos;
        if (check_end()) {
            if (has_encoding)
                return XmlDeclaration(decl_pos, std::move(version), std::move(encoding));
            else
                return XmlDeclaration(decl_pos, std::move(version), *standalone);
        }
        else if (standalone) {
            m_wif.close();
            throw XMLPARSEERR(pos, L"The standalone attribute must come last in a xml declaration");
        }

        attr = read_attribute(L"standalone");
        if (attr.m_value == L"yes")
            standalone = true;
        else if (attr.m_value == L"no")
            standalone = false;
        else {
            pos = m_src_pos;
            pos.m_col -= attr.m_value.length() + 1;
            m_wif.close();
            throw XMLCHARSEQERR(pos, L"yes or no", attr.m_value);
        }

        if (!check_end()) {
            m_wif.close();
            throw XMLPARSEERR(pos, L"A xml declaration must not have attributes other than version, encoding standalone");
        }

        return XmlDeclaration(decl_pos, std::move(version), std::move(encoding), *standalone);
    }

    std::optional<std::wstring> XmlParser::read_pi_data()
    {
        std::wstring buf;
        SourcePosition pos = m_src_pos;

        wchar_t wc = wpeek();
        bool has_whitespace_after_target = false;
        if (s_whitespaces.find(wc) != s_whitespaces.end()) {
            has_whitespace_after_target = true;
            read_whitespace();
        }

        while (true)
        {
            wc = wread();
            if (wc != L'?')
                buf.push_back(wc);
            else {
                wc = wread();
                if (wc == L'>') {
                    // end of processing instruction
                    // --> if instruction target is followed by whitespace but no data is given, throw exception
                    if (buf.length() != 0u)
                        return buf;
                    else if (has_whitespace_after_target) {
                        m_wif.close();
                        throw XMLPARSEERR(pos, L"A processing instruction with no data must be closed "
                            "immediately after the target (no whitespace allowed)");
                    }
                    else
                        return { };
                }
                else {
                    buf.push_back(L'?');
                    buf.push_back(wc);
                }
            }
        }
    }

    std::wstring XmlParser::read_comment()
    {
        // expects '<!--' to be the last characters extracted from the input stream,
        // so that the next character in the stream is the first character of the comment
        std::wstring buf;
        wchar_t wc;
        SourcePosition pos;

        while (true)
        {
            pos = m_src_pos;
            wc = wread();
            buf.push_back(wc);

            // a xml comment must not contain two consecutive hyphens
            // i.e. if we encounter two consecutive hyphens we have either reached
            // the end of the comment or we throw an exception
            if (wc == L'-') {
                wc = wread();
                if (wc != L'-')
                    buf.push_back(wc);
                else {
                    wc = wread();
                    if (wc == L'>')
                        return buf.substr(0u, buf.length() - 1u);
                    else {
                        m_wif.close();
                        throw XMLPARSEERR(pos, L"A comment must not contain two consecutive hyphens "
                            "except to close the comment");
                    }
                }
            }
        }
    }

    XmlDocument XmlParser::read_xml_doc()
    {
        if (m_wif.is_open())
            m_wif.close();

        // set exception mask for stream and open xml file for reading
        m_wif.exceptions(m_wif.badbit | m_wif.failbit);
        m_wif.open(m_src_pos.m_file);
        m_src_pos.m_line = 1u;
        m_src_pos.m_col = 1u;

        wchar_t wc;
        SourcePosition pos;
        while (true)
        {
            switch (m_state)
            {
            case pkmn::xml::ParserState::e_null:
                // save position for possible exception
                pos = m_src_pos;
                wc = wread(true);

                if (wc == L'<') {
                    // reached markup opening --> update parser state
                    push_state(ParserState::e_ang_o);
                }
                else if (wc == (wchar_t)WEOF) {
                    // reached end of file --> update parser state
                    set_state(ParserState::e_eof);
                }
                else if (s_whitespaces.find(wc) == s_whitespaces.end()) {
                    m_wif.close();
                    throw XMLCHARSEQERR(pos, L"\'<\' or whitespace", std::wstring(1u, wc));
                }
                else
                    read_whitespace(true);

                break;
            case pkmn::xml::ParserState::e_ang_o:
            {
                wc = wpeek();

                if (wc == L'?')
                {
                    // xml declaration or processing instruction
                    // --> extract '?' from stream and update parser state
                    // (delay decision whether a xml declaration or a processing instruction
                    // has been reached until next loop pass)
                    wread();
                    set_state(ParserState::e_ang_o_qm);
                }
                else if (wc == L'!')
                {
                    // comment or DTD (not supported yet)
                    // --> extract '!' from stream and update parser state
                    // (delay decision whether a xml comment or a DTD has been reached until next loop pass)
                    wread();
                    set_state(ParserState::e_ang_o_em);
                }
                else if (wc == L'/')
                {
                    // end tag (only allowed if we have open start tags on the stack)
                    pos = m_src_pos;
                    --pos.m_col;
                    if (m_open_elements.empty()) {
                        m_wif.close();
                        throw XMLCHARSEQERR(pos, L"A start tag or an empty element tag", L"An end tag");
                    }

                    // extract '/' and end tag identifier from stream
                    wread();
                    std::unordered_set<wchar_t> delims;
                    delims.insert(L'>');
                    pos = m_src_pos; // save position for possible exception
                    std::wstring tag = read_identifier(delims);

                    // match end tag identifier against open start tag
                    const std::wstring match = m_open_elements.top()->get_name();
                    if (tag != match) {
                        m_wif.close();
                        throw XMLCHARSEQERR(pos, match, tag);
                    }

                    // extract whitespace and closing angular bracket from stream
                    read_whitespace();
                    pos = m_src_pos; // save position for possible exception
                    wc = wread();
                    if (wc != L'>') {
                        m_wif.close();
                        throw XMLCHARSEQERR(pos, L">", std::wstring(1u, wc));
                    }

                    // pop element from stack and return parser to previous state
                    // (need to pop two states from stack: one for the closed element
                    // and one that was pushed when the end tag was reached)
                    m_open_elements.pop();
                    pop_state();
                    pop_state();
                }
                else
                {
                    // start tag or empty element tag
                    // --> extract identifier from stream
                    std::unordered_set<wchar_t> delims;
                    delims.insert(L'>');
                    delims.insert(L'/');
                    std::wstring tag = read_identifier(delims);

                    // create XmlElement object and add it to document structure
                    pos = m_src_pos;
                    --pos.m_col;
                    std::shared_ptr<XmlElement> parent = m_open_elements.empty() ? nullptr : m_open_elements.top();
                    std::shared_ptr<XmlElement> element = std::make_shared<XmlElement>(pos, std::move(tag), parent);
                    if (parent != nullptr)
                        parent->add_child(element);
                    else if (m_doc.get_root() == nullptr)
                        m_doc.set_root(element);
                    else {
                        m_wif.close();
                        throw XMLPARSEERR(pos, L"Encountered a second root element. "
                            "Every XML document must have exactly one root element.");
                    }

                    // push element to stack and update parser state
                    m_open_elements.push(element);
                    set_state(ParserState::e_tag);
                }

                break;
            }
            case pkmn::xml::ParserState::e_ang_o_qm:
            {
                // --> decide between xml declaration or processing instruction
                std::unordered_set<wchar_t> delims;
                delims.insert(L'?');
                std::wstring target = read_identifier(delims);
                pos = m_src_pos;
                pos.m_col -= 2u;

                if (target == L"xml")
                {
                    // xml declaration
                    if (m_doc.get_xml_declaration()) {
                        m_wif.close();
                        throw XMLPARSEERR(pos, L"Encountered a second xml declaration. "
                            "Every XML document must not have more than one xml declarations.");
                    }
                    else {
                        pop_state();
                        if (m_state == ParserState::e_null
                            && m_doc.get_pre_root_misc_stuff().empty()
                            && m_doc.get_root() == nullptr
                            && m_doc.get_post_root_misc_stuff().empty()
                            && m_open_elements.empty())
                        {
                            m_doc.set_xml_decl(read_xml_decl(pos));
                        }
                        else {
                            m_wif.close();
                            throw XMLPARSEERR(pos, L"The XML declaration, if there is one, "
                                "must be at the very beginning of the XML document");
                        }
                    }
                }
                else
                {
                    // xml processing instruction
                    std::shared_ptr<XmlElement> parent = m_open_elements.empty() ? nullptr : m_open_elements.top();
                    std::shared_ptr<XmlProcessingInstruction> element =
                        std::make_shared<XmlProcessingInstruction>(pos, std::move(target), parent, read_pi_data());

                    if (parent != nullptr)
                        parent->add_child(element);
                    else if (m_doc.get_root() == nullptr)
                        m_doc.add_misc_stuff_pre_root(element);
                    else
                        m_doc.add_misc_stuff_post_root(element);

                    pop_state();
                }

                break;
            }
            case pkmn::xml::ParserState::e_ang_o_em:
            {
                // --> decide between xml comment and DTD (not supported yet)
                pos = m_src_pos;
                pos.m_col -= 2u;
                std::wstring buf(L"<!");

                wc = wpeek();
                if (wc == L'-')
                {
                    // xml comment
                    buf.push_back(wread());
                    buf.push_back(wread());
                    std::wstring expected = L"<!--";
                    if (buf != expected) {
                        m_wif.close();
                        throw XMLCHARSEQERR(pos, expected, buf);
                    }

                    std::shared_ptr<XmlElement> parent = m_open_elements.empty() ? nullptr : m_open_elements.top();
                    std::shared_ptr<XmlComment> comment = std::make_shared<XmlComment>(pos, read_comment(), parent);

                    if (parent != nullptr)
                        parent->add_child(comment);
                    else if (m_doc.get_root() == nullptr)
                        m_doc.add_misc_stuff_pre_root(comment);
                    else
                        m_doc.add_misc_stuff_post_root(comment);

                    pop_state();
                }
                else
                {
                    // TODO implement DTD stuff
                    throw NOTSUPPORTED(L"XML parsing of DTD stuff");
                }

                break;
            }
            case pkmn::xml::ParserState::e_tag:
            {
                wc = wpeek();
                if (s_whitespaces.find(wc) != s_whitespaces.end())
                    read_whitespace();

                pos = m_src_pos;
                wc = wpeek();
                if (wc == L'>') {
                    // end of start tag --> update parser state
                    wread();
                    set_state(ParserState::e_elem);
                }
                else if (wc == L'/')
                {
                    wread();
                    wc = wread();
                    if (wc == L'>') {
                        // end of empty element tag --> pop element from stack and update parser state
                        m_open_elements.pop();
                        pop_state();
                    }
                    else {
                        m_wif.close();
                        throw XMLCHARSEQERR(pos, L"/>", std::wstring(L"/") + std::wstring(1u, wc));
                    }
                }
                else
                {
                    // xml attribute
                    m_open_elements.top()->add_attribute(read_attribute());
                }
                break;
            }
            case pkmn::xml::ParserState::e_elem:
            {
                wc = wpeek();
                if (s_whitespaces.find(wc) != s_whitespaces.end())
                    read_whitespace();

                std::wstring buf;
                pos = m_src_pos;
                while (true)
                {
                    wc = wread();
                    if (wc != L'<')
                        buf.push_back(wc);
                    else
                    {
                        // reached next markup opening
                        // --> if buf is not empty, add textdata to current element and update parser state
                        if (buf.length() > 0u) {
                            std::shared_ptr<XmlElement> parent = m_open_elements.top();
                            std::shared_ptr<XmlText> text = std::make_shared<XmlText>(pos, std::move(buf), parent);
                            parent->add_child(text);
                        }
                        push_state(ParserState::e_ang_o);
                        break;
                    }
                }
                break;
            }
            case pkmn::xml::ParserState::e_eof:
                m_wif.close();
                // throw exception if there is no root element
                if (m_doc.get_root() == nullptr) {
                    throw XMLPARSEERR(m_src_pos, L"Found no root element while parsing the file. "
                        "Every XML document must have exactly one root element.");
                }
                return m_doc;
            default:
                m_wif.close();
                throw XMLILLSTATEERR(m_src_pos, m_state);
            }
        }
    }
}
